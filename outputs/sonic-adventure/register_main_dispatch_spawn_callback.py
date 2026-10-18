"""Register the spawn callback and two helpers with native lifecycle evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_spawn_callback_verification.json'
LIFECYCLE='analysis/main_dispatch_spawn_callback_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_spawn_callback_translation.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text())
 manifest=json.loads((PROJECT/MANIFEST).read_text())
 assert report['cases_passed']==612 and report['counts']==dict(callback=256,transform=80,emit=276,spawn=0,schedule=0)
 for k,v in dict(rows_covered=205,conditional_edges_covered=16,call_sites_covered=11,boundary_comparisons=10512,
                 numerical_operations=470,partial_guard_cases=58,independent_invariant_cases=554,
                 children_checked=1622,external_game_service_calls=0).items():assert report[k]==v,k
 for k,v in dict(lifecycles_passed=368,stage_comparisons=1104,boundary_comparisons=5160,numerical_operations=128,
                 created_parents=200,created_particle_tasks=288,external_game_service_calls=0).items():assert life[k]==v,k
 assert report['routines']==manifest['routines']
 assert len(life['cases'])==368 and all(c['independent_invariants'] and len(c['stages'])==3 and all(s['full_state_match'] for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_spawn_callback.dll')==report['dll_sha256']==life['dll_sha256']
 vendor=PROJECT/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h'
 assert digest(vendor)=='cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],
             end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],
             retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,
             native_build_script='build_main_dispatch_spawn_callback.ps1',verifier_script='verify_main_dispatch_spawn_callback.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             status='All original rows, branches and call sites covered with native dependencies; reached numerical limits and connected lifecycle verified within documented scope.',
             comparison_level=report['scope'],limitations=report['limitations']+life['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);added+=1
  else:old.update(entry)
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,
                  lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),
                  body_comparisons=612,lifecycle_stage_comparisons=1104,created_parents=200,created_particle_tasks=288,
                  external_game_service_calls=0,native_dependencies=report['native_dependencies'],
                  numerical_vendor=dict(path='third_party/skmp-sh4-fpu/sh4_fpu_approx.h',sha256=digest(vendor)),
                  remaining_stored_callbacks=['0x8c04481a','dynamic setter table values'],scope=report['scope']+' '+life['scope'],
                  limitations=report['limitations']+life['limitations'])
 addresses={0x8C07B320,0x8C09852E,0x8C0985EC,0x8C09865C,0x8C098786,0x8C09878C,0x8C098A80,0x8C098B0C,0x8C098B42}
 addresses.update(int(d['address'],16) for d in report['native_dependencies'])
 for address in addresses:
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_spawn_callback_transform_and_emission']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_spawn_callback_transform_and_emission']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} spawn callback/transform/emission bodies; {len(registry["routines"])} entries. Particle callback 8c04481a remains unfinished.')


if __name__=='__main__':main()
