"""Qualify completed configured startup captures; never count a running capture."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__)];records=[];checks=0
 dll=digest(ROOT/'work/sonic-adventure/build/movie_packet_flash.dll')
 for profile in ('warm','cold'):
  path=ROOT/f'work/sonic-adventure/movie_packet_flash_configured_startup_{profile}_capture.json'
  if not path.exists():
   assert profile=='cold';continue
  capture=json.loads(path.read_text());files.append(path)
  for pinned,h in capture['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
  r=capture['record'];c=r['input']
  assert capture['dll_sha256']==dll and capture['full_original_native_comparison'] and r['full_original_native_comparison'];checks+=3
  assert r['status']==-2 and r['stop_pc']=='0x8c64387c' and r['uninterrupted_declared_entry']=='0x8c0546c0';checks+=3
  assert r['full_state_match'] and r['ordered_boundaries_match'] and not c.get('prime_console');checks+=3
  assert c['full_flash_storage'] and c['flash_writable'] and c['info_bytes']==[0]*12+[49]+[0]*8+[1,0,0];checks+=3
  assert r['flash_storage_snapshot'][5][0]>=1 and r['flash_storage_snapshot'][5][1]>=5 and r['flash_storage_snapshot'][-3]>=258;checks+=3
  assert r['flash_storage_snapshot'][5][2:]==[0,0] and r['flash_storage_snapshot'][-2:]==[0,0];checks+=2
  import console_service_reference
  assert r['flash_storage_sha256']==hashlib.sha256(console_service_reference.inputs(c)[1]).hexdigest();checks+=1
  assert r['packet_native_calls']['0x8c64bddc']>=1 and r['packet_native_calls']['0x8c604044']>=1 and r['packet_native_calls']['0x8c603c08']>=1;checks+=3
  assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000 and r['vram_sha256']==hashlib.sha256(bytes(0x800000)).hexdigest();checks+=4
  assert not r['sound_trace_dropped'];checks+=1
  if profile=='warm':
   assert r['boundary_comparisons']==1576 and r['disc_read_attempts']==0 and r['flash_storage_snapshot'][5]==[1,5,0,0] and r['flash_storage_snapshot'][-3:]==[258,0,0];checks+=4
   assert not any(r['sound_state']) and not any(r['runtime_state'][4:11]);checks+=2
  else:
   driver=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV';files.append(driver)
   assert r['filesystem_root_before']==capture['before_interface']=='0x0' and r['filesystem_initialized_flag']==1;checks+=3
   assert r['disc_read_attempts']==89 and r['masaru_payload_matches'] and r['masaru_bytes']==33408 and r['masaru_sha256_at_stop']==digest(driver);checks+=4
   assert list(r['runtime_state'][4:])==[0,2,2,66752,2,0,0,1] and r['original_request_calls']==r['empty_server_calls']==r['context_vector_entries']==2;checks+=4
   wanted=bytearray(b'SEGA'*0x80000);wanted[:33408-32]=driver.read_bytes()[32:]
   assert r['sound_ram_sha256']==hashlib.sha256(wanted).hexdigest();checks+=1
   assert r['sound_state']==[18,1065264,72,4261056,1065282,0] and r['sound_trace_events']==1065282;checks+=2
   assert len(r['sound_fill_calls'])==2 and all(args==[0x8c67c1c4,0xa0800000,4,0x80000] for args in r['sound_fill_calls']);checks+=2
   assert c['fifo_ready_count']==0x200000 and c['irq_samples']==[0,8]+[0]*16;checks+=2
  keys=('input','status','stop_pc','steps','boundary_comparisons','disc_read_attempts','filesystem_root_before','filesystem_root_after','filesystem_initialized_flag','flash_storage_snapshot','flash_storage_sha256','packet_native_calls','clear_native','clear_reference','full_vram_bytes_compared','sound_state','sound_ram_sha256','sound_trace_events','sound_trace_dropped','runtime_state','exception_state','original_request_calls','empty_server_calls','context_vector_entries','sound_fill_calls','full_state_match','ordered_boundaries_match')
  records.append({k:r[k] for k in keys}|dict(profile=profile))
 assert records and records[0]['profile']=='warm'
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,actual_next_frontier='0x8c64387c',direct_console_priming_used=False,completed_profiles=[r['profile'] for r in records],erased_flash_profile_qualified=False,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Completed fresh original/native configured earlier-startup captures only. Original0546c0initialization parses explicitly supplied synthetic console info/valid raw flash packet5,validates CRC,usesactualnativeoriginalpacketservices and reaches64387c. WholeRAM/VRAM/Sound/CPU/FPU/banked/special/runtime/device/boundary/flash state compares. No direct-test priming,helper result,guestcheckpoint,shortenedcopyorcompletionflag. Raw platform info/flash differ from the erased profile; this is not proof that erased flash exits. Cold evidence is included only when its actual completed capture exists. Not reset/bootROM/physical timing/ARM/audio/gameplay proof.')
 (P/'analysis/movie_packet_flash_startup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Completed configured startup profiles',report['completed_profiles'],'checks',checks)
if __name__=='__main__':main()
