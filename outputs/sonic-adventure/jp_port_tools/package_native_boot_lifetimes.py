"""Freeze verified lifecycle evidence without publishing combined game DLLs."""
import argparse, ast, hashlib, json, shutil
from pathlib import Path
from package_native_sound_placement import prior


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def read(path):
    return json.loads(Path(path).read_bytes())


def main():
    ap = argparse.ArgumentParser()
    names = ('workspace', 'build', 'check', 'parent', 'ownership', 'sound',
             'clock', 'placement', 'audit', 'output')
    for name in names:
        ap.add_argument('--' + name, type=Path, required=True)
    a = ap.parse_args(); r = a.workspace.resolve(); p = r / 'outputs/sonic-adventure'
    d = a.output.resolve(); assert not d.exists() and d.is_relative_to(r / 'work/sonic-adventure')
    b = read(a.build / 'build_verification.json'); bh = sha(a.build / 'build_verification.json')
    specs = (('check', 'verify_native_boot_exit.py'),
        ('parent', 'verify_native_main_boot_lifetimes.py'),
        ('ownership', 'verify_native_main_boot_ownership.py'),
        ('sound', 'verify_native_sound_exit.py'), ('clock', 'verify_native_sound_clock.py'),
        ('placement', 'verify_native_sound_placement.py'), ('audit', 'audit_native_boot_lifetimes.py'))
    reports = {}; evidence = {'build': a.build / 'build_verification.json'}
    for name, tool in specs:
        source = getattr(a, name) / 'verification.json'; v = read(source)
        assert v['build_verification_sha256'] == bh, name
        assert v['auditor_sha256' if name == 'audit' else 'verifier_sha256'] == sha(p / 'jp_port_tools' / tool)
        reports[name] = v; evidence[name] = source
    assert reports['check']['checks'] == 737 and len(reports['check']['cases']) == 15
    assert reports['check']['cases']['process_exit']['actual_exit_code'] == 0
    assert reports['check']['cases']['process_failure']['actual_exit_code'] == 70
    assert reports['check']['cases']['denied_write']['observations']['initial_denied_write_exit_result'] == 6
    assert reports['parent']['checks'] == 458 and reports['parent']['cases']['fatal_driver']['actual_exit_code'] == 72
    assert reports['ownership']['checks'] == 293 and reports['sound']['runtime_checks'] == 652
    assert reports['clock']['runtime_checks'] >= 3000 and reports['clock']['fresh_process_cases'] == 19
    assert reports['placement']['checks'] == 35
    assert len(reports['audit']['reused_Frame_objects']) == 144
    assert len(reports['audit']['unchanged_prior_artifacts']) == 55
    assert reports['audit']['prior_qualification_packages_checked'] == 17
    for name, digest in b['authored_source_sha256'].items():
        assert sha(r / name) == digest, name
    old = p / 'analysis/jp_native_main_boot_package.json'; seen = set(); prior(r, old, seen)
    authored = [p / 'native' / name for name in ('jp_native_boot_lifetimes.h',
        'jp_native_boot_lifetimes.c', 'jp_native_boot_exit.h',
        'jp_native_system_boot_exit.inc', 'jp_native_exit_boot.inc')]
    authored += [p / 'jp_port_tools' / name for name in ('build_native_boot_lifetimes.py',
        'build_native_boot_exit.py', 'verify_native_boot_exit.py',
        'verify_native_main_boot_lifetimes.py', 'audit_native_boot_lifetimes.py',
        'package_native_boot_lifetimes.py')]
    authored += [p / 'JP_NATIVE_BOOT_LIFETIMES.txt']
    for path in authored:
        if path.suffix == '.py':
            ast.parse(path.read_text(encoding='utf-8-sig'))
    targets = {name: p / 'analysis' / ('jp_native_boot_lifetimes_' + name + '.json')
               for name in evidence}
    package = p / 'analysis/jp_native_boot_lifetimes_package.json'
    assert not package.exists() and all(not path.exists() for path in targets.values())
    d.mkdir(); proofs = {}
    for name, target in targets.items():
        shutil.copyfile(evidence[name], target); proofs[target.relative_to(r).as_posix()] = sha(target)
    regressions = sum(reports[n][key] for n, key in (('parent', 'checks'),
        ('ownership', 'checks'), ('sound', 'runtime_checks'),
        ('clock', 'runtime_checks'), ('placement', 'checks')))
    report = dict(status='real_native_boot_card_font_System_early_final_exit_scoped_qualification',
        new_checks=737, selected_regression_checks=regressions,
        combined_qualified_scoped_checks=940335 + 737,
        actual_original_card_save_fibers_and_font_providers=True,
        same_existing_System_registration_and_global_exit=True,
        terminal_global_card_font_exit_handoff_verified=True,
        real_denied_write_preserves_image_and_retains_authority_on_timeout=True,
        callback_reentry_precedes_terminal_mutation=True,
        VMU_store_and_LCD_remain_borrowed=True,
        early_capture_blocks_device_until_caller_releases=True,
        original_source_and_previous_proofs_preserved=True,
        build_verification_sha256=bh,
        private_DLL_sha256={key: b[key + '_sha256'] for key in
            ('frames_DLL', 'System_DLL', 'authored_exit_DLL', 'main_boot_DLL')},
        prior_qualifications_preserved={'main_boot': dict(path=old.relative_to(r).as_posix(), sha256=sha(old))},
        authored_source_sha256={path.relative_to(r).as_posix(): sha(path) for path in authored},
        proof_files_sha256=proofs, combined_game_SDK_binaries_publishable=False,
        public_research_build_payloads_changed=False, retail_assets_or_captures_packaged=False,
        full_game_init_or_stage_gameplay_verified=False, playable=False,
        overall_completion_percent=10, goal_active=True)
    result = d / 'qualification.json'; result.write_text(json.dumps(report, indent=2) + '\n')
    shutil.copyfile(result, package)
    print(report['status'], 737, 'new;', regressions, 'regression checks;',
          report['combined_qualified_scoped_checks'], 'combined reported scopes', flush=True)


if __name__ == '__main__':
    main()
