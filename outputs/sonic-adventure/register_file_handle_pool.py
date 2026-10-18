import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/file_handle_pool_verification.json';e=json.loads((p/report).read_text());assert e['cases_passed']==18
b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for name,start,end in [('sa_file_decode_handle_allocate',0x8c644b6e,0x8c644be6),('sa_file_decode_handle_release',0x8c644bfe,0x8c644c12)]:
    if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(b[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['file_handle_pool']=e;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';s=path.read_text();marker='Decode handle pool:'
if marker not in s:path.write_text(s+'\n'+marker+' build_file_decode_handle_allocate.ps1/build_file_decode_handle_release.ps1/verify_file_handle_pool.py. '+e['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';s=path.read_text()
if 'verify_file_handle_pool.py' not in s:path.write_text(s+"\n& (Join-Path $PSScriptRoot 'build_file_decode_handle_allocate.ps1')\n& (Join-Path $PSScriptRoot 'build_file_decode_handle_release.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_file_handle_pool.py')\nif ($LASTEXITCODE -ne 0) { throw 'File handle pool verification failed' }\n")
