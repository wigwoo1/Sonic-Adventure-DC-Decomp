"""Append only five qualified original bodies and preserve all previous rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=P/'analysis/movie_slot_initialize_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(5,199,30)
 for pinned,h in q['sha256'].items():assert digest(pinned)==h,pinned
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1457]
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=path.relative_to(P).as_posix(),status='Qualified fixed-record initialization and input status queries: five own bodies,199 executed rows,28 executed/2 proved infeasible outcomes. Actual original helpers/metadata providers;1015 fresh comparisons in210-source DLL. Canonical clear/decoder adapters receive zero extra credit.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1462 and registry['routines'][:1457]==previous;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';s=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','sha256','coverage_by_routine')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_SLOT_INITIALIZE.txt');s['verified_progress']['movie_slot_initialize_integration']=data;s['verified_progress']['recovered_buildable_routines']=s['recovered_buildable_routines']=1462;sp.write_text(json.dumps(s,indent=2)+'\n');print('REGISTERED1462')
if __name__=='__main__':main()
