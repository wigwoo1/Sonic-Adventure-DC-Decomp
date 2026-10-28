"""Real vector conversion, response and conversion calls on retained RAM/FP state."""
import hashlib,itertools,json
from pathlib import Path
import movement_vectors_fixtures as fixture
P=fixture.P


def main():
    ram=fixture.v.seed_ram();records=[];checks=0;orientation=0x8cb50000
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    for converter,mode in itertools.product((0x8c079000,0x8c078eb0),(0,4)):
        m,s,f,cfg=fixture.prepare(ram,dict(entry=0x8c074cf0,mode=mode,flags=0x4000 if converter==0x8c079000 else 0,
                                         velocity=(0,1,0),normal=(0,1,0)))
        fixture.vector(m,orientation,(0,1,0))
        for offset in (0x14,0x18,0x1c):fixture.put(m,orientation+offset,0)
        stages=((converter,[orientation,fixture.C+0x1c,0]),
                (0x8c074cf0,[fixture.A,fixture.B,fixture.C,fixture.OUT]),
                (converter,[orientation,fixture.OUT,0]))
        initial_depth=fixture.get(m,0x8c88f69c);initial_stack=fixture.get(m,0x8c88f5f8)
        for entry,args in stages:
            cfg['entry']=entry;s.pc=entry;s.pr=0x8cfed000
            for j,arg in enumerate(args):s.r[4+j]=arg
            initial_sp=s.r[15]
            result,path,m,s,f=fixture.fast.compare(m,s,f,cfg)
            require(result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match'],'actual sequence stage complete/full-state')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==initial_sp,'ordinary call return/stack')
            require(fixture.get(m,0x8c88f69c)==initial_depth and fixture.get(m,0x8c88f5f8)==initial_stack,'matrix stack restored across each actual converter')
            records.append(dict(converter=hex(converter),mode=mode,entry=hex(entry),status=0,original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        expected=fixture.bits(7) if converter==0x8c078eb0 else fixture.bits(8-fixture.number(0x3e4ccccc))
        require([fixture.get(m,fixture.OUT+4*j) for j in range(3)]==[0,expected,0],'independent identity-orientation response sequence result')
    files=[Path(__file__),P/'movement_vectors_fixtures.py',P/'movement_vectors_reference.py',P/'write_watch_movement_vectors.py',fixture.v.MP,P/'analysis/movement_vectors_connection.json',fixture.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,
      dll_sha256=hashlib.sha256(fixture.v.effects.DLL.read_bytes()).hexdigest(),supplied_game_callee_outputs=0,
      supplied_execution_frames=0,new_original_routine_credit=0,
      sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
      scope='Four actual converter->response->converter sequences on same RAM/FP state, two original converter entries and both FP banks. '
      'Identity orientation/zero integer angles; real matrix/scalar/response calls. No callee outputs or execution frames supplied. '
      'Host starts ordinary calls with arguments. This checks the helper sequence,not complete0730d8 execution, '
      'natural player/query initialization,fullmovement/gameplay or frame timing.')
    (P/'analysis/movement_vectors_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('MOVEMENT SEQUENCE PASS',len(records),checks,flush=True)


if __name__=='__main__':main()
