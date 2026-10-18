"""Register overlay asset integration and one completely recovered selector."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
NAMES=('overlay_model_assets_inventory','overlay_model_inventory_verification','overlay_model_assets_verification','overlay_face_model_selector_verification')

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 paths=[PROJECT/f'analysis/{name}.json' for name in NAMES];inventory,parser,native,selector=[json.loads(p.read_text()) for p in paths]
 assert (inventory['modules_scanned'],inventory['modules_with_model_candidates'],inventory['validated_model_candidates'],inventory['segment_records'])==(42,39,15984,45425)
 assert inventory['node_backed_model_candidates']==15975 and len(inventory['face_nonalternate_strip_candidates'])==2
 assert (parser['independent_header_checks'],parser['raw_object_pointer_checks'],parser['ram_alias_checks'])==(15984,22401,126) and parser['inventory_reproduces_byte_for_byte']
 for k,v in dict(cases_passed=392,selected_models=46,modules_exercised=39,independent_projection_cases=184,original_face_strip_read_cases=24,original_face_strip_reads=24,boundary_comparisons=5084,numerical_operations=94656,external_service_boundaries=0,queue_flushes=4600,queue_events=49452,near_invocations=496,newly_recovered_entries=0).items():assert native[k]==v,k
 assert all(c['status']==0 and c['full_state_match'] and c['queue_match'] and c['original_asset_spans_preserved'] for c in native['cases'])
 assert selector['cases_passed']==2176 and selector['instruction_rows_covered']==88 and selector['conditional_edges_covered']==10 and selector['external_call_sites']==0
 assert len(selector['cases'])==2176 and all(c['full_state_match'] and c['independent_model_scale_checks'] for c in selector['cases'])
 for report in (inventory,parser,native,selector):
  for path,h in report['sha256'].items():assert digest(path)==h,path
 assert digest(ROOT/'work/sonic-adventure/build/overlay_model_assets.dll')==native['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_face_model_selector.dll')==selector['dll_sha256']
 assert digest(PROJECT/selector['source'])==selector['source_sha256']
 integration=dict(inventory='analysis/'+NAMES[0]+'.json',inventory_verification='analysis/'+NAMES[1]+'.json',verification='analysis/'+NAMES[2]+'.json',selector_verification='analysis/'+NAMES[3]+'.json',report_sha256={str(path.relative_to(PROJECT)):digest(path) for path in paths},mapped_modules=42,model_candidates=15984,segment_records=45425,native_cases=392,native_modules=39,original_face_strip_read_cases=24,newly_recovered_entries=1,new_instruction_rows=88,scope=inventory['scope']+' '+native['scope']+' '+selector['scope'],limitations=inventory['limitations']+native['limitations']+selector['limitations'],next_task='Trace the AL_MAIN caller/installation path to selector 0c905548 and connect selected object rendering, retaining exact recovered model/lighting behavior; continue runtime material-override tracing.')
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
 address=int(selector['address'],16);end=int(selector['end_exclusive'],16);data=(ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes();load=int(selector['load_address'],16)
 assert hashlib.sha256(data[address-load:end-load]).hexdigest()==selector['retail_span_sha256']
 entry=dict(module='AL_MAIN.BIN',load_address=selector['load_address'],name='sa_overlay_face_model_selector_0c905548',address=selector['address'],end_exclusive=selector['end_exclusive'],indexed_instruction_bytes=176,retail_span_sha256=selector['retail_span_sha256'],source=selector['source'],verification='analysis/overlay_face_model_selector_verification.json',native_build_script='build_overlay_face_model_selector.ps1',verifier_script='verify_overlay_face_model_selector.py',instruction_inventory='analysis/overlay-face-model-selector/0c905548_instructions.tsv',descriptive_name_is_inferred=True,status='Complete 88-row selector; 2176 original full-state comparisons and all 10 conditional outcomes, including original scale/FMOV-width effects and model priority.',comparison_level=selector['scope'],limitations=selector['limitations'])
 old=next((r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==address),None)
 added=int(old is None)
 if old is None:registry['routines'].append(entry);old=entry
 else:old.update(entry)
 old.setdefault('integration_evidence',{})['native_overlay_model_assets']=integration
 targets={int(t,16) for c in native['cases'] for t in c['parent_targets']}|{0x8c6209a8,0x8c62159a,0x8c611444,0x8c6069a4,0x8c606a88}
 for target in targets:
  routine=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==target)
  routine.setdefault('integration_evidence',{})['native_overlay_model_assets']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_overlay_model_assets']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} new selector; {len(registry["routines"])} routines total. Overlay model discovery itself receives zero routine credit.')
if __name__=='__main__':main()
