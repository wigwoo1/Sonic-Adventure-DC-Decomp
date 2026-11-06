"""Audit retained original effects bodies, unique state and real update callers."""
import argparse,ctypes as C,hashlib,json,re,struct
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def all_symbols(path):
    # The older shared helper intentionally returns external symbols only.
    # This audit must also inspect static storage, COFF class 3.
    raw=path.read_bytes();machine,_,_,start,count,opt,_=struct.unpack_from('<HHIIIHH',raw)
    assert machine==0x8664 and opt==0;strings=start+18*count;i=0
    while i<count:
        name,_,section,_,storage,aux=struct.unpack_from('<8sIhHBB',raw,start+18*i)
        if storage in (2,3):
            if name[:4]==bytes(4):at=strings+struct.unpack_from('<I',name,4)[0];name=raw[at:raw.index(0,at)]
            else:name=name.rstrip(b'\0')
            yield dict(name=name.decode(),section=section,storage=storage)
        i+=1+aux
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['effects_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    base=Path(b['effects_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    before=(base/'external_game_sound.c').read_bytes();current=(a.build/'external_game_sound.c').read_bytes()
    original=(a.upstream/'src/sound/sound.c').read_bytes()
    names=[x.decode() for x in re.findall(rb'^(?:static )?(?:void|int|Bool)\s+(\w+)\([^;{}]*\)\n\{',original,re.M)]
    pending=set(b['effects_added_functions']);missing=[];bodies={};old_preserved=[]
    for name in names:
        if name in ('sound_fx_zone_step','sound_orbit_step'):missing.append(name);continue
        native='jp_game_sound_set_qsound' if name=='sound_set_qsound' else name
        body=function(current,native);old=function(original,name)
        bodies[name]=dict(original_sha256=hashlib.sha256(old).hexdigest(),native_sha256=hashlib.sha256(body).hexdigest(),newly_integrated=name in pending)
        if name not in pending:
            assert body==function(before,native),name;old_preserved.append(name)
        else:assert hashlib.sha256(body).hexdigest()==b['effects_adapted_function_sha256'][name],name
    assert (len(names),len(bodies),len(pending),len(old_preserved))==(68,66,51,15)
    assert b'WORK_AREA' not in b'\n'.join(function(current,n) for n in ('sound_load_file','sound_load_shot_bank','sound_load_sequence_bank','sound_load_program_bank'))
    owner=(a.build/'sound_owner_effects.c').read_bytes()
    assert b'jp_sound_dispatch_bind(frame_service,frame_effects,&sound,&sound.info.dispatch_token)' in owner
    assert b'if(jp_effects_file_close_owned())return JP_SOUND_CLEANUP;' in owner
    link=next(x for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['game_sound_DLL'])
    assert link['exit_code']==link['diagnostic_bytes']==0
    objects=[Path(x) for x in link['command'] if x.endswith('.obj')]
    shared=('sound_slots','sound_ports','qsound_owners','battle_rival','player_work','debug_page','stage_current','act_current','adxpd_internal_error')
    definitions={n:[] for n in shared}
    for obj in objects:
        for symbol in all_symbols(obj):
            if symbol['name'] in definitions and symbol['section']>0:definitions[symbol['name']].append(obj.name)
    assert all(len(v)==1 for v in definitions.values()),definitions
    lib=C.CDLL(b['game_sound_DLL'])
    for name in b['effects_exports']:assert hasattr(lib,name),name
    for name in ('jp_effects_queue','jp_effects_request','jp_effects_load_file','jp_effects_control','jp_effects_snapshot'):assert hasattr(lib,name),name
    unchanged={}
    for key in ('frames_DLL','present_DLL','System_DLL','GDFS_DLL'):
        assert sha(b[key])==b[key+'_sha256']==sha(base/Path(b[key]).name);unchanged[key]=b[key+'_sha256']
    preserved_objects={}
    for obj in objects:
        if obj.name in ('game_sdk.obj','sound_owner.obj','external_mw_sdcs.obj','external_mw_sdbuf.obj'):continue
        assert sha(obj)==sha(base/obj.name);preserved_objects[obj.name]=sha(obj)
    main=function((a.upstream/'src/main.c').read_bytes(),'main')
    frame=function((a.upstream/'src/system/frame.c').read_bytes(),'frame_wait_step')
    action=function((a.upstream/'src/stage/stage.c').read_bytes(),'stage_run_action')
    field=function((a.upstream/'src/stage/stage.c').read_bytes(),'stage_run_field')
    assert main.count(b'sound_send_queue();')==frame.count(b'sound_send_queue();')==1
    assert main.index(b'game_mode_step();')<main.index(b'sound_send_queue();')<main.index(b'frame_wait();')
    for body in (action,field):
        assert b'if (frame_step < frame_step_count + extra)' in body and body.count(b'frame_wait_step(')==1
    result=dict(status='original_effects_source_unique_shared_state_native_link_and_update_caller_scope_audited',
        original_module_functions=len(names),retained_original_functions=len(bodies),new_original_sound_functions=len(pending),unchanged_prior_original_sound_functions=old_preserved,
        still_unintegrated_placement_drawing_callbacks=missing,original_function_bodies=bodies,unique_actual_state_definitions=definitions,
        actual_linked_objects={obj.name:sha(obj) for obj in objects},unchanged_prior_objects=preserved_objects,unchanged_parent_DLLs=unchanged,
        update_cadence=dict(outer_loop='Original main calls the queue once after game_mode_step and before frame_wait.',intermediate_steps='Action/field loops call frame_wait_step between their logical steps; it calls the same queue.',
            consequence='For an ordinary frame containing N logical steps, N-1 intermediate calls plus one outer call give N queue updates. Additional rendered pictures and independent ADXT service are not queue clocks.',native_full_game_main_loop_executed=False),
        build_verification_sha256=sha(a.build/'build_verification.json'),auditor_sha256=sha(__file__),full_new_spatial_FLOAT_bitwise_SH4_equivalence_verified=False,
        full_gameplay_or_stage_start_verified=False,combined_game_SDK_binaries_publishable=False,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(bodies),'original functions;',len(pending),'new;',len(preserved_objects),'unchanged objects',flush=True)
if __name__=='__main__':main()
