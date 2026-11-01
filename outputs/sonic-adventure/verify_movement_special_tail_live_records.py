"""Whole-entry real callbacks, live record mutation and repeated response."""
import hashlib
import itertools
import json
from fractions import Fraction
from pathlib import Path

import movement_special_tail_live_record_fixtures as inputs
import verify_movement_special_tail_direct as direct
from verify_movement_special_tail_gates import rounded_fraction

P = Path(__file__).resolve().parent


def scene():
    nodes = []
    for j in range(2):
        low = [dict(key=.5, point=(.5, 0, .5), extent=64,
                    vertices=direct.PLANES[k]) for k in (2, 4, 0)]
        nodes.append(dict(object_index=j, low=low, low_head=2,
                          high=[dict(key=-100)]))
    return dict(static=[dict(id=inputs.TASK, flags=0x40),
                        dict(id=inputs.TASK, flags=0)], nodes=nodes)


def configurations():
    for index, alias, mode, rm in itertools.product((0, 1), (False, True), (0, 4), (0, 1)):
        yield dict(kind='live_record_response', index=index,
                   callback_record_alias=alias, position=(.5, 0, .5),
                   velocity=(-16, -64, -16), previous_owner=inputs.DATA,
                   state_speed=1, moving_data=True, moving_offset=(1, 0, 0),
                   major=9, minor=2, mode=mode, rm=rm, **scene())
    yield dict(kind='callback_numeric_frontier', index=0,
               callback_record_alias=False, position=(.5, 0, .5),
               velocity=(-16, -64, -16), previous_owner=inputs.DATA,
               state_speed=1, moving_data=True, moving_offset=(0, 0, 0),
               major=9, minor=2, mode=0, rm=0, **scene())


def main():
    parent = P/'verify_movement_special_tail_direct.py'
    source = parent.read_text()
    marker = 'movement_special_tail_direct_verification.json'
    assert source.count(marker) == 1
    source = source.replace(marker, 'movement_special_tail_live_record_verification.json')
    marker = 'rm=(n//2)%2'
    assert source.count(marker) == 1
    source = source.replace(marker, "rm=c.get('rm',(n//2)%2)")
    marker = "('raw_numeric_mode','fpu_disabled','mapped_guard')"
    assert source.count(marker) == 1
    source = source.replace(marker, "('raw_numeric_mode','fpu_disabled','mapped_guard','callback_numeric_frontier')")
    marker = '        seen.update(path)'
    assert source.count(marker) == 1
    source = source.replace(marker, """        callbacks=[b for b in result['boundaries'] if b['target']==hex(q.IMPULSE)]
        require(bool(callbacks), 'actual original impulse callback executes inside full routine')
        record=q.DATA+(0x1c if c['index'] else 0)
        character=record-0x10 if c['callback_record_alias'] else q.D
        require(all(b['arguments'][:3]==[character,q.A,record] for b in callbacks), 'actual character/actor/live-record callback arguments')
        if c['kind']=='callback_numeric_frontier':
            require(result['status']==-1 and result['stop_pc']=='0x8c0751d4', 'declared unsupported FMAC frontier;not a normal gameplay return')
            bank=(f.fpscr>>21)&1
            require(any((int(f.banks[bank][j])&0x7f800000)==0x7f800000 for j in (0,2,3)), 'nonfinite operand reaches existing FMAC software-domain guard')
        else:
            if not c['callback_record_alias']:
                require(0x8c077dc0 in path and 0x8c077dc4 in path, 'nonzero prior response follows actual nonzero child response')
                require(s.r[0]==12, 'whole routine returns original response12')
            else:
                require(s.r[0]==12, 'aliased live-record case returns original response12')
                expected=tuple(Fraction(q.number(q.get(before,record+0x10+4*j))) for j in range(3))
                impulse=tuple(Fraction(q.number(q.get(before,record+0x3c+4*j))) for j in range(3))
                for callback in callbacks:
                    expected=tuple(Fraction(rounded_fraction(a+3*b,c['rm'])) for a,b in zip(expected,impulse))
                require(vector(m,record+0x10)==tuple(map(float,expected)), 'independent fused rational live-record translation updates')
                oracles+=1
"""+marker)
    ns = dict(__name__='special_tail_live_records_replay', __file__=str(parent))
    exec(compile(source, str(parent), 'exec'), ns)
    ns.update(q=inputs, configurations=configurations,
              Fraction=Fraction, rounded_fraction=rounded_fraction)
    ns['main']()
    target = P/'analysis/movement_special_tail_live_record_verification.json'
    report = json.loads(target.read_text())
    normal = [r for r in report['cases'] if r['configuration']['kind'] != 'callback_numeric_frontier']
    assert len(normal) == 16 and all(r['status'] == 0 for r in normal)
    assert {(r['configuration']['mode'],r['configuration']['rm']) for r in normal} == {(0,0),(0,1),(4,0),(4,1)}
    report['normal_completions'] = 16
    report['declared_numeric_frontier_cases'] = 1
    report['scope'] = ('Full0777e0 ordinary-entry comparisons with original world, '
                       'collision,07506e and actual0751c0 callback execution. '
                       'Sixteen normal cases cover both record slots, four '
                       'bank/rounding combinations, real repeated positive '
                       'response and live-record alias mutation. Eight independent '
                       'exact fused-rational mutation oracles. One separate '
                       'nonfinite FMAC software-frontier case is not a normal '
                       'gameplay return. Callback pointer/table/record inputs '
                       'are authored;retail callback assignment, count guard '
                       'contracts, physical FPU and full game remain unqualified. '
                       'No callback results/contact outputs/frames supplied.')
    for file in (Path(__file__),P/'movement_special_tail_live_record_fixtures.py',
                 P/'movement_special_tail_context_fixtures.py',P/'verify_movement_special_tail_gates.py'):
        report['sha256'][str(file)] = hashlib.sha256(file.read_bytes()).hexdigest()
    target.write_text(json.dumps(report,indent=2)+'\n')
    print('SPECIAL TAIL LIVE RECORD PASS',report['original_native_comparisons'],
          report['independent_checks'],report['status_counts'],flush=True)


if __name__ == '__main__':
    main()
