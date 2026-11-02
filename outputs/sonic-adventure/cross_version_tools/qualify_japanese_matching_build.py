"""Independently compare every rebuilt image to the supplied Japanese disc.

Run inside the pinned Linux checkout after all, check and test succeed.
Only hashes and verification metadata leave the private retail build tree.
"""
import hashlib
import json
import re
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

S = Path('/root/sa-source-review')
W = Path('/mnt/c/Users/wigwo/Documents/Codex/2026-10-03/de/work/sonic-adventure')
assert Path.cwd() == S
sys.path.insert(0, str(S / 'tools'))
from images import binaries, files

def sha(path):
    result = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(4 * 1024 * 1024), b''):
            result.update(block)
    return result.hexdigest()

commit = subprocess.check_output(['git', 'rev-parse', 'HEAD'], text=True).strip()
assert commit == '25a3d3154dbe657325e97dbe8c5291086f6af5c0'
assert not subprocess.check_output(['git', 'status', '--porcelain'], text=True).strip()
expected = {}
for line in (S / 'SHA256SUMS').read_text().splitlines():
    if line.strip():
        digest, name = line.split(maxsplit=1)
        expected[name] = digest
image_names = binaries()
assert len(image_names) == 247
paths = files()
rows = []
for name in image_names:
    relative = paths[name]
    original = S / 'orig' / relative
    rebuilt = S / 'build' / relative
    original_hash, rebuilt_hash = sha(original), sha(rebuilt)
    assert original_hash == rebuilt_hash == expected['orig/' + relative], name
    assert original.stat().st_size == rebuilt.stat().st_size
    rows.append(dict(image=name, disc_path=relative,
                     bytes=rebuilt.stat().st_size, sha256=rebuilt_hash,
                     matches_original_and_pinned_checksum=True))

logs = {}
for name in ('japanese_v1007_full_matching_build.log',
             'japanese_v1007_source_check.log',
             'japanese_v1007_full_tests.log'):
    path = W / name
    logs[name] = dict(path=str(path), sha256=sha(path), bytes=path.stat().st_size)
test_text = (W / 'japanese_v1007_full_tests.log').read_text()
summary = re.search(r'Ran (\d+) tests in ([0-9.]+)s\s+OK(?: \(skipped=(\d+)\))?', test_text)
assert summary, 'Full upstream test run has no successful terminal summary'
readme = (S / 'README.md').read_text()
assert '**100% of the game\'s code builds from source, byte for byte.**' in readme
assert '| **All** | **247** | **5,820,458** | **97.7%** | **2.3%** |' in readme
result = dict(
    status='verified_all_247_Japanese_images_match_the_supplied_disc_byte_for_byte',
    checked_at_UTC=datetime.now(timezone.utc).isoformat(),
    upstream_repository='https://github.com/reshaped/sonic-adventure',
    upstream_commit=commit, source_checkout_clean=True,
    target='Japanese HDR-0001 version 1.007',
    matching_build_command='make -j4 all', matching_build_terminal_status=0,
    disc_free_source_check_command='make -j4 check', source_check_terminal_status=0,
    full_test_command='make test', full_test_terminal_status=0,
    full_tests_run=int(summary[1]), full_test_seconds=float(summary[2]),
    full_tests_skipped=int(summary[3] or 0),
    all_image_count=len(rows), all_image_bytes=sum(row['bytes'] for row in rows),
    images=rows, logs=logs,
    source_matching_code_bytes=5820458, source_C_code_percent=97.7,
    source_assembly_code_percent=2.3,
    source_accounting_verified_by_make_check=True,
    upstream_sum_table_sha256=sha(S / 'SHA256SUMS'),
    upstream_readme_sha256=sha(S / 'README.md'),
    private_retail_build_root=str(S / 'build'),
    public_game_code_imported=False, public_rebuilt_disc_binaries_uploaded=False,
    public_source_reuse_permissions_established=False,
    USA_Rev_A_matching_not_established=True,
    native_PC_port_or_requested_FPS_not_qualified=True,
    estimated_whole_project_completion_percent=10,
    measured_whole_project_coverage=False, goal_status='active',
    limitations='This verifies a complete matching Japanese Dreamcast rebuild, using original disc data for declared assets. It does not establish USA Rev A equivalence, runtime port correctness, game-source redistribution permission, or a native PC/Android/3DS port.',
)
target = W / 'japanese_v1007_full_matching_verification.json'
assert not target.exists()
target.write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps({key: value for key, value in result.items() if key not in ('images', 'logs')}, indent=2))
