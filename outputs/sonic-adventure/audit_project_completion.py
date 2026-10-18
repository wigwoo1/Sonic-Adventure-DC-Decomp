"""Conservative weighted acceptance milestone score; never code/effort coverage."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def read(name):
    return json.loads((ROOT / name).read_text())


def main():
    status = read('project_status.json')
    extraction = read('analysis/extraction_verification.json')
    prs = read('analysis/prs_native_verification.json')
    coverage = read('analysis/recovery_coverage_audit.json')
    inventory = read('disc_inventory.json')
    ghidra_export = read('analysis/ghidra-all/export_summary.json')
    ghidra_index = ROOT / 'analysis/ghidra-all/function_index.tsv'

    extracted = (
        extraction.get('files_checked') == 2832
        and extraction.get('bytes_checked') == 1020124956
        and extraction.get('errors') == []
        and bool(inventory)
    )
    decoded = (
        prs.get('files_verified') == 849
        and prs.get('decoded_bytes_verified') == 276341445
        and len(prs.get('files', [])) == 849
        and all(item.get('content_hash_matches') and item.get('guards_preserved') for item in prs['files'])
    )
    milestones = [
        ('Disc inventory and verified extraction', 5, extracted, 'analysis/extraction_verification.json'),
        ('PRS decoding verified against extracted resources', 5, decoded, 'analysis/prs_native_verification.json'),
        ('Complete executable module inventory and code/data boundaries', 10, False, 'analysis/recovery_coverage_audit.json'),
        ('Full game-code decompilation and original behavior verification', 24, False, 'analysis/recovery_coverage_audit.json'),
        ('Playable native PC game with renderer/input/audio/assets and optional SUPIR HD texture pack', 18, False, 'project_status.json:milestones; HD_TEXTURE_AND_DISPLAY_PLAN.txt'),
        ('Verified 60/120/240 FPS, resolution support through 8K and texture/render/aspect settings', 9, False, 'project_status.json:milestones; HD_TEXTURE_AND_DISPLAY_PLAN.txt'),
        ('Playable Android port', 7, False, 'project_status.json:milestones'),
        ('Playable Nintendo 3DS port with stereoscopic 3D', 7, False, 'project_status.json:milestones'),
        ('Nintendo 64 demake with platform-adapted models and assets', 5, False, 'project_status.json:requirements.additional_versions'),
        ('PlayStation demake with platform-adapted models and assets', 5, False, 'project_status.json:requirements.additional_versions'),
        ('Complete source/documentation and reproducible final handoff', 5, False, 'SOURCE_HANDOFF.txt'),
    ]
    assert sum(weight for _, weight, _, _ in milestones) == 100
    score = sum(weight for _, weight, done, _ in milestones if done)
    report = {
        'overall_completion_percent': score,
        'method': 'Weighted acceptance milestone score v2; completed milestones only. Weights are declared planning choices, not measured effort or code coverage. The native-PC gate includes the optional SUPIR HD texture pack; the FPS/resolution gate includes texture/render/aspect settings. The scope includes the N64 and PlayStation demakes and their model/asset adaptation. Existing weights are preserved. No fractional credit for unfinished work.',
        'uncertainty': 'Not an estimate of remaining time, difficulty or percentage of code recovered. Broad unfinished gates intentionally stay uncredited.',
        'evidence_basis': 'Existing verification artifacts inspected; extraction/decoding not rerun during this audit. Full goal remains active.',
        'main_binary_draft_inventory': {
            'summary': 'analysis/ghidra-all/export_summary.json',
            'function_index': 'analysis/ghidra-all/function_index.tsv',
            'indexed_functions': ghidra_export['indexed_functions'],
            'draft_pseudocode_functions': ghidra_export['export_statuses']['draft_pseudocode'],
            'decompilation_failures': ghidra_export['export_statuses']['failed'],
            'warnings_in_draft': ghidra_export['warnings_in_draft'],
            'function_index_bytes': ghidra_index.stat().st_size,
            'whole_game_coverage_verified': ghidra_export['whole_game_coverage_verified'],
            'limitation': 'Ghidra pseudocode is preliminary and is not buildable or behavior-verified source; its function index is not a complete executable code/data boundary audit.'
        },
        'requirements': status['requirements'],
        'milestones': [
            {'name': name, 'weight_percent': weight, 'complete': done, 'evidence': evidence}
            for name, weight, done, evidence in milestones
        ],
        'registry_warning': {
            'registered_routines': coverage['registered_routines'],
            'indexed_candidates': coverage['indexed_candidates_in_export_snapshots'],
            'unindexed_non_texture_binaries': coverage['non_texture_binaries_without_function_index'],
            'source_c_files': len(list((ROOT / 'source').glob('*.c'))),
            'note': 'Registry is incomplete and candidate indexes are not an exhaustive code inventory; these counts are not coverage.',
        },
    }
    (ROOT / 'analysis/project_completion_audit.json').write_text(json.dumps(report, indent=2) + '\n')
    status.setdefault('verified_progress', {}).update({
        'main_binary_draft_pseudocode_functions': ghidra_export['export_statuses']['draft_pseudocode'],
        'main_binary_draft_decompilation_failures': ghidra_export['export_statuses']['failed'],
        'main_binary_draft_warning_count': ghidra_export['warnings_in_draft'],
        'main_binary_draft_inventory_report': 'analysis/ghidra-all/export_summary.json',
    })
    (ROOT / 'project_status.json').write_text(json.dumps(status, indent=2) + '\n')
    print(f'{score}% weighted whole-project milestone completion; full scope unfinished')


if __name__ == '__main__':
    main()
