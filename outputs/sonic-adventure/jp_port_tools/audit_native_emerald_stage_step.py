"""Compile the original stage step and audit the current Emerald Coast providers."""
import argparse
import json
import shlex
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function, sha
from build_native_renderer_object_v3 import coff_symbols


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (
        path.resolve() for path in (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    path = 'src/stage/stage.c'
    raw = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{path}'])
    assert raw == (upstream / path).read_bytes()
    body = owner_function(raw, 'stage_run_step')
    assert body.startswith(b'static int ')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    recipe = manifest['emerald_stage_tick']['commands'][0]
    out.mkdir()
    source, obj = out / 'stage_run_step.c', out / 'stage_run_step.obj'
    headers = ('ninja.h', 'collision/collision.h', 'collision/polygon.h', 'debug/debug.h',
               'draw/model.h', 'effect/particle.h', 'event/play.h', 'hud/hud.h', 'main.h', 'stage/stage.h',
               'system/frame.h', 'system/pad.h', 'system/task.h', 'system/thread.h',
               'system/timer.h')
    source.write_bytes(''.join(f'#include "{name}"\n' for name in headers).encode() + body[7:])
    command = recipe[:recipe.index('-c')] + ['-ffreestanding', '-c', str(source), '-o', str(obj)]
    result = subprocess.run(command, capture_output=True)
    (out / 'compile.stderr').write_bytes(result.stderr)
    assert result.returncode == 0 and not result.stderr, result.stderr.decode(errors='replace')
    imports = sorted(s['name'] for s in coff_symbols(obj) if s['section'] == 0)
    providers = {name: [] for name in imports}
    for arg in shlex.split((base / 'link.rsp').read_text()):
        candidate = Path(arg)
        if candidate.suffix != '.obj':
            continue
        for symbol in coff_symbols(candidate):
            name = symbol['name']
            if symbol['section'] > 0 and name in providers:
                providers[name].append(candidate.relative_to(root).as_posix())
    report = dict(upstream_commit=PIN, pinned_stage_source_sha256=sha(upstream / path),
                  baseline_manifest_sha256=sha(parent), compiled_object_sha256=sha(obj),
                  object_providers=providers,
                  imports_without_object_provider=[name for name in imports if not providers[name]],
                  stage_step_linked=False, playable=False, overall_completion_percent=10)
    (out / 'verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
