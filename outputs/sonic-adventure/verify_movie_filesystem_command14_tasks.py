"""Original command14 range reads using original worker and raw device inputs."""
import itertools,json,sys
from pathlib import Path
from movie_filesystem_command14_vectors import Session,v,raw,put,digest,P
import movie_channel_request_fixtures as backend

def configurations():
    for i,(channel,order,endian) in enumerate(itertools.product((0,7),(0,1),(0,1))):
        yield dict(kind='core',channel=channel,order=order,endian=endian,count=i%3,start=1 if i==7 else 0)
    for i,kind in enumerate(('bad_data','bad_commit','special','unplug','abort_unplug','abort_bad','retry_success','retry_exhaust','live_infinite','protocol_data','protocol_commit','mutex_data','mutex_commit','retry_unplug','bad_chain')):
        yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=(i>>1)&1,count=1,start=0)

def main():
    assert v.effects.DLL.name=='movie_filesystem_command14.dll'
    if '--merge' in sys.argv:raise SystemExit('Use independent shard reports until reviewed merge is provided')
    configs=list(configurations());smoke='--smoke' in sys.argv
    shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
    if shard is not None:
        assert shard in range(3)
        configs=[c for i,c in enumerate(configs) if i%3==shard]
    if smoke:configs=configs[:1]
    if '--case' in sys.argv:configs=[c for c in configurations() if c['kind']==sys.argv[sys.argv.index('--case')+1]]
    records=[];comparisons=checks=0;seen=set();edges=set();ram=v.seed_ram();routes=set()
    for config in configs:
        kind=config['kind'];channel=config['channel'];order=config['order'];endian=config['endian'];count=config['count'];start=config['start']
        budget=1 if kind in ('retry_success','retry_exhaust','retry_unplug') else 0
        s=Session(ram,channel=channel,order=order,endian=endian,mode=4 if (channel+order+endian)&1 else 0,command=1,retry_budget=budget)
        done=0;case_checks=0
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(config,label);checks+=1;case_checks+=1
        def harvest():
            nonlocal done,comparisons
            for r,path in s.phases[done:]:
                require(r['status']==0,'host execution completed');require(r['full_state_match'],'all original/native state');require(r['ordered_boundaries_match'],'ordered original/native effects');comparisons+=1
            done=len(s.phases)
        def frame_pc():return v.load(s.m,v.load(s.m,s.task+0x18))
        def transfer(data=None,status=8,unplug=False,ack=False):
            queued=s.tokens()[0];require(queued>0,'actual queued request ID');sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            def intact():require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'transport preserves original savedframes/contexts')
            s.submit();harvest();intact();require(s.tokens()[:2]==[0,queued],'actual inflight request ID')
            s.observe(511,status=status,unplug=unplug,ack=ack,block_data=data,metadata_words={6:509,8:2,10:507,12:2,16:32} if data is None and not ack else None);intact();s.pump();harvest();intact();require(s.tokens()[1:]==[0,queued],'actual completed request ID')
        harvest();s.queue();harvest();transfer();s.resume();harvest()
        byte_order='big' if endian else 'little';root=b'U'*16+bytes(496);fat=bytearray(1024)
        for index,value in ((507,506),(506,0xfffa),(20,20 if kind=='bad_chain' else (0xfffa if config['length']==1 else 21)),(21,22 if config['length']==3 else 0xfffa),(22,0xfffa)):fat[index*2:index*2+2]=value.to_bytes(2,byte_order)
        def entry(type_,label):
            b=bytearray(32);b[0]=type_;b[1]=0xa5;b[2:4]=(20).to_bytes(2,byte_order);b[4:16]=(label+bytes(12))[:12];b[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,3));b[24:26]=config['length'].to_bytes(2,byte_order);b[26:28]=(1).to_bytes(2,byte_order);return bytes(b)
        directory=entry(0xcc,b'SONIC')+entry(0x33,b'DATA')+bytes(960)
        destinations=[v.load(s.m,s.descriptor+off) for off in (0x20,0x24,0x28)]
        plan=[(511,destinations[0],root),(509,destinations[1],bytes(fat[:512])),(510,destinations[1]+512,bytes(fat[512:])),(507,destinations[2],directory[:512]),(506,destinations[2]+512,directory[512:])]
        for block,dest,data in plan:
            for part in range(2):transfer(data[part*256:(part+1)*256]);s.resume();harvest()
            require(v.sample(s.m,dest,512)==data,'independent raw initialization bytes')
        require(v.load(s.m,s.result_word)==0 and v.sample(s.m,s.descriptor+10,3)==bytes((1,1,1)),'actual initialization completion')
        buffers=[v.sample(s.m,a,n) for a,n in zip(destinations,(512,1024,1024))]
        initial_reads=[b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']
        require(initial_reads==[[channel,b,d] for b,d,_ in plan],'independent original initialization read plan')

        acquired_pool=v.sample(s.m,0x8c8a1210,8*72)
        require(v.load(s.m,0x8c8a1450)==8 and acquired_pool==bytes(8*72),'original initialized eight-slot empty handlepool')
        allocated=[]
        if kind=='pool_full':
            for i in range(8):
                s.call('original_handle_allocation',0x8c64b2a6,[channel]);harvest();allocated.append(s.s.r[0]);require(allocated[-1]==0x8c8a1210+i*72,'original allocator acquires each busy slot')
        pool_before=v.sample(s.m,0x8c8a1210,8*72)
        label=b'MISSING' if kind.startswith('missing') else b'SONIC'
        raw(s.m,s.channel_record+0x2c,label+bytes(13-len(label)));out=0x8cdb0000;sentinel=bytes([0xa7])*2048;raw(s.m,out,sentinel)
        offset=config['offset'];arg_count=config['requested'];first=len(s.phases);postyield_seen=set();prior_counters=[v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]
        s.call('original_file_read_arguments_API',0x8c647cc4,[channel,channel,0,out],(offset,arg_count,0));harvest()
        s.call('original_file_read_opcode_API',0x8c647cb4,[channel,14]);harvest();s.resume();harvest()
        expected_result=-251 if kind=='missing_auto' else -191 if kind in ('missing_explicit','bad_chain') else -249 if kind=='pool_full' else -255 if kind in ('read_error','read_late_error') else 0
        early=kind in ('missing_auto','missing_explicit','bad_chain','pool_full')
        actual_count=arg_count or config['length']-offset
        assert actual_count>=0,'auto-count underflow is outside this bounded completed-task matrix'
        blocks=min(actual_count,max(0,config['length']-offset)) if not early else 0
        data=bytes((j*29+17+config['seed'])&255 for j in range(2048));requests=[];copied=0;errors=0
        error_index=0 if kind=='read_error' else 1 if kind=='read_late_error' else None
        for block_index in range(blocks):
            expected_dest=out+block_index*512;expected_block=20+offset+block_index
            new=[b['arguments'][:3] for r,_ in s.phases[first:] for b in r['boundaries'] if b['target']=='0x8c660a7e']
            require(new==requests+[[channel,expected_block,expected_dest]],'independent original offset/FAT-followed read plan');requests.append([channel,expected_block,expected_dest])
            if block_index==error_index:
                transfer(status=7,ack=True);r,path=s.resume();harvest();postyield_seen.update(path);errors=1;break
            for half in range(2):
                require(s.tokens()[0]>0,'original queued read request')
                at=(offset+block_index)*512+half*256
                transfer(data[at:at+256]);r,path=s.resume();harvest();postyield_seen.update(path)
            copied+=512
        require(s.tokens()[0]==0,'no excess raw request after original range/EOF handling')
        require(v.sample(s.m,out,2048)==data[offset*512:offset*512+copied]+sentinel[copied:],'exact caller range payload and untouched EOF/error tail')
        require(all(v.sample(s.m,a,len(b))==b for a,b in zip(destinations,buffers)),'all original root/FAT/directory bytes unchanged by read')
        require(v.sample(s.m,0x8c8a1210,8*72)==pool_before,'close clears allocated record;busy records unchanged on pool exhaustion')
        boundaries=[b for r,_ in s.phases[first:] for b in r['boundaries']]
        cb=[b for b in boundaries if b['pr']=='0x8c647bb4']
        require(v.load(s.m,s.result_word)==expected_result&0xffffffff and len(cb)==1 and cb[0]['arguments'][:3]==[channel,14,expected_result&0xffffffff],'independent original completion callback result')
        require(v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'actual command resets to idle')
        reads=[b['arguments'][:3] for b in boundaries if b['target']=='0x8c660a7e'];require(reads==requests,'no hidden file-data read requests')
        require(not any('write_request_input' in b for b in boundaries),'file read submits no writes')
        size_calls=[b for b in boundaries if b['target']=='0x8c64a360'];require(len(size_calls)==int(arg_count==0),'zero requested count queries original file length;nonzero bypasses size query')
        require([v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]==(prior_counters if early else [1+len(requests),actual_count+errors]),'independent original range attempt/error counters')
        require(len([b for b in boundaries if b['target']=='0x8c64965c'])==(0 if early else len(requests) if errors else actual_count),'original loop includes zero-result EOF calls after data runs out')
        seeks=[b for b in boundaries if b['target']=='0x8c64bb4c'];require(len(seeks)==int(not early) and all(b['arguments'][1:3]==[offset,0] for b in seeks),'actual original seek offset/mode input')
        for handle in allocated:s.call('original_handle_release',0x8c64b2d4,[handle]);harvest()
        require(v.sample(s.m,0x8c8a1210,8*72)==bytes(8*72),'test-owned original allocated handles released')
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual owner teardown')
        seen.update(s.seen);edges.update(s.edges)
        records.append(dict(configuration=config,result=hex(expected_result&0xffffffff),requests=requests,phases=s.records,independent_checks=case_checks,postyield_parent_pcs=sorted(hex(pc) for pc in postyield_seen if pc in ROUTES)))
        print('COMMAND14 TASK',len(records),kind,channel,'PASS',comparisons,flush=True)
    coverage=[]
    for body in v.ANALYSIS_ROUTINES:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'verify_movie_filesystem_write_tasks.py',P/'movie_filesystem_command14_vectors.py',P/'movie_filesystem_command14_session.py',P/'movie_filesystem_command14_reference.py',P/'write_watch_filesystem_command14.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_command14_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for rec in records for pc in rec['postyield_parent_pcs']}),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual initialized command14 range read and seek. Auto-count=size-offset only nonunderflow cases in completed-task matrix;explicit offset can exceed end. Seek return ignored by original master. EOF zero-result calls retain caller tail;no metadata writes. Original rawheader/root/FAT/linkedDIR,argument/opcode APIs/scheduler/open/handle allocation/read/close. Onlyrawdevicebytes supplied;no metadata/frame/ID/PR/calleeoutput/result substitutions. Exactdata/EOFsentinels/FATfollowedreadplan/rootFATDIRpreservation/poolcleanup/callback/idle/ownerteardown. Busyhandles acquired/releasedbyoriginalallocator/clear. This read-only task domain doesnotqualifydirtyclosewritebranches orphysicalDMA/fullfilesystem/game.')
    name='movie_filesystem_command14_tasks'+('' if shard is None else '_'+str(shard))+'_verification.json'
    if '--case' in sys.argv:dest=P.parents[1]/'work/sonic-adventure'/('command14_case_'+configs[0]['kind']+'.json')
    else:dest=P/'analysis'/name
    dest.write_text(json.dumps(report,indent=2)+'\n');print('COMMAND14 TASK PASS',comparisons,checks,flush=True)


ROUTES={0x8c647b3c,0x8c64bc22,0x8c64bc3a}

def configurations():
    items=[('auto_le',0,0,3),('auto_be',1,0,3),('last_block',2,1,3),('explicit_two',1,2,3),('explicit_overread',2,3,3),('at_end_auto',3,0,3),('at_end_explicit',3,1,3),('past_end_explicit',5,2,3),('empty_auto',0,0,0),('empty_explicit',1,1,0),('missing_auto',0,0,3),('missing_explicit',0,1,3),('bad_chain',0,0,3),('pool_full',0,0,3),('read_error',1,1,3),('read_late_error',0,3,3)]
    for i,(kind,offset,requested,length) in enumerate(items):yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=i&1,count=1,start=0,offset=offset,requested=requested,length=length,seed=i*7)
if __name__=='__main__':main()
