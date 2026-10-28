"""Append the qualified triangle wrapper; preserve every previous registry entry."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/collision_render_qualification.json';q=json.loads(qp.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['executed_conditional_outcomes'],q['proven_infeasible_conditional_outcomes'])==(1,332,31,1)
 for pin,h in q['sha256'].items():assert digest(pin)==h,pin
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());previous=registry['routines'][:1586];assert len(previous)==1586
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=qp.relative_to(P).as_posix(),status='Original triangle renderer software-qualified within declared mapped-memory/numeric ABI domains. All332instructions/31feasibleoutcomes execute;one additional syntacticoutcome provedinfeasible fromnormalentry. Physicalhardware/pixels/gameplay andexportednonlocalcontinuations remainunqualified.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1587 and registry['routines'][:1586]==previous and len({(r.get('module','1ST_READ.BIN'),r['address']) for r in registry['routines']})==1584
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('routines','sha256')};data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='COLLISION_RENDER.txt');status['verified_progress']['collision_render_integration']=data
 status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1587;sp.write_text(json.dumps(status,indent=2)+'\n');print('REGISTERED1587/unique1584/629confirmedlinked;10%weightedmilestones')
if __name__=='__main__':main()
