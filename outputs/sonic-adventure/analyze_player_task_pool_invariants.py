"""Bind fixed-bound/register-counter induction to every original loop instruction."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    listing=P/'analysis/player-work-allocation/8c09844a_instructions.tsv'
    rows={int(x['address'],16):x for x in csv.DictReader(listing.open(),delimiter='\t')}
    assert len(rows)==114
    text={a:x['instruction'].lstrip('_') for a,x in rows.items()}
    for a,x in rows.items():assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(x['bytes'])
    literal=lambda a:int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')
    assert literal(0x8c0985a4)==0x30000
    assert literal(0x8c0985a8)==0x8c79f044 and literal(0x8c0985ac)==0x8c79f048
    head=literal(0x8c0985b0);assert head==0x8c79f04c and head+32<=0xffffffff
    header={0x8c098450:'mov.l 0x8c0985a4,r4',0x8c098452:'bsr 0x8c098a80',0x8c09845c:'tst r3,r3',0x8c098460:'bt/s 0x8c098526',
            0x8c098464:'mov.l 0x8c0985b0,r4',0x8c098466:'mov r4,r7',0x8c098468:'add #0x20,r7',0x8c09846a:'mov r4,r5',
            0x8c09846c:'cmp/hs r7,r5',0x8c09846e:'bt/s 0x8c09847e',0x8c098470:'mov #0x0,r4',
            0x8c098472:'mov.l r4,@r5',0x8c098474:'add #0x4,r5',0x8c098476:'mov.l r4,@r5',0x8c098478:'add #0x4,r5',0x8c09847a:'cmp/hs r7,r5',0x8c09847c:'bf 0x8c098472',
            0x8c09847e:'mov #0x10,r12',0x8c098480:'mov.l @r6,r6',0x8c098482:'mov #0x10,r11',0x8c098484:'mov r4,r1',0x8c098486:'shll8 r12',
            0x8c098488:'mov r6,r5',0x8c09848a:'add #-0x1,r12',0x8c09848c:'mov r4,r7',0x8c09848e:'shll8 r11'}
    for a,t in header.items():assert text[a]==t,hex(a)
    # The fixed unsigned interval cannot wrap. The first CMP/HS is false;
    # four two-word bodies advance R5 by8 from head to head+32.
    assert head<head+32
    for j in range(4):assert head+8*j<head+32 and head+8*(j+1)<=head+32
    assert head+8*4==head+32
    loop={a:t for a,t in text.items() if 0x8c098490<=a<=0x8c098524}
    stores=re.compile(r'mov\.l r[01456],@(?:r[016]|\(0x(?:4|8|c|10|14|18|1c|20|24|28|2c),r[016]\))')
    register_moves={'mov r4,r5','mov r5,r1','mov r5,r0','mov r5,r6','mov r6,r1'}
    comparisons={0x8c098490,0x8c09849e,0x8c0984c0,0x8c0984e4,0x8c098508}
    branches={0x8c098492,0x8c0984b6,0x8c0984da,0x8c0984fe,0x8c098522}
    increments={0x8c098498,0x8c0984bc,0x8c0984e0,0x8c098504}
    assert {a for a,t in loop.items() if t=='add #0x1,r7'}==increments
    assert {a for a,t in loop.items() if t.startswith('cmp/')}==comparisons
    assert {a for a,t in loop.items() if t.startswith('bf/s ')}==branches
    for a,t in loop.items():
        assert (stores.fullmatch(t) or t in register_moves or t in ('add #0x1,r7','add #0x30,r5','cmp/ge r12,r7','cmp/ge r11,r7') or a in branches), (hex(a),t)
    exact={0x8c098490:'cmp/ge r12,r7',0x8c098492:'bf/s 0x8c098498',0x8c098494:'add #0x30,r5',0x8c098496:'mov r4,r5',
           0x8c09849e:'cmp/ge r12,r7',0x8c0984b6:'bf/s 0x8c0984bc',0x8c0984b8:'add #0x30,r5',0x8c0984ba:'mov r4,r5',
           0x8c0984c0:'cmp/ge r12,r7',0x8c0984da:'bf/s 0x8c0984e0',0x8c0984dc:'add #0x30,r5',0x8c0984de:'mov r4,r5',
           0x8c0984e4:'cmp/ge r12,r7',0x8c0984fe:'bf/s 0x8c098504',0x8c098500:'add #0x30,r5',0x8c098502:'mov r4,r5',
           0x8c098508:'cmp/ge r11,r7',0x8c098522:'bf/s 0x8c098490',0x8c098524:'mov r5,r6'}
    for a,t in exact.items():assert text[a]==t,hex(a)
    for compare,branch in zip(sorted(comparisons),sorted(branches)):
        assert all(t.startswith(('mov ','mov.l ')) for a,t in loop.items() if compare<a<branch)
    assert len([a for a,t in text.items() if t.startswith(('jsr ','bsr '))])==1
    # The accepted instruction grammar contains no memory loads,calls,or writes
    # to R4/R11/R12. Only the four ADDs can write R7. Stores and MOVs between
    # comparisons and branches do not alter T. ADD-immediate also preserves T.
    # Thus normal entry establishes the following induction,independently of
    # the allocated pointer or RAM contents,with immutable instructions.
    for iteration in range(1024):
        n=4*iteration
        assert 0<=n<=4092 and n%4==0
        assert all(n+phase<4095 for phase in (0,1,2))
        assert (n+3>=4095)==(iteration==1023)
        assert (n+4<4096)==(iteration<1023)
        assert 0<=n+4<=4096<0x80000000
    dead_rows=['0x8c098496','0x8c0984ba','0x8c0984de']
    dead_edges=[['0x8c09846e','0x8c09847e'],['0x8c098492','0x8c098496'],['0x8c0984b6','0x8c0984ba'],['0x8c0984da','0x8c0984de']]
    source=P/'source/sa_player_work_allocation.c';code=source.read_text()
    assert all('L'+a[2:]+':' in code for a in dead_rows)
    assert 'SA_BOUND_RETAIL_STALLS' not in code
    files=[Path(__file__),original,listing,source,P/'analysis/player_work_allocation_translation.json']
    result=dict(status='source_bound_fixed_pointer_interval_and_register_counter_invariants_passed',original_entry='0x8c09844a',original_rows_byte_checked=114,
                source_bound_infeasible_rows=dead_rows,source_bound_infeasible_outcomes=dead_edges,
                pointer_interval=dict(start=hex(head),end_exclusive=hex(head+32),word_count=8,zeroing_loop_iterations=4),
                loop_iterations=1024,task_count=4096,counter_phase_values={'098492':'4j','0984b6':'4j+1','0984da':'4j+2','0984fe':'4j+3','098522':'4j+4'},
                induction='R4=0,R7=0,R12=4095,R11=4096 before loop. Four phases add exactly1 toR7 each;no otherR7writes,noR4/R11/R12writes,no memory loads or calls inside. At outer iterationj=0..1023,firstthree compares see4j,4j+1,4j+2<4095. Fourthcompare reaches4095 only inlastiteration,setsfinalnextlinknull. Loop exitsat4096 without signed wrap. Fixedunsigned scheduler-head interval startsbelow itsnonwrappingend,soinitialskip cannotoccur.',
                premises=['Normal original function entry,not an arbitrary nonlocal resume.','Original instructions and literal words remain unchanged throughout execution.','Synchronous supported coherent-RAM software semantics;no asynchronous modification of registers or T.','Faults,callee stops,corrupted returns and nonlocal control transfers are not normal completed executions.'],
                private_memory_counter_alias_premise_required=False,all_loop_counters_are_registers=True,physical_hardware_or_full_game_proven=False,
                runtime_code_changed=False,guards_removed=0,new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_task_pool_invariants.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('PLAYER TASK POOL INVARIANTS PASS:114 original rows bound;3rows/4outcomes infeasible in normal immutable synchronous execution',flush=True)
if __name__=='__main__':main()
