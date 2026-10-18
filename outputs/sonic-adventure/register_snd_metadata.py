import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_snd_metadata.c'
s=source.read_text();start=s.index('/* Static C recovery');end=s.index('SA_STAGE_EXPORT',start)
s=s[:start]+'/* Retail SND metadata wrapper, 8c669e6a..8c669f2e.\n * Input size and returned 16-bit offset use signed acceptance checks.\n * Parser and quotient resolve to recovered native functions in verification. */\n'+s[end:];source.write_text(s)
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_snd_metadata_verification.json'
evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==1440
if not any(r['name']=='sa_sound_snd_metadata' for r in registry['routines']):
 registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_snd_metadata',address='0x8c669e6a',end_exclusive='0x8c669f2e',source='source/sa_sound_snd_metadata.c',retail_span_sha256=hashlib.sha256(binary[0x659e6a:0x659f2e]).hexdigest(),verification=report,status='Compiled and matched within connected report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_snd_metadata']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nSND metadata wrapper 8c669e6a: build_sound_snd_metadata.ps1 and verify_sound_snd_metadata.py reproduce 1440 retail comparisons. Native parser/quotient, no modeled calls. Independent signed size/offset checks, signed byte stride, output/format changes and input guards. Includes offset32768 rejection and offset65560 wrapping to positive24 after successful parsing. Constructed fixtures only; playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_snd_metadata.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_snd_metadata.py')\nif ($LASTEXITCODE -ne 0) { throw 'SND metadata verification failed' }\n")
