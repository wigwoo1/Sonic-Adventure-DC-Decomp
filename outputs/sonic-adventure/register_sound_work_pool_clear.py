import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;name='sa_sound_work_pool_clear';report='analysis/sound_work_pool_clear_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==24
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c65b5fc',end_exclusive='0x8c65b606',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0x64b5fc:0x64b606]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_work_pool_clear']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='sound_work_pool_clear:'
if marker not in text:path.write_text(text+'\n'+marker+' build_sound_work_pool_clear.ps1/verify_sound_work_pool_clear.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_work_pool_clear.py' not in text:path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_sound_work_pool_clear.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_work_pool_clear.py')\nif ($LASTEXITCODE -ne 0) { throw 'Sound work reset verification failed' }\n")
