import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/wave_retail_pipeline_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==162
names={'sa_sound_wave_header_detect','sa_memory_compare_bytes','sa_sound_data_chunk_state_initialize','sa_sound_data_chunk_metadata','sa_signed_division','sa_unsigned_quotient','sa_memory_fill','sa_sound_data_transfer_dispatch','sa_file_pending_state_get'}
bits={asset['bits'] for asset in evidence['assets']}
if 16 in bits:names.add('sa_sound_data_transfer16_step')
if 8 in bits:names.add('sa_sound_data_transfer8_step')
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for r in registry['routines']:
 if r['name'] in names:
  extra=r.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
path.write_text(json.dumps(registry,indent=2)+'\n');path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['wave_retail_pipeline']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
with (p/'SOURCE_HANDOFF.txt').open('a') as f:f.write('\nOriginal WAVE integration: verify_wave_retail_pipeline.py reproduces162 comparisons across9 untouched extracted files,banks,capacities0/2/128. Detector->initializer->transfer dispatcher run on persistent state, native comparator/metadata/division/fill/pending/transfer. Independent RIFF chunk parsing validates fixed metadata/retail data-offset assumptions,original hashes and actual payload samples. Destination/source guards,phase/count/submission arguments checked. Up to3 frames per fixture; query/submission callbacks modeled, physical audio/fullgame unverified. All9 standalone SPSD/WAVE inventory matches are WAVE; no standalone SPSD assets found. Overall project remains 10%.\n')
with (p/'build_and_verify.ps1').open('a') as f:f.write("\n& $python (Join-Path $PSScriptRoot 'verify_wave_retail_pipeline.py')\nif ($LASTEXITCODE -ne 0) { throw 'Original WAVE pipeline verification failed' }\n")
print('Verified original WAVE widths:',sorted(bits))
