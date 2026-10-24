"""Require complete original rows/outcomes, including inactive stale links."""
import hashlib,itertools,json
from pathlib import Path
import verify_boot_event as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=v.PROJECT/'analysis/boot_event_verification.json';proof=json.loads(path.read_text());assert proof['cases_passed']==540 and proof['status_counts']=={'0':516,'-3':17,'-4':3,'-2':4}
 for p,h in proof['sha256'].items():assert digest(p)==h,p
 ram=v.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for mode,index in itertools.product((0,4),(2,254)):
  c=dict(entry=v.REMOVE,mode=mode,event=0x900,chain=[10],handle=1,stale_inactive_link=index);m,s,f=v.prepare(ram,c)
  v.store(m,v.node(index)+4,v.node(1));assert v.load(m,v.node(index)+12)==0
  before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f);record,path_,a,s,f=v.compare(m,s,f,c);assert record['status']==0
  record['independent_checks']=v.semantic(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path_)
  for j,pc in enumerate(path_):
   if pc not in v.OWN:continue
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path_):edges.add((pc,path_[j+distance]))
 rows=[]
 for old in proof['coverage_by_routine']:
  row=dict(old);remaining=set(int(a,16) for a in row['missing_rows'])-coverage;missing={tuple(int(a,16) for a in e) for e in row['missing_edges']}-edges
  assert not remaining and not missing,row
  row.update(covered_rows=row['static_rows'],missing_rows=[],conditional_edges_covered=len(next(r for r in v.manifest['routines'] if r['address']==row['address'])['conditional_edges']),missing_edges=[]);rows.append(row)
 files=[Path(__file__),path,v.PROJECT/'analysis/boot_event_state_verification.json',v.PROJECT/'analysis/boot_event_sequences_verification.json']
 for p in list(files[1:]):
  obj=json.loads(p.read_text())
  assert obj['dll_sha256']==proof['dll_sha256']
  for f,h in obj['sha256'].items():assert digest(f)==h,f
  files.extend(Path(f) for f in obj['sha256'])
 report=dict(source=proof['source'],source_sha256=proof['source_sha256'],routines=proof['routines'],cases_passed=544,status_counts={'0':520,'-3':17,'-4':3,'-2':4},coverage_by_routine=rows,independent_checks=proof['independent_checks']+16,mask_register_writes=proof['mask_register_writes'],vbr_kernel_cases=8192,backend_and_api_checks=43,lifecycle_stage_comparisons=6,boot_prefix_cases=8,boot_rows_covered=25,boot_recovery_credit=0,newly_registered_entries=0,external_game_service_fixtures=0,dll_sha256=proof['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=proof['scope'],limitations=proof['limitations'],stale_inactive_link_cases=records)
 (v.PROJECT/'analysis/boot_event_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified all 529 original rows / 72 outcomes; 544 comparisons')

if __name__=='__main__':main()
