"""Extend original initializer evidence; keep incomplete firmware paths explicit."""
import csv,ctypes,hashlib,json,types
from pathlib import Path
import verify_movie_disc_paths as v
import disc_mount_platform_reference as media
P=v.P;ROOT=v.ROOT;INIT=v.old.INIT
MEDIA=P/'analysis/movie_mount_media.json';PROFILES=json.loads(MEDIA.read_text())['profiles']
platform=types.SimpleNamespace(**v.platform.__dict__);platform.DiscReference=media.DiscReference
compare=types.FunctionType(v.compare.__code__,dict(v.compare.__globals__,platform=platform),'compare')

def main():
 ram=v.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram()
 prior_path=P/'analysis/movie_disc_init_verification.json';prior=json.loads(prior_path.read_text());baseline=next(x for x in prior['coverage_by_routine'] if x['address']=='0x8c600038')
 body=set(v.PER_BODY[INIT]);coverage=body-{int(a,16) for a in baseline['missing_rows']}
 translation=json.loads((P/'analysis/movie_disc_init_translation.json').read_text());routine=next(r for r in translation['routines'] if int(r['address'],16)==INIT)
 wanted={tuple(int(a,16) for a in pair) for pair in routine['conditional_edges']};edges=wanted-{tuple(int(a,16) for a in pair) for pair in baseline['missing_edges']}
 records=[];checks=0
 expected={'normal':(0,2),'bad_signature':(-5,1),'empty_root':(-5,2),'root_full':(-12,2),'special':(0,3),'special_twelve':(0,3),'special_missing':(-4,2),'special_empty_prefix':(-4,2),'special_empty_child':(-5,3),'special_child_full':(-12,3),'special_fail_0':(-4,1),'special_fail_1':(-4,2),'special_fail_2':(-4,3)}
 for name,row in PROFILES.items():
  c=dict(entry=INIT,mode=0,rtc_samples=[],fifo_samples=[0]*128,scan_samples=[0,1]*32,slots=2,capacity=row['capacity'],mount_profile=name,fail_after=row['fail_after'])
  m,s,f=v.old.prepare(ram,c);v.store(m,0x8c66c530,row['volume_fad'])
  prefix=bytes(row['prefix']);header=prefix.ljust(12,b' ')+bytes((1 if row['special'] else 0,2))
  v.write_bytes(m,0x8c0080f0,header)
  before=v.effects.State.from_buffer_copy(s)
  record,path,a,s,f=compare(m,s,f,c);result=ctypes.c_int32(s.r[0]).value;root=v.load(a,0x8c673414);flag=int.from_bytes(v.sample(a,root+0x38,2),'little')
  assert record['status']==0 and record['full_state_match'] and record['ordered_boundaries_match'];checks+=3
  assert (result,record['disc_read_attempts'])==expected[name],(name,result,record['disc_read_attempts']);checks+=2
  assert root==v.old.CTX and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
  assert flag==int(result==0);checks+=1
  assert v.load(a,root)==2 and v.load(a,root+0x14)==root+0x1080 and v.load(a,root+0x18)==root+0x10d4;checks+=3
  if result==0:
   assert v.load(a,root+4)==45158 and v.load(a,root+8)==row['volume_fields']['132'];checks+=2
   table=v.load(a,root+16);block=3 if row['special'] else 2
   assert v.load(a,table)==len(row['directories'][str(block)]);checks+=1
   if row['special']:
    selected=v.load(a,0x8c88e970);assert v.sample(a,selected,4)==b'\1\0\2\0' and v.sample(a,selected+8,len(prefix)+1)==prefix+b'\0' and v.load(a,selected+4)==45153;checks+=3
  coverage.update(set(path)&body)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  record.update(guest_r0=hex(s.r[0]),filesystem_root=hex(root),initialized_flag=flag);records.append(record)
  print(name,result,record['disc_read_attempts'],flush=True)
 files=[Path(__file__),Path(media.__file__),P/'prepare_movie_mount_media.py',MEDIA,prior_path,P/'analysis/movie_disc_paths_qualification.json',P/'analysis/movie_disc_init_translation.json',P/'source/sa_movie_disc_init.c',P/'source/native_disc_queue.c']
 report=dict(cases_passed=len(records),independent_checks=checks,initializer_address=hex(INIT),static_rows=len(body),combined_covered_rows=len(coverage),newly_executed_rows=len(coverage)-baseline['covered_rows'],combined_executed_outcomes=len(wanted&edges),missing_rows=[hex(a) for a in sorted(body-coverage)],missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)],qualified=False,new_registry_credit=0,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Retained earlier initializer evidence plus direct original/native full-state comparisons through invented media and explicit input header/global-sector preconditions. No native service result or original helper result is replaced. Initializer remains partial; firmware error paths are not proved unreachable. Existing 171-source DLL/source/proofs unchanged. No natural boot or gameplay claim.')
 (P/'analysis/movie_mount_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Initializer',len(coverage),'/',len(body),'rows;',len(wanted&edges),'/',len(wanted),'outcomes;',checks,'checks')
if __name__=='__main__':main()
