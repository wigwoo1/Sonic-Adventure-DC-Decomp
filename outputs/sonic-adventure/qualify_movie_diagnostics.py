"""Qualify two diagnostic wrappers, preserving canonical formatting metadata."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
NEW={'0x8c65a2fc','0x8c65a33e'}
CANONICAL={'0x8c10d400','0x8c10e576','0x8c10e304','0x8c10d3f0','0x8c10e50e','0x8c10e566','0x8c600ec6','0x8c10d25c'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 names=('movie_diagnostics_translation','movie_diagnostics_connection','movie_diagnostics_verification','movie_diagnostics_semantics_verification','movie_diagnostics_default_base_verification','movie_diagnostics_alignment_invariant','movie_diagnostics_prefix_verification','movie_diagnostics_regression_verification','movie_diagnostics_frontier','sh4_widen_compare_verification','movie_record_constructors_qualification')
 for name in names:
  path=P/f'analysis/{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
  for pinned,h in report['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 main=reports['movie_diagnostics_verification'];extra=reports['movie_diagnostics_default_base_verification'];prefix=reports['movie_diagnostics_prefix_verification'];reg=reports['movie_diagnostics_regression_verification'];backend=reports['sh4_widen_compare_verification'];invariant=reports['movie_diagnostics_alignment_invariant'];translation=reports['movie_diagnostics_translation'];sem=reports['movie_diagnostics_semantics_verification']
 assert translation['instruction_rows']==2805 and len(translation['routines'])==15
 assert main['cases_passed']==351 and main['independent_checks']==738 and main['status_counts']=={'0':333,'-2':15,'1':2,'-3':1}
 assert sem['comparisons']==351 and sem['independent_semantic_checks']==568 and extra['cases_passed']==45 and extra['independent_checks']==225 and extra['status_counts']=={'0':45}
 assert prefix['cases_passed']==2 and prefix['cold_profile_reexecuted'] and prefix['independent_checks']==31
 assert reg['total_comparisons']==350 and reg['saved_record_checks']==21302 and backend['cases_passed']==52815
 dll=digest(ROOT/'work/sonic-adventure/build/movie_diagnostics.dll')
 assert all(r['dll_sha256']==dll for r in (main,extra,prefix,reg,backend))
 cold=next(c for c in prefix['cases'] if c['input']['cold_filesystem']);warm=next(c for c in prefix['cases'] if not c['input']['cold_filesystem'])
 assert cold['disc_read_attempts']==89 and cold['masaru_payload_matches'] and cold['masaru_sha256_at_stop']==prefix['masaru_sha256'] and cold['filesystem_root_before']=='0x0' and cold['filesystem_root_after']=='0x8c6946e0' and cold['filesystem_initialized_flag']==1
 assert not warm['disc_read_attempts'] and not warm['masaru_payload_matches']
 assert all(c['status']==-2 and c['stop_pc']=='0x8c65a568' and c['clear_native']['writes']==0x200000 for c in prefix['cases'])
 assert invariant['executed_original_prefixes']==64 and invariant['source_rows_retained']
 qualified=[]
 for routine in translation['routines']:
  if routine['address'] not in NEW|CANONICAL:continue
  wanted={int(r['address'],16) for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};wanted_edges={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};covered=set();edges=set()
  for report in (main,extra):
   row=next((r for r in report['coverage_by_routine'] if r['address']==routine['address']),None)
   if row:covered|=wanted-{int(x,16) for x in row['missing_rows']};edges|=wanted_edges-{tuple(int(x,16) for x in e) for e in row['missing_edges']}
  dead={int(x,16) for x in invariant['unreachable_rows']} if routine['address']==invariant['address'] else set();dead_edges={tuple(int(x,16) for x in e) for e in invariant['unreachable_edges']} if dead else set()
  assert not covered&dead and not edges&dead_edges and covered|dead==wanted and edges|dead_edges==wanted_edges
  qualified.append(dict(address=routine['address'],rows=len(wanted),executed_rows=len(covered),proven_unreachable_rows=len(dead),outcomes=len(wanted_edges),executed_outcomes=len(edges),proven_unreachable_outcomes=len(dead_edges),new_credit=routine['address'] in NEW))
 assert len(qualified)==10 and sum(r['rows'] for r in qualified)==500 and sum(r['outcomes'] for r in qualified)==142
 new=[r for r in qualified if r['new_credit']];assert sum(r['rows'] for r in new)==49 and sum(r['executed_rows'] for r in new)==45 and sum(r['outcomes'] for r in new)==10
 routines=[dict(r,module='1ST_READ.BIN',source='source/sa_movie_diagnostics.c',source_sha256=digest(P/'source/sa_movie_diagnostics.c')) for r in translation['routines'] if r['address'] in NEW]
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in sorted(CANONICAL):
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);canonical.append({k:row[k] for k in ('address','name','source','verification')});files.extend((P/row['source'],P/row['verification']))
 report=dict(routines=routines,newly_registered_entries=2,qualified_rows=49,executed_rows=45,proven_unreachable_rows=4,qualified_conditional_outcomes=10,executed_conditional_outcomes=9,proven_unreachable_outcomes=1,compiled_source_count=177,static_adapter_bodies=15,static_adapter_rows=2805,integrated_qualified_bodies=qualified,integrated_qualified_rows=500,integrated_executed_rows=496,integrated_outcomes=142,integrated_executed_outcomes=141,direct_comparisons=351,primary_checks=738,independent_semantic_checks=568,default_radix_comparisons=45,default_radix_checks=225,prefix_comparisons=2,prefix_checks=31,regression_comparisons=350,regression_record_checks=21302,total_comparisons=748,backend_cases=52815,new_cold_prefix_reexecuted=True,cold_prefix_steps=cold['steps'],cold_prefix_boundaries=cold['boundary_comparisons'],cold_prefix_disc_reads=89,masaru_bytes=33408,masaru_sha256=prefix['masaru_sha256'],masaru_native_load_address='0x0cd00000',canonical_reuse=canonical,reused_additional_credit=0,parent_bodies_qualified=False,next_dependency='0x8c65a568',dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Two complete own diagnostic wrappers qualify, including a four-byte-aligned ABI range proof. Eight canonical formatting/string/copy/query bodies receive zero duplicate credit. Larger formatter/floating/decimal/rounding bodies remain partial. Native CPU conversion/comparison has a separate software-contract proof, not game or physical hardware credit.',limitations=['Aligned valid guest stack, immutable code and no asynchronous register/stack corruption are required for the four unreachable logger rows. All source rows remain present.','Direct diagnostic inputs explicitly supply format/argument RAM and callback values. Callback fixtures invoke actual existing retail code; unknown callbacks/callees remain stops. Output is guest RAM, not physical console output.','Independent semantic checks exclude only 1024 bytes below initial SP from their second RAM oracle; the primary original/native comparison checks every stack byte and full machine/device state.','Unsupported integer specifier tests supply the original incoming R8 radix 8/10/16; this preserves the default branch without claiming normal callers use that specifier.','Formatter, floating formatter, decimal conversion and rounding are partial. Two actual FPU exception controls stop with FPSCR preserved; physical trap-vector delivery is not implemented.','The cold caller comparison is newly executed in this DLL with zero filesystem/interface roots before original producers. The prepared warm root keeps actual missing-file behavior. Driver bytes are loaded, not executed or decompiled.','Console/flash/disc/timing/status services remain declared software-host policy. Natural reset, complete startup, graphics/gameplay and requested playable ports/settings remain unfinished.'])
 (P/'analysis/movie_diagnostics_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified two diagnostic wrappers; eight canonical integrations; 748 comparisons plus 52815 CPU backend cases')
if __name__=='__main__':main()
