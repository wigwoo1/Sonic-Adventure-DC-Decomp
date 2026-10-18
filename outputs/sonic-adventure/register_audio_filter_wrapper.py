import json
from pathlib import Path
root=Path(__file__).resolve().parent
report='analysis/audio_filter_wrapper_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==54
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
names={'sa_audio_filter_coefficients','sa_math_result_error_wrapper'};found=set()
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['audio_filter_wrapper']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Connected audio coefficient/result wrapper:'
if marker not in text:
    path.write_text(text+'\n'+marker+' verify_audio_filter_wrapper.py reproduces54 full retail comparisons with native65adc0 and native10f90c connected. Independent angle/coefficient formula, exact rational square-root oracle, full RAM/CPU/FPU state, output guards, nested stack/PR restoration and unchanged prior error field for finite injected results. Only lower trigonometric10fcbc modeled. Exceptional modes, original-header configuration integration, physical audio/fullgame remain unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_audio_filter_wrapper.py' not in text:
    path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_audio_filter_wrapper.py')\nif ($LASTEXITCODE -ne 0) { throw 'Connected audio filter wrapper verification failed' }\n")
