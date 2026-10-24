"""Continue actual boot after its original MOVIE.BIN request reaches host I/O."""
import hashlib,itertools,json,sys
from pathlib import Path
import verify_movie_transition as v
import verify_boot_wait as wait
file=v.previous.previous.previous.previous.previous
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def coverage_rows(coverage,edges):
 rows=[]
 models=((v.previous.previous,v.previous.previous.routines),(v.previous,v.previous.manifest['routines']),(v,v.manifest['routines']))
 for model,routines in models:
  for routine in routines:
   body=model.PER_BODY[int(routine['address'],16)];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
   rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 return rows

def main():
 ram=file.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();boot_rows=set()
 selected=[(mode,renderer,profile,mode//4*2+renderer-1) for mode,renderer,profile in itertools.product((0,4),(1,2),('elapsed','exhausted'))]+[(mode,1,'finite',0) for mode in (0,4)]
 if '--smoke' in sys.argv:selected=[(0,1,profile,0) for profile in ('elapsed','exhausted')]
 for mode,renderer,profile,signal in selected:
  c=dict(entry=wait.boot.BOOT,mode=mode,renderer=renderer,existing=0,chain=[],gate=1,pending=1,active=True,command_ticks=8,timeout_b=4,ticks=[0,0,20000000]+list(range(20000001,20000300)),names=[b'MOVIE.BIN'],sizes=[184592],sector_start=510488)
  c['clock_profile']=profile;c['port_data']=0xa055|(signal<<8)
  c['scan_samples']=[] if profile=='finite' else [0x400] if profile=='exhausted' else [0x101ff,0x400,0x80000000,0x10001]
  if profile!='finite':c['ticks']=[0,0,20000000]+list(range(20000001,20000250))+[40000250]+list(range(40000251,40000550))
  original,_,_=file.previous.previous.previous.previous.prepare(ram,dict(c));m,s,f=v.prepare(ram,c)
  m[0x8a3380:0x8a3400]=original[0x8a3380:0x8a3400];file.store(m,file.load(m,0x8c60521c),file.previous.previous.CHANNEL);file.put16(m,file.FD+0x4c,0)
  assert file.text(m,0x8c160064)==b'MOVIE.BIN'
  record,path,a,s,f=v.compare(m,s,f,c)
  print(mode,renderer,record['status'],record['stop_pc'],record['disc_read_attempts'],len(path),flush=True)
  reads=[b for b in record['boundaries'] if int(b['target'],16)==file.GDREAD];assert len(reads)>=1
  assert reads[0]['arguments'][:2]==[510488,91] and record['disc_read_attempts']>=91
  dest=reads[0]['arguments'][2]&0xffffff;entry_boundary=next(b for b in record['boundaries'] if b['target']=='0x8ceb0000');assert entry_boundary['loaded_movie_image_sha256']=='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'
  assert entry_boundary['movie_entry_live_requests']==0 and entry_boundary['movie_entry_read_attempts']==91
  assert all(read==[21,v.previous.PORT,c['port_data'],2] for read in record['video_reads'])
  assert bool(record['video_reads'])==(profile!='finite')
  assert record['port_data']==c['port_data']
  assert record['ccr_state']==(0,1,0) and record['ccr_events']==[[20,0xff00001c,0,4],[19,0xff00001c,0x800,4]]
  assert file.load(a,file.load(a,0x8ceb0180))==(0 if entry_boundary['arguments'][0]==7 else 5)
  if profile!='finite':assert file.load(a,file.load(a,0x8c6088b8))==0xffffffff
  assert record['scan_cursor']==(0 if profile=='finite' else len(c['scan_samples']))
  assert record['scan_samples_unchanged']
  assert record['scan_reads']==[[8,v.SCAN,value,4] for value in c['scan_samples']]
  if profile=='exhausted':assert record['status']==-4 and record['stop_pc']=='0x8c60459e'
  for j,pc in enumerate(path):
   if pc not in v.ROWS:continue
   word=int.from_bytes(a[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if word>>8 in (0x8d,0x8f) else 1
   if word>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
  record['independent_checks']=16;record['loaded_movie_sha256']=entry_boundary['loaded_movie_image_sha256'];record['load_destination_offset']=hex(dest);records.append(record);coverage.update(path);boot_rows.update(set(path)&set(wait.boot.geometry.PER_BODY[wait.boot.BOOT]))
 files=[Path(__file__),Path(v.__file__),v.P/'boot_disc_reference.py',v.MP,v.P/'analysis/movie_transition_connection.json']
 report=dict(cases_passed=len(records),independent_checks=len(records)*16,boot_rows_covered=len(boot_rows),boot_static_rows=51,boot_recovery_credit=0,status_counts={str(code):sum(r['status']==code for r in records) for code in set(r['status'] for r in records)},next_dependencies=sorted(set(r['stop_pc'] for r in records)),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Ten uninterrupted boot profiles cross both prior renderers/FPU modes, advancing or exhausted raw scan-status inputs and finite-clock controls. All four cable statuses are represented across the renderer/FPU pairs. Original disc metadata/request/loader/close, module entry bytes and real startup stores are retained. Full reached/final machine, RAM, disc, cache and software peripheral state match. Samples supply raw status observations, never a completed game-service result.',limitations=['Startup tables and prior software peripheral state remain fixtures. Native disc/coherent-cache/CCR behavior is declared platform policy. No natural boot, physical BIOS/devices, gameplay or playable port is implied.'],cases=records)
 def encode(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 report['coverage_by_routine']=coverage_rows(coverage,edges)
 if '--smoke' in sys.argv:print('Display wait boot smoke and report assembly passed',len(records));return
 (v.P/'analysis/movie_transition_prefix_verification.json').write_text(json.dumps(report,indent=2,default=encode)+'\n');print({k:report[k] for k in ('cases_passed','boot_rows_covered','next_dependencies')})
if __name__=='__main__':main()
