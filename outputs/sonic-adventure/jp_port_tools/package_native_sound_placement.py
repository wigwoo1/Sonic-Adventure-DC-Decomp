"""Freeze callback qualification without changing earlier source or proofs."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def prior(r,path,seen):
    key=path.resolve()
    if key in seen:return
    seen.add(key);value=read(path)
    for field in ('authored_source_sha256','proof_files_sha256'):
        for n,h in value.get(field,{}).items():assert sha(r/n)==h,n
    for row in value.get('prior_qualifications_preserved',{}).values():
        assert sha(r/row['path'])==row['sha256'];prior(r,r/row['path'],seen)
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','behavior','marker','data','audit','regression','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(a.build/'build_verification.json');bh=sha(a.build/'build_verification.json')
    evidence={'build':a.build/'build_verification.json'};reports={}
    for n,tool in (('behavior','verify_native_sound_placement.py'),('marker','verify_native_sound_marker.py'),('data','verify_native_sound_placement_data.py'),('audit','audit_native_sound_placement.py'),('regression','verify_native_effects.py')):
        path=getattr(a,n)/'verification.json';value=read(path);assert value['build_verification_sha256']==bh
        field='auditor_sha256' if n=='audit' else 'verifier_sha256';assert value[field]==sha(p/'jp_port_tools'/tool)
        evidence[n]=path;reports[n]=value
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert all(c['exit_code']==c['diagnostic_bytes']==0 for c in b['commands'][:2])
    assert [reports[n]['checks'] for n in ('behavior','marker','data','regression')]==[35,94,3343,39862]
    assert reports['audit']['original_sound_functions']==68 and len(reports['audit']['unchanged_prior_sound_functions'])==66
    assert reports['marker']['undersized_vertex_scratch_rejected'] and len(reports['marker']['cases'])==4
    assert reports['data']['original_calls']=={'njRotateXYZ':256,'njRotateZXY':256}
    regression=reports['regression'];assert len(regression['cases'])==11
    for left,right in (('queue_cpu','queue_dma'),('streams_cpu','streams_dma')):
        for field in ('PCM_sha256','RAM_sha256'):assert regression['cases'][left]['observations'][field]==regression['cases'][right]['observations'][field]
    old=p/'analysis/jp_native_effects_package.json';seen=set();prior(r,old,seen);assert read(old)['combined_qualified_scoped_checks']==936112
    tools=['build_native_sound_placement.py','verify_native_sound_placement.py','verify_native_sound_marker.py','verify_native_sound_placement_data.py','audit_native_sound_placement.py','package_native_sound_placement.py']
    authored=[p/'jp_port_tools'/n for n in tools]+[p/'JP_NATIVE_SOUND_PLACEMENT.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    proof={};d.mkdir()
    for n,source in evidence.items():
        target=p/'analysis'/('jp_native_sound_placement_'+n+'.json');assert not target.exists();shutil.copyfile(source,target);proof[target.relative_to(r).as_posix()]=sha(target)
    result=dict(status='original_sound_placement_callbacks_marker_rendering_and_sampled_composite_rotation_scoped_qualification',new_checks=3472,selected_regression_checks=39862,combined_qualified_scoped_checks=939584,retained_original_sound_module_functions=68,new_original_sound_callbacks=2,new_original_dependency_bodies=6,all_game_routines_or_every_sound_behavior_qualified=False,complete_gameplay_or_full_editor_qualified=False,build_verification_sha256=bh,private_sound_DLL_sha256=b['game_sound_DLL_sha256'],prior_qualifications_preserved={'effects':dict(path=old.relative_to(r).as_posix(),sha256=sha(old))},authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},proof_files_sha256=proof,original_source_and_previous_proofs_preserved=True,combined_game_SDK_binaries_publishable=False,retail_assets_or_captures_packaged=False,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    qualification=d/'qualification.json';qualification.write_text(json.dumps(result,indent=2)+'\n');target=p/'analysis/jp_native_sound_placement_package.json';assert not target.exists();shutil.copyfile(qualification,target)
    print(result['status'],3472,'new scoped checks;',39862,'regression checks;',939584,'combined qualified scopes',flush=True)
if __name__=='__main__':main()
