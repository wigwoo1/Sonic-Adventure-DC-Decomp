import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;name='sa_math_trig_octant_dispatch';report='analysis/math_trig_octant_dispatch_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==96
path=root/f'source/{name}.c';text=path.read_text();a=text.index('/* Static C recovery');b=text.index('SA_STAGE_EXPORT',a)
path.write_text(text[:a]+'/* Retail finite trigonometric octant dispatcher10fcbc..10fd8c.\n * Absolute input, external reduction, odd-index adjustment, helper/sign selection.\n * Lower10fbb8/110218/110238 remain external; finite PR/SZ/RM0 scope. */\n'+text[b:])
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
if not any(r['name']==name for r in registry['routines']):
    registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c10fcbc',end_exclusive='0x8c10fd8c',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0xffcbc:0xffd8c]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['math_trig_octant_dispatch']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Trigonometric octant dispatch10fcbc:'
if marker not in text:
    path.write_text(text+'\n'+marker+' build_math_trig_octant_dispatch.ps1/verify_math_trig_octant_dispatch.py reproduce96 retail comparisons across104 instructions. Independent absolute-input forwarding,quarter-pi literal,odd adjustment,all8 selections/signs,index masking,nested stack/PR restoration. Both banks,zero and signed finite inputs. Lower reducer10fbb8/result helpers110218/110238 modeled. Exceptional inputs and complete trigonometric accuracy remain unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_math_trig_octant_dispatch.py' not in text:
    path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_math_trig_octant_dispatch.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_math_trig_octant_dispatch.py')\nif ($LASTEXITCODE -ne 0) { throw 'Math octant dispatch verification failed' }\n")
