"""Register verified readiness only; keep reader/request/continuations bounded."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_channel_io_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(1,38,6)
 for pin,h in q['sha256'].items():assert digest(pin)==h,pin
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1488];assert len(previous)==1488
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=path.relative_to(P).as_posix(),status='Qualified original readinessbody38rows/sixoutcomes;actualnestedIO-failure/task-resumption graphverifiedseparately;activeparser/requestbackendremainpartial.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1489 and registry['routines'][:1488]==previous
 for address in ('0x8c6608a4','0x8c646982','0x8c6469c8','0x8c660494','0x8c6604e6','0x8c6604c0','0x8c660518','0x8c647980'):
  assert not any(r['address']==address and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry['routines'])
 rp.write_text(json.dumps(registry,indent=2)+'\n');sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','sha256','coverage_by_routine','canonical_reuse')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_CHANNEL_IO.txt');status['verified_progress']['movie_channel_io_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1489;sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1489/unique1486;reader/requestbackendremainpartial')
if __name__=='__main__':main()
