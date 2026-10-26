"""Append two complete contexts; preserve older canonical bodies and requirements."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_interrupt_context_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==2 and q['total_comparisons']==666
 for path,h in q['sha256'].items():assert digest(path)==h,path
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=qp.relative_to(P).as_posix(),status='Qualified complete original normal/nested interrupt context. Full345own rows and8outcomes;666original/native integration/regression/compatibility comparisons. Full CPU/FPU/banked/special return state restored. Explicit untimed software admission; physical timing and automatic runtime delivery not claimed.',limitations=q['limitations'])
  prior=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if prior is None:registry['routines'].append(row)
  else:assert prior==row
 assert len(registry['routines'])==1396;rp.write_text(json.dumps(registry,indent=2)+'\n')
 integration={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','qualified_conditional_outcomes','executed_integration_rows','executed_conditional_outcomes','compiled_source_count','total_comparisons','authored_host_transfer_cases','authored_irq_admission_cases','host_support_recovery_credit','retained_default_frontier','partial_bootstrap_frontier','dll_sha256','limitations')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_INTERRUPT_CONTEXT.txt')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status['verified_progress']['movie_interrupt_context_integration']=integration;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1396;sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered2contexts;1396entries;partial helper/fragments excluded')
if __name__=='__main__':main()
