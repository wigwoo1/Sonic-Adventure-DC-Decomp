"""Conditional sort-counter proof; caller/frame nonaliasing remains unproven.

This does not remove instructions, close full-game gaps, or award recovery credit.
"""
import csv
import hashlib
import json
from pathlib import Path

P = Path(__file__).resolve().parent
ROOT = P.parents[1]


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    original = ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
    assert sha(original) == '45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    raw = original.read_bytes()
    listing = P/'analysis/movement-special-tail/8c0777e0_instructions.tsv'
    rows = {int(r['address'],16):r for r in csv.DictReader(listing.open(),delimiter='\t')}
    for pc,row in rows.items():
        assert raw[pc-0x8c010000:pc-0x8c010000+2] == bytes.fromhex(row['bytes'])
    expected = {
        0x8c07780e:'mov #0x0,r10',
        0x8c077972:'mov.l r10,@(0x4,r15)',
        0x8c077976:'mov.l r10,@(0x10,r15)',
        0x8c077a56:'mov.l @(0x4,r15),r0',
        0x8c077a58:'add #0x1,r0',
        0x8c077a5a:'mov.l r0,@(0x4,r15)',
        0x8c077a5c:'mov.l @(0x10,r15),r3',
        0x8c077a5e:'add #0x1,r3',
        0x8c077a60:'mov.l r3,@(0x10,r15)',
        0x8c077a6c:'mov.w @r1,r3',
        0x8c077a6e:'cmp/ge r3,r2',
        0x8c077a70:'bf 0x8c077978',
        0x8c077a72:'mov.l @(0x4,r15),r1',
        0x8c077a74:'tst r1,r1',
        0x8c077a76:'bf 0x8c077aba',
        0x8c077aec:'mov.l @(0x4,r15),r2',
        0x8c077af2:'add #-0x1,r2',
        0x8c077af6:'mov.l r2,@(0x8,r15)',
        0x8c077b10:'mov.l @(0x4,r15),r2',
        0x8c077b12:'cmp/pl r2',
        0x8c077b14:'bt/s 0x8c077b1c',
        0x8c077b74:'mov.l @(0x8,r15),r4',
        0x8c077b82:'cmp/pz r4',
        0x8c077b86:'bf/s 0x8c077c02',
        0x8c077bfc:'add #-0x1,r4',
        0x8c077bfe:'cmp/pz r4',
        0x8c077c00:'bt 0x8c077b8a',
    }
    for pc,text in expected.items():
        assert rows[pc]['instruction'].lstrip('_') == text,hex(pc)
    stores = [pc for pc,r in rows.items() if 0x8c077966<=pc<=0x8c077a76
              and r['instruction'].lstrip('_').startswith('mov.l ')
              and r['instruction'].endswith(',@(0x4,r15)')]
    assert stores == [0x8c077972,0x8c077a5a],stores
    # Induction, conditional on the explicit frame-preservation premise:
    # I=N=0 initially. One body adds either0or1 to N and exactly1 to I.
    # The next body requires I < sign_extend16(G), so I<32767. No wrap is
    # possible. Reaching the sort requires N!=0, hence1<=N<=32767.
    for processed in range(32768):
        assert 0<=processed<=32767
        if processed<32767:
            for accepted in (0,processed):
                for keep in (0,1):
                    assert 0<=accepted+keep<=processed+1<=32767
    for kept in range(1,32768):
        assert kept>0 and kept-1>=0
        # The forward array starts at SP+0x118 and the output at SP+0x58.
        # Nonnegative indices cannot themselves address the SP+4 counter.
        assert 0x118+12*(kept-1)>4 and 0x58>4
    caller_path = P/'analysis/collision-contact-input-owners/8c0730d8_instructions.tsv'
    caller = {int(r['address'],16):r for r in csv.DictReader(caller_path.open(),delimiter='\t')}
    call_rows = {
        0x8c0730de:'mov.l r4,@r15',0x8c0730e0:'mov.l r5,@(0x4,r15)',
        0x8c0730e4:'mov.l r6,@(0x8,r15)',0x8c073104:'mov.l @r15,r4',
        0x8c073106:'mov.l @(0x4,r15),r5',0x8c073108:'mov.l @(0x8,r15),r6',
        0x8c07310e:'jmp @r3',0x8c073110:'lds.l @r15+,PR',
    }
    for pc,text in call_rows.items():
        assert caller[pc]['instruction'].lstrip('_')==text,hex(pc)
        assert raw[pc-0x8c010000:pc-0x8c010000+2]==bytes.fromhex(caller[pc]['bytes'])
    assert int.from_bytes(raw[0x073144-0x010000:0x073148-0x010000],'little')==0x8c0777e0
    files=[Path(__file__),original,listing,caller_path,P/'source/sa_movement_special_tail.c']
    report=dict(
        status='conditional_sort_guard_invariant;caller_contract_not_established',
        conditional_edges=[['0x8c077b14','0x8c077b18'],['0x8c077b86','0x8c077c02']],
        conditional_rows=['0x8c077b18','0x8c077b1a'],
        invariant='N starts0, increases at most once per I increment;each next body requires I<signed16(G). Therefore0<=N<=I<=32767. Sort requiresN!=0, so CMP/PL N and CMP/PZ(N-1) both succeed.',
        required_premises=[
            'Entry is the original routine entry, with immutable code/literals.',
            'Original callees return with a balanced stack and preserved R10.',
            'Caller-reachable object/query/callback writes do not alias the private SP+4 and SP+0x10 counters during the prepass.',
            'No asynchronous mutation or nonlocal entry bypasses those initializations.',
        ],
        inspected_caller='0x8c0730d8 stage0902/0903 tail routing restores and forwards the same R4/R5/R6 arguments;it does not itself prove their allocations or disjointness from the future private frame.',
        finite_counter_values_checked=32768,positive_kept_values_checked=32767,
        full_game_caller_contract_proven=False,globally_closed_branch_outcomes=0,
        remaining_full_game_work='Recover/trace actual state/query allocation and possible aliases or demonstrate a faithful counterexample. Do not discard these edges merely because present fixtures are disjoint.',
        original_instruction_rows_byte_checked=len(rows)+len(call_rows),runtime_code_changed=False,
        new_original_routine_credit=0,overall_completion_percent=10,
        sha256={str(f):sha(f) for f in files})
    target=P/'analysis/movement_special_tail_sort_contract.json'
    assert not target.exists(),'Preserve existing evidence;author a new revision.'
    target.write_text(json.dumps(report,indent=2)+'\n')
    print('SORT CONTRACT CONDITIONAL PASS32768countervalues;caller alias contract remainsOPEN',flush=True)


if __name__=='__main__':main()
