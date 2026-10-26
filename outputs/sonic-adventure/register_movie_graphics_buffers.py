"""Append seven now-qualified bodies,preserving every prior canonical registry row."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_graphics_buffers_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(7,918,152)
 for file,h in q['sha256'].items():assert digest(file)==h,file
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for old in q['canonical_reuse']:assert old in registry['routines']
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=path.relative_to(P).as_posix(),status='Qualified complete graphics/descriptor builder within declared valid mapped/aligned/list/pool/buffer ABI;7bodies918rows,150executed outcomes+2proved infeasible.706fresh original/native comparisons9731checks supplement2473unchanged-build retained comparisons. Reused providers0duplicatecredit;sevenlargergraphicsbodiesremainpartial.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1423;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','executed_conditional_outcomes','proven_infeasible_conditional_outcomes','qualified_conditional_outcomes','compiled_source_count','native_source_or_build_changed','fresh_original_native_comparisons','fresh_independent_checks','retained_original_native_comparisons','total_evidence_original_native_comparisons','remaining_unqualified_larger_bodies','reused_additional_credit','dll_sha256','limitations')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_GRAPHICS_BUFFERS.txt')
 status['verified_progress']['movie_graphics_buffers_integration']=data;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1423;sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered7graphics/descriptorbuilders;1423entries;all1416priorcanonicalrowsretained')
if __name__=='__main__':main()
