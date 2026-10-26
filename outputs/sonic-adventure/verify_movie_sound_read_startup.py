"""Qualify completed fresh caller captures without rerunning their expensive loops."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];DRIVER=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 records=[];files=[Path(__file__),DRIVER];checks=0;dll=digest(ROOT/'work/sonic-adventure/build/movie_sound_read.dll');driver=DRIVER.read_bytes()
 retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes();word=raw[0x8c67c1c4-0x8c010000:0x8c67c1c8-0x8c010000];assert word==b'SEGA';files.append(retail)
 for profile in ('warm','cold'):
  path=ROOT/f'work/sonic-adventure/movie_sound_read_startup_{profile}_capture.json';capture=json.loads(path.read_text());files.append(path)
  for pinned,h in capture['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
  r=capture['record'];assert capture['full_original_native_comparison'] and r['full_original_native_comparison'] and capture['dll_sha256']==dll;checks+=3
  assert r['status']==-2 and r['stop_pc']=='0x8c64bddc' and r['uninterrupted_declared_entry']=='0x8c0546c0';checks+=3
  assert r['full_state_match'] and r['ordered_boundaries_match'] and not r['sound_trace_dropped'];checks+=3
  assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000 and r['vram_sha256']==hashlib.sha256(bytes(0x800000)).hexdigest();checks+=4
  entries=[b for b in r['boundaries'] if b['target']=='0x8c010180'];assert len(entries)==1 and entries[0]['arguments']==[1,0,1,3];checks+=2
  if profile=='cold':
   assert r['filesystem_root_before']==capture['before_interface']=='0x0' and r['filesystem_root_after']=='0x8c6946e0' and r['filesystem_initialized_flag']==1;checks+=4
   assert r['disc_read_attempts']==89 and r['masaru_payload_matches'] and r['masaru_bytes']==33408 and r['masaru_sha256_at_stop']==hashlib.sha256(driver).hexdigest();checks+=4
   assert list(r['runtime_state'][4:])==[0,2,2,66752,2,0,0,1] and r['original_request_calls']==r['empty_server_calls']==r['context_vector_entries']==2;checks+=4
   assert len(r['sound_fill_calls'])==2 and all(args==[0x8c67c1c4,0xa0800000,4,0x80000] for args in r['sound_fill_calls']);checks+=2
   wanted=bytearray(word*0x80000);wanted[:len(driver)-32]=driver[32:]
   assert r['sound_ram_sha256']==hashlib.sha256(wanted).hexdigest();checks+=1
   assert r['sound_state']==[18,1065264,72,4261056,1065282,0] and r['sound_trace_events']==1065282;checks+=2
   assert all(r['native_getter_calls'][hex(entry)]>0 for entry in (0x8c667150,0x8c667188,0x8c66910c));checks+=3
   assert r['native_getter_calls']['0x8c667150']+r['native_getter_calls']['0x8c66910c']==18;checks+=1
   assert sum(b['target']=='0x8c10ebe8' for b in r['boundaries'])>=2 and sum(b['target']=='0x8c668f40' for b in r['boundaries'])>=2;checks+=2
   assert r['input']['fifo_ready_count']==0x200000 and r['input']['irq_samples']==[0,8]+[0]*16 and r['input']['sound_trace_capacity']==0x110000;checks+=3
   assert r['irq_cursor']<=18 and r['fifo_cursor']<=0x200000;checks+=2
  else:
   assert r['filesystem_root_before']==r['filesystem_root_after']=='0x8cc80000' and not r['disc_read_attempts'] and not any(r['sound_state']);checks+=3
   assert not any(r['runtime_state'][4:11]) and r['runtime_state'][11]==1 and not r['original_request_calls'];checks+=3
  keys=('input','status','stop_pc','steps','boundary_comparisons','disc_read_attempts','filesystem_root_before','filesystem_root_after','filesystem_initialized_flag','masaru_bytes','masaru_sha256_at_stop','masaru_payload_matches','clear_native','clear_reference','full_vram_bytes_compared','sound_state','sound_ram_sha256','sound_trace_sha256','sound_trace_events','sound_trace_dropped','write_watch','full_state_match','ordered_boundaries_match','runtime_state','exception_state','vbr','masks','irq_status','fifo_cursor','irq_cursor','original_request_calls','empty_server_calls','context_vector_entries','sound_fill_calls','native_getter_calls')
  records.append({k:r[k] for k in keys}|dict(profile=profile,elapsed_seconds=capture['elapsed_seconds']))
 report=dict(cases_passed=2,independent_checks=checks,cases=records,fresh_original_native_startup_comparisons=2,cold_driver_load_reads=89,full_original_vram_clear_bytes=0x800000,cold_original_sound_fill_operations=2,cold_original_sound_fill_bytes=0x400000,cold_actual_host_transfer_bytes=66752,cold_original_requests_returned=2,cold_actual_irq_deliveries=2,cold_complete_sound_trace_events=1065282,sound_trace_events_dropped=0,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Two fresh uninterrupted original/native comparisons from declared earlier entry0546c0. Cold original producers initialize filesystem/interface fromzero, load exactMASARUwith89reads, execute full8MiBVRAMclear, two original full2MiBSEGAfills and two real33376-byte native transfers/original IRQ/RTE returns before reaching actual64bddcfrontier. Existing getters now execute native10d460andactualSoundRAM. EverySoundevent/fullRAM/VRAM/Sound/CPU/FPU/banked/special/device/runtime/boundary state compares. No droppedSoundtrace,guestcheckpoint,skippedoriginalinstruction,shortenedcopy,completedflagorhelperreturn. Warmcontrol retains its missing-file route. Cold declares longer finite rawzeroFIFO/IRQobservations; these do not generate completion. Initial RAM/console/platform policy is explicit; not reset/bootROM/physical timing/ARM/audio/gameplay proof.')
 (P/'analysis/movie_sound_read_startup_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Fresh startup comparisons',2,'checks',checks)
if __name__=='__main__':main()
