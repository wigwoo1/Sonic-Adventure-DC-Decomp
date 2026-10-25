"""Qualify the reached state getter and its now-closed configuration caller."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
NEW={'0x8c65a568','0x8c643624'};REUSED={'0x8c65a5ac','0x8c65a5cc','0x8c65a5d4','0x8c65a5b4'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_request_fields_translation','movie_request_fields_connection','movie_request_fields_verification','movie_request_fields_lifecycle_verification','movie_request_fields_prefix_verification','movie_request_fields_regression_verification','movie_request_fields_frontier','movie_init_records_translation','movie_diagnostics_qualification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 translation=reports['movie_request_fields_translation'];direct=reports['movie_request_fields_verification'];life=reports['movie_request_fields_lifecycle_verification'];prefix=reports['movie_request_fields_prefix_verification'];reg=reports['movie_request_fields_regression_verification']
 assert translation['instruction_rows']==44 and len(translation['routines'])==5
 assert direct['cases_passed']==79 and direct['independent_checks']==425 and direct['status_counts']=={'0':66,'-3':13}
 assert life['cases_passed']==30 and life['sequences']==3 and life['calls_per_sequence']==10 and life['independent_checks']==222
 assert prefix['cases_passed']==2 and prefix['cold_profile_reexecuted'] and prefix['independent_checks']==37
 assert reg['total_comparisons']==746 and reg['saved_record_checks']==45458
 dll=digest(ROOT/'work/sonic-adventure/build/movie_request_fields.dll');assert all(r['dll_sha256']==dll for r in (direct,life,prefix,reg))
 cold=next(c for c in prefix['cases'] if c['input']['cold_filesystem']);warm=next(c for c in prefix['cases'] if not c['input']['cold_filesystem'])
 assert cold['disc_read_attempts']==89 and cold['masaru_payload_matches'] and cold['masaru_sha256_at_stop']==prefix['masaru_sha256'] and cold['filesystem_root_before']=='0x0' and cold['filesystem_root_after']=='0x8c6946e0' and cold['filesystem_initialized_flag']==1
 assert not warm['disc_read_attempts'] and not warm['masaru_payload_matches']
 assert cold['stop_pc']=='0x8c65a200' and warm['stop_pc']=='0x8c64bddc'
 assert all(c['status']==-2 and c['clear_native']['writes']==0x200000 for c in prefix['cases'])
 rows=direct['coverage_by_routine'];assert len(rows)==6 and sum(r['static_rows'] for r in rows)==108 and sum(r['conditional_edges_covered'] for r in rows)==16
 for r in rows:assert r['covered_rows']==r['static_rows'] and not r['missing_edges']
 selected=[r for r in rows if r['address'] in NEW];assert len(selected)==2 and sum(r['static_rows'] for r in selected)==89 and sum(r['conditional_edges_covered'] for r in selected)==14
 routines=[]
 for data,source in ((translation['routines'],'source/sa_movie_request_fields.c'),(reports['movie_init_records_translation']['routines'],'source/sa_movie_init_records.c')):
  for r in data:
   if r['address'] in NEW:routines.append(dict(r,module='1ST_READ.BIN',source=source,source_sha256=digest(P/source)))
 assert len(routines)==2
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in sorted(REUSED):
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);canonical.append({k:row[k] for k in ('address','name','source','verification')});files.extend((P/row['source'],P/row['verification']))
 report=dict(routines=routines,newly_registered_entries=2,qualified_rows=89,executed_rows=89,qualified_conditional_outcomes=14,executed_conditional_outcomes=14,compiled_source_count=178,static_new_adapter_bodies=5,static_new_adapter_rows=44,integrated_bodies=6,integrated_rows=108,integrated_outcomes=16,direct_comparisons=79,independent_direct_checks=425,lifecycle_comparisons=30,independent_lifecycle_checks=222,prefix_comparisons=2,independent_prefix_checks=37,regression_comparisons=746,regression_record_checks=45458,total_comparisons=857,new_cold_prefix_reexecuted=True,cold_prefix_steps=cold['steps'],cold_prefix_boundaries=cold['boundary_comparisons'],cold_prefix_disc_reads=89,masaru_bytes=33408,masaru_sha256=prefix['masaru_sha256'],masaru_native_load_address='0x0cd00000',canonical_reuse=canonical,reused_additional_credit=0,next_dependency='0x8c65a200',next_dependencies={'cold_valid_driver':'0x8c65a200','warm_missing_file':'0x8c64bddc'},dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Complete own request-state getter and original configuration caller qualify, all rows/outcomes executed. Four canonical field providers receive zero duplicate credit. Actual producer/consumer API sequences and fresh original/native caller prefix are verified. The loaded valid SDRV header sends the cold path to missing 65a200; the warm missing-file control instead reaches 64bddc. Both remain explicit original-service stops.',limitations=['Direct tests use explicit record/root/output and call-argument preconditions. Independent exact RAM oracles include every guest-stack/error write and the first store retained before a second paired-store fault. Host fault stops do not implement physical address-exception delivery.','The three ten-call API sequences retain RAM, ordinary registers and FPU results; named special inputs remain fixed and these integer bodies leave MACL unchanged. Explicit call entry/return link and arguments are supplied. This is not a natural full-startup claim.','The cold comparison starts filesystem/interface roots at zero before original producers; the prepared warm control retains its actual missing-file behavior. No getter, setter, driver or firmware result is supplied.','Driver bytes are loaded, not executed/decompiled. Full eight-MiB clear writes and its established decoder/fingerprint scope are retained; this is not a cryptographic collision-proof trace claim.','The new DLL replays previous diagnostic/float controls; the earlier 52815 CPU-backend cases remain historical rather than newly rerun in this stage. Larger formatter/decimal/rounding paths remain partial.','Console/flash/disc/timing/status remain explicit host policy. Natural reset, complete startup, graphics/gameplay and all requested playable ports/settings are unfinished.'])
 (P/'analysis/movie_request_fields_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified state getter and configuration caller; 89 rows / 14 outcomes; 857 comparisons')
if __name__=='__main__':main()
