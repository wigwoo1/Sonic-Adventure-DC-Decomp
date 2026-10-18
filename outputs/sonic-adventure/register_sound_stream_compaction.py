import hashlib,json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/sound_stream_compaction_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==60
binary=(root.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for kind,start,end in [('mono',0x8c667960,0x8c667978),('stereo',0x8c66792c,0x8c667960)]:
    name=f'sa_sound_stream_{kind}_compact';source=f'source/{name}.c'
    file=root/source;file.write_text(file.read_text().replace('retail Tikal helper','retail audio stream helper'))
    if not any(r['name']==name for r in registry['routines']):
        registry['routines'].append(dict(module='1ST_READ.BIN',name=name,address=f'0x{start:08x}',end_exclusive=f'0x{end:08x}',source=source,retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_stream_compaction']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native stream sample compaction:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_sound_stream_compaction.py and build_sound_stream_mono_compact.ps1/build_sound_stream_stereo_compact.ps1. '+evidence['scope']+' Overall completion10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_stream_compaction.py' not in text:path.write_text(text+"\n& (Join-Path $PSScriptRoot 'build_sound_stream_mono_compact.ps1')\n& (Join-Path $PSScriptRoot 'build_sound_stream_stereo_compact.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_sound_stream_compaction.py')\nif ($LASTEXITCODE -ne 0) { throw 'Stream compaction verification failed' }\n")
