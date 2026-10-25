"""Fresh original caller including the complete original 2-MiB transfer."""
import hashlib,json,time
from pathlib import Path
import write_watch_transfer_start as fast
import verify_movie_console as console
import verify_movie_init_records as init
v=fast.old;P=v.P;ROOT=v.ROOT;DRIVER=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 driver=DRIVER.read_bytes();ram=v.seed_ram();base=next(c for c in console.cases() if c['entry']==0x8c0546c0)
 files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'sound_ram_reference.py',P/'source/sound_ram_bus.c',P/'source/sound_ram_bus.h',v.MP,P/'analysis/movie_transfer_wait_translation.json',P/'analysis/movie_bulk_transfer_translation.json',P/'analysis/movie_transfer_start_connection.json',P/'analysis/sound_ram_bus_verification.json',P/'analysis/write_watch_qualification.json',DRIVER]
 for cold in (False,True):
  c=v.Input(base|dict(cold_filesystem=cold,progress=True,instruction_budget=0x4000000))
  if cold:c.pop('fifo_samples',None);c['fifo_ready_count']=0x40000
  m,s,f=init.prepare(ram,c);before_root=v.load(m,0x8c673414);before_interface=v.load(m,0x8c673420)
  print('START transfer prefix',cold,flush=True);started=time.perf_counter();record,path,a,s,f=fast.compare(m,s,f,c);elapsed=time.perf_counter()-started
  root=v.load(a,0x8c673414);payload=v.sample(a,0x0cd00000,len(driver));edges=set()
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  record.update(filesystem_root_before=hex(before_root),filesystem_root_after=hex(root),filesystem_initialized_flag=int.from_bytes(v.sample(a,root+0x38,2),'little'),masaru_sha256_at_stop=hashlib.sha256(payload).hexdigest(),masaru_payload_matches=payload==driver,masaru_bytes=len(driver))
  record['cache_writeback_instructions']=sum(v.ROWS.get(pc,'').startswith('ocbwb ') for pc in path)
  if cold:
   request=next(b for b in record['boundaries'] if b['target']=='0x8c65a5dc');client=request['arguments'][0]
   start=next(b for b in record['boundaries'] if b['target']=='0x8c10faa4');descriptor=start['arguments'][0];sdk=v.load(a,0x8c67c1d0)
   record.update(client_record=hex(client),client_state=v.load(a,client+4),client_completion=v.load(a,client+24),client_callback=hex(v.load(a,client+16)),sdk_current_record=hex(v.load(a,sdk+0x190)),sdk_pending=v.load(a,sdk+0x1a4),descriptor=hex(descriptor),descriptor_active=v.load(a,descriptor+44),channel_index=v.load(a,descriptor),completion_wait_tests=path.count(0x8c65a6ec),empty_server_calls=sum(b['target']=='0x8c666e02' for b in record['boundaries']))
  capture=dict(record=record,before_interface=hex(before_interface),elapsed_seconds=elapsed,executed_rows=[hex(x) for x in sorted(set(path))],executed_edges=[[hex(x),hex(y)] for x,y in sorted(edges)],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},full_original_native_comparison=True)
  target=ROOT/f'work/sonic-adventure/movie_transfer_start_prefix_{"cold" if cold else "warm"}_capture.json';target.write_text(json.dumps(capture,indent=2)+'\n')
  print('CAPTURED transfer prefix',cold,record['status'],record['stop_pc'],record['boundary_comparisons'],record['disc_read_attempts'],record['sound_state'],flush=True)
if __name__=='__main__':main()
