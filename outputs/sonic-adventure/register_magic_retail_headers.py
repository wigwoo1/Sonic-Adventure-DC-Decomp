import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/magic_retail_headers_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==654
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
 if routine['name'] in {'sa_sound_magic_header_metadata','sa_sound_magic_extension_word','sa_sound_magic_stream_parameters','sa_signed_division'}:
  extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
path.write_text(json.dumps(registry,indent=2)+'\n');path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['magic_retail_headers']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nOriginal magic8000 header verification: verify_magic_retail_headers.py reproduces654 full retail comparisons across109 original files,banks,metadata65ae44/extension65af64/parameters65afca. Native signed division,no modeled calls. Uses original64-byte header prefix and actual file size; independent BE field parsing,validation/partial output,offset/quotient,guards/source-header preservation. Header hashes recorded; full payload not loaded/decoded. Initializer/file configuration/physical playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& $python (Join-Path $PSScriptRoot 'verify_magic_retail_headers.py')\nif ($LASTEXITCODE -ne 0) { throw 'Original magic headers verification failed' }\n")
