"""Byte-backed ordinary-entry proof of contradictory owner response gate predicates."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/movement_response_owner_gate_proof.json';assert not target.exists()
    manifest=P/'analysis/player_movement_owner_translation.json';t=json.loads(manifest.read_text());b=t['routines'][0]
    listing=Path(b['listing']);rows={int(x['address'],16):x for x in csv.DictReader(listing.open(),delimiter='\t')}
    raw=(R/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    for a,row in rows.items():assert bytes.fromhex(row['bytes'])==raw[a-0x8c010000:a-0x8c010000+2] and int(row['length'])==2
    assert len(rows)==629
    text={a:row['instruction'].lstrip('_') for a,row in rows.items()}
    delayed={a for a,row in rows.items() if row['instruction'].startswith('_')}
    nodes=set(rows)-delayed;succ={};branch_targets=set();conditional_count=0
    for a in nodes:
        op=text[a]
        if op.startswith(('bt ','bf ','bt/s ','bf/s ')):
            destination=int(op.split()[-1],16);branch_targets.add(destination);conditional_count+=1
            succ[a]={destination,a+(4 if '/s ' in op else 2)}
        elif op.startswith('bra '):
            destination=int(op.split()[-1],16);branch_targets.add(destination);succ[a]={destination}
        elif op.startswith(('bsr ','jsr ')):succ[a]={a+4}
        elif op=='rts':succ[a]=set()
        else:
            assert not op.startswith(('jmp ','braf ','bsrf ','rte')),op
            succ[a]={a+2}
        assert succ[a]<=nodes,(hex(a),sorted(map(hex,succ[a]-nodes)))
    assert not branch_targets&delayed
    assert conditional_count*2==92
    expected={0x8c072de0:'mov r5,r0',0x8c072de2:'cmp/eq #0x4,r0',0x8c072de4:'bt 0x8c072dea',
              0x8c072de6:'bra 0x8c072fee',0x8c072dea:'mov r5,r0',0x8c072dec:'cmp/eq #0x5,r0',
              0x8c072dee:'bt 0x8c072df4',0x8c072df0:'bra 0x8c072fee',0x8c072e84:'bsr 0x8c072946'}
    for a,op in expected.items():assert text[a]==op,(hex(a),text[a])
    assert succ[0x8c072de4]=={0x8c072dea,0x8c072de6}
    assert succ[0x8c072dee]=={0x8c072df4,0x8c072df0}
    # The first taken comparison binds the full unchanged R5 value to exactly4.
    # No call,instruction writing R5,or delay slot lies on the second comparison.
    assert [text[a] for a in (0x8c072dea,0x8c072dec,0x8c072dee)]==['mov r5,r0','cmp/eq #0x5,r0','bt 0x8c072df4']
    only_value_after_first_taken_gate=4
    assert only_value_after_first_taken_gate!=5
    def reachable(blocked=None):
        seen=set();todo=[0x8c072b44]
        while todo:
            a=todo.pop()
            if a in seen:continue
            seen.add(a)
            todo.extend(n for n in succ[a] if (a,n)!=blocked)
        return seen
    before=reachable();after=reachable((0x8c072dee,0x8c072df4))
    assert 0x8c072e84 in before and 0x8c072e84 not in after
    assert 0x8c072f58 in after,'The other response call remains structurally feasible'
    dead=before-after
    retained_dead_rows=dead|{a+2 for a in dead if a+2 in delayed}
    report=dict(status='contradictory_gate_excludes072e84_from_ordinary_owner_entry',root_entry='0x8c072b44',instruction_rows_byte_checked=629,conditional_outcomes=92,
        impossible_edge=['0x8c072dee','0x8c072df4'],required_equal_values=[4,5],unchanged_register='R5',infeasible_call='0x8c072e84',retained_feasible_call='0x8c072f58',
        structurally_gated_rows=sorted(map(hex,retained_dead_rows)),structurally_gated_row_count=len(retained_dead_rows),
        native_instructions_removed=0,new_runtime_guards=0,new_original_routine_credit=0,owner_registered=False,overall_completion_percent=10,
        scope='Ordinary entry and ordinary call returns only. CFG retains both alternatives of every other predicate. Blocking only the impossible second taken edge disconnects the entire072e84call region. The immediately preceding taken gate binds unchangedR5to4;the next taken gate requires5. No code is removed and no input branch is forced. Does not prove arbitrary externally supplied intermediate PCs,asynchronous register corruption,nonlocal resumes,whole-owner behavior or physical hardware.',
        sha256={str(p):sha(p) for p in (Path(__file__),manifest,listing,P/'source/sa_player_movement_owner.c',R/'work/sonic-adventure/extracted/1ST_READ.BIN')})
    target.write_text(json.dumps(report,indent=2)+'\n');print('OWNER GATE PROOF:072e84infeasible;072f58retained;',len(retained_dead_rows),'original gated rows kept in source',flush=True)
if __name__=='__main__':main()
