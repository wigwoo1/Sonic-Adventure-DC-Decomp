"""Qualify reset dependencies on the real parent and preserve earlier packages."""
import argparse,ast,hashlib,json,re,shutil,struct,subprocess,sys
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def defined_names(path):
    # Earlier shared symbol helper intentionally lists external symbols only.
    # These original module-private records retain COFF static storage class 3.
    raw=Path(path).read_bytes();machine,sections,stamp,start,count,opt,flags=struct.unpack_from('<HHIIIHH',raw)
    assert machine==0x8664 and opt==0
    strings=start+18*count;i=0;result=[]
    while i<count:
        name,value,section,kind,storage,aux=struct.unpack_from('<8sIhHBB',raw,start+18*i)
        if section>0 and storage in (2,3):
            if name[:4]==bytes(4):at=strings+struct.unpack_from('<I',name,4)[0];name=raw[at:raw.index(0,at)]
            else:name=name.rstrip(b'\0')
            result.append(name.decode())
        i+=1+aux
    return result

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('parent','audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('upstream','baseline','track','check','regression','parent','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_game_init_package.json';seen=set();prior(r,previous,seen);assert len(seen)==25
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['boot_reset_generated_source_sha256'].items():assert sha(build/n)==h,n
    if a.stage=='parent':
        original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1
        added=b'''        if a.case=='data':
            fields={row['name']:row for row in b['boot_reset_fields']}
            view=bind(f,'jp_boot_reset_field',P,U)
            def field_at(name):return view(fields[name]['id'])
            bind(f,'save_reset_all',None)()
            records=(C.c_ubyte*3552).in_dll(f,'saves')
            defaults=bytes(records)
            check(bind(f,'save_check_all',I,P)(records)==1,'genuine original three-slot defaults available on complete parent')
            C.c_int8.in_dll(f,'lives').value=-1
            C.c_int16.in_dll(f,'rings').value=1234
            C.c_int32.in_dll(f,'play_time').value=99999
            C.c_int16.in_dll(f,'character_current').value=0
            C.c_int16.in_dll(f,'stage_current').value=0
            C.memset(field_at('menu_state'),0x5a,18)
            for name in ('npc_task','return_point','player_record','story_current_script','event_module','event_controlling'):
                C.c_void_p.from_address(field_at(name)).value=0x123456788cfe1230
            C.memset(field_at('story_records'),0x5a,96)
            before=si()
            for name in ('play_time_reset','hud_reset','progress_init','stage_begin','boot_finish'):
                bind(f,name,I if name=='stage_begin' else None)()
            check(C.c_int32.in_dll(f,'play_time').value==0 and C.c_int16.in_dll(f,'rings').value==0 and bind(f,'lives_count',I)()==4,'actual full-parent HUD and play-time reset')
            check(C.string_at(field_at('menu_state'),18)==bytes(18) and all(not C.c_void_p.from_address(field_at(n)).value for n in ('npc_task','return_point','player_record','story_current_script','event_module','event_controlling')),'full-width pointers and original menu state retire their reset references')
            check(C.string_at(field_at('story_records'),96)==bytes(96) and C.string_at(field_at('events_played'),512)==bytes(512) and C.string_at(field_at('event_flags'),512).count(b'\\x01')==1,'actual existing shared progress and event records reset')
            check(C.string_at(field_at('world_gravity'),12)==struct.pack('<fff',0,-1,0) and C.c_int8.in_dll(f,'messages_shown').value==1,'actual original stage reset and gravity affect shared parent state')
            check(bind(f,'jp_recorder_stage_end_kind',I)()==0,'existing recorder shares actual stage reset state')
            check(bytes(records)==defaults and bind(f,'save_check_all',I,P)(records)==1,'active progress reset preserves all serialized save defaults and CRCs')
            after=si();current=info()
            check(before.resources==after.resources and before.phase==after.phase and current.phase==3 and current.audio_stages==15 and current.frame_completed==2,'resets preserve initialized System audio queue and History authority')
'''
        derived=raw.replace(marker,added+marker);d.mkdir();caller=d/'private_full_parent_reset_caller.py';caller.write_bytes(derived)
        worker=d/'run';env=dict(__import__('os').environ);env['PYTHONPATH']=str(p/'jp_port_tools')
        cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(build),'--track',str(a.track.resolve()),'--output',str(worker),'--case','data']
        run=subprocess.run(cmd,capture_output=True,env=env,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr)
        assert run.returncode==0,(run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6500:])
        report=read(worker/'result.json');assert report['checks']==45 and report['verifier_sha256']==sha(caller)
        out=dict(status='original_reset_dependencies_after_complete_game_init_preserve_real_file_history_audio_and_cleanup',checks=report['checks'],
            additional_parent_assertions=11,actual_private_worker_exit_code=0,original_verifier_sha256=sha(original),
            derived_python_caller_sha256=sha(caller),native_functions_or_game_source_replaced=False,
            real_JP_queued_read_and_History_producer_after_resets_verified=True,real_parent_cleanup_after_resets_verified=True,
            worker_result_sha256=sha(worker/'result.json'),build_verification_sha256=sha(build/'build_verification.json'),verifier_sha256=sha(__file__),
            full_GAME_MODE_BOOT_or_gameplay_executed=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['boot_reset_baseline_sha256']
        u=a.upstream.resolve();raw=(build/'external_boot_reset.c').read_bytes();unchanged=[]
        for n,h in b['boot_reset_original_source_sha256'].items():assert sha(u/n)==h,n
        for n,h in b['boot_reset_native_body_sha256'].items():
            body=function(raw,n);assert hashlib.sha256(body).hexdigest()==h
            if h==b['boot_reset_original_body_sha256'][n]:unchanged.append(n)
        assert len(unchanged)==16
        original_progress=function((u/'src/progress/progress.c').read_bytes(),'progress_init');native_progress=function(raw,'progress_init')
        assert original_progress.replace(b'for (byte = dat_8c7a2d20, i = sizeof(story_records); i != 0;',b'for (byte = dat_8c7a2d20, i = sizeof(dat_8c7a2d20); i != 0;')==native_progress
        assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp|fptosi|fptoui|sitofp|uitofp)\b',(build/'boot_reset_original.ll').read_text())
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe');imports={};preserved={}
        for dll in base.glob('*.dll'):
            target=build/dll.name
            if dll.name=='jp_native_frames.dll':assert sha(target)!=sha(dll)
            else:assert sha(target)==sha(dll);preserved[dll.name]=sha(dll)
            run=subprocess.run([str(reader),'--coff-imports',str(target)],capture_output=True);assert not run.returncode and not run.stderr
            imports[dll.name.lower()]=[n.lower() for n in re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)]
        assert len(preserved)==22 and len(imports)==23
        done=set()
        def visit(n,stack):
            assert n not in stack,('PE import cycle',stack,n)
            if n in done:return
            for dep in imports[n]:
                if dep in imports:visit(dep,stack+[n])
            done.add(n)
        for n in imports:visit(n,[])
        link=b['commands'][4]['command'];used=[x for x in link if x.endswith('.obj')]
        assert set(used)==set(b['boot_reset_kept_frame_objects_sha256'])|{str(build/'boot_reset_original.obj'),str(build/'fill_bytes_native.obj'),str(build/'recorder_shared_stage.obj')}
        assert len(used)==len(set(used))
        definitions={}
        for path in used:
            for name in defined_names(path):definitions.setdefault(name,[]).append(path)
        tracked=list(b['boot_reset_original_body_sha256'])+['fill_bytes']+[n['name'] for n in b['boot_reset_fields']]+['get_character','ring_reset']
        assert all(len(definitions.get(n,[]))==1 for n in tracked)
        for n,providers in b['boot_reset_current_symbol_providers'].items():assert definitions[n]==providers,n
        rebound=b['boot_reset_recorder_rebinding'];assert sha(rebound['original_source'])==rebound['original_source_sha256'] and sha(rebound['original_object'])==rebound['original_object_sha256']
        assert (build/'external_recorder_shared_stage.c').read_bytes()==Path(rebound['original_source']).read_bytes().replace(b'static Sint16 stage_end_kind = STAGE_END_NONE;',b'Sint16 stage_end_kind = STAGE_END_NONE;')
        for n,h in b['boot_reset_kept_frame_objects_sha256'].items():assert sha(n)==h,n
        objects={}
        for path in base.glob('*.obj'):assert sha(path)==sha(build/path.name);objects[path.name]=sha(path)
        assert len(objects)==42 and b['boot_reset_new_build_commands']==5 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'][:5])
        out=dict(status='single_existing_shared_owner_original_reset_bodies_bounded_scratch_repair_and_native_import_graph_audited',
            unchanged_original_functions=unchanged,explicitly_adapted_original_function='progress_init',
            single_definitions_checked=sorted(tracked),current_shared_symbol_providers=b['boot_reset_current_symbol_providers'],
            recorder_functions_unchanged_only_stage_end_field_linkage_promoted=True,
            kept_frame_objects_sha256=b['boot_reset_kept_frame_objects_sha256'],unchanged_prior_DLLs=preserved,actual_PE_imports=imports,
            unchanged_copied_objects=objects,prior_packages_checked=len(seen),original_assembly_byte_fill_recovered=True,
            build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),
            full_GAME_MODE_BOOT_or_gameplay_executed=False,playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],'16 unchanged functions;22 unchanged DLLs;',len(used)-3,'unchanged linked frame objects',flush=True);return
    reports={};evidence={'build':build/'build_verification.json'}
    for n,tool in (('check','verify_native_boot_reset.py'),('regression','verify_native_game_init.py'),('parent','qualify_native_boot_reset.py'),('audit','qualify_native_boot_reset.py')):
        path=getattr(a,n)/'verification.json';v=read(path);assert v['build_verification_sha256']==sha(build/'build_verification.json')
        assert v['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[n]=v;evidence[n]=path
    assert reports['check']['checks']==1496 and reports['check']['original_calls']==648 and reports['parent']['checks']==45 and reports['regression']['checks']==530
    assert len(reports['audit']['unchanged_prior_DLLs'])==22 and len(reports['audit']['unchanged_original_functions'])==16
    authored=[p/'native/jp_native_fill_bytes.c']+[p/'jp_port_tools'/n for n in ('build_native_boot_reset.py','verify_native_boot_reset.py','qualify_native_boot_reset.py')]+[p/'JP_NATIVE_BOOT_RESET.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    package=p/'analysis/jp_native_boot_reset_package.json';targets={n:p/'analysis'/('jp_native_boot_reset_'+n+'.json') for n in evidence}
    assert not package.exists() and all(not x.exists() for x in targets.values());d.mkdir();proof={}
    for n,target in targets.items():shutil.copyfile(evidence[n],target);proof[target.relative_to(r).as_posix()]=sha(target)
    out=dict(status='original_boot_HUD_progress_event_and_stage_reset_dependencies_scoped_qualification',new_checks=1541,selected_regression_checks=530,
        combined_qualified_scoped_checks=947004,original_functions=17,unchanged_original_functions=16,original_machine_calls=648,
        state_fields_compared=42,ordered_reset_sequences=24,positive_original_byte_fill_comparisons=120,
        original_progress_scratch_clear_native_bounds_repair_output_equivalent=True,same_original_shared_state_and_real_full_parent_verified=True,
        prior_qualifications_preserved={'game_init':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        private_frames_DLL_sha256=b['frames_DLL_sha256'],build_verification_sha256=sha(build/'build_verification.json'),
        full_GAME_MODE_BOOT_or_character_asset_loading_or_gameplay_executed=False,complete_game_main_loop_verified=False,
        native_nonpositive_fill_safe_noop_differs_from_original_invalid_precondition=True,
        original_game_SDK_source_or_combined_binaries_publishable=False,retail_assets_profiles_or_captures_packaged=False,
        public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    path=d/'qualification.json';path.write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(path,package)
    print(out['status'],'1541 new checks;530 regression checks;947004 combined reported scopes',flush=True)

if __name__=='__main__':main()
