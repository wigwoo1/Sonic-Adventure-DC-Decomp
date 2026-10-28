"""Live selector/table overlaps and an actual output-produced subsequent mode."""
import hashlib,itertools,json
from pathlib import Path
import movement_parameters_fixtures as q
from verify_movement_parameters_direct import expected_fields,expected_scalar
P=q.P

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(name,mode,view,label);checks+=1
    cases=[
      ('selector_first_store',dict(selector_address=q.STATE+0x114-9,table_words=[dict(index=0,offset=0x78,word=0x3f800001)]),[0x3f800001,q.bits(9),q.bits(17)]),
      ('selector_second_store',dict(selector_address=q.STATE+0xf4-9,table_words=[dict(index=0,offset=0x58,word=0x3f800002)]),[q.bits(4),0x3f800002,q.bits(18)]),
      ('selector_flag_OR',dict(selector_address=q.STATE+0x5c-9,flags=4),[q.bits(8),q.bits(12),q.bits(20)]),
      ('scalar_overwrites_first_field',dict(output_address=q.STATE+0x114-0x3c,flags=8),[0x3e199999,q.bits(8),q.bits(16)]),
      ('first_store_changes_second_table_read',dict(state_address=q.TABLE+0x58-0x114),[q.bits(4),q.bits(4),q.bits(16)]),
      ('second_store_changes_third_table_read',dict(state_address=q.TABLE+0x3c-0xf4),[q.bits(4),q.bits(8),q.bits(8)]),
      ('same_records',dict(selector_address=q.STATE,output_address=q.STATE),[q.bits(4),q.bits(8),q.bits(16)])]
    for (name,inputs,expected),mode,view in itertools.product(cases,(0,4),(0x8c000000,0xac000000)):
        c=dict(inputs,mode=mode,view=view);m,s,f,cfg=q.prepare(ram,c);sp=s.r[15];state=c.get('state_address',q.STATE)
        r,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'complete original/native full state and boundaries')
        require([b['target'] for b in r['boundaries']]==[hex(q.ENTRY)],'original leaf executes without game callee substitution')
        require(s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'normal entry/return and stack')
        require([q.get(m,state+off) for off in (0x114,0xf4,0xd8)]==expected,'independent exact overlap result')
        require(q.get(m,c.get('output_address',q.OUTPUT)+0x3c)==expected_scalar(c.get('flags',0)),'independent scalar priority')
        records.append(dict(name=name,mode=mode,view=hex(view),inputs=inputs,status=0,original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    # First original call writes1.5 into the actual state-mode word through R5.
    # The next original normal-entry call consumes that untouched produced word,
    # selecting the400000 scaling path. No intermediate result/frame is supplied.
    for mode,view in itertools.product((0,4),(0x8c000000,0xac000000)):
        name='produced_mode_sequence';c=dict(mode=mode,view=view,flags=128,output_address=q.STATE+0x1e4-0x3c)
        m,s,f,cfg=q.prepare(ram,c)
        for phase in range(2):
            s.pc=q.ENTRY;s.pr=0x8cfed000;before=bytes(m);sp=s.r[15]
            state_flags=q.get(before,q.STATE+0x1e4)
            require(state_flags==(0 if phase==0 else 0x3fc00000),'prior actual scalar store produces next mode')
            expected=expected_fields(before,0,state_flags,0)
            r,path,m,s,f=q.fast.compare(m,s,f,cfg)
            require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'actual produced-state consumer matches original/native full state')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'second normal entry/return without supplied frame')
            require([q.get(m,q.STATE+off) for off in (0x114,0xf4,0xd8)]==expected,'independent finite produced-mode fields')
            require(q.get(m,q.STATE+0x1e4)==0x3fc00000,'original scalar writes the next observed state mode')
            records.append(dict(name=name,mode=mode,view=hex(view),phase=phase,consumed_mode=hex(state_flags),status=0,original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    files=[Path(__file__),P/'verify_movement_parameters_direct.py',P/'movement_parameters_fixtures.py',P/'movement_parameters_reference.py',P/'write_watch_movement_parameters.py',q.v.MP,P/'analysis/movement_parameters_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,overlap_cases=28,actual_produced_mode_sequences=4,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},
      scope='Seven initial pointer/table layouts in both FP banks andP1/P2RAM views. Independent exact fields expose repeated selector reloads, initial flagOR, table reads after stores, scalar overwrite and shared records. Four actual two-call sequences consume the first scalar store as next mode. No intermediate game outputs/frames. Authored input layouts, not natural gameplay initialization or physicalMMU/FPU proof.')
    (P/'analysis/movement_parameters_aliases_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('PARAMETERS ALIASES PASS',len(records),checks,flush=True)

if __name__=='__main__':main()
