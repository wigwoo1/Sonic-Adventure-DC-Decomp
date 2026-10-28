"""Actual callback mutates the live record before its caller reloads offsets."""
import hashlib,itertools,json
from pathlib import Path
import movement_record_adjustment_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def vector(m,a):return tuple(q.number(q.get(m,a+4*j)) for j in range(3))
def main():
    ram=q.v.seed_ram();records=[];checks=0
    for index,mode,rm,velocity in itertools.product((0,1),(0,4),(0,1),((0,0,0),(3,4,12))):
        c=dict(kind='actual_callback_mutates_live_record',entry=q.OWNER,index=index,mode=mode,rm=rm,velocity=velocity,callback=q.IMPULSE)
        m,s,f,cfg=q.prepare(ram,c);record=q.RECORD+(0x1c if index else 0)
        # Initial pointer alias only: the original0751c0 stores at R4+20,
        # so its real writes change this record's translation at record+10.
        character=record-0x10;q.put(m,q.TABLE+index*4,character)
        before=bytes(m);initial=type(s).from_buffer_copy(s)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        def require(ok,label):
            nonlocal checks
            assert ok,(c,label);checks+=1
        require(result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match'],'complete original/native state and ordered boundaries')
        require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'normal return and balanced frame')
        old=vector(before,record+0x10);impulse=vector(before,record+0x3c)
        changed=tuple(a+3*b for a,b in zip(old,impulse))
        require(vector(m,record+0x10)==changed,'real original callback changes live record translation')
        origin=vector(before,q.A+0x20)
        start=tuple(a+b for a,b in zip(origin,vector(before,q.OUT))) if velocity==(0,0,0) else vector(before,q.C+4)
        require(vector(m,q.C+4)==tuple(a+b for a,b in zip(start,changed)),'caller reloads changed offsets after callback;no stale snapshot')
        require(vector(m,q.A+0x20)==origin,'temporary actor position restored')
        callback=[b for b in result['boundaries'] if b['target']==hex(q.IMPULSE)]
        require(len(callback)==1 and callback[0]['arguments'][:3]==[character,q.A,record],'actual callback receives aliased character/actor/live-record arguments')
        require(callback[0]['record_callback_input']['actor_position']==list(map(q.bits,start)),'callback sees actual temporary actor position')
        require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
        records.append(dict(configuration=c,character_pointer=hex(character),status=0,full_state_match=True,ordered_boundaries_match=True,boundaries=result['boundaries'],live_translation_before=old,live_translation_after=changed))
    files=[Path(__file__),P/'movement_record_adjustment_fixtures.py',P/'write_watch_movement_record_adjustment.py',P/'movement_record_adjustment_reference.py',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=len(records),cases=records,dll_sha256=digest(q.v.effects.DLL),new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,sha256={str(p):digest(p) for p in files},scope='Authored initial pointer alias with actual original0751c0 callback execution. Real callback stores mutate the record;07506e must reload its changed translation after restoring actor position. No retail callback assignment claimed;no supplied results/frames.')
    assert (len(records),checks)==(16,128)
    (P/'analysis/movement_record_adjustment_callback_alias_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('RECORD CALLBACK LIVE ALIAS PASS16comparisons/128checks',flush=True)
if __name__=='__main__':main()
