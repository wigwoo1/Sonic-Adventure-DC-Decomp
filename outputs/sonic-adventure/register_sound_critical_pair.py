import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/sound_critical_pair_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==24
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for name,start,end in [('sa_sound_critical_enter',0x8c65ad68,0x8c65ad8e),('sa_sound_critical_exit',0x8c65ad8e,0x8c65adb4)]:
 if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_critical_pair']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Nested audio critical sections:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_sound_critical_pair.py. '+evidence['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_critical_pair.py' not in text:path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_sound_critical_enter.ps1')\n& (Join-Path $PSScriptRoot 'build_sound_critical_exit.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_critical_pair.py')\nif ($LASTEXITCODE -ne 0) { throw 'Nested critical section verification failed' }\n")
