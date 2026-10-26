"""Fresh uninterrupted earlier-caller comparisons with original full transfer loops."""
import hashlib,json,sys,time
from pathlib import Path
import write_watch_graphics_init as fast
import verify_movie_console as console
import verify_movie_packet_flash as fixtures
import verify_movie_init_records as init
v=fast.old;P=v.P;ROOT=v.ROOT;DRIVER=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def save_failure(exc,profile):
 """Retain useful partial evidence if a costly comparison fails."""
 tb=exc.__traceback__
 while tb:
  local=tb.tb_frame.f_locals
  if 'wanted' in local and 'events' in local and 'other' in local and 'rc' in local:
   native=local['s'];reference=local['other'];actual=local['events'];wanted=local['wanted'];first=None
   for i,(a,b) in enumerate(zip(actual,wanted)):
    if a!=b:first=dict(index=i,native_target=hex(a[0]),reference_target=hex(b[0]),different_fields=[j for j,(x,y) in enumerate(zip(a,b)) if x!=y]);break
   report=dict(profile=profile,error=str(exc)[:2500],native_status=local['rc'],reference_status=local['status'],native_stop_pc=hex(native.pc),reference_stop_pc=hex(reference.pc),native_pr=hex(native.pr),reference_pr=hex(reference.pr),native_boundaries=len(actual),reference_boundaries=len(wanted),first_boundary_difference=first,reference_last_targets=[hex(e[0]) for e in wanted[-12:]],native_last_targets=[hex(e[0]) for e in actual[-12:]],dll_sha256=digest(v.effects.DLL),full_original_native_comparison=False)
   (ROOT/f'work/sonic-adventure/movie_graphics_init_startup_{profile}_failure.json').write_text(json.dumps(report,indent=2)+'\n');return
  tb=tb.tb_next
def main():
 selected=sys.argv[1:];assert not selected or all(x in ('warm','cold') for x in selected)
 profiles=selected or ['warm','cold'];driver=DRIVER.read_bytes();ram=v.seed_ram();base=next(c for c in console.cases() if c['entry']==0x8c0546c0)
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),P/'transfer_runtime_reference_cpu.py',P/'transfer_runtime_reference.py',P/'windows_write_watch.py',P/'sound_ram_reference.py',v.MP,P/'analysis/movie_graphics_init_connection.json',P/'flash_storage_reference.py',P/'analysis/flash_storage_verification.json',P/'analysis/write_watch_qualification.json',DRIVER]
 for profile in profiles:
  cold=profile=='cold';c=v.Input(base|dict(cold_filesystem=cold,progress=True,instruction_budget=0x10000000,runtime_attached=True,full_flash_storage=True,flash_writable=True,sound_trace_capacity=0x110000))
  # Explicit persistent host configuration, not a guest execution checkpoint.
  # The original startup still initializes SYSINFO and parses/checks this data.
  info=[0]*24;info[12]=ord('1');info[21]=1
  c['info_bytes']=info
  payload=bytes(info[16:24])+bytes([255])*52
  c.update(fixtures.profile(records=[fixtures.record(5,payload)]))
  for key in v.EXCEPTION_KEYS:c[key]=0
  if cold:
   c.pop('fifo_samples',None);c['fifo_ready_count']=0x200000;c['irq_samples']=list(c.get('irq_samples',[]))+[0]*16
  # This binds only oracle instruction-address metadata, not RAM or CPU state.
  # Original645de0 must create the vector bytes and handler fields itself.
  v.bind_vectors(c.get('vbr',0x8ce50000));m,s,f=init.prepare(ram,c)
  before_root=v.load(m,0x8c673414);before_interface=v.load(m,0x8c673420)
  print('START original graphics startup',profile,flush=True);started=time.perf_counter()
  try:r,path,a,s,f=fast.compare(m,s,f,c)
  except BaseException as exc:save_failure(exc,profile);raise
  elapsed=time.perf_counter()-started
  root=v.load(a,0x8c673414);payload=v.sample(a,0x0cd00000,len(driver));edges=set()
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  r.update(filesystem_root_before=hex(before_root),filesystem_root_after=hex(root),filesystem_initialized_flag=int.from_bytes(v.sample(a,root+0x38,2),'little'),masaru_sha256_at_stop=hashlib.sha256(payload).hexdigest(),masaru_payload_matches=payload==driver,masaru_bytes=len(driver))
  r['empty_server_calls']=sum(b['target']=='0x8c666e02' for b in r['boundaries']);r['original_request_calls']=sum(b['target']=='0x8c65a5dc' for b in r['boundaries']);r['context_vector_entries']=sum(b['target']=='0x8ce50600' for b in r['boundaries'])
  r['sound_fill_calls']=[b['arguments'] for b in r['boundaries'] if b['target']=='0x8c10d770' and b['arguments'][3]==0x80000]
  r['native_getter_calls']={hex(entry):sum(b['target']==hex(entry) for b in r['boundaries']) for entry in (v.FIELD,v.BYTE,v.WORD)}
  assert not c.get('prime_console'), 'Full startup must run original initialization'
  r['packet_native_calls']={row['address']:sum(b['target']==row['address'] for b in r['boundaries']) for row in v.manifest['routines']}
  library=v.load(a,v.ROOT_WORD);r['audio_library_root']=hex(library)
  r['audio_command_bytes']=list(v.sample(a,v.COMMAND,20))
  if library:
   meta=library+0x158;queue=v.load(a,meta+12);r['audio_queue_metadata']={name:v.load(a,meta+offset) for name,offset in (('base',12),('capacity',16),('pending',20),('tail',24))};r['audio_queue_first_record']=list(v.sample(a,queue,28))
   assert 0<=r['audio_queue_metadata']['pending']<=r['audio_queue_metadata']['capacity']<=256
   r['audio_queue_records']=[list(v.sample(a,queue+i*28,28)) for i in range(r['audio_queue_metadata']['pending'])]
  else:r['audio_queue_metadata']=None;r['audio_queue_first_record']=None;r['audio_queue_records']=None
  r['midi_command_bytes']=list(v.sample(a,v.SEND_COMMAND,20))
  contexts=sorted({b['arguments'][0] for b in r['boundaries'] if b['target']=='0x8c643c18'})
  r['midi_context_bytes']=[dict(address=hex(ptr),byte1=v.sample(a,ptr+1,1)[0]) for ptr in contexts]
  r['handoff_buffer_state']={name:v.load(a,address) for name,address in (('pointer',0x8c88f7c8),('size',0x8c88f7cc))}
  r['native_sound_final_sha256']=r['sound_ram_sha256'];r['uninterrupted_declared_entry']='0x8c0546c0';r['full_original_native_comparison']=True
  capture=dict(record=r,before_interface=hex(before_interface),elapsed_seconds=elapsed,executed_rows=[hex(a) for a in sorted(set(path))],executed_edges=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},full_original_native_comparison=True,scope='Fresh execution from declared earlier-caller entry0546c0 with explicitly configured mutable flash storage and synthetic console info/valid CRC packet5, not reset/boot-ROM proof or guest checkpoint. No direct-function console priming; original startup performs initialization. Original initialization/data/clear loops execute fully; explicit console/platform observations and initial RAM fixture remain declared. Runtime derives data/IRQ completion itself. Complete state and all retained SoundRAM trace events compare; no dropped Sound trace allowed at qualification.')
  target=ROOT/f'work/sonic-adventure/movie_graphics_init_startup_{profile}_capture.json';target.write_text(json.dumps(capture,indent=2)+'\n')
  print('CAPTURED',profile,r['status'],r['stop_pc'],r['boundary_comparisons'],r['disc_read_attempts'],r['runtime_state'],r['sound_trace_events'],r['sound_trace_dropped'],flush=True)
if __name__=='__main__':main()
