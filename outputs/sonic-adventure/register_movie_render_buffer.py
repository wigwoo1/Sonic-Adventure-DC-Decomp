"""Append three qualified accounting bodies without changing older records."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_render_buffer_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(15,1491,370)
 for pinned,h in q['sha256'].items():assert digest(pinned)==h,pinned
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')};row.update(verification=path.relative_to(P).as_posix(),status='Qualified original render-buffer own body:15newroutines1491rows/370outcomes (369execute/oneproveddead),727direct/231retained/2fullstartup comparisons in203-source build;actualfrontier65393a. Finitemappedmetadata/private-stack contract;older source/proofs retained.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1448;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';s=json.loads(sp.read_text());data={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','executed_rows','executed_conditional_outcomes','qualified_conditional_outcomes','compiled_source_count','new_direct_original_native_comparisons','new_direct_independent_checks','fresh_retained_original_native_comparisons','fresh_retained_independent_checks','fresh_full_startup_comparisons','fresh_startup_independent_checks','fresh_original_native_comparisons','retained_202_source_original_native_comparisons','total_evidence_original_native_comparisons','actual_next_frontier','remaining_unqualified_graphics_root_outcomes','reused_additional_credit','dll_sha256','limitations')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_RENDER_BUFFER.txt');s['verified_progress']['movie_render_buffer_integration']=data;s['verified_progress']['recovered_buildable_routines']=s['recovered_buildable_routines']=1448;sp.write_text(json.dumps(s,indent=2)+'\n');print('Registered15renderbodies;1448entries;1433previouscanonicalrows preserved')
if __name__=='__main__':main()
