import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();sp=p/'project_status.json';status=json.loads(sp.read_text())
for stem,start,end in [('sound_data_transfer16_step',0x8c66a804,0x8c66a8fa),('sound_data_transfer8_step',0x8c66a8fa,0x8c66a9fa)]:
 name=f'sa_{stem}';source=p/f'source/{name}.c';s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
 source.write_text(s[:a]+f'/* Retail data-chunk audio transfer {start:08x}..{end:08x}.\n * Native pending getter; query/submission callbacks supplied by caller.\n * 8-bit mono stores raw sample; stereo stores sample shifted left8. */\n'+s[b:])
 report=f'analysis/{stem}_native_pending_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==288
 if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=f'0x{start:08x}',end_exclusive=f'0x{end:08x}',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
 status['verified_progress'][stem]=evidence
 with (p/'build_and_verify.ps1').open('a') as f:f.write(f"\n& (Join-Path $PSScriptRoot 'build_{stem}.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_{stem}_native_pending.py')\nif ($LASTEXITCODE -ne 0) {{ throw 'Data-chunk transfer verification failed' }}\n")
path.write_text(json.dumps(registry,indent=2)+'\n');sp.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nData-chunk16-bit transfer66a804 and8-bit66a8fa: corresponding build_sound_data_transfer16_step.ps1/build_sound_data_transfer8_step.ps1 and native_pending verifiers reproduce288 cases each. Native pending getter; independent conversion/buffer guards/count/submission/phase checks,capacities0/2/4,mono/stereo,source preservation.16-bit samples copied little-endian;8-bit mono samples stored unshifted while stereo shifts left8,matching retail rather than normalizing. Query/submission callbacks modeled; initializer-to-loader integration/physical audio/fullgame unverified. Overall project remains 10%.\n')
