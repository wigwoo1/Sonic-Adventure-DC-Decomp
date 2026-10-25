"""Validate completed full caller captures without losing expensive executions."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes();word=raw[0x8c67c1c4-0x8c010000:0x8c67c1c8-0x8c010000];assert word==b'SEGA'
 files=[Path(__file__),retail];records=[];checks=0;dll=digest(ROOT/'work/sonic-adventure/build/movie_queue_family.dll')
 for cold in (False,True):
  path=ROOT/f'work/sonic-adventure/movie_queue_family_prefix_{"cold" if cold else "warm"}_capture.json';data=json.loads(path.read_text());files.append(path)
  for pinned,h in data['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
  r=data['record'];assert data['full_original_native_comparison'] and data['dll_sha256']==dll;checks+=2
  assert r['status']==-2 and r['stop_pc']==('0x8c10faa4' if cold else '0x8c64bddc');checks+=2
  assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000;checks+=2
  init=[b for b in r['boundaries'] if b['target']=='0x8c010180'];assert len(init)==1 and init[0]['arguments']==[1,0,1,3];checks+=2
  assert r['sound_trace_dropped']==0;checks+=1
  if cold:
   assert r['filesystem_root_before']=='0x0' and data['before_interface']=='0x0' and r['filesystem_root_after']=='0x8c6946e0' and r['filesystem_initialized_flag']==1;checks+=4
   assert r['disc_read_attempts']==89 and r['masaru_payload_matches'] and r['masaru_bytes']==33408 and r['masaru_sha256_at_stop']=='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9';checks+=4
   assert r['sound_state']==[0,0x80000,0,0x200000,0x80000,0] and r['sound_trace_events']==0x80000 and r['sound_ram_sha256']==hashlib.sha256(word*0x80000).hexdigest();checks+=3
   transfers=[b for b in r['boundaries'] if b['target']=='0x8c10d770' and b['arguments'][3]==0x80000];assert len(transfers)==1 and transfers[0]['arguments']==[0x8c67c1c4,0xa0800000,4,0x80000];checks+=2
   assert any(b['target']=='0x8c65a5dc' for b in r['boundaries']) and any(b['target']=='0x8c667080' for b in r['boundaries']);checks+=2
   route=[b for b in r['boundaries'] if b['target'] in ('0x8c667080','0x8c666f98','0x8c666d10','0x8c10f9f4','0x8c10faa4')]
   assert [b['target'] for b in route]==['0x8c667080','0x8c666f98','0x8c666d10','0x8c10f9f4','0x8c10faa4'];checks+=1
   assert route[0]['arguments'][0]==route[2]['arguments'][0] and route[0]['arguments'][1]==route[2]['arguments'][1]==0;checks+=1
   assert route[3]['arguments'][1:]==[0xa0800000,0x0cd00020,33408-32];checks+=1
   assert route[4]['arguments'][0]==route[3]['arguments'][0] and route[4]['arguments'][1]==0;checks+=1
  else:
   assert r['filesystem_root_before']==r['filesystem_root_after']=='0x8cc80000' and not r['disc_read_attempts'] and not r['masaru_payload_matches'] and not any(r['sound_state']);checks+=4
  keys=('input','status','stop_pc','steps','boundary_comparisons','disc_read_attempts','filesystem_root_before','filesystem_root_after','filesystem_initialized_flag','masaru_bytes','masaru_sha256_at_stop','masaru_payload_matches','clear_native','clear_reference','sound_state','sound_ram_sha256','sound_trace_sha256','sound_trace_events','sound_trace_dropped','write_watch','full_state_match','ordered_boundaries_match')
  records.append({k:r[k] for k in keys}|dict(elapsed_seconds=data['elapsed_seconds']))
 report=dict(cases_passed=2,independent_checks=checks,cold_profile_reexecuted=True,full_original_transfer_words=0x80000,full_original_transfer_bytes=0x200000,retail_fill_word='0x41474553',retail_fill_ascii='SEGA',cases=records,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Fresh uninterrupted caller comparisons. Cold original producers load exact MASARU bytes with 89 real reads, clear full eight-MiB VRAM, execute all 524288 original two-MiB sound-RAM stores of the retail SEGA word and reach missing original transfer start 10faa4. Warm missing-file control retains 64bddc. Cold declares a longer finite sequence of raw zero FIFO observations; warm retains its older input list. All sound trace events are retained and compared exactly. No helper return, guest checkpoint or shortened transfer is supplied; ARM/driver execution and complete startup remain unfinished.')
 (P/'analysis/movie_queue_family_prefix_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Validated full advancing caller:',checks,'checks')
if __name__=='__main__':main()
