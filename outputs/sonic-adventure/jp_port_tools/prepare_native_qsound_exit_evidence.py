"""Stage unchanged historical source required by the frozen exit verifier.

These audit files are not compilation inputs for the current QSound DLL.
"""
import argparse
import hashlib
import json
import shutil
from pathlib import Path


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    for name in ('workspace', 'build', 'prior-exit'):
        ap.add_argument('--' + name, dest=name.replace('-', '_'), type=Path, required=True)
    a = ap.parse_args()
    root, build, prior = a.workspace.resolve(), a.build.resolve(), a.prior_exit.resolve()
    assert build.is_relative_to(root / 'work/sonic-adventure')
    b = json.loads((build / 'build_verification.json').read_bytes())
    old = json.loads((prior / 'build_verification.json').read_bytes())
    rows = []
    for name, expected in b['sound_exit_generated_sha256'].items():
        source, target = prior / name, build / name
        assert source.parent == prior and target.parent == build
        assert old['sound_exit_generated_sha256'][name] == expected and sha(source) == expected
        if target.exists():
            assert sha(target) == expected
        else:
            shutil.copyfile(source, target)
            assert sha(target) == expected
        rows.append(dict(source=str(source), audit_copy=str(target), sha256=expected))
    report = dict(status='historical_exit_verifier_inputs_preserved_not_compiled',
                  current_build_sha256=sha(build / 'build_verification.json'),
                  prior_exit_build_sha256=sha(prior / 'build_verification.json'),
                  current_compiled_owner_sha256=sha(build / 'sound_owner_qsound.c'), copies=rows)
    assert report['current_compiled_owner_sha256'] == b['qsound_owner_generated_sha256']
    output = build / 'qsound-exit-regression-inputs.json'
    if output.exists():
        assert json.loads(output.read_bytes()) == report
    else:
        output.write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], len(rows), 'byte-identical audit sources')


if __name__ == '__main__':
    main()
