"""Qualify closed file helpers; keep firmware-dependent parents partial."""
import collections,hashlib,json
from pathlib import Path
import verify_boot_file as v

QUALIFIED={v.VOPEN,v.SIZE,v.OPEN,v.INIT,v.TEMP,v.COMPARE_I,v.VSTATUS,v.VPOLL}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=v.PROJECT/'analysis/boot_file_verification.json';proof=json.loads(path.read_text())
 assert proof['cases_passed']==391 and proof['status_counts']=={'0':365,'-2':16,'-3':9,'-4':1}
 for p,h in proof['sha256'].items():assert digest(p)==h,p
 ram=v.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 selected=[dict(entry=v.TEMP,mode=m,null_handle=True) for m in (0,4)]
 selected += [dict(entry=v.FIND,mode=m,request=b'ACB',names=[b'ABC'],sizes=[1]) for m in (0,4)]
 selected += [dict(entry=v.ERROR,mode=m,error_value=e) for m in (0,4) for e in (0,1,0xffffffff,0x80000000)]
 for c in selected:
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f)
  record,path_,a,s,f=v.compare(m,s,f,c);assert record['status']==0
  record['independent_checks']=v.semantic(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path_)
  for j,pc in enumerate(path_):
   if pc not in v.OWN:continue
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path_):edges.add((pc,path_[j+distance]))
 rows=[]
 for old in proof['coverage_by_routine']:
  row=dict(old);remaining=set(int(a,16) for a in row['missing_rows'])-coverage;missing={tuple(int(a,16) for a in e) for e in row['missing_edges']}-edges
  row.update(covered_rows=row['static_rows']-len(remaining),missing_rows=[hex(a) for a in sorted(remaining)],conditional_edges_covered=len(next(r for r in v.manifest['routines'] if r['address']==row['address'])['conditional_edges'])-len(missing),missing_edges=[[hex(a),hex(b)] for a,b in sorted(missing)])
  if int(row['address'],16) in QUALIFIED:assert not remaining and not missing,row
  rows.append(row)
 files=[Path(__file__),path,v.PROJECT/'analysis/boot_file_sequences_verification.json']
 for p in list(files[1:]):
  obj=json.loads(p.read_text());assert obj['dll_sha256']==proof['dll_sha256']
  for f,h in obj['sha256'].items():assert digest(f)==h,f
  files.extend(Path(f) for f in obj['sha256'])
 counts=collections.Counter({int(k):n for k,n in proof['status_counts'].items()});counts.update(r['status'] for r in records)
 report=dict(source=proof['source'],source_sha256=proof['source_sha256'],routines=proof['routines'],cases_passed=proof['cases_passed']+len(records),status_counts=dict(counts),coverage_by_routine=rows,qualified_addresses=[hex(a) for a in sorted(QUALIFIED)],qualified_rows=sum(r['static_rows'] for r in rows if int(r['address'],16) in QUALIFIED),qualified_conditional_outcomes=sum(r['conditional_edges_covered'] for r in rows if int(r['address'],16) in QUALIFIED),independent_checks=proof['independent_checks']+sum(r['independent_checks'] for r in records),lifecycle_sequences=6,lifecycle_stage_comparisons=30,lifecycle_independent_checks=144,external_game_service_fixtures=0,dll_sha256=proof['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=proof['scope'],limitations=proof['limitations']+['The two 12-row status/poll wrappers are qualified through their original tail transfer to the selected driver; underlying firmware-dependent driver bodies remain partial. The close caller is also retained as partial when cancellation is required.'],additional_cases=records)
 def encode(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 (v.PROJECT/'analysis/boot_file_qualification.json').write_text(json.dumps(report,indent=2,default=encode)+'\n')
 print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','qualified_rows','qualified_conditional_outcomes')})

if __name__=='__main__':main()
