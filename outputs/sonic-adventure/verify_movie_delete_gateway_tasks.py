"""Original command7 guards after raw acquisition and a next-command frontier."""
import json,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];parent=P/'verify_movie_filesystem_write_tasks.py'
source=parent.read_text().split('        def command(label,begin,total):')[0].replace('movie_filesystem_write_vectors','movie_filesystem_persist_vectors').replace("'movie_filesystem_write.dll'","'movie_filesystem_persist.dll'")
source+='''
        baseline_buffers=[v.sample(s.m,a,n) for a,n in zip(destinations,(512,1024,1024))]
        event=None;pre=len(s.phases)
        if kind=='cache_clear':
            s.call('original_cache_invalidation_API',0x8c64850a,[channel]);harvest()
            require(v.sample(s.m,s.descriptor+8,5)==bytes(5),'original cache invalidation flags')
        if kind=='raw_absent':
            sp=v.load(s.m,s.task+0x18);frame=v.sample(s.m,sp,s.idle_sp+64-sp);contexts=v.sample(s.m,s.owner+0x210,32*0x124)
            require(s.tokens()[:2]==[0,0],'idle raw scan without a storage request')
            s.submit();harvest();require(v.load(s.m,s.context+0xfc0)==0,'actual primary scan submission')
            event=s.observe(status=7,unplug=True,ack=True);s.pump();harvest()
            require(v.sample(s.m,sp,len(frame))==frame and v.sample(s.m,s.owner+0x210,len(contexts))==contexts,'raw device scan preserves worker frames andcontexts')
            require(s.tokens()[:2]==[0,0],'raw removal creates no storage request')
        label=b'MISSING' if kind=='missing' else b'SONIC';raw(s.m,s.channel_record+0x2c,label+bytes(13-len(label)))
        expected_counter=v.load(s.m,v.load(s.m,0x8c649de4)+v.load(s.m,s.descriptor+4)*80)
        first=len(s.phases);opcode=8 if kind=='next_command8' else 7
        s.call('original_next_arguments_API',0x8c647cc4,[channel,channel,0,0],(0,0,0));harvest();s.call('original_next_opcode_API',0x8c647cb4,[channel,opcode]);harvest();r,trace=s.resume()
        for at,(phase,path) in enumerate(s.phases[done:]):
            wanted=-2 if kind=='next_command8' and at==len(s.phases)-done-1 else 0
            require(phase['status']==wanted and phase['full_state_match'] and phase['ordered_boundaries_match'],'full original/native completion or exact missing frontier');comparisons+=1
        done=len(s.phases)
        boundaries=[b for result,_ in s.phases[first:] for b in result['boundaries']];cb=[b for b in boundaries if b['pr']=='0x8c647bb4']
        if kind=='next_command8':
            require(r['stop_pc'] not in {b['target'] for b in cb} and not cb and v.load(s.m,s.channel_record+8)==8,'next missing dependency leaves original command pending')
            frontier=dict(pc=r['stop_pc'],pr=hex(s.s.pr),arguments=[hex(s.s.r[i]) for i in range(4,8)],pending=True)
        else:
            result=(-251 if kind=='missing' else -255)&0xffffffff
            require(v.load(s.m,s.result_word)==result and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c),'independent gateway result/idle')
            require(len(cb)==1 and cb[0]['arguments'][:3]==[channel,7,result],'one original error callback')
            require([v.sample(s.m,a,n) for a,n in zip(destinations,(512,1024,1024))]==baseline_buffers,'all acquired root/FAT/DIR bytes unchanged on rejected delete')
            require(v.load(s.m,s.descriptor+0x30)==1 and v.load(s.m,s.descriptor+0x34)==expected_counter,'independent initial progress and selected-slot counter')
            require(not any('write_request_input' in b for b in boundaries),'no write request for rejected command7')
            invalidations=[b['pr'] for b in boundaries if b['target']=='0x8c64850a']
            expected_invalidations=[] if kind=='missing' else (['0x8c649d64'] if kind=='raw_absent' else [])+['0x8c647b82']
            require(invalidations==expected_invalidations,'exact gateway and commonworker invalidation callsites')
            s.resume();harvest();s.call('original_completed_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual completed guard owner teardown')
            frontier=None
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,raw_device_event=event,frontier=frontier,phases=s.records,independent_checks=case_checks))
        print('DELETE GATEWAY TASK',len(records),kind,channel,comparisons,flush=True)
    old_manifest=P/'analysis/movie_filesystem_delete_translation.json';body=next(b for b in json.loads(old_manifest.read_text())['routines'] if b['address']=='0x8c649d20')
    missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
    files=[Path(__file__),parent,old_manifest,P/'movie_filesystem_persist_vectors.py',P/'movie_filesystem_persist_session.py',P/'movie_filesystem_persist_reference.py',P/'write_watch_filesystem_persist.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_persist_connection.json',v.effects.DLL]
    report=dict(original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=[dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes))],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original rawheader/root/FAT/linkedDIR acquisition then actualcommand7 API/scheduler. Missingname, originalcache-invalidation API andprimaryscan rawremoval yieldindependenterrors/callback/idle/teardown withoutwrites. Rawscan changes noframes/contexts/IDs. Command8 actualfrontier remains pending without inventedcompletion ordestruction. No newnativebuild orcode;existing233DLL used. Naturalbusy/admission/latch acquisition notclaimed.')
    name='movie_delete_gateway_tasks'+('' if shard is None else '_'+str(shard))+'_verification.json'
    (P/'analysis'/name).write_text(json.dumps(report,indent=2)+'\\n');print('DELETE GATEWAY TASK PASS',comparisons,checks,flush=True)
'''
ns=dict(__name__='gateway_task_replay',__file__=str(Path(__file__)),parent=parent);exec(compile(source,str(Path(__file__)),'exec'),ns)
def configs():
    for channel in (0,7):
        for kind in ('missing','cache_clear','raw_absent','next_command8'):yield dict(kind=kind,channel=channel,order=int(channel==7),endian=int(channel==7),count=1,start=0)
ns['configurations']=configs
if __name__=='__main__':ns['main']()
