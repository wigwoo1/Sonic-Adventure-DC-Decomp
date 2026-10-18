import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/math_trig_polynomials_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==96
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for name,start,end in [('sa_math_cosine_polynomial',0x8c110218,0x8c110234),('sa_math_sine_polynomial',0x8c110238,0x8c110258)]:
    source=root/f'source/{name}.c';text=source.read_text();a=text.index('/* Static C recovery');b=text.index('SA_STAGE_EXPORT',a)
    source.write_text(text[:a]+f'/* Retail polynomial helper{start:08x}..{end:08x}.\n * Seven original coefficients evaluated with six fused multiply-add steps.\n * Native numerical helpers; finite PR/SZ0 and RM0/1 verification scope. */\n'+text[b:])
    if not any(r['name']==name for r in registry['routines']):
        registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['math_trig_polynomials']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native trigonometric polynomials110218/110238:'
if marker not in text:
    path.write_text(text+'\n'+marker+' build_math_trig_polynomials.ps1/verify_math_trig_polynomials.py reproduce96 retail comparisons for14/16 instruction helpers. Original seven-word coefficient tables and hashes, six native fused multiply-add steps, native multiplication, explicit Horner evaluation with exact dyadic oracle, signed zero/finite reduced arguments through+/-pi/4,both banks,RM0/1,full state/unchanged RAM. No modeled calls. PR/SZ0 only; range reduction, exceptional inputs, caller integration and fullgame unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_math_trig_polynomials.py' not in text:
    path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_math_trig_polynomials.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_math_trig_polynomials.py')\nif ($LASTEXITCODE -ne 0) { throw 'Trigonometric polynomial verification failed' }\n")
