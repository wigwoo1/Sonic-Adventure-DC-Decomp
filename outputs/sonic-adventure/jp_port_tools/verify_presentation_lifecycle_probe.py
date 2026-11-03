"""Qualify retained ownership, actual task hooks, and replay callback records.

The controlled fixture does not execute rasterization or the full frame loop.
"""
import argparse
import ctypes as C
import json
import random
from pathlib import Path
from native_task_test_support_v2 import Runtime, Task, Work, Layout, sha
from native_clock_test_support import Clock, Counter
from verify_native_task_runtime_v2 import basic_heap_and_pool, allocation_failures

P, I, U = C.c_void_p, C.c_int, C.c_uint32


class Probe(Runtime):
    def __init__(self, directory):
        self.directory = directory.resolve()
        self.build = json.loads((directory / 'build_verification.json').read_text())
        assert self.build['status'] == 'actual_presentation_lifecycle_probe_linked'
        self.dll = Path(self.build['DLL'])
        assert sha(self.dll) == self.build['DLL_sha256']
        self.clock = Clock(self.dll)
        self.lib = self.clock.lib
        self.checks, self.callback_errors, self.callbacks = 0, [], []
        signatures = {
            'jp_task_runtime_get_layout': (None, C.POINTER(Layout)),
            'jp_heap_allocate': (P, I), 'jp_heap_allocate_zeroed': (P, I, I),
            'jp_heap_release': (None, P), 'jp_heap_statistics': (None, C.POINTER(I), C.POINTER(I)),
            'task_create': (P, C.c_uint16, I, P), 'task_create_child': (P, C.c_uint16, P, P),
            'present_draw': (None, P, P), 'present_run': (None, P, P), 'present_forget': (None, P),
            'jp_lifecycle_reset': (None, I, I), 'jp_lifecycle_seed': (I, P, U),
            'jp_lifecycle_owner': (P, I, I), 'jp_lifecycle_owner_kind': (I, I, I),
            'jp_lifecycle_flags': (U, P), 'jp_lifecycle_replays': (I, I, I, I),
            'jp_lifecycle_redraw': (None, I), 'jp_lifecycle_camera': (None, P),
            'jp_lifecycle_wide': (None, C.c_float, I), 'jp_lifecycle_anchor': (C.c_float,),
            'jp_lifecycle_bind_player': (P, P, I, I),
            'jp_lifecycle_unbind_player': (None, P),
            'jp_lifecycle_region': (P, P, I, C.POINTER(U)),
        }
        for name in ['jp_heap_reset', 'task_init', 'task_step_all', 'task_draw_all', 'task_destroy_all']:
            signatures[name] = (None,)
        for name in ['task_destroy', 'task_kill', 'task_step_children', 'task_draw_children']:
            signatures[name] = (None, P)
        for name in ['jp_lifecycle_shadow_count', 'jp_lifecycle_cover', 'jp_lifecycle_lost']:
            signatures[name] = (I,)
        signatures['jp_lifecycle_current'] = (P,)
        for name, (result, *args) in signatures.items():
            fn = getattr(self.lib, name)
            fn.restype, fn.argtypes = result, args
        self.layout = Layout()
        self.lib.jp_task_runtime_get_layout(C.byref(self.layout))
        self.verify(self.layout.task_bytes == C.sizeof(Task), 'task ABI')
        self.verify(self.layout.work_bytes == C.sizeof(Work), 'work ABI')
        self.noop = self.callback(lambda task: None)

    def fresh(self, keep=1, redraw=0):
        self.call('jp_lifecycle_reset', 0, 0)
        base = super().fresh()
        self.call('jp_lifecycle_reset', keep, redraw)
        self.counter = Counter()
        self.verify(self.counter.start(self.clock, 1000000) == 0, 'clock initialized')
        self.clock.write(0xa2)
        return base

    def new(self, flags=2, level=0, step=None):
        task = self.call('task_create', flags, level, self.noop if step is None else step)
        self.verify(bool(task), 'real task allocated')
        return task

    def region(self, task, which):
        size = U()
        address = self.call('jp_lifecycle_region', task, which, C.byref(size))
        return address, size.value


def deletion(t):
    rng = random.Random(0x4c494645)
    for iteration in range(64):
        base = t.fresh(keep=1 + iteration % 2)
        tasks = [t.new() for _ in range(5)]
        victim = tasks[2]
        with_destructor = iteration % 2
        observed = []
        for task in [tasks[0], victim, tasks[1], victim, tasks[3], tasks[4]]:
            t.verify(t.call('jp_lifecycle_seed', task, rng.randrange(1, 128)) == 1, 'retained records seeded')
        def destructor(address):
            observed.append(address)
            t.verify(t.call('jp_lifecycle_flags', address) == 0, 'marks cleared before destructor')
            t.verify(t.call('jp_lifecycle_owner_kind', 0, 1) == 3, 'draw released before destructor')
            t.verify(t.call('jp_lifecycle_shadow_count') == 4, 'shadows removed before destructor')
            t.verify(bool(Task.from_address(address).work), 'work survives until destructor completes')
        if with_destructor:
            Task.from_address(victim).destructor = t.callback(destructor)
        initial = [0, 0xa0, 0xa2, 0xff][iteration % 4]
        t.clock.write(initial)
        t.counter.tick += 100
        timer_before = t.clock.timer()
        t.call('task_destroy', victim)
        t.verify(observed == ([victim] if with_destructor else []), 'destructor exactly once')
        t.verify(t.clock.tstr() == initial and t.clock.timer() == timer_before, 'cleanup preserves timer start bits')
        t.verify(t.call('jp_lifecycle_flags', victim) == 0, 'destroyed task flags cleared')
        for list_index in range(4):
            for record in range(6):
                is_victim = record in [1, 3]
                kind = t.call('jp_lifecycle_owner_kind', list_index, record)
                t.verify(kind == ((3 if with_destructor else 2) if is_victim else 1), 'retained owner safely retagged')
                if not is_victim:
                    t.verify(t.call('jp_lifecycle_owner', list_index, record) ==
                             [tasks[0], victim, tasks[1], victim, tasks[3], tasks[4]][record], 'unrelated owner unchanged')
        t.verify([t.call('jp_lifecycle_owner', 4, i) for i in range(4)] ==
                 [tasks[0], tasks[1], tasks[3], tasks[4]], 'stable shadow compaction')
        for index in [1, 3]:
            t.verify(t.call('jp_lifecycle_replays', index, 0, 1) == (not with_destructor), 'released sprite never replays')
            t.verify(t.call('jp_lifecycle_replays', index, 1, 0) == 0, 'deleted model not held in second picture')
        replacement = t.new()
        t.verify(replacement == victim, 'actual pool immediately reuses freed slot')
        t.verify(t.call('jp_lifecycle_flags', replacement) == 0, 'reused slot inherits no marks')
        t.call('jp_lifecycle_seed', replacement, 0x7f)
        for list_index in range(4):
            t.verify(t.call('jp_lifecycle_owner_kind', list_index, 1) == (3 if with_destructor else 2),
                     'slot reuse cannot relabel old records')
        for task in tasks[:2] + tasks[3:] + [replacement]:
            t.call('task_destroy', task)
        t.verify(t.stats() == base, 'destruction and slot reuse release real heap records')
    # KEEP_NONE intentionally leaves lists alone; such lists must not be replayed.
    t.fresh(keep=0)
    task = t.new()
    t.call('jp_lifecycle_seed', task, 7)
    t.call('present_forget', task)
    t.verify(t.call('jp_lifecycle_owner', 0, 0) == task and t.call('jp_lifecycle_flags', task) == 7,
             'inactive retention cleanup is a no-op')
    t.call('task_destroy', task)
    return dict(real_deletion_and_reuse_cycles=64, keeper_modes=[0, 1, 2])


def scheduling(t):
    t.fresh()
    seen = []
    def child_step(address):
        seen.append(('child', address))
        t.verify(t.call('jp_lifecycle_current') == address, 'child current-task identity')
        t.verify(t.call('jp_lifecycle_anchor') == 91 and t.call('jp_lifecycle_cover') == 1, 'child inherits parent aspect state')
        t.call('jp_lifecycle_wide', -72, 0)
    def parent_step(address):
        seen.append(('parent', address))
        t.verify(t.call('jp_lifecycle_current') == address, 'root current-task identity')
        t.verify(t.call('jp_lifecycle_anchor') == 0 and t.call('jp_lifecycle_cover') == 0, 'root starts centered')
        t.call('jp_lifecycle_wide', 91, 1)
        t.call('task_step_children', address)
        t.verify(t.call('jp_lifecycle_current') == address, 'nested step restores parent identity')
        t.verify(t.call('jp_lifecycle_anchor') == 91 and t.call('jp_lifecycle_cover') == 1, 'nested step restores parent aspect')
    parent = t.new(step=t.callback(parent_step))
    child = t.call('task_create_child', 2, t.callback(child_step), parent)
    t.verify(bool(child), 'real child task created')
    t.call('task_step_all')
    t.verify(seen == [('parent', parent), ('child', child)], 'actual root/child hooks execute once')
    t.verify(t.call('jp_lifecycle_current') is None and t.call('jp_lifecycle_anchor') == 0, 'outer state restored')
    t.call('jp_lifecycle_redraw', 1)
    t.call('task_step_all')
    t.call('task_step_children', parent)
    t.verify(len(seen) == 2, 'replay suppresses real root and child simulation steps')
    drawn = []
    parent_work = Work.from_address(Task.from_address(parent).work)
    child_work = Work.from_address(Task.from_address(child).work)
    parent_work.timer, child_work.timer = 123, 234
    def child_draw(address):
        drawn.append(address)
        t.verify(t.call('jp_lifecycle_current') == child, 'child draw receives its own replay context')
        child_work.timer = 888
    def parent_draw(address):
        drawn.append(address)
        parent_work.timer = 777
        t.call('task_draw_children', address)
        t.verify(t.call('jp_lifecycle_current') == parent, 'child draw restores parent replay context')
    Task.from_address(parent).draw = t.callback(parent_draw)
    Task.from_address(child).draw = t.callback(child_draw)
    t.call('task_draw_all')
    # task_draw_level also traverses children after the root callback. This
    # fixture deliberately draws the child inside that callback as well.
    t.verify(drawn == [parent, child, child], 'nested plus automatic child draw hooks route replay')
    t.verify((parent_work.timer, child_work.timer) == (123, 234), 'both nested draw snapshots restored')
    t.call('task_destroy_all')
    return dict(root_and_child_hooks=True, root_and_child_draw_hooks=True, replay_suppresses_simulation=True)


def callbacks(t):
    rng = random.Random(0x52454353)
    for iteration in range(48):
        t.fresh(redraw=1)
        task = t.new()
        work = Work.from_address(Task.from_address(task).work)
        work.timer = rng.randrange(-32768, 32768)
        work.pos.x, work.pos.y, work.pos.z = 11.5, -4, 908
        before = bytes(work)
        calls = []
        def draw(address):
            calls.append(address)
            t.verify(t.call('jp_lifecycle_current') == task, 'replay callback current-task identity')
            t.verify(t.call('jp_lifecycle_anchor') == 0, 'draw starts centered')
            w = Work.from_address(Task.from_address(address).work)
            w.timer = 222
            w.pos.x, w.pos.y, w.pos.z = 1, 2, 3
            t.call('jp_lifecycle_wide', 77, 1)
        Task.from_address(task).draw = t.callback(draw)
        t.call('task_draw_all')
        t.verify(calls == [task] and bytes(work) == before, 'extra callback restores complete task work')
        t.verify(t.call('jp_lifecycle_current') is None and t.call('jp_lifecycle_anchor') == 0 and
                 t.call('jp_lifecycle_cover') == 0, 'callback restores outer identity/aspect')
        for suppression in ['camera', 'later', 'killed']:
            if suppression == 'camera': t.call('jp_lifecycle_camera', task)
            if suppression == 'later':
                t.call('jp_lifecycle_camera', None)
                t.call('jp_lifecycle_seed', task, 1)
            if suppression == 'killed': t.call('task_kill', task)
            t.call('task_draw_all')
            t.verify(calls == [task], suppression + ' callback suppressed')
        t.call('task_destroy', task)
    t.fresh(redraw=1)
    task = t.new()
    t.call('jp_lifecycle_seed', task, 0x20)
    t.call('present_draw', task, None)
    t.verify(t.call('jp_lifecycle_flags', task) == 0x22, 'no callback marks held draws')
    t.call('present_draw', task, Task.from_address(task).step)
    t.verify(t.call('jp_lifecycle_flags', task) == 0x22, 'step-as-draw held without stepping')
    work = Work.from_address(Task.from_address(task).work)
    callback = t.callback(lambda address: setattr(work, 'timer', 423))
    t.call('jp_lifecycle_redraw', 0)
    t.call('present_draw', task, callback)
    t.verify(work.timer == 423, 'ordinary draw keeps original callback changes')
    t.call('task_destroy', task)
    return dict(replay_restore_cycles=48, guard_cases=['camera', 'later', 'killed', 'no_callback', 'step_as_draw'])


def players(t):
    cases = 0
    for slot in range(8):
        for count in range(10):
            base = t.fresh(redraw=1)
            task = t.new(flags=3)
            record = t.call('jp_lifecycle_bind_player', task, slot, count)
            t.verify(bool(record), 'player/motion/shapes allocated using real heap')
            regions = [t.region(task, which) for which in range(3)]
            for which, (address, size) in enumerate(regions):
                if size: C.memset(address, (which + 1) * 17, size)
            before = [C.string_at(address, size) if size else b'' for address, size in regions]
            calls = []
            def draw(address):
                calls.append(address)
                for region, size in regions:
                    if size: C.memset(region, 0x5a, size)
                Work.from_address(Task.from_address(address).work).pos.x = 777
            work = Work.from_address(Task.from_address(task).work)
            work.pos.x = -123
            before_work = bytes(work)
            t.call('present_draw', task, t.callback(draw))
            t.verify(len(calls) == (count <= 8), 'overflow refuses callback before mutation')
            t.verify(t.call('jp_lifecycle_lost') == (count > 8), 'shape-capacity fallback explicit')
            t.verify(bytes(work) == before_work, 'player work restored')
            for (region, size), expected in zip(regions, before):
                t.verify((C.string_at(region, size) if size else b'') == expected, 'player/motion/shapes restored byte-for-byte')
            t.call('task_destroy', task)
            t.call('jp_heap_release', record)
            t.verify(t.stats() == base, 'player fixture and lifecycle free complete allocation')
            cases += 1
    return dict(player_slot_and_shape_cases=cases, supported_maximum_shapes=8)


def lifetime_mutation(t):
    t.verify(t.build.get('replay_lifetime_guard_linked'), 'real lifetime guard linked')
    cases = 0
    for keep in [0, 1, 2]:
        for flags in [0, 2, 3]:
            for reuse in [False, True]:
                base = t.fresh(keep=keep, redraw=1)
                task = t.new(flags=flags)
                player = t.call('jp_lifecycle_bind_player', task, 0, 8) if flags == 3 else None
                if flags & 2: Work.from_address(Task.from_address(task).work).timer = 123
                replacements = []
                def delete_self(address):
                    if player: t.call('jp_lifecycle_unbind_player', address)
                    t.call('task_destroy', address)
                    if player: t.call('jp_heap_release', player)
                    if reuse:
                        new = t.new(flags=2)
                        replacements.append(new)
                        Work.from_address(Task.from_address(new).work).timer = 777
                t.call('present_draw', task, t.callback(delete_self))
                if reuse:
                    new = replacements[0]
                    t.verify(new == task, 'destructive callback immediately reuses same slot')
                    t.verify(Work.from_address(Task.from_address(new).work).timer == 777,
                             'old snapshot never overwrites replacement records')
                    t.call('task_destroy', new)
                else:
                    t.verify(Task.from_address(task).work is None, 'no restore through freed work pointer')
                t.verify(t.call('jp_lifecycle_current') is None, 'destructive callback restores current context')
                t.verify(t.stats() == base, 'destructive replay cleanup restores complete heap')
                # A later unrelated replay still has a paired, valid scope.
                later = t.new()
                later_work = Work.from_address(Task.from_address(later).work)
                later_work.timer = 456
                t.call('present_draw', later, t.callback(lambda address: setattr(later_work, 'timer', 999)))
                t.verify(later_work.timer == 456, 'scope unwinds after invalidation')
                t.call('task_destroy', later)
                cases += 1
    for keep in [0, 1, 2]:
        for delete_outer in [False, True]:
            base = t.fresh(keep=keep, redraw=1)
            outer, inner = t.new(), t.new()
            Work.from_address(Task.from_address(outer).work).timer = 123
            Work.from_address(Task.from_address(inner).work).timer = 234
            replacements = []
            def inner_draw(address):
                victim = outer if delete_outer else inner
                t.call('task_destroy', victim)
                new = t.new()
                replacements.append(new)
                Work.from_address(Task.from_address(new).work).timer = 777
            def outer_draw(address):
                Work.from_address(Task.from_address(outer).work).timer = 345
                t.call('present_draw', inner, inner_callback)
            inner_callback = t.callback(inner_draw)
            t.call('present_draw', outer, t.callback(outer_draw))
            replacement = replacements[0]
            t.verify(replacement == (outer if delete_outer else inner), 'nested deletion reuses expected identity')
            t.verify(Work.from_address(Task.from_address(replacement).work).timer == 777,
                     'nested invalidation protects replacement')
            survivor = inner if delete_outer else outer
            t.verify(Work.from_address(Task.from_address(survivor).work).timer == (234 if delete_outer else 123),
                     'unrelated active scope still restores original snapshot')
            t.call('task_destroy', survivor)
            t.call('task_destroy', replacement)
            t.verify(t.stats() == base and t.call('jp_lifecycle_current') is None,
                     'nested scopes leave no lifetime or heap state')
            cases += 1
    return dict(destructive_callback_cases=cases, keeper_modes=[0, 1, 2],
                player_self_destruction_and_reuse=True, nested_outer_invalidation=True)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', type=Path, required=True)
    ap.add_argument('--report', type=Path, required=True)
    a = ap.parse_args()
    assert not a.report.exists()
    t = Probe(a.build)
    groups = {}
    for name, test in [('deletion', deletion), ('scheduling', scheduling), ('callbacks', callbacks),
                       ('players', players), ('lifetime_mutation', lifetime_mutation),
                       ('heap_regression', basic_heap_and_pool),
                       ('allocation_rollback_regression', allocation_failures)]:
        before = t.checks
        detail = test(t)
        groups[name] = dict(checks=t.checks - before, details=detail)
        print(name, groups[name]['checks'], 'checks passed', flush=True)
    t.clock.shutdown()
    report = dict(status='actual_task_retention_and_supported_replay_callbacks_verified', checks=t.checks,
                  groups=groups, DLL_sha256=sha(t.dll), build_report_sha256=sha(a.build / 'build_verification.json'),
                  verifier_sha256=sha(Path(__file__)),
                  support_sha256=sha(Path(__file__).with_name('native_task_test_support_v2.py')),
                  clock_support_sha256=sha(Path(__file__).with_name('native_clock_test_support.py')),
                  heap_regression_verifier_sha256=sha(Path(__file__).with_name('verify_native_task_runtime_v2.py')),
                  full_presentation_linked=False, rendered_game_image=False, gameplay_verified=False,
                  limitations=['Isolated actual functions and complete task object; no renderer or frame loop execution.',
                               'Task deletion/reuse is qualified; replacing/freeing individual record pointers without task destruction remains unqualified.',
                               'The eight-shape fallback prevents replay; it does not make malformed records safe.',
                               'Windows x64 host ABI only; no 60/120/240 FPS gameplay or localization claim.'],
                  game_containing_artifact_publishable=False, overall_completion_percent=10,
                  goal_paused=False, playable=False)
    a.report.write_text(json.dumps(report, indent=2) + '\n')
    print(t.checks, 'lifecycle integration checks passed')


if __name__ == '__main__':
    main()
