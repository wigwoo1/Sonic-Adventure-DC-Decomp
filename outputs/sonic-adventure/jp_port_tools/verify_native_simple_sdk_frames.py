'''Exercise original keyed Simple action with real Sonic assets and GPU presentation.'''
import argparse, ctypes as C, hashlib, inspect, json, os, subprocess, sys
from pathlib import Path
import verify_native_character_tree as tree_suite


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def read(path):
    return json.loads(Path(path).read_bytes())


def derive():
    raw=inspect.getsource(tree_suite.exercise)
    anchor="    entries=[];get=bind(motions,'jp_motion_models_entry',I,U,P)"
    setup='''    from verify_native_action_poses import Action, Motion
    from verify_native_ninja_global import Control
    action_view=View()
    check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(0,b'sonic_updraft_action',C.byref(action_view))==0,
          'original keyed Sonic action retained')
    action=Action.from_address(action_view.data)
    motion=Motion.from_address(action.motion)
    check(action.object==view.data and motion.frames==33,'original keyed action targets the canonical tree')
'''
    assert raw.count(anchor)==1
    raw=raw.replace(anchor,setup+anchor)
    draw="    bind(f,'draw_object',None,P,P)(view.data,overrides)"
    replacement='''    class ModelSpan(C.Structure):
        _fields_=[('model',P),('points',S),('normals',S),('materials',S),('spans',P),('meshsets',S)]
    registrations=(ModelSpan*len(models))()
    for index,(address,model,spans) in enumerate(models):
        registrations[index]=ModelSpan(address,extent(C.cast(model.points,P).value,12),
            extent(C.cast(model.normals,P).value,12),extent(C.cast(model.materials,P).value,20),
            C.addressof(spans),extent(C.cast(model.meshes,P).value,48))
    class LightAttr(C.Structure):
        _fields_=[('source',I),('ambient',F),('diffuse',F),('specular',F),
                  ('near',F),('far',F),('callback',P),('inner',I),('outer',I),('argb',F*4)]
    class Light(C.Structure):
        _fields_=[('matrix',F*16),('position',F*3),('direction',F*3),
                  ('stat',I),('reserved',I),('calculated',F*50),('attr',LightAttr)]
    assert Light.attr.offset==296
    light=Light()
    light.matrix[:]=identity
    light.direction[:]=[0,-1,0]
    light.attr.ambient=.7
    light.attr.diffuse=.3
    control=Control.in_dll(f,'_nj_light_control_')
    prior_light=control.lights[0]
    control.lights[0]=C.addressof(light)
    check(bind(f,'jp_simple_calc_initialize',I,U,S)(2048,65600)==0,'bounded Simple projection owner ready')
    check(bind(f,'jp_simple_model_scope_begin',I,P,S)(registrations,len(models))==0,'canonical model extents registered for the original action')
    try:
        bind(f,'njSimpleAction',None,P,F)(action_view.data,8.5)
    finally:
        draw_status=bind(f,'jp_simple_model_scope_end',I)()
        control.lights[0]=prior_light
    check(draw_status==0,('original Simple action draws every visible registered model',draw_status))
    check(bind(f,'jp_simple_calc_shutdown',I)()==0,'Simple scratch retired after original action')
'''
    assert raw.count(draw)==1
    raw=raw.replace(draw,replacement)
    release="and bind(motions,'jp_motions_release',I,Q)(view.token)==0"
    assert raw.count(release)==1
    raw=raw.replace(release,"and bind(motions,'jp_motions_release',I,Q)(action_view.token)==0 "+release)
    raw=raw.replace('sonic_original_model_tree','sonic_simple_action')
    raw=raw.replace('genuine_original_Sonic_model_tree_real_JP_textures_palette_full_parent_retained_GPU_frame_and_actual_Window_verified',
                    'original_Simple_action_keyed_Sonic_model_and_retained_GPU_presentation_verified')
    return raw


def exercise(c,inputs,track):
    raw=derive()
    path=c['d']/'private_simple_action_caller.py'
    path.write_text(raw)
    namespace=dict(vars(tree_suite),__file__=__file__)
    exec(compile(raw,str(path),'exec'),namespace)
    namespace['exercise'](c,inputs,track)
    result=read(c['d']/'result.json')
    result.update(derived_caller_sha256=sha(path),original_Simple_action_frame=8.5,
                  registered_models_from_compiled_extents=True)
    (c['d']/'result.json').write_text(json.dumps(result,indent=2)+'\n')


def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(a.build/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    # Use one DLL directory so explicit loads and import resolution share owners.
    for key,value in tuple(b.items()):
        if key.endswith('_DLL') and isinstance(value,str) and value.endswith('.dll'):
            local=a.build.resolve()/Path(value).name
            assert sha(local)==b[key+'_sha256'],key
            b[key]=str(local)
    manifest=d/'coherent_build_manifest.json'
    manifest.write_text(json.dumps(b,indent=2)+'\n')
    original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1;prefix=raw.split(marker)[0]
    for old,new in [(b"('jp_textures_initialize',(32,1024,32<<20))",b"('jp_textures_initialize',(1024,4096,128<<20))"),(b"('jp_render_state_initialize',(128,1<<20))",b"('jp_render_state_initialize',(2048,16<<20))")]:assert prefix.count(old)==1;prefix=prefix.replace(old,new)
    anchor=b"b=read(a.build/'build_verification.json')"
    assert prefix.count(anchor)==1
    prefix=prefix.replace(anchor,('b=read('+repr(str(manifest))+')').encode())
    caller=d/'private_actual_parent_caller.py';caller.write_bytes(prefix+('        from verify_native_simple_sdk_frames import exercise\n        exercise(locals(), '+repr(str(a.inputs.resolve()))+', '+repr(str(a.track.resolve()))+")\nif __name__=='__main__':main()\n").encode());cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal']
    run=subprocess.run(cmd,capture_output=True,timeout=240,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr);assert not run.returncode,(run.returncode,run.stderr.decode(errors='replace')[-7000:])
    out=read(d/'run/result.json');out.update(original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller));(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True)
if __name__=='__main__':main()
