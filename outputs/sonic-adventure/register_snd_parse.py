import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
source=p/'source/sa_sound_snd_parse.c'
s=source.read_text();start=s.index('/* Static C recovery');end=s.index('SA_STAGE_EXPORT',start)
s=s[:start]+'/* Retail .snd/.sd-NUL header parser, 8c669c14..8c669e38.\n * Encoding 1/2 produces 8-bit samples; encoding 3 produces 16-bit samples.\n * Returns data+big-endian offset and computes frames using native quotient. */\n'+s[end:];source.write_text(s)
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report='analysis/sound_snd_parse_verification.json'
if not any(r['name']=='sa_sound_snd_parse' for r in registry['routines']):
 registry['routines'].append(dict(module='1ST_READ.BIN',name='sa_sound_snd_parse',address='0x8c669c14',end_exclusive='0x8c669e38',source='source/sa_sound_snd_parse.c',retail_span_sha256=hashlib.sha256(binary[0x659c14:0x659e38]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
inventory=[];scanned=0
for file in (p.parents[1]/'work/sonic-adventure/extracted/files').rglob('*'):
 if not file.is_file():continue
 scanned+=1
 with file.open('rb') as f:header=f.read(24)
 if header[:4] in (b'.snd',b'.sd\0'):
  inventory.append(dict(path=str(file.relative_to(p.parents[1])),bytes=file.stat().st_size,header_hex=header.hex()))
(p/'analysis/snd_asset_inventory.json').write_text(json.dumps(dict(files_scanned=scanned,standalone_header_matches=inventory,scope='Leading 24-byte headers of extracted files; embedded or compressed headers not scanned.'),indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_snd_parse']=json.loads((p/report).read_text());status['verified_progress']['snd_asset_inventory']=dict(files_scanned=scanned,standalone_matches=len(inventory),report='analysis/snd_asset_inventory.json');path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nSND header parser 8c669c14: build_sound_snd_parse.ps1 and verify_sound_snd_parse.py reproduce 720 retail comparisons. Native unsigned quotient connected; independent header rejection, return pointer, rate/channels/bits/frames/type and preserved buffers checked. Constructed fixtures only; asset playback/fullgame unverified. snd_asset_inventory.json scans extracted file prefixes, not embedded/compressed data. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_snd_parse.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_snd_parse.py')\nif ($LASTEXITCODE -ne 0) { throw 'SND parser verification failed' }\n")
print('Scanned',scanned,'files;',len(inventory),'standalone SND headers')
