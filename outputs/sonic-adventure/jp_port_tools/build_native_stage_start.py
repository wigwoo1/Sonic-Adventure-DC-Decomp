"""Build and check the original table-driven player placement branch."""
import argparse
import ctypes as C
import hashlib
import json
import re
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest

CHARACTERS = ('sonic', 'tails', 'knuckles', 'big', 'amy', 'e102')


def pinned(upstream, path):
    raw = (upstream/path).read_bytes()
    assert raw == subprocess.check_output(['git', '-C', str(upstream), 'show', PIN+':'+path])
    return raw


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    src = pinned(args.upstream, 'src/stage/stage.c')
    stage_h = pinned(args.upstream, 'src/stage/stage.h')
    player_h = pinned(args.upstream, 'src/player/player.h')
    tables = [re.search(rb'^static struct start_point '+name.encode()+rb'_starts\[\] = \{.*?^\};', src, re.M|re.S).group()
              for name in CHARACTERS]
    structure = re.search(rb'^struct start_point \{.*?^\};', src, re.M|re.S).group()
    original = owner_function(src, 'stage_place_player')
    branch = original.split(b'            switch (PLAYER_VARS(work)->character) {', 1)[1]
    branch = b'            switch (PLAYER_VARS(work)->character) {'+branch.split(b'\n        }\n    }', 1)[0]
    branch = branch.replace(b'            for (;;) {', b'            if (start == NULL) return 1;\n            for (;;) {')
    out.mkdir()
    unit = out/'stage_start.c'
    unit.write_bytes(b'#include <ninja.h>\n#include "stage/stage.h"\n#include "player/player.h"\n#include "system/task.h"\n'
                     +structure+b'\n'+b'\n'.join(tables)+b'''
__declspec(dllexport) int jp_stage_place_from_start_table(
    struct task_work *work, int stage_current, int act_current)
{
    struct start_point *start = NULL;
    if (work == NULL) return 1;
'''+branch+b'''
    return 0;
}

__declspec(dllexport) int jp_stage_start_probe(int character, int stage, int act,
                                             NJS_POINT3 *position, Angle *yaw)
{
    struct task_work work = {0};
    PLAYER_VARS(&work)->character = character;
    work.pos = *position;
    work.ang.x = 123;
    work.ang.y = *yaw;
    work.ang.z = 456;
    int status = jp_stage_place_from_start_table(&work, stage, act);
    if (work.ang.x != 123 || work.ang.z != 456) return 2;
    *position = work.pos;
    *yaw = work.ang.y;
    return status;
}
''')
    parent = args.baseline.resolve()/'build_verification.json'
    baseline = json.loads(parent.read_bytes())
    assert digest(baseline['frames_DLL']) == baseline['frames_DLL_sha256']
    recipe = baseline['sonic_draw']['commands'][0]
    dll = out/'jp_native_stage_start.dll'
    command = recipe[:recipe.index('-c')]+['-shared', str(unit), '-o', str(dll)]
    build = subprocess.run(command, capture_output=True)
    (out/'build.stderr').write_bytes(build.stderr)
    assert build.returncode == 0 and not build.stderr, build.stderr.decode(errors='replace')

    def constants(raw):
        return {name.decode(): int(number) for name, number in
                re.findall(rb'^#define ((?:STAGE|CHARACTER)_\w+) (\d+)\b', raw, re.M)}

    stages, characters = constants(stage_h), constants(player_h)
    stages['STAGE_NONE'] = stages['STAGE_COUNT']
    rows = re.compile(rb'\{(STAGE_\w+), (\d+), \{([^}]+)\}, (-?(?:0x[0-9a-fA-F]+|\d+))\},')
    owner = C.CDLL(str(dll))
    probe = owner.jp_stage_start_probe
    vec = C.c_float*3
    probe.argtypes = [C.c_int, C.c_int, C.c_int, C.POINTER(vec), C.POINTER(C.c_int32)]
    probe.restype = C.c_int
    checks = 0
    for name, table in zip(CHARACTERS, tables):
        found = rows.findall(table)
        assert len(found) == table.count(b'{STAGE_') and found[-1][0] == b'STAGE_NONE'
        expected = {}
        for stage, act, xyz, yaw in found:
            coords = tuple(C.c_float(float(x.strip().rstrip(b'F'))).value for x in xyz.split(b','))
            expected.setdefault((stages[stage.decode()], int(act)), (coords, int(yaw, 0)))
        for stage in range(-1, stages['STAGE_COUNT']+2):
            for act in range(-1, stages['STAGE_ACTS']+1):
                xyz, yaw = vec(1, 2, 3), C.c_int32(-1)
                assert probe(characters['CHARACTER_'+name.upper()],stage,act,C.byref(xyz),C.byref(yaw)) == 0
                assert (tuple(xyz), yaw.value) == expected.get((stage,act), ((0,0,0),0)), (name,stage,act)
                checks += 1
    for character in (-1, characters['CHARACTER_COUNT']):
        xyz, yaw = vec(1, 2, 3), C.c_int32(-1)
        assert probe(character, 1, 0, C.byref(xyz), C.byref(yaw)) == 1
        assert tuple(xyz) == (1,2,3) and yaw.value == -1
        checks += 1
    report = dict(status='verified_table_driven_player_placement', checks=checks,
                  original_source_commit=PIN, original_source_sha256=hashlib.sha256(src).hexdigest(),
                  generated_source_sha256=digest(unit), private_DLL_sha256=digest(dll),
                  build_command=command, playable=False, overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(f'{checks} native stage start cases passed')


if __name__ == '__main__':
    main()
