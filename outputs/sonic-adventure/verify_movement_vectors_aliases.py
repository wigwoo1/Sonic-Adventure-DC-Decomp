"""Normal-entry output/input overlaps; no disjointness assumption in predicate proofs."""
import hashlib,itertools,json,types
from pathlib import Path
import movement_vectors_fixtures as fixture
P=fixture.P


def main():
    ram=fixture.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    for out,mode in itertools.product((fixture.OUT,fixture.A,fixture.B+0x58,fixture.C+0x18),(0,4)):
        prepare=types.FunctionType(fixture.prepare.__code__,dict(fixture.prepare.__globals__,OUT=out),'prepare_overlapping_output')
        m,s,f,cfg=prepare(ram,dict(entry=0x8c074cf0,mode=mode));initial_sp=s.r[15]
        initial_hash=hashlib.sha256(m).hexdigest()
        result,path,actual,s,f=fixture.fast.compare(m,s,f,cfg)
        require(result['status']==0,'real response completes under authored alias')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'whole original/native state and boundaries')
        require(s.pc==s.pr==0x8cfed000 and s.r[15]==initial_sp,'normal entry/return,not an injected execution frame')
        require(fixture.get(actual,out)==fixture.get(actual,out+8)==0,'real horizontal output stores')
        records.append(dict(output_address=hex(out),mode=mode,input_ram_sha256=initial_hash,status=0,
                            original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    files=[Path(__file__),P/'movement_vectors_fixtures.py',P/'movement_vectors_reference.py',P/'write_watch_movement_vectors.py',fixture.v.MP,P/'analysis/movement_vectors_connection.json',fixture.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,
      dll_sha256=hashlib.sha256(fixture.v.effects.DLL.read_bytes()).hexdigest(),supplied_game_callee_outputs=0,
      supplied_execution_frames=0,new_original_routine_credit=0,
      sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
      scope='Response output overlaps point input, state flags or query normal;paired FP banks and disjoint control. '
      'Relocate authored OUTPUT INPUT buffer before execution;all aliases are actual initial RAM layouts. '
      'Original stores/loads/callees execute,not manufactured callee outputs. Full-state comparison and horizontal '
      'output assertions,not independent complete physics or natural gameplay initialization evidence.')
    (P/'analysis/movement_vectors_aliases_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('MOVEMENT ALIASES PASS',len(records),checks,flush=True)


if __name__=='__main__':main()
