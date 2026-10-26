"""Record completed startup evidence separately; add no duplicate routine credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_midi_commands_startup_verification.json';r=json.loads(path.read_text())
 assert r['cases_passed']==2 and r['completed_profiles']==['warm','cold'] and r['independent_checks']==118
 for file,h in r['sha256'].items():assert digest(file)==h,file
 cold=next(c for c in r['cases'] if c['profile']=='cold')
 assert cold['boundary_comparisons']==583168 and cold['sound_trace_events']==1065282 and not cold['sound_trace_dropped']
 for case in r['cases']:assert case['audio_queue_metadata']['pending']==case['audio_queue_metadata']['tail']==39
 path_status=P/'project_status.json';s=json.loads(path_status.read_text())
 s['verified_progress']['movie_midi_commands_startup']=dict(completed_original_native_profiles=2,independent_checks=118,actual_next_frontier='0x8c656940',newly_registered_entries=0,direct_console_priming_used=False,configured_raw_platform_inputs_explicit=True,new_midi_commands_enqueued=32,queue_entry_bytes_independently_checked=1092,existing_setter_additional_credit=0,actual_commands_pending=39,arm_acknowledgement_supplied=False,audible_output_verified=False,cold_original_vram_clear_bytes=0x800000,cold_original_sound_fill_bytes=0x400000,cold_actual_transfer_bytes=66752,cold_original_request_returns=2,cold_actual_irq_returns=2,cold_sound_events_compared=1065282,cold_sound_events_dropped=0,verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_MIDI_COMMANDS.txt',scope=r['scope'])
 path_status.write_text(json.dumps(s,indent=2)+'\n');print('Recorded2startup profiles;39actualpendingcommands;noARM/audiooutputclaim')
if __name__=='__main__':main()
