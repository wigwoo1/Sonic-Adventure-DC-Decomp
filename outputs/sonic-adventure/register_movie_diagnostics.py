"""Append qualified request-pool bodies; retain canonical helper metadata."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_diagnostics_qualification.json';q=json.loads(qp.read_text());assert q['qualified_rows']==49 and q['newly_registered_entries']==2
 for path,h in q['sha256'].items():assert digest(path)==h,path
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 for routine in q['routines']:
  a,b=(int(routine[k],16) for k in ('address','end_exclusive'));assert hashlib.sha256(raw[a-0x8c010000:b-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  row=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],source=routine['source'],retail_span_sha256=routine['retail_span_sha256'],verification=qp.relative_to(P).as_posix(),status='Qualified complete own original diagnostic wrapper under explicit aligned-stack/callback/format ABI. Four logger rows and one outcome are proven unreachable under that ABI; remaining own rows/outcomes execute in full-state comparisons. Larger callees remain partial.',limitations=q['limitations'])
  previous=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if previous is None:registry['routines'].append(row)
  else:assert previous==row
 integration={k:v for k,v in q.items() if k not in ('routines','sha256')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_DIAGNOSTICS.txt')
 for canonical in q['canonical_reuse']:
  row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==canonical['address'])
  for key in ('name','source','verification'):assert row[key]==canonical[key]
  row['movie_diagnostics_integration']=integration
 assert len(registry['routines'])==1376;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_diagnostics_integration']=integration;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1376;sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered two diagnostic wrappers; 1376 entries; older canonical fields retained')
if __name__=='__main__':main()
