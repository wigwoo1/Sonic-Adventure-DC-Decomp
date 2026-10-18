import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;name='sa_sound_decode_block_stereo';report='analysis/sound_decode_stereo_samples_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==1152
path=root/f'source/{name}.c';text=path.read_text();a=text.index('/* UNVERIFIED');b=text.index('SA_STAGE_EXPORT',a)
path.write_text(text[:a]+'/* Retail stereo block decoder65b238..65b428.\n * 32 samples per18-byte block; signed packed nibbles and predictor history.\n * Verified recurrence,scale4,three coefficient pairs; other scales unverified. */\n'+text[b:])
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address='0x8c65b238',end_exclusive='0x8c65b428',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[0x64b238:0x64b428]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_decode_block_stereo']=evidence;status.get('unverified_work',{}).pop('stereo_decoder_initial_retail_comparisons',None);path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Verified stereo block decoder:'
if marker not in text:path.write_text(text+'\n'+marker+' build_sound_decode_blocks_draft.ps1/verify_sound_decode_stereo_samples.py. '+evidence['scope']+' This supersedes the earlier unverified stereo draft status; mono is already verified within its reports. Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_decode_stereo_samples.py' not in text:path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_sound_decode_blocks_draft.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_decode_stereo_samples.py')\nif ($LASTEXITCODE -ne 0) { throw 'Mono decoder verification failed' }\n")
