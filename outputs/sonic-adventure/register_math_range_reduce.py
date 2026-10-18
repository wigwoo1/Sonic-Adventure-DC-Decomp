import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;name='sa_math_range_reduce';report='analysis/math_range_reduce_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==144
path=root/f'source/{name}.c';text=path.read_text();a=text.index('/* Static C recovery');b=text.index('SA_STAGE_EXPORT',a)
path.write_text(text[:a]+'/* Retail range reducer10fbb8..10fc9c.\n * Quotient index and signed binary32 remainder; native numerical helpers.\n * Infinity divisor equality supported; NaN and zero rejection are integer checks.\n * PR/SZ/RM0 verified within report scope; large quotient branch unverified. */\n'+text[b:])
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
if not any(r['name']==name for r in registry['routines']):
    registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c10fbb8',end_exclusive='0x8c10fc9c',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0xffbb8:0xffc9c]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['math_range_reduce']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native range reducer10fbb8:'
if marker not in text:
    path.write_text(text+'\n'+marker+' build_math_range_reduce.ps1/verify_math_range_reduce.py reproduce144 retail comparisons across114 instructions without modeled calls. Independent quotient index/signed rounded remainder,zero/nonfinite rejection,infinity divisor,output guards and full RAM/CPU/FPU/SR/PR/FPUL/MACL checks. Inputs through1e6,both banks,PR/SZ/RM0. Local equality helper explicitly accepts infinity. Quotient>=2^31 branch,denormals,other modes/traps,trigonometric integration/fullgame remain unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_math_range_reduce.py' not in text:
    path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_math_range_reduce.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_math_range_reduce.py')\nif ($LASTEXITCODE -ne 0) { throw 'Native range reducer verification failed' }\n")
