"""Byte-backed normal-entry proofs for two repeated-comparison branch outcomes."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    listing=P/'analysis/movement-vector-dependencies/8c074cf0_instructions.tsv'
    rows=list(csv.DictReader(listing.open(),delimiter='\t'))
    original=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    for row in rows:
        a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
    texts={int(r['address'],16):r['instruction'].lstrip('_') for r in rows}
    assert not any(t.startswith(('jmp ','braf ','bsrf ','rte')) for t in texts.values())
    delay={int(r['address'],16) for r in rows if r['instruction'].startswith('_')}
    predecessors={a:set() for a in texts if a not in delay}
    for a,t in texts.items():
        if a in delay or t=='rts':continue
        delayed=t.startswith(('bt/s ','bf/s ','bra ','jsr ','bsr '))
        fall=a+(4 if delayed else 2)
        if t.startswith(('bt ','bf ','bt/s ','bf/s ')):targets=[int(t.split()[-1],16),fall]
        elif t.startswith('bra '):targets=[int(t.split()[-1],16)]
        elif t.startswith(('jsr ','bsr ')):targets=[a+4]
        else:targets=[fall]
        for target in targets:
            assert target in predecessors,(hex(a),hex(target))
            predecessors[target].add(a)
    assert predecessors[0x8c074d36]=={0x8c074d20,0x8c074d2a}
    assert predecessors[0x8c074d3e]=={0x8c074d3c}
    assert predecessors[0x8c074d46]=={0x8c074d44}
    assert predecessors[0x8c074e3c]=={0x8c074e3a}
    assert predecessors[0x8c074e3e]=={0x8c074e3c}
    expected={
      0x8c074d1c:'mov.l @(r0,r14),r3',0x8c074d1e:'tst r4,r3',0x8c074d20:'bt 0x8c074d36',
      0x8c074d24:'fldi0 fr3',0x8c074d26:'fmov.s @(r0,r12),fr2',
      0x8c074d28:'fcmp/gt fr3,fr2',0x8c074d2a:'bt 0x8c074d36',
      0x8c074d38:'mov.l @(r0,r14),r2',0x8c074d3a:'tst r2,r4',0x8c074d3c:'bt 0x8c074d5a',
      0x8c074d40:'fldi0 fr3',0x8c074d42:'fmov.s @(r0,r12),fr2',
      0x8c074d44:'fcmp/gt fr3,fr2',0x8c074d46:'bf 0x8c074d5a',
      0x8c074e38:'fcmp/gt fr5,fr4',0x8c074e3a:'bf 0x8c074eb6',
      0x8c074e3c:'fcmp/gt fr5,fr4',0x8c074e3e:'bf 0x8c074e8c'}
    for a,t in expected.items():assert texts[a]==t,(hex(a),texts[a])
    assert texts[0x8c074d0c]=='mov #0x40,r4' and texts[0x8c074d18]=='shll8 r4'
    # The admitted paths from D20 to D46 have no calls or RAM stores. R12/R14
    # address bases and R4's4000 mask remain intact. Even overlapping input
    # records are stable because nothing writes them between either reload.
    interval={a:t for a,t in texts.items() if 0x8c074d20<=a<=0x8c074d46}
    assert not any(t.startswith(('jsr ','bsr ','jmp ')) for t in interval.values())
    assert not any(re.search(r',@',t) for t in interval.values())
    assert not any(re.search(r',r(?:4|12|14)$',t) and not t.startswith(('tst ','cmp/')) for t in interval.values())
    assert texts[0x8c074d22]==texts[0x8c074d3e]=='mov #0x8,r0'
    assert texts[0x8c074d36]=='mov #0x60,r0'
    # Compare result depends on input bits and unchanged control fields, not
    # previously set cause/sticky bits. Nonzero returns are stops, not outcomes.
    compare=P/'source/sh4_compare_bits.h';code=compare.read_text()
    controls={int(x,16) for x in re.findall(r'status&0x([0-9a-f]+)u',code)}
    assert controls=={0x80000,0x800,0x40000}
    assert 'status&=~0x3f000u' in code and 'status|=0x10040u' in code
    assert all(not mask&(0x3f000|0x10040) for mask in controls)
    assert 'else if(zero)t=0;' in code and '*sr=(*sr&~1u)|t;' in code
    proof=dict(status='verified_normal_entry_repeated_comparison_predicates',address='0x8c074cf0',
      static_conditional_outcomes=40,infeasible_outcomes=[['0x8c074d46','0x8c074d5a'],['0x8c074e3e','0x8c074e8c']],
      domain_excluded_rows=[],domain_excluded_outcomes=[],original_code_removed=False,
      graph_predecessors={hex(a):list(map(hex,sorted(predecessors[a]))) for a in (0x8c074d36,0x8c074d3e,0x8c074d46,0x8c074e3c,0x8c074e3e)},
      arguments=[
        'D36 has only D20 and D2a predecessors. D20 taken means the4000bit is clear; '
        'the same mask and stable RAM reload atD38 then forceD3c toD5a, bypassingD46. '
        'To reachD46, D20 must fall through andD2a must be taken, so the first greater '
        'comparisonD28 was true. D40/D42 reload the same operands with the same address '
        'bases/offset8 and FP bank/size. There are no intervening calls or RAM stores. '
        'Thus the repeated greater comparisonD44 is also true if it completes;BF D46 cannot takeD5a.',
        'E3c is reached only by E3a fallthrough, which requires E38 greater comparison true. '
        'E3c is the identical compare instruction on unchangedFR4/FR5. It changes only '
        'comparison status/cause flags;the helper decision reads unchangedPR,DN,enable '
        'controls. Its T result remains true if it completes, so BF E3e cannot takeE8c.',
        'NaNs cannot make a true first greater comparison; unsupported/trapping numerical '
        'states stop instead of establishing a false completed second result. '
        'The argument requires original normal entry/fixed code, stable mapped RAM '
        'without asynchronous modification, and the existing software comparison semantics. '
        'Arbitrary mid-function entries, physical exception/MMU/interrupt delivery and '
        'self-modifying code are not qualified. Input-record disjointness is not assumed.'
      ],new_original_routine_credit=0,
      sha256={str(p):digest(p) for p in (Path(__file__),listing,original,compare,P/'source/sh4_compare.c',P/'source/sa_movement_vectors.c')})
    (P/'analysis/movement_vectors_predicate_proof.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('MOVEMENT PREDICATES PROVED2outcomes;nooriginalrows/outcomesexcluded')


if __name__=='__main__':main()
