import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/math_trig_native_polynomials_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==96
names={'sa_math_trig_octant_dispatch','sa_math_cosine_polynomial','sa_math_sine_polynomial'};found=set()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['math_trig_native_polynomials']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Connected native trig dispatcher/polynomials:'
if marker not in text:
    path.write_text(text+'\n'+marker+' verify_math_trig_native_polynomials.py reproduces96 full retail comparisons with native10fcbc/110218/110238 and native numerical helpers. Independent original-table Horner evaluation checks all8 selections/signs and index masking,full RAM/CPU/FPU/SR/PR/MACL and stack restoration. Only reducer10fbb8 modeled. Actual range reduction, exceptional inputs/modes,coefficient integration and fullgame remain unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_math_trig_native_polynomials.py' not in text:
    path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_math_trig_native_polynomials.py')\nif ($LASTEXITCODE -ne 0) { throw 'Connected trig polynomial verification failed' }\n")
