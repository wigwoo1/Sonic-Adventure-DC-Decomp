"""Continue actual boot after its original MOVIE.BIN request reaches host I/O."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_entry as v
import verify_boot_wait as wait
file=v.previous.previous.previous
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=file.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();boot_rows=set()
 for mode,renderer,profile in itertools.product((0,4),(1,2),('finite','elapsed')):
  c=dict(entry=wait.boot.BOOT,mode=mode,renderer=renderer,existing=0,chain=[],gate=1,pending=1,active=True,command_ticks=8,timeout_b=4,ticks=[0,0,20000000]+list(range(20000001,20000300)),names=[b'MOVIE.BIN'],sizes=[184592],sector_start=510488)
  c['clock_profile']=profile
  if profile=='elapsed':c['ticks']=[0,0,20000000]+list(range(20000001,20000250))+[40000250]+list(range(40000251,40000550))
  original,_,_=file.previous.previous.previous.previous.prepare(ram,dict(c));m,s,f=v.prepare(ram,c)
  m[0x8a3380:0x8a3400]=original[0x8a3380:0x8a3400];file.store(m,file.load(m,0x8c60521c),file.previous.previous.CHANNEL);file.put16(m,file.FD+0x4c,0)
  assert file.text(m,0x8c160064)==b'MOVIE.BIN'
  record,path,a,s,f=v.compare(m,s,f,c)
  print(mode,renderer,record['status'],record['stop_pc'],record['disc_read_attempts'],len(path),flush=True)
  reads=[b for b in record['boundaries'] if int(b['target'],16)==file.GDREAD];assert len(reads)>=1
  assert reads[0]['arguments'][:2]==[510488,91] and record['disc_read_attempts']>=91
  dest=reads[0]['arguments'][2]&0xffffff;entry_boundary=next(b for b in record['boundaries'] if b['target']=='0x8ceb0000');assert entry_boundary['loaded_movie_image_sha256']=='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'
  assert entry_boundary['movie_entry_live_requests']==0 and entry_boundary['movie_entry_read_attempts']==91
  assert record['ccr_state']==(0,1,0) and record['ccr_events']==[[20,0xff00001c,0,4],[19,0xff00001c,0x800,4]]
  assert file.load(a,file.load(a,0x8ceb0180))==(0 if entry_boundary['arguments'][0]==7 else 5)
  if profile=='elapsed':assert file.load(a,file.load(a,0x8c6088b8))==0xffffffff
  for j,pc in enumerate(path):
   if pc not in v.ROWS:continue
   word=int.from_bytes(a[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if word>>8 in (0x8d,0x8f) else 1
   if word>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
  record['independent_checks']=9;record['loaded_movie_sha256']=entry_boundary['loaded_movie_image_sha256'];record['load_destination_offset']=hex(dest);records.append(record);coverage.update(path);boot_rows.update(set(path)&set(wait.boot.geometry.PER_BODY[wait.boot.BOOT]))
 files=[Path(__file__),Path(v.__file__),v.P/'boot_disc_reference.py',v.MP,v.MAIN_MP,v.P/'analysis/movie_entry_connection.json']
 report=dict(cases_passed=len(records),independent_checks=72,boot_rows_covered=len(boot_rows),boot_static_rows=51,boot_recovery_credit=0,status_counts={str(code):sum(r['status']==code for r in records) for code in set(r['status'] for r in records)},next_dependencies=sorted(set(r['stop_pc'] for r in records)),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Eight uninterrupted boot prefixes with finite/exhausted and explicit elapsed-time clock samples use original request/name, actual disc metadata, native host sector I/O and the original loader/close chain. Full reached/final machine, RAM, disc and software peripheral/CCR state match. The 184592-byte MOVIE image at its original entry boundary matches the extracted file hash; module startup then executes its real stores.',limitations=['Startup tables and prior software peripheral state remain fixtures. Native disc/coherent-cache/CCR behavior is declared platform policy. No natural boot, physical BIOS/devices, gameplay or playable port is implied.'],cases=records)
 def encode(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 report['coverage_by_routine']=[]
 for routine in v.routines:
  body=v.PER_BODY[int(routine['address'],16)];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  report['coverage_by_routine'].append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 (v.P/'analysis/movie_entry_prefix_verification.json').write_text(json.dumps(report,indent=2,default=encode)+'\n');print({k:report[k] for k in ('cases_passed','boot_rows_covered','next_dependencies')})
if __name__=='__main__':main()
