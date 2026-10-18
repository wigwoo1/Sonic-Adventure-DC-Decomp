import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();sp=p/'project_status.json';status=json.loads(sp.read_text())
for stem,start,end,report,count in [('sound_data_bytepair_transfer_step',0x8c66a9fa,0x8c66ab2a,'analysis/sound_data_bytepair_transfer_step_native_pending_verification.json',288),('sound_data_transfer_dispatch',0x8c66ab5e,0x8c66ab86,'analysis/sound_data_transfer_dispatch_verification.json',480)]:
 name=f'sa_{stem}';source=p/f'source/{name}.c';s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
 source.write_text(s[:a]+f'/* Retail data-chunk encoding path {start:08x}..{end:08x}.\n * Selector2 reconstructs stereo byte pairs(0,2)/(1,3);1 expands8-bit;0 copies16-bit.\n * Dispatcher skips unknown selectors. Native helpers connected by caller. */\n'+s[b:])
 evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==count
 if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=f'0x{start:08x}',end_exclusive=f'0x{end:08x}',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
 status['verified_progress'][stem]=evidence
 verify=f'verify_{stem}'+('_native_pending' if count==288 else '')+'.py'
 with (p/'build_and_verify.ps1').open('a') as f:f.write(f"\n& (Join-Path $PSScriptRoot 'build_{stem}.ps1')\n& $python (Join-Path $PSScriptRoot '{verify}')\nif ($LASTEXITCODE -ne 0) {{ throw 'Data encoding verification failed' }}\n")
path.write_text(json.dumps(registry,indent=2)+'\n');sp.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nData byte-pair transfer66a9fa and dispatcher66ab5e: build_sound_data_bytepair_transfer_step.ps1/native_pending verifier reproduce288 retail cases; build_sound_data_transfer_dispatch.ps1/verify_sound_data_transfer_dispatch.py reproduce480 connected cases. Stereo byte pairs(0,2)/(1,3),mono consecutiveLE16,other native16/8-bit paths and unknown-selector no-op independently checked. Frame limits,phase/submission arguments,buffers/source preserved. Pending getter native; query/submission callbacks modeled. Actual loader/physical playback/fullgame unverified. Overall project remains 10%.\n')
