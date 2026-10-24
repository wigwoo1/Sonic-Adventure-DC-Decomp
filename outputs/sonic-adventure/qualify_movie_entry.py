"""Qualify the raw entry thunk and two closed helpers; keep startup partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_entry_verification','movie_entry_prefix_verification','movie_module_input_verification','movie_runtime_inventory')];direct,prefix,provenance,inventory=[json.loads(p.read_text()) for p in paths]
 assert direct['cases_passed']==62 and direct['status_counts']=={'-3':16,'0':46} and direct['independent_checks']==594
 assert prefix['cases_passed']==8 and prefix['status_counts']=={'-4':4,'-2':4} and prefix['boot_rows_covered']==33 and prefix['next_dependencies']==['0x8c10d914','0x8c602612']
 assert provenance['raw_input_cases']==1 and provenance['rejected_provenance_cases']==7 and inventory['indexed_candidates']==536
 for proof in (direct,prefix):assert proof['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_entry.dll')
 files=list(paths)
 for proof in (direct,prefix,provenance,inventory):
  for path,h in proof['sha256'].items():assert digest(path)==h,path
  files.extend(Path(path) for path in proof['sha256'])
 selected={0x8ceb0000,0x8c6436d0,0x8c041e60};routines=[];rows=[]
 for routine in direct['routines']:
  address=int(routine['address'],16)
  if address not in selected:continue
  row=next(r for r in direct['coverage_by_routine'] if r['address']==routine['address']);assert row['covered_rows']==row['static_rows'] and not row['missing_rows'] and not row['missing_edges']
  r=dict(routine);module='MOVIE.BIN' if address==0x8ceb0000 else '1ST_READ.BIN';source='source/sa_movie_entry.c' if module=='MOVIE.BIN' else 'source/sa_movie_main.c'
  r.update(module=module,source=source,source_sha256=digest(P/source));routines.append(r);rows.append(row)
 assert len(routines)==3
 movie=next(r for r in direct['coverage_by_routine'] if r['address']=='0x8ceb0020');boot_movie=next(r for r in prefix['coverage_by_routine'] if r['address']=='0x8ceb0020');missing=set(movie['missing_rows'])&set(boot_movie['missing_rows'])
 files.append(Path(__file__))
 q=dict(routines=routines,coverage_by_routine=rows,cases_passed=62,status_counts=direct['status_counts'],independent_checks=594,boot_prefix_cases=8,boot_prefix_independent_checks=72,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_rows_covered=364-len(missing),movie_startup_static_rows=364,movie_startup_recovery_credit=0,newly_registered_entries=3,new_module_thunks=1,new_main_helpers=2,qualified_rows=36,qualified_conditional_outcomes=2,retail_binary=inventory['retail_binary'],retail_binary_sha256=inventory['retail_binary_sha256'],load_address=inventory['load_address'],raw_provenance_checks=8,indexed_movie_candidates=536,dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='One two-instruction raw MOVIE entry branch thunk plus complete 8-row driver-field setter and 26-row view-default copy. Native partial startup and SDK adapters are connected and compared without claiming full parent recovery. All qualified rows/outcomes execute; original raw identity/base and all legacy audit paths are checked.',limitations=direct['limitations']+prefix['limitations']+['Sixteen direct module-entry cases preserve the actual invalid-memory stops from uninitialized frame fixtures. They do not establish valid standalone startup.','Four continuous boot cases exhaust the original finite clock samples; four explicitly later-clock profiles complete the real wait and reach unrecovered status query 8c10d914. No device query/callback return is fabricated.','Only the entry thunk receives MOVIE recovery credit. The 364-row startup parent, 16 main adapter proposals and Ghidra candidates remain partially verified or unqualified according to their individual evidence.'])
 (P/'analysis/movie_entry_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified raw MOVIE entry thunk and two closed helpers; partial startup rows',q['movie_startup_rows_covered'],'/364')
if __name__=='__main__':main()
