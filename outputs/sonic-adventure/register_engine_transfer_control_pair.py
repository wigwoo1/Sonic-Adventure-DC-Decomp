import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());status_path=p/'project_status.json';status=json.loads(status_path.read_text())
for stem,start,end in [('engine_transfer_control_restore', 2349919776, 2349919828), ('engine_transfer_control_install', 2349919900, 2349919952)]:
 report='analysis/engine_transfer_control_pair_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==128
 name='sa_'+stem
 if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
 status['verified_progress'][stem]=evidence
 handoff=p/'SOURCE_HANDOFF.txt';text=handoff.read_text();marker=stem+':'
 if marker not in text:handoff.write_text(text+'\n'+marker+f' build_{stem}.ps1/verify_engine_transfer_control_pair.py. '+evidence['scope']+' Overall project10%.\n')
 build=p/'build_and_verify.ps1';text=build.read_text()
 if f'build_{stem}.ps1' not in text:build.write_text(text+f"\n& (Join-Path $PSScriptRoot 'build_{stem}.ps1')\n")
text=build.read_text()
if 'verify_engine_transfer_control_pair.py' not in text:build.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_engine_transfer_control_pair.py')\nif ($LASTEXITCODE -ne 0) { throw 'Transfer control verification failed' }\n")

path.write_text(json.dumps(registry,indent=2)+'\n');status_path.write_text(json.dumps(status,indent=2)+'\n')
