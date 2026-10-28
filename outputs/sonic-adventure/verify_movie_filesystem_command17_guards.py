"""Original batch-command guards; caller APIs/raw scans and declared SDK busy input."""
import json,sys
from pathlib import Path
from movie_filesystem_command17_vectors import Session,v,raw,put,digest,P
import movie_filesystem_command17_session as session_module

def disabled_constructor():
    source=session_module.source;marker='[0x21,1<<channel,DEVICE_BUFFER,0]';assert source.count(marker)==1
    # Parameterize only the caller's channel mask, preserving every constructor
    # instruction, native implementation and all other fixture behavior.
    source=source.replace(marker,'[0x21,0,DEVICE_BUFFER,0]')
    ns=dict(__name__='original_disabled_interface_fixture',__file__=str(P/'movie_input_interface_session.py'))
    exec(compile(source,'original_interface_mask0_fixture','exec'),ns)
    return ns['Session']

def main():
    ram=v.seed_ram();disabled=disabled_constructor();records=[];seen=set();edges=set();comparisons=checks=0
    configs=[dict(kind=kind,channel=ch,order=int(ch==7),endian=int(ch==7)) for kind in ('disabled','no_header','raw_absent','bad_root','busy') for ch in (0,7)]
    if '--case' in sys.argv:configs=[c for c in configs if c['kind']==sys.argv[sys.argv.index('--case')+1]]
    for config in configs:
        kind=config['kind'];channel=config['channel'];cls=disabled if kind=='disabled' else Session
        s=cls(ram,channel=channel,command=1 if kind=='bad_root' else 18,order=config['order'],endian=config['endian'],mode=4 if channel else 0,retry_budget=0);done=case_checks=0
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(config,label);checks+=1;case_checks+=1
        def harvest():
            nonlocal comparisons,done
            for r,path in s.phases[done:]:
                require(r['status']==0,'execution completed');require(r['full_state_match'],'full original/native machine');require(r['ordered_boundaries_match'],'ordered original/native effects');comparisons+=1
            done=len(s.phases)
        def transfer(data=None):
            queued=s.tokens()[0];require(queued>0,'original queued request ID');sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            def intact():require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'transport preserves original savedframe/contexts')
            s.submit();harvest();intact();require(s.tokens()[:2]==[0,queued],'actual inflight request')
            s.observe(511,block_data=data);intact();s.pump();harvest();intact();require(s.tokens()[1:]==[0,queued],'actual completed request');s.resume();harvest()
        harvest()
        if kind=='bad_root':
            s.queue();harvest();transfer()
            data=b''.join((0xa5000000^(channel<<20)^(511<<8)^j).to_bytes(4,'little') for j in range(128))
            for half in range(2):transfer(data[half*256:(half+1)*256])
            require(v.load(s.m,s.result_word)==0 and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original initialization completes after bad-format root; format return is ignored by64b1f0')
            require(v.sample(s.m,s.descriptor+8,1)==b'\1','original command1 acquired metadata admission flag')
            root_pointer=v.load(s.m,s.descriptor+0x20)
            require(v.sample(s.m,root_pointer,512)==data and data[:16]!=b'U'*16,'actual original root read acquired deliberately invalid raw format')
        if kind=='raw_absent':
            sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            require(s.tokens()[:2]==[0,0],'raw idle scan has no storage request');s.submit();harvest();s.observe(status=7,unplug=True,ack=True);s.pump();harvest()
            require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'raw removal preserves original task state')
        if kind=='busy':
            # Explicit mapped SDK device-busy state. This is not claimed to be
            # naturally acquired contention or physical device proof. The real
            # busy getter and all original guards still execute; no return,
            # parsed metadata, frame or request ID is supplied.
            put(s.m,s.d['status']+0xc,1,1)
        if kind=='disabled':require(v.sample(s.m,s.descriptor+2,1)==b'\0','original interface mask0 leaves channel disabled')
        if kind in ('no_header','busy'):require(v.sample(s.m,s.descriptor+8,1)==b'\0','original constructor has not acquired metadata')
        for entry in (0x8c647c50,0x8c647c6c):s.call('original_callback_registration_API',entry,[0x8c05180c]);harvest()
        buffers={a:v.sample(s.m,a,n) for off,n in ((0x20,512),(0x24,1024),(0x28,1024)) if (a:=v.load(s.m,s.descriptor+off))}
        metadata=v.sample(s.m,s.output,76) if s.output else None
        counter_base=v.load(s.m,v.load(s.m,0x8c649eec)+v.load(s.m,s.descriptor+4)*80)
        first=len(s.phases);s.call('original_guard_arguments_API',0x8c647cc4,[channel,channel,0,0],(0,0,0));harvest();s.call('original_guard_opcode_API',0x8c647cb4,[channel,17]);harvest();s.resume();harvest()
        result=-253 if kind=='bad_root' else -255
        bs=[b for rr,path in s.phases[first:] for b in rr['boundaries']];cb=[b for b in bs if b['pr']=='0x8c647bb4']
        require(v.load(s.m,s.result_word)==result&0xffffffff and len(cb)==1 and cb[0]['arguments'][:3]==[channel,17,result&0xffffffff],'independent guard result/callback')
        require(s.tokens()[:2]==[0,0] and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original idle without storage request')
        require([v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]==[1,counter_base],'initial counters without attempted write')
        require(not any('write_request_input' in b or b['target'] in ('0x8c649c68','0x8c64ab36','0x8c64afe0','0x8c647bde') for b in bs),'guards precede deletion/persistence/progress')
        require(all(v.sample(s.m,a,len(data))==data for a,data in buffers.items()),'all constructed filesystem buffers unchanged')
        if metadata is not None:require(v.sample(s.m,s.output,76)==metadata,'decoded metadata unchanged')
        invalidations=[b['pr'] for b in bs if b['target']=='0x8c64850a'];require(invalidations==([] if kind=='bad_root' else ['0x8c649e50','0x8c647b82'] if kind=='raw_absent' else ['0x8c647b82']),'exact original invalidation callsites; worker invalidates only-255')
        s.resume();harvest();s.call('original_guard_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'original owner teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result&0xffffffff),phases=s.records,independent_checks=case_checks,device_busy_input_is_declared_mapped_state=kind=='busy',natural_busy_acquisition_proven=False))
        print('COMMAND17 GUARD',kind,channel,'PASS',comparisons,checks,flush=True)
    coverage=[]
    for body in v.ANALYSIS_ROUTINES:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges;coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'movie_filesystem_command17_vectors.py',P/'movie_filesystem_command17_reference.py',P/'movie_filesystem_command17_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_command17_task_fixtures.py',P/'write_watch_filesystem_command17.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_command17_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(f):digest(f) for f in files},scope='Original opcode17 guards via original interface mask0,constructor-only missingmetadata,raw primaryscan removal,originalcommand1 acquisition ofbadformatrawroot,anddeclaredmappedSDKbusybyte. Alloriginalhelpers execute;no suppliedcalleeoutput/parsedmetadata/frame/ID/PR. Busybyte1 isexplicitABIstate,notnaturallyacquired contention. Exactrejection/callback/idle/counters/invalidation/no mutation orwrites/teardown. No physicalstorage/game claim.')
    dest=P/'analysis/movie_filesystem_command17_guards_verification.json'
    if '--case' in sys.argv:dest=P.parents[1]/'work/sonic-adventure'/('command17_guard_'+configs[0]['kind']+'.json')
    dest.write_text(json.dumps(report,indent=2)+'\n');print('COMMAND17 GUARDS PASS',comparisons,checks,flush=True)
if __name__=='__main__':main()
