"""Qualify original region/cable query bodies under explicit native ABI inputs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];SELECTED={'0x8c10c044','0x8c10c07e'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_initialize_translation','movie_console_connection','console_abi_evidence','console_service_backend_verification','movie_console_verification','movie_console_query_invariant','movie_console_regression_verification','movie_console_frontier'):
  path=P/f'analysis/{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
  for pinned,h in report['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 direct=reports['movie_console_verification'];proof=reports['movie_console_query_invariant'];regression=reports['movie_console_regression_verification'];backend=reports['console_service_backend_verification']
 assert direct['cases_passed']==228 and direct['independent_checks']==2077 and direct['status_counts']=={'0':202,'-2':18,'-3':5,'-4':3}
 assert backend['api_cases']==43 and backend['independent_checks']==405 and regression['total_comparisons']==69 and regression['saved_record_checks']==4298
 assert direct['dll_sha256']==backend['dll_sha256']==regression['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_console.dll')
 assert proof['unreachable_instruction_rows']==['0x8c10c0ae','0x8c10c0b0'] and proof['unreachable_edge']==['0x8c10c0ac','0x8c10c0ae'] and proof['exhaustive_original_suffix_inputs']==256
 selected=[r for r in direct['coverage_by_routine'] if r['address'] in SELECTED];assert len(selected)==2 and sum(r['static_rows'] for r in selected)==64 and sum(r['covered_rows'] for r in selected)==62
 for row in selected:
  assert row['missing_rows']==(proof['unreachable_instruction_rows'] if row['address']=='0x8c10c07e' else [])
  assert row['missing_edges']==([proof['unreachable_edge']] if row['address']=='0x8c10c07e' else [])
 routines=[dict(r,module='1ST_READ.BIN',source='source/sa_movie_initialize.c',source_sha256=digest(P/'source/sa_movie_initialize.c')) for r in reports['movie_initialize_translation']['routines'] if r['address'] in SELECTED]
 parents=[c for c in direct['cases'] if c['input']['entry']==0x8c0546c0];assert len(parents)==3 and {tuple(c['initializer_call_arguments'][0][:3]) for c in parents}=={(1,0,1),(58,0,1),(56,0,1)}
 assert all(c['clear_native']['writes']==0x200000 and c['stop_pc']=='0x8c092fa0' for c in parents)
 reused=json.loads((P/'analysis/movie_initialize_integration.json').read_text())['canonical_reuse']
 report=dict(routines=routines,newly_registered_entries=2,qualified_rows=64,executed_rows=62,proven_unreachable_rows=2,qualified_conditional_outcomes=18,executed_conditional_outcomes=17,proven_unreachable_conditional_outcomes=1,compiled_source_count=173,direct_comparisons=228,independent_checks=2077,backend_api_cases=43,backend_independent_checks=405,regression_comparisons=69,regression_saved_record_checks=4298,total_game_comparisons=297,canonical_reuse=reused,reused_additional_credit=0,caller_rows_executed=65,caller_rows_total=177,caller_qualified=False,caller_initializer_arguments=[[1,0,1],[58,0,1],[56,0,1]],caller_next_dependency='0x8c092fa0',cold_start_reexecuted_in_new_dll=False,dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='Two complete original query bodies qualify with byte-checked range evidence for two unreachable rows/one outcome. Seven canonical helpers retain their evidence and get zero extra credit. Original caller and flash-override parent remain partial. Native provider implements separately declared host ABI behavior, not decompiled BIOS.',limitations=['Borrowed invented information/flash bytes and vector bindings are explicit native-platform inputs; they are not authentic console records or reset-produced state.','Primary KOS wrapper/header evidence supports the vector/selectors, pointer-return contract and flash byte-count return. Copy location, initialization order and scratch-register preservation are explicit host choices.','Original flash override checks for a zero result after a two-byte request; the count-returning host service does not invent that zero. Its parent remains partial.','The range proof assumes immutable original instructions, normal call/return ABI, fault-stop behavior and no asynchronous CPU register or stack corruption. All original code remains.','All three original caller variants retain the full eight-MiB clear but stop at 8c092fa0. The separate long cold benchmark is retained, not rerun. Natural boot, physical devices, graphics/gameplay and all requested playable ports/settings remain unfinished.'])
 (P/'analysis/movie_console_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified two original query bodies: 62 executed rows plus two proven unreachable; 297 game comparisons')
if __name__=='__main__':main()
