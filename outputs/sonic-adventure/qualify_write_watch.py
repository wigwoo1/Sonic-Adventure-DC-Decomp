"""Gate opt-in adoption on unit, dual-check direct and four startup executions."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 reports={};files=[Path(__file__)]
 for name in ('windows_write_watch_verification','write_watch_driver_control_verification','write_watch_prefix_verification','movie_driver_control_qualification','movie_bulk_family_inventory'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 unit=reports['windows_write_watch_verification'];direct=reports['write_watch_driver_control_verification'];prefix=reports['write_watch_prefix_verification'];old=reports['movie_driver_control_qualification']
 assert len(reports['movie_bulk_family_inventory']['routines'])==5 and reports['movie_bulk_family_inventory']['new_registry_credit']==0
 assert unit['independent_checks']==1272 and unit['randomized_sequences']==4 and unit['epochs_per_sequence']==97 and unit['full_bytes_compared_at_every_randomized_epoch'] and unit['expected_corruption_and_boundary_errors_rejected']
 assert direct['cases_passed']==121 and direct['saved_record_checks']==7744 and direct['legacy_full_hashes_compared_simultaneously'] and not direct['game_dll_changed']
 assert prefix['cases_passed']==4 and prefix['dual_profiles']==prefix['fast_profiles']==prefix['cold_profiles_freshly_reexecuted']==2 and prefix['all_prior_caller_result_fields_matched']
 dll=digest(ROOT/'work/sonic-adventure/build/movie_driver_control.dll');assert dll==old['dll_sha256']==prefix['dll_sha256']==direct['dll_sha256']
 for c in prefix['cases']:
  assert c['status']==-2 and c['stop_pc']==('0x8c65a8c8' if c['cold_filesystem'] else '0x8c64bddc') and c['clear_writes']==0x200000
  assert c['disc_read_attempts']==(89 if c['cold_filesystem'] else 0)
  if c['cold_filesystem']:assert c['masaru_payload_matches'] and c['masaru_sha256']==old['masaru_sha256']
 registry=P/'analysis/recovered_routines.json';rows=json.loads(registry.read_text())['routines'];assert len(rows)==1379
 report=dict(verification_tool_only=True,newly_registered_entries=0,compiled_game_sources_unchanged=179,game_dll_unchanged=True,dll_sha256=dll,unit_checks=1272,full_byte_randomized_epochs=388,direct_comparisons=121,saved_direct_record_checks=7744,startup_comparisons=4,prefix_saved_record_checks=prefix['independent_saved_record_checks'],fresh_original_native_comparisons=125,registered_routines=1379,registered_unique_pairs=len({(r.get('module','1ST_READ.BIN'),r['address']) for r in rows}),full_goal_complete=False,guide='WINDOWS_WRITE_WATCH.txt',startup_cases=prefix['cases'],sha256={str(p):digest(p) for p in files},scope='Opt-in exact boundary RAM/VRAM checker qualifies against simultaneously retained full-memory hashes and all frozen startup result fields. Every original instruction/callee and recorded boundary still executes; initial/final full bytes and all other state checks remain. No decompilation or port milestone credit.',limitations=['Windows-only allocation/page-tracking API with single owning verification thread and no concurrent writes. One window per allocation. Raw native pointers may not outlive their owner; exported buffer views retain ownership and prevent explicit close.','The proof of equality of untouched pages relies on the documented OS write-watch contract; dirty pages are compared byte-for-byte, including conservative extra pages. Four randomized sequences also compare full bytes at every epoch.','Cold/warm profiles retain explicit finite software-host device inputs and existing complete-clear decoder/fingerprint scope. This does not establish physical interrupt/timing/cache behavior, BIOS decompilation, driver execution or full startup.','Reported elapsed times are observations from one ordered local run, dual then fast, not a controlled performance benchmark or a measurement of model intelligence, usage costs or whole-project completion.','Default historical verifiers and all native game source/build/proof bytes remain preserved. All requested playable ports and settings remain unfinished.'])
 (P/'analysis/write_watch_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified verification-only exact boundary checker; zero new game-routine credit')
if __name__=='__main__':main()
