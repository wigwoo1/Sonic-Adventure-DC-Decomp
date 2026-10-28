"""Byte/CFG-backed stable scalar-RAM comparison proof; original edge retained."""
import csv,hashlib,json,re
from pathlib import Path
from prove_movement_model_response_predicates import graph
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    original=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    listing=P/'analysis/movement-projection-response/8c64016c_instructions.tsv';rows,t,pred=graph(listing,raw);assert len(rows)==251
    expected={0x8c640258:'mov #0xc,r0',0x8c64025c:'fldi0 fr3',0x8c640260:'fmov.s @(r0,r12),fr2',0x8c640262:'fcmp/eq fr3,fr2',0x8c640264:'bf/s 0x8c640308',0x8c640266:'fsub fr1,fr4',0x8c640268:'mov #0x10,r0',0x8c64026a:'fmov.s @(r0,r12),fr2',0x8c64026c:'fcmp/eq fr3,fr2',0x8c64026e:'bf 0x8c6402b8',0x8c6402b8:'mov #0xc,r0',0x8c6402ba:'fldi0 fr3',0x8c6402bc:'fmov.s @(r0,r12),fr2',0x8c6402be:'fcmp/eq fr3,fr2',0x8c6402c0:'bf 0x8c640308'}
    for a,text in expected.items():assert t[a]==text,(hex(a),t[a],text)
    chain=(0x8c640260,0x8c640262,0x8c640264,0x8c640268,0x8c64026a,0x8c64026c,0x8c64026e,0x8c6402b8,0x8c6402ba,0x8c6402bc,0x8c6402be,0x8c6402c0)
    for previous,current in zip(chain,chain[1:]):assert pred[current]=={previous},hex(current)
    for a in chain+(0x8c640266,):
        text=t[a];assert not re.search(r',@',text),(hex(a),text)
        assert not text.startswith(('jsr ','bsr ','jmp ','frchg','fschg','lds ','ldc '))
        assert not re.search(r',r12$',text),(hex(a),text)
    caller=P/'analysis/collision-contact-input-owners/8c0730d8_instructions.tsv';crows=list(csv.DictReader(caller.open(),delimiter='\t'));caller_text={int(r['address'],16):r['instruction'].lstrip('_') for r in crows}
    for a,text in {0x8c073a3e:'mov r11,r5',0x8c073a42:'mov r12,r6',0x8c073a46:'mov r14,r7',0x8c073a4e:'mov.l r2,@-r15',0x8c073a50:'jsr @r3',0x8c073a52:'mov r13,r4'}.items():
        assert caller_text[a]==text
        row=next(r for r in crows if int(r['address'],16)==a);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
    report=dict(status='byte_backed_stable_scalar_RAM_repeated_zero_comparison',original_rows_byte_checked=257,scalar_profile_infeasible_outcomes=[['0x8c6402c0','0x8c640308']],scalar_profile='PR0/SZ0,stable ordinary mapped RAM,fixed original code,normal entry and unchanged software compare policy',unqualified_broader_precision_or_physical_outcomes=[['0x8c6402c0','0x8c640308']],proved_unreachable_original_rows=[],domain_excluded_rows=[],domain_excluded_outcomes=[],original_code_removed=False,parent_caller_still_unregistered='0x8c0730d8',arguments=[
      'The only admitted predecessor chain to6402c0 passes640264 without taking BF and64026e taking BF. The first completed comparison therefore read output+c equal to binary32 zero. R0 is explicitly+c for both reads; R12 does not change. No call, RAM store, FPU-mode change or control-register update occurs along this path, including the264delay instruction. Both scalar reads see identical bits. Both compare to fresh/unchanged positive zero; negative zero also compares equal and NaN cannot pass the first equality. The second BF cannot take308 in this explicit stable scalar RAM software profile.',
      'This does not establish universal physical SZ1/foreign precision, MMIO, asynchronous interrupt or self-modifying-code behavior. All original instructions and the original edge remain emitted. Broader precision/physical qualification remains open; no input domain or original row was deleted to obtain coverage.',
      'Actual0730d8 call73a50 supplies R5/R6/R7 from its live movement/state/query registers, R4 in its delay slot, and an ordinary fifth stack argument. Authored fixtures exercise these normal function arguments; no execution frame or callee output is supplied. Caller identity/argument evidence does not qualify the entire0730d8parent.'
      ],new_original_routine_credit=0,sha256={str(p):digest(p) for p in (Path(__file__),P/'prove_movement_model_response_predicates.py',original,listing,caller,P/'source/sa_movement_projection_response.c',P/'source/sh4_compare.c',P/'source/sh4_compare_bits.h')})
    (P/'analysis/movement_projection_response_predicate_proof.json').write_text(json.dumps(report,indent=2)+'\n');print('PROJECTION PREDICATE PASS1scalar-profile outcome;all1005rows and originaledge retained')
if __name__=='__main__':main()
