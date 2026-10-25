"""Qualify original 8c65a200 without duplicate credit for its transfer services."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_driver_control_translation','movie_driver_control_connection','movie_driver_control_verification','movie_driver_control_prefix_verification','movie_driver_control_regression_verification','movie_driver_control_frontier','movie_request_fields_qualification','movie_hardware_qualification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 t=reports['movie_driver_control_translation'];direct=reports['movie_driver_control_verification'];prefix=reports['movie_driver_control_prefix_verification'];reg=reports['movie_driver_control_regression_verification']
 assert t['instruction_rows']==52 and len(t['routines'])==1
 assert direct['cases_passed']==121 and direct['independent_checks']==1978 and direct['status_counts']=={'0':108,'-4':6,'-2':3,'-3':4}
 row,=direct['coverage_by_routine'];assert row['static_rows']==row['covered_rows']==52 and row['conditional_edges_covered']==2 and not row['missing_edges']
 assert prefix['cases_passed']==2 and prefix['cold_profile_reexecuted'] and prefix['independent_checks']==39
 assert reg['total_comparisons']==825 and reg['previous_direct_cases_replayed']==824 and reg['saved_record_checks']==50277
 dll=digest(ROOT/'work/sonic-adventure/build/movie_driver_control.dll');assert all(r['dll_sha256']==dll for r in (direct,prefix,reg))
 cold=next(c for c in prefix['cases'] if c['input']['cold_filesystem']);warm=next(c for c in prefix['cases'] if not c['input']['cold_filesystem'])
 assert cold['stop_pc']=='0x8c65a8c8' and warm['stop_pc']=='0x8c64bddc'
 assert cold['disc_read_attempts']==89 and cold['masaru_payload_matches'] and cold['masaru_sha256_at_stop']==prefix['masaru_sha256'] and cold['control_word']==1
 assert cold['filesystem_root_before']=='0x0' and cold['filesystem_root_after']=='0x8c6946e0' and cold['filesystem_initialized_flag']==1
 assert not warm['disc_read_attempts'] and not warm['masaru_payload_matches']
 assert all(c['status']==-2 and c['clear_native']['writes']==0x200000 for c in prefix['cases'])
 routine=t['routines'][0];assert routine['address']=='0x8c65a200'
 routine=dict(routine,module='1ST_READ.BIN',source='source/sa_movie_driver_control.c',source_sha256=digest(P/'source/sa_movie_driver_control.c'))
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in ('0x8c10d460','0x8c10d770'):
  item=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  canonical.append({k:item[k] for k in ('address','name','source','verification')});files.extend((P/item['source'],P/item['verification']))
 report=dict(routines=[routine],newly_registered_entries=1,qualified_rows=52,executed_rows=52,qualified_conditional_outcomes=2,executed_conditional_outcomes=2,compiled_source_count=179,direct_comparisons=121,independent_direct_checks=1978,prefix_comparisons=2,independent_prefix_checks=39,regression_comparisons=825,regression_record_checks=reg['saved_record_checks'],total_comparisons=948,new_cold_prefix_reexecuted=True,cold_prefix_steps=cold['steps'],cold_prefix_boundaries=cold['boundary_comparisons'],cold_prefix_disc_reads=89,masaru_bytes=33408,masaru_sha256=prefix['masaru_sha256'],masaru_native_load_address='0x0cd00000',canonical_reuse=canonical,reused_additional_credit=0,next_dependency='0x8c65a8c8',next_dependencies={'cold_valid_driver':'0x8c65a8c8','warm_missing_file':'0x8c64bddc'},dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Complete original control-word read, bit-0 set and write, interrupt-mask save/restore and 256-iteration delay. Both existing transfer services execute their real code and receive zero duplicate recovery credit. Fresh valid-driver cold execution reaches original wrapper 65a8c8; warm missing-file control retains 64bddc.',limitations=['Direct initialized channel/context tables, finite FIFO samples and control-register storage are explicit software-host fixtures. No game helper return value is supplied. This does not model physical electrical timing, interrupt acceptance, audio or driver execution.','The independent RAM oracle checks exact shadow/channel/status/counter effects and unchanged remaining RAM outside a 128-byte callee stack window. The primary original/native oracle checks every stack byte and full CPU/FPU/special/banked/device/boundary state.','Fault controls preserve actual partial state but do not implement physical address or privilege exception-vector delivery. Valid SR modes are declared; the normal routine preserves the original interrupt mask and ends with T=1.','Both caller profiles execute newly in this build. Full eight-MiB clear writes retain the established decoder/fingerprint scope, not a cryptographic collision-proof trace. Loaded MASARU bytes are not executed or decompiled.','Earlier CPU-backend tests and older component lifecycle evidence remain historical; 824 earlier direct controls and one earlier warm full-clear startup replay run in the current DLL. The full accumulated historical suite is not claimed.','Natural reset, complete startup, graphics/gameplay, playable ports, requested FPS/resolutions, SUPIR and settings remain unfinished.'])
 (P/'analysis/movie_driver_control_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified original control-word update: 52 rows, two outcomes, 948 comparisons')
if __name__=='__main__':main()
