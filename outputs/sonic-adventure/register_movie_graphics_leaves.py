"""Append only fully qualified graphics leaves without altering prior canonical rows."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/movie_graphics_leaves_qualification.json';q=json.loads(path.read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'])==(4,352,74)
 for p,h in q['sha256'].items():assert digest(p)==h,p
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 for old in q['canonical_reuse']:assert old in registry['routines']
 for routine in q['routines']:
  row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
  row.update(verification=path.relative_to(P).as_posix(),status='Qualified complete graphics configuration/address leaf within declared mapped aligned ABI;four leaves352own rows74outcomes,2473fresh original/native comparisons. Five reused adapters and13partial larger bodies receive zero additional credit. No pixels/gameplay/physicaltiming claim.',limitations=q['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
  if old is None:registry['routines'].append(row)
  else:assert old==row
 assert len(registry['routines'])==1416;rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';s=json.loads(sp.read_text());data={k:q[k] for k in ('newly_registered_entries','newly_qualified_rows','qualified_conditional_outcomes','reused_adapter_rows','reused_additional_credit','unqualified_new_larger_bodies','compiled_source_count','total_original_native_comparisons','dll_sha256','limitations')}
 data.update(verification=path.relative_to(P).as_posix(),verification_sha256=digest(path),guide='MOVIE_GRAPHICS_LEAVES.txt');s['verified_progress']['movie_graphics_leaves_integration']=data;s['verified_progress']['recovered_buildable_routines']=s['recovered_buildable_routines']=1416;sp.write_text(json.dumps(s,indent=2)+'\n');print('Registered four graphics leaves;1416entries;all earlier canonical rows unchanged')
if __name__=='__main__':main()
