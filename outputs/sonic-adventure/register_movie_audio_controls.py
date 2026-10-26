"""Append only the three fully qualified original command controls."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_audio_controls_qualification.json';q=json.loads(path.read_text())
 assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(3,147,26)
 for file,h in q['sha256'].items():assert digest(file)==h,file
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for old in q['canonical_reuse']:
  current=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==old['address'])
  for key,value in old.items():assert current[key]==value,(old['address'],key)
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=path.relative_to(P).as_posix(),status='Qualified complete original audio command control;147ownrows/26outcomes across3new routines.1555freshoriginal/native comparisons;actual sequential command construction/copies/ring/mask/error/partial-failure states. Reused SDK/copy/logger gets zero additional credit;ARM/audio output not claimed.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1410;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';s=json.loads(sp.read_text())
 integration={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','qualified_conditional_outcomes','executed_integration_rows','shared_fragment_additional_credit','compiled_source_count','total_original_native_comparisons','dll_sha256','limitations')}
 integration.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_AUDIO_CONTROLS.txt')
 s['verified_progress']['movie_audio_controls_integration']=integration;s['verified_progress']['recovered_buildable_routines']=s['recovered_buildable_routines']=1410
 sp.write_text(json.dumps(s,indent=2)+'\n');print('Registered3originalcontrols;1410entries;reuse0credit')
if __name__=='__main__':main()
