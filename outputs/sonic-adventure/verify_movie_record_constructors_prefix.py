"""Uninterrupted original caller, real cold filesystem and MASARU driver reads."""
import csv,hashlib,json,sys
from pathlib import Path
import verify_movie_record_constructors as v
P=v.P;ROOT=v.ROOT;DRIVER=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV'
EXPECTED='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 driver=DRIVER.read_bytes();assert len(driver)==33408 and hashlib.sha256(driver).hexdigest()==EXPECTED
 ram=v.seed_ram()
 base=next(c for c in v.old.old.cases() if c['entry']==0x8c0546c0)
 records=[];coverage=set();edges=set();checks=0
 for cold in ((False,) if '--warm' in sys.argv else (False,True)):
  c=base|dict(cold_filesystem=cold,progress=True)
  m,s,f=v.old.prepare(ram,c);before_root=v.load(m,0x8c673414);before_interface=v.load(m,0x8c673420)
  assert (before_root==0 and before_interface==0) if cold else before_root==0x8cc80000;checks+=1
  print('START',('cold original filesystem' if cold else 'retained prepared filesystem'),flush=True)
  record,path,a,s,f=v.compare(m,s,f,c)
  assert record['status']==-2 and record['stop_pc']=='0x8c65a2fc';checks+=2
  assert record['full_state_match'] and record['ordered_boundaries_match'];checks+=2
  assert record['clear_native']['writes']==record['clear_reference']['writes']==0x200000 and record['full_vram_bytes_compared']==0x800000;checks+=2
  init=[b for b in record['boundaries'] if b['target']=='0x8c010180'];assert len(init)==1 and init[0]['arguments']==[1,0,1,3];checks+=2
  opened=[b for b in record['boundaries'] if b['target']=='0x8c6016d6'];assert opened and v.old.sample(m,opened[-1]['arguments'][0],11)==b'masaru.drv\0';checks+=2
  root=v.load(a,0x8c673414);flag=int.from_bytes(v.old.sample(a,root+0x38,2),'little')
  payload=v.old.sample(a,0x0cd00000,len(driver));payload_hash=hashlib.sha256(payload).hexdigest()
  if cold:
   assert root==0x8c6946e0 and flag==1;checks+=2
   assert record['disc_read_attempts']==89 and payload==driver;checks+=2
   stat=[b for b in record['boundaries'] if b['target']=='0x8c600e2c'];assert stat and stat[-1]['arguments'][0]!=0;checks+=1
   reads=[b for b in record['boundaries'] if b['target']=='0x8c600f1e'];assert reads and reads[-1]['arguments'][1:3]==[17,0x0cd00000];checks+=1
  else:
   assert root==before_root and not record['disc_read_attempts'] and payload_hash!=EXPECTED;checks+=3
  record.update(filesystem_root_before=hex(before_root),filesystem_root_after=hex(root),filesystem_initialized_flag=flag,masaru_sha256_at_stop=payload_hash,masaru_original_sha256=EXPECTED,masaru_payload_matches=payload_hash==EXPECTED,masaru_bytes=33408)
  records.append(record);coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  print('DONE',cold,record['disc_read_attempts'],payload_hash,record['boundary_comparisons'],flush=True)
 files=[Path(__file__),Path(v.__file__),v.MP,P/'analysis/movie_record_constructors_connection.json',P/'analysis/movie_init_records_qualification.json',DRIVER]
 rows=[]
 for routine in v.manifest['routines']:
  body=set(v.PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 report=dict(cases_passed=len(records),independent_checks=checks,cold_profile_reexecuted='--warm' not in sys.argv,masaru_bytes=33408,masaru_sha256=EXPECTED,coverage_by_routine=rows,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},cases=records,scope='Uninterrupted original 8c0546c0 caller with native console/flash input spans, original early initializer and real track transport. The cold profile explicitly zeros filesystem/interface roots before original producers construct them; it is a new full original/native comparison, not reused old cold state. The warm prepared-filesystem control retains its actual missing-file behavior. Both stop at the actual missing original 8c65a2fc. No helper, descriptor, read or driver result is invented; physical reset, device execution and complete driver/game behavior remain unfinished.')
 name='movie_record_constructors_prefix_warm_verification.json' if '--warm' in sys.argv else 'movie_record_constructors_prefix_verification.json'
 (P/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n');print('Original caller/driver prefix',len(records),'comparisons',checks,'checks')
if __name__=='__main__':main()
