import json,hashlib
from pathlib import Path
p=Path(__file__).resolve().parent
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
statuspath=p/'project_status.json';status=json.loads(statuspath.read_text())
for stem,start,end in [('sound_snd_transfer16_step',0x8c66a01c,0x8c66a12e),('sound_snd_transfer8_step',0x8c66a13c,0x8c66a236)]:
 source=p/f'source/sa_{stem}.c';s=source.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
 s=s[:a]+f'/* Retail SND sample transfer step, {start:08x}..{end:08x}.\n * Pending getter resolves to native code; query/submission are caller callbacks. */\n'+s[b:];source.write_text(s)
 report=f'analysis/{stem}_native_pending_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==96
 if not any(r['name']==f'sa_{stem}' for r in registry['routines']):registry['routines'].append(dict(module='1ST_READ.BIN',name=f'sa_{stem}',address=f'0x{start:08x}',end_exclusive=f'0x{end:08x}',source=f'source/sa_{stem}.c',retail_span_sha256=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),verification=report,status='Compiled and matched within report scope'))
 status['verified_progress'][stem]=evidence
 with (p/'build_and_verify.ps1').open('a') as f:f.write(f"\n& (Join-Path $PSScriptRoot 'build_{stem}.ps1')\n& $python (Join-Path $PSScriptRoot 'verify_{stem}_native_pending.py')\nif ($LASTEXITCODE -ne 0) {{ throw 'SND transfer verification failed' }}\n")
path.write_text(json.dumps(registry,indent=2)+'\n');statuspath.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nSND16-bit transfer66a01c and8-bit transfer66a13c: respective build_sound_snd_transfer16_step.ps1/build_sound_snd_transfer8_step.ps1 and native_pending verifiers reproduce96 retail cases each. Native file pending getter, query/submission callbacks modeled. Independent exact sample conversion,mono/separated stereo destinations,capacity/cursor,phase and source/buffer guards. Retail8-bit submission length equals frames*channels despite16-bit output stores; retained and verified rather than corrected.16-bit length equals frames*channels*2. Physical audio/fullgame unverified. Overall project remains 10%.\n')
