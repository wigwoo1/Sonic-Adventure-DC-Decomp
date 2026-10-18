import json
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());status_path=p/'project_status.json';status=json.loads(status_path.read_text())
for stem in ['sound_subordinate_field8_set']:
 report=f'analysis/{stem}_boundaries_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==2016
 names={'sa_'+stem,'sa_sound_error_report','sa_sound_linear_source_identity_get'};found=set()
 for r in registry['routines']:
  if r['name'] in names:
   found.add(r['name']);extra=r.setdefault('additional_verification',[])
   if report not in extra:extra.append(report)
 assert found==names
 status['verified_progress'][stem+'_boundaries']=evidence
 path2=p/'SOURCE_HANDOFF.txt';text=path2.read_text();marker=stem+' expanded boundaries:'
 if marker not in text:path2.write_text(text+'\n'+marker+' verify_sound_subordinate_field8_boundaries.py. '+evidence['scope']+' Overall project10%.\n')
 path2=p/'build_and_verify.ps1';text=path2.read_text()
 if 'verify_sound_subordinate_field8_boundaries.py' not in text:path2.write_text(text+f"\n& $python (Join-Path $PSScriptRoot 'verify_sound_subordinate_field8_boundaries.py')\nif ($LASTEXITCODE -ne 0) {{ throw 'Native sound setter error verification failed' }}\n")
path.write_text(json.dumps(registry,indent=2)+'\n');status_path.write_text(json.dumps(status,indent=2)+'\n')
