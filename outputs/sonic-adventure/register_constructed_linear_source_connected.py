import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/constructed_linear_source_connected_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==50 and evidence['native_operations_compared']==850
names={'sa_sound_linear_source_'+stem for stem in ['pool_initialize','create','destroy','lock_enter','lock_exit','reset','capacity','query','commit','return','contiguous_check']}
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=set()
for routine in registry['routines']:
 if routine['name'] in names:
  found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
assert found==names;path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['constructed_linear_source_connected']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Persistent native constructed linear-source sequences:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_constructed_linear_source_connected.py. '+evidence['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_constructed_linear_source_connected.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_constructed_linear_source_connected.py')\nif ($LASTEXITCODE -ne 0) { throw 'Linear source connected verification failed' }\n")
