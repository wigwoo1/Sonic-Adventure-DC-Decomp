"""Byte-backed selector exhaustiveness and stable scalar-profile predicate proofs."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def graph(path,raw):
    rows=list(csv.DictReader(path.open(),delimiter='\t'));texts={int(r['address'],16):r['instruction'].lstrip('_') for r in rows}
    for r in rows:
        a=int(r['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
    delay={int(r['address'],16) for r in rows if r['instruction'].startswith('_')};pred={a:set() for a in texts if a not in delay}
    for a,t in texts.items():
        if a in delay or t=='rts' or t.startswith(('jmp ','braf ','rte')):continue
        fall=a+(4 if t.startswith(('bt/s ','bf/s ','bra ','jsr ','bsr ')) else 2)
        if t.startswith(('bt ','bf ','bt/s ','bf/s ')):targets=[int(t.split()[-1],16),fall]
        elif t.startswith('bra '):targets=[int(t.split()[-1],16)]
        else:targets=[fall]
        for b in targets:assert b in pred,(hex(a),hex(b));pred[b].add(a)
    return rows,texts,pred

def main():
    original=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    walk=P/'analysis/movement-model-response/8c036bc0_instructions.tsv';owner=P/'analysis/movement-model-response/8c0742d4_instructions.tsv'
    wr,w,wp=graph(walk,raw);rr,r,rp=graph(owner,raw)
    assert len(wr)==377 and len(rr)==519
    assert w[0x8c036be0]=='mov.l @r14,r0' and w[0x8c036be2]=='and #0x7,r0'
    values=(7,0,1,2,3,4,5,6)
    compares=(0x8c036be4,0x8c036be8,0x8c036bec,0x8c036bf0,0x8c036bf8,0x8c036c00,0x8c036c08,0x8c036c10)
    for a,value in zip(compares,values):assert w[a]==f'cmp/eq #0x{value:x},r0'
    assert w[0x8c036c12]=='bf 0x8c036c18' and w[0x8c036c18]=='bra 0x8c036ef4' and w[0x8c036c1a]=='nop'
    chain=(0x8c036be2,0x8c036be4,0x8c036be6,0x8c036be8,0x8c036bea,0x8c036bec,0x8c036bee,0x8c036bf0,0x8c036bf2,0x8c036bf8,0x8c036bfa,0x8c036c00,0x8c036c02,0x8c036c08,0x8c036c0a,0x8c036c10,0x8c036c12)
    for previous,current in zip(chain,chain[1:]):assert wp[current]=={previous},hex(current)
    assert wp[0x8c036c18]=={0x8c036c12}
    assert not any(int(x,16)==0x8c036c1a for row in wr for x in row['flows'].split())
    assert not any(v not in values for v in range(8))
    assert not any(re.search(r',r0$',w[a]) and not w[a].startswith(('and ','cmp/','tst ')) for a in chain)
    expected={0x8c0745dc:'mov #-0x30,r0',0x8c0745de:'extu.b r0,r0',0x8c0745e0:'fmov.s @(r0,r13),fr3',
      0x8c0745e2:'mov #0x38,r0',0x8c0745e4:'fmov.s @(r0,r13),fr2',0x8c0745e6:'fcmp/gt fr2,fr3',0x8c0745e8:'bf 0x8c074606',
      0x8c074610:'mov #0x38,r0',0x8c074612:'fmov.s @(r0,r13),fr5',0x8c07461e:'mov #-0x30,r0',0x8c074620:'extu.b r0,r0',
      0x8c074622:'fmov.s @(r0,r13),fr3',0x8c074624:'fcmp/gt fr3,fr5',0x8c074626:'bt 0x8c074644'}
    for a,t in expected.items():assert r[a]==t,(hex(a),r[a])
    assert rp[0x8c07460c]=={0x8c0745fe,0x8c074604}
    assert rp[0x8c074626]=={0x8c074624}
    admitted=[a for a in r if 0x8c0745dc<=a<=0x8c074626 and not 0x8c074606<=a<=0x8c07460a]
    for a in admitted:
        t=r[a];assert not re.search(r',@',t),(hex(a),t)
        assert not t.startswith(('jsr ','bsr ','jmp ','frchg','fschg','lds ','ldc '))
        assert not re.search(r',r13$',t),(hex(a),t)
    # Current ordinary transform path is already restricted by the preserved
    # matrix-pop backend. This is existing software ABI policy, not a new guard.
    pop=P/'source/model_matrix_stack_pop.c';pop_code=pop.read_text()
    assert 'if(fpu->fpscr&0x180000u)return -1;' in pop_code
    assert pop_code.count('fpu->fpscr^=0x100000u;')==2
    compare=P/'source/sh4_compare_bits.h';compare_code=compare.read_text()
    controls={int(x,16) for x in re.findall(r'status&0x([0-9a-f]+)u',compare_code)}
    assert controls=={0x80000,0x800,0x40000} and 'status&=~0x3f000u' in compare_code
    proof=dict(status='byte_backed_original_selector_and_existing_scalar_profile_predicates',original_rows_byte_checked=896,
      integer_infeasible_outcomes=[['0x8c036c12','0x8c036c18']],proved_unreachable_original_rows=['0x8c036c18','0x8c036c1a'],
      scalar_profile_infeasible_outcomes=[['0x8c074626','0x8c074644']],scalar_profile='PR0/SZ0,stable mapped RAM,fixed code,normal entry and unchanged software compare policy',
      domain_excluded_rows=[],domain_excluded_outcomes=[],original_code_removed=False,
      arguments=[
        '036be2 masks the single loaded selector with7. The only admitted predecessor chain to036c12 successively rulesout7,0,1,2,3,4,5. '
        'The final compare must therefore see6. BF036c12 cannot take036c18. The latter branch and itsNOPdelay slot have no other predecessor. '
        'This covers all32bit inputflags by the originalAND invariant;no input-value exclusion/disjointness assumption or discarded instruction.',
        'In the current ordinary scalar PR0/SZ0 profile,0745e6 compares cap(state+d0)>speed(state+38). Reaching07460c requires this comparison true '
        '(only5fe/604 enter60c). The admitted read path has no calls,RAM stores,R13changes orFPUmode changes.074612 reloads identical speed and '
        '074622 reloads identical cap.074624 tests the opposite speed>cap. It cannot be true after a completed true cap>speed. '
        'NaNs cannot make the first greater comparison true;exception stops are stops,not a completed opposite result. Sticky/cause updates do not change operands/controls.',
        'The scalar predicate is explicitly a current PR0/SZ0 software-contract proof. Existing matrix-pop backend rejectsPR/SZprofiles;selected rawmodes '
        'are separately tested as reached unsupported stops. This is not a claim that arbitrary physicalSZ1 execution or foreign mode-changing callbacks '
        'make the original626edge universally impossible. Broader precision/physicalFPU behavior remains unfinished;the original edge stays in source.',
        'All proofs require fixed original code and normal-entry execution with stable mappedRAM. Arbitrary mid-body entry,selfmodification, '
        'asynchronous hardware/MMU/exception behavior are unqualified. No original row or branch is removed;two integer-chain rows are accounted as unreachable,not deleted.'
      ],new_original_routine_credit=0,
      sha256={str(p):digest(p) for p in (Path(__file__),original,walk,owner,pop,compare,P/'source/sh4_compare.c',P/'source/sa_movement_model_response.c')})
    (P/'analysis/movement_model_response_predicate_proof.json').write_text(json.dumps(proof,indent=2)+'\n');print('MODEL RESPONSE PREDICATES PASS:integer1edge/2rows;scalar-profile1edge;all source preserved')

if __name__=='__main__':main()
