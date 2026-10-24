"""Uninterrupted boot reaches its actual MOVIE.BIN firmware read boundary."""
import hashlib,itertools,json
from pathlib import Path
import verify_boot_file as v
import verify_boot_wait as wait

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 inventory=v.PROJECT.parents[1]/'work/sonic-adventure/extracted/disc_inventory.json'
 movie=next(r for r in json.loads(inventory.read_text())['files'] if r['path']=='SONICADV/MOVIE.BIN')
 assert movie['size']==184592 and movie['lba']==510338
 ram=v.previous.previous.previous.previous.previous.seed_ram();records=[];boot_rows=set()
 for mode,renderer in itertools.product((0,4),(1,2)):
  c=dict(entry=wait.boot.BOOT,mode=mode,renderer=renderer,existing=0,chain=[],gate=1,pending=1,active=True,command_ticks=8,timeout_b=4,ticks=[0,0,20000000]+list(range(20000001,20000300)),names=[b'MOVIE.BIN'],sizes=[movie['size']])
  original,_,_=v.previous.previous.previous.previous.prepare(ram,dict(c));m,s,f=v.prepare(ram,c)
  m[0x8a3380:0x8a3400]=original[0x8a3380:0x8a3400];v.store(m,v.load(m,0x8c60521c),v.previous.previous.CHANNEL)
  v.put16(m,v.FD+0x4c,0);v.store(m,v.DIRECTORY+16,movie['lba'])
  assert v.text(m,0x8c160064)==b'MOVIE.BIN'
  record,path,a,s,f=v.compare(m,s,f,c)
  assert record['status']==-2 and record['stop_pc']==hex(v.BIOS)
  targets={int(b['target'],16) for b in record['boundaries']};assert {v.THUNK,v.LOAD,v.VOPEN,v.OPEN,v.SIZE,v.READ,v.GDREAD}<=targets
  assert v.load(a,v.FD+8)==movie['lba']+150 and v.load(a,v.FD+12)==movie['size']
  reads=[b for b in record['boundaries'] if int(b['target'],16)==v.GDREAD];assert len(reads)==1
  assert reads[0]['arguments'][0]==movie['lba']+150 and reads[0]['arguments'][1]==91
  record['independent_checks']=8;records.append(record);boot_rows.update(set(path)&set(wait.boot.geometry.PER_BODY[wait.boot.BOOT]))
 files=[Path(__file__),Path(v.__file__),inventory,v.MANIFEST,v.PROJECT/'analysis/boot_file_connection.json']
 report=dict(cases_passed=len(records),independent_checks=32,boot_rows_covered=len(boot_rows),boot_static_rows=51,boot_recovery_credit=0,movie_metadata=movie,firmware_boundary=hex(v.BIOS),external_game_service_fixtures=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Four uninterrupted outer boot prefixes execute the original MOVIE.BIN request without changing its name or manufacturing an I/O result. Directory metadata uses the extracted disc inventory. The actual read reaches the unresolved firmware vector with FAD 510488 and 91 sectors. Runtime driver/filesystem/directory/handle tables and prior peripheral snapshots remain explicit fixtures.',limitations=['No physical disc bytes are transferred and no firmware completion, natural initialization, IRQ delivery or playable boot is claimed.','The parent file loader and outer boot remain partial. Existing native disc queue/sector-reader/service code is the next reusable platform dependency.'],cases=records)
 def encode(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 (v.PROJECT/'analysis/boot_file_prefix_verification.json').write_text(json.dumps(report,indent=2,default=encode)+'\n')
 print({k:report[k] for k in ('cases_passed','independent_checks','boot_rows_covered','firmware_boundary')})

if __name__=='__main__':main()
