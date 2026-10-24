"""Continue actual boot after its original MOVIE.BIN request reaches host I/O."""
import hashlib,itertools,json
from pathlib import Path
import verify_boot_disc as v
import verify_boot_wait as wait
file=v.previous
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=file.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();boot_rows=set()
 for mode,renderer in itertools.product((0,4),(1,2)):
  c=dict(entry=wait.boot.BOOT,mode=mode,renderer=renderer,existing=0,chain=[],gate=1,pending=1,active=True,command_ticks=8,timeout_b=4,ticks=[0,0,20000000]+list(range(20000001,20000300)),names=[b'MOVIE.BIN'],sizes=[184592],sector_start=510488)
  original,_,_=file.previous.previous.previous.previous.prepare(ram,dict(c));m,s,f=v.prepare(ram,c)
  m[0x8a3380:0x8a3400]=original[0x8a3380:0x8a3400];v.store(m,v.load(m,0x8c60521c),file.previous.previous.CHANNEL);file.put16(m,file.FD+0x4c,0)
  assert file.text(m,0x8c160064)==b'MOVIE.BIN'
  record,path,a,s,f=v.compare(m,s,f,c)
  print(mode,renderer,record['status'],record['stop_pc'],record['disc_read_attempts'],len(path),flush=True)
  reads=[b for b in record['boundaries'] if int(b['target'],16)==file.GDREAD];assert len(reads)==1
  assert reads[0]['arguments'][:2]==[510488,91] and record['disc_read_attempts']==91
  dest=reads[0]['arguments'][2]&0xffffff;payload=bytes(a[dest:dest+184592]);assert hashlib.sha256(payload).hexdigest()=='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'
  assert file.word16(a,file.FD+0x4c)==0 and not any(j[1] for j in record['disc_snapshot'][7])
  record['independent_checks']=7;record['loaded_movie_sha256']=hashlib.sha256(payload).hexdigest();record['load_destination_offset']=hex(dest);records.append(record);coverage.update(path);boot_rows.update(set(path)&set(wait.boot.geometry.PER_BODY[wait.boot.BOOT]))
 files=[Path(__file__),Path(v.__file__),v.P/'boot_disc_reference.py',v.MP,v.P/'analysis/boot_disc_connection.json']
 report=dict(cases_passed=len(records),independent_checks=28,boot_rows_covered=len(boot_rows),boot_static_rows=51,boot_recovery_credit=0,status_counts={str(code):sum(r['status']==code for r in records) for code in set(r['status'] for r in records)},next_dependencies=sorted(set(r['stop_pc'] for r in records)),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Four uninterrupted boot prefixes use original request/name, actual disc metadata, native host sector I/O and the original loader/close chain. Full reached/final machine, RAM, disc and software peripheral state match. The loaded 184592-byte MOVIE image matches the extracted file hash.',limitations=['Startup tables and prior software peripheral state remain fixtures. Native disc/coherent-cache behavior is declared platform policy. No natural boot, physical BIOS/devices, gameplay or playable port is implied.'],cases=records)
 def encode(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 (v.P/'analysis/boot_disc_prefix_verification.json').write_text(json.dumps(report,indent=2,default=encode)+'\n');print({k:report[k] for k in ('cases_passed','boot_rows_covered','next_dependencies')})
if __name__=='__main__':main()
