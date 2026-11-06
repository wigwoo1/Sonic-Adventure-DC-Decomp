"""Verify full-parent use, audit source ownership, and freeze character scope."""
import argparse,ast,hashlib,json,os,re,shlex,shutil,subprocess,sys
from pathlib import Path
from build_native_texture_runtime import function
from package_native_sound_placement import prior
from build_native_character_textures import definitions
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('parent','audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('baseline','inputs','track','check','pixels','vq','regression','motion','parent','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json');bh=sha(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');previous=p/'analysis/jp_native_motion_exit_package.json';seen=set();prior(r,previous,seen);assert len(seen)==29
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['character_texture_generated_source_sha256'].items():assert sha(build/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(b['GPU_DLL'])==b['GPU_DLL_sha256']
    if a.stage=='parent':
        original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1
        added=b'''        if a.case=='data':
            from native_asset_test_support import Assets,Limits
            from verify_native_motion_store import Info as MotionInfo,View
            assets=Assets(f);motions=C.CDLL(b['motion_DLL'])
            before_surfaces=bind(f,'jp_textures_live_surfaces',U)()
            check(assets.init(CHARACTER_ASSET_ROOT,C.byref(Limits(4<<20,8<<20,32<<20,512)))==0,'actual character asset provider after complete game_init')
            check(bind(system,'jp_system_start_motions',I,S,U)(8<<20,8)==0 and bind(motions,'jp_motions_select',I,I)(0)==0,'same System starts actual original Sonic motion selection')
            action=View();model=View()
            check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(0,b'sonic_updraft_action',C.byref(action))==0 and bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(7,b'sonic_object',C.byref(model))==0,'actual full-parent model/action views retained by caller')
            check(bind(f,'jp_character_textures_initialize',I)()==0 and bind(f,'jp_character_textures_acquire',I,U)(0)==0,'genuine original Sonic texture loops load actual retail files on complete parent')
            view=P();check(bind(f,'jp_character_textures_list',I,U,U,P)(0,0,C.byref(view))==0 and view.value>0xffffffff,'actual canonical original Sonic texture-list binding available')
            before=si();check(shutdown(5000)==11 and clean()==6 and si().resources==before.resources,'retained actual model/action consumers protect full parent while caller-owned textures are live')
            old_present=snapshot(present,'jp_present_snapshot',PresentInfo).presentations
            bind(present,'frame_wait',None)();bind(present,'frame_wait',None)()
            check(snapshot(present,'jp_present_snapshot',PresentInfo).presentations==old_present+2,'actual game clock/audio/presentation progresses with genuine character textures and model views live')
            check(bind(f,'jp_character_textures_shutdown',I)()==8,'caller cannot close character owner before original texture references retire')
            check(bind(f,'jp_character_textures_release',I,U)(0)==0 and bind(f,'jp_character_textures_shutdown',I)()==0,'caller explicitly closes genuine texture consumer before parent teardown')
            check(assets.evict()==0 and assets.shutdown()==0 and bind(f,'jp_textures_live_surfaces',U)()==before_surfaces,'actual character-only file/texture references retire without removing earlier parent surfaces')
            check(bind(motions,'jp_motions_release',I,Q)(action.token)==0 and bind(motions,'jp_motions_release',I,Q)(model.token)==0,'actual model/action consumer releases protection only after its texture users retire')
'''
        anchor=b"('jp_textures_initialize',(32,1024,32<<20))";assert raw.count(anchor)==1;raw=raw.replace(anchor,b"('jp_textures_initialize',(1024,4096,128<<20))")
        raw=raw.replace(marker,added+marker);end=b'    faulthandler.disable();fault.close();out=';assert raw.count(end)==1
        raw=raw.replace(end,b"    check(not snapshot(motions,'jp_motions_snapshot',MotionInfo).allocated_bytes,'same System automatically closes actual motion owner after subsequent real file/History work')\n"+end)
        root=repr(str((a.inputs/'assets').resolve())).encode();raw=b'CHARACTER_ASSET_ROOT='+root+b'\n'+raw;d.mkdir();caller=d/'private_complete_parent_character_caller.py';caller.write_bytes(raw)
        cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(build),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','data']
        run=subprocess.run(cmd,capture_output=True,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),timeout=180,creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr)
        assert not run.returncode,(run.returncode,run.stderr.decode(errors='replace')[-6000:]);v=read(d/'run/result.json')
        out=dict(status='genuine_character_texture_model_action_views_and_explicit_consumer_retirement_on_complete_parent_verified',checks=v['checks'],additional_real_native_presentations=2,
            subsequent_actual_disc_read_History_and_System_motion_cleanup_verified=True,automatic_character_texture_System_Exit_authority=False,
            original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller),worker_result_sha256=sha(d/'run/result.json'),
            build_verification_sha256=bh,verifier_sha256=sha(__file__),full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],v['checks'],'checks',flush=True);return
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['character_texture_baseline_sha256']
        iv=read(a.inputs/'inputs.json');assert sha(a.inputs/'inputs.json')==b['character_texture_input_manifest_sha256']
        unchanged={}
        for path in base.glob('*.dll'):
            if path.name in ('jp_native_frames.dll','jp_native_window_input.dll'):assert sha(path)!=sha(build/path.name)
            else:assert sha(path)==sha(build/path.name);unchanged[path.name]=sha(path)
        assert len(unchanged)==22
        for name,h in dict(b['character_texture_kept_objects_sha256'],**b['character_texture_kept_GPU_objects_sha256']).items():assert sha(name)==h,name
        assert b['character_texture_new_build_commands']==16 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'][:16])
        for row in b['commands'][:16]:
            if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
        raw=(build/'external_character_texture_family.c').read_bytes();source=(a.inputs/'original_texture.c').read_bytes()
        for name,h in b['character_texture_original_body_sha256'].items():assert function(raw,name)==function(source,name) and hashlib.sha256(function(raw,name)).hexdigest()==h
        adaptations={};definitions_count=0
        for name,row in b['character_texture_selected_definitions'].items():
            original=definitions((a.inputs/(name+'.i')).read_text());current=definitions((build/('external_character_'+name+'.c')).read_text());assert set(current)==set(row['names'])
            for symbol,(head,body) in current.items():
                before=original[symbol][1];assert hashlib.sha256(before.encode()).hexdigest()==row['original_definition_sha256'][symbol]
                expected=before.replace('static const ','static ',1) if symbol in row['writable_native_texture_names'] else before
                assert body==expected;definitions_count+=1
            adaptations[name]=row['writable_native_texture_names']
        assert [symbol for names in adaptations.values() for symbol in names]==['e102hit_textures']
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        def pe(flag,path):
            run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr;return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
        old_exports=set(pe('--coff-exports',old['frames_DLL']));now=set(pe('--coff-exports',b['frames_DLL']))
        expected={x['name'] for x in b['character_texture_registry']}|{'character_users','e102beam_textures','jp_character_texture_definition_count','jp_character_texture_definition'}|{'jp_character_textures_'+n for n in ('initialize','acquire','release','release_all','recover','shutdown','snapshot','list')}
        assert now==old_exports|expected and not {'texture_load_character','texture_release_character','texture_release_characters','jp_original_texture_load_character','jp_original_texture_release_character','jp_original_texture_release_characters'}&now
        assert set(pe('--coff-exports',old['GPU_DLL']))==set(pe('--coff-exports',b['GPU_DLL']))
        assert not any('legacy' in n for n in pe('--coff-exports',b['GPU_DLL']))
        graph={}
        for path in build.glob('*.dll'):graph[path.name.lower()]=[n.lower() for n in pe('--coff-imports',path) if (build/n).exists()]
        active=set();done=set()
        def visit(name):
            assert name not in active,('DLL cycle',name);active.add(name)
            for child in graph[name]:
                if child not in done:visit(child)
            active.remove(name);done.add(name)
        for name in graph:
            if name not in done:visit(name)
        out=dict(status='original_character_data_reference_loops_single_texture_owner_and_extended_VQ1_GPU_link_audited',unchanged_DLL_sha256=unchanged,
            unchanged_frame_objects=len(b['character_texture_kept_objects_sha256']),unchanged_GPU_objects=len(b['character_texture_kept_GPU_objects_sha256']),
            original_texture_definitions=definitions_count,original_function_bodies_preserved=3,original_table_and_character_users_preserved=True,writable_native_adaptations=adaptations,
            extra_Frame_exports=sorted(expected),GPU_exports_preserved=True,actual_local_PE_import_graph=graph,prior_packages_checked=29,
            build_verification_sha256=bh,auditor_sha256=sha(__file__),full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],definitions_count,'data definitions;22 unchanged DLLs',flush=True);return
    tools={'check':'verify_native_character_textures.py','pixels':'verify_native_character_texture_pixels.py','vq':'verify_native_vq1_mip.py','regression':'verify_native_game_init.py','motion':'verify_native_motion_exit.py','parent':'qualify_native_character_textures.py','audit':'qualify_native_character_textures.py'}
    reports={};evidence={'build':build/'build_verification.json'}
    for name,tool in tools.items():
        path=getattr(a,name)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['auditor_sha256' if name=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[name]=v;evidence[name]=path
    assert [reports[n]['checks'] for n in ('check','pixels','vq','regression','motion')]==[8199,3049,18443,530,918]
    assert reports['pixels']['actual_GPU_images']==418 and reports['pixels']['pixels_compared']==711168 and reports['vq']['new_native_cases']==6144 and reports['vq']['selected_prior_regression_checks']==5860
    authored=[p/'native'/n for n in ('jp_native_character_textures.h','jp_native_character_textures.inc','jp_native_texels_vq1.c')]+[p/'jp_port_tools'/n for n in ('prepare_native_character_textures.py','build_native_character_textures.py','verify_native_character_textures.py','verify_native_character_texture_pixels.py','verify_native_vq1_mip.py','qualify_native_character_textures.py')]+[p/'JP_NATIVE_CHARACTER_TEXTURES.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    target=p/'analysis/jp_native_character_textures_package.json';assert not target.exists();d.mkdir();proof={}
    for name,path in evidence.items():
        to=p/'analysis'/('jp_native_character_textures_'+name+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    new=sum(reports[n]['checks'] for n in ('check','pixels','vq','parent'));regression=reports['regression']['checks']+reports['motion']['checks']+reports['vq']['selected_prior_regression_checks'];total=read(previous)['combined_qualified_scoped_checks']+new
    out=dict(status='all_eight_genuine_character_texture_definitions_original_reference_loops_actual_retail_GPU_pixels_recovery_and_VQ1_mip_scoped_qualification',new_checks=new,selected_regression_checks=regression,combined_qualified_scoped_checks=total,
        original_character_IDs=8,original_texture_lists=17,original_texture_entries=418,actual_GPU_images=418,pixels_compared=711168,actual_native_window_presents=1,
        retained_original_character_function_bodies=3,original_writable_native_storage_adaptation='e102hit_textures const qualifier removed only; actual source loader writes texture bindings',
        primary_backed_VQ1_native_cases=6144,actual_source_end_guard_page_verified=True,original_first_loaded_globalIndex_aliases_verified=True,
        prior_qualifications_preserved={'motion_exit':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},proof_files_sha256=proof,
        build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],private_GPU_DLL_sha256=b['GPU_DLL_sha256'],
        character_texture_System_Exit_authority_automatic=False,task_render_texture_view_acquisition_automatic=False,full_set_character_3D_character_animation_GAME_MODE_BOOT_or_gameplay_verified=False,
        pixel_scope='Opaque point-filtered native-size largest-level comparisons; full mip chains upload, source-backed VQ1 tests; no console capture equivalence',
        original_source_and_proofs_preserved=True,retail_data_or_screenshots_or_combined_game_SDK_DLLs_publishable=False,public_research_build_payloads_changed=False,
        playable=False,overall_completion_percent=10,goal_active=True)
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new checks;',regression,'regressions;',total,'overlapping scoped checks',flush=True)
if __name__=='__main__':main()
