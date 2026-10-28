"""Prove why pristine original preflight cannot take its negative-result edge."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';b=binary.read_bytes();base=0x8c010000
    word=lambda a:int.from_bytes(b[a-base:a-base+2],'little')
    long=lambda a:int.from_bytes(b[a-base:a-base+4],'little')
    assert (word(0x8c64b2dc),word(0x8c64b2de))==(0x000b,0xe000)
    assert long(0x8c649f00)==0x8c64b2dc and word(0x8c649e8e)==0x420b
    assert word(0x8c649e92)==0x4011 and word(0x8c649e94)==0x8b21
    bridge=P/'source/sa_movie_filesystem_command17_connected.c'
    assert 'if(target==0x8c64b2dcu)return sa_movie_filesystem_delete_8c64b2dc(' in bridge.read_text()
    previous=P/'analysis/movie_filesystem_delete_direct_verification.json';r=json.loads(previous.read_text())
    cases=[c for c in r['cases'] if c['input']['kind']=='canonical_zero']
    assert len(cases)==8
    files=[Path(__file__),binary,bridge,previous,P/'verify_movie_filesystem_delete_direct.py',P/'analysis/movie-filesystem-delete/8c64b2dc_instructions.tsv',P/'source/sa_movie_filesystem_delete.c']
    report=dict(status='structurally_unreachable_negative_preflight_with_pristine_original_code',excluded_outcome=['0x8c649e94','0x8c649eda'],excluded_rows=[],original_helper='0x8c64b2dc',original_helper_bytes=b[0x8c64b2dc-base:0x8c64b2e0-base].hex(),literal='0x8c649f00',call='0x8c649e8e',return_pc='0x8c649e92',canonical_duplicate_credit=0,new_routine_credit=0,historical_canonical_zero_cases=8,historical_cases_not_new_comparisons=True,reason='Original helper is RTS with MOV#0,R0 in its delay slot. It always returns R0=0,without any callbacks or loads. Caller CMP/PZ R0 therefore setsT=1;immediate BF cannot branch to649eda. Current bridge calls the preserved original-instruction adapter;historical direct cases independently checked exactR0/CPU/RAM in8profiles. No suppliedcalleeoutput orchangedhelper is used to manufacture negativecoverage. Aggregate-delete-error path still reaches649eda,so no instruction row is excluded.',limitations=['Pristine original instructions and normal completed-call ABI only;self-modifying code or corrupt return state is not asserted.','This proves one branch outcome impossible,not physical storage/full filesystem/game correctness.'],sha256={str(f):digest(f) for f in files})
    (P/'analysis/movie_filesystem_command17_zero_preflight_proof.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND17 PREFLIGHT NEGATIVE EDGE PROVED UNREACHABLE;0routinecredit')
if __name__=='__main__':main()
