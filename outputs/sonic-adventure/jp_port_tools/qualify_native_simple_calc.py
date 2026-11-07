"""Freeze genuine full projection, native ownership and whole-original evidence."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from package_native_sound_placement import prior
from build_native_read_queue import PIN,owner_function
from build_native_simple_calc import derive
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','check','loops','init','frames','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_simple_projection_package.json';seen=set();prior(r,previous,seen);assert len(seen)==43
    old=read(a.baseline/'build_verification.json');b=read(a.build/'build_verification.json')
    bh=sha(a.build/'build_verification.json');recipe=b['simple_calc_recipe']
    assert b['simple_calc_baseline_sha256']==sha(a.baseline/'build_verification.json')==read(previous)['build_verification_sha256']
    for meta in (old,b):
        for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h
        for k,v in meta.items():
            if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
                assert sha(v)==meta[k+'_sha256']
                if k!='frames_DLL':assert b[k+'_sha256']==old[k+'_sha256']
    raw=(a.upstream/'sdk/ninja/njModelS.c').read_bytes()
    assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(a.upstream),'show',PIN+':sdk/ninja/njModelS.c'])
    assert hashlib.sha256(raw).hexdigest()==recipe['original_source_sha256']
    for n,h in recipe['original_function_sha256'].items():assert hashlib.sha256(owner_function(raw,n)).hexdigest()==h
    original=a.build/'original_simple_calc.c';assert original.read_bytes()==derive(raw)
    command=b['commands'][0]['command'];x=subprocess.run(command,capture_output=True);assert not x.returncode and not x.stderr
    adapted,stats=rewrite(original.read_bytes(),json.loads(x.stdout),['_njSimpleCalcModel'])
    assert adapted==(a.build/'original_simple_calc_rz.c').read_bytes() and stats==recipe['scalar_arithmetic_adaptation']
    assert stats['total_operators_rewritten']==28
    for n,h in recipe['generated_source_sha256'].items():assert sha(a.build/n)==h
    for n,h in recipe['objects_sha256'].items():assert sha(n)==h
    for n,h in b['simple_calc_kept_objects_sha256'].items():assert sha(n)==h
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(a.build/'simple_calc_original.ll').read_text())
    def link(meta):return next(x['command'] for x in meta['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll')
    before={x for x in link(old) if x.endswith('.obj')};after={x for x in link(b) if x.endswith('.obj')}
    assert before<=after and after-before==set(recipe['objects_sha256'])
    for row in b['commands'][:5]:
        assert row['exit_code']==row['diagnostic_bytes']==0
        if 'response_file' in row:
            assert sha(row['response_file'])==row['response_file_sha256']
            assert shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
    reports={n:read(getattr(a,n)/'verification.json') for n in ('check','loops','init','frames')}
    for n,count,tool in (('check',3341,'verify_native_simple_calc.py'),('loops',1554,'verify_native_simple_projection.py'),
                        ('init',530,'verify_native_game_init.py'),('frames',439,'verify_native_event_queue_frames.py')):
        row=reports[n];assert row['checks']==count and row['build_verification_sha256']==bh and row['verifier_sha256']==sha(p/'jp_port_tools'/tool)
    v=reports['check'];assert v['helper_calls']==128 and v['model_calls']==512
    assert len(v['helper_records'])==128 and all(x['expected']==x['actual'] for x in v['helper_records'])
    assert {(x['bank'],x['matrix'],x['control'],x['normals'],x['scenario'],x['count']) for x in v['model_records']}=={
        (bank,m,c,n,s,k) for bank in (0,1) for m in (0,1) for c in (0,2,256,258) for n in (False,True)
        for s in ('mixed','near','far','left') for k in (0,1,4,16)}
    assert all(x['expected_result']==x['actual_result'] and x['expected_snapshot']==x['actual_snapshot'] and
               x['expected_sha256']==x['actual_sha256'] for x in v['model_records'])
    assert v['independent32_byte_scratch_and_borrowed_globals_restoration_verified'] and not v['original_game_function_hooks_used']
    f=reports['frames'];assert f['actual_window_presents']==10 and f['texture_Off_backend_bindings_and_uploads_zero']
    for row in f['screenshots']+f['rendered_original_action_frames']+[f['animated_preview']]:assert sha(row['path'])==row['sha256']
    d.mkdir();audit=dict(status='pinned_full_projection_RZ_repairs_owned_scratch_and_whole_original_matrix_light_model_calls_audited',
        auditor_sha256=sha(__file__),build_verification_sha256=bh,previous_packages_verified=43,
        new_checks=3341,selected_regression_checks=2523,original_game_source_or_proofs_modified=False,
        full_Simple_material_mesh_or_aura_rendering_verified=False,playable=False)
    (d/'audit.json').write_text(json.dumps(audit,indent=2)+'\n')
    evidence={'build':a.build/'build_verification.json','check':a.check/'verification.json',
        'loops':a.loops/'verification.json','init':a.init/'verification.json','frames':a.frames/'verification.json','audit':d/'audit.json'}
    proof={}
    for n,path in evidence.items():
        to=p/'analysis'/('jp_native_simple_calc_'+n+'.json');assert not to.exists()
        shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    files=['native/jp_native_simple_calc.c','native/jp_native_simple_calc.h',
        'jp_port_tools/build_native_simple_calc.py','jp_port_tools/verify_native_simple_calc.py',
        'jp_port_tools/qualify_native_simple_calc.py','JP_NATIVE_SIMPLE_CALC.txt']
    for n in files:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    out=dict(status='original_matrix_light_helpers_and_full_Simple_model_projection_scoped_qualification',
        new_checks=3341,selected_regression_checks=2523,
        combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+3341,
        whole_original_helper_calls=128,whole_original_model_projection_calls=512,
        genuine_full_projection_RZ_operators=28,independent_owned32_byte_scratch=True,
        borrowed16_byte_renderer_pointer_count_and_data_restored=True,
        full_width_pointer_and_uninitialized_work_repairs_explicit=True,
        original_game_function_hooks_used=False,
        full_Simple_material_mesh_aura_original_Sonic_callback_or_gameplay_verified=False,
        prior_qualifications_preserved={'simple_projection':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in files},
        proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],
        public_research_build_payloads_changed=False,
        retail_generated_external_source_captures_or_combined_DLLs_publishable=False,
        original_source_and_proofs_preserved=True,playable=False,overall_completion_percent=10,goal_active=True)
    target=p/'analysis/jp_native_simple_calc_package.json';assert not target.exists()
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target)
    print(out['status'],'3341 new checks;2523 regressions',flush=True)
if __name__=='__main__':main()
