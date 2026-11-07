"""Link the original player history ring and seed it from stage placement."""
import argparse
import ctypes
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'baseline', 'stage-start', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    base, stage, out = (p.resolve() for p in (args.baseline, args.stage_start, args.output))
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    manifest_path = base/'build_verification.json'
    manifest = json.loads(manifest_path.read_bytes())
    assert digest(manifest['frames_DLL']) == manifest['frames_DLL_sha256']
    player = pinned(args.upstream, 'src/player/player.c')
    stage_report = json.loads((stage/'verification.json').read_bytes())
    stage_source = stage/'stage_start.c'
    assert digest(stage_source) == stage_report['generated_source_sha256']
    tables = stage_source.read_bytes().split(b'__declspec(dllexport) int jp_stage_start_probe(', 1)[0]
    data = re.findall(rb'^\w+ player_history_(?:index|pos|ang)\[[^;]+;', player, re.M)
    assert len(data) == 3
    functions = []
    for name in ('player_get_history', 'player_set_history', 'player_note_history'):
        body = owner_function(player, name).removeprefix(b'static ')
        functions.append(body)
    out.mkdir()
    source = out/'player_history.c'
    source.write_bytes(tables+b'\n'+b'\n'.join(data+functions)+b'''
__declspec(dllexport) int jp_stage_place_table_and_seed_history(
    struct task_work *work, int stage, int act)
{
    if (!work) return 1;
    Uint8 player = PLAYER_VARS(work)->player;
    if (player >= PLAYER_COUNT || player_work[player] != work) return 1;
    if (jp_stage_place_from_start_table(work, stage, act)) return 1;
    return player_set_history(player, 0, &work->pos, NULL) ? 0 : 1;
}

__declspec(dllexport) int jp_player_history_probe(void)
{
    struct task *first = task_create(TASK_WORK, 1, NULL);
    struct task *second = task_create(TASK_WORK, 1, NULL);
    int result = 0;
    NJS_POINT3 position;
    struct angles angle;
    if (!first || !second || player_work[0] || player_work[1]) {
        result = 1;
        goto finish;
    }
    PLAYER_VARS(first->work)->character = CHARACTER_SONIC;
    PLAYER_VARS(second->work)->character = CHARACTER_TAILS;
    PLAYER_VARS(second->work)->player = 1;
    if (!jp_stage_place_table_and_seed_history(first->work, STAGE_BEACH, 0)) {
        result = 2;
        goto finish;
    }
    player_work[0] = first->work;
    player_work[1] = second->work;
    if (jp_stage_place_table_and_seed_history(first->work, STAGE_BEACH, 0) ||
        jp_stage_place_table_and_seed_history(second->work, STAGE_WINDY, 2) ||
        !player_get_history(0, 0, &position, NULL) ||
        position.x != -9.0F || position.y != 16.0F || position.z != 4.0F ||
        first->work->ang.y != 0x6000 ||
        !player_get_history(1, 0, &position, NULL) ||
        position.x != 1093.0F || position.y != -158.0F || position.z != -1254.0F) {
        result = 3;
        goto unbind;
    }
    for (int i = 1; i <= 257; i++) {
        first->work->pos.x = (Float)i;
        first->work->ang.y = i;
        player_note_history(0);
    }
    for (int back = 0; back < PLAYER_HISTORY_STEPS; back++) {
        if (!player_get_history(0, back, &position, &angle) ||
            position.x != (Float)(257-back) || angle.y != 257-back) {
            result = 4;
            goto unbind;
        }
    }
    if (!player_get_history(1, 0, &position, NULL) || position.x != 1093.0F ||
        player_get_history(2, 0, NULL, NULL) || player_set_history(2, 0, NULL, NULL))
        result = 5;
unbind:
    player_work[0] = NULL;
    player_work[1] = NULL;
finish:
    if (second) task_destroy(second);
    if (first) task_destroy(first);
    return result;
}
''')
    obj = out/'player_history.obj'
    dll = out/'jp_native_frames.dll'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out/dep.name)
    compile_args = manifest['sonic_draw']['commands'][0]
    compile_args = compile_args[:compile_args.index('-c')]
    link_driver = manifest['sonic_scheduled_draw']['commands'][-1][0]
    link = shlex.split((base/'link.rsp').read_text())
    prior = {path: digest(path) for path in link if path.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [
        str(obj), '-Wl,/export:player_get_history', '-Wl,/export:player_set_history',
        '-Wl,/export:player_note_history']
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link), encoding='utf-8')
    commands = [compile_args+['-c', str(source), '-o', str(obj)], [link_driver, '@'+str(rsp)]]
    for tag, command in zip(('compile', 'link'), commands):
        process = subprocess.run(command, capture_output=True)
        (out/(tag+'.stderr')).write_bytes(process.stderr)
        assert process.returncode == 0 and not process.stderr, process.stderr.decode(errors='replace')
    assert all(digest(path) == sha for path, sha in prior.items())
    owner = ctypes.CDLL(str(dll))
    owner.heap_init.restype = owner.task_init.restype = None
    owner.jp_player_history_probe.restype = ctypes.c_int
    owner.heap_init()
    owner.task_init()
    status = owner.jp_player_history_probe()
    assert status == 0, status
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), player_history=dict(
        source_commit=PIN, parent_manifest_sha256=digest(manifest_path),
        stage_report_sha256=digest(stage/'verification.json'), generated_source_sha256=digest(source),
        object_sha256=digest(obj), unchanged_objects_sha256=prior, commands=commands,
        task_placement_cases=2, history_steps_recorded=257, history_slots_verified=256,
        missing_owner_and_player_isolation_verified=True))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    print('Original history verified across 256 ring slots and two native player tasks')


if __name__ == '__main__':
    main()
