"""Append one qualified handler and preserve all existing canonical fields."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_transfer_interrupt_qualification.json';q=json.loads(qp.read_text());assert q['qualified_rows']==39 and q['total_comparisons']==775
 for path,h in q['sha256'].items():assert digest(path)==h,path
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());routine=q['routines'][0]
 row={k:routine[k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
 row.update(verification=qp.relative_to(P).as_posix(),status='Qualified complete own installed transfer interrupt body:39executed original rows and2conditional outcomes. Full original/native state and independent effects match in explicit initialized ABI/software-platform domain.',limitations=q['limitations'])
 previous=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
 if previous is None:registry['routines'].append(row)
 else:assert previous==row
 assert len(registry['routines'])==1392
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 integration={k:q[k] for k in ('newly_registered_entries','qualified_rows','qualified_conditional_outcomes','compiled_source_count','total_comparisons','retained_default_frontier','dll_sha256','limitations')}
 integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_TRANSFER_INTERRUPT.txt')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status['verified_progress']['movie_transfer_interrupt_integration']=integration;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=1392
 sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered transfer interrupt;1392entries;prior canonical rows retained')
if __name__=='__main__':main()
