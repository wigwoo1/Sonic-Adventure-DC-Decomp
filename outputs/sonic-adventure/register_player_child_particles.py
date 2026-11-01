"""Append two scoped particle originals while preserving prior registry rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/player_child_particles_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2
 for path,value in q['sha256'].items():assert sha(path)==value,path
 rp=P/'analysis/recovered_routines.json';reg=json.loads(rp.read_text());prior=reg['routines'][:];assert len(prior)==1640
 for b in q['routines']:
  assert not any(x.get('module','1ST_READ.BIN')==b['module'] and x['address']==b['address'] for x in prior)
  row={k:b[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=qp.relative_to(P).as_posix(),status='Complete original particle spawning or draw wrapper software-qualified in documented domains;347group rows/14outcomes execute. Actual spawning/exhaustion/lifecycle andclipped/suppressed draw match. Visible textured rendering stops atoriginal uninitializeddescriptor606a9e;retail asset-loader integration remains unfinished.',limitations=q['limitations']);reg['routines'].append(row)
 assert len(reg['routines'])==1642 and reg['routines'][:1640]==prior and len({(x.get('module','1ST_READ.BIN'),x['address']) for x in reg['routines']})==1639
 rp.write_text(json.dumps(reg,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routines')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=sha(qp),guide='PLAYER_CHILD_PARTICLES.txt');status['verified_progress']['player_child_particles']=data
 status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1642
 sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1642entries/1639unique/686confirmedlinks;goalactive',flush=True)
if __name__=='__main__':main()
