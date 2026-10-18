import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_aiff_transfer_dispatch.c'
s=source.read_text();start=s.index('/* Static C recovery');end=s.index('SA_STAGE_EXPORT',start)
s=s[:start]+'/* Retail AIFF transfer selector. Tail-calls the 8-bit routine for format 1,\n * otherwise the 16-bit routine. Host callback resolves these native routines. */\n'+s[end:];source.write_text(s)
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_aiff_transfer_dispatch_verification.json'
if not any(r['name']=='sa_sound_aiff_transfer_dispatch' for r in registry['routines']):
 registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_aiff_transfer_dispatch',address='0x8c669bfc',end_exclusive='0x8c669c0c',source='source/sa_sound_aiff_transfer_dispatch.c',retail_span_sha256=hashlib.sha256(binary[0x659bfc:0x659c0c]).hexdigest(),verification=report,status='Compiled and matched within connected report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_aiff_transfer_dispatch']=json.loads((p/report).read_text());path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nAIFF transfer dispatcher 8c669bfc: build_sound_aiff_transfer_dispatch.ps1 and verify_sound_aiff_transfer_dispatch.py reproduce 480 retail comparisons. Native 8/16-bit transfer and pending getter connected; format flags 0/1/2/ffff/8001, phase, channel, frame, pending and FPU-bank variants. Exact independently checked conversion, buffers and submission arguments. Query/submission callbacks modeled; physical audio and full game unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_aiff_transfer_dispatch.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_aiff_transfer_dispatch.py')\nif ($LASTEXITCODE -ne 0) { throw 'AIFF dispatch verification failed' }\n")
