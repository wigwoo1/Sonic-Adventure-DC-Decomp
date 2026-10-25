"""Register closed driver dependencies without replacing older canonical evidence."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_driver_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==10 and q['qualified_rows']==934
 for path,h in q['sha256'].items():assert digest(path)==h,path
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for routine in q['routines']:
  a,b=(int(routine[k],16) for k in ('address','end_exclusive'));assert hashlib.sha256(binary[a-0x8c010000:b-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  row=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],source=routine['source'],retail_span_sha256=routine['retail_span_sha256'],verification=qp.relative_to(P).as_posix(),status='Qualified complete own original body in 90 full-state comparisons plus independent checks and explicit software-platform verification. Driver/reset/clear parents remain partial and receive no credit.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 integration={k:v for k,v in q.items() if k not in ('routines','sha256')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_DRIVER.txt',adapter_source='source/sa_movie_driver_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_driver_connected.c'))
 for address in q['reused_existing_entries']+['0x8c010180']:
  row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);row['movie_driver_integration']=integration
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_driver_integration']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered ten original driver dependencies; entries',len(registry['routines']))
if __name__=='__main__':main()
