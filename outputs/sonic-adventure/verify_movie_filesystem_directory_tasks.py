"""Original command1, raw linked directory reads, completion and teardown."""
import hashlib,itertools,json,sys
from pathlib import Path
from movie_filesystem_directory_session import Session,v
P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
    for ch,order,endian,count in itertools.product((0,7),(0,1),(0,1),(0,1,2)):
        yield dict(channel=ch,order=order,endian=endian,mode=4 if (ch+order+endian+count)&1 else 0,count=count,kind='valid')
    for ch,kind in itertools.product((0,7),('early_end','count_limit','cycle','first_error','late_error','latch')):
        yield dict(channel=ch,order=ch&1,endian=ch&1,mode=4 if ch else 0,count=2,kind=kind)
def main():
    assert v.effects.DLL.name=='movie_filesystem_directory.dll'
    configs=list(configurations());smoke='--smoke' in sys.argv
    if smoke:configs=[next(c for c in configs if c['kind']==k and c['count']==2) for k in ('valid','cycle','first_error','latch')]
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=comparisons=0
    for config in configs:
        s=Session(ram,command=1,**{k:config[k] for k in ('channel','order','endian','mode')});done=0;case_checks=0;kind=config['kind'];count=config['count']
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(config,label);checks+=1;case_checks+=1
        def harvest():
            nonlocal done,comparisons
            for r,path in s.phases[done:]:
                require(r['status']==0,'host status');require(r['full_state_match'],'entire machine/devices');require(r['ordered_boundaries_match'],'ordered boundaries');comparisons+=1
            done=len(s.phases)
        def snap():
            sp=v.load(s.m,s.task+0x18);return sp,v.sample(s.m,sp,s.idle_sp+64-sp),v.sample(s.m,s.owner+0x210,32*0x124)
        def intact(before):
            sp,frame,contexts=before;require(v.sample(s.m,sp,len(frame))==frame,'saved stack');require(v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'32 contexts')
        def transfer(data=None,status=8):
            queued=s.tokens()[0];require(queued>0,'original request queued');before=snap()
            s.submit();harvest();intact(before);require(s.tokens()[:2]==[0,queued],'original inflight')
            s.observe(511,status,block_data=data,metadata_words={6:509,8:2,10:507,12:count} if data is None and status==8 else None);intact(before)
            s.pump();harvest();intact(before);require(s.tokens()[1:]==[0,queued],'original completed')
        harvest();s.queue();harvest();transfer();metadata=s.metadata_expected();s.resume();harvest()
        require([v.load(s.m,s.output+j*4) for j in range(19)]==metadata,'independent metadata decode')
        destinations=[v.load(s.m,s.descriptor+o) for o in (0x20,0x24,0x28)];root_dest,table_dest,directory_dest=destinations
        root=bytearray((j*37+s.channel*13)&255 for j in range(512));root[:16]=b'U'*16
        fat=bytearray(1024);links={507:0xfffa if kind=='early_end' else 506,506:507 if kind=='cycle' else 505 if kind=='count_limit' else 0xfffa}
        for index,value in links.items():fat[index*2:index*2+2]=value.to_bytes(2,'big' if s.endian else 'little')
        expected=[bytearray(v.sample(s.m,a,n)) for a,n in zip(destinations,(512,1024,1536))]
        plan=[(511,root_dest,bytes(root),0,0),(509,table_dest,bytes(fat[:512]),1,0),(510,table_dest+512,bytes(fat[512:]),1,512)]
        directory_blocks=[];block=507
        for i in range(count):
            directory_blocks.append(block);plan.append((block,directory_dest+i*512,bytes((j*13+i*31+s.channel)&255 for j in range(512)),2,i*512))
            block=links.get(block,0xfffa)
            if block==0xfffa:break
        error_index=3 if kind in ('first_error','latch') else 4 if kind=='late_error' else None
        if error_index is not None:plan=plan[:error_index+1]
        stats=[v.load(s.m,s.descriptor+o) for o in (0x30,0x34)];errors=0
        for op,(block,dest,raw,region,offset) in enumerate(plan):
            for part in range(1 if op==error_index else 2):
                before=snap();token=s.tokens()[0];serial=v.load(s.m,s.context+0xfe0)
                s.call('pending_directory_task',v.AI_SCHEDULE);harvest()
                require(s.tokens()[0]==token and v.load(s.m,s.context+0xfe0)==serial and snap()[0]==before[0],'pending duplicates no packet/frame')
                status=251 if kind=='latch' and op==error_index else 7 if op==error_index else 8
                payload=raw[part*256:(part+1)*256] if status==8 else None
                transfer(payload,status);s.resume();harvest()
                if status!=8:errors+=1
                else:expected[region][offset+part*256:offset+(part+1)*256]=payload
                require(all(v.sample(s.m,a,len(b))==b for a,b in zip(destinations,expected)),'raw buffers and untouched tails')
        calls=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']
        require(calls==[[s.channel,b,d] for b,d,*_ in plan],'linked block indices/buffer increments')
        require([v.load(s.m,s.descriptor+o) for o in (0x30,0x34)]==[stats[0]+len(plan),stats[1]+errors],'read/error counters')
        progress=[b for r,_ in s.phases for b in r['boundaries'] if b['pr']=='0x8c647694' and b['target']=='0x8c05180c']
        require(len(progress)==len(plan),'one progress callback per read')
        for i,event in enumerate(progress):require(event['arguments'][:4]==[s.channel,1,stats[0]+i,stats[1]+int(i==error_index)],'progress arguments')
        result=0xffffff01 if kind in ('first_error','late_error') else 0
        require(v.load(s.m,s.result_word)==result and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original completion result/state')
        callbacks=[b for r in s.records for b in r['callback_events']]
        require(len(callbacks)==1 and callbacks[0]['arguments'][:3]==[s.channel,1,result],'original completion callback')
        require(s.records[-1]['stop_pc']=='0x8cfed000','returned through original scheduler')
        require({0x8c64a9a8,0x8c64b246}<=s.seen,'original directory caller/return')
        if count:require(0x8c64a9e0 in s.seen,'async directory continuation')
        if count and kind!='first_error' and kind!='latch':require(0x8c64b1c0 in s.seen,'real table lookup')
        if kind=='latch':require(v.load(s.m,0x8c89cf90+s.channel*4)==1,'generic error latch')
        if kind=='valid':require(v.sample(s.m,s.descriptor+10,3)==bytes((1,1,1)),'original cache flags')
        s.call('completed_idle',v.AI_SCHEDULE);harvest();s.call('original_owner_cancel',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'owner teardown')
        seen.update(s.seen);edges.update(s.edges)
        records.append(dict(input=config,read_calls=calls,independent_checks=case_checks,phases=s.records,task_frames_never_repaired=True,completed_ids_never_injected=True))
        print('DIRECTORY TASK',len(records),config,'comparisons',comparisons,flush=True)
    if smoke:print('DIRECTORY SMOKE PASS',len(records),comparisons,checks);return
    coverage=[]
    for r in v.manifest['routines']:
        missing=set(r['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,r['conditional_edges']))-edges
        coverage.append(dict(address=r['address'],static_rows=r['instruction_rows'],executed_rows=r['instruction_rows']-len(missing),executed_outcomes=len(r['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
    files=[Path(__file__),P/'movie_filesystem_directory_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_directory_task_fixtures.py',Path(v.__file__),P/'write_watch_filesystem_directory.py',v.MP,P/'analysis/movie_filesystem_directory_connection.json',v.effects.DLL]
    report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_qualified_nonlocal_routes=['0x8c64a9e0','0x8c64b246'],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original command1 through root/table/linked directory/completion/idle/teardown. Raw physical response bytes only;no parsed results,task frames,request IDs orcallback results supplied. Counts0/1/2,earlyend,countlimit,cycleboundedbyoriginalcount,first/lateerror andgenericlatch. Independent buffers/untouchedtails/indices/counters/callbacks. Declared device input domain;not physical acquisition/fullfilesystem/gameplay.')
    (P/'analysis/movie_filesystem_directory_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('DIRECTORY TASK PASS',len(records),comparisons,checks,flush=True)
if __name__=='__main__':main()
