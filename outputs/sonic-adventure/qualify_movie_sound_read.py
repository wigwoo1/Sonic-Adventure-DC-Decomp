"""Qualify connected existing getters and fresh earlier-startup transfer execution."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_sound_read_translation','movie_sound_read_connection','movie_sound_read_verification','movie_sound_read_irq_control_verification','movie_sound_read_retained_verification','movie_sound_read_startup_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_sound_read_translation'];direct=reports['movie_sound_read_verification'];control=reports['movie_sound_read_irq_control_verification'];reg=reports['movie_sound_read_retained_verification'];startup=reports['movie_sound_read_startup_verification']
 assert t['instruction_rows']==70 and len(t['routines'])==3 and reports['movie_sound_read_connection']['compiled_source_count']==194
 assert direct['cases_passed']==123 and direct['independent_checks']==1083
 assert all(r['covered_rows']==r['static_rows'] and r['covered_outcomes']==r['conditional_outcomes'] and not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
 assert sum(r['conditional_outcomes'] for r in direct['coverage_by_routine'])==6
 assert control['original_native_comparisons']==18 and control['lifecycles_passed']==6 and control['independent_checks']==120
 assert reg['total_original_native_comparisons']==711 and startup['cases_passed']==2 and startup['sound_trace_events_dropped']==0
 dll=digest(ROOT/'work/sonic-adventure/build/movie_sound_read.dll');assert all(r['dll_sha256']==dll for r in (direct,control,reg,startup))
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for definition in t['routines']:
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==definition['address'])
  assert row['retail_span_sha256']==definition['retail_span_sha256']
  canonical.append({k:row[k] for k in ('address','name','source','verification','status')});files.extend((P/row['source'],P/row['verification']))
 next_folder=P/'analysis/movie-startup-frontier';files.extend(sorted(next_folder.glob('*')));files.extend((P/'ghidra/export_movie_startup_frontier.ps1',P/'source/sa_movie_transfer_wait.c'))
 limitations=[
 'Three existing original field667150,byte667188andword66910cgetters execute all70own rows and6outcomes with the actual already-connected10d460provider and mappedSoundRAM. Canonical sources/proofs/names/status fields remain unchanged; no duplicate recovery credit. Native mapped-bus errors retain actual reached partial state rather than older modeled lower-read results.',
 'Two fresh full original/native captures start at declared earlier entry0546c0 with explicit initial RAM/console/platform policy, not a reset/bootROM checkpoint. Cold original producers create filesystem/interface fromzero, load exactMASARUwith89disc reads, clear full8MiBVRAM, perform two original full2MiBSEGAfills and two actual33376-byte native copies/IRQ/RTE completions before reaching genuine missing64bddc. All1065282Soundevents are retained and compared, plus fullRAM/VRAM/Sound/CPU/FPU/banked/special/device/runtime/boundary state. No guest checkpoint,helper result,completion flag,shortened original transfer or skipped instruction is supplied.',
 'Cold declares longer finite rawzeroFIFO/IRQ observations than the older stopped profile. These inputs do not generate pending/completion; native data movement and original handler/SDK code do. Six paired controls preserve actual empty-status-stream exhaustion at645bc0 after real copy, with pendingbit/active descriptor/completion0. Warm missing-file control remains unchanged in behavior.',
 'The new verification model resets its research wait counter on each actual REQUEST entry, matching the already-existing function-local native counter. The previous oracle improperly shared that research count across repeated startup requests. No game instruction/native production code was changed for this correction. Original unbounded production wait and research-only two-continue guard remain explicit.',
 'The authored platform remains untimed, CPU-triggered and tied to a known wait yield point. Physical timing/security/suspend/external triggers, broader asynchronous scheduling, ARM execution/audio/pixels/gameplay and full reset/boot remain unfinished. Next64bddcis exported only and receives no recovery credit; separate bootstrapac000010remains unresolved. Old proof bytes are preserved; current startup captures are fresh, with no historical full-copy/video-clear result substituted.',
 'All full-source/publicGitHub/documentation,PC60/120/240FPS/up to8K,Android/stereoscopic3DS/demake,SUPIRHDtoggle/texturepercent/render/aspect goals remain unfinished. TextureOff must use vertex colors. Overall10% is weighted milestones,not measured recovered-code coverage.'
 ]
 report=dict(integration_adapters=t['routines'],canonical_reuse=canonical,newly_registered_entries=0,newly_qualified_rows=0,qualified_integration_rows=70,qualified_conditional_outcomes=6,reused_additional_credit=0,compiled_source_count=194,direct_comparisons=123,independent_direct_checks=1083,finite_irq_control_comparisons=18,independent_irq_control_checks=120,retained_cases_replayed_fresh=711,fresh_original_native_startup_comparisons=2,total_original_native_comparisons=854,independent_startup_checks=startup['independent_checks'],cold_original_sound_fill_bytes=0x400000,cold_full_vram_clear_bytes=0x800000,cold_actual_transfer_bytes=66752,cold_requests_returned=2,cold_irq_deliveries=2,cold_complete_sound_trace_events=1065282,sound_trace_events_dropped=0,actual_next_frontier='0x8c64bddc',next_frontier_recovery_credit=0,partial_bootstrap_frontier='0xac000010',dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Original getters now use real lower native reads and SoundRAM; original earlier cold startup advances through actual transfer/IRQ completion to64bddc. Three already registered routines reused;854fresh original/native comparisons,full original data loops and exact complete Soundtrace. Physical/full-game/reset claims remain excluded.',limitations=limitations)
 (P/'analysis/movie_sound_read_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified194-source startup/getters;854freshcomparisons;70reusedrows/6outcomes;0duplicatecredit')
if __name__=='__main__':main()
