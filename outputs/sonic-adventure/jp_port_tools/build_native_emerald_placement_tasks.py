"""Add the original single-player placement dispatch to the native SET owner."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


STATE = br'''
static struct object_kind kinds[84];
static struct object_list list = {84, kinds};
static struct object_list *placement_list = &list;
static struct task_work *center;
static int placed_once, retiring;

static void retire_tasks(void)
{
    retiring = 1;
    for (int i = 0; i < placement_count; ++i) {
        if (placements[i].flags & PLACEMENT_LIVE)
            task_destroy(placements[i].task);
    }
    retiring = 0;
    placed_once = 0;
}
'''

API = br'''
/* Configure callbacks between acts with the registry closed. A null callback
 * clears a binding. Run the placement and registry APIs between task ticks. */
__declspec(dllexport) int jp_emerald_kind_register(unsigned id, int level,
    int attr, float range, void (*step)(struct task *))
{
    if (id >= (unsigned)list.count || level < 0 || level > 6 ||
        attr < 0 || attr > 15 || !(range >= 0 && range <= FLT_MAX) ||
        owned || retiring) return -1;
    kinds[id] = (struct object_kind){TASK_WORK, level, attr, range, NULL, step, NULL};
    return 0;
}

__declspec(dllexport) int jp_emerald_placement_step(struct task_work *player,
                                                    struct task_work *camera)
{
    if (!owned || retiring || center) return -1;
    center = player ? player : camera;
    if (!center) return 0;
    place_near_one();
    placed_once = 1;
    center = NULL;
    int live = 0;
    for (int i = 0; i < placement_count; ++i)
        live += !!(placements[i].flags & PLACEMENT_LIVE);
    return live;
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (path.resolve() for path in
                                  (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    prior = base/'emerald_set.c'
    assert digest(prior) == manifest['emerald_set']['source_sha256']
    assert b'#define BEACH_KINDS 84' in pinned(upstream, 'modules/stg01/kinds.h')
    source = prior.read_bytes()
    raw = pinned(upstream, 'src/object/placement.c')
    place = owner_function(raw, 'place_near_one')
    begin = place.index(b'    if (player_work[0]')
    end = place.index(b'    x = center->pos.x;', begin)
    place = place[:begin] + place[end:]
    place = place.replace(b'    register struct task_work *center;\n', b'')
    place = place.replace(b'        kind = &placement_list->kinds[entry->id & SET_KIND];',
                          b'        unsigned id = entry->id & SET_KIND;\n'
                          b'        if (id >= (unsigned)placement_list->count ||\n'
                          b'            !placement_list->kinds[id].step) continue;\n'
                          b'        kind = &placement_list->kinds[id];')
    # This adapter has no saved-pose producer. Retain the original ordinary
    # placement path and keep saved-pose handling for the later leave adapter.
    place = place.replace(b'                    heap_free(placement->leave.pose);',
                          b'                    free(placement->leave.pose);')
    anchor = b'__declspec(dllexport) int jp_emerald_set_close(void)\n{'
    assert source.count(anchor) == 1
    source = source.replace(anchor, STATE + anchor +
                            b'\n    if (retiring || center) return -1;\n    retire_tasks();')
    anchor = b'    if (act >= 3 || jp_stage_resources_get'
    assert source.count(anchor) == 1
    source = source.replace(anchor, b'    if (retiring || center || act >= 3 || jp_stage_resources_get')
    source = b'#include <float.h>\n' + source + owner_function(raw, 'in_range') + place + API
    out.mkdir()
    unit, obj, dll = (out/name for name in ('placement_tasks.c', 'placement_tasks.obj', 'jp_native_frames.dll'))
    unit.write_bytes(source)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = manifest['emerald_set']['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'emerald_set.obj')
    link[link.index(old)] = str(obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_placement_tasks=dict(parent_sha256=digest(parent),
                                                 source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast placement task dispatch compiled and linked')


if __name__ == '__main__':
    main()
