"""Regress classic Exit, audit same-provider linkage, and freeze scoped evidence."""
import argparse,ast,hashlib,json,os,re,shlex,shutil,subprocess,sys
from pathlib import Path
from build_native_read_queue import owner_function
from package_native_sound_placement import prior

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def text(p):return Path(p).read_text(encoding='utf-8-sig').encode()
def block(raw,marker):
    assert raw.count(marker)==1
    start=raw.index(marker);at=raw.index(b'{',start);depth=1;end=at+1
    while depth:
        depth+=(raw[end]==123)-(raw[end]==125);end+=1
    return raw[start:end]

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('legacy','audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('baseline','inputs','check','owner','pixels','init','motion','legacy','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json');bh=sha(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_character_textures_package.json';seen=set();prior(r,previous,seen);assert len(seen)==30
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['asset_character_generated_source_sha256'].items():assert sha(build/n)==h,n
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256'],key
    if a.stage=='legacy':
        original=p/'jp_port_tools/verify_native_exit_logo.py';raw=original.read_bytes()
        old=b"for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n";new=b"for n,h in b['asset_character_generated_source_sha256'].items():assert sha(d/n)==h,n"
        assert raw.count(old)==1;raw=raw.replace(old,new);d.mkdir();caller=d/'private_classic_exit_caller.py';caller.write_bytes(raw);cases={}
        for case in ('cleanup','quit','pending_quit','partial','unbound'):
            child=d/case;child.mkdir();cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(build),'--inputs',str(a.inputs.resolve()),'--asset-root',str((a.inputs/'SONICADV').resolve()),'--output',str(child),'--worker',case]
            run=subprocess.run(cmd,capture_output=True,timeout=120,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),creationflags=subprocess.CREATE_NO_WINDOW)
            (child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr);expected=66 if case=='unbound' else 0
            assert run.returncode==expected and not run.stderr,(case,run.returncode,run.stderr.decode(errors='replace')[-6500:])
            v=read(child/'result.json');v['actual_process_exit_code']=run.returncode
            if case in ('quit','pending_quit'):
                destructor=read(child/'destructor.json');assert destructor['reentrant_cleanup']==4 and destructor['input_status']==4 and destructor['wait_status']==2
            cases[case]=v;print('classic',case,v['checks'],'checks',flush=True)
        assert cases['cleanup']['retained_asset_view_cleanup'] and cases['cleanup']['all_resources_completed']==16383
        out=dict(status='classic_unclaimed_Assets_bit32_retained_decoded_loader_view_task_destruction_pending_GPU_and_process_exits_regressed',checks=sum(v['checks'] for v in cases.values()),cases=cases,
            original_verifier_sha256=sha(original),private_caller_sha256=sha(caller),caller_only_adaptation='validate current generated-source manifest; native providers/calls/assertions unchanged',
            full_570_image_logo_suite_repeated=False,build_verification_sha256=bh,verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');return
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['asset_character_baseline_sha256']
        unchanged={};changed=[]
        for path in base.glob('*.dll'):
            if sha(path)==sha(build/path.name):unchanged[path.name]=sha(path)
            else:changed.append(path.name)
        assert len(unchanged)==20 and set(changed)=={Path(b[k]).name for k in ('frames_DLL','System_DLL','authored_exit_DLL','main_boot_DLL')}
        assert b['asset_character_new_build_commands']==10
        for row in b['commands'][:10]:
            assert row['exit_code']==row['diagnostic_bytes']==0
            if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
        for n,h in b['asset_character_original_source_sha256'].items():assert sha(n)==h,n
        for n,h in b['asset_character_kept_frame_objects_sha256'].items():assert sha(n)==h,n
        links=[]
        for meta in (old,b):links.append(next(x['command'] for x in meta['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
        objects=[{n for n in cmd if n.endswith('.obj')} for cmd in links]
        assert len(objects[0]-objects[1])==len(objects[1]-objects[0])==3
        assert objects[0]&objects[1]==set(b['asset_character_kept_frame_objects_sha256'])
        for name,h in old['character_texture_kept_objects_sha256'].items():
            if name in b['asset_character_kept_frame_objects_sha256']:assert b['asset_character_kept_frame_objects_sha256'][name]==h
        memory=next(Path(n) for n in b['asset_character_original_source_sha256'] if n.endswith('authored_pvm_memory.c'))
        assert text(build/'memory_asset_activity.c')==text(memory)+b'\n__declspec(dllexport) int jp_asset_loader_activity(void){return source!=NULL||pvm.list!=NULL||pvm.busy;}\n'
        source=text(base/'external_character_texture_family.c');now=text(build/'character_family_with_exit.c')
        for name in ('texture_load_character','texture_release_character','texture_release_characters'):assert owner_function(source,name)==owner_function(now,name),name
        old_exit=text(base/'exit_with_motions.c');now_exit=text(build/'exit_with_assets.c')
        for name in ('syBtExit','rollback_claims'):assert owner_function(old_exit,name)==owner_function(now_exit,name),name
        marker=b'if(state.info.remaining&JP_EXIT_ASSETS)';assert block(old_exit,marker)==block(now_exit,marker)
        assert b'JP_EXIT_ASSETS=32' in text(p/'native/jp_native_exit.h').replace(b' ',b'')
        assert b'#define JP_EXIT_CHARACTER_TEXTURES UINT32_C(33554432)' in text(p/'native/jp_native_character_exit.h')
        for name in ('system_with_motions.c','system_with_assets.c'):
            raw=text((base if name.endswith('motions.c') else build)/name)
            assert raw.count(b'jp_exit_prepare_bootstrap(CORE,bootstrap_cleanup,&system_owner)')==1
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        def pe(flag,path):
            run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr
            return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
        expected={
            'frames_DLL':{'jp_asset_loader_activity'}|{'jp_assets_exit_'+n for n in ('activity','snapshot','can_close','can_retire_after_character','can_retire','claim','cancel','close','finish','close_unclaimed')}|{'jp_character_exit_'+n for n in ('activity','snapshot','can_close','can_retire','claim','cancel','close','finish','close_unclaimed')},
            'System_DLL':{'jp_system_'+n for n in ('start_assets','adopt_assets','start_character_textures','adopt_character_textures')},
            'authored_exit_DLL':{'jp_exit_attach_bootstrap_assets','jp_exit_attach_bootstrap_characters'},'main_boot_DLL':set()}
        exports={}
        for key,names in expected.items():
            before=set(pe('--coff-exports',old[key]));after=set(pe('--coff-exports',b[key]));assert after==before|names,(key,sorted(after-before),sorted(names));exports[key]=sorted(names)
        graph={path.name.lower():[n.lower() for n in pe('--coff-imports',path) if (build/n).exists()] for path in build.glob('*.dll')};active=set();done=set()
        def visit(name):
            assert name not in active,('DLL cycle',name);active.add(name)
            for child in graph[name]:
                if child not in done:visit(child)
            active.remove(name);done.add(name)
        for name in graph:
            if name not in done:visit(name)
        out=dict(status='same_actual_asset_character_providers_single_bootstrap_legacy_ASSETS_body_preserved_and_acyclic_PE_link_audited',unchanged_DLL_sha256=unchanged,changed_DLLs=changed,
            unchanged_frame_objects=len(objects[0]&objects[1]),replaced_frame_objects=3,original_character_loop_bodies_preserved=3,original_memory_PVM_body_preserved=True,
            original_ASSETS_bit32_cleanup_block_unchanged=True,original_syBtExit_and_final_claim_rollback_unchanged=True,same_System_bootstrap_registration_verified=True,
            added_exports=exports,actual_local_PE_import_graph=graph,prior_packages_checked=30,build_verification_sha256=bh,auditor_sha256=sha(__file__),playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],len(unchanged),'unchanged DLLs',flush=True);return
    tools={'check':'verify_native_asset_character_exit.py','owner':'verify_native_character_textures.py','pixels':'verify_native_character_texture_pixels.py','init':'verify_native_game_init.py','motion':'verify_native_motion_exit.py','legacy':'qualify_native_asset_character_exit.py','audit':'qualify_native_asset_character_exit.py'}
    reports={};evidence={'build':build/'build_verification.json'}
    for n,tool in tools.items():
        path=getattr(a,n)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool)
        reports[n]=v;evidence[n]=path
    assert [reports[n]['checks'] for n in ('check','owner','pixels','init','motion')]==[1411,8199,3049,530,918]
    assert len(reports['check']['cases'])==19 and reports['check']['actual_process_exit_codes']==[0,70]
    assert reports['pixels']['actual_GPU_images']==418 and reports['pixels']['pixels_compared']==711168
    authored=[p/'native'/n for n in ('jp_native_asset_exit.h','jp_native_asset_exit.inc','jp_native_character_exit.h','jp_native_character_exit.inc','jp_native_system_assets.h','jp_native_system_assets.inc','jp_native_exit_assets.h','jp_native_exit_assets.inc')]+[p/'jp_port_tools'/n for n in ('build_native_asset_character_exit.py','verify_native_asset_character_exit.py','qualify_native_asset_character_exit.py')]+[p/'JP_NATIVE_ASSET_CHARACTER_EXIT.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    target=p/'analysis/jp_native_asset_character_exit_package.json';assert not target.exists();d.mkdir();proof={}
    for n,path in evidence.items():
        to=p/'analysis'/('jp_native_asset_character_exit_'+n+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    new=reports['check']['checks'];regression=sum(reports[n]['checks'] for n in ('owner','pixels','init','motion','legacy'));total=read(previous)['combined_qualified_scoped_checks']+new
    out=dict(status='same_actual_Assets_and_Character_ownership_System_MainBoot_bootstrap_final_Exit_retirement_restart_and_partial_recovery_qualified',new_checks=new,selected_regression_checks=regression,combined_qualified_scoped_checks=total,
        new_lifecycle_cases=19,classic_Exit_regression_cases=5,actual_character_GPU_images_regressed=418,pixels_compared=711168,
        original_game_function_bodies_newly_recovered=0,asset_capability_snapshot_bytes=64,character_capability_snapshot_bytes=48,existing_Assets_bit=32,new_character_bit=33554432,
        strict_live_user_decoded_view_and_SDK_loader_preflight=True,ordinary_restart_and_stale_capabilities_verified=True,zero_user_partial_texture_recovery_verified=True,
        late_History_failure_retry_and_global_terminalization_verified=True,actual_card_callback_order_verified=True,actual_process_exit_codes=[0,70,66],
        prior_qualifications_preserved={'character_textures':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},proof_files_sha256=proof,
        build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],private_System_DLL_sha256=b['System_DLL_sha256'],private_exit_DLL_sha256=b['authored_exit_DLL_sha256'],private_main_boot_DLL_sha256=b['main_boot_DLL_sha256'],
        automatic_asset_character_System_Exit_retirement_qualified=True,automatic_task_render_model_texture_view_acquisition=False,
        full_set_character_Chao_reset_3D_animation_GAME_MODE_BOOT_or_gameplay_verified=False,forced_allocation_failure_token_exhaustion_and_failed_claim_rollback_verified=False,
        original_source_and_proofs_preserved=True,retail_data_or_screenshots_or_combined_game_SDK_DLLs_publishable=False,public_research_build_payloads_changed=False,
        playable=False,overall_completion_percent=10,goal_active=True)
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target)
    print(out['status'],new,'new checks;',regression,'regressions;',total,'overlapping scoped checks',flush=True)
if __name__=='__main__':main()
