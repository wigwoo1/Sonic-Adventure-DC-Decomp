import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_snd_state_initialize.c'
s=source.read_text();start=s.index('/* Static C recovery');end=s.index('SA_STAGE_EXPORT',start)
s=s[:start]+'/* Retail SND stream-state initializer, 8c669f2e..8c66a016.\n * Initializes format 4 and the parser-derived encoding selector.\n * Metadata, parser, quotient and fill resolve to recovered native functions. */\n'+s[end:];source.write_text(s)
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_snd_state_initialize_verification.json'
evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==1440
if not any(r['name']=='sa_sound_snd_state_initialize' for r in registry['routines']):
 registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_snd_state_initialize',address='0x8c669f2e',end_exclusive='0x8c66a016',source='source/sa_sound_snd_state_initialize.c',retail_span_sha256=hashlib.sha256(binary[0x659f2e:0x65a016]).hexdigest(),verification=report,status='Compiled and matched within connected report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_snd_state_initialize']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nSND stream-state initializer 8c669f2e: build_sound_snd_state_initialize.ps1 and verify_sound_snd_state_initialize.py reproduce 1440 retail comparisons. Native metadata/parser/quotient/fill, no modeled calls. Complete256-byte state independently checked across accepted/rejected header fixtures, including reset fields,copied defaults,internal pointer,signed channel/stride,format4 and encoding selector. Actual loader/playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_snd_state_initialize.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_snd_state_initialize.py')\nif ($LASTEXITCODE -ne 0) { throw 'SND state verification failed' }\n")
