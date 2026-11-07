'''Exercise original keyed Simple action with real Sonic assets and GPU presentation.'''
import argparse, ctypes as C, hashlib, inspect, json, os, subprocess, sys
from pathlib import Path
import verify_native_character_tree as tree_suite
import verify_native_simple_sdk_frames as simple_frames


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def read(path):
    return json.loads(Path(path).read_bytes())


def derive():
    raw=simple_frames.derive()

    def swap(old,new):
        nonlocal raw
        assert raw.count(old)==1,old
        raw=raw.replace(old,new)

    anchor="    def extent(pointer,element):"
    swap(anchor,'''    aura_view=View()
    check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(0,b'sonic_aura_object',C.byref(aura_view))==0,
          'original aura object retained from the Sonic motion bank')
    bank_entry=bind(motions,'jp_motion_s_mot_entry',I,U,P)
    for index in range(bind(motions,'jp_motion_s_mot_count',U)()):
        entry=Entry()
        assert bank_entry(index,C.byref(entry))==0
        entries.append(entry)
'''+anchor)
    anchor="    class ModelSpan(C.Structure):"
    swap(anchor,'''    models.clear();nodes.clear();seen.clear()
    visit(aura_view.data)
    check(len(nodes)==73 and len(models)==20,'real aura tree uses 73 nodes and 20 meshes')
    check(all(model.material_count==0 and extent(C.cast(model.materials,P).value,20)==1
              for _,model,_ in models),'all aura models use one proven shared material despite zero declared count')
'''+anchor)
    swap("        bind(f,'njSimpleAction',None,P,F)(action_view.data,8.5)",
         '''        control_flags=U.in_dll(f,'_nj_control_3d_flag_')
        saved_flags=control_flags.value
        control_flags.value|=0x20
        try:
            bind(f,'jp_sonic_aura_probe',None,P,P,F,I)(aura_view.data,action_view.data,8.5,0)
        finally:
            control_flags.value=saved_flags''')
    swap('off_rgba!=rgba and sum(', 'off_rgba==rgba and report.texture_uploads==0 and sum(')
    swap("check(decoded.triangle_count>100 and decoded.vertex_count>100,",
         "check(decoded.batch_count==3*len(models) and decoded.triangle_count>100 and decoded.vertex_count>100,")
    swap("mode) for shot,mode in [(path,'original_textures'),(off_path,'textures_off_vertex_material_colours')]", 
         "mode) for shot,mode in [(path,'original_untextured_aura'),(off_path,'texture_sampling_disabled')]")
    swap("and bind(motions,'jp_motions_release',I,Q)(action_view.token)==0",
         "and bind(motions,'jp_motions_release',I,Q)(aura_view.token)==0 and bind(motions,'jp_motions_release',I,Q)(action_view.token)==0")
    return raw.replace('sonic_simple_action','sonic_original_aura').replace(
        'original_Simple_action_keyed_Sonic_model_and_retained_GPU_presentation_verified',
        'original_Sonic_aura_callback_and_retained_GPU_presentation_verified')


def exercise(c,inputs,track):
    raw=derive()
    path=c['d']/'private_simple_action_caller.py'
    path.write_text(raw)
    namespace=dict(vars(tree_suite),__file__=__file__)
    exec(compile(raw,str(path),'exec'),namespace)
    namespace['exercise'](c,inputs,track)
    result=read(c['d']/'result.json')
    result.update(derived_caller_sha256=sha(path),original_aura_action_frame=8.5,
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
    caller=d/'private_actual_parent_caller.py';caller.write_bytes(prefix+('        from verify_native_sonic_aura_frames import exercise\n        exercise(locals(), '+repr(str(a.inputs.resolve()))+', '+repr(str(a.track.resolve()))+")\nif __name__=='__main__':main()\n").encode());cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal']
    run=subprocess.run(cmd,capture_output=True,timeout=240,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr);assert not run.returncode,(run.returncode,run.stderr.decode(errors='replace')[-7000:])
    out=read(d/'run/result.json');out.update(original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller));(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True)
if __name__=='__main__':main()
