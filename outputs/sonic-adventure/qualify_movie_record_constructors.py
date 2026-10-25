"""Qualify five original constructor/record bodies with explicit comparison scope."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];SELECTED={'0x8c666e50','0x8c666ef0','0x8c6669c4','0x8c667240','0x8c10ec40'};REUSED={'0x8c10cd38','0x8c65acac','0x8c10d31c'}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_record_constructors_translation','movie_record_constructors_connection','movie_record_constructors_verification','movie_record_constructors_prefix_verification','movie_record_constructors_regression_verification','movie_record_constructors_frontier','movie_init_records_qualification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 translation=reports['movie_record_constructors_translation'];direct=reports['movie_record_constructors_verification'];prefix=reports['movie_record_constructors_prefix_verification']
 assert translation['instruction_rows']==484 and len(translation['routines'])==9
 assert direct['cases_passed']==67 and direct['independent_checks']==374 and direct['status_counts']=={'0':61,'-3':4,'-2':2}
 assert prefix['cases_passed']==2 and prefix['cold_profile_reexecuted'] and prefix['independent_checks']==31
 assert direct['dll_sha256']==prefix['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_record_constructors.dll')
 regression=reports['movie_record_constructors_regression_verification']
 assert regression['total_comparisons']==285 and regression['saved_record_checks']==17337 and regression['dll_sha256']==direct['dll_sha256']
 cold=next(c for c in prefix['cases'] if c['input']['cold_filesystem']);warm=next(c for c in prefix['cases'] if not c['input']['cold_filesystem'])
 assert cold['disc_read_attempts']==89 and cold['masaru_payload_matches'] and cold['masaru_sha256_at_stop']==prefix['masaru_sha256'] and cold['filesystem_root_before']=='0x0' and cold['filesystem_root_after']=='0x8c6946e0' and cold['filesystem_initialized_flag']==1
 assert not warm['disc_read_attempts'] and not warm['masaru_payload_matches']
 assert all(c['status']==-2 and c['stop_pc']=='0x8c65a2fc' and c['clear_native']['writes']==0x200000 for c in prefix['cases'])
 selected=[r for r in direct['coverage_by_routine'] if r['address'] in SELECTED];assert len(selected)==5 and sum(r['static_rows'] for r in selected)==406
 for r in selected:assert r['covered_rows']==r['static_rows'] and not r['missing_edges']
 assert sum(r['conditional_edges_covered'] for r in selected)==34
 routines=[dict(r,module='1ST_READ.BIN',source='source/sa_movie_record_constructors.c',source_sha256=digest(P/'source/sa_movie_record_constructors.c')) for r in translation['routines'] if r['address'] in SELECTED]
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in sorted(REUSED):
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  canonical.append({k:row[k] for k in ('address','name','source','verification')});files.append(P/row['source'])
 report=dict(routines=routines,newly_registered_entries=5,qualified_rows=406,executed_rows=406,qualified_conditional_outcomes=34,executed_conditional_outcomes=34,compiled_source_count=175,static_adapter_bodies=9,static_adapter_rows=484,direct_comparisons=67,independent_direct_checks=374,prefix_comparisons=2,independent_prefix_checks=31,total_comparisons=69,new_cold_prefix_reexecuted=True,cold_prefix_steps=cold['steps'],cold_prefix_boundaries=cold['boundary_comparisons'],cold_prefix_disc_reads=89,cold_filesystem_root='0x8c6946e0',masaru_bytes=33408,masaru_sha256=prefix['masaru_sha256'],masaru_native_load_address='0x0cd00000',parent_bodies_qualified=False,next_dependency='0x8c65a2fc',canonical_reuse=canonical,reused_additional_credit=0,dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='Five complete own original constructor/record bodies qualify with explicit input preconditions. Three copy-entry support adapters and the remaining parent stay research support with zero new credit. Canonical copy/fill/error helpers retain their original metadata. Caller and cold driver-load comparison remain partial at missing 8c65a2fc.',limitations=['Direct constructor tests use explicit root/data/template/record preconditions. The independent semantic oracle checks all non-stack RAM; the original comparison separately checks all stack bytes and complete machine/device state.','Shared copy-tail entry labels are service boundaries only on actual transfers, not sequential fallthrough. Their overlapping instruction spans receive zero new routine credit. The odd-word dispatch adapter is exercised for the reached 28-byte contract; other sizes are not claimed qualified.','The five qualified bodies have 406 own rows and 34 own conditional outcomes, all executed. Parent 8c666b34 remains partial at 38 of 40 rows and missing diagnostic entry 8c65a2fc.','Cold startup explicitly zeros filesystem/interface roots before original producers. The warm prepared-root control keeps actual missing-file behavior. No descriptor, read or driver result is supplied.','Driver bytes are loaded, not decompiled or executed. Console/flash/disc/timing services remain declared software-host policy; natural reset, physical hardware, graphics/gameplay and requested ports/settings remain unfinished.' ])
 report.update(regression_comparisons=285,regression_saved_record_checks=17337,total_comparisons=354)
 (P/'analysis/movie_record_constructors_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified five constructor/record bodies; cold driver payload verified; 354 comparisons including regressions')
if __name__=='__main__':main()
