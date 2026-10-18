import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;name='sa_sound_source_return';report='analysis/sound_source_return_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==384
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c65e644',end_exclusive='0x8c65e712',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0x64e644:0x64e712]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_source_return']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='sound_source_return:'
if marker not in text:path.write_text(text+'\n'+marker+' build_sound_source_return.ps1/verify_sound_source_return.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_source_return.py' not in text:path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_sound_source_return.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_source_return.py')\nif ($LASTEXITCODE -ne 0) { throw 'Sound source query verification failed' }\n")
