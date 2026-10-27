"""Observe the next real command20 boundary; no routine qualification credit."""
import hashlib,json
from pathlib import Path
from movie_filesystem_directory_session import Session,v
P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    assert v.effects.DLL.name=='movie_filesystem_directory.dll'
    binary=(P.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    def word(a,n=4,signed=False):return int.from_bytes(binary[a-0x8c010000:a-0x8c010000+n],'little',signed=signed)
    assert 0x8c6479b8+word(0x8c6479c8+40,2,True)==0x8c647b52 and word(0x8c647c40)==0x8c64bcd6
    records=[];checks=comparisons=0;ram=v.seed_ram()
    for channel in (0,7):
        s=Session(ram,channel=channel,command=20);s.queue()
        for i,(r,path) in enumerate(s.phases):
            assert r['status']==(-2 if i==len(s.phases)-1 else 0);assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=3;comparisons+=1
        r=s.phases[-1][0]
        assert r['stop_pc']=='0x8c64bcd6' and s.records[-1]['targets'][-1]=='0x8c64bcd6';checks+=2
        assert s.tokens()==[0,0,0] and not any(r['callback_events'] for r in s.records);checks+=2
        assert s.s.r[4]==channel and s.s.r[5]==s.channel_record+0x2c and s.s.pr==0x8c647b68;checks+=3
        records.append(dict(channel=channel,phases=s.records,arguments=[hex(x) for x in s.s.r[4:8]],pr=hex(s.s.pr),source_wait_limits_added=0,frames_or_results_repaired=False))
    files=[Path(__file__),P/'movie_filesystem_directory_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_directory_task_fixtures.py',Path(v.__file__),v.effects.DLL]
    report=dict(original_native_comparisons=comparisons,independent_checks=checks,cases=records,missing_callee='0x8c64bcd6',new_routine_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original command20 API andactual active scheduler stop at first missing64bcd6 onchannels0/7. Static original dispatch-table/pointer bytecheck. No header/transportresponse,parserresult,taskframe orcallbackresult supplied. Boundary observation only;callee behavior unqualified.')
    (P/'analysis/movie_filesystem_directory_frontier_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('DIRECTORY FRONTIER PASS',comparisons,checks,flush=True)
if __name__=='__main__':main()
