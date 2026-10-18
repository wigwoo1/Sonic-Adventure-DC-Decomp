"""Register one discovered thunk and native deferred-deletion integration proof."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_context_callback_verification.json'
INVENTORY='analysis/main_dispatch_context_callback_translation.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
 report=json.loads((PROJECT/REPORT).read_text());inventory=json.loads((PROJECT/INVENTORY).read_text())
 assert report['counts']==dict(cancel=640,restore=640,gate=4,callback=32,schedule=3592)
 for k,v in dict(stage_comparisons=4908,direct_callback_cases=32,created_lifecycles=1284,created_tasks_executed=644,
                 constructed_scheduler_lifecycles=512,new_rows_covered=3,reused_rows_covered=36,
                 conditional_edges_covered=8,call_sites_covered=2,boundary_comparisons=10548,
                 external_game_service_calls=0).items():assert report[k]==v,k
 assert report['routines']==inventory['routines'] and report['existing_dependencies']==inventory['existing_dependencies']
 assert len(report['stages'])==4908 and all(s['full_state_match'] for s in report['stages'])
 for record in (report,inventory):
  for p,h in record['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_context_callback.dll')==report['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
 registry_path=PROJECT/'analysis/recovered_routines.json'
 registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in inventory['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],
             end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],
             retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,
             native_build_script='build_main_dispatch_context_callback.ps1',verifier_script='verify_main_dispatch_context_callback.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             status='Newly discovered tail thunk verified with native removal request, scheduling and delayed deletion; all original rows reached.',
             comparison_level=report['scope'],limitations=report['limitations'],boundary_evidence=inventory['boundary_evidence'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);added+=1
  else:old.update(entry)
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),inventory=INVENTORY,
                  inventory_sha256=digest(PROJECT/INVENTORY),stage_comparisons=4908,created_lifecycles=1284,
                  created_tasks_executed=644,constructed_scheduler_lifecycles=512,external_game_service_calls=0,
                  reused_native_sources=[{k:r[k] for k in ('address','source','source_sha256')} for r in inventory['existing_dependencies']],
                  interpretation='Callback 8c01b358 requests deferred removal by storing 8c09865c in task +10; the next scheduler pass deletes the task and releases its work.',
                  remaining_stored_callbacks=['0x8c044680','dynamic setter callback table values'],scope=report['scope'],limitations=report['limitations'])
 for address in (0x8C01AE60,0x8C01AAE0,0x8C098786,0x8C09878C,0x8C09865C,0x8C09852E,
                 0x8C0985EC,0x8C098A80,0x8C098B0C,0x8C098B42,0x8C0AFB10):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_context_callback_deferred_deletion']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_context_callback_deferred_deletion']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} discovered original callback; {len(registry["routines"])} entries. Existing scheduler/removal helpers credited as integration only.')


if __name__=='__main__':main()
