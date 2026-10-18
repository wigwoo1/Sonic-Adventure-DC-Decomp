import json
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
report='analysis/sound_driver_gate_get_verification.json';e=json.loads((p/report).read_text());assert e['cases_passed']==56
r=next(r for r in registry['routines'] if r['name']=='sa_sound_driver_gate_get');extra=r.setdefault('additional_verification',[])
if report not in extra:extra.append(report)
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_driver_gate_get_independent']=e;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'build_and_verify.ps1';s=path.read_text()
if 'verify_sound_driver_gate_get.py' not in s:path.write_text(s+"\n& $python (Join-Path $PSScriptRoot 'verify_sound_driver_gate_get.py')\nif ($LASTEXITCODE -ne 0) { throw 'Gate getter independent verification failed' }\n")
path=p/'SOURCE_HANDOFF.txt';s=path.read_text();marker='sound_driver_gate_get independent:'
if marker not in s:path.write_text(s+'\n'+marker+' verify_sound_driver_gate_get.py. '+e['scope']+' Overall project10%.\n')
