import hashlib, json
from pathlib import Path
root=Path(__file__).resolve().parent
name='sa_audio_filter_coefficients'; report='analysis/audio_filter_coefficients_verification.json'
evidence=json.loads((root/report).read_text()); assert evidence['cases_passed']==54
path=root/f'source/{name}.c'; text=path.read_text()
a=text.index('/* Static C recovery'); b=text.index('SA_STAGE_EXPORT',a)
path.write_text(text[:a]+'/* Retail coefficient routine65adc0..65ae44.\n * Native binary32 arithmetic; trigonometric callee10f90c external.\n * PR/SZ/RM0 supported within verification scope. */\n'+text[b:])
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json'; registry=json.loads(path.read_text())
if not any(item['name']==name for item in registry['routines']):
    registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c65adc0',end_exclusive='0x8c65ae44',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0x64adc0:0x64ae44]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json'; status=json.loads(path.read_text());status['verified_progress']['audio_filter_coefficients']=evidence
path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt'; text=path.read_text(); marker='Audio filter coefficients65adc0:'
if marker not in text:
    path.write_text(text+'\n'+marker+' build_audio_filter_coefficients.ps1 and verify_audio_filter_coefficients.py reproduce54 full retail comparisons: signed extension0/+500/-500, rates22050/32000/44100, trig0/.5/1 and both FPU banks. Native66 instructions and numerical helpers; independent binary32 formula, exact rational FSQRT oracle, angle forwarding, guarded16-bit output and preserved state. Trigonometric callee10f90c modeled. PR/SZ/RM0 only; exceptional inputs, initializer integration, physical audio and fullgame unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1'; text=path.read_text()
if 'verify_audio_filter_coefficients.py' not in text:
    path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_audio_filter_coefficients.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_audio_filter_coefficients.py')\nif ($LASTEXITCODE -ne 0) { throw 'Audio coefficient verification failed' }\n")
