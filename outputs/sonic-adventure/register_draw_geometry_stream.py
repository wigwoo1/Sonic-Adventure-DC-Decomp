"""Register four new helpers; preserve the already registered header helper."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_geometry_stream_verification.json'
MANIFEST='analysis/draw_geometry_stream_translation.json'
CALLER='analysis/overlay_draw_geometry_stream_verification.json'
CONNECTION='analysis/draw_geometry_stream_connection.json'
PRIOR='analysis/overlay_draw_gbr_geometry_verification.json'
ENTRIES={0x8c037294,0x8c60a61a,0x8c037350,0x8c604d68}
COMPLETE=ENTRIES|{0x8c036ffc}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,manifest,caller,connection,prior=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION,PRIOR)]
 for report in (body,manifest,caller,connection,prior):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and manifest['instruction_rows']==891 and len(manifest['routines'])==9
 assert body['cases_passed']==912 and connection['compiled_source_count']==113
 assert body['rows_covered']==497 and body['conditional_edges_covered']==35
 assert body['independent_cases']==627 and body['independent_checks']==3570
 assert set(body['registerable_entries'])=={hex(a) for a in COMPLETE}
 complete=[r for r in body['coverage_by_routine'] if int(r['address'],16) in COMPLETE]
 assert len(complete)==5 and sum(r['rows_covered'] for r in complete)==384 and sum(r['conditional_edges_covered'] for r in complete)==34
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in complete)
 assert all(r['status']!=-2 for r in body['cases'] if r['input']['entry'] in COMPLETE)
 assert caller['cases_passed']==210 and caller['prior_selected_cases_retested']==146 and caller['affected_prior_cases_retested']==48 and caller['additional_camera_profiles']==64
 assert caller['direct_service_cases']==64 and caller['label_event_profiles']==12
 assert caller['native_original_initializer_stages']==210 and caller['native_original_viewport_stages']==64
 assert caller['selected_prior_case_indices']==prior['selected_prior_case_indices']
 assert [r['input'] for r in caller['cases'][:146]]==[r['input'] for r in prior['cases'][:146]]
 camera_keys={'viewport_width','viewport_height','viewport_distance','viewport_reset_stage'}
 assert [{k:v for k,v in r['input'].items() if k not in camera_keys} for r in caller['cases'][146:]]==[r['input'] for r in prior['cases'][146:]]
 assert all(r['input']['viewport_width']==640 and r['input']['viewport_height']==480 and r['input']['viewport_distance']==320 and r['native_original_viewport_stage_match'] for r in caller['cases'][146:])
 assert not ({'0x8c037294','0x8c60a61a'}&set(caller['first_unconnected_targets']))
 assert caller['target_invocations']['0x8c037294']>0 and caller['target_invocations']['0x8c60a61a']>0
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0
 for report in (body,caller):
  assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] and c['gbr_final_and_ordered_entries_match'] for c in report['cases'])
 assert all(r['native_original_initializer_stage_match'] for r in caller['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_geometry_stream.dll')==body['dll_sha256']==caller['dll_sha256']
 # Removing only new declarations/routes and the public name reproduces the
 # previous bridge exactly. Earlier GBR/bus/native routines are preserved.
 source=(PROJECT/connection['source']).read_text()
 macro='#define BDECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*,const struct sa_vertex_bus*)\n'
 assert source.count(macro)==1;source=source.replace(macro,'')
 for r in manifest['routines']:
  decl=f'BDECL({r["name"]});\n';route=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n'
  assert source.count(decl)==source.count(route)==1;source=source.replace(decl,'').replace(route,'')
 source=source.replace('sa_draw_geometry_stream_connected','sa_draw_gbr_geometry_connected')
 assert source==(PROJECT/'source/sa_draw_gbr_geometry_connected.c').read_text()
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=4,new_entry_static_rows=379,new_entry_conditional_outcomes=34,complete_reused_header_entries=1,reused_header_static_rows=5,body_comparisons=912,independent_cases=body['independent_cases'],independent_checks=body['independent_checks'],caller_comparisons=210,prior_selected_cases_retested=146,affected_prior_cases_retested=48,updated_camera_profiles=64,checked_renderer_initializer_preparation_stages=body['native_original_renderer_initializer_stages']+caller['native_original_initializer_stages'],checked_viewport_preparation_stages=64,caller_geometry_rows_reached=caller['new_geometry_rows_reached'],caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],first_unconnected_targets=caller['first_unconnected_targets'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],remaining_partial_geometry_proposals=4,remaining_partial_motion_proposals=15,reused_routine_credit=0,external_game_service_fixtures=0,draw_candidate_recovery_credit=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover material flags at 8c037618 and its primitive dependencies, plus the observed next scale sampler at 8c60b462. Preserve real viewport/GBR preparation and original model/motion records. Incomplete geometry/view/frame/boot and AL_MAIN parents remain unregistered.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 header=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==0x8c036ffc)
 assert header['source']=='source/sa_global_word_flag_copy_8c036ffc.c' and header['end_exclusive']=='0x8c037006'
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  if start not in ENTRIES:continue
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_geometry_stream.ps1',verifier_script='verify_draw_geometry_stream.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original projection/lighting/angle/header/viewport helper: all exported instructions/outcomes and reached real native dependencies checked within qualified numerical, RAM and software queue domains. Full rendering remains partial.',comparison_level=integration['scope'],limitations=integration['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_geometry_stream']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-ENTRIES;attached=0
 for address in sorted(reused):
  old=next((r for r in registry['routines'] if int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_geometry_stream']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_geometry_stream']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} geometry stream entries; {len(registry["routines"])} total. Linked {attached} existing entries including the preserved header with zero duplicate credit. Four geometry/view/frame/boot proposals remain partial.')
if __name__=='__main__':main()
