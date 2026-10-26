"""Record completed earlier startup separately from the canonical producer proof."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=P/'analysis/movie_audio_mode_startup_verification.json';r=json.loads(path.read_text())
 assert r['cases_passed']==2 and r['completed_profiles']==['warm','cold'] and r['independent_checks']==88
 for file,h in r['sha256'].items():assert digest(file)==h,file
 cold=next(c for c in r['cases'] if c['profile']=='cold')
 assert cold['boundary_comparisons']==582880 and cold['sound_trace_events']==1065282 and not cold['sound_trace_dropped']
 for case in r['cases']:
  assert case['audio_queue_metadata']['pending']==case['audio_queue_metadata']['tail']==1
 status_path=P/'project_status.json';status=json.loads(status_path.read_text())
 status['verified_progress']['movie_audio_mode_startup']=dict(completed_original_native_profiles=2,independent_checks=88,actual_next_frontier='0x8c6437f0',newly_registered_entries=0,direct_console_priming_used=False,configured_raw_platform_inputs_explicit=True,actual_stereo_command_pending=1,arm_acknowledgement_supplied=False,audible_output_verified=False,cold_original_vram_clear_bytes=0x800000,cold_original_sound_fill_bytes=0x400000,cold_actual_transfer_bytes=66752,cold_original_request_returns=2,cold_actual_irq_returns=2,cold_sound_events_compared=1065282,cold_sound_events_dropped=0,verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_AUDIO_MODE.txt',scope=r['scope'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print('Recorded2audio-mode startup profiles;oneactualpendingcommand,noARMack/audiooutputclaim')
if __name__=='__main__':main()
