"""Original opcode15 whole-device reads with caller/raw-device inputs only."""
import json,sys
from pathlib import Path
from movie_filesystem_command15_vectors import Session,v,raw,put,digest,P

def configurations():
    items=[('one_le',0,0,0,0,None,0,7),('one_be',0,7,1,1,None,0,7),('two_le',1,0,1,0,None,0,7),('three_be',2,7,0,1,None,0,7),('extent33',33,0,0,1,None,0,7),('first_error',2,7,1,0,0,0,7),('later_error',2,0,1,1,1,0,7),('partial_error',2,7,0,1,1,1,7),('special_error',1,0,0,0,0,0,251),('full512',511,7,1,1,None,0,7)]
    for kind,geometry,channel,order,endian,error_block,error_half,error_status in items:
        yield dict(kind=kind,geometry=geometry,channel=channel,order=order,endian=endian,error_block=error_block,error_half=error_half,error_status=error_status)

def block_bytes(block,channel):
    return b''.join((0xa5000000^(channel<<20)^(block<<8)^j).to_bytes(4,'little') for j in range(128))

def main():
    assert v.effects.DLL.name=='movie_filesystem_command15.dll'
    configs=list(configurations());shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None;full='--full' in sys.argv;case=sys.argv[sys.argv.index('--case')+1] if '--case' in sys.argv else None
    if shard is not None:assert shard in range(3);configs=[c for i,c in enumerate(configs) if i%3==shard and c['kind']!='full512']
    if full:configs=[c for c in configs if c['kind']=='full512']
    if case:configs=[c for c in configurations() if c['kind']==case]
    ram=v.seed_ram();records=[];comparisons=checks=0;seen=set();edges=set()
    for config in configs:
        channel=config['channel'];geometry=config['geometry'];s=Session(ram,channel=channel,order=config['order'],endian=config['endian'],mode=4 if (channel+config['order']+config['endian'])&1 else 0,command=18,retry_budget=0);done=0;case_checks=0
        def require(ok,label):
            nonlocal checks,case_checks
            assert ok,(config,label);checks+=1;case_checks+=1
        def harvest():
            nonlocal done,comparisons
            for r,path in s.phases[done:]:
                require(r['status']==0,'host execution completed');require(r['full_state_match'],'complete original/native machine');require(r['ordered_boundaries_match'],'ordered original/native effects');comparisons+=1
            done=len(s.phases)
        def transfer(data=None,status=8):
            queued=s.tokens()[0];require(queued>0,'original queued request ID');sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            def intact():require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'raw transport preserves original savedframe/32contexts')
            s.submit();harvest();intact();require(s.tokens()[:2]==[0,queued],'actual queued to inflight ID')
            s.observe(geometry,status=status,block_data=data,ack=data is None and status!=8);intact();s.pump();harvest();intact();require(s.tokens()[1:]==[0,queued],'actual inflight to completed ID')
        harvest();s.queue();harvest();transfer();expected_metadata=s.metadata_expected();s.resume();harvest()
        require([v.load(s.m,s.output+j*4) for j in range(19)]==expected_metadata,'independent raw header decode;no supplied metadata')
        if geometry==511:
            boot=block_bytes(geometry,channel);before=len([b for rr,path in s.phases for b in rr['boundaries'] if b['target']=='0x8c660a7e']);require(before==1,'original query18 block read for standard device')
            for half in range(2):transfer(boot[half*256:(half+1)*256]);s.resume();harvest()
        require(v.load(s.m,s.result_word)==(0x21 if geometry==511 else -125)&0xffffffff and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original query18 completes validation/classification')
        require(v.load(s.m,s.output)==geometry,'last-block index acquired through original raw-header reader')
        initial_reads=[b['arguments'][:3] for rr,path in s.phases for b in rr['boundaries'] if b['target']=='0x8c660a7e']
        require(len(initial_reads)==int(geometry==511),'small unsupported geometry uses original metadata-query error path')
        metadata=v.sample(s.m,s.output,76);pool=v.sample(s.m,0x8c8a1210,8*72);out=0x8cda0010;size=(geometry+1)*512;sentinel=bytes([0xa7])*size;guard=bytes([0x6d])*16;raw(s.m,out-16,guard+sentinel+guard);wanted=bytearray(sentinel)
        counter_base=v.load(s.m,v.load(s.m,0x8c64b5d4)+v.load(s.m,s.descriptor+4)*80)
        put(s.m,0x8c66d3a0,0);first=len(s.phases);s.call('original_bulk_read_arguments_API',0x8c647cc4,[channel,channel,out,0],(0,0,0));harvest();s.call('original_bulk_read_opcode_API',0x8c647cb4,[channel,15]);harvest();s.resume();harvest()
        requests=[];progress=[];postyield=set();failed=False;copied=0
        for block in range(geometry+1):
            calls=[b['arguments'][:3] for rr,path in s.phases[first:] for b in rr['boundaries'] if b['target']=='0x8c660a7e'];request=[channel,block,out+block*512];require(calls==requests+[request],'literal increasing block and512-byte destination plan');requests.append(request)
            data=block_bytes(block,channel)
            for half in range(2):
                failure=block==config['error_block'] and half==config['error_half']
                transfer(None if failure else data[half*256:(half+1)*256],config['error_status'] if failure else 8)
                if not failure:wanted[block*512+half*256:block*512+(half+1)*256]=data[half*256:(half+1)*256];copied+=256
                rr,path=s.resume();harvest();postyield.update(path)
                require(v.sample(s.m,out,size)==wanted,'independent entire caller image and untouched error tail');require(v.sample(s.m,out-16,16)==guard and v.sample(s.m,out+size,16)==guard,'caller output guards')
                require(v.sample(s.m,s.output,76)==metadata and v.sample(s.m,0x8c8a1210,8*72)==pool,'metadata and handlepool unchanged')
                if failure:failed=True;break
            progress.append([channel,15,len(requests),counter_base+int(failed)])
            actual_progress=[b['arguments'] for rr,path in s.phases[first:] for b in rr['boundaries'] if b['target']=='0x8c647bde'];require(actual_progress==progress,'independent progress after every completed block')
            if block%32==0 or block==geometry or failed:print('BULKREAD',config['kind'],'block',block,'/',geometry,'comparisons',comparisons,flush=True)
            if failed:break
        result=(-255 if config['error_status']==7 else -65536) if failed else 0
        boundaries=[b for rr,path in s.phases[first:] for b in rr['boundaries']];callbacks=[b for b in boundaries if b['pr']=='0x8c647bb4']
        require(s.tokens()[0]==0 and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'original worker idle,no extra request')
        require(v.load(s.m,s.result_word)==result&0xffffffff and len(callbacks)==1 and callbacks[0]['arguments'][:3]==[channel,15,result&0xffffffff],'independent original result callback')
        require([b['arguments'] for b in boundaries if b['target']=='0x8c647bde']==progress,'independent progress/error callbacks')
        require([v.load(s.m,s.descriptor+off) for off in (0x30,0x34)]==[1+len(requests),counter_base+int(failed)],'original final counters')
        require([b['arguments'][:3] for rr,path in s.phases for b in rr['boundaries'] if b['target']=='0x8c660a7e']==initial_reads+requests,'no hidden reads');require(not any('write_request_input' in b for b in boundaries),'whole-device read submits no writes')
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'original owner teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,result=hex(result&0xffffffff),copied_bytes=copied,output_sha256=__import__('hashlib').sha256(wanted).hexdigest(),requests=requests,progress=progress,postyield_parent_pcs=sorted(hex(pc) for pc in postyield if pc in {0x8c64b5e4,0x8c647abc}),phases=s.records,independent_checks=case_checks))
        print('COMMAND15 TASK',config['kind'],'PASS',comparisons,checks,flush=True)
    coverage=[]
    for body in v.ANALYSIS_ROUTINES:
        rows=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges;coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(rows),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(rows),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'movie_filesystem_command15_vectors.py',P/'movie_filesystem_command15_reference.py',P/'movie_filesystem_command15_session.py',P/'movie_filesystem_command15_task_fixtures.py',P/'write_watch_filesystem_command15.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_command15_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for c in records for pc in c['postyield_parent_pcs']}),dll_sha256=digest(v.effects.DLL),sha256={str(f):digest(f) for f in files},scope='Original initialized metadataquery18 thenactualwhole-deviceopcode15,originalAPIs/scheduler/rawtransport/pump. Standardgeometry511 readsall512blocks withcoherentstableper-blockrawimage;smallerrawgeometries0/1/2/33 explicitlyuseoriginalquery18unsupportedgeometry-125 whilepreservingdecodedmetadata,notforcedparsedstate. Exact512stride/blockplan/256byteparts/fullcallerimage/errorprefix-tail/guards/metadata/pool/counters/callback/idle/teardown. No suppliedcalleeoutput/frame/IDs/PR. Retrybudget0viaoriginalAPI,abortpolicy0explicit. PhysicalDMA/storage/fullgameunfinished.')
    suffix='_full' if full else '_'+str(shard) if shard is not None else '';dest=P/'analysis'/('movie_filesystem_command15_tasks'+suffix+'_verification.json')
    if case:dest=P.parents[1]/'work/sonic-adventure'/('command15_case_'+case+'.json')
    dest.write_text(json.dumps(report,indent=2)+'\n');print('COMMAND15 TASKS PASS',comparisons,checks,flush=True)
if __name__=='__main__':main()
