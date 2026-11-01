"""Append four qualified originals without altering prior registry rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/player_child_records_and_extent_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==4
 for path,value in q['sha256'].items():assert sha(path)==value,path
 rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:];assert len(prior)==1636
 for b in q['routines']:
  assert not any(x.get('module','1ST_READ.BIN')==b['module'] and x['address']==b['address'] for x in prior)
  row={k:b[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=qp.relative_to(P).as_posix(),status='Complete original collision extent or child record allocation/unlink/cleanup software-qualified in declared domains;all142group rows/22outcomes execute. Actual collision attachment andstored child update lifecycles andgenuine exhaustion match. Complete attachment/effect/rendering andretail startup remain unfinished.',limitations=q['limitations']);reg['routines'].append(row)
 assert len(reg['routines'])==1640 and reg['routines'][:1636]==prior
 assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1637
 rp.write_text(json.dumps(reg,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_CHILD_RECORDS_AND_EXTENT.txt')
 status['verified_progress']['player_child_records_and_extent']=data
 status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1640
 sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1640entries/1637unique/684confirmedlinks;goal active',flush=True)
if __name__=='__main__':main()
