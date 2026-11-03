"""Extend an existing real production link audit with verified component objects.

No component test DLL, test probe, extra state owner or placeholder is linked.
The remaining unresolved set must equal the prior set minus declared services.
"""
import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    for n in ['workspace', 'baseline-audit', 'component', 'compiler', 'output']:
        ap.add_argument('--' + n, dest=n.replace('-', '_'), type=Path, required=True)
    a = ap.parse_args()
    r, d, component = a.workspace.resolve(), a.output.resolve(), a.component.resolve()
    assert d.is_relative_to(r / 'work/sonic-adventure') and not d.exists()
    assert not d.is_relative_to(r / 'work/sonic-adventure/build')
    prior = json.loads(a.baseline_audit.read_text())
    build_path = component / 'build_verification.json'
    b = json.loads(build_path.read_text())
    assert b['exit_code'] == b['diagnostic_bytes'] == 0
    assert sha(a.compiler) == b['compiler_sha256']
    for name, h in b['authored_source_sha256'].items():
        assert sha(r / name) == h
    objects = {Path(n).resolve(): h for n, h in prior['linked_objects_sha256'].items()}
    for obj, h in objects.items():
        assert sha(obj) == h
    additions = {}
    for n, h in b['production_objects_sha256'].items():
        obj = (component / n).resolve()
        assert obj.is_relative_to(component) and obj.suffix == '.obj'
        assert sha(obj) == h and obj not in objects and 'probe' not in obj.name
        additions[obj] = h
    resolved = set(b['expected_resolved_services'])
    assert additions and resolved and resolved <= set(prior['combined_remaining_imports'])
    cmd = list(prior['command'])
    assert Path(cmd[0]).resolve() == a.compiler.resolve()
    assert str(cmd[-1]).endswith('.dll')
    cmd[-1] = str(d / 'production_unfinished.dll')
    cmd[cmd.index('-Wl,kernel32.lib'):cmd.index('-Wl,kernel32.lib')] = list(map(str, additions))
    d.mkdir(parents=True)
    run = subprocess.run(cmd, capture_output=True)
    log = d / 'link.txt'
    log.write_bytes(run.stdout + run.stderr)
    text = log.read_text()
    remaining = sorted(set(re.findall(r'undefined symbol: (\S+)', text)))
    assert run.returncode != 0 and not (d / 'production_unfinished.dll').exists()
    assert set(remaining) == set(prior['combined_remaining_imports']) - resolved
    errors = [line for line in text.splitlines() if 'error:' in line]
    assert len(errors) == len(remaining) + 1 and errors[-1].startswith('clang: error: linker command failed')
    presentation = [o for o, h in objects.items() if h == prior['production_presentation_object_sha256']]
    assert len(presentation) == 1
    original_imports = {s['name'] for s in coff_symbols(presentation[0]) if s['section'] == 0 and s['value'] == 0}
    assert len(original_imports) == prior['production_presentation_original_imports']
    satisfied = sorted(original_imports - set(remaining))
    report = dict(prior)
    report.update(status='real_native_component_dependency_extension_verified', command=cmd,
                  link_exit_code=run.returncode, log_sha256=sha(log),
                  combined_remaining_imports=remaining, combined_remaining_import_count=len(remaining),
                  satisfied_presentation_imports=satisfied, satisfied_presentation_import_count=len(satisfied),
                  newly_resolved_services=sorted(resolved),
                  linked_objects_sha256={str(o): h for o, h in objects.items()},
                  baseline_audit_sha256=sha(a.baseline_audit), component_build_report_sha256=sha(build_path),
                  component_objects_sha256={str(o): h for o, h in additions.items()},
                  verifier_sha256=sha(Path(__file__)))
    report['linked_objects_sha256'].update({str(o): h for o, h in additions.items()})
    (d / 'verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print('Resolved:', ', '.join(sorted(resolved)), '; remaining production imports:', len(remaining))


if __name__ == '__main__':
    main()
