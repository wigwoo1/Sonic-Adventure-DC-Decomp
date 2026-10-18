import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/math_range_reduce_large_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==28
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=False
for routine in registry['routines']:
    if routine['name']=='sa_math_range_reduce':
        found=True;extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['math_range_reduce_large']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Large quotient reducer verification:'
if marker not in text:
    path.write_text(text+'\n'+marker+' verify_math_range_reduce_large.py adds28 full retail comparisons without modeled calls. Signed boundary inputs near2^31/2^32 and2^33,divisor1,both banks. Independent early saturating truncation when quotient exponent<2^32,strict >2^31 threshold,subtraction2^32,second signed truncation,uint32 storage/reconstruction and remainder. Preserves original saturation behavior rather than ideal mathematical remainder. PR/SZ/RM0; other inputs/modes,traps,caller integration/fullgame unverified. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_math_range_reduce_large.py' not in text:
    path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_math_range_reduce_large.py')\nif ($LASTEXITCODE -ne 0) { throw 'Large quotient range reduction verification failed' }\n")
