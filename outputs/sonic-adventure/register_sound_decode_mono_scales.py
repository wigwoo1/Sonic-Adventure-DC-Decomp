import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/sound_decode_mono_scales_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==180
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=False
for routine in registry['routines']:
    if routine['name']=='sa_sound_decode_block_mono':
        found=True;extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_decode_mono_scales']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Mono decoder scale boundaries:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_sound_decode_mono_scales.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_decode_mono_scales.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_sound_decode_mono_scales.py')\nif ($LASTEXITCODE -ne 0) { throw 'Mono decoder scales verification failed' }\n")
