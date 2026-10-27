"""Locate the next real worker dependency after raw filesystem initialization."""
import hashlib,itertools,json
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_movie_filesystem_write_tasks.py'
source=parent.read_text();marker='        def command(label,begin,total):';assert source.count(marker)==1
source=source.split(marker)[0]+'''
        raw(s.m,s.channel_record+0x2c,b'SONIC'+bytes(8))
        first=len(s.phases)
        s.call('command7_arguments_api',0x8c647cc4,[channel,channel,0,0],(0,0,0))
        s.call('command7_opcode_api',0x8c647cb4,[channel,7])
        r,path=s.resume()
        for index,(phase,trace) in enumerate(s.phases[done:]):
            require(phase['status']==(-2 if index==len(s.phases)-done-1 else 0),'actual missing-callee status only at frontier')
            require(phase['full_state_match'] and phase['ordered_boundaries_match'],'complete original/native frontier state/order');comparisons+=1
        require(r['stop_pc']=='0x8c649d20' and s.s.pr==0x8c647a70,'independent actual command7 callee/return')
        require(list(s.s.r)[4:6]==[channel,s.channel_record+0x2c],'actual command7 argument ABI')
        require(v.load(s.m,s.channel_record+8)==7 and not any(b['pr']=='0x8c647bb4' for result,_ in s.phases[first:] for b in result['boundaries']),'pending command retained;no invented completion')
        require(v.sample(s.m,s.channel_record+0x2c,13)==b'SONIC'+bytes(8),'caller filename retained')
        records.append(dict(configuration=config,stop_pc=r['stop_pc'],return_pc=hex(s.s.pr),arguments=list(s.s.r)[4:6],phases=s.records,independent_checks=case_checks))
        print('COMMAND7 FRONTIER',channel,order,endian,comparisons,flush=True)
    files=[Path(__file__),parent,P/'movie_filesystem_write_session.py',P/'movie_filesystem_write_reference.py',P/'movie_filesystem_write_task_fixtures.py',P/'write_watch_filesystem_write.py',P/'filesystem_write_reference_cpu.py',v.effects.DLL]
    report=dict(status='verified_actual_missing_command7_frontier',original_task_sessions=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,next_original_dependency='0x8c649d20',actual_parent_return='0x8c647a70',dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Two actual constructor/rawheader/root/FAT/directory initialization sessions beforeoriginalcommand7 caller APIs andscheduler668e60. Original/native execution bothstop-2 atactualmissing649d20,PR647a70,arguments[channel,originalfilenamebuffer]. No callee output,taskframe,PR,ID or completion supplied. Initialization callback completedearlier;command7remains pending,nocompletecommand7/teardown/wholefunctioncredit.')
    (P/'analysis/movie_filesystem_command7_frontier_verification.json').write_text(json.dumps(report,indent=2)+'\\n');print('COMMAND7 FRONTIER PASS',comparisons,checks,flush=True)
'''
ns=dict(__name__='command7_frontier',__file__=str(Path(__file__)),parent=parent)
exec(compile(source,str(parent),'exec'),ns)
ns['configurations']=lambda:iter([dict(kind='frontier',channel=0,order=0,endian=0,count=1,start=0),dict(kind='frontier',channel=7,order=1,endian=1,count=1,start=0)])
if __name__=='__main__':ns['main']()
