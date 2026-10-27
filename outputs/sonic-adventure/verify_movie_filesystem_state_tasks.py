"""Actual task-created root reads, retries, raw responses and caller resumptions."""
import hashlib,itertools,json,sys
from pathlib import Path
from movie_filesystem_state_session import Session,v,tasks
P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
    for channel,order,endian,mode in itertools.product((0,7),(0,1),(0,1),(0,4)):
        yield dict(channel=channel,order=order,endian=endian,mode=mode,kind='success')
    for channel,kind in itertools.product((0,7),('bad','generic','unplug','abort_unplug','abort_bad','retry_success','retry_exhaust','live_infinite','protocol_retry')):
        yield dict(channel=channel,order=channel&1,endian=channel&1,mode=4 if channel else 0,kind=kind)
    for progress in (0,0x8cfef000):yield dict(channel=0,order=0,endian=0,mode=0,kind='progress',progress=progress)
def main():
    configs=list(configurations());smoke='--smoke' in sys.argv
    if smoke:configs=[configs[0]]+[next(c for c in configs if c['kind']==kind) for kind in ('generic','unplug','protocol_retry')]+[c for c in configs if c['kind']=='progress']
    ram=v.seed_ram();records=[];seen=set();edges=set();comparisons=checks=0
    for config in configs:
        kind=config['kind'];budget=1 if kind in ('retry_success','retry_exhaust') else 0
        s=Session(ram,command=1,retry_budget=budget,**{k:config[k] for k in ('channel','order','endian','mode')});done=0;count=0
        def require(condition,label):
            nonlocal checks,count
            assert condition,(config,label);checks+=1;count+=1
        def harvest(status=0):
            nonlocal done,comparisons
            for i in range(done,len(s.phases)):
                result,path=s.phases[i];expected=status if i==len(s.phases)-1 else 0
                require(result['status']==expected,'host status');require(result['full_state_match'],'entire machine/device state');require(result['ordered_boundaries_match'],'ordered boundaries');comparisons+=1
            done=len(s.phases)
        def snapshot():
            sp=v.load(s.m,s.task+0x18)
            return sp,v.sample(s.m,sp,s.idle_sp+64-sp),v.sample(s.m,s.owner+0x210,32*0x124)
        def intact(before):
            sp,frame,contexts=before
            require(v.sample(s.m,sp,len(frame))==frame,'transport preserves saved stack')
            require(v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'transport preserves all task contexts')
        def transfer(status=8,unplug=False,data=None):
            queued=s.tokens()[0];require(queued>0,'request is originally queued');before=snapshot()
            s.submit();harvest();intact(before);require(s.tokens()[:2]==[0,queued],'original queued to inflight')
            s.observe(511,status,unplug,data);intact(before)
            s.pump();harvest();intact(before);require(s.tokens()[1:]==[0,queued],'original inflight to completed')
        harvest()
        # Explicit initial coherent software-policy input, before the command.
        # This is not a task frame, result, decoded metadata or completed token.
        abort=int(kind.startswith('abort_'));tasks.put(s.m,0x8c66d3a0,abort)
        s.queue();harvest();require(s.command_apis_preserved_task_records,'original APIs preserve task contexts')
        transfer();metadata=s.metadata_expected();s.resume();harvest()
        require([v.load(s.m,s.output+j*4) for j in range(19)]==metadata,'independent header decoding before block bytes')
        require({0x8c64ae32,0x8c647610}<=s.seen,'actual root gateway and retry reader execute')
        require(v.load(s.m,v.load(s.m,s.task+0x18))==0x8c660b2e,'original nested block wait')
        require(s.tokens()[0]>0 and s.tokens()[2]==1,'original root read request after header completion')
        dest=v.load(s.m,s.descriptor+0x20);require(dest!=0,'original root destination')
        stats_before=[v.load(s.m,s.descriptor+o) for o in (0x30,0x34)];expected=bytearray(v.sample(s.m,dest,512))
        if kind=='live_infinite':
            before=snapshot();s.call('retry_policy_changed_through_original_API',0x8c647534,[0xffffffff]);harvest();intact(before)
        if kind=='progress':
            before=snapshot();s.call('progress_callback_original_API',0x8c647c6c,[config['progress']]);harvest();intact(before)
        if kind in ('bad','abort_bad'):responses=[(7,False,None)]
        # Retail word660bee contains251;it selects the65536 error at660c04.
        elif kind=='generic':responses=[(251,False,None)]
        elif kind in ('unplug','abort_unplug'):responses=[(8,True,None)]
        elif kind=='retry_exhaust':responses=[(7,False,None)]*2
        else:
            prefix=[(7,False,None)]*(2 if kind=='live_infinite' else 1 if kind=='retry_success' else 0)
            if kind=='protocol_retry':prefix=[(252,False,None)]
            responses=prefix+[(8,False,bytes((j*37+phase*17+s.channel)&255 for j in range(256))) for phase in range(2)]
        errors=0;copied=0;successful=kind in ('success','retry_success','live_infinite','protocol_retry','progress')
        expected_terminal=-2 if successful and config.get('progress')!=0x8cfef000 else -2 if kind=='progress' else 0
        # A null progress callback still reaches the missing filesystem parser.
        if kind=='progress' and not config['progress']:expected_terminal=-2
        for index,(status,unplug,data) in enumerate(responses):
            before=snapshot();pending=s.tokens()[0];counter=v.load(s.m,s.context+0xfe0)
            s.call('pending_root_block',v.AI_SCHEDULE);harvest()
            require(s.tokens()[0]==pending and v.load(s.m,s.context+0xfe0)==counter,'waiting creates no duplicate request')
            require(snapshot()[0]==before[0],'pending nested frame depth retained')
            transfer(status,unplug,data)
            s.resume();terminal=index==len(responses)-1;harvest(expected_terminal if terminal else 0)
            if status==7 or status==251 or unplug:errors+=1
            if data is not None:
                expected[copied*256:copied*256+256]=data;copied+=1
                require(v.sample(s.m,dest,512)==expected,'independent assembled root bytes')
            else:require(v.sample(s.m,dest,512)==expected,'failed response does not copy bytes')
        require(0x8c647646 in s.seen,'actual retry-reader continuation after block completion')
        early=kind=='abort_unplug';wanted_stats=stats_before if early else [(stats_before[0]+1)&0xffffffff,(stats_before[1]+errors)&0xffffffff]
        require([v.load(s.m,s.descriptor+o) for o in (0x30,0x34)]==wanted_stats,'original operation/error counters')
        progress=[b for result,_ in s.phases for b in result['boundaries'] if b['pr']=='0x8c647694' and b['target'] not in ('0x8c647bde',)]
        callback=config.get('progress',0x8c05180c)
        require(len(progress)==(0 if early or callback==0 else 1),'actual progress callback selection')
        if progress:
            require(progress[0]['target']==hex(callback) and progress[0]['arguments'][:4]==[s.channel,1,stats_before[0],wanted_stats[1]],'original progress callback arguments')
        if successful:
            stop=hex(callback) if kind=='progress' and callback else '0x8c64a4b2'
            if kind=='progress' and callback==0:stop='0x8c64a4b2'
            require(s.records[-1]['stop_pc']==stop,'actual next missing dependency')
            require(v.load(s.m,s.channel_record+8)==1 and v.load(s.m,s.result_word)==0,'command remains pending')
            require(not any(p['callback_events'] for p in s.records),'no fabricated completion callback')
            if stop=='0x8c64a4b2':require(0x8c64b21e in s.seen,'actual root gateway caller continuation')
        else:
            wanted_result=0xffffff01 if kind in ('bad','abort_bad','abort_unplug','retry_exhaust') else 0xffff0000
            require(v.load(s.m,s.result_word)==wanted_result,'original terminal command error')
            require(v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'worker cleared pending command')
            completions=[b for r in s.records for b in r['callback_events']]
            require(len(completions)==1 and completions[0]['arguments'][:3]==[s.channel,1,wanted_result],'original completion callback')
            s.call('completed_error_idle',v.AI_SCHEDULE);harvest()
            s.call('original_owner_cancel',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'original error teardown')
        seen.update(s.seen);edges.update(s.edges)
        records.append(dict(input=config,retry_budget=budget,abort_policy_input=abort,root_destination=hex(dest),stats_before=stats_before,stats_after=wanted_stats,independent_checks=count,phases=s.records,task_frames_never_repaired=True,completed_ids_never_injected=True))
        print('FILESYSTEM TASK',len(records),config,'comparisons',comparisons,flush=True)
    if smoke:print('FILESYSTEM TASK SMOKE PASS',len(records),comparisons,checks);return
    manifest=json.loads((P/'analysis/movie_filesystem_state_translation.json').read_text());coverage=[]
    for body in manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
    files=[Path(__file__),P/'movie_filesystem_state_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_state_task_fixtures.py',Path(v.__file__),P/'write_watch_filesystem_state.py',P/'analysis/movie_filesystem_state_translation.json',P/'analysis/movie_filesystem_state_connection.json',v.effects.DLL]
    report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original command1 task/header response/gateway/retrying block read with real submission and raw device observations. Success reads512bytes then stops at real64a4b2;errors complete through original progress/completion/idle/teardown. Explicit initial abort-policy global and original retry/callback APIs are inputs,not supplied callee results. Actual frame/token/counter preservation and byte/callback oracles. Original/native full machine/device comparisons;physical transport and complete filesystem/game remain unfinished. Table reader64ae62 remains a proposal until separately qualified.')
    (P/'analysis/movie_filesystem_state_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('FILESYSTEM TASK PASS',len(records),comparisons,checks,flush=True)
if __name__=='__main__':main()
