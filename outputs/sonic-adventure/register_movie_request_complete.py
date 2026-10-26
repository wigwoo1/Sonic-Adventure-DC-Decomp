"""Append the now-complete original request while preserving all earlier fields."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_request_complete_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==1 and q['combined_original_native_comparisons']==711
 for path,h in q['sha256'].items():assert digest(path)==h,path
 routine=q['routines'][0];rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
 row.update(verification=qp.relative_to(P).as_posix(),status='Qualified complete original request own body:139instructions/all26conditional outcomes;12fresh comparisons and270independent checks close four queue/callback branches. Unchanged193-source DLL and699retained comparisons remain hash-verified. Asynchronous return is not DMA completion; broader runtime/boot remains unfinished.',limitations=q['limitations'])
 existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==row['module'] and r['address']==row['address']),None)
 if existing is None:registry['routines'].append(row)
 else:assert existing==row
 assert len(registry['routines'])==1397;rp.write_text(json.dumps(registry,indent=2)+'\n')
 integration={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','qualified_conditional_outcomes','compiled_source_count','fresh_original_native_comparisons','fresh_independent_checks','retained_original_native_comparisons','combined_original_native_comparisons','retained_authored_platform_cases','retained_continuous_requests_returned','production_code_changed','retained_default_frontier','partial_bootstrap_frontier','dll_sha256','limitations')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_REQUEST_COMPLETE.txt')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status['verified_progress']['movie_request_complete_integration']=integration;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1397;sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered original request;1397entries;older canonical fields preserved')
if __name__=='__main__':main()
