"""Real mixed-object late-clamp selection in both banks and rounding modes."""
import hashlib
import itertools
import json
from pathlib import Path

import movement_special_tail_context_fixtures as inputs
import verify_movement_special_tail_direct as direct

P = Path(__file__).resolve().parent


def configurations():
    for count, mode, rm in itertools.product((2, 3), (0, 4), (0, 1)):
        nodes = []
        # Original world ordering makes the selected floor arrive after a wall.
        # No contact list, candidate result or execution frame is pre-authored.
        for j in range(count):
            plane = direct.PLANES[0 if j == 0 else 2]
            nodes.append(dict(object_index=j,
                              low=[dict(key=.5, point=(.5, 0, .5), extent=64,
                                        vertices=plane)],
                              high=[dict(key=-100)]))
        yield dict(kind='mixed_object_late_clamp', position=(.5, 0, .5),
                   velocity=(0, -3, 0), angles=(0x4000, 0, 0),
                   previous_owner=inputs.DATA, mode=mode, rm=rm,
                   static=[dict(id=0) for _ in range(count)], nodes=nodes)


def main():
    parent = P / 'verify_movement_special_tail_direct.py'
    source = parent.read_text()
    name = 'movement_special_tail_direct_verification.json'
    assert source.count(name) == 1
    source = source.replace(name, 'movement_special_tail_object_order_verification.json')
    mode = 'rm=(n//2)%2'
    assert source.count(mode) == 1
    source = source.replace(mode, "rm=c.get('rm',(n//2)%2)")
    marker = '        seen.update(path)'
    assert source.count(marker) == 1
    source = source.replace(marker, """        require(any(a==0x8c077b26 and path[j+1]==0x8c077b2c for j,a in enumerate(path[:-1])), 'real later-selected clamp takes unequal-index branch')
""" + marker)
    ns = dict(__name__='special_tail_object_order_replay', __file__=str(parent))
    exec(compile(source, str(parent), 'exec'), ns)
    ns['q'] = inputs
    ns['configurations'] = configurations
    ns['main']()
    target = P / 'analysis/movement_special_tail_object_order_verification.json'
    report = json.loads(target.read_text())
    assert {(c['configuration']['mode'], c['configuration']['rm'])
            for c in report['cases']} == {(0, 0), (0, 1), (4, 0), (4, 1)}
    report['scope'] = ('Original0777e0 full-state and ordered-boundary comparisons '
                       'with two/three real authored scene objects. The original '
                       'world/contact producers select a later floor clamp and '
                       'exercise the unequal selected-index branch. All four '
                       'bank/rounding combinations checked explicitly. No output '
                       'tables/results/frames supplied. Published code/observer '
                       'unchanged. Remaining branches and full game unqualified.')
    for f in (Path(__file__), P / 'movement_special_tail_context_fixtures.py'):
        report['sha256'][str(f)] = hashlib.sha256(f.read_bytes()).hexdigest()
    target.write_text(json.dumps(report, indent=2) + '\n')
    print('SPECIAL TAIL OBJECT ORDER PASS', report['original_native_comparisons'],
          report['independent_checks'], flush=True)


if __name__ == '__main__':
    main()
