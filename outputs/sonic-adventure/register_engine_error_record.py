import hashlib,json
from pathlib import Path
p=Path(__file__).resolve().parent;binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());status_path=p/'project_status.json';status=json.loads(status_path.read_text())
for stem,start,end in [('engine_error_record',0x8c65acac,0x8c65acb6)]:
 report=f'analysis/{stem}_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==216
 name='sa_'+stem
 if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=hex(start),end_exclusive=hex(end),source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
 status['verified_progress'][stem]=evidence
 handoff=p/'SOURCE_HANDOFF.txt';text=handoff.read_text();marker=stem+':'
 if marker not in text:handoff.write_text(text+'\n'+marker+f' build_{stem}.ps1/verify_{stem}.py. '+evidence['scope']+' Overall project10%.\n')
 build=p/'build_and_verify.ps1';text=build.read_text()
 if f'verify_{stem}.py' not in text:build.write_text(text+f"\n& (Join-Path $PSScriptRoot 'build_{stem}.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_{stem}.py')\nif ($LASTEXITCODE -ne 0) {{ throw 'Source size getter verification failed' }}\n")
path.write_text(json.dumps(registry,indent=2)+'\n');status_path.write_text(json.dumps(status,indent=2)+'\n')
