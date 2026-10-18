import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_packed_transfer_step.c'
s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
s=s[:a]+'/* Retail packed audio transfer, 8c66a524..8c66a61a.\n * Copies little-endian16 samples, separating stereo buffers.\n * Native pending getter; query/submission callbacks supplied by caller. */\n'+s[b:];source.write_text(s)
test=p/'verify_sound_packed_transfer_step_native_pending.py';test.write_text(test.read_text().replace('Native123-instruction','Native117-instruction'))
report='analysis/sound_packed_transfer_step_native_pending_verification.json';rp=p/report;evidence=json.loads(rp.read_text());assert evidence['cases_passed']==96;evidence['scope']=evidence['scope'].replace('Native123-instruction','Native117-instruction');rp.write_text(json.dumps(evidence,indent=2)+'\n')
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
if not any(r['name']=='sa_sound_packed_transfer_step' for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_packed_transfer_step',address='0x8c66a524',end_exclusive='0x8c66a61a',source='source/sa_sound_packed_transfer_step.c',retail_span_sha256=hashlib.sha256(binary[0x65a524:0x65a61a]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_packed_transfer_step']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nPacked audio transfer66a524: build_sound_packed_transfer_step.ps1/verify_sound_packed_transfer_step_native_pending.py reproduce96 retail comparisons with native file pending getter. Independent little-endian16 sample copy,mono/separated stereo buffers,frame clamp/cursor,source/destination guards,submission lengths and phases. Query/submission callbacks modeled; initializer-to-loader integration and physical playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_packed_transfer_step.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_packed_transfer_step_native_pending.py')\nif ($LASTEXITCODE -ne 0) { throw 'Packed audio transfer verification failed' }\n")
