"""Record integration progress without crediting authored support as game recovery."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_transfer_runtime_qualification.json';q=json.loads(qp.read_text());assert q['total_comparisons']==699 and q['newly_registered_entries']==0
 for path,h in q['sha256'].items():assert digest(path)==h,path
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1396
 assert not any(r['address']=='0x8c65a5dc' for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN')
 integration={k:q[k] for k in ('newly_registered_entries','host_support_recovery_credit','compiled_source_count','continuous_requests_returned','continuous_lifecycles','total_comparisons','authored_platform_cases','partial_request','retained_default_frontier','partial_bootstrap_frontier','dll_sha256','limitations')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_TRANSFER_RUNTIME.txt')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status['verified_progress']['movie_transfer_runtime_integration']=integration;sp.write_text(json.dumps(status,indent=2)+'\n');print('Recorded persistent transfer runtime;1396entries unchanged; request stays partial')
if __name__=='__main__':main()
