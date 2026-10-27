"""Original command20 writes with raw device ACK/error/removal inputs only."""
import itertools,json,sys
from pathlib import Path
from movie_filesystem_write_vectors import Session,v,raw,put,digest,P
import movie_channel_request_fixtures as backend

def configurations():
    for i,(channel,order,endian) in enumerate(itertools.product((0,7),(0,1),(0,1))):
        yield dict(kind='core',channel=channel,order=order,endian=endian,count=i%3,start=1 if i==7 else 0)
    for i,kind in enumerate(('bad_data','bad_commit','special','unplug','abort_unplug','abort_bad','retry_success','retry_exhaust','live_infinite','protocol_data','protocol_commit','mutex_data','mutex_commit','retry_unplug','bad_chain')):
        yield dict(kind=kind,channel=7 if i&1 else 0,order=i&1,endian=(i>>1)&1,count=1,start=0)

def main():
    assert v.effects.DLL.name=='movie_filesystem_write.dll'
    if '--merge' in sys.argv:return merge()
    configs=list(configurations());smoke='--smoke' in sys.argv
    shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
    if shard is not None:
        assert shard in range(3)
        configs=[c for i,c in enumerate(configs) if i%3==shard]
    if smoke:configs=[dict(kind='core',channel=0,order=0,endian=0,count=1,start=0)]
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
        def command(label,begin,total):
            require(v.load(s.m,s.channel_record+8)==0,'caller filename only while idle');raw(s.m,s.channel_record+0x2c,label+bytes(13-len(label)))
            s.call('write_arguments_api',0x8c647cc4,[channel,channel,0,0x8cdb0000],(begin,total,0));harvest();s.call('write_opcode_api',0x8c647cb4,[channel,20]);harvest();first=len(s.phases);s.resume();harvest();return first
        prechecks=[]
        if config==list(configurations())[0]:
            for label,begin,total,result in ((b'MISSING',0,1,0xffffff05),(b'DATA',0,1,0xffffff0b),(b'SONIC',2,0,0xffffff00),(b'SONIC',0,3,0xffffff00),(b'SONIC',1,2,0xffffff00)):
                first=command(label,begin,total);cb=[b for r,_ in s.phases[first:] for b in r['boundaries'] if b['pr']=='0x8c647bb4']
                require(v.load(s.m,s.result_word)==result and len(cb)==1 and cb[0]['arguments'][:3]==[channel,20,result],'independent precheck result/callback');s.resume();harvest();prechecks.append(dict(name=label.decode(),start=begin,count=total,result=hex(result)))
        data=bytes((j*13+17)&255 for j in range(1024));raw(s.m,0x8cdb0000,data)
        # Declared original software policy input; no callee output is supplied.
        put(s.m,0x8c66d3a0,int(kind.startswith('abort_')))
        if kind=='mutex_data':put(s.m,backend.LOCK,0x80,1)
        first=command(b'SONIC',start,count);yields=[];responses=[];expected_requests=[];failures=0;attempts=0;part=0;block_index=0;packet_count=0
        if kind=='live_infinite':s.call('original_live_retry_policy_api',0x8c647534,[0xffffffff]);harvest()
        if kind=='mutex_data':
            require(s.tokens()[0]==0 and frame_pc()==0x8c660c6a,'real data mutex yield');yields.append(hex(frame_pc()));put(s.m,backend.LOCK,0,1);s.resume();harvest()
        while s.tokens()[0] or (kind=='retry_unplug' and frame_pc()==0x8c660be6 and v.load(s.m,s.channel_record+8)):
            if not s.tokens()[0]:
                require(frame_pc()==0x8c660be6,'original retry not-ready yield');yields.append(hex(frame_pc()));s.resume();harvest();continue
            packet_count+=1;require(packet_count<=40,'finite declared response plan')
            pc=frame_pc();require(pc in (0x8c660c7c,0x8c660cf6),'actual data/commit response yield');yields.append(hex(pc))
            commit=part==4;require(pc==(0x8c660cf6 if commit else 0x8c660c7c),'independent data/commit phase')
            expected_requests.append(dict(mapped_unit=s.mapped,command=13 if commit else 12,parameters=[2,((part<<16)|(start+block_index))&0xffffffff],words=0 if commit else 32,data=[] if commit else list(data[block_index*512+part*128:block_index*512+(part+1)*128])))
            status=7;unplug=False;repeat=False;error=False
            if kind in ('bad_data','abort_bad') and not commit:status=8;error=True
            if kind=='bad_commit' and commit:status=8;error=True
            if kind=='special' and commit:status=251;error=True
            if kind in ('unplug','abort_unplug','retry_unplug') and packet_count==1:unplug=True;error=True
            if kind in ('retry_success','retry_exhaust','live_infinite') and failures<(2 if kind in ('retry_exhaust','live_infinite') else 1):status=8;error=True
            if kind=='protocol_data' and not commit and packet_count==1:status=252;repeat=True
            if kind=='protocol_commit' and commit and len([x for x in responses if x['status']==252])==0:status=252;repeat=True
            transfer(status=status,unplug=unplug,ack=True);responses.append(dict(status=status,unplug=unplug))
            # Transport shares this mutex. Contention starts after the real
            # transport/pump, before the waiting task tries to queue commit.
            if kind=='mutex_commit' and part==3:put(s.m,backend.LOCK,0x80,1)
            s.resume();harvest()
            if error:failures+=1;part=0;attempts+=1
            elif not repeat:
                if commit:block_index+=1;part=0;attempts+=1
                else:part+=1
            if kind=='mutex_commit' and part==4 and not error:
                require(s.tokens()[0]==0 and frame_pc()==0x8c660ce4,'real commit mutex yield');yields.append(hex(frame_pc()));put(s.m,backend.LOCK,0,1);s.resume();harvest()
        result={'bad_data':-255,'bad_commit':-255,'special':-192,'unplug':-65536,'abort_unplug':-255,'abort_bad':-255,'retry_exhaust':-255,'retry_unplug':-255,'bad_chain':-191}.get(kind,0)&0xffffffff
        require(v.load(s.m,s.result_word)==result and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'independent final result/idle fields')
        boundaries=[b for r,_ in s.phases[first:] for b in r['boundaries']]
        actual=[b['write_request_input'] for b in boundaries if 'write_request_input' in b]
        require(len(actual)==len(expected_requests),'actual original request-input count')
        for got,wanted in zip(actual,expected_requests):require({k:got[k] for k in wanted}==wanted,'independent request descriptor/phase/data input')
        callbacks=[b for b in boundaries if b['pr']=='0x8c647bb4'];require(len(callbacks)==1 and callbacks[0]['arguments'][:3]==[channel,20,result],'one actual command completion callback')
        progress=[b['arguments'] for b in boundaries if b['target']=='0x8c647bde']
        increments=0 if kind in ('bad_chain','abort_unplug') else (count if not result else 1)
        expected_errors=failures+(1 if kind=='retry_unplug' else 0)
        require(v.load(s.m,s.descriptor+0x30)==1+increments and v.load(s.m,s.descriptor+0x34)==count+(0 if kind=='abort_unplug' else expected_errors),'independent descriptor counters')
        require(progress==[[channel,20,1+i,count+expected_errors] for i in range(increments)],'independent progress callback input')
        require(v.sample(s.m,0x8cdb0000,1024)==data,'caller data unchanged')
        require(all(v.sample(s.m,a,len(b))==b for a,b in zip(destinations,buffers)),'initialized metadata buffers unchanged')
        require([b['arguments'][:3] for r,_ in s.phases for b in r['boundaries'] if b['target']=='0x8c660a7e']==initial_reads,'no hidden block reads')
        s.resume();harvest();s.call('original_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual owner teardown')
        seen.update(s.seen);edges.update(s.edges);routes.update(yields)
        records.append(dict(configuration=config,prechecks=prechecks,result=hex(result),requests=actual,responses=responses,genuine_saved_resume_pcs=yields,progress_calls=progress,phases=s.records,independent_checks=case_checks))
        print('WRITE TASK',len(records),kind,channel,order,endian,'PASS',comparisons,flush=True)
    if smoke:print('WRITE TASK SMOKE PASS',comparisons,checks);return
    coverage=[]
    manifests=[v.manifest,json.loads((P/'analysis/movie_filesystem_command_translation.json').read_text())]
    for body in [b for m in manifests for b in m['routines'] if m is manifests[0] or b['address']=='0x8c64bcd6']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),P/'movie_filesystem_write_vectors.py',P/'movie_filesystem_write_session.py',P/'movie_filesystem_write_task_fixtures.py',Path(v.__file__),P/'write_watch_filesystem_write.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_write_connection.json',v.effects.DLL]
    report=dict(task_sessions_passed=len(records),completed_initializations=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,genuine_saved_resume_pcs=sorted(routes),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual raw header/root/FAT/directory initialization andcommand20 APIs. Raw emptyACK/error/removal responses driveactual SDKpump/resumption. Callerfilename/data/ranges,live originalretrypolicy API andexternalmutexcontention areinputs;no taskframe,requestID,PR,parsedmetadata,calleeoutput orcallbackresult supplied. Observer recordsrequestbuilder inputs only;serializedbytes independently tested inseparate packetsuite. Backend optionalhook constructor-clearedNULL;nonNULLhook domain remainsunqualified. Raw backendblock argument retainsoriginal suppliedstart coordinate;no invented FATtranslation. Notphysicalhardware/fullfilesystem/playablegame.')
    name='movie_filesystem_write_tasks'+('' if shard is None else '_'+str(shard))+'_verification.json'
    (P/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n');print('WRITE TASK PASS',comparisons,checks,flush=True)

def merge():
    paths=[P/f'analysis/movie_filesystem_write_tasks_{i}_verification.json' for i in range(3)]
    reports=[json.loads(p.read_text()) for p in paths];common=reports[0];pins={}
    for r in reports:
        assert r['dll_sha256']==digest(v.effects.DLL)
        for path,h in r['sha256'].items():assert digest(path)==h,path;pins[path]=h
    cases=[c for r in reports for c in r['cases']];expected=list(configurations())
    assert len(cases)==len(expected)==23 and sorted(json.dumps(c['configuration'],sort_keys=True) for c in cases)==sorted(json.dumps(c,sort_keys=True) for c in expected)
    coverage=[]
    for first in common['coverage_by_routine']:
        cvs=[next(c for c in r['coverage_by_routine'] if c['address']==first['address']) for r in reports]
        rows=set.intersection(*(set(c['missing_rows']) for c in cvs));edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
        coverage.append(dict(address=first['address'],static_rows=first['static_rows'],executed_rows=first['static_rows']-len(rows),executed_outcomes=first['executed_outcomes']+len(first['missing_outcomes'])-len(edges),missing_rows=sorted(rows),missing_outcomes=sorted(edges)))
    for p in paths:pins[str(p)]=digest(p)
    result=dict(common,task_sessions_passed=23,completed_initializations=23,original_native_comparisons=sum(r['original_native_comparisons'] for r in reports),independent_checks=sum(r['independent_checks'] for r in reports),cases=sorted(cases,key=lambda c:expected.index(c['configuration'])),coverage_by_routine=coverage,genuine_saved_resume_pcs=sorted({p for r in reports for p in r['genuine_saved_resume_pcs']}),sha256=pins,independent_process_shards=3)
    (P/'analysis/movie_filesystem_write_tasks_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('WRITE TASK MERGED PASS',result['original_native_comparisons'],result['independent_checks'],flush=True)
if __name__=='__main__':main()
