"""Add only the four behavior-qualified request bodies; retain previous proofs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=P/'analysis/movie_channel_request_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(4,89,4)
 for pin,h in q['sha256'].items():assert digest(pin)==h,pin
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1489];assert len(previous)==1489
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=path.relative_to(P).as_posix(),status='Original lock/request/packetbody qualified in coherentRAM single guestexecutor domain;actualpacketbuilder/diagnostics execute. Responsepump/headerparse remain incomplete.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1493 and registry['routines'][:1489]==previous
 pump=next(r for r in previous if r['address']=='0x8c646654' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN');assert pump['source']=='source/sa_input_service_cleanup_8c646654.c'
 for address in ('0x8c6608a4','0x8c646982','0x8c6469c8','0x8c647980'):assert not any(r['address']==address and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry['routines'])
 rp.write_text(json.dumps(registry,indent=2)+'\n');sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','sha256','coverage_by_routine')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_CHANNEL_REQUEST.txt');status['verified_progress']['movie_channel_request_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1493;sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1493/unique1490;responsepump/parser still incomplete')
if __name__=='__main__':main()
