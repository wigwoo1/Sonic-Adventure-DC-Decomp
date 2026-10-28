"""Original command20 writes with raw device ACK/error/removal inputs only."""
import itertools,json,sys
from pathlib import Path
from movie_filesystem_command11_vectors import Session,v,raw,put,digest,P
import movie_channel_request_fixtures as backend

def configurations():
    for i,(channel,order,endian) in enumerate(itertools.product((0,7),(0,1),(0,1))):
        yield dict(kind='core',channel=channel,order=order,endian=endian,count=i%3,start=1 if i==7 else 0)
    for i,kind in enumerate(('bad_data','bad_commit','special','unplug','abort_unplug','abort_bad','retry_success','retry_exhaust','live_infinite','protocol_data','protocol_commit','mutex_data','mutex_commit','retry_unplug','bad_chain')):
        yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=(i>>1)&1,count=1,start=0)

def main():
    assert v.effects.DLL.name=='movie_filesystem_command11.dll'
    if '--merge' in sys.argv:raise SystemExit('No merge needed for this suite')
    configs=list(configurations());smoke='--smoke' in sys.argv
    shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
    if shard is not None:
        assert shard in range(3)
        configs=[c for i,c in enumerate(configs) if i%3==shard]
    if smoke:configs=configs[:1]
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
        for index,value in ((507,506),(506,0xfffa),(20,20 if kind=='bad_chain' else 21),(21,0xfffa)):fat[index*2:index*2+2]=value.to_bytes(2,byte_order)
        def entry(type_,label):
            b=bytearray(32);b[0]=type_;b[1]=0xa5;b[2:4]=(20).to_bytes(2,byte_order);b[4:16]=(label+bytes(12))[:12];b[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,3));b[24:26]=(2).to_bytes(2,byte_order);b[26:28]=(1).to_bytes(2,byte_order);return bytes(b)
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

        put(s.m,0x8c66d3a0,config['abort_policy'])
        raw(s.m,s.channel_record+0x2c,b'SONIC'+bytes(8));out=0x8cdb0000;sentinel=bytes([0xa7])*1024;raw(s.m,out,sentinel)
        first=len(s.phases);s.call('original_file_read_arguments_API',0x8c647cc4,[channel,channel,0,out],(0,0,0));harvest();s.call('original_file_read_opcode_API',0x8c647cb4,[channel,11]);harvest();s.resume();harvest()
        require(s.tokens()[0]>0,'original read queued before raw response error')
        transfer(status=7,ack=True);s.resume();harvest()
        result=-255&0xffffffff
        require(s.tokens()[0]==0,'original error propagation stops further reads')
        require(v.sample(s.m,out,1024)==sentinel,'failed first read preserves all caller bytes')
        require(all(v.sample(s.m,a,len(b))==b for a,b in zip(destinations,buffers)),'read error preserves acquired root/FAT/DIR bytes')
        require(v.sample(s.m,0x8c8a1210,8*72)==bytes(8*72),'original close releases handle after failed read')
        boundaries=[b for r,_ in s.phases[first:] for b in r['boundaries']];cb=[b for b in boundaries if b['pr']=='0x8c647bb4']
        require(v.load(s.m,s.result_word)==result and len(cb)==1 and cb[0]['arguments'][:3]==[channel,11,result],'independent original error callback')
        require(v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'failed read returns worker to idle')
        reads=[b['arguments'][:3] for b in boundaries if b['target']=='0x8c660a7e'];require(reads==[[channel,20,out]],'failed read submits exactly one expected block')
        require(not any('write_request_input' in b for b in boundaries),'error cleanup submits no writes')
        require(v.load(s.m,s.descriptor+0x30)==2 and v.load(s.m,s.descriptor+0x34)==3,'original-255 response increments both counters under either policy;earlyabort compares-208')
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual owner teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result),requests=reads,phases=s.records,independent_checks=case_checks))
        print('COMMAND11 READERROR',len(records),config,'PASS',comparisons,flush=True)
    coverage=[]
    for body in v.manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'verify_movie_filesystem_write_tasks.py',P/'movie_filesystem_command11_vectors.py',P/'movie_filesystem_command11_session.py',P/'movie_filesystem_command11_reference.py',P/'write_watch_filesystem_command11.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_command11_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original initialized command11 read-error propagation usingrawstatus7 emptydevice response. Explicitcoherent softwareabortpolicy0/1 input;no calleeoutput/decodedmetadata/frame/ID/PR supplied. Exactcallerpayload/rootFATDIRpreservation,requestplan,ordinary-255counter,close/handlecleanup,callback/idle/ownerteardown. NotphysicalDMA/fullfilesystem/dirtyclosequalification.')
    name='movie_filesystem_command11_read_errors'+('' if shard is None else '_'+str(shard))+'_verification.json';(P/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n');print('COMMAND11 READERROR PASS',comparisons,checks,flush=True)

def configurations():
    for channel in (0,7):
        for policy in (0,1):yield dict(kind='read_error',channel=channel,order=int(channel==7),endian=int(channel==7),count=1,start=0,abort_policy=policy)
if __name__=='__main__':main()
