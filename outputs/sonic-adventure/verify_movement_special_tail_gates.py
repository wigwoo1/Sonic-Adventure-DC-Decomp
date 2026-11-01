"""Additive whole-entry coverage of the special tail's input-controlled gates.

The published routine, reference, observer and earlier evidence stay unchanged.
Only ordinary actor, scene, geometry and parameter inputs are authored here.
"""
import hashlib
import itertools
import json
from fractions import Fraction
from pathlib import Path

import movement_special_tail_context_fixtures as inputs
import verify_movement_special_tail_direct as direct

P = Path(__file__).resolve().parent


def rounded_fraction(value, rm):
    """Independent exact rational-to-binary32 rounding for finite test inputs."""
    sign = int(value < 0)
    value = abs(value)
    lo, hi = 0, 0x7f7fffff
    while lo < hi:
        mid = (lo + hi + 1) // 2
        if Fraction(inputs.number(mid)) <= value:
            lo = mid
        else:
            hi = mid - 1
    low = Fraction(inputs.number(lo))
    if not rm and low != value:
        assert lo < 0x7f7fffff
        upper = Fraction(inputs.number(lo + 1))
        if upper - value < value - low or (upper - value == value - low and lo & 1):
            lo += 1
    # These cases deliberately use finite normal results or zero. Do not model
    # untested denormal/overflow behavior by quietly extending this oracle.
    assert not lo or lo >= 0x800000
    return inputs.number(lo | (sign << 31))


def read_only_ending_capture():
    captures = []
    source = inputs.fast.record_input_code
    marker = "  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
    assert source.count(marker) == 1
    source = source.replace(marker, marker + """
  if target==0x8c06cd22 and pr[0]==0x8c078202:
   def ending_words(a):
    return tuple(int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff)+4*j,4),'little') for j in range(3))
   ending_capture.append((ending_words(0x8ca00020),ending_words(0x8ca10004)))
""")
    ns = dict(inputs.fast.compare.__globals__, ending_capture=captures)
    exec(compile(source, 'read_only_special_tail_ending_capture', 'exec'), ns)
    compare = ns['compare']

    def run(*args):
        captures.clear()
        return compare(*args)
    return run, captures


def configurations():
    modes = tuple(itertools.product((0, 4), (0, 1)))
    controls = (
        dict(table_value=0, cancel_oracle=True),
        dict(table_value=.5, cancel_oracle=True),
        dict(table_value=1),
        dict(table_value=.5, character_word=1, cancel_oracle=True),
        dict(table_value=.5, state_word_a=1),
        dict(table_value=.5, state_speed=2),
        dict(table_value=.5, slope=.5),
        dict(table_value=.5, lift=1),
        dict(table_value=.5, flags=0x4003),
    )
    for values, speed, (mode, rm) in itertools.product(controls, (0, 1, 3), modes):
        c = dict(kind='alternate_ending_gates', position=(.5, 0, .5),
                 velocity=(speed, 0, 0), flags=3, table_value=.5,
                 major=9, minor=2, mode=mode, rm=rm, **direct.scene())
        c.update(values)
        if c.get('cancel_oracle'):
            c['required_pc'] = 0x8c07823c
        yield c
    for velocity, count, owner, selected, (mode, rm) in itertools.product(
            ((0, -3, 0), (0, -8, 0)), (1, 2), (0, inputs.DATA),
            (0, inputs.world.OBJECTS), modes):
        yield dict(kind='quarter_turn_clamp', position=(.5, 0, .5),
                   angles=(0x4000, 0, 0), velocity=velocity,
                   previous_owner=owner, previous_object=selected,
                   mode=mode, rm=rm, required_pc=0x8c0779d2,
                   **direct.scene(count=count))
    for reference, prior, flags, lift, (mode, rm) in itertools.product(
            ((0, 1, 0), (0, -1, 0)), (0, 1), (0, 1), (0, 1), modes):
        yield dict(kind='prior_ground_and_reference', position=(.5, 0, .5),
                   velocity=(0, 0, 0), reference=reference,
                   prior_status=prior, flags=flags, lift=lift,
                   mode=mode, rm=rm, **direct.scene())
    for flags, character, (mode, rm) in itertools.product(
            (2, 0x400000), (0, 6), modes):
        yield dict(kind='object_type_filter', position=(.5, 0, .5),
                   velocity=(0, 0, 0), character_kind=character,
                   required_pc=0x8c077982 if character == 6 else 0x8c07798a,
                   mode=mode, rm=rm, **direct.scene(flags=flags))
    for yaw, velocity, (mode, rm) in itertools.product(
            (0x2000, 0xc000), ((0, 0, 0), (1, 0, 0)), modes):
        yield dict(kind='response_yaw_rebuild', position=(.5, 0, .5),
                   velocity=velocity, angles=(0, yaw, 0), mode=mode, rm=rm,
                   **direct.scene())
    slope = ((0, 0, 0), (0, 0, 4), (4, 2, 0))
    for normal, (mode, rm) in itertools.product(((0, 1, 0), (0, -1, 0)), modes):
        yield dict(kind='sloped_zero_motion', position=(.5, 0, .5),
                   velocity=(0, 0, 0), reference=normal,
                   mode=mode, rm=rm, **direct.scene(vertices=slope))


def main():
    inputs.fast.compare, captures = read_only_ending_capture()
    parent = P / 'verify_movement_special_tail_direct.py'
    source = parent.read_text()
    output = 'movement_special_tail_direct_verification.json'
    assert source.count(output) == 1
    source = source.replace(output, 'movement_special_tail_gates_verification.json')
    marker = "        seen.update(path)"
    assert source.count(marker) == 1
    source = source.replace(marker, """        if c.get('required_pc'):
            require(c['required_pc'] in path, 'source-guided intended gate executes')
        if c.get('cancel_oracle'):
            require(0x8c078278 in path, 'complete ending cancellation block executes')
            require(len(ending_capture)==1, 'one read-only pre-ending input snapshot')
            start,motion=ending_capture[0]
            expected=tuple(rounded_fraction(Fraction(q.number(a))-Fraction(q.number(b))/2,c['rm']) for a,b in zip(start,motion))
            require(vector(m,q.A+0x20)==expected, 'independent exact half-motion cancellation position')
            require(vector(m,q.B+4)==(0,0,0), 'independent cancellation zeros auxiliary velocity')
            oracles+=1
""" + marker)
    ns = dict(__name__='special_tail_gates_replay', __file__=str(parent))
    exec(compile(source, str(parent), 'exec'), ns)
    ns['q'] = inputs
    ns['configurations'] = configurations
    ns['ending_capture'] = captures
    ns['rounded_fraction'] = rounded_fraction
    ns['Fraction'] = Fraction
    ns['main']()
    path = P / 'analysis/movement_special_tail_gates_verification.json'
    r = json.loads(path.read_text())
    r['scope'] = ('Whole0777e0 original-entry comparisons with real original '
                  'world/contact producers and callees. Ordinary quarter-turn '
                  'clamp, class filters, prior-ground/reference, yaw, slope and '
                  'alternate-ending inputs. Exact half-motion cancellation '
                  'position from read-only pre-ending inputs and zero velocity '
                  'checked with independent exact rational rounding. Published '
                  'C/reference/observer/earlier proofs unchanged. Uncovered '
                  'rows/outcomes, nonlocal resumes, physical hardware and full '
                  'game remain unqualified; no new routine credit.')
    for f in (Path(__file__), P / 'movement_special_tail_context_fixtures.py'):
        r['sha256'][str(f)] = hashlib.sha256(f.read_bytes()).hexdigest()
    path.write_text(json.dumps(r, indent=2) + '\n')
    print('SPECIAL TAIL GATES PASS', r['original_native_comparisons'],
          r['independent_checks'], r['independent_oracle_cases'], flush=True)


if __name__ == '__main__':
    main()
