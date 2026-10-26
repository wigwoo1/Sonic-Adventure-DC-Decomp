"""Append qualified mode/sender bodies without changing earlier canonical data."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 path=P/'analysis/movie_audio_mode_qualification.json';q=json.loads(path.read_text())
 assert q['newly_registered_entries']==2 and q['newly_qualified_rows']==120 and q['qualified_conditional_outcomes']==16
 for file,h in q['sha256'].items():assert digest(file)==h,file
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for old in q['canonical_reuse']:
  current=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==old['address'])
  for key,value in old.items():assert current[key]==value,(old['address'],key)
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=path.relative_to(P).as_posix(),status='Qualified complete original audio-mode/SDK command producer;120own rows/16outcomes across2new routines.1311freshoriginal/native comparisons;actual20-byte copy,ready/count/tail/mask/error state. Shared copy fragment receives zero duplicate credit;ARM consumption/audio output not claimed.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1407
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=P/'project_status.json';status=json.loads(status_path.read_text())
 integration={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','qualified_conditional_outcomes','executed_integration_rows','shared_fragment_additional_credit','compiled_source_count','total_original_native_comparisons','dll_sha256','limitations')}
 integration.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_AUDIO_MODE.txt')
 status['verified_progress']['movie_audio_mode_integration']=integration
 status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1407
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print('Registered2originalaudio/SDKbodies;1407entries;sharedcopy0credit')
if __name__=='__main__':main()
