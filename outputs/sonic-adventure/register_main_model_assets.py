"""Register original model discovery and native stream integration with zero new routine credit."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
INVENTORY='analysis/main_model_assets_inventory.json'
PARSER='analysis/main_model_inventory_verification.json'
REPORT='analysis/main_model_assets_verification.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 inventory,parser,report=[json.loads((PROJECT/name).read_text()) for name in (INVENTORY,PARSER,REPORT)]
 assert inventory['validated_model_candidates']==860 and inventory['segment_records']==1905 and len(inventory['rejected_header_candidates'])==5
 assert inventory['object_node_candidates']==858 and inventory['node_backed_model_candidates']==854
 assert inventory['face_nonalternate_strip_candidates']==[] and len(inventory['known_particle_roots'])==2
 assert parser['independent_header_checks']==860 and parser['profile_literal_checks']==12 and parser['malformed_cases_rejected']==17 and parser['alternate_winding_flag_check'] and parser['inventory_reproduces_byte_for_byte']
 for k,v in dict(cases_passed=192,selected_models=24,independent_projection_cases=96,boundary_comparisons=5680,numerical_operations=222924,external_service_boundaries=0,queue_flushes=14280,queue_events=146708,near_invocations=976,newly_recovered_entries=0).items():assert report[k]==v,k
 assert len(report['cases'])==192 and all(c['status']==0 and c['full_state_match'] and c['queue_match'] and c['original_asset_spans_preserved'] for c in report['cases'])
 assert {c['input']['camera'] for c in report['cases']}=={'front','crossing'}
 assert {c['input']['mode'] for c in report['cases']}=={0,0x200000} and {c['input']['cursor_offset'] for c in report['cases']}=={0,32}
 for item in (inventory,parser,report):
  for path,h in item['sha256'].items():assert digest(Path(path))==h,path
 assert digest(ROOT/'work/sonic-adventure/build/main_model_assets.dll')==report['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN')==inventory['binary_sha256']==report['binary_sha256']=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 integration=dict(inventory=INVENTORY,inventory_sha256=digest(PROJECT/INVENTORY),inventory_verification=PARSER,inventory_verification_sha256=digest(PROJECT/PARSER),verification=REPORT,verification_sha256=digest(PROJECT/REPORT),model_candidates=860,node_backed_candidates=854,segment_records=1905,native_models=24,body_comparisons=192,newly_recovered_entries=0,new_instruction_rows=0,static_face_8000_combinations=0,scope=inventory['scope']+' '+parser['scope']+' '+report['scope'],limitations=inventory['limitations']+parser['limitations']+report['limitations'],next_task='Extend bounded model discovery to decoded overlays with established load addresses and investigate runtime material overrides; static absence in the main executable does not prove the face-strip branch unreachable.')
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));before=len(registry['routines'])
 parent_targets={int(t,16) for c in report['cases'] for t in c['parent_targets']}
 addresses=parent_targets|{0x8c6209a8,0x8c62159a,0x8c611444,0x8c6069a4,0x8c606a88}
 for address in sorted(addresses):
  routine=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  routine.setdefault('integration_evidence',{})['native_main_model_assets']=integration
 assert len(registry['routines'])==before
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_main_model_assets']=integration
 status['verified_progress']['recovered_buildable_routines']=before
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered main-model inventory and native evidence on {len(addresses)} existing routines; zero new routines, {before} total.')

if __name__=='__main__':main()
