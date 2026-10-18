import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;name='sa_sound_stream_work_tick';report='analysis/sound_stream_work_tick_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==100
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c65bb4e',end_exclusive='0x8c65bb88',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0x64bb4e:0x64bb88]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_stream_work_tick']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='sound_stream_work_tick:'
if marker not in text:path.write_text(text+'\n'+marker+' build_sound_stream_work_tick.ps1/verify_sound_stream_work_tick.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_stream_work_tick.py' not in text:path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_sound_stream_work_tick.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_stream_work_tick.py')\nif ($LASTEXITCODE -ne 0) { throw 'Odd decode flag verification failed' }\n")
