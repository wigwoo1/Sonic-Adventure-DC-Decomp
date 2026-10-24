"""Input coverage qualification and explicit stable-bitmap failure-path proof."""
import hashlib,json,random
from pathlib import Path
import verify_boot_input as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 proof_path=v.PROJECT/'analysis/boot_input_verification.json';proof=json.loads(proof_path.read_text())
 assert proof['cases_passed']==407 and proof['status_counts']=={'0':397,'-3':10}
 for path,h in proof['sha256'].items():assert digest(path)==h,path
 assert digest(v.effects.DLL)==proof['dll_sha256']
 # CAP tests bitmap bit zero; READ requests the same bit, then LOCATE scans
 # 80000000 down to bit zero. Distinct stack/data and no concurrent writer
 # imply LOCATE cannot return zero in that reached call. READ returns zero.
 cap=v.PER_BODY[v.CAPABILITIES];read=v.PER_BODY[v.READ];loc=v.PER_BODY[v.LOCATE];ram=v.previous.previous.previous.seed_ram()
 for a,t in {0x8c601db0:'mov.l @r13,r0',0x8c601db2:'tst #0x1,r0',0x8c601db4:'bt/s 0x8c601dcc',0x8c601dba:'mov r13,r5',0x8c601dbc:'jsr @r2',0x8c601dc0:'cmp/pz r0',0x8c601dc2:'bt 0x8c601dcc'}.items():assert cap[a]==t,(hex(a),cap[a])
 assert v.load(ram,0x8c601e48)==v.READ and read[0x8c646b7a]=='mov #0x1,r5' and read[0x8c646b7c]=='bsr 0x8c646b42'
 assert read[0x8c646b7e]=='mov.l @(0x4,r15),r4' and read[0x8c646b78]=='mov.l r5,@(0x4,r15)'
 assert loc[0x8c646b4a]=='mov.l @r4+,r12' and loc[0x8c646b48]=='mov #0x20,r0'
 assert v.load(ram,0x8c646c3c)==0x80000000 and loc[0x8c646b66]=='shlr r6' and loc[0x8c646b56]=='cmp/eq r6,r5'
 assert loc[0x8c646b5a]=='mov r7,r0' and read[0x8c646bb0]=='mov #0x0,r0'
 rng=random.Random(0x601da6);bitmaps=[1<<i for i in range(32)]+[rng.getrandbits(32)|1 for _ in range(64)];records=[]
 for bitmap in bitmaps:
  c=dict(entry=v.LOCATE,mode=0,features=bitmap,bit=1);m,s,f=v.prepare(ram,c)
  record,path,a,s,f=v.compare(m,s,f,c);assert record['status']==0
  expected=v.info(m,0)+4+4*((bitmap&0xfffffffe).bit_count()) if bitmap&1 else 0
  assert s.r[0]==expected and (not bitmap&1 or s.r[0]!=0)
  record['independent_checks']=2;records.append(record)
 rows=[];dead_rows={0x8c601dc4,0x8c601dc6,0x8c601dc8,0x8c601dca};dead_edge={(0x8c601dc2,0x8c601dc4)}
 for old in proof['coverage_by_routine']:
  row=dict(old);address=int(row['address'],16)
  if address==v.CAPABILITIES:
   assert {int(x,16) for x in row['missing_rows']}==dead_rows and {tuple(int(x,16) for x in e) for e in row['missing_edges']}==dead_edge
   row['structurally_unreachable_rows']=list(row['missing_rows']);row['structurally_unreachable_edges']=list(row['missing_edges'])
  else:assert not row['missing_rows'] and not row['missing_edges'],row
  row['unexplained_missing_rows']=[];row['unexplained_missing_edges']=[];rows.append(row)
 files=[Path(__file__),proof_path,v.PROJECT/'analysis/boot_input_sequence_verification.json',v.PROJECT/'analysis/boot_input_lifecycle_verification.json']
 for p in list(files[1:]):
  obj=json.loads(p.read_text())
  for path,h in obj['sha256'].items():assert digest(path)==h,path
  files.extend(Path(path) for path in obj['sha256'])
 report=dict(source=proof['source'],source_sha256=proof['source_sha256'],routines=proof['routines'],cases_passed=503,status_counts={'0':493,'-3':10},independent_checks=proof['independent_checks']+192,coverage_by_routine=rows,executed_adapter_rows=704,static_adapter_rows=708,structurally_unreachable_rows=4,external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=proof['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=proof['scope'],limitations=proof['limitations']+['Four original capability-read error rows and their one edge remain explicitly unexecuted. With a stable bit-zero-present bitmap, the nested locator/read cannot return failure; 96 independent original locator comparisons validate pointer packing. Concurrent driver mutation is not modeled, and those four rows receive no executed-row credit.'],stable_bitmap_cases=records)
 (v.PROJECT/'analysis/boot_input_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified six new input bodies; 503 comparisons; 704 executed rows and four explicitly unreachable error rows')

if __name__=='__main__':main()
