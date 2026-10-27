"""Real command7 initialization, RAM removal and original persistence frontier."""
import json,itertools
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_movie_filesystem_write_tasks.py'
source=parent.read_text();marker='        def command(label,begin,total):';assert source.count(marker)==1
source=source.split(marker)[0].replace('from movie_filesystem_write_vectors','from movie_filesystem_delete_vectors').replace("'movie_filesystem_write.dll'","'movie_filesystem_delete.dll'")
source=source.replace("b[24:26]=(2).to_bytes(2,byte_order)","b[24:26]=count.to_bytes(2,byte_order)")
marker="        directory=entry(0xcc,b'SONIC')+entry(0x33,b'DATA')+bytes(960)";assert source.count(marker)==1
source=source.replace(marker,"        index=config['index'];directory=bytes(32*index)+entry(0xcc,b'SONIC')+bytes(1024-32*(index+1))")
marker="        def entry(type_,label):";assert source.count(marker)==1
source=source.replace(marker,"        if count==1:fat[40:42]=(0xfffa).to_bytes(2,byte_order)\n        if count==3:\n            fat[42:44]=(22).to_bytes(2,byte_order);fat[44:46]=(0xfffa).to_bytes(2,byte_order)\n"+marker)
source+='''
        dirty=v.sample(s.m,s.descriptor+0x48,32)
        raw(s.m,s.channel_record+0x2c,(b'MISSING' if kind=='missing' else b'SONIC')+bytes(6 if kind=='missing' else 8))
        first=len(s.phases)
        s.call('command7_arguments_api',0x8c647cc4,[channel,channel,0,0],(0,0,0));s.call('command7_opcode_api',0x8c647cb4,[channel,7]);r,path=s.resume()
        pending=kind!='missing'
        for at,(phase,trace) in enumerate(s.phases[done:]):
            require(phase['status']==(-2 if pending and at==len(s.phases)-done-1 else 0),'actual host frontier/completion status')
            require(phase['full_state_match'] and phase['ordered_boundaries_match'],'complete original/native state and effects');comparisons+=1
        done=len(s.phases)
        directory_after=bytearray(directory);fat_after=bytearray(fat);dirty_after=bytearray(dirty)
        if pending:
            directory_after[index*32:(index+1)*32]=bytes(32);dirty_after[index>>4]=1
            if kind!='bad_chain':
                for head in range(20,20+count):fat_after[head*2:head*2+2]=(0xfffc).to_bytes(2,byte_order)
            require(r['stop_pc']=='0x8c64ab36' and s.s.pr==0x8c649db6 and s.s.r[4]==channel,'actual next persistence dependency and parent return/argument')
            require(v.load(s.m,s.channel_record+8)==7 and not any(b['pr']=='0x8c647bb4' for result,_ in s.phases[first:] for b in result['boundaries']),'command pending without invented completion')
        else:
            cb=[b for result,_ in s.phases[first:] for b in result['boundaries'] if b['pr']=='0x8c647bb4']
            require(v.load(s.m,s.result_word)==0xffffff05 and v.sample(s.m,s.channel_record,0x2c)==bytes(0x2c) and len(cb)==1 and cb[0]['arguments'][:3]==[channel,7,0xffffff05],'original missing-name result/callback/idle')
        require(v.sample(s.m,destinations[0],512)==root,'original root buffer unchanged')
        require(v.sample(s.m,destinations[1],1024)==fat_after,'independent RAM FAT free or preservation')
        require(v.sample(s.m,destinations[2],1024)==directory_after and v.sample(s.m,s.descriptor+0x48,32)==dirty_after,'independent record clear and exact dirty byte')
        reads=[b['arguments'][:3] for result,_ in s.phases for b in result['boundaries'] if b['target']=='0x8c660a7e'];require(reads==initial_reads,'no extra reads from RAM mutation')
        require(s.tokens()[0:2]==[0,0] and not any('write_request_input' in b for result,_ in s.phases[first:] for b in result['boundaries']),'no fabricated persistence or write request before missing writer')
        if not pending:
            s.resume();harvest();s.call('missing_name_owner_teardown',v.OWNER_ALL);harvest();require(v.sample(s.m,s.owner,0x2694)==bytes(0x2694),'actual completed error teardown')
        seen.update(s.seen);edges.update(s.edges);records.append(dict(configuration=config,pending_persistence=pending,stop_pc=r['stop_pc'],phases=s.records,independent_checks=case_checks))
        print('DELETE TASK',len(records),kind,channel,order,endian,comparisons,flush=True)
    coverage=[]
    for body in v.manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};outcomes=set(map(tuple,body['conditional_edges']))-edges
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
    files=[Path(__file__),parent,P/'movie_filesystem_delete_session.py',P/'movie_filesystem_delete_reference.py',P/'movie_filesystem_delete_task_fixtures.py',P/'write_watch_filesystem_delete.py',P/'filesystem_write_reference_cpu.py',v.MP,P/'analysis/movie_filesystem_delete_connection.json',v.effects.DLL]
    report=dict(status='partially_verified_command7_RAM_removal_persistence_missing',original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,coverage_by_routine=coverage,next_original_dependency='0x8c64ab36',actual_parent_return='0x8c649db6',dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Eight original rawheader/root/FAT/linked-directory acquisitions followedbycommand7 APIs/scheduler. Independent RAM record/FAT/dirty/root buffers,callback/error/idle/teardown andnoadditionalI/O assertions. Missingname completesoriginal-251;othercases reachactualmissing64ab36 PR649db6 withRAMchangesnotpersisted. No parsedrecord/calleeoutput/frame/ID/PR orcallbackreturn supplied. No persisteddeletion/fullcommand7/physicalhardware/gameclaim.')
    (P/'analysis/movie_filesystem_delete_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\\n');print('DELETE TASK PASS',comparisons,checks,flush=True)
'''
ns=dict(__name__='delete_task_replay',__file__=str(Path(__file__)),parent=parent);exec(compile(source,str(parent),'exec'),ns)
def configs():
    kinds=['valid_two','missing','bad_chain','zero_count','index16','one','three','valid_two']
    for i,(channel,order,endian) in enumerate(itertools.product((0,7),(0,1),(0,1))):
        kind=kinds[i];yield dict(kind=kind,channel=channel,order=order,endian=endian,count={'zero_count':0,'one':1,'three':3}.get(kind,2),start=0,index=16 if kind=='index16' else 0)
ns['configurations']=configs
if __name__=='__main__':ns['main']()
