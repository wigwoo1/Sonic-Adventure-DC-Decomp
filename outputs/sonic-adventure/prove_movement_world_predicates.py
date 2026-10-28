"""Prove one normal-entry comparison outcome; explicitly preserve two null-read gaps."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    listing=P/'analysis/movement-world-owner/8c028bfe_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
    original=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    for row in rows:
        a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
    texts={int(r['address'],16):r['instruction'].lstrip('_') for r in rows}
    assert not any(t.startswith(('jmp ','braf ','bsrf ','rte')) for t in texts.values())
    delay={int(r['address'],16) for r in rows if r['instruction'].startswith('_')}
    predecessors={a:set() for a in texts if a not in delay}
    for a,t in texts.items():
        if a in delay or t=='rts':continue
        fall=a+(4 if t.startswith(('bt/s ','bf/s ','bra ','jsr ','bsr ')) else 2)
        if t.startswith(('bt ','bf ','bt/s ','bf/s ')):targets=[int(t.split()[-1],16),fall]
        elif t.startswith('bra '):targets=[int(t.split()[-1],16)]
        else:targets=[fall]
        for target in targets:assert target in predecessors,(hex(a),hex(target));predecessors[target].add(a)
    expected={0x8c028c8a:'fmov.s @(r0,r4),fr2',0x8c028c96:'fcmp/gt fr2,fr4',0x8c028c98:'bf/s 0x8c028cb8',
      0x8c028c9c:'fmov.s @(r0,r6),fr3',0x8c028c9e:'fcmp/gt fr8,fr3',0x8c028ca0:'bf 0x8c028cb8',
      0x8c028ca2:'fmov.s @(r0,r4),fr3',0x8c028ca4:'fcmp/gt fr4,fr3',0x8c028ca6:'bt 0x8c028cb8',
      0x8c028cba:'bf 0x8c028cc0',0x8c028cbc:'bra 0x8c028d92',0x8c028e04:'fmov.s @(r0,r4),fr3',0x8c028e0a:'tst r4,r4',0x8c028e0c:'bf 0x8c028d96'}
    for a,t in expected.items():assert texts[a]==t,(hex(a),texts[a])
    assert predecessors[0x8c028c9c]=={0x8c028c98} and predecessors[0x8c028ca2]=={0x8c028ca0}
    assert predecessors[0x8c028ca4]=={0x8c028ca2} and predecessors[0x8c028ca6]=={0x8c028ca4}
    interval={a:t for a,t in texts.items() if 0x8c028c8a<=a<=0x8c028ca6}
    assert not any(t.startswith(('jsr ','bsr ','jmp ')) or re.search(r',@',t) for t in interval.values())
    assert not any(re.search(r',r(?:0|4)$',t) and not t.startswith(('tst ','cmp/')) for t in interval.values())
    assert not any(re.search(r',fr4$',t) and not t.startswith('fcmp/') for a,t in interval.items() if a>=0x8c028c96)
    compare=P/'source/sh4_compare_bits.h';code=compare.read_text()
    controls={int(x,16) for x in re.findall(r'status&0x([0-9a-f]+)u',code)}
    assert controls=={0x80000,0x800,0x40000} and not any(x&(0x3f000|0x10040) for x in controls)
    assert 'else if(zero)t=0;' in code and 'else if((a^b)&0x80000000u)t=!(a&0x80000000u);' in code
    assert 'else t=(a&0x80000000u)?a<b:a>b;' in code
    report=dict(status='one_proved_outcome_two_explicit_null_read_frontiers',address='0x8c028bfe',infeasible_outcomes=[['0x8c028ca6','0x8c028cb8']],
      unaccounted_outcomes=[['0x8c028cba','0x8c028cbc'],['0x8c028e0c','0x8c028e0e']],unexecuted_rows=['0x8c028cbc','0x8c028cbe'],
      original_code_removed=False,new_original_routine_credit=0,owner_eligible_for_registration=False,
      arguments=[
        'To reachCA6, C98 must fall through, requiring C96 true:FR4>FR2. The second gateCA0 must also fall through. '
        'C8a andCA2 load the same key bits from the sameR4+R0. There are no intervening calls/RAMstores, '
        'base/offset changes,FR4writes orFPUmode changes. ThusCA4 compares that samekey>FR4 and cannot be true '
        'afterFR4>key. Ordered infinities andsignedzero preserve this antisymmetry;NaNs cannotmakeC96true. '
        'Unsupported/trapping states stop. The pinned softwarecomparison reads unchangedPR/DN/enable controls; '
        'cause/stickyflag changes do not change this ordering. Fixedcode/stablemappedRAM/normalentry only.',
        'Cba fallthrough requiresR4zero afterearlierR4+10 keyreads;E0c fallthrough similarly followsE04 keyread. '
        'The currentmappedRAM/bus ABI stops atnull+10 beforethose outcomes. This is not proof of physicalDreamcast '
        'unreachability:lowaddresses can involvebootROM,which isnot modeled/qualifiedhere. Do not inventBIOSbytes '
        'or treat a sentinel/disjointness assumption asproof. Keepbothoutcomes andCbc/Cbe unqualified; '
        '028bfe remainsunregisteredpendingactualsceneconstruction/inputinvariants orverifiedlowaddressreadsupport.'
      ],sha256={str(p):digest(p) for p in (Path(__file__),listing,original,compare,P/'source/sh4_compare.c',P/'source/sa_movement_world.c')})
    (P/'analysis/movement_world_predicate_proof.json').write_text(json.dumps(report,indent=2)+'\n')
    print('WORLD PREDICATES:1proved,2null-readoutcomes/2rowsOPEN;028bfeunregistered')

if __name__=='__main__':main()
