import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_packed_header_metadata.c'
s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
s=s[:a]+'/* Retail packed-header audio metadata, 8c66a378..8c66a440.\n * Final width16/stride2/type1 overrides branch-specific intermediate values.\n * Source format identity not established; no external calls. */\n'+s[b:];source.write_text(s)
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_packed_header_metadata_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==1260
if not any(r['name']=='sa_sound_packed_header_metadata' for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_packed_header_metadata',address='0x8c66a378',end_exclusive='0x8c66a440',source='source/sa_sound_packed_header_metadata.c',retail_span_sha256=hashlib.sha256(binary[0x65a378:0x65a440]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_packed_header_metadata']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nPacked audio metadata66a378: build_sound_packed_header_metadata.ps1/verify_sound_packed_header_metadata.py reproduce1260 retail comparisons with no external calls. Independent byte7*16 offset,wrapped channel count,LE16 rate,final width16/stride2/type1,signed length half towardzero and encoding selector/unknown preservation. Intermediate branch values are overwritten by retail normalization. Source format identity/header validation/loader/playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_packed_header_metadata.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_packed_header_metadata.py')\nif ($LASTEXITCODE -ne 0) { throw 'Packed audio metadata verification failed' }\n")
