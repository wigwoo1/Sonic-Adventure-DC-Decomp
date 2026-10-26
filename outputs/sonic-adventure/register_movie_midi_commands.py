"""Append only the newly qualified sender; leave existing setter canonical data intact."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_midi_commands_qualification.json';q=json.loads(path.read_text())
 assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['reused_adapter_rows'])==(1,43,6,11)
 for file,h in q['sha256'].items():assert digest(file)==h,file
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for old in q['canonical_reuse']:
  current=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==old['address'])
  for key,value in old.items():assert current[key]==value,(old['address'],key)
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=path.relative_to(P).as_posix(),status='Qualified complete original MIDI command86 sender,43ownrows6outcomes;2245fresh original/native comparisons. Existing11-row setter adapter and SDK/copy/logger receive zero additional recovery credit. Actual sparse packet stores/queue/mask/error/partial-failure states preserved;no ARM/audio claim.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1412;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';s=json.loads(sp.read_text())
 data={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','qualified_conditional_outcomes','executed_integration_rows','reused_adapter_rows','reused_additional_credit','compiled_source_count','total_original_native_comparisons','dll_sha256','limitations')}
 data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_MIDI_COMMANDS.txt')
 s['verified_progress']['movie_midi_commands_integration']=data;s['verified_progress']['recovered_buildable_routines']=s['recovered_buildable_routines']=1412
 sp.write_text(json.dumps(s,indent=2)+'\n');print('Registered1MIDI sender;1412entries;existingsetterunchanged0credit')
if __name__=='__main__':main()
