"""Bind the actual QSound family, current regressions and preserved evidence."""
import argparse
import hashlib
import json
import shutil
import sys
from pathlib import Path


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def read(path):
    return json.loads(Path(path).read_bytes())


def main():
    ap = argparse.ArgumentParser()
    for name in ('workspace', 'build', 'check', 'clock', 'exit', 'owner', 'system', 'frame', 'startup', 'output'):
        ap.add_argument('--' + name, type=Path, required=True)
    a = ap.parse_args()
    root, build, output = a.workspace.resolve(), a.build.resolve(), a.output.resolve()
    public = root / 'outputs/sonic-adventure'
    assert not output.exists() and output.is_relative_to(root / 'work/sonic-adventure')
    report_files = dict(build=build / 'build_verification.json', verification=a.check / 'verification.json',
                        clock=a.clock / 'verification.json', exit=a.exit / 'verification.json',
                        owner=a.owner / 'verification.json', system=a.system / 'verification.json',
                        frame=a.frame / 'verification.json', saves=build / 'save-verification.json',
                        startup=a.startup / 'verification.json', audit_inputs=build / 'qsound-exit-regression-inputs.json')
    reports = {name: read(path) for name, path in report_files.items()}
    b, v, clock = reports['build'], reports['verification'], reports['clock']
    build_hash = sha(report_files['build'])
    for name in ('verification', 'clock', 'exit', 'owner', 'system', 'frame'):
        assert reports[name]['build_verification_sha256'] == build_hash, name
    for name in ('saves', 'startup'):
        assert reports[name]['build_report_sha256'] == build_hash, name
    assert v['runtime_checks'] == 18469 and v['fresh_process_cases'] == 8
    assert v['original_complete_instruction_calls'] == 3292
    assert not v['original_function_hooks_used'] and not v['synthetic_driver_ACKs']
    assert not v['full_game_request_queue_spatial_acoustic_or_streams_verified']
    assert clock['fresh_process_cases'] == 19 and clock['runtime_checks'] > 3000
    assert [reports[n]['runtime_checks'] for n in ('exit', 'owner', 'frame')] == [652, 1471, 98]
    assert reports['system']['checks'] == 152 and reports['saves']['checks'] == 591
    assert reports['saves']['original_calls'] == 29 and reports['startup']['checks'] == 7281
    assert reports['startup']['actual_GPU_images'] == reports['startup']['actual_Presents'] == 570
    assert reports['audit_inputs']['current_build_sha256'] == build_hash
    assert all(not row['exit_code'] and not row['diagnostic_bytes'] for row in b['commands'][:4])
    for name, expected in b['authored_source_sha256'].items():
        assert sha(root / name) == expected, name
    for name, key in (('external_game_sound.c', 'qsound_generated_source_sha256'),
                      ('sound_owner_qsound.c', 'qsound_owner_generated_sha256'),
                      ('qsound_layout32.c', 'qsound_guest_layout_source_sha256'),
                      ('qsound_layout32.obj', 'qsound_guest_layout_object_sha256')):
        assert sha(build / name) == b[key], name
    for row in reports['audit_inputs']['copies']:
        assert sha(row['source']) == sha(row['audit_copy']) == row['sha256']
    assert b['game_sound_generated_source_sha256'] == b['qsound_generated_source_sha256']
    assert b['qsound_SDK_function_body_unchanged'] and b['qsound_ID_storage_bits'] == 64
    assert b['qsound_wire_position_bytes'] == 7
    old_path = public / 'analysis/jp_native_sound_clock_package.json'
    old = read(old_path)
    for group in ('authored_source_sha256', 'proof_files_sha256'):
        for name, expected in old[group].items():
            assert sha(root / name) == expected, name
    base_file = next(root / name for name in old['proof_files_sha256']
                     if name.endswith('/build_verification.json') and sha(root / name) == b['qsound_baseline_sha256'])
    base = read(base_file)
    unchanged = {}
    for key, row in b['qsound_preserved_prior_DLLs'].items():
        assert sha(row['path']) == row['sha256'] and sha(b[key]) == b[key + '_sha256'], key
        if key not in ('game_sound_DLL', 'sound_SDK_DLL'):
            assert b[key + '_sha256'] == row['sha256']
            unchanged[key] = row
    for name in ('jp_native_g2_audio.dll', 'jp_native_aica.dll', 'jp_native_audio.dll'):
        assert sha(build / name) == sha(base_file.parent / name), name
        unchanged[name] = dict(path=str(base_file.parent / name), sha256=sha(build / name))
    assert b['present_DLL_sha256'] == base['present_DLL_sha256']
    assert old['original_chain_regression_checks'] == 79103 and old['actual_original_chain_entry_calls'] == 679
    ref = v['cases']['driver_reference']['observations']
    for name in ('driver_cpu', 'driver_dma'):
        obs = v['cases'][name]['observations']
        assert obs['final_RAM_sha256'] == ref['final_RAM_sha256'] and obs['PCM_sha256'] == ref['PCM_sha256']
    assert ref['final_RAM_sha256'] != v['cases']['driver_noop']['observations']['final_RAM_sha256']
    for name, script in (('verification', 'verify_native_qsound.py'), ('clock', 'verify_native_sound_clock.py'),
                         ('exit', 'verify_native_sound_exit.py'), ('owner', 'verify_native_game_sound.py'),
                         ('system', 'verify_native_system.py'), ('frame', 'verify_native_sound_dispatch.py'),
                         ('saves', 'verify_native_boot_saves.py'), ('startup', 'verify_native_system_startup.py')):
        assert reports[name]['verifier_sha256'] == sha(public / 'jp_port_tools' / script), name
    capture = a.startup / 'movie0/native-reset-boot-logo.png'
    assert sha(capture) == '9b28f7b43d8786cfee116fc34b39ed33ff44fb8ed93a0f62b21477b4902d2071'
    sys.path.insert(0, str(public))
    from publish_github_snapshot import qualified_native_builds
    selected = [dict(path=rel, bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
                for _, rel, raw in qualified_native_builds(root)]
    assert selected == old['public_build_selection'] and len(selected) == 10
    authored = ['native/jp_native_qsound.h', 'native/jp_native_qsound.inc',
                'jp_port_tools/qsound_observation.inc', 'jp_port_tools/build_native_qsound.py',
                'jp_port_tools/verify_native_qsound.py', 'jp_port_tools/prepare_native_qsound_exit_evidence.py',
                'jp_port_tools/package_native_qsound.py', 'JP_NATIVE_QSOUND.txt']
    previous_graph_checks = sum(reports[n]['runtime_checks'] for n in ('exit', 'owner', 'frame'))
    previous_graph_checks += sum(reports[n]['checks'] for n in ('system', 'saves', 'startup'))
    assert previous_graph_checks == 10245
    result = dict(status='original_QSound_command_copy_ownership_native64_and_real_firmware_path_qualified',
                  runtime_checks=v['runtime_checks'] + clock['runtime_checks'] + previous_graph_checks,
                  new_QSound_checks=v['runtime_checks'], fresh_QSound_process_cases=8,
                  current_timing_checks=clock['runtime_checks'], current_graph_regression_checks=previous_graph_checks,
                  complete_original_QSound_instruction_calls=v['original_complete_instruction_calls'],
                  original_unhooked_save_calls=29, actual_GPU_images=570, actual_Presents=570,
                  retained_prior_chain_checks_not_rerun=79103, retained_prior_chain_original_entry_calls=679,
                  authored_source_sha256={(public / name).relative_to(root).as_posix(): sha(public / name) for name in authored},
                  proof_files_sha256={path.resolve().relative_to(root).as_posix(): sha(path) for path in report_files.values()},
                  preserved_previous_qualification=dict(path=str(old_path), sha256=sha(old_path)),
                  all_prior_qualified_source_and_proofs_preserved=True, unchanged_prior_DLLs=unchanged,
                  public_build_selection=selected, native_ID_bits=64, wire_position_bytes=7,
                  original_ID_minus_one_zero_sentinel_retained=True,
                  complete_CPU_DMA_reference_RAM_and_PCM_match=True,
                  driver_ACK_synthesized=False, original_instruction_function_hooks=False,
                  acoustic_spatial_behavior_full_game_queue_streams_main_gameplay_verified=False,
                  combined_original_external_binaries_or_retail_media_published=False,
                  publication_pending_next_daily_review=True, playable=False, overall_completion_percent=10)
    output.mkdir()
    (output / 'qualification.json').write_text(json.dumps(result, indent=2) + '\n')
    report_files['package'] = output / 'qualification.json'
    for name, source in report_files.items():
        suffix = '' if name == 'verification' else '_' + name
        target = public / 'analysis' / ('jp_native_qsound' + suffix + '.json')
        assert not target.exists()
        shutil.copyfile(source, target)
        assert sha(source) == sha(target)
    print(result['status'], result['runtime_checks'], 'current checks;', v['runtime_checks'], 'new QSound checks')


if __name__ == '__main__':
    main()
