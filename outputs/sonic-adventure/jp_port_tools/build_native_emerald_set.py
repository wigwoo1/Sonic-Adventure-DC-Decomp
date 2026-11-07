"""Link the original bounded SET placement initialization into the beach runtime."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


ADAPTER = br'''
_Static_assert(sizeof(struct set_entry) == 32, "SET entry ABI");
_Static_assert(sizeof(struct set_file) == 32, "SET header ABI");

__declspec(dllexport) int jp_emerald_set_close(void)
{
    clear_placements();
    placement_entries = NULL;
    placement_file_count = NULL;
    free(owned);
    owned = NULL;
    return 0;
}

/* Select a complete act atomically. The placement registry owns its copy
 * so destroying the disc resources cannot invalidate its entries. */
__declspec(dllexport) int jp_emerald_set_select(unsigned act)
{
    struct resource_view view;
    if (act >= 3 || jp_stage_resources_get(1 + act, &view) ||
        view.bytes < sizeof(struct set_file)) return -1;
    Uint32 count;
    memcpy(&count, view.data, sizeof count);
    if (count > PLACEMENT_COUNT ||
        count > (view.bytes - sizeof(struct set_file)) / sizeof(struct set_entry))
        return -1;
    size_t bytes = sizeof(struct set_file) + count * sizeof(struct set_entry);
    struct set_file *copy = malloc(bytes);
    if (!copy) return -1;
    memcpy(copy, view.data, bytes);
    jp_emerald_set_close();
    owned = copy;
    placement_file_count = &owned->count;
    placement_entries = (struct set_entry *)(owned + 1);
    add_placements();
    return placement_count;
}

__declspec(dllexport) int jp_emerald_set_get(unsigned index, struct set_entry *out)
{
    if (!out || index >= (unsigned)placement_count ||
        !(placements[index].flags & PLACEMENT_ACTIVE)) return -1;
    *out = *placements[index].entry;
    return 0;
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
    raw = pinned(upstream, 'src/object/placement.c')
    pinned(upstream, 'src/object/placement.h')
    clear = owner_function(raw, 'clear_placements')
    add = owner_function(raw, 'add_placements')
    assert add.count(b'i <= last;') == 1
    add = add.replace(b'i <= last;', b'i < last;')
    out.mkdir()
    unit, obj, dll = (out/name for name in ('emerald_set.c', 'emerald_set.obj', 'jp_native_frames.dll'))
    unit.write_bytes(br'''
#include <stdlib.h>
#include <string.h>
#include "object/placement.h"

struct resource_view { const void *data; size_t bytes; };
int jp_stage_resources_get(unsigned, struct resource_view *);
static struct set_file *owned;
static Uint32 *placement_file_count;
static struct set_entry *placement_entries;
static Sint16 placement_count;
static struct placement placements[PLACEMENT_COUNT];
''' + clear + add + ADAPTER)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = manifest['emerald_stage_loader']['commands']
    link = shlex.split((base/'link.rsp').read_text())
    link.insert(link.index('-o'), str(obj))
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
                    emerald_set=dict(parent_sha256=digest(parent), source_sha256=digest(unit),
                                     commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast placement initialization compiled and linked')


if __name__ == '__main__':
    main()
