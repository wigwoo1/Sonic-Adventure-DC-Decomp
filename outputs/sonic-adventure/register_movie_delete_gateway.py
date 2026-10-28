"""Append only the newly qualified existing delete gateway; preserve old entries."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/movie_delete_gateway_qualification.json';q=json.loads(qp.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['executed_rows'])==(1,92,89)
    for pin,h in q['sha256'].items():assert digest(pin)==h,pin
    rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1545];assert len(previous)==1545
    for routine in q['routines']:
        row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=qp.relative_to(P).as_posix(),status='Existing original delete gateway qualified in declared ABI/raw-ACK domain:89 executed rows plus3 bytechecked canonical-return impossible rows. Full filesystem/game unfinished.',limitations=q['limitations'])
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
        if old is None:registry['routines'].append(row)
        else:assert old==row
    assert len(registry['routines'])==1546 and registry['routines'][:1545]==previous and len({(r.get('module','1ST_READ.BIN'),r['address']) for r in registry['routines']})==1543
    rp.write_text(json.dumps(registry,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('routines','sha256')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_DELETE_GATEWAY.txt');status['verified_progress']['movie_delete_gateway_integration']=data
    status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1546;sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1546/unique1543;goalactive10%',flush=True)
if __name__=='__main__':main()
