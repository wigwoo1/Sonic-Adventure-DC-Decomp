"""Freeze the real SDK helper link, pinned sources and scoped runtime evidence."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from package_native_sound_placement import prior
from build_native_read_queue import PIN,owner_function
from build_native_sdk_motion import convert_casts
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','check','init','frames','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_event_frames_package.json';seen=set();prior(r,previous,seen);assert len(seen)==41
    old=read(a.baseline/'build_verification.json');b=read(a.build/'build_verification.json')
    bh=sha(a.build/'build_verification.json');recipe=b['sdk_motion_recipe']
    assert b['sdk_motion_baseline_sha256']==sha(a.baseline/'build_verification.json')==read(previous)['build_verification_sha256']
    for meta in (old,b):
        for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h
        for k,v in meta.items():
            if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
                assert sha(v)==meta[k+'_sha256']
                if k!='frames_DLL':assert old[k+'_sha256']==b[k+'_sha256']
    for n,h in recipe['original_source_sha256'].items():
        raw=(a.upstream/n).read_bytes()
        assert hashlib.sha256(raw).hexdigest()==h
        assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(a.upstream),'show',PIN+':'+n])
    assert recipe['existing_material_setter_owner_reused'] and recipe['njMotionS_compiled_but_not_linked']
    assert recipe['remaining_Simple_missing_provider']=='njSimpleDrawModel'
    for n,h in b['sdk_motion_kept_objects_sha256'].items():assert sha(n)==h
    for n,h in recipe['new_objects_sha256'].items():assert sha(n)==h
    def linked(meta):
        return next(x['command'] for x in meta['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll')
    old_objects={n for n in linked(old) if n.endswith('.obj')}
    new_objects={n for n in linked(b) if n.endswith('.obj')}
    assert old_objects<=new_objects and new_objects-old_objects==set(recipe['new_objects_sha256'])
    assert str(a.build.resolve()/'njMotionS.obj') not in new_objects
    for row in b['commands'][:len(b['commands'])-len(old['commands'])]:
        assert row['exit_code']==row['diagnostic_bytes']==0
        if 'response_file' in row:
            assert sha(row['response_file'])==row['response_file_sha256']
            assert shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
    for tag,unit in recipe['units'].items():
        for n,h in unit['generated_source_sha256'].items():assert sha(a.build/n)==h
        assert sha(a.build/(tag+'.obj'))==unit['object_sha256']
        assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(a.build/(tag+'.ll')).read_text())
    # Reconstruct and re-run the actual typed arithmetic/cast transformations.
    raw=(a.upstream/'sdk/ninja/njMotSrc.c').read_bytes()
    raw=raw.replace(b'#ifdef GCC\n#include "ninja/njmotion_zam.h"\n#endif\n',b'')
    raw=raw.replace(owner_function(raw,'njInitShape'),b'')
    anchor=b'#include <sg_xpt.h>\n'
    raw=raw.replace(anchor,anchor+b'#define GCC 1\n#include "jp_native_sprite_numeric.h"\n#include "jp_native_sdk_motion_numeric.h"\n')
    assert raw==(a.build/'njMotSrc.c').read_bytes()
    def ast_run(filename):
        cmd=next(x['command'] for x in b['commands'] if '-ast-dump=json' in x['command'] and Path(x['command'][-1]).name==filename)
        x=subprocess.run(cmd,capture_output=True);assert x.returncode==0 and not x.stderr;return json.loads(x.stdout)
    unit=recipe['units']['njMotSrc']
    adapted,stats=rewrite(raw,ast_run('njMotSrc.c'),unit['functions'])
    assert stats==unit['scalar_arithmetic_adaptation'] and stats['total_operators_rewritten']==147
    assert adapted==(a.build/'njMotSrc_rz.c').read_bytes()
    converted,casts=convert_casts(adapted,ast_run('njMotSrc_rz.c'),unit['functions'])
    assert converted==(a.build/'njMotSrc_rz_casts.c').read_bytes() and casts==unit['integer_float_cast_adaptation']
    reports={name:read(getattr(a,name)/'verification.json') for name in ('check','init','frames')}
    for name,count,tool in (('check',577,'verify_native_sdk_motion.py'),('init',530,'verify_native_game_init.py'),('frames',439,'verify_native_event_queue_frames.py')):
        row=reports[name];assert row['checks']==count and row['build_verification_sha256']==bh and row['verifier_sha256']==sha(p/'jp_port_tools'/tool)
    v=reports['check'];assert v['original_complete_calls']==144 and not v['original_game_function_hooks_used']
    assert {x['function'] for x in v['records']}=={'linear_vector','cubic_vector','linear_angles','cubic_angles','linear_angles_short'}
    assert all(x['expected']==x['actual'] for x in v['records'])
    f=reports['frames'];assert f['actual_window_presents']==10 and f['texture_Off_backend_bindings_and_uploads_zero']
    for row in f['screenshots']+f['rendered_original_action_frames']+[f['animated_preview']]:assert sha(row['path'])==row['sha256']
    d.mkdir();audit=dict(status='pinned_SDK_motion_source_RZ_casts_shared_owners_and_sampled_runtime_audited',
        auditor_sha256=sha(__file__),build_verification_sha256=bh,previous_packages_verified=41,
        new_checks=577,selected_regression_checks=969,Simple_renderer_or_aura_verified=False,playable=False)
    (d/'audit.json').write_text(json.dumps(audit,indent=2)+'\n')
    evidence={'build':a.build/'build_verification.json','check':a.check/'verification.json',
              'init':a.init/'verification.json','frames':a.frames/'verification.json','audit':d/'audit.json'}
    proof={}
    for n,path in evidence.items():
        to=p/'analysis'/('jp_native_sdk_motion_'+n+'.json');assert not to.exists()
        shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    files=['native/jp_native_sdk_motion_numeric.c','native/jp_native_sdk_motion_numeric.h',
           'jp_port_tools/build_native_sdk_motion.py','jp_port_tools/verify_native_sdk_motion.py',
           'jp_port_tools/qualify_native_sdk_motion.py','JP_NATIVE_SDK_MOTION.txt']
    for n in files:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    out=dict(status='original_Ninja_motion_helpers_sampled_interpolators_and_existing_render_regressions_qualified',
        new_checks=577,selected_regression_checks=969,
        combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+577,
        complete_original_interpolator_calls=144,RZ_arithmetic_operations=147,
        signed_integer_float_cast_adaptation=casts,original_motion_helper_source_functions=42,
        original_inline_FIPR_FLOAT_FTRC_value_bridges=True,
        original_game_function_hooks_used=False,existing_global_and_shape_material_providers_reused=True,
        Simple_motion_object_compiled_but_not_linked=True,remaining_Simple_renderer_dependency='njSimpleDrawModel',
        full_shape_motion_Simple_tree_aura_or_original_player_callback_verified=False,
        prior_qualifications_preserved={'event_frames':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in files},
        proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],
        public_research_build_payloads_changed=False,
        retail_generated_external_source_captures_or_combined_DLLs_publishable=False,
        original_source_and_proofs_preserved=True,playable=False,overall_completion_percent=10,goal_active=True)
    target=p/'analysis/jp_native_sdk_motion_package.json';assert not target.exists()
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target)
    print(out['status'],'577 new checks;969 regressions',flush=True)
if __name__=='__main__':main()
