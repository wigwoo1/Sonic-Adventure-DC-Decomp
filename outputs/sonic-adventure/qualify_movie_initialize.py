"""Record partial caller integration, without awarding whole-body recovery credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
REUSED={'0x8c051760','0x8c604510','0x8c055c44','0x8c10ba1e','0x8c10c0d0','0x8c10c0f8','0x8c10da50'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_initialize_translation','movie_initialize_connection','movie_initialize_verification','movie_initialize_regression_verification'):
  path=P/f'analysis/{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
  for pinned,h in report['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 translation=reports['movie_initialize_translation'];direct=reports['movie_initialize_verification'];regression=reports['movie_initialize_regression_verification'];connection=reports['movie_initialize_connection']
 assert translation['instruction_rows']==332 and len(translation['routines'])==11 and connection['compiled_source_count']==172
 assert direct['cases_passed']==68 and direct['independent_checks']==294 and direct['status_counts']=={'0':32,'-2':31,'-3':5}
 assert direct['dll_sha256']==regression['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_initialize.dll')
 assert regression['original_bridge_preserved_except_added_routes'] and regression['retained_cold_trace_has_no_added_routes'] and not regression['cold_start_reexecuted_in_new_dll']
 assert regression['warm_result']['clear_native']['writes']==0x200000
 rows=direct['coverage_by_routine'];assert sum(r['static_rows'] for r in rows)==332 and sum(r['covered_rows'] for r in rows)==83
 for row in rows:
  if row['address'] in REUSED:assert not row['missing_rows'] and not row['missing_edges']
  else:assert row['missing_rows']
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in sorted(REUSED):
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  canonical.append(dict(address=address,name=row['name'],source=row['source'],verification=row['verification']));files.append(P/row['source'])
 report=dict(compiled_source_count=172,static_adapter_bodies=11,static_adapter_rows=332,executed_adapter_rows=83,direct_comparisons=68,independent_checks=294,retained_warm_startup_comparisons=1,total_comparisons=69,canonical_reuse=canonical,reused_additional_credit=0,new_registry_credit=0,parent_bodies_qualified=False,coverage_by_routine=rows,caller_address='0x8c0546c0',caller_rows_executed=15,caller_rows_total=177,original_initializer_reached_from_caller=False,missing_service_vectors=['0x8c0000b0','0x8c0000b8'],cold_start_reexecuted_in_new_dll=False,dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},next_task='Establish explicit native console-information/flash service input contracts from original syscall wrappers and primary ABI evidence; preserve the unresolved-callback controls and resume this original caller.',scope='Reached original caller and new static adapters are partial. Seven canonical helpers retain names/source/proof and receive zero extra credit. Unresolved BIOS callbacks record entry and stop; the verifier never supplies a result or continues. New 172-source build retains the warm/full-clear regression; old cold evidence remains separate. No natural boot, pixels or playable port claim.')
 (P/'analysis/movie_initialize_integration.json').write_text(json.dumps(report,indent=2)+'\n');print('Partial caller integration: 11 adapters / 332 rows, 83 reached; 69 comparisons; zero new routine credit')
if __name__=='__main__':main()
