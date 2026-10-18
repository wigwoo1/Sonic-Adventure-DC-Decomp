import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent;name='sa_sound_data_chunk_state_initialize';report='analysis/sound_data_chunk_state_initialize_verification.json'
source=p/f'source/{name}.c';s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a);source.write_text(s[:a]+'/* Retail audio data-chunk state initializer66a714..66a804.\n * Native metadata/division/fill helpers; initializes format1. */\n'+s[b:])
evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==1728
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c66a714',end_exclusive='0x8c66a804',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0x65a714:0x65a804]).hexdigest(),verification=report,status='Compiled and matched within connected report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_data_chunk_state_initialize']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nData-chunk audio state initializer66a714: build_sound_data_chunk_state_initialize.ps1/verify_sound_data_chunk_state_initialize.py reproduce1728 retail comparisons with native metadata/division/fill, no modeled calls. Independent256-byte state on success/rejection,normalization,signed extensions,resets,copied defaults,internal pointer,format1,selector and returned offset. Constructed fixtures only; actual assets/loader/playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_data_chunk_state_initialize.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_data_chunk_state_initialize.py')\nif ($LASTEXITCODE -ne 0) { throw 'Data-chunk state verification failed' }\n")
