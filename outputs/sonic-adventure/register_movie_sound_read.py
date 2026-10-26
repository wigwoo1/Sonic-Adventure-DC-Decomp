"""Record actual earlier-startup integration without rewriting canonical getters."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_sound_read_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==0 and q['total_original_native_comparisons']==854
 for path,h in q['sha256'].items():assert digest(path)==h,path
 rows=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(rows)==1397
 for old in q['canonical_reuse']:
  current=next(r for r in rows if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==old['address'])
  for key,value in old.items():assert current[key]==value,(old['address'],key)
 integration={k:q[k] for k in ('newly_registered_entries','qualified_integration_rows','qualified_conditional_outcomes','compiled_source_count','total_original_native_comparisons','cold_original_sound_fill_bytes','cold_full_vram_clear_bytes','cold_actual_transfer_bytes','cold_requests_returned','cold_irq_deliveries','cold_complete_sound_trace_events','sound_trace_events_dropped','actual_next_frontier','partial_bootstrap_frontier','dll_sha256','limitations')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_SOUND_READ_STARTUP.txt')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status['verified_progress']['movie_sound_read_startup_integration']=integration;sp.write_text(json.dumps(status,indent=2)+'\n');print('Recorded startup/sound-read integration;1397entries unchanged;goalunfinished')
if __name__=='__main__':main()
