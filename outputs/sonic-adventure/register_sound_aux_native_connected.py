import json
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());status_path=p/'project_status.json';status=json.loads(status_path.read_text())
for stem in ['sound_aux']:
 report='analysis/sound_aux_native_connected_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==392
 names={'sa_sound_subordinate_aux_set','sa_sound_subordinate_aux_get','sa_sound_error_report'};found=set()
 for r in registry['routines']:
  if r['name'] in names:
   found.add(r['name']);extra=r.setdefault('additional_verification',[])
   if report not in extra:extra.append(report)
 assert found==names
 status['verified_progress'][stem+'_native_connected']=evidence
 path2=p/'SOURCE_HANDOFF.txt';text=path2.read_text();marker=stem+' native diagnostics:'
 if marker not in text:path2.write_text(text+'\n'+marker+' verify_'+stem+'_native_connected.py. '+evidence['scope']+' Overall project10%.\n')
 path2=p/'build_and_verify.ps1';text=path2.read_text()
 if f'verify_{stem}_native_connected.py' not in text:path2.write_text(text+f"\n& $python (Join-Path $PSScriptRoot 'verify_{stem}_native_connected.py')\nif ($LASTEXITCODE -ne 0) {{ throw 'Native sound setter error verification failed' }}\n")
path.write_text(json.dumps(registry,indent=2)+'\n');status_path.write_text(json.dumps(status,indent=2)+'\n')
