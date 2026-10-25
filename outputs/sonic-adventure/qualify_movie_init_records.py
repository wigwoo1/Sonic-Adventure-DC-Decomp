"""Qualify original request-slot allocation while preserving partial loader scope."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];SELECTED={'0x8c65aa30','0x8c643ac0'};REUSED={'0x8c65acac','0x8c5ff2cc','0x8c5ff2de'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_init_records_translation','movie_init_records_connection','movie_init_records_verification','movie_init_records_prefix_verification','movie_init_records_regression_verification','movie_init_records_frontier','movie_console_qualification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 translation=reports['movie_init_records_translation'];direct=reports['movie_init_records_verification'];prefix=reports['movie_init_records_prefix_verification']
 assert translation['instruction_rows']==882 and len(translation['routines'])==22
 assert direct['cases_passed']==59 and direct['independent_checks']==309 and direct['status_counts']=={'0':53,'-3':6}
 assert prefix['cases_passed']==2 and prefix['cold_profile_reexecuted'] and prefix['independent_checks']==31
 assert direct['dll_sha256']==prefix['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_init_records.dll')
 regression=reports['movie_init_records_regression_verification']
 assert regression['total_comparisons']==226 and regression['saved_record_checks']==13738 and regression['dll_sha256']==direct['dll_sha256']
 cold=next(c for c in prefix['cases'] if c['input']['cold_filesystem']);warm=next(c for c in prefix['cases'] if not c['input']['cold_filesystem'])
 assert cold['disc_read_attempts']==89 and cold['masaru_payload_matches'] and cold['masaru_sha256_at_stop']==prefix['masaru_sha256'] and cold['filesystem_root_before']=='0x0' and cold['filesystem_root_after']=='0x8c6946e0' and cold['filesystem_initialized_flag']==1
 assert not warm['disc_read_attempts'] and not warm['masaru_payload_matches']
 assert all(c['status']==-2 and c['stop_pc']=='0x8c666e50' and c['clear_native']['writes']==0x200000 for c in prefix['cases'])
 selected=[r for r in direct['coverage_by_routine'] if r['address'] in SELECTED];assert len(selected)==2 and sum(r['static_rows'] for r in selected)==113
 for r in selected:assert r['covered_rows']==r['static_rows'] and not r['missing_edges']
 assert sum(r['conditional_edges_covered'] for r in selected)==34
 routines=[dict(r,module='1ST_READ.BIN',source='source/sa_movie_init_records.c',source_sha256=digest(P/'source/sa_movie_init_records.c')) for r in translation['routines'] if r['address'] in SELECTED]
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in sorted(REUSED):
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  canonical.append({k:row[k] for k in ('address','name','source','verification')});files.append(P/row['source'])
 report=dict(routines=routines,newly_registered_entries=2,qualified_rows=113,executed_rows=113,qualified_conditional_outcomes=34,executed_conditional_outcomes=34,compiled_source_count=174,static_adapter_bodies=22,static_adapter_rows=882,direct_comparisons=59,independent_direct_checks=309,prefix_comparisons=2,independent_prefix_checks=31,total_comparisons=61,new_cold_prefix_reexecuted=True,cold_prefix_steps=cold['steps'],cold_prefix_boundaries=cold['boundary_comparisons'],cold_prefix_disc_reads=89,cold_filesystem_root='0x8c6946e0',masaru_bytes=33408,masaru_sha256=prefix['masaru_sha256'],masaru_native_load_address='0x0cd00000',parent_bodies_qualified=False,next_dependency='0x8c666e50',canonical_reuse=canonical,reused_additional_credit=0,dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='Two complete original request-pool allocation bodies qualify. Three canonical helpers receive zero extra credit. Actual original caller, cold filesystem and driver load replay in a new full original/native comparison; driver/setup parents remain partial at missing 8c666e50.',limitations=['Direct request-pool tests use explicit root/slot/flag/output preconditions and exact RAM/stack/error-record oracles, not a claim that every test pool was naturally constructed.','Cold startup explicitly zeros filesystem/interface roots before original producers; the warm prepared-root control retains its actual missing-file behavior. No descriptor, file read or driver result is supplied.','The driver byte hash proves the 33408 original bytes were loaded at 0cd00000 through actual 17-sector transport following 72 initialization reads. It does not establish driver execution, hardware output or a fully recovered driver module.','Both caller profiles retain every full eight-MiB clear write, and all reached/final machine, RAM, FPU, software-device, disc/input state and ordered boundaries match.','Native peripheral/console/flash/disc behavior remains declared host policy. Natural reset, physical hardware, graphics/gameplay and all requested playable ports/settings remain unfinished.'])
 report.update(regression_comparisons=226,regression_saved_record_checks=13738,total_comparisons=287)
 (P/'analysis/movie_init_records_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified two request-slot bodies; cold driver payload verified; 287 comparisons including regressions')
if __name__=='__main__':main()
