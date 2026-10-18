"""Register three complete packet/default-scale helpers; partial clippers get no credit."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_primitive_stream_verification.json'
MANIFEST='analysis/draw_primitive_stream_translation.json'
CALLER='analysis/overlay_draw_primitive_stream_verification.json'
CONNECTION='analysis/draw_primitive_stream_connection.json'
PRIOR='analysis/overlay_draw_material_setup_verification.json'
ENTRIES={0x8c037990,0x8c037c3c,0x8c60b462}
COMPLETE=ENTRIES

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,manifest,caller,connection,prior=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION,PRIOR)]
 for report in (body,manifest,caller,connection,prior):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 cpu_path=PROJECT/'analysis/draw_primitive_cpu_frontier.json';kernel_path=PROJECT/'analysis/sh4_signed_product_verification.json'
 cpu,kernel=[json.loads(p.read_text()) for p in (cpu_path,kernel_path)]
 for report in (cpu,kernel):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert cpu['dmuls_instructions']==16 and cpu['sr_load_instructions']==8 and kernel['cases_passed']==2217 and not kernel['connected_to_game'] and kernel['newly_recovered_game_entries']==0
 assert body['routines']==manifest['routines'] and manifest['instruction_rows']==1030 and len(manifest['routines'])==6
 assert body['cases_passed']==189 and connection['compiled_source_count']==115
 assert body['rows_covered']==915 and body['conditional_edges_covered']==103
 assert body['independent_cases']==81 and body['independent_checks']==387
 assert set(body['registerable_entries'])=={hex(a) for a in COMPLETE}
 complete=[r for r in body['coverage_by_routine'] if int(r['address'],16) in COMPLETE]
 assert len(complete)==3 and sum(r['rows_covered'] for r in complete)==298 and sum(r['conditional_edges_covered'] for r in complete)==26
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in complete)
 assert all(r['status']!=-2 for r in body['cases'] if r['input']['entry'] in COMPLETE)
 assert caller['cases_passed']==210 and caller['prior_selected_cases_retested']==146 and caller['affected_prior_cases_retested']==112 and caller['additional_camera_profiles']==64
 assert caller['direct_service_cases']==64 and caller['label_event_profiles']==12
 assert caller['native_original_initializer_stages']==210 and caller['native_original_viewport_stages']==64
 assert caller['selected_prior_case_indices']==prior['selected_prior_case_indices']
 assert [r['input'] for r in caller['cases']]==[r['input'] for r in prior['cases']]
 assert not ({'0x8c037990','0x8c60b462'}&set(caller['first_unconnected_targets']))
 assert caller['target_invocations']['0x8c037990']>0 and caller['target_invocations']['0x8c60b462']>0
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0
 for report in (body,caller):
  assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] and c['gbr_final_and_ordered_entries_match'] for c in report['cases'])
 assert all(r['native_original_initializer_stage_match'] for r in caller['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_primitive_stream.dll')==body['dll_sha256']==caller['dll_sha256']
 # Removing only new declarations/routes and the public name reproduces the
 # previous bridge exactly. Earlier GBR/bus/native routines are preserved.
 source=(PROJECT/connection['source']).read_text()
 macro='#define BDECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*,const struct sa_vertex_bus*)\n'
 assert source.count(macro)==1 # The inherited BDECL definition is preserved.
 for r in manifest['routines']:
  decl=f'BDECL({r["name"]});\n';route=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n'
  assert source.count(decl)==source.count(route)==1;source=source.replace(decl,'').replace(route,'')
 source=source.replace('sa_draw_primitive_stream_connected','sa_draw_material_setup_connected')
 assert source==(PROJECT/'source/sa_draw_material_setup_connected.c').read_text()
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=3,new_entry_static_rows=298,new_entry_conditional_outcomes=26,body_comparisons=189,independent_cases=body['independent_cases'],independent_checks=body['independent_checks'],caller_comparisons=210,prior_selected_cases_retested=146,affected_prior_cases_retested=112,retained_camera_profiles=64,checked_renderer_initializer_preparation_stages=body['native_original_renderer_initializer_stages']+caller['native_original_initializer_stages'],checked_viewport_preparation_stages=64,caller_geometry_rows_reached=caller['new_geometry_rows_reached'],caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],first_unconnected_targets=caller['first_unconnected_targets'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],remaining_partial_geometry_proposals=4,remaining_partial_primitive_proposals=3,remaining_partial_motion_proposals=15,reused_routine_credit=0,external_game_service_fixtures=0,draw_candidate_recovery_credit=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover the next observed drawing boundary 8c609cb0 (48 cases). Extend and verify MACH and privileged integer-bank state before claiming the remaining clipping helpers complete. Preserve real viewport/GBR preparation and original model/motion records. Incomplete geometry/view/frame/boot and AL_MAIN parents remain unregistered.')
 integration.update(cpu_frontier='analysis/draw_primitive_cpu_frontier.json',cpu_frontier_sha256=digest(cpu_path),isolated_signed_product_verification='analysis/sh4_signed_product_verification.json',isolated_signed_product_verification_sha256=digest(kernel_path),isolated_signed_product_comparisons=2217,isolated_kernel_connected_to_game=False,isolated_kernel_recovery_credit=0)
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  if start not in ENTRIES:continue
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_primitive_stream.ps1',verifier_script='verify_draw_primitive_stream.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original packet/default-scale helper: all exported instructions/outcomes and reached real native dependencies checked within qualified numerical, RAM and software queue domains. Full rendering remains partial.',comparison_level=integration['scope'],limitations=integration['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_primitive_stream']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-ENTRIES;attached=0
 for address in sorted(reused):
  old=next((r for r in registry['routines'] if int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_primitive_stream']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_primitive_stream']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} primitive stream entries; {len(registry["routines"])} total. Linked {attached} existing entries with zero duplicate recovery credit. Three primitive and four geometry/view/frame/boot proposals remain partial.')
if __name__=='__main__':main()
