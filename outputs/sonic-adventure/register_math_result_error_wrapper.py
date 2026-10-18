import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent
name='sa_math_result_error_wrapper';report='analysis/math_result_error_wrapper_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==72
path=root/f'source/{name}.c';text=path.read_text();a=text.index('/* Static C recovery');b=text.index('SA_STAGE_EXPORT',a)
path.write_text(text[:a]+'/* Retail result/error wrapper10f90c..10f946.\n * Calls10fcbc; preserves result bits; infinity1100,NaN1101 atglobal7ac108.\n * PR/SZ0 verification scope; lower calculation remains external. */\n'+text[b:])
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
if not any(r['name']==name for r in registry['routines']):
    registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c10f90c',end_exclusive='0x8c10f946',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0xff90c:0xff946]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['math_result_error_wrapper']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Math result-error wrapper10f90c:'
if marker not in text:
    path.write_text(text+'\n'+marker+' build_math_result_error_wrapper.ps1/verify_math_result_error_wrapper.py reproduce72 retail comparisons of29 instructions. Independent bit classification, exact returned bits, global-error preservation for finite results, infinity1100/NaN1101, neighboring guards, stack/PR preservation and lower argument trace. Lower10fcbc modeled; trigonometric numerics, audio coefficient integration and fullgame unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_math_result_error_wrapper.py' not in text:
    path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_math_result_error_wrapper.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_math_result_error_wrapper.py')\nif ($LASTEXITCODE -ne 0) { throw 'Math result wrapper verification failed' }\n")
