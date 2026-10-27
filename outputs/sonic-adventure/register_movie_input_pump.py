"""Register only freshly qualified response/API bodies; preserve old registry rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=P/'analysis/movie_input_pump_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(9,706,95)
 for pin,h in q['sha256'].items():assert digest(pin)==h,pin
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1493];assert len(previous)==1493
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=path.relative_to(P).as_posix(),status='Originalresponse/APIbody qualifiedwithactualpump/response/callbackcallees in coherentRAMsingle-guest domain;physicalsubmission/naturalgameparserremainunfinished.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1502 and registry['routines'][:1493]==previous
 assert not any(r['address']=='0x8c6608a4' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry['routines'])
 rp.write_text(json.dumps(registry,indent=2)+'\n');sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:value for k,value in q.items() if k not in ('routines','canonical_reuse','sha256','coverage_by_routine','next_canonical_dependencies')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_INPUT_PUMP.txt');status['verified_progress']['movie_input_pump_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1502;sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1502/unique1499;submission/activeparser still unfinished')
if __name__=='__main__':main()
