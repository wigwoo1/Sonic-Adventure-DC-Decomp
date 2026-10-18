import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_packed_state_initialize.c'
s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
s=s[:a]+'/* Retail packed audio-state initializer, 8c66a440..8c66a524.\n * Native metadata66a378 and fill10d31c resolve through caller dispatch.\n * Initializes format2 and retains unknown encoding selectors. */\n'+s[b:];source.write_text(s)
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_packed_state_initialize_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==1260
if not any(r['name']=='sa_sound_packed_state_initialize' for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_packed_state_initialize',address='0x8c66a440',end_exclusive='0x8c66a524',source='source/sa_sound_packed_state_initialize.c',retail_span_sha256=hashlib.sha256(binary[0x65a440:0x65a524]).hexdigest(),verification=report,status='Compiled and matched within connected report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_packed_state_initialize']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nPacked audio-state initializer66a440: build_sound_packed_state_initialize.ps1/verify_sound_packed_state_initialize.py reproduce1260 retail comparisons with native metadata/fill and no modeled calls. Independent complete256-byte state across encoding/channel/offset/rate/signed length variants; resets,copied defaults,internal pointer,format2 and unknown-selector preservation. Provided size3 does not trigger validation; fixture has full64-byte data. Actual format identity/loader/playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_packed_state_initialize.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_packed_state_initialize.py')\nif ($LASTEXITCODE -ne 0) { throw 'Packed audio state verification failed' }\n")
