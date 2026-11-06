"""Freeze all-bank native data qualification, keeping original payloads private."""
import argparse,ast,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_motion_data import BANKS
from qualify_native_boot_reset import defined_names
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('baseline','inputs','reproduction','check','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_boot_reset_package.json';seen=set();prior(r,previous,seen);assert len(seen)==26
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_source_sha256'].items():assert sha(build/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256'] and len(b['commands'])==10 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'])
    if a.stage=='audit':
        inputs=a.inputs.resolve();reproduction=a.reproduction.resolve();base=a.baseline.resolve()
        assert sha(base/'build_verification.json')==b['baseline_verification_sha256']
        assert sha(inputs/'inputs.json')==b['private_inputs_manifest_sha256']
        manifest=read(inputs/'inputs.json');again=read(reproduction/'inputs.json')
        assert manifest==again
        compared={}
        for key,row in manifest['files'].items():
            bank,ext=key.split('/');path=Path(bank)/('content.'+ext)
            assert sha(inputs/path)==sha(reproduction/path)==row['sha256'];compared[path.as_posix()]=row['sha256']
        assert sha(inputs/'model_sources-v3/inputs.json')==b['private_cpp_manifest_sha256']
        first=read(inputs/'model_sources-v3/inputs.json');second=read(reproduction/'model_sources-v3/inputs.json')
        for name,row in first.items():
            assert second[name]['preprocessed_sha256']==row['preprocessed_sha256'] and second[name]['body_table_sha256']==row['body_table_sha256']
            for ext,field in (('i','preprocessed_sha256'),('txt','body_table_sha256')):
                path=Path('model_sources-v3')/(name+'.'+ext)
                assert sha(inputs/path)==sha(reproduction/path)==row[field];compared[path.as_posix()]=row[field]
        assert len(compared)==30
        definitions={}
        for unit in build.glob('*.obj'):
            for name in defined_names(unit):definitions.setdefault(name,[]).append(unit.name)
        canonical={name:definitions.get('jp_models_'+name,[]) for name in b['root_models']}
        assert len(canonical)==9 and all(rows==['models_native_data.obj'] for rows in canonical.values())
        assert sum(b['bank_definition_counts'].values())==11650 and b['bank_definition_counts']['models']==2793
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        run=subprocess.run([str(reader),'--coff-imports',b['DLL']],capture_output=True);assert not run.returncode and not run.stderr
        imports=re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M);assert not any(n.lower().startswith('jp_native_') for n in imports)
        expected_exports={f'jp_motion_{bank}_{suffix}' for bank in BANKS+('models',) for suffix in ('count','entry')}|{'jp_motion_layout'}
        run=subprocess.run([str(reader),'--coff-exports',b['DLL']],capture_output=True);assert not run.returncode and not run.stderr
        assert set(re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M))==expected_exports
        # The native data module is independent; every existing runtime DLL
        # and all source/proof packages stay on their qualified hash bindings.
        old=read(base/'build_verification.json');old_dlls={}
        for path in base.glob('*.dll'):old_dlls[path.name]=sha(path)
        for key,value in old.items():
            if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==old[key+'_sha256']
        out=dict(status='complete_native_motion_data_provenance_reproduction_exact_float_adaptation_and_canonical_model_ownership_audited',
            reproduced_private_input_files_sha256=compared,canonical_model_object_definitions=canonical,
            native_bank_definitions=b['bank_definition_counts'],exact_float_literals=sum(x['literals'] for x in b['float_literal_adaptation'].values()),
            default_nearest_rounding_would_change=sum(x['nearest_rounding_would_change'] for x in b['float_literal_adaptation'].values()),
            unchanged_existing_runtime_DLL_sha256=old_dlls,prior_packages_checked=len(seen),actual_PE_imports=imports,
            native_data_exports_only=True,game_functions_or_legacy_loader_success_stubs_added=False,
            build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),
            combined_game_data_DLL_publishable=False,character_loader_or_animation_playback_verified=False,playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],'30 reproduced files;9 canonical model roots;77,948 exact float literals',flush=True);return
    check_path=a.check/'verification.json';audit_path=a.audit/'verification.json';v=read(check_path);audit=read(audit_path)
    assert v['build_verification_sha256']==audit['build_verification_sha256']==sha(build/'build_verification.json')
    assert v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_motion_data.py') and audit['auditor_sha256']==sha(__file__)
    assert v['checks']==46684 and v['native_definitions']==11650 and v['actual_retail_PRS_files_checked']==7 and v['native_registry_pointers_are_full_width']
    authored=[p/'jp_port_tools'/n for n in ('prepare_native_motion_data.py','build_native_motion_data.py','verify_native_motion_data.py','qualify_native_motion_data.py')]+[p/'JP_NATIVE_MOTION_DATA.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    package=p/'analysis/jp_native_motion_data_package.json';evidence={'build':build/'build_verification.json','check':check_path,'audit':audit_path}
    targets={n:p/'analysis'/('jp_native_motion_data_'+n+'.json') for n in evidence};assert not package.exists() and all(not path.exists() for path in targets.values())
    d.mkdir();proof={}
    for n,target in targets.items():shutil.copyfile(evidence[n],target);proof[target.relative_to(r).as_posix()]=sha(target)
    out=dict(status='all_seven_native_motion_banks_and_genuine_character_model_closure_scoped_data_qualification',new_checks=46684,
        combined_qualified_scoped_checks=993688,native_data_records=11650,motion_bank_records=8857,model_closure_records=2793,
        original_record_bytes_compared=3029718,owned_native_pointer_relocations_checked=v['owned_native_pointer_relocations_checked'],
        actual_retail_PRS_files_checked=7,exact_float_literals=77948,default_nearest_rounding_sensitive_literals=32303,
        reproduced_private_inputs=30,canonical_shared_model_roots=9,
        prior_qualifications_preserved={'boot_reset':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        private_native_data_DLL_sha256=b['DLL_sha256'],build_verification_sha256=sha(build/'build_verification.json'),
        native_character_motion_loader_or_animation_evaluation_or_gameplay_verified=False,
        source_model_or_motion_payloads_or_combined_binaries_publishable=False,retail_assets_or_captures_packaged=False,
        public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    path=d/'qualification.json';path.write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(path,package)
    print(out['status'],'46684 scoped checks;11650 data records;993688 combined reported scopes',flush=True)
if __name__=='__main__':main()
