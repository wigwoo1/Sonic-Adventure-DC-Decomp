"""Original SH-4 versus native spawn/setter and connected notification callers."""
import ctypes
import gc
import hashlib
import itertools
import json
import sys
from pathlib import Path
import verify_main_dispatch_notification as base
import sh4_binary32

PROJECT = Path(__file__).resolve().parent
DLL = base.ROOT / 'work/sonic-adventure/build/main_dispatch_spawn_setter.dll'
LIB = ctypes.CDLL(str(DLL))
SPAWN, SET, CANCEL = base.previous.SPAWN, base.previous.SET, base.previous.CANCEL
TABLE, HANDLERS = base.literal(0x8C07B300), base.literal(0x8C07B304)
OUTER, OWNER, OLDHEAD = 0x8CE90000, 0x8CEA0000, 0x8CEB0000
SPAWN_CALLBACK = base.literal(0x8C07B4C0)
SENTINEL = 0xBADC0FFE
load, store = base.load, base.store
original_prepare = base.prepare
LAST = {}


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def captured(fn):
    def invoke(*args):
        result = fn(*args)
        LAST['registers'] = list(args[3])
        return result
    return invoke


FUNCTIONS = {}
for key, name in [*((k, fn.__name__) for k, fn in base.FUNCTIONS.items()),
                  ('spawn', 'sa_main_dispatch_spawn_connected'),
                  ('set', 'sa_main_dispatch_callback_set_connected')]:
    fn = getattr(LIB, name)
    fn.argtypes = base.previous.ARGS + [base.OBSERVE]
    fn.restype = ctypes.c_int
    FUNCTIONS[key] = captured(fn)


def prepare(c, n):
    entry = c['entry']
    memory, state, fpu = original_prepare(dict(c, entry='notify' if entry in ('spawn', 'set') else entry), n)
    # A well-formed doubly-linked free-task list, not arbitrary stale prev words.
    store(memory, base.TASK + 4, 0)
    store(memory, base.NEXTTASK + 4, base.TASK)
    if c.get('old_head'):
        store(memory, base.LEVELS + 6 * 4, OLDHEAD)
        store(memory, OLDHEAD, 0)
        store(memory, OLDHEAD + 4, 0)
    slot = c.get('slot', 0) & 255
    path = c.get('pointer_path', 2)
    owner = TABLE + slot * 4 - 12 if c.get('owner_alias_slot') else OWNER
    store(memory, TABLE + slot * 4, OUTER if path else 0)
    store(memory, OUTER + 0x70, owner if path == 2 else 0)
    # Aliased destinations must be initialized before publishing the table entry.
    if not c.get('owner_alias_slot'):
        store(memory, owner + 12, SENTINEL)
    index = c.get('index', 8)
    offset = (index << 2) & 0xFFFFFFFF
    handler_slot = (HANDLERS + offset) & 0xFFFFFFFF
    callback = c.get('callback', (0x8C044680 ^ offset) & 0xFFFFFFFF)
    store(memory, handler_slot, callback)
    if entry == 'spawn':
        source = {'object': base.previous.OBJECT, 'task': base.TASK,
                  'work': base.ARENA + 16}[c.get('source_alias', 'object')]
        state.pc = SPAWN
        state.r[4] = source
        store(memory, source + 8, c.get('character', 255), 1)
    elif entry == 'set':
        state.pc = SET
        state.r[4] = c.get('slot', 0)
        state.r[5] = index & 0xFFFFFFFF
    LAST.clear()
    LAST.update(memory=memory, owner=owner, callback=callback,
                destination_before=load(memory, owner + 12))
    return memory, state, fpu


def cases():
    yield from base.cases()
    for heap, empty, character, mode in itertools.product((0, 16, 64, 80, 128), (False, True), (0, 1, 127, 128, 255), range(8)):
        yield dict(entry='spawn', heap=heap, task_empty=empty, character=character, mode=mode << 19)
    for heap, character, mode in itertools.product((64, 128), (0, 255), range(8)):
        yield dict(entry='spawn', heap=heap, character=character, mode=mode << 19, old_head=True)
    for alias, character, mode in itertools.product(('task', 'work'), (0, 1, 127, 128, 255), range(8)):
        yield dict(entry='spawn', heap=128, character=character, mode=mode << 19, source_alias=alias)
    for slot, index, path, mode in itertools.product((0, 255, 256, 0xABCDEF80), (0, 1, 8, 0xFFFFFFFF, 0x40000000), range(3), range(8)):
        yield dict(entry='set', slot=slot, index=index, pointer_path=path, mode=mode << 19, null_external=True)
    for callback, mode in itertools.product((0, 0xDEADBEEF, 0xFFFFFFFF), range(8)):
        yield dict(entry='set', slot=255, index=8, pointer_path=2, callback=callback, mode=mode << 19, owner_alias_slot=True)
    # Exercise native spawn after the notification consumes a first heap block.
    for state, availability, heap, character, mode in itertools.product(range(46, 51), (0, 2, 4), (64, 128, 256), (0, 128, 255), (0, 7)):
        yield dict(entry='gate', state=state, available=availability, heap=heap, id=character, mode=mode << 19, null_external=True)
    for state, path, mode in itertools.product((0, 44, 45, 51, 128), range(3), range(8)):
        yield dict(entry='gate', state=state, pointer_path=path, mode=mode << 19)


def setup():
    translation_path = PROJECT / 'analysis/main_dispatch_spawn_setter_translation.json'
    translation = json.loads(translation_path.read_text())
    inventory_path = PROJECT / 'analysis/main_dispatch_task_disposal_inventory.json'
    inventory = json.loads(inventory_path.read_text())
    baseline = PROJECT / 'analysis/main_dispatch_notification_cleanup_verification.json'
    for report in (translation, inventory, json.loads(baseline.read_text())):
        for p, h in report['sha256'].items():
            assert digest(Path(p)) == h, p
    base.FUNCTIONS = FUNCTIONS
    base.prepare = prepare
    base.EXTERNAL = {CANCEL}
    base.TRACKED |= {SPAWN, SET, base.DISPOSE, 0x8C098998, 0x8C098E60, 0x8C098E98, 0x8C098B42}
    allowed = set()
    for report in (translation, inventory,
                   json.loads((PROJECT / 'analysis/main_dispatch_notification_translation.json').read_text()),
                   json.loads((PROJECT / 'analysis/main_dispatch_sound_translation.json').read_text())):
        for routine in report['routines']:
            allowed.update(int(a, 16) for a in routine['reachable_addresses'])
    for start, end in ((base.previous.START, base.previous.END), (0x8C604370, 0x8C604376),
                       (0x8C646868, 0x8C646876), (0x8C65F3A0, 0x8C65F3B2),
                       (0x8C10CA5C, 0x8C10CB06), (0x8C09852E, 0x8C0985EC),
                       (0x8C0985EC, 0x8C09865C), (0x8C098A80, 0x8C098B0C), (0x8C098B0C, 0x8C098B42)):
        allowed.update(range(start, end, 2))
    sh4_binary32.execute_multiply = base.previous.multiply_expected
    return translation_path, translation, inventory_path, inventory, baseline, allowed


def independent(c, path, status):
    if status:
        return []
    m, checks = LAST['memory'], []
    if c['entry'] == 'spawn':
        created = not c.get('task_empty') and c.get('heap', 128) >= 80
        if created:
            work = base.ARENA + 16
            character = 0 if c.get('source_alias') in ('task', 'work') else c.get('character', 255)
            assert load(m, base.LEVELS + 24) == base.TASK
            assert load(m, base.TASK + 0x10) == SPAWN_CALLBACK
            assert load(m, base.TASK + 0x20) == work
            assert [load(m, work + i, 1) for i in range(64)] == [character if i == 8 else 0 for i in range(64)]
            assert LAST['registers'][0] == (character if character < 128 else character - 256) & 0xFFFFFFFF
            assert load(m, base.TASK) == (OLDHEAD if c.get('old_head') else 0)
            if c.get('old_head'):
                assert load(m, OLDHEAD + 4) == base.TASK
            assert load(m, base.FREE_TASK) == base.NEXTTASK
            checks.append('spawn_success_work_callback_list_and_signed_byte')
        else:
            assert LAST['registers'][0] == 0
            assert load(m, base.FREE_TASK) == (0 if c.get('task_empty') else base.TASK)
            assert load(m, base.LEVELS + 24) == (OLDHEAD if c.get('old_head') else 0)
            size = c.get('heap', 128)
            assert load(m, base.FREE_HEAP) == (base.ARENA if size else 0)
            if size:
                assert [load(m, base.ARENA + 4 * i) for i in range(4)] == [base.ARENA + size, 0, base.ARENA + size, 0]
            checks.append('spawn_failure_free_task_level_and_heap_preserved')
    if SET in path or c['entry'] == 'set':
        expected = LAST['callback'] if c.get('pointer_path', 2) == 2 else LAST['destination_before']
        assert load(m, LAST['owner'] + 12) == expected
        if c['entry'] == 'set' and c.get('pointer_path', 2) == 2:
            assert LAST['registers'][5] == (c.get('index', 8) << 2) & 0xFFFFFFFF
        checks.append('callback_store_or_null_noop_without_callback_execution')
    if c['entry'] == 'gate' and SPAWN in path:
        assert load(m, base.previous.OBJECT, 1) == 50
        assert load(m, base.previous.WORK + 0x124, 2) == 46
        spawn_task = load(m, base.LEVELS + 24)
        if spawn_task:
            work = load(m, spawn_task + 0x20)
            assert load(m, spawn_task + 0x10) == SPAWN_CALLBACK
            assert load(m, work + 8, 1) == c.get('id', 255)
        checks.append('gate_motion_and_native_spawn_payload')
    return checks


def main():
    translation_path, translation, inventory_path, inventory, baseline, allowed = setup()
    rows = set(); edges = set(); calls = set(); records = []; failures = {}
    new_rows = {int(a, 16) for r in translation['routines'] for a in r['reachable_addresses']}
    totals = dict(boundary_comparisons=0, partial_guard_error_cases=0, independent_invariants=0,
                  completed_without_external_calls=0)
    counts = dict.fromkeys(FUNCTIONS, 0)

    def check(c, n):
        path, boundaries, status, invariant, external = base.run(c, n, allowed)
        rows.update(set(path) & new_rows)
        for i, address in enumerate(path):
            if address not in new_rows:
                continue
            word = int.from_bytes(base.binary[address - 0x8C010000:address - 0x8C010000 + 2], 'little')
            if word >> 8 in (0x89, 0x8B, 0x8D, 0x8F):
                nxt = i + (2 if word >> 8 in (0x8D, 0x8F) else 1)
                if nxt < len(path):
                    edges.add((address, path[nxt]))
            if word >> 12 == 0xB or word & 0xF0FF in (0x400B, 0x402B):
                calls.add(address)
        checks = independent(c, path, status)
        counts[c['entry']] += 1
        totals['boundary_comparisons'] += boundaries
        totals['partial_guard_error_cases'] += status != 0
        totals['independent_invariants'] += bool(checks) or invariant
        totals['completed_without_external_calls'] += status == 0 and not external
        if status == 0:
            for target in external:
                failures.setdefault(target, c)
        records.append(dict(input=c, steps=len(path), boundary_comparisons=boundaries, status=status,
                            independent_checks=checks, inherited_invariant=invariant,
                            external_calls=[hex(t) for t in external], full_state_match=True))

    if '--smoke' in sys.argv:
        for n, c in enumerate([
            dict(entry='spawn', heap=128, character=255), dict(entry='spawn', heap=64),
            dict(entry='spawn', heap=80, old_head=True, character=0), dict(entry='spawn', task_empty=True),
            dict(entry='spawn', source_alias='task'), dict(entry='spawn', source_alias='work'),
            *[dict(entry='set', pointer_path=p, slot=0xABCDEF80, index=0xFFFFFFFF) for p in range(3)],
            dict(entry='set', owner_alias_slot=True, slot=255, callback=0),
            dict(entry='gate', state=48, heap=256, id=128), dict(entry='gate', state=0, pointer_path=2),
            dict(entry='gate', state=48, heap=128, id=255), dict(entry='notify'),
        ]):
            check(c, n)
        print('Smoke checks passed:', len(records), counts)
        return
    for n, c in enumerate(cases()):
        check(c, n)
        if (n + 1) % 200 == 0:
            print(n + 1, 'spawn/setter and connected comparisons passed', flush=True)
        if (n + 1) % 25 == 0:
            gc.collect()
    assert rows == new_rows, ('uncovered rows', [hex(a) for a in new_rows - rows])
    assert edges == {tuple(int(a, 16) for a in edge) for r in translation['routines'] for edge in r['conditional_edges']}
    assert calls == {int(a, 16) for r in translation['routines'] for a in r['external_call_sites']}
    for target, c in sorted(failures.items()):
        check(dict(c, fail_target=target), len(records))
    files = [Path(__file__), Path(base.__file__), PROJECT/'verify_main_dispatch_prefix_gate.py',
             PROJECT/'verify_startup.py', translation_path, inventory_path, baseline,
             PROJECT/'build_main_dispatch_spawn_setter.ps1', PROJECT/'source/sa_main_dispatch_spawn_setter.c',
             PROJECT/'source/sa_main_dispatch_spawn_setter_connected.c', PROJECT/'analysis/main_dispatch_notification_translation.json',
             PROJECT/'analysis/main_dispatch_sound_translation.json']
    for name in ('sa_main_dispatch_notification.c', 'sa_main_dispatch_prefix_gate.c', 'sa_main_dispatch_sound.c',
                 'sa_input_channel_info_pointer.c', 'sa_input_device_record_index_helpers.c', 'callback_tail_thunks.c',
                 'sa_signed_division.c', 'sa_task_create.c', 'sa_task_create_base.c', 'sa_heap_allocate.c',
                 'sa_heap_allocate_zeroed.c', 'sh4_multiply_extended.c', 'sh4_multiply.c', 'sh4_fmac.c', 'fpu_cleanup_transfer.h'):
        files.append(PROJECT/'source'/name)
    files.extend(PROJECT/r['source'] for r in inventory['routines'])
    report = dict(source=translation['source'], source_sha256=digest(PROJECT/translation['source']),
                  routines=translation['routines'], cases_passed=len(records), counts=counts,
                  rows_covered=len(rows), conditional_edges_covered=len(edges), call_sites_covered=len(calls),
                  **totals, service_error_cases=len(failures), sha256={str(p):digest(p) for p in files},
                  dll_sha256=digest(DLL), binary_sha256=digest(base.previous.BINARY),
                  scope='Two original integer helpers and native notification/prefix/task/heap/sound chain compared against retail instructions using full RAM/GPR/SR/PR/FPU/FPUL/MACL and ordered CPU boundary states; independent byte/work/list/heap/callback invariants.',
                  limitations=['Only cancel 8c01ae60 remains a service fixture in the tested prefix. Stored task/callback pointers are not invoked by these helpers; their eventual execution remains a separate dependency.',
                               'Constructed valid RAM, table and heap metadata; alias/index-wrap cases are instruction semantics checks, not claims of gameplay reachability. Inherited arithmetic precision/rounding guards remain.',
                               'No MMIO, hardware exception dispatch, concurrency, live gameplay, full decompilation or playable port proof.'], cases=records)
    (PROJECT/'analysis/main_dispatch_spawn_setter_verification.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('cases_passed', 'counts', 'rows_covered', 'conditional_edges_covered', 'call_sites_covered', *totals, 'service_error_cases')}, indent=2))


if __name__ == '__main__':
    try:
        main()
    finally:
        sh4_binary32.execute_multiply = base.previous.scalar.finite_execute
