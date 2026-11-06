"""Audit and freeze the complete original initializer on the native lifecycle."""
import argparse, ast, hashlib, json, re, shutil, subprocess
from pathlib import Path
from build_native_read_queue import owner_function, PIN
from build_native_texture_runtime import function
from package_native_sound_placement import prior

def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path): return json.loads(Path(path).read_bytes())

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--stage', choices=('audit', 'package'), required=True)
    for n in ('workspace', 'build', 'output'):
        ap.add_argument('--' + n, type=Path, required=True)
    for n in ('upstream', 'baseline', 'check', 'regression', 'audit'):
        ap.add_argument('--' + n, type=Path)
    a = ap.parse_args(); r = a.workspace.resolve(); p = r/'outputs/sonic-adventure'
    build = a.build.resolve(); d = a.output.resolve(); b = read(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n, h in b['authored_source_sha256'].items(): assert sha(r/n) == h, n
    for n, h in b['game_init_generated_source_sha256'].items(): assert sha(build/n) == h, n
    for n, h in b['game_init_import_header_sha256'].items(): assert sha(build/'game_init_imports'/n) == h, n
    previous = p/'analysis/jp_native_read_queue_exit_package.json'; seen = set()
    prior(r, previous, seen); assert len(seen) == 24
    assert read(previous)['combined_qualified_scoped_checks'] == 944933
    if a.stage == 'audit':
        base = a.baseline.resolve(); old = read(base/'build_verification.json'); u = a.upstream.resolve()
        assert sha(base/'build_verification.json') == b['game_init_baseline_sha256']
        originals = {}
        for n, h in b['game_init_original_source_sha256'].items():
            raw = (u/n).read_bytes(); assert sha(u/n) == h
            assert raw == subprocess.check_output(['C:/Program Files/Git/cmd/git.exe', '-C', str(u), 'show', PIN+':'+n])
            originals[n] = raw
        body = function(originals['src/main.c'], 'game_init')
        assert hashlib.sha256(body).hexdigest() == b['game_init_original_body_sha256']
        native = function((build/'external_game_init.c').read_bytes(), 'game_init')
        check = b'    if(jp_game_init_failed())return;\n'
        assert native.count(check) == 7 and native.replace(check, b'') == body
        assert hashlib.sha256(native).hexdigest() == b['game_init_native_body_sha256']
        callback = function(originals['src/system/loader.c'], 'loader_note_error')
        assert function((build/'external_loader_note_error.c').read_bytes(), 'loader_note_error') == callback
        assert hashlib.sha256(callback).hexdigest() == b['game_init_loader_error_body_sha256']
        assert (build/'external_sofdec_preinit.c').read_bytes() == originals['sdk/sofdec/mwsfdini.c']
        config = (build/'native_sofdec_config.c').read_bytes()
        for decl in (b'Uint32 sygIntMode = SYD_INT_MODE_LOCALSTACK;', b'void *sygIntStack = NULL;'):
            assert originals['sdk/shinobi/syhw.c'].count(b'\n'+decl) == 1 and config.count(decl) == 1
        assert b'syHwInit(' not in config
        prior_system = base/'system_with_read_queue.c'; assert sha(prior_system) == b['game_init_prior_System_source_sha256']
        system = (build/'system_with_game_init.c').read_bytes()
        for name in ('bootstrap_cleanup', 'system_clock_activity'):
            assert owner_function(system, name) == owner_function(prior_system.read_bytes(), name)
        assert system.count(b'rc=jp_exit_prepare_bootstrap(') == 1
        guarded = ('jp_system_configure', 'jp_system_initialize', 'jp_system_bind_exit', 'jp_system_shutdown',
                   'jp_system_start_sound', 'jp_system_start_sound_clock', 'jp_system_main_initialization_guard',
                   'jp_system_adopt_boot_lifetimes', 'jp_system_adopt_history', 'jp_system_start_history',
                   'jp_system_adopt_read_queue', 'jp_system_start_read_queue')
        for name in guarded:
            f = owner_function(system, name)
            assert f.count(b'if(system_owner.game_init_token&&!system_owner.game_init_call)return JP_SYSTEM_REENTRANT;') == 1
        claim = owner_function(system, 'jp_system_game_init_claim')
        assert b'system_owner.info.resources!=(CORE|JP_EXIT_SOUND|JP_EXIT_CARD|JP_EXIT_PRINT)' in claim
        assert claim.index(b'jp_system_main_initialization_guard(1)') < claim.index(b'system_owner.game_init_token=')
        release = owner_function(system, 'jp_system_game_init_release')
        assert release.index(b'game_init_access(token)') < release.index(b'jp_system_main_initialization_guard(0)') < release.index(b'system_owner.game_init_token=0')
        for name, callee in (('queue', b'jp_system_start_read_queue()'), ('history', b'jp_system_start_history(options)')):
            f = owner_function(system, 'jp_system_game_init_'+name)
            assert f.index(b'game_init_access(token)') < f.index(b'system_owner.game_init_call=1') < f.index(callee) < f.index(b'system_owner.game_init_call=0')
        parent_row = next(x['command'] for x in old['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name == 'main_boot_with_lifetimes.c')
        parent_path = Path(parent_row[parent_row.index('-c')+1]); assert sha(parent_path) == b['game_init_prior_MainBoot_source_sha256']
        parent = (build/'main_boot_with_game_init.c').read_bytes()
        assert owner_function(parent, 'jp_main_boot_initialize') == owner_function(parent_path.read_bytes(), 'jp_main_boot_initialize')
        shutdown = owner_function(parent, 'jp_main_boot_shutdown')
        assert shutdown.index(b'jp_system_game_init_activity()') < shutdown.index(b'buExit(')
        preserved = {}; imports = {}; reader = Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        changed = {'System_DLL', 'main_boot_DLL'}
        for k, v in old.items():
            if (k == 'DLL' or k.endswith('_DLL')) and isinstance(v, str) and v.endswith('.dll'):
                assert sha(b[k]) == b[k+'_sha256']; name = Path(v).name.lower()
                if k not in changed: assert sha(v) == sha(b[k]); preserved[name] = sha(v)
                else: assert sha(v) != sha(b[k])
        assert len(preserved) == 17
        for dll in build.glob('*.dll'):
            run = subprocess.run([str(reader), '--coff-imports', str(dll)], capture_output=True)
            assert not run.returncode and not run.stderr
            imports[dll.name.lower()] = [n.lower() for n in re.findall(r'^  Name: (.+)$', run.stdout.decode(), re.M)]
        assert len(imports) == 23 == len(list(base.glob('*.dll')))+1
        support = {}
        for dll in base.glob('*.dll'):
            if dll.name.lower() not in {'jp_native_system.dll', 'jp_native_main_boot.dll'}:
                assert sha(dll) == sha(build/dll.name)
                if dll.name.lower() not in preserved: support[dll.name.lower()] = sha(dll)
        assert len(support) == 3
        assert all('jp_native_game_init.dll' not in rows for rows in imports.values())
        assert {n for n in imports['jp_native_game_init.dll'] if n.startswith('jp_native_')} == {
            'jp_native_main_boot.dll', 'jp_native_system.dll', 'jp_native_present.dll', 'jp_native_game_sound.dll',
            'jp_native_gdfs.dll', 'jp_native_history.dll', 'jp_native_read_queue.dll'}
        done = set()
        def visit(n, stack):
            assert n not in stack, ('PE import cycle', stack, n)
            if n in done: return
            for dep in imports[n]:
                if dep in imports: visit(dep, stack+[n])
            done.add(n)
        for name in imports: visit(name, [])
        objects = {}
        for path in base.glob('*.obj'):
            assert sha(path) == sha(build/path.name); objects[path.name] = sha(path)
        assert len(objects) == 34
        expected = {'jp_native_system.dll': ['system_game_init_owner.obj'],
                    'jp_native_main_boot.dll': ['main_boot_original.obj', 'main_boot_game_init_owner.obj'],
                    'jp_native_game_init.dll': ['game_init_original.obj', 'loader_error_original.obj',
                        'sofdec_preinit_original.obj', 'sofdec_native_config.obj', 'game_init_owner.obj']}
        for row in b['commands'][:10]:
            assert row['exit_code'] == row['diagnostic_bytes'] == 0
            cmd = row['command']
            if '-shared' in cmd:
                assert [Path(x).name for x in cmd if x.endswith('.obj')] == expected.pop(Path(cmd[-1]).name)
                if Path(cmd[-1]).name == 'jp_native_main_boot.dll':
                    old_link = next(x['command'] for x in old['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name == 'jp_native_main_boot.dll')
                    original = next(Path(x) for x in old_link if Path(x).name == 'main_boot_original.obj')
                    assert sha(original) == sha(build/'main_boot_original.obj')
        assert not expected and b['game_init_new_build_commands'] == 10
        export_run = subprocess.run([str(reader), '--coff-exports', b['game_init_DLL']], capture_output=True, check=True)
        exports = re.findall(r'^  Name: (.+)$', export_run.stdout.decode(), re.M)
        for name in ('game_mode', 'loader_error_flag', 'mwsfdg_intStack', 'sygIntMode', 'sygIntStack', 'game_init', 'loader_note_error'):
            assert exports.count(name) == 1
        out = dict(status='complete_original_initializer_order_shared_globals_Sofdec_storage_and_same_native_System_graph_audited',
            build_verification_sha256=sha(build/'build_verification.json'), auditor_sha256=sha(__file__),
            original_successful_body_unchanged_except_seven_checked_returns=True, original_loader_error_body_unchanged=True,
            original_Sofdec_preinit_translation_unit_unchanged=True, writable_configuration_from_original_GCC_definitions=True,
            original_System_bootstrap_callback_and_clock_activity_unchanged=True, original_MainBoot_initializer_and_object_unchanged=True,
            existing_System_entrypoints_guarded=len(guarded), actual_PE_imports=imports, unchanged_prior_DLLs=preserved,
            unchanged_prior_objects=objects, unchanged_extra_support_DLLs=support,
            prior_packages_checked=len(seen), no_higher_module_import_cycle=True,
            active_Windows_interrupt_stack_or_movie_playback_verified=False, whole_game_main_loop_or_gameplay_verified=False,
            public_combined_game_SDK_DLLs=False, playable=False, overall_completion_percent=10)
        d.mkdir(); (d/'verification.json').write_text(json.dumps(out, indent=2)+'\n')
        print(out['status'], '17 unchanged provider DLLs+3 support DLLs;34 unchanged objects;24 prior packages', flush=True); return
    reports = {}; evidence = {'build': build/'build_verification.json'}
    for n, tool in (('check', 'verify_native_game_init.py'), ('regression', 'verify_native_read_queue_exit.py'), ('audit', 'qualify_native_game_init.py')):
        path = getattr(a, n)/'verification.json'; value = read(path)
        assert value['build_verification_sha256'] == sha(build/'build_verification.json')
        assert value['auditor_sha256' if n == 'audit' else 'verifier_sha256'] == sha(p/'jp_port_tools'/tool)
        reports[n] = value; evidence[n] = path
    assert reports['check']['checks'] == 530 and len(reports['check']['cases']) == 19 and reports['check']['full_game_init_executed']
    assert reports['regression']['checks'] == 968 and len(reports['regression']['cases']) == 21
    assert len(reports['audit']['unchanged_prior_DLLs']) == 17 and len(reports['audit']['unchanged_prior_objects']) == 34
    for key in ('game_init_DLL', 'System_DLL', 'main_boot_DLL'): assert sha(b[key]) == b[key+'_sha256']
    authored = [p/'native'/n for n in ('jp_native_game_init.h', 'jp_native_game_init.c', 'jp_native_system_game_init.h', 'jp_native_system_game_init.inc')]
    authored += [p/'jp_port_tools'/n for n in ('build_native_game_init.py', 'verify_native_game_init.py', 'qualify_native_game_init.py')]
    authored += [p/'JP_NATIVE_GAME_INIT.txt']
    for path in authored:
        if path.suffix == '.py': ast.parse(path.read_text(encoding='utf-8-sig'))
    targets = {n: p/'analysis'/('jp_native_game_init_'+n+'.json') for n in evidence}
    package = p/'analysis/jp_native_game_init_package.json'
    assert not package.exists() and all(not x.exists() for x in targets.values()); d.mkdir(); proof = {}
    for n, target in targets.items():
        shutil.copyfile(evidence[n], target); proof[target.relative_to(r).as_posix()] = sha(target)
    out = dict(status='complete_original_game_init_native_parent_two_frames_owned_audio_queue_History_and_error_callback_scoped_qualification',
        new_checks=530, cases=19, selected_regression_checks=968, combined_qualified_scoped_checks=945463,
        complete_original_game_init_executed=True, real_native_presentations=2, real_JP_file_bytes_and_GDFS_error_callback_verified=True,
        shared_original_main_globals_and_Sofdec_configuration_storage_verified=True,
        actual_partial_constructor_failure_and_cleanup_retry_verified=True, actual_owned_ADXT_player_after_initialization_verified=True,
        same_System_constructor_capability_and_MainBoot_preflight_verified=True,
        prior_qualifications_preserved={'read_queue_exit': dict(path=previous.relative_to(r).as_posix(), sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix(): sha(x) for x in authored}, proof_files_sha256=proof,
        build_verification_sha256=sha(build/'build_verification.json'),
        private_DLL_sha256={n: b[n+'_sha256'] for n in ('game_init_DLL', 'System_DLL', 'main_boot_DLL')},
        game_mode_step_or_main_loop_or_gameplay_executed=False, active_Windows_interrupt_stack_or_movie_playback_verified=False,
        actual_OS_cancel_timeout_or_token_exhaustion_or_failed_rollback_cancellation_verified=False,
        combined_game_SDK_binaries_publishable=False, retail_assets_or_profiles_or_private_fixtures_packaged=False,
        public_research_build_payloads_changed=False, playable=False, overall_completion_percent=10, goal_active=True)
    path = d/'qualification.json'; path.write_text(json.dumps(out, indent=2)+'\n'); shutil.copyfile(path, package)
    print(out['status'], '530 new checks;968 regression checks;945463 combined reported scopes', flush=True)

if __name__ == '__main__': main()
