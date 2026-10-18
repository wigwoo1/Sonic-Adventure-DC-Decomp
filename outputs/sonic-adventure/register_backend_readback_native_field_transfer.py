import json
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());status_path=p/'project_status.json';status=json.loads(status_path.read_text())
for stem in ['backend_readback']:
 report='analysis/backend_readback_native_field_transfer_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==112
 names={'sa_sound_backend_readback','sa_sound_descriptor_byte_query','sa_sound_byte_read','sa_device_word_read','sa_sound_backend_volume_get','sa_sound_volume_byte_read','sa_sound_backend_pan_get','sa_sound_backend_field_get','sa_sound_field_word_read'};found=set()
 for r in registry['routines']:
  if r['name'] in names:
   found.add(r['name']);extra=r.setdefault('additional_verification',[])
   if report not in extra:extra.append(report)
 assert found==names
 status['verified_progress'][stem+'_native_field_transfer']=evidence
 path2=p/'SOURCE_HANDOFF.txt';text=path2.read_text();marker=stem+' native diagnostics:'
 if marker not in text:path2.write_text(text+'\n'+marker+' verify_'+stem+'_native_field_transfer.py. '+evidence['scope']+' Overall project10%.\n')
 path2=p/'build_and_verify.ps1';text=path2.read_text()
 if f'verify_{stem}_native_field_transfer.py' not in text:path2.write_text(text+f"\n& $python (Join-Path $PSScriptRoot 'verify_{stem}_native_field_transfer.py')\nif ($LASTEXITCODE -ne 0) {{ throw 'Native sound setter error verification failed' }}\n")
path.write_text(json.dumps(registry,indent=2)+'\n');status_path.write_text(json.dumps(status,indent=2)+'\n')
