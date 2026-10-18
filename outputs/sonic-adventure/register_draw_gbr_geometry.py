"""Register seven complete original renderer/GBR/geometry helpers."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_gbr_geometry_verification.json'
RELOCATION='analysis/draw_gbr_relocation_verification.json'
MANIFEST='analysis/draw_gbr_geometry_translation.json'
CALLER='analysis/overlay_draw_gbr_geometry_verification.json'
CONNECTION='analysis/draw_gbr_geometry_connection.json'
PRIOR_CALLER='analysis/overlay_draw_motion_sampling_verification.json'
ENTRIES={0x8c036f60,0x8c03718c,0x8c60a3f8,0x8c64f31c,0x8c64f244,0x8c64f2dc,0x8c60b3e2}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,relocation,manifest,caller,connection,prior=[json.loads((PROJECT/p).read_text()) for p in (BODY,RELOCATION,MANIFEST,CALLER,CONNECTION,PRIOR_CALLER)]
 for report in (body,relocation,manifest,caller,connection,prior):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and manifest['instruction_rows']==428 and len(manifest['routines'])==7
 assert {int(r['address'],16) for r in manifest['routines']}==ENTRIES
 assert body['cases_passed']==716 and relocation['cases_passed']==26 and connection['compiled_source_count']==112
 assert body['rows_covered']==428 and body['conditional_edges_covered']==40 and not body['unconnected_targets'] and not relocation['unconnected_targets']
 assert body['independent_cases']==577 and relocation['independent_cases']==16
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in body['coverage_by_routine'])
 assert set(body['registerable_entries'])=={hex(a) for a in ENTRIES}
 assert caller['cases_passed']==210 and caller['prior_selected_cases_retested']==146 and caller['affected_prior_cases_retested']==48 and caller['additional_camera_profiles']==64
 assert caller['direct_service_cases']==64 and caller['label_event_profiles']==12 and caller['native_original_initializer_stages']==210
 assert caller['selected_prior_case_indices']==prior['selected_case_indices']
 assert [r['input'] for r in caller['cases'][:146]]==[r['input'] for r in prior['cases']]
 assert [r['status'] for r in caller['cases'][:146]]==[r['status'] for r in prior['cases']]
 assert all(r['input'].get('renderer_view_profile') for r in caller['cases'][146:])
 assert not ({'0x8c03718c','0x8c60a3f8'}&set(caller['first_unconnected_targets']))
 assert caller['target_invocations'][hex(0x8c03718c)]>0 and caller['target_invocations'][hex(0x8c60a3f8)]>0
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0
 for report in (body,relocation,caller):
  assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] and c['gbr_final_and_ordered_entries_match'] for c in report['cases'])
 assert all(r['native_original_initializer_stage_match'] for r in caller['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_gbr_geometry.dll')==body['dll_sha256']==relocation['dll_sha256']==caller['dll_sha256']
 # Removing only the new routes/declarations and explicit GBR plumbing must
 # reproduce the old bridge; old routine/dispatch behavior is not rewritten.
 source=(PROJECT/connection['source']).read_text()
 macro='#define GDECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*)\n'
 assert source.count(macro)==1;source=source.replace(macro,'')
 for r in manifest['routines']:
  decl=f'GDECL({r["name"]});\n';route=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr);\n'
  assert source.count(decl)==source.count(route)==1;source=source.replace(decl,'').replace(route,'')
 source=source.replace('const struct sa_vertex_bus *bus;uint32_t *gbr;};','const struct sa_vertex_bus *bus;};').replace('observer observe,const struct sa_vertex_bus *bus,uint32_t *gbr){','observer observe,const struct sa_vertex_bus *bus){').replace('struct context c={&c.stop,fpul,macl,entry,user,observe,bus,gbr};','struct context c={&c.stop,fpul,macl,entry,user,observe,bus};').replace('sa_draw_gbr_geometry_connected','sa_draw_motion_sampling_connected')
 assert source==(PROJECT/'source/sa_draw_motion_sampling_connected.c').read_text()
 preparation_stages=sum(r['input']['entry']!=0x8c036f60 for report in (body,relocation) for r in report['cases'])+caller['native_original_initializer_stages']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),relocation_verification=RELOCATION,relocation_verification_sha256=digest(PROJECT/RELOCATION),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=7,new_entry_static_rows=428,new_entry_conditional_outcomes=40,body_comparisons=742,independent_cases=body['independent_cases']+relocation['independent_cases'],independent_checks=body['independent_checks']+relocation['independent_checks'],caller_comparisons=210,prior_selected_cases_retested=146,affected_prior_cases_retested=48,additional_camera_profiles=64,checked_renderer_initializer_preparation_stages=preparation_stages,original_gbr=body['original_gbr_literal'],caller_gbr_rows_reached=caller['new_gbr_rows_reached'],caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],first_unconnected_targets=caller['first_unconnected_targets'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],remaining_partial_motion_proposals=15,reused_routine_credit=0,external_game_service_fixtures=0,draw_candidate_recovery_credit=0,scope=body['scope']+' '+relocation['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover geometry preparation 8c037294 and the next original sampling helper 8c60a61a. Keep real GBR/renderer initialization, explicit camera/input scope and original geometry/motion records. Do not credit incomplete hierarchy/mesh/blend bodies or AL_MAIN parent.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_gbr_geometry.ps1',verifier_script='verify_draw_gbr_geometry.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original renderer initialization/geometry/sampling helper with explicit GBR: all exported instructions/outcomes and reached real native dependencies checked. Qualified numerical/valid-RAM/input domains retained; full renderer remains partial.',comparison_level=integration['scope'],limitations=integration['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_gbr_geometry']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-ENTRIES;attached=0
 for address in sorted(reused):
  old=next((r for r in registry['routines'] if int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_gbr_geometry']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_gbr_geometry']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} explicit-GBR geometry entries; {len(registry["routines"])} total. Linked {attached} existing entries with zero duplicate credit; incomplete motion/renderer/parent bodies remain unregistered.')
if __name__=='__main__':main()
