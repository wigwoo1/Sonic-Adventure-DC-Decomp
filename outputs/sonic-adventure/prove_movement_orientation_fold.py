"""Original-byte/CFG/data-backed scalar range certificate for074ee4's fold.

No game source, branch, numeric backend or observer is changed. This proof is
conditional on the stated original-image scalar software contract; it is not
a universal hardware, mutable-math-table or foreign-mode-change claim.
"""
import hashlib,json,re
from pathlib import Path
import movement_orientation_response_reference as original_view
from movement_orientation_fold_intervals import certificate
from prove_movement_model_response_predicates import graph
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def decode_float(w):
    n,m=(w>>8)&15,(w>>4)&15;tail=w&15
    binary={0:'fadd',1:'fsub',2:'fmul',3:'fdiv',4:'fcmp/eq',5:'fcmp/gt',12:'fmov'}
    if tail in binary:return f'{binary[tail]} fr{m},fr{n}'
    if tail==14:return f'fmac fr0,fr{m},fr{n}'
    if tail==8:return f'fmov.s @r{m},fr{n}'
    if tail==9:return f'fmov.s @r{m}+,fr{n}'
    if tail==10:return f'fmov.s fr{m},@r{n}'
    if tail==11:return f'fmov.s fr{m},@-r{n}'
    if tail==6:return f'fmov.s @(r0,r{m}),fr{n}'
    if tail==7:return f'fmov fr{m},@(r0,r{n})'
    key=w&0xf0ff
    if key in {0xf04d,0xf05d,0xf06d,0xf07d,0xf08d,0xf09d}:return {0xf04d:'fneg',0xf05d:'fabs',0xf06d:'fsqrt',0xf07d:'fsrra',0xf08d:'fldi0',0xf09d:'fldi1'}[key]+f' fr{n}'
    if key==0xf03d:return f'ftrc fr{n},FPUL'
    if key==0xf02d:return f'float FPUL,fr{n}'
    if key==0xf00d:return f'fsts FPUL,fr{n}'
    if key==0xf01d:return f'flds fr{n},FPUL'
    raise AssertionError(hex(w))

def main():
    binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();base=0x8c010000
    assert digest(binary)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    def word(a):return int.from_bytes(raw[a-base:a-base+4],'little')
    rows=original_view.ROWS
    def at(a,text):assert rows[a]==text,(hex(a),rows.get(a),text)
    bodies={name:(a,b) for name,a,b in (
      ('wrapper',0x8c10d0f8,0x8c10d132),('quadrants',0x8c10e6a0,0x8c10e6e6),
      ('core',0x8c10ef84,0x8c10f114),('reduced',0x8c10fb94,0x8c10fbb4),
      ('classifier',0x8c10fbb8,0x8c10fc9c),('scaling',0x8c10e7b8,0x8c10e856),('signed_sqrt',0x8c63a9c4,0x8c63a9d8))}
    bound=[]
    for name,(a,b) in bodies.items():
        assert all(pc in rows for pc in range(a,b,2)),name
        fp=[]
        for pc in range(a,b,2):
            text=rows[pc];opcode=int.from_bytes(raw[pc-base:pc-base+2],'little')
            if opcode>>12==15:
                assert decode_float(opcode)==text,(name,hex(pc),decode_float(opcode),text);fp.append((hex(pc),text))
            assert text not in ('frchg','fschg') and not (text.startswith('lds ') and text.endswith(',FPSCR')),(name,hex(pc),text)
        bound.append(dict(name=name,start=hex(a),end_exclusive=hex(b),rows=(b-a)//2,raw_span_sha256=hashlib.sha256(raw[a-base:b-base]).hexdigest(),decoded_float_operations=fp))
    # The classifier has no callee and does not write divisorFR5 or the core's
    # saved FP12..15. Its integer result is deliberately left unconstrained.
    for name in ('classifier','reduced','scaling'):
        a,b=bodies[name]
        for pc in range(a,b,2):
            t=rows[pc];assert not t.startswith(('jsr ','bsr ','jmp ')),(name,hex(pc),t)
            assert not re.search(r',r13$',t),(name,hex(pc),t)
            destination=re.search(r',fr(\d+)$',t)
            if destination:assert int(destination[1]) not in ({5,12,13,14,15} if name=='classifier' else {12,13,14,15}),(name,hex(pc),t)
    # Every real call in the scalar closure is bound to its original target.
    assert {pc for pc in range(*bodies['core'],2) if rows[pc].startswith(('jsr ','bsr '))}=={0x8c10f056,0x8c10f09c,0x8c10f0ba,0x8c10f0f2}
    for slot,target in {0x8c10d138:0x8c10e6a0,0x8c10e6e8:0x8c10ef84,0x8c10f13c:0x8c10fbb8,0x8c10f140:0x8c10fb94,0x8c10f14c:0x8c10e7b8}.items():assert word(slot)==target,(hex(slot),hex(word(slot)),hex(target))
    literals={}
    def constant(slot,expected):
        pointer=word(slot);actual=word(pointer);assert actual==expected,(hex(slot),hex(actual),hex(expected));literals[hex(slot)]=dict(pointer=hex(pointer),word=hex(actual));return actual
    pi=constant(0x8c10e6f4,0x40490fdb);constant(0x8c10e6f0,0xc0490fdb)
    pi2=constant(0x8c10f124,0x3fc90fdb);one=constant(0x8c10f128,0x3f800000);pi4=constant(0x8c10f12c,0x3f490fdb)
    constant(0x8c10f130,0);constant(0x8c10f134,0xbf800000);constant(0x8c10f138,0x3e000000)
    offsets={'1/4':constant(0x8c10f144,0x3e7adbb0),'1/2':constant(0x8c10f150,0x3eed6338),'3/4':constant(0x8c10f158,0x3f24bc7d),'1':pi4}
    constant(0x8c10f148,0xbe800000);constant(0x8c10f154,0xbf000000);constant(0x8c10f15c,0xbf400000);constant(0x8c10f160,0x3f400000)
    nan=word(word(0x8c10f120));assert nan>0x7f800000
    coefficient_pointer=word(0x8c10fbb4);coefficients=[word(coefficient_pointer+4*i) for i in range(6)]
    assert coefficients==[0xbdba2de9,0x3de38e38,0xbe124925,0x3e4ccccd,0xbeaaaaab,0x3f800000]
    # Original scaling accepts v in[0,1] and signed exponents-2/-1 here.
    # Its only nonzero normal result preserves the fraction and changes the
    # exponent E to E+k. Every subnormal or underflowing normal returns+0.
    # Enumerate exponent classes symbolically: the unchanged 23fraction bits
    # range over all values, rather than sampling two endpoint floats.
    scale_rows={
      0x8c10e7c8:'mov.l @r6,r7',0x8c10e7cc:'mov.l @r6,r14',0x8c10e7ce:'and r3,r7',
      0x8c10e7d0:'cmp/eq r3,r7',0x8c10e7d2:'bf/s 0x8c10e7ee',
      0x8c10e7ee:'mov.l 0x8c10e870,r2',0x8c10e7f0:'cmp/pl r7',0x8c10e7f6:'bt/s 0x8c10e802',
      0x8c10e7fa:'tst r14,r5',0x8c10e7fc:'bf 0x8c10e836',0x8c10e7fe:'bra 0x8c10e82e',
      0x8c10e802:'mov #-0x17,r3',0x8c10e804:'shad r3,r7',0x8c10e806:'add r4,r7',
      0x8c10e808:'cmp/pl r7',0x8c10e80a:'bf 0x8c10e812',0x8c10e80c:'mov.w 0x8c10e856,r3',
      0x8c10e80e:'cmp/ge r3,r7',0x8c10e810:'bf 0x8c10e83e',0x8c10e812:'cmp/pz r4',
      0x8c10e814:'bf 0x8c10e82a',0x8c10e82a:'tst r14,r5',0x8c10e82c:'bf 0x8c10e836',
      0x8c10e82e:'fmov fr4,fr0',0x8c10e83e:'mov.l 0x8c10e868,r2',0x8c10e840:'mov #0x17,r3',
      0x8c10e842:'and r14,r5',0x8c10e844:'shad r3,r7',0x8c10e846:'and r14,r2',
      0x8c10e848:'or r7,r5',0x8c10e84a:'or r2,r5',0x8c10e84c:'mov.l r5,@r6',0x8c10e84e:'fmov.s @r6,fr0',
      0x8c10f08c:'mov #-0x2,r4',0x8c10f096:'mov.l 0x8c10f14c,r3',
      0x8c10f0aa:'mov #-0x1,r4',0x8c10f0b4:'mov.l 0x8c10f14c,r3'}
    for pc,text in scale_rows.items():at(pc,text)
    assert word(0x8c10e858)==0x7f800000 and word(0x8c10e85c)==0x80000000 and word(0x8c10e868)==0x7fffff
    assert int.from_bytes(raw[0x8c10e856-base:0x8c10e858-base],'little')==255
    constant(0x8c10e870,0);constant(0x8c10e874,0x80000000)
    scaling_classes=[]
    for k in (-2,-1):
        for exponent in range(128):
            assert exponent<255 and k<0
            fraction_max=0 if exponent==127 else 0x7fffff
            target=exponent+k
            if exponent==0 or target<=0:
                result_lo=result_hi=0
            else:
                assert 0<target<255
                result_lo=target<<23;result_hi=result_lo|fraction_max
                # Positive binary32 order is integer-word order; the unchanged
                # fraction makes the bound hold for EVERY fraction in this class.
                assert result_hi<=(exponent<<23)|fraction_max
            assert 0<=result_lo<=result_hi<=0x3f800000
            scaling_classes.append(dict(exponent=exponent,power=k,fraction_range=[0,fraction_max],output_word_range=[result_lo,result_hi]))
    # Exact original operation order of reduction, polynomial and case bodies.
    specs={
      (0x8c10f026,0x8c10f02e):['fmov.s @r2,fr14','fmov fr3,fr15'],
      (0x8c10f03c,0x8c10f04c):['fmov.s @r15,fr3','fmov.s @r2,fr2','fadd fr3,fr5','fmov fr4,fr14','fadd fr3,fr2','fmov fr2,fr15','fdiv fr5,fr15'],
      (0x8c10f04c,0x8c10f066):['fmov.s @r3,fr5','fmov fr15,fr4','fmov.s @r3,fr13'],
      (0x8c10f08a,0x8c10f0a8):['fmov.s @r1,fr2','fmov.s @r2,fr3','fadd fr15,fr2','fadd fr3,fr14','fmov fr2,fr12','fmov fr15,fr4','fadd fr0,fr13','fdiv fr13,fr12','fmov fr12,fr4'],
      (0x8c10f0a8,0x8c10f0c6):['fmov.s @r1,fr2','fmov.s @r2,fr3','fadd fr15,fr2','fadd fr3,fr14','fmov fr2,fr12','fmov fr15,fr4','fadd fr0,fr13','fdiv fr13,fr12','fmov fr12,fr4'],
      (0x8c10f0c6,0x8c10f0e0):['fmov.s @r1,fr2','fmov.s @r3,fr0','fmov fr15,fr1','fadd fr2,fr1','fmov.s @r2,fr3','fmac fr0,fr15,fr13','fadd fr3,fr14','fmov fr1,fr4','fdiv fr13,fr4'],
      (0x8c10f0e0,0x8c10f0f2):['fadd fr15,fr13','fmov.s @r1,fr2','fmov.s @r2,fr3','fadd fr15,fr2','fadd fr3,fr14','fmov fr2,fr4','fdiv fr13,fr4'],
      (0x8c10f0f2,0x8c10f102):['fmov fr14,fr5','fadd fr0,fr5','fneg fr5','fmov fr5,fr0'],
      (0x8c10fb94,0x8c10fbb4):['fmov fr4,fr6','fmul fr4,fr6','fmov.s @r5+,fr5','fmov.s @r5+,fr3','fmov fr6,fr0','fmac fr0,fr5,fr3','fmov fr3,fr5','fmov fr4,fr0','fmov fr5,fr6','fmul fr6,fr0']}
    for (a,b),expected in specs.items():assert [rows[pc] for pc in range(a,b,2) if rows[pc].startswith('f')]==expected,(hex(a),expected)
    at(0x8c10fb9a,'mov #0x5,r4');at(0x8c10fba0,'add #-0x1,r4');at(0x8c10fba4,'cmp/pl r4');at(0x8c10fba8,'bt/s 0x8c10fb9e')
    at(0x8c10ef9c,'cmp/pz r3');at(0x8c10efa0,'mov #0x0,r4');at(0x8c10efa2,'mov #0x8,r4');at(0x8c10effa,'mov #0x8,r13');at(0x8c10effe,'and r4,r13');at(0x8c10f0fa,'tst r13,r13')
    at(0x8c10e6c0,'bt/s 0x8c10e6de');at(0x8c10e6cc,'bt 0x8c10e6d6');at(0x8c10e6dc,'fadd fr3,fr4');assert word(0x8c10e6ec)==0x80000000
    listing=P/'analysis/movement-orientation-response/8c074ee4_instructions.tsv';owner,ot,pred=graph(listing,raw)
    assert len(owner)==157 and pred[0x8c074fd6]=={0x8c074fd4} and pred[0x8c074fd8]=={0x8c074fd6} and pred[0x8c074fda]=={0x8c074fd8}
    for pc,text in {0x8c074fa0:'fmov fr15,fr3',0x8c074fa2:'fmul fr15,fr3',0x8c074fa4:'fmov fr14,fr0',0x8c074fa8:'fmac fr0,fr14,fr3',0x8c074faa:'jsr @r3',0x8c074fac:'fmov fr3,fr4',0x8c074fbc:'jsr @r3',0x8c074fbe:'fmov.s @r15,fr4',0x8c074fc8:'fmul fr3,fr2',0x8c074fcc:'ftrc fr2,FPUL',0x8c074fd0:'and r3,r14',0x8c074fd2:'cmp/gt r2,r14',0x8c074fd4:'bf 0x8c074fdc'}.items():assert ot[pc]==text
    scale=word(0x8c07518c);assert (scale,word(0x8c075190),word(0x8c075194),word(0x8c075198))==(0x4622f986,0xffff,0x8000,0x10000)
    result=certificate(coefficients,pi4,pi2,pi,scale,offsets)
    source=P/'source/sa_movement_orientation_response.c';assert all(('L'+hex(pc)[2:]+':') in source.read_text() for pc in (0x8c074fd6,0x8c074fd8,0x8c074fda))
    files=[Path(__file__),P/'movement_orientation_fold_intervals.py',P/'movement_orientation_response_reference.py',P/'prove_movement_model_response_predicates.py',binary,listing,source,
      P/'source/sa_collision_angle.c',P/'source/sa_draw_frame_dependencies.c',P/'source/sa_draw_distance_helpers.c',P/'source/sh4_multiply.c',P/'source/sh4_fmac.c',P/'source/sh4_add_sub.c',P/'source/sh4_divide.c',P/'source/sh4_sqrt.c',P/'source/sh4_truncate_i32.c',P/'source/sh4_compare_bits.h']
    report=dict(status='certified_original_math_scalar_profile_angle_fold_bound',
      profile='Normal entry; PR0/SZ0,RM0/RM1,completed preserved binary32 arithmetic; fixed original instruction/literal/coefficient inputs; ordinary mapped RAM stack and no asynchronous foreign writes or mode changes',
      original_math_bodies=bound,literal_bindings=literals,coefficient_table=hex(coefficient_pointer),coefficient_words=list(map(hex,coefficients)),coefficient_table_sha256=hashlib.sha256(raw[coefficient_pointer-base:coefficient_pointer-base+24]).hexdigest(),scaling_symbolic_exponent_classes=scaling_classes,interval_certificate=result,
      scalar_profile_infeasible_outcomes=[['0x8c074fd4','0x8c074fd6']],scalar_profile_unreachable_rows=['0x8c074fd6','0x8c074fd8','0x8c074fda'],
      unqualified_broader_outcomes=[['0x8c074fd4','0x8c074fd6']],original_code_removed=False,runtime_guards_added=False,original_math_data_or_precision_mutation_qualified=False,universal_physical_infeasibility_proved=False,
      arguments=[
        'Caller forms x^2+z^2 with completed scalar multiply/FMAC; it cannot be negative or negative zero. Existing signed sqrt therefore returns nonnegative magnitude or+Inf, or stops on unsupported/NaN inputs. The original atan2 numerator is nonnegative. Neither an ideal atan result nor a supplied callee output is used.',
        'Core finite reduction has v=abs(input) below1 or rounded(abs-1)/(abs+1) above1. Monotone completed scalar rounding preserves0<=v<=1. Zero,exact1,large/Inf andNaN exits are separate original cases. Classifier has no calls and preservesFR5/FR12..15 and integerR13. Its quotient is unconstrained in the proof. Every selected core formula plusfallback is enclosed, so no guessed classifier partition is needed.',
        'The byte-bound five-FMAC polynomial has a positive factor<=1 for squared argument in[0,1]. Exact Q derivative identity proves the rational lower partial monotone. Actual coefficient error, square error, five FMAC errors and final multiply error give the recorded rational error budget. Signed reduced inputs are enclosed using oddness and Q monotonicity. Every case numerator/positive denominator/offset is outward-rounded, including return-path additions.',
        'Actual core return sign follows original sign flags. Quadrant code adds positive retailPI only for a negative saved denominator and nonnegative saved numerator. All finite completed results lie in the certified interval. Inf ratios use finitePI/2 exits. NaN exits stayNaN through the wrapper/multiply or stop; preservedFTRC mapsNaN to80000000 when it completes, whose masked phase is0. Error-code writes cannot create a positive infinity return under the stated original-data contract.',
        'Certified final finite product is strictly above-1 and below32769. Completed scalarFTRC therefore yields integer0..32768;maskffff does not change it. SignedCMP cannot be greater than8000. The threefold instructions have no alternate predecessor. This closes only the stated original-math scalar software profile. Mutated/aliased math literals or coefficients,PR/SZ,foreign mode changes,physical exception/MMU/FPU behavior remain unqualified. Every original branch/row stays emitted.'
      ],new_original_routine_credit=0,sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_orientation_fold_certificate.json').write_text(json.dumps(report,indent=2)+'\n');print('ORIENTATION FOLD SCALAR CERTIFICATE PASS',result['conversion_product'],';original code retained;broader cases OPEN')
if __name__=='__main__':main()
