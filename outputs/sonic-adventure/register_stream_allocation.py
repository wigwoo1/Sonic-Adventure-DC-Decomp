import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/stream_allocation_verification.json';e=json.loads((p/report).read_text());assert e['cases_passed']==36
b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for name,start,end in [('sa_sound_stream_create',0x8c6673b4,0x8c667440),('sa_sound_stream_destroy',0x8c667440,0x8c667464)]:
    if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(b[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope; destructor verified on constructor failure only'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['stream_allocation']=e;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';s=path.read_text();marker='Sound stream construction:'
if marker not in s:path.write_text(s+'\n'+marker+' build_sound_stream_create.ps1/build_sound_stream_destroy.ps1/verify_stream_allocation.py. '+e['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';s=path.read_text()
if 'verify_stream_allocation.py' not in s:path.write_text(s+"\n& (Join-Path $PSScriptRoot 'build_sound_stream_create.ps1')\n& (Join-Path $PSScriptRoot 'build_sound_stream_destroy.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_stream_allocation.py')\nif ($LASTEXITCODE -ne 0) { throw 'Stream allocation verification failed' }\n")
