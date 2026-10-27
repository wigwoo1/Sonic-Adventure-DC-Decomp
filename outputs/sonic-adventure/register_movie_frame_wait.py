"""Append the qualified display synchronizer; no credit for partial frame/queue."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_frame_wait_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(1,66,2)
 for pinned,h in q['sha256'].items():assert digest(pinned)==h,pinned
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1471];assert len(previous)==1471
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=path.relative_to(P).as_posix(),status='Qualified original display synchronizer66executed rows/two outcomes,96direct display cases with actual MMIO/buffer helpers;wait/queue/resource frontiers remain explicit. Five canonical adapters get zero extra credit.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1472 and registry['routines'][:1471]==previous
 for a in ('0x8c051790','0x8c055c4c','0x8c608c82','0x8c64c34c','0x8c053ac0'):assert not any(r['address']==a and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry['routines'])
 rp.write_text(json.dumps(registry,indent=2)+'\n');sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','sha256','coverage_by_routine')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_FRAME_WAIT.txt');status['verified_progress']['movie_frame_wait_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1472;sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1472;wait/queue/earliercaller stillpartial')
if __name__=='__main__':main()
