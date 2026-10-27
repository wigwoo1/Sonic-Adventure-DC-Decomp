"""Record verified integration progress without editing the routine registry."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=P/'analysis/movie_record_address_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['qualified_integration_rows'],q['fresh_original_native_comparisons'])==(0,9,388)
 for pinned,h in q['sha256'].items():assert digest(pinned)==h,pinned
 registry=P/'analysis/recovered_routines.json';assert digest(registry)==q['registry_sha256'] and len(json.loads(registry.read_text())['routines'])==1469
 sp=P/'project_status.json';status=json.loads(sp.read_text());assert status['recovered_buildable_routines']==status['verified_progress']['recovered_buildable_routines']==1469
 data={k:v for k,v in q.items() if k not in ('sha256','coverage_by_routine')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_RECORD_ADDRESS.txt');status['verified_progress']['movie_record_address_integration']=data;sp.write_text(json.dumps(status,indent=2)+'\n')
 assert digest(registry)==q['registry_sha256'];print('PROGRESS recorded;registry unchanged1469;warm0546c0 completes')
if __name__=='__main__':main()
