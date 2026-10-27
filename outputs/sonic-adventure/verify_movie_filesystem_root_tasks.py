"""Original root validation, positive table loops and real error-latch behavior."""
import hashlib,itertools,json,sys
from pathlib import Path
from movie_filesystem_root_session import Session,v
P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
    for ch,order,endian,count in itertools.product((0,7),(0,1),(0,1),(0,1,2)):
        yield dict(channel=ch,order=order,endian=endian,mode=4 if (ch+order+endian+count)&1 else 0,count=count,kind='valid')
    for ch,kind in itertools.product((0,7),('bad_first','bad_last','table_bad','table_latch','table_late_bad')):
        yield dict(channel=ch,order=ch&1,endian=ch&1,mode=4 if ch else 0,count=2,kind=kind)
def main():
    assert v.effects.DLL.name=='movie_filesystem_root.dll'
    configs=list(configurations());smoke='--smoke' in sys.argv
    if smoke:configs=[configs[0],next(c for c in configs if c['kind']=='valid' and c['count']==2)]+[next(c for c in configs if c['kind']==k) for k in ('bad_last','table_bad','table_latch')]
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=comparisons=0
    for config in configs:
        s=Session(ram,command=1,**{k:config[k] for k in ('channel','order','endian','mode')});done=0;case_checks=0;kind=config['kind'];count=config['count']
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(config,label);checks+=1;case_checks+=1
        def harvest(status=0):
            nonlocal done,comparisons
            for i in range(done,len(s.phases)):
                r,path=s.phases[i];wanted=status if i==len(s.phases)-1 else 0
                require(r['status']==wanted,'host status');require(r['full_state_match'],'entire machine/devices');require(r['ordered_boundaries_match'],'ordered boundaries');comparisons+=1
            done=len(s.phases)
        def snap():
            sp=v.load(s.m,s.task+0x18);return sp,v.sample(s.m,sp,s.idle_sp+64-sp),v.sample(s.m,s.owner+0x210,32*0x124)
        def intact(before):
            sp,frame,contexts=before;require(v.sample(s.m,sp,len(frame))==frame,'transport preserves saved stack');require(v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'transport preserves32contexts')
        def transfer(data=None,status=8):
            queued=s.tokens()[0];require(queued>0,'request originally queued');before=snap()
            s.submit();harvest();intact(before);require(s.tokens()[:2]==[0,queued],'original queued to inflight')
            s.observe(511,status,block_data=data,metadata_words={6:509,8:count} if data is None and status==8 else None);intact(before)
            s.pump();harvest();intact(before);require(s.tokens()[1:]==[0,queued],'original inflight to completed')
        harvest();s.queue();harvest();transfer();metadata=s.metadata_expected();s.resume();harvest()
        require([v.load(s.m,s.output+j*4) for j in range(19)]==metadata,'raw metadata independently decoded')
        require(v.load(s.m,s.output+0x10)==count and v.load(s.m,s.output+0xc)==509,'original table count/start fields')
        root_dest=v.load(s.m,s.descriptor+0x20);table_dest=v.load(s.m,s.descriptor+0x24)
        require(int.from_bytes(v.sample(s.m,s.descriptor+0x10,2),'little',signed=True)>=count,'original allocated table capacity')
        root=bytearray((j*37+s.channel*13)&255 for j in range(512));root[:16]=b'U'*16;root[16]=0
        if kind.startswith('bad_'):root[0 if kind=='bad_first' else 15]=0xff
        root_expected=bytearray(v.sample(s.m,root_dest,512));table_expected=bytearray(v.sample(s.m,table_dest,1024))
        stats_before=[v.load(s.m,s.descriptor+o) for o in (0x30,0x34)];read_plan=[(511,root_dest,bytes(root))]
        if not kind.startswith('bad_'):
            for block in range(count):read_plan.append((509+block,table_dest+block*512,bytes((j*13+block*31+s.channel)&255 for j in range(512))))
        error_index=2 if kind=='table_late_bad' else 1 if kind in ('table_bad','table_latch') else None
        if error_index is not None:read_plan=read_plan[:error_index+1]
        frontier=kind=='valid';errors=0
        for op,(block,dest,raw) in enumerate(read_plan):
            for part in range(1 if op==error_index else 2):
                before=snap();token=s.tokens()[0];serial=v.load(s.m,s.context+0xfe0)
                s.call('pending_root_or_table',v.AI_SCHEDULE);harvest()
                require(s.tokens()[0]==token and v.load(s.m,s.context+0xfe0)==serial and snap()[0]==before[0],'pending wait duplicates no packet/frame')
                status=251 if kind=='table_latch' and op==error_index else 7 if op==error_index else 8
                payload=raw[part*256:(part+1)*256] if status==8 else None
                transfer(payload,status);last=op==len(read_plan)-1 and part==(0 if op==error_index else 1)
                s.resume();harvest(-2 if last and frontier else 0)
                if status!=8:errors+=1
                else:
                    expected=root_expected if op==0 else table_expected;offset=part*256 if op==0 else (op-1)*512+part*256
                    expected[offset:offset+256]=payload
                require(v.sample(s.m,root_dest,512)==root_expected and v.sample(s.m,table_dest,1024)==table_expected,'independent root/table bytes and untouched tails')
        require({0x8c64a4b2,0x8c660dac}<=s.seen,'original root/status bodies execute')
        expected_stats=[(stats_before[0]+len(read_plan))&0xffffffff,(stats_before[1]+errors)&0xffffffff]
        require([v.load(s.m,s.descriptor+o) for o in (0x30,0x34)]==expected_stats,'original read operation/error counters')
        calls=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']
        require(calls==[[s.channel,block,dest] for block,dest,_ in read_plan],'actual block indices and buffer addresses')
        progress=[b for r,_ in s.phases for b in r['boundaries'] if b['pr']=='0x8c647694' and b['target']=='0x8c05180c']
        require(len(progress)==len(read_plan),'one actual progress callback per read operation')
        for i,event in enumerate(progress):
            require(event['arguments'][:4]==[s.channel,1,(stats_before[0]+i)&0xffffffff,(stats_before[1]+(1 if error_index==i else 0))&0xffffffff],'original progress callback arguments')
        if not kind.startswith('bad_') and count:
            require({0x8c64ae62,0x8c64ae9a,0x8c64b236}<=s.seen,'actual positive table loop and nonlocal return routes')
        if frontier:
            require(s.records[-1]['stop_pc']=='0x8c64a9a8','real next directory dependency')
            require(v.load(s.m,s.channel_record+8)==1 and v.load(s.m,s.result_word)==0,'command remains pending')
            require(not any(r['callback_events'] for r in s.records),'no fabricated completion')
        else:
            result=0xffffff01 if kind in ('table_bad','table_late_bad') else 0
            require(v.load(s.m,s.result_word)==result and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original worker completion result/state')
            callbacks=[b for r in s.records for b in r['callback_events']]
            require(len(callbacks)==1 and callbacks[0]['arguments'][:3]==[s.channel,1,result],'original completion callback')
            if kind=='table_latch':
                require(v.load(s.m,0x8c89cf90+s.channel*4)==1,'original caller latched the generic table error')
                s.call('validate_original_error_latch',0x8c64a4b2,[s.channel]);harvest();require(s.s.r[0]==0xffffff03,'real latch causes original root validation error')
            if kind.startswith('bad_'):require(v.sample(s.m,s.descriptor+0xb,2)==bytes(2),'bad signature invalidates table/directory cache flags')
            s.call('completed_idle',v.AI_SCHEDULE);harvest();s.call('original_owner_cancel',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'original owner teardown')
        seen.update(s.seen);edges.update(s.edges)
        records.append(dict(input=config,root_destination=hex(root_dest),table_destination=hex(table_dest),stats_before=stats_before,stats_after=expected_stats,read_calls=calls,independent_checks=case_checks,phases=s.records,task_frames_never_repaired=True,completed_ids_never_injected=True))
        print('ROOT TASK',len(records),config,'comparisons',comparisons,flush=True)
    if smoke:print('ROOT TASK SMOKE PASS',len(records),comparisons,checks);return
    table=json.loads((P/'analysis/movie_filesystem_state_translation.json').read_text());table=next(r for r in table['routines'] if r['address']=='0x8c64ae62')
    coverage=[]
    for r in (v.manifest['routines'][0],table):
        missing=set(r['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,r['conditional_edges']))-edges
        coverage.append(dict(address=r['address'],static_rows=r['instruction_rows'],executed_rows=r['instruction_rows']-len(missing),executed_outcomes=len(r['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
    files=[Path(__file__),P/'movie_filesystem_root_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_root_task_fixtures.py',Path(v.__file__),P/'write_watch_filesystem_root.py',v.MP,P/'analysis/movie_filesystem_root_connection.json',P/'analysis/movie_filesystem_state_translation.json',v.effects.DLL]
    report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_qualified_nonlocal_routes=['0x8c64ae9a','0x8c64b236'],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original command1 header/root/table reads with declared raw metadata/signature/tablebytes,real submission/pump/contexts andcallback/error-latch paths. No taskframes,completed IDs,decoded metadata orparser/callback results supplied. Valid rawroot has16bytes55;count0/1/2 reaches actual64a9a8. Bad signatures complete original result0 with invalidatedcacheflags;table-255 propagates;table-65536 latches error andoriginalresult0. Positive table-loop resumption andbuffer/index/counter/callback oracles. Directory parsing,physical acquisition/fullgame unfinished.')
    (P/'analysis/movie_filesystem_root_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ROOT TASK PASS',len(records),comparisons,checks,flush=True)
if __name__=='__main__':main()
