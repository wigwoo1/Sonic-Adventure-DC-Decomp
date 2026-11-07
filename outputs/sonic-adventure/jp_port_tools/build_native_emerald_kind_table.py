"""Bind the native placement registry to Emerald Coast's loaded kind table."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest
from build_native_stage_start import pinned
from extract_native_emerald_collision import LOAD


API = br'''
/* Dreamcast data layout. The three addresses are provenance only. */
struct kind_record {
    uint8_t create, level;
    int16_t attr;
    float range;
    uint32_t param, step, name;
};
_Static_assert(sizeof(struct kind_record) == 20, "Dreamcast kind ABI");

__declspec(dllexport) int jp_emerald_kind_read(unsigned id, struct kind_record *out)
{
    struct resource_view view;
    if (!out || id >= sizeof kinds / sizeof kinds[0] || jp_stage_resources_get(0, &view) ||
        view.bytes < KIND_LIST_OFFSET + 8 ||
        view.bytes < KIND_TABLE_OFFSET + sizeof(struct kind_record) * (id + 1))
        return -1;
    const unsigned char *image = view.data;
    uint16_t count;
    uint32_t table_address;
    memcpy(&count, image + KIND_LIST_OFFSET, sizeof count);
    memcpy(&table_address, image + KIND_LIST_OFFSET + 4, sizeof table_address);
    if (count != sizeof kinds / sizeof kinds[0] || table_address != KIND_TABLE_ADDRESS)
        return -1;
    memcpy(out, image + KIND_TABLE_OFFSET + sizeof *out * id, sizeof *out);
    return 0;
}

/* The host binds code explicitly. Dreamcast addresses are never executed or
 * handed to callbacks as host pointers. Parameter relocation is separate. */
__declspec(dllexport) int jp_emerald_kind_bind(unsigned id, void (*step)(struct task *))
{
    if (owned || retiring || id >= sizeof kinds / sizeof kinds[0]) return -1;
    if (!step) {
        kinds[id] = (struct object_kind){0};
        return 0;
    }
    struct kind_record row;
    if (jp_emerald_kind_read(id, &row) || row.param || !row.step || row.level > 6 ||
        row.create & ~15u || row.attr < 0 || row.attr > 15 ||
        !(row.range >= 0 && row.range <= FLT_MAX)) return -1;
    kinds[id] = (struct object_kind){row.create, row.level, row.attr, row.range,
                                    NULL, step, NULL};
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
    prior = manifest['emerald_placement_tasks']
    source = base/'placement_tasks.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    symbols = pinned(upstream, 'config/stg01/symbols.txt').decode()
    address = {name: int(re.search(r'^(0x[0-9a-f]+)\s+' + symbol + r'\b', symbols, re.M)[1], 16)
               for name, symbol in (('TABLE', 'beach_object_kinds'), ('LIST', 'beach_object_list'))}
    defines = ''.join(f'#define KIND_{name}_OFFSET {value - LOAD}u\n'
                      for name, value in address.items())
    defines += f'#define KIND_TABLE_ADDRESS {address["TABLE"]}u\n'
    out.mkdir()
    unit, obj, dll = (out/name for name in ('kind_table.c', 'kind_table.obj', 'jp_native_frames.dll'))
    unit.write_bytes(b'#include <stdint.h>\n' + source.read_bytes() + defines.encode() + API)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'placement_tasks.obj')
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
                    emerald_kind_table=dict(symbol_addresses=address, source_sha256=digest(unit),
                                            parent_sha256=digest(parent), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast object kind table compiled and linked')


if __name__ == '__main__':
    main()
