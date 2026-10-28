"""Original-byte and CFG proof for a repeated stable-RAM flag test."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    listing=P/'analysis/movement-parameters-owner/8c075214_instructions.tsv'
    rows=list(csv.DictReader(listing.open(),delimiter='\t'))
    original=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    for row in rows:
        a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
    assert len(rows)==216
    texts={int(r['address'],16):r['instruction'].lstrip('_') for r in rows}
    assert not any(t.startswith(('jsr ','bsr ','jmp ','braf ','bsrf ','rte')) for t in texts.values())
    delay={int(r['address'],16) for r in rows if r['instruction'].startswith('_')}
    predecessors={a:set() for a in texts if a not in delay}
    for a,t in texts.items():
        if a in delay or t=='rts':continue
        fall=a+(4 if t.startswith(('bt/s ','bf/s ','bra ')) else 2)
        if t.startswith(('bt ','bf ','bt/s ','bf/s ')):targets=[int(t.split()[-1],16),fall]
        elif t.startswith('bra '):targets=[int(t.split()[-1],16)]
        else:targets=[fall]
        for target in targets:
            assert target in predecessors,(hex(a),hex(target))
            predecessors[target].add(a)
    expected={
      0x8c07522e:'mov #0x79,r0',0x8c075230:'shll2 r0',
      0x8c075232:'mov.l @(r0,r6),r0',0x8c075234:'tst #0x2,r0',
      0x8c075236:'bt/s 0x8c0752b8',0x8c075238:'mov r14,r13',
      0x8c0752b8:'mov #0x79,r0',0x8c0752ba:'mov.l 0x8c0753b8,r3',
      0x8c0752bc:'shll2 r0',0x8c0752be:'mov.l @(r0,r6),r2',
      0x8c0752c0:'tst r3,r2',0x8c0752c2:'bt 0x8c075328',
      0x8c075328:'mov.l @(r0,r6),r0',0x8c07532a:'tst #0x2,r0',0x8c07532c:'bf 0x8c075382'}
    for a,t in expected.items():assert texts[a]==t,(hex(a),texts[a])
    assert predecessors[0x8c0752b8]=={0x8c075236}
    assert predecessors[0x8c075328]=={0x8c0752c2}
    assert predecessors[0x8c07532c]=={0x8c07532a}
    chain=[0x8c075230,0x8c075232,0x8c075234,0x8c075236,
           0x8c0752b8,0x8c0752ba,0x8c0752bc,0x8c0752be,
           0x8c0752c0,0x8c0752c2,0x8c075328,0x8c07532a,0x8c07532c]
    for previous,current in zip(chain,chain[1:]):assert predecessors[current]=={previous},hex(current)
    # The only route from the first flag read to the repeated test. Intervening
    # scaled branches go straight to5382 and cannot enter this route midway.
    admitted=[0x8c075232,0x8c075234,0x8c075236,0x8c075238,
              0x8c0752b8,0x8c0752ba,0x8c0752bc,0x8c0752be,
              0x8c0752c0,0x8c0752c2,0x8c075328,0x8c07532a,0x8c07532c]
    for a in admitted:
        t=texts[a]
        assert not re.search(r',@',t),(hex(a),t)
        assert not re.search(r',r6$',t),(hex(a),t)
        assert not t.startswith(('jsr ','bsr ','jmp ','ldc ','lds '))
    assert int.from_bytes(raw[0x753b8-0x10000:0x753bc-0x10000],'little')==0x400000
    proof=dict(status='verified_normal_entry_repeated_stable_RAM_flag_predicate',address='0x8c075214',
      original_rows_byte_checked=216,static_conditional_outcomes=16,
      infeasible_outcomes=[['0x8c07532c','0x8c075382']],domain_excluded_rows=[],domain_excluded_outcomes=[],original_code_removed=False,
      graph_predecessors={hex(a):list(map(hex,sorted(predecessors[a]))) for a in (0x8c0752b8,0x8c075328,0x8c07532c)},
      admitted_read_to_test_path=list(map(hex,admitted)),
      argument='Normal entry setsR0=0x79<<2=0x1e4 before5232 reads state[R6+1e4]. Only5236taken enters52b8, so bit2 was clear. '
      '52b8 reestablishesR0=1e4;52c2taken is the sole predecessor of5328 and preserves that offset andR6. '
      '5328 rereads the identical state word. The admitted path has no RAM stores, calls, address-base changes or asynchronous writes. '
      'Thus bit2 remains clear;TST2 at532a setsT=1 and BF532c cannot take5382. Input/table/stack/output disjointness is not assumed. '
      'The earlier flag OR and saved-register stores occur before the first read and do not invalidate the argument. '
      'Fixed original code, normal entry and stable mapped RAM are required. Arbitrary mid-body entry, self-modifying code, '
      'physical asynchronous/MMIO/exception/MMU delivery remain unqualified. No original instruction or conditional edge is removed.',
      new_original_routine_credit=0,sha256={str(p):digest(p) for p in (Path(__file__),listing,original,P/'source/sa_movement_parameters.c')})
    (P/'analysis/movement_parameters_predicate_proof.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PARAMETERS PREDICATE PASS:1 byte-backed normal-entry outcome;216 rows preserved')

if __name__=='__main__':main()
