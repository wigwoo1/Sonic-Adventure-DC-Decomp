"""Prepare a conservative, read-only cleanup manifest; never deletes files."""
import hashlib
import json
import os
import zipfile
import zlib
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'outputs/sonic-adventure'
REPORT = OUT / 'analysis/workspace_cleanup_2026-10-15.json'
TOOLCHAIN = ROOT / 'work/sonic-adventure/toolchain'


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def relative(path):
    return path.relative_to(ROOT).as_posix()


def main():
    assert not REPORT.exists(), 'Keep the previous cleanup record; do not overwrite it.'
    candidates, archives = [], []

    def candidate(path, reason):
        resolved = path.resolve(strict=True)
        assert resolved.is_relative_to(ROOT) and not path.is_symlink()
        candidates.append({'path': relative(path), 'bytes': path.stat().st_size,
                           'sha256': digest(path), 'reason': reason})

    for archive in sorted(TOOLCHAIN.glob('*.zip')):
        checked, differences, errors = [], [], []
        with zipfile.ZipFile(archive) as packed:
            for member in packed.infolist():
                if member.is_dir():
                    continue
                extracted = (TOOLCHAIN / member.filename).resolve()
                assert extracted.is_relative_to(TOOLCHAIN.resolve())
                if not extracted.is_file():
                    errors.append({'member': member.filename, 'reason': 'missing installed file'})
                    continue
                crc = 0
                with extracted.open('rb') as stream:
                    for chunk in iter(lambda: stream.read(1024 * 1024), b''):
                        crc = zlib.crc32(chunk, crc)
                if extracted.stat().st_size != member.file_size or crc != member.CRC:
                    # The only accepted local configuration change is explicitly recorded.
                    if member.filename == 'ghidra_12.1.4_PUBLIC/support/launch.properties':
                        original = packed.read(member).decode('utf-8')
                        installed = extracted.read_text(encoding='utf-8')
                        normalized = installed.replace(
                            'VMARGS=-Dapplication.settingsdir=' + str(TOOLCHAIN / 'user-home/settings').replace('\\', '/'),
                            '#VMARGS=-Dapplication.settingsdir=').replace(
                            'VMARGS=-Dapplication.cachedir=' + str(TOOLCHAIN / 'user-home/cache').replace('\\', '/'),
                            '#VMARGS=-Dapplication.cachedir=')
                        if normalized.rstrip() == original.rstrip():
                            differences.append({'member': member.filename,
                                'reason': 'Installed configuration uses project-local settings and cache directories.',
                                'archive_original_text': original, 'installed_sha256': digest(extracted)})
                        else:
                            errors.append({'member': member.filename, 'reason': 'unexpected configuration change'})
                    else:
                        errors.append({'member': member.filename, 'reason': 'installed content differs'})
                checked.append([member.filename, member.file_size, member.CRC])
        info = {'archive': relative(archive), 'installed_files_checked': len(checked),
                'checked_member_index_sha256': hashlib.sha256(json.dumps(checked).encode()).hexdigest(),
                'method': 'Every installed member checked by length and CRC32 against ZIP directory.',
                'retained_configuration_differences': differences, 'errors': errors}
        archives.append(info)
        if not errors:
            candidate(archive, 'Redundant download archive; installed contents verified and retained.')
        print(archive.name, 'installed files checked:', len(checked), 'errors:', len(errors), flush=True)

    # Only project-owned caches, never third-party runtime packages or source repositories.
    for directory, subdirs, filenames in os.walk(OUT):
        subdirs[:] = [name for name in subdirs if name != '.git' and not (Path(directory) / name).is_symlink()]
        if Path(directory).name != '__pycache__':
            continue
        for name in filenames:
            path = Path(directory) / name
            if path.suffix == '.pyc' and (path.parent.parent / (name.split('.cpython-')[0] + '.py')).is_file():
                candidate(path, 'Regenerable Python cache; corresponding Python source retained.')

    scratch = OUT / 'build_and_verify_tail.temp.ps1'
    maintained = OUT / 'build_and_verify.ps1'
    scratch_lines = scratch.read_text(encoding='utf-8-sig').splitlines()
    maintained_lines = iter(maintained.read_text(encoding='utf-8-sig').splitlines())
    assert all(any(line == other for other in maintained_lines) for line in scratch_lines)
    candidate(scratch, 'Obsolete temporary build-script copy; every line retained in order in build_and_verify.ps1.')
    duplicate = ROOT / 'register_bitstream_pad.py'
    assert duplicate.read_bytes() == (OUT / duplicate.name).read_bytes()
    candidate(duplicate, 'Misplaced duplicate; identical maintained script retained under outputs/sonic-adventure.')

    # Refuse removal if source, documentation, or evidence explicitly names a disposable path.
    excluded = {item['path'] for item in candidates} | {relative(Path(__file__))}
    references = []
    needles = ['build_and_verify_tail.temp.ps1', 'ghidra.zip', 'jdk.zip',
               'ghidra_12.0.4_PUBLIC_20260303.zip']
    for path in OUT.rglob('*'):
        if path.is_file() and relative(path) not in excluded and path.suffix.lower() in (
                '.py', '.ps1', '.json', '.txt', '.md', '.c', '.h', '.cmake'):
            content = path.read_text(encoding='utf-8-sig', errors='replace')
            for needle in needles:
                if needle in content:
                    references.append({'file': relative(path), 'reference': needle})
    assert not references, references

    protected = []
    for path in sorted(OUT.rglob('*')):
        if path.is_file() and relative(path) not in excluded and '__pycache__' not in path.parts:
            protected.append({'path': relative(path), 'bytes': path.stat().st_size, 'sha256': digest(path)})
    report = {'created_utc': datetime.now(timezone.utc).isoformat(), 'status': 'prepared',
              'scope': 'Project-owned Python caches, verified extracted tool downloads, and confirmed duplicate scratch files only.',
              'candidates': candidates, 'candidate_count': len(candidates),
              'candidate_bytes': sum(item['bytes'] for item in candidates),
              'archive_checks': archives, 'blocking_references': references,
              'protected_output_files': protected,
              'retained': ['All game inputs and assets', 'All installed toolchains and dependency packages',
                           'All recovered and in-progress C source', 'All verification reports and Ghidra exports',
                           'All historical sources used as comparison baselines',
                           'All images and third-party source repositories',
                           'Ambiguous older scripts, logs, backups, and build products']}
    REPORT.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('Prepared', len(candidates), 'files;', report['candidate_bytes'], 'bytes; protected', len(protected), 'files.', flush=True)


if __name__ == '__main__':
    main()
