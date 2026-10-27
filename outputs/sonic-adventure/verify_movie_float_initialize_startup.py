"""Check fresh207-source warmstartup, without relabeling retained206cold evidence."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 current=ROOT/'work/sonic-adventure/movie_float_initialize_startup_warm_capture.json';previous=ROOT/'work/sonic-adventure/movie_buffer_initialize_startup_warm_capture.json';new=json.loads(current.read_text());old=json.loads(previous.read_text());r=new['record'];a=old['record'];checks=0
 for path,h in new['sha256'].items():assert digest(path)==h,path
 dll=digest(ROOT/'work/sonic-adventure/build/movie_float_initialize.dll')
 assert new['dll_sha256']==dll and new['full_original_native_comparison'] and r['full_state_match'] and r['ordered_boundaries_match'];checks+=4
 assert r['status']==-2 and r['uninterrupted_declared_entry']=='0x8c0546c0' and not r['input'].get('prime_console');checks+=3
 assert r['boundaries'][:len(a['boundaries'])]==a['boundaries'] and len(r['boundaries'])==4376==len(a['boundaries'])+3;checks+=2
 assert [b['target'] for b in r['boundaries'][4373:]]==['0x8c60d1f2','0x8c5ff2cc','0x8c5ff39c'];checks+=1
 assert r['boundaries'][4373]['target']=='0x8c60d1f2' and r['packet_native_calls']=={'0x8c60edb0':1,'0x8c60d1f2':1};checks+=2
 call=a['boundaries'][-1];words=r['drawing_state_words'];assert call['target']=='0x8c60edb0'
 for address,value in ((0x8c88f64c,call['arguments'][0]),(0x8c88f650,call['arguments'][1]),(0x8c88f62c,0),(0x8c88f660,0xffffffff),(0x8c88f664,0),(0x8c88f668,0x437f0000),(0x8c88f66c,0),(0x8c88f670,0),(0x8c88f674,0)):assert words[hex(address)]==value;checks+=1
 direct=P/'analysis/movie_float_initialize_verification.json';d=json.loads(direct.read_text());assert words[d['setter_destination']]==r['boundaries'][4373]['arguments'][0];checks+=1
 for key in ('vram_sha256','full_vram_bytes_compared','clear_native','clear_reference','sound_ram_sha256','sound_state','sound_trace_events','sound_trace_dropped','flash_storage_snapshot','flash_storage_sha256','audio_queue_records','midi_command_bytes','midi_context_bytes','audio_queue_metadata','original_request_calls','empty_server_calls','context_vector_entries','disc_read_attempts'):
  assert r[key]==a[key],key;checks+=1
 assert r['clear_native']['writes']==0x200000 and r['clear_reference']['steps']==0x200000*16 and r['full_vram_bytes_compared']==0x800000;checks+=3
 assert not r['sound_trace_dropped'] and len(r['audio_queue_records'])==39 and sum(b['target']=='0x8c643c18' for b in r['boundaries'])==32;checks+=3
 wanted=set().union(*(set(b['reachable_addresses']) for b in json.loads((P/'analysis/movie_float_initialize_translation.json').read_text())['routines']));assert wanted<=set(new['executed_rows']);checks+=1
 files=[Path(__file__),current,previous,direct,P/'capture_movie_float_initialize_startup.py',P/'movie_float_initialize_reference.py',P/'write_watch_float_initialize.py',P/'write_watch_buffer_initialize.py',ROOT/'work/sonic-adventure/movie_buffer_initialize_startup_cold_capture.json',P/'analysis/movie_buffer_initialize_startup_verification.json']
 report=dict(cases_passed=1,independent_checks=checks,boundary_comparisons=4376,exact_206_source_boundary_prefix=True,actual_next_frontier=r['stop_pc'],drawing_state_words=words,previous_cold_profile_retained_not_rerun=True,new_cold_profile_claimed=False,executed_rows=27,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Fresh207-source uninterrupted configured0546c0warmstartup;complete original/nativeCPU/FPU/RAM/VRAM/Sound/platform/runtime/flash andorderedboundaries match. Exact206boundaryprefix;independent actualpointer/size/reset/fourfloat/globalsetter outputs checked;all27newrows execute. Earlierwarmclear/VRAM/flash/39SDKrecords/32MIDIoracles retained. Prior206fullcoldproof remains intact andhashchecked,notrerun orclaimedas207-sourcecoldstartup. No checkpoint/helperreturn/priming/resetboot/gameplayclaim;precision/alignmentphysicalexceptiondeliveryunfinished.')
 (P/'analysis/movie_float_initialize_startup_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('FLOAT STARTUP',checks,r['stop_pc'])
if __name__=='__main__':main()
