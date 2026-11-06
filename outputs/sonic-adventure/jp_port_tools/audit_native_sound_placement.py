"""Audit newly recovered callbacks without changing earlier qualifications."""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from audit_native_effects import all_symbols
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(a.baseline/'build_verification.json')==b['placement_baseline_sha256']
    original=(a.upstream/'src/sound/sound.c').read_bytes();current=(a.build/'external_game_sound.c').read_bytes();before=(a.baseline/'external_game_sound.c').read_bytes()
    assert current.startswith(before) and sha(a.build/'external_game_sound.c')==b['placement_source_sha256']
    for unit,h in b['placement_original_source_sha256'].items():assert sha(a.upstream/unit)==h,unit
    units={'src/sound/sound.c':['sound_fx_zone_step','sound_orbit_step'],'src/object/placement.c':['in_range','placement_try_leave_by','placement_try_leave'],'src/debug/editor.c':['placement_editing'],'src/draw/tree.c':['draw_nodes','draw_tree']}
    bodies={}
    for unit,names in units.items():
        for name in names:
            old=function((a.upstream/unit).read_bytes(),name);adapted=old
            assert hashlib.sha256(old).hexdigest()==b['placement_original_function_sha256'][name]
            if name=='sound_orbit_step':adapted=adapted.replace(b'(int)task',b'(intptr_t)task')
            if name=='draw_tree':adapted=adapted.replace(b'{\n',b'{\n    if(object==&marker_object&&jp_sound_marker_reserve())return;\n',1)
            assert function(current,name)==adapted,name
            bodies[name]=dict(original_sha256=hashlib.sha256(old).hexdigest(),native_sha256=hashlib.sha256(adapted).hexdigest())
    names=[n.decode() for n in re.findall(rb'^(?:static )?(?:void|int|Bool)\s+(\w+)\([^;{}]*\)\n\{',original,re.M)]
    assert len(names)==68
    preserved=[]
    for name in names:
        native='jp_game_sound_set_qsound' if name=='sound_set_qsound' else name
        if name not in ('sound_fx_zone_step','sound_orbit_step'):
            assert function(current,native)==function(before,native),name;preserved.append(name)
    assert len(preserved)==66
    for n,h in b['placement_matching_marker_includes_sha256'].items():assert sha(a.build/'matching_marker'/n)==h,n
    commands=b['commands'][:2];assert all(x['exit_code']==x['diagnostic_bytes']==0 for x in commands)
    link=commands[1]['command'];objects=[Path(n) for n in link if n.endswith('.obj')]
    for declaration in (b'Sint16 edit_mode = 0;',b'static Sint16 slot_count = 1;',b'static struct task *slot_tasks[OBJECT_SLOTS];'):
        assert current.count(declaration)==1 and declaration in (a.upstream/'src/debug/editor.c').read_bytes()
    # Only the original default editor slot is exposed in this scoped build.
    # Clang constant-folds slot_count and scalarizes slot_tasks to slot_tasks.0.
    shared=('sound_slots','sound_ports','qsound_owners','player_work','edit_mode','slot_tasks.0','marker_object')
    definitions={n:[] for n in shared}
    for obj in objects:
        for symbol in all_symbols(obj):
            if symbol['name'] in definitions and symbol['section']>0:definitions[symbol['name']].append(obj.name)
    assert all(len(v)==1 for v in definitions.values()),definitions
    unchanged={}
    for file in a.baseline.iterdir():
        if file.suffix.lower() not in ('.obj','.dll','.lib') or file.name in ('game_sdk.obj','jp_native_game_sound.dll','jp_native_game_sound.lib'):continue
        assert sha(file)==sha(a.build/file.name),file.name;unchanged[file.name]=sha(file)
    lib=C.CDLL(b['game_sound_DLL'])
    for name in ('sound_fx_zone_step','sound_orbit_step','placement_try_leave','placement_editing','draw_tree','njRotateXYZ','njRotateZXY','peer_sound_marker_status'):assert hasattr(lib,name),name
    rotation_sources={}
    for name in ('njRotateXYZ','njRotateZXY'):
        rel='sdk/ninja/'+name+'.c';path=a.upstream/rel
        pinned=subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(a.upstream),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+rel])
        assert path.read_bytes()==pinned;rotation_sources[rel]=sha(path)
    result=dict(status='all68_original_sound_functions_retained_and_unique_placement_dependencies_audited',original_sound_functions=68,unchanged_prior_sound_functions=preserved,new_original_bodies=bodies,unique_state_definitions=definitions,editor_scope='Original membership body and default one-slot storage; complete editor not integrated. Optimizer folds slot_count and scalarizes slot_tasks.',unchanged_baseline_binary_objects=unchanged,original_rotation_sources_sha256=rotation_sources,build_verification_sha256=sha(a.build/'build_verification.json'),auditor_sha256=sha(__file__),generic_tree_or_complete_editor_qualified=False,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(preserved),'unchanged prior bodies;',len(unchanged),'unchanged binaries/objects',flush=True)
if __name__=='__main__':main()
