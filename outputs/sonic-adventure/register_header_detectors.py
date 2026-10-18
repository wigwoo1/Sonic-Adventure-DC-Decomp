import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());root=p.parents[1];binary=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_header_detectors_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==88
for name,start,end in [('sa_sound_snd_header_detect',0x8c669e38,0x8c669e6a),('sa_sound_spsd_header_detect',0x8c66a61a,0x8c66a62e),('sa_sound_wave_header_detect',0x8c66ab2a,0x8c66ab5e)]:
 source=p/f'source/{name}.c';s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a);source.write_text(s[:a]+f'/* Retail audio signature detector {start:08x}..{end:08x}.\n * Native four-byte comparator connected through caller dispatch. */\n'+s[b:])
 if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=f'0x{start:08x}',end_exclusive=f'0x{end:08x}',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n');path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_header_detectors']=evidence
assets=[];scanned=0
for file in (root/'work/sonic-adventure/extracted/files').rglob('*'):
 if not file.is_file():continue
 scanned+=1
 with file.open('rb') as f:header=f.read(12)
 kind='SPSD' if header[:4]==b'SPSD' else 'WAVE' if header[:4]==b'RIFF' and header[8:12]==b'WAVE' else None
 if kind:assets.append(dict(path=str(file.relative_to(root)),format=kind,bytes=file.stat().st_size,sha256=hashlib.sha256(file.read_bytes()).hexdigest()))
inventory=dict(files_scanned=scanned,assets=assets,scope='Standalone leading12-byte signatures; embedded/compressed bank data not scanned. Asset detector/metadata execution not yet verified.')
(p/'analysis/spsd_wave_asset_inventory.json').write_text(json.dumps(inventory,indent=2)+'\n');status['verified_progress']['spsd_wave_asset_inventory']=dict(files_scanned=scanned,standalone_matches=len(assets),report='analysis/spsd_wave_asset_inventory.json');path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nAudio signature detectors SND669e38/SPSD66a61a/WAVE66ab2a: build_sound_header_detectors.ps1/verify_sound_header_detectors.py reproduce88 retail cases with native byte comparator,no modeled calls. Both SND tags,SPSD,RIFF/WAVE,each tag byte XOR1/XOR128,banks,independent accept/input preservation. spsd_wave_asset_inventory.json records original-file leading signature inventory/hashes; embedded/compressed headers not scanned and actual asset execution not yet verified. Short-buffer safety/loader/playback/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_header_detectors.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_header_detectors.py')\nif ($LASTEXITCODE -ne 0) { throw 'Audio detector verification failed' }\n")
print('Scanned',scanned,'files;',len(assets),'SPSD/WAVE assets')
