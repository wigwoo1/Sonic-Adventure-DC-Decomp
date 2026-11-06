"""Link the pinned original stream joints to the existing real audio callback gate."""
import argparse, hashlib, json, re, shutil, subprocess
from pathlib import Path

PIN = '25a3d3154dbe657325e97dbe8c5291086f6af5c0'

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        ap.add_argument('--'+name, type=Path, required=True)
    a = ap.parse_args()
    r, u, base, d = (getattr(a, n).resolve() for n in ('workspace', 'upstream', 'baseline', 'output'))
    p = r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b = json.loads((base/'build_verification.json').read_bytes())
    for n, h in b['authored_source_sha256'].items():
        assert sha(r/n) == h, n
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c', '.h', '.obj', '.dll', '.lib'):
            shutil.copyfile(src, d/src.name)
    for n in ('include', 'inputs', 'sound_imports'):
        shutil.copytree(base/n, d/n)
    for n, h in b['generated_sources_sha256'].items():
        src, dst = base/n, d/n
        assert src.resolve().is_relative_to(base) and dst.resolve().is_relative_to(d) and sha(src) == h, n
        dst.parent.mkdir(parents=True, exist_ok=True)
        if dst.exists():
            assert sha(dst) == h, n
        else:
            shutil.copyfile(src, dst)
    names = ['sdk/sofdec/'+n+'.c' for n in ('sj_rbf', 'sj_mem', 'sj_utl', 'sj_patch', 'adx_crs')]
    originals = {n: sha(u/n) for n in names}
    for n, h in originals.items():
        raw = subprocess.check_output(['C:/Program Files/Git/cmd/git.exe', '-C', str(u), 'show', PIN+':'+n])
        assert hashlib.sha256(raw).hexdigest() == h, n
    prefix = b'''#include <stdint.h>
#include <machine.h>
#define JP_G2_API __declspec(dllimport)
#include "jp_native_g2_audio.h"
/* Same real creator-thread callback gate as the existing SDK sound owner.
 * These internal SDK interfaces require that owner to be open. */
#undef get_imask
#undef set_imask
#define get_imask() jp_g2_gate_get()
#define set_imask(level) ((void)jp_g2_gate_set(level))
'''
    suffix = b'''
_Static_assert(sizeof(SJCK)==16 && sizeof(SJ_IF)==96,"native chunk and vtable");
_Static_assert(sizeof(SJRBF_OBJ)==72 && sizeof(SJMEM_OBJ)==64,"native joint records");
_Static_assert(sizeof(Sint32)==4 && sizeof(UUID)==16,"SDK scalar and UUID widths");
'''
    generated = d/'external_adx_joints.c'
    generated.write_bytes(prefix+b'\n'.join((u/n).read_bytes() for n in names)+suffix)
    def row(path):
        return list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and
                         Path(x['command'][x['command'].index('-c')+1]) == path))
    template = row(base/'external_game_sound.c')
    common = [x.replace(str(base), str(d)) for x in template[:template.index('-c')]]
    compile_ = common+['-I'+str(u/'sdk'), '-fwrapv', '-c', str(generated), '-o', str(d/'adx_joints.obj')]
    link = list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and
                     Path(x['command'][-1]) == Path(b['game_sound_DLL'])))
    link = [x.replace(str(base), str(d)) for x in link]
    exports = sorted(set(n.decode() for n in re.findall(
        rb'^(?:void|SJ|UUID \*|Sint32|char \*)\s*((?:SJRBF_|SJMEM_|SJ_SplitChunk|mwSjSplitChunk|ADXCRS_)[A-Za-z0-9_]*)\(',
        generated.read_bytes(), re.M)))
    link[4:4] = [str(d/'adx_joints.obj')]+['-Wl,/export:'+n for n in exports]
    commands = []
    for i, cmd in enumerate((compile_, link)):
        proc = subprocess.run(cmd, capture_output=True)
        log = d/f'joints_build_{i}.txt'
        log.write_bytes(proc.stdout+proc.stderr)
        commands.append(dict(command=cmd, exit_code=proc.returncode, diagnostic_bytes=log.stat().st_size))
        (d/'joints_compile_attempt.json').write_text(json.dumps(commands, indent=2)+'\n')
        assert proc.returncode == 0 and log.stat().st_size == 0, (str(log), log.read_text())
    updates, preserved = {}, {}
    for key, value in b.items():
        if (key == 'DLL' or key.endswith('_DLL')) and isinstance(value, str) and Path(value).suffix == '.dll':
            assert sha(value) == b[key+'_sha256'], key
            preserved[key] = dict(path=value, sha256=sha(value))
            updates[key] = str(d/Path(value).name)
            updates[key+'_sha256'] = sha(updates[key])
    updates.update(status='original_stream_ring_memory_chunk_and_critical_section_modules_linked_unverified',
        commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'], **{Path(__file__).resolve().relative_to(r).as_posix(): sha(__file__)}),
        joints_baseline_sha256=sha(base/'build_verification.json'), joints_original_source_sha256=originals,
        joints_generated_source_sha256=sha(generated), joints_exports=exports, joints_preserved_prior_DLLs=preserved,
        raw_SDK_interfaces_require_live_creator_thread_audio_owner=True,
        original_source_bodies_changed=False, stream_frontend_or_full_gameplay_verified=False,
        public_artifact_qualified=False, playable=False, overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b, **updates), indent=2)+'\n')
    print(updates['status'], len(exports), 'SDK exports', flush=True)

if __name__ == '__main__':
    main()
