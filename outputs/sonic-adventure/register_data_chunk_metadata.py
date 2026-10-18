import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_data_chunk_metadata.c'
s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
s=s[:a]+'/* Retail audio data-chunk metadata, 8c66a63c..8c66a714.\n * Scans aligned words for retail data marker; signed division computes frames.\n * Width4 normalizes to16 with type4 and unsigned frame calculation. */\n'+s[b:];source.write_text(s)
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_data_chunk_metadata_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==1728
if not any(r['name']=='sa_sound_data_chunk_metadata' for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_data_chunk_metadata',address='0x8c66a63c',end_exclusive='0x8c66a714',source='source/sa_sound_data_chunk_metadata.c',retail_span_sha256=hashlib.sha256(binary[0x65a63c:0x65a714]).hexdigest(),verification=report,status='Compiled and matched within connected report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_data_chunk_metadata']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nAudio data-chunk metadata66a63c: build_sound_data_chunk_metadata.ps1/verify_sound_data_chunk_metadata.py reproduce1728 retail comparisons with native signed/unsigned division and no modeled calls. Independent aligned marker search limit,missing/out-of-range marker rejection,offset,metadata,signed frame division/zero divisor,4-bit normalization/unsigned calculation and unknown-selector preservation. Constructed fixtures only; marker at last allowed word,actual assets,loader/playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_data_chunk_metadata.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_data_chunk_metadata.py')\nif ($LASTEXITCODE -ne 0) { throw 'Data-chunk metadata verification failed' }\n")
