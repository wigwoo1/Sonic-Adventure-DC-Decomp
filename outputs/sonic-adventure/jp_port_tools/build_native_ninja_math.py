"""Build authored native Ninja math; no game source or assets are linked."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--workspace', required=True, type=Path)
    ap.add_argument('--compiler', required=True, type=Path)
    a = ap.parse_args()
    r = a.workspace.resolve()
    p = r / 'outputs/sonic-adventure'
    w = r / 'work/sonic-adventure'
    out = w / 'build/jp_ninja_math.dll'
    assert not out.exists(), 'Preserve an existing DLL; use a new output version.'
    sources = [p / 'native/jp_ninja_math.c', p / 'native/jp_math_freestanding_runtime.c']
    command = [str(a.compiler.resolve()), '--target=x86_64-pc-windows-msvc',
               '-std=c17', '-shared', '-O2', '-ffp-contract=off', '-fno-builtin',
               '-Wall', '-Wextra', '-Werror', '-fuse-ld=lld', '-nostdlib',
               '-Wl,/noentry', *map(str, sources), '-o', str(out)]
    result = subprocess.run(command, capture_output=True)
    log = w / 'jp_ninja_math_build.txt'
    assert not log.exists()
    log.write_bytes(result.stdout + result.stderr)
    inputs = sources + [p / 'native/jp_ninja_math.h',
                       p / 'third_party/skmp-sh4-fpu/sh4_fpu_approx.h', Path(__file__)]
    report = dict(status='native_math_build_passed_behavior_not_yet_qualified' if result.returncode == 0 else 'native_math_build_failed',
                  command=command, exit_code=result.returncode, diagnostic_bytes=log.stat().st_size,
                  log_sha256=sha(log), compiler_sha256=sha(a.compiler),
                  source_sha256={str(s.relative_to(r)): sha(s) for s in inputs},
                  DLL=str(out), DLL_sha256=sha(out) if result.returncode == 0 else None,
                  original_game_source_linked=False, retail_assets_embedded=False,
                  upstream_game_source_modified=False, full_game=False,
                  overall_completion_percent=10, goal_paused=False)
    target = p / 'analysis/jp_ninja_math_build_verification.json'
    assert not target.exists()
    target.write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], 'diagnostic bytes:', report['diagnostic_bytes'])
    raise SystemExit(result.returncode)

if __name__ == '__main__':
    main()
