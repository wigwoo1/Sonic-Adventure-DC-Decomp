"""Append integration evidence without changing canonical registrations."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_initialize_integration.json';q=json.loads(qp.read_text());assert q['new_registry_credit']==q['reused_additional_credit']==0 and not q['parent_bodies_qualified']
 for path,h in q['sha256'].items():assert digest(path)==h,path
 integration={k:v for k,v in q.items() if k!='sha256'};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_INITIALIZE.txt')
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());before=len(registry['routines'])
 for canonical in q['canonical_reuse']:
  row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==canonical['address'])
  for key in ('name','source','verification'):assert row[key]==canonical[key]
  row['movie_initialize_integration']=integration
 assert len(registry['routines'])==before==1365;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_initialize_integration']=integration;sp.write_text(json.dumps(status,indent=2)+'\n');print('Appended caller integration; retained 1365 entries, zero extra recovery credit')
if __name__=='__main__':main()
