import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent
report='analysis/sound_linear_source_locks_verification.json';e=json.loads((p/report).read_text());assert e['cases_passed']==256
b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for kind,start,end in [('enter',0x8c65e08c,0x8c65e0a4),('exit',0x8c65e0a4,0x8c65e0bc)]:
    name='sa_sound_linear_source_lock_'+kind
    if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(b[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_linear_source_locks']=e;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';s=path.read_text();marker='Linear source interrupt mask helpers:'
if marker not in s:path.write_text(s+'\n'+marker+' build_sound_linear_source_lock_enter.ps1/build_sound_linear_source_lock_exit.ps1/verify_sound_linear_source_locks.py. '+e['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';s=path.read_text()
if 'verify_sound_linear_source_locks.py' not in s:path.write_text(s+"\n& (Join-Path $PSScriptRoot 'build_sound_linear_source_lock_enter.ps1')\n& (Join-Path $PSScriptRoot 'build_sound_linear_source_lock_exit.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_linear_source_locks.py')\nif ($LASTEXITCODE -ne 0) { throw 'Sound source interrupt mask verification failed' }\n")
