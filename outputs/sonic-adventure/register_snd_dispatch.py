import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
statuspath=p/'project_status.json';status=json.loads(statuspath.read_text())
report='analysis/sound_snd_transfer_dispatch_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==1280
for name,start,end in [('sa_sound_snd_table_transfer_step',0x8c66a244,0x8c66a348),('sa_sound_snd_transfer_dispatch',0x8c66a348,0x8c66a364)]:
 source=p/f'source/{name}.c';s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
 s=s[:a]+f'/* Retail SND encoding path, {start:08x}..{end:08x}.\n * Selector 2 uses retail table67c3ec; selector1 expands raw bytes; otherwise BE16.\n * Native transfer/pending calls connected; query/submission callbacks supplied by caller. */\n'+s[b:];source.write_text(s)
 if not any(r['name']==name for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=f'0x{start:08x}',end_exclusive=f'0x{end:08x}',source=f'source/{name}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within connected report scope'))
status['verified_progress']['sound_snd_transfer_dispatch']=evidence
path.write_text(json.dumps(registry,indent=2)+'\n');statuspath.write_text(json.dumps(status,indent=2)+'\n')
table=binary[0x66c3ec:0x66c5ec]
(p/'analysis/snd_sample_lookup_table.json').write_text(json.dumps(dict(address='0x8c67c3ec',bytes=512,sha256=hashlib.sha256(table).hexdigest(),unsigned_16bit_samples=[int.from_bytes(table[i:i+2],'little') for i in range(0,512,2)],scope='Retail lookup data for encoding selector2. Exact table indexing tested; codec identity not asserted.'),indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nSND lookup-table conversion66a244 and encoding dispatcher66a348: build_sound_snd_transfer_dispatch.ps1/verify_sound_snd_transfer_dispatch.py reproduce1280 retail comparisons with native three transfer paths and pending getter. All256 table indexes exercised in stereo128-frame fixture; exact independently checked output,capacity2/128,offset1,separation256,phase/submission arguments and buffer guards. snd_sample_lookup_table.json records retail512-byte table/hash; codec identity not asserted. Query/submission callbacks modeled; physical audio/fullgame unverified. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& (Join-Path $PSScriptRoot 'build_sound_snd_transfer_dispatch.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_snd_transfer_dispatch.py')\nif ($LASTEXITCODE -ne 0) { throw 'SND dispatch verification failed' }\n")
