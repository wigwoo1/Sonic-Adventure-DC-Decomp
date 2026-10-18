"""Verify exact cleanup targets and unchanged retained output files."""
import hashlib
import json
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'outputs/sonic-adventure'


def check(item):
    path = ROOT / item['path']
    if not path.is_file() or path.stat().st_size != item['bytes']:
        return item['path']
    with path.open('rb') as stream:
        actual = hashlib.file_digest(stream, 'sha256').hexdigest()
    return None if actual == item['sha256'] else item['path']


def main():
    plan = json.loads((OUT / 'analysis/workspace_cleanup_2026-10-15.json').read_text(encoding='utf-8-sig'))
    result_path = OUT / 'analysis/workspace_cleanup_result_2026-10-15.json'
    result = json.loads(result_path.read_text(encoding='utf-8-sig'))
    assert result['completed']
    assert result['deleted_file_count'] == plan['candidate_count']
    assert result['deleted_bytes'] == plan['candidate_bytes']
    assert {item['path'] for item in result['deleted_files']} == {item['path'] for item in plan['candidates']}
    assert all(not (ROOT / item['path']).exists() for item in plan['candidates'])
    assert len(plan['archive_checks']) == 3
    assert not any(item['errors'] for item in plan['archive_checks'])
    with ThreadPoolExecutor(max_workers=8) as pool:
        mismatches = [path for path in pool.map(check, plan['protected_output_files']) if path]
    assert not mismatches, mismatches
    result['verification'] = {
        'verified_utc': datetime.now(timezone.utc).isoformat(),
        'protected_output_files_unchanged': len(plan['protected_output_files']),
        'protected_file_mismatches': mismatches,
        'all_deleted_targets_absent': True,
        'installed_archive_members_checked_before_deletion': sum(item['installed_files_checked'] for item in plan['archive_checks']),
        'scope': 'Retained output files compared with pre-deletion SHA256 manifest before subsequent project audits refresh their own reports.'}
    result_path.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(result['deleted_file_count'], 'files removed;', result['deleted_bytes'], 'bytes reclaimed;',
          len(plan['protected_output_files']), 'retained output files unchanged.')


if __name__ == '__main__':
    main()
