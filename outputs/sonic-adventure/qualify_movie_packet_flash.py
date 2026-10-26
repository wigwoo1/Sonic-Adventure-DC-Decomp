"""Qualify eight complete packet bodies; keep three settings outcomes uncredited."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
COMPLETE={'0x8c604044','0x8c604148','0x8c604250','0x8c603d22','0x8c603fe4','0x8c603c08','0x8c603ec4','0x8c603e08'}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_packet_startup_translation','movie_packet_startup_connection','movie_packet_flash_connection','movie_packet_flash_verification','movie_packet_flash_errors_verification','movie_packet_flash_retained_verification','flash_storage_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_packet_startup_translation'];direct=reports['movie_packet_flash_verification'];errors=reports['movie_packet_flash_errors_verification'];retained=reports['movie_packet_flash_retained_verification'];backend=reports['flash_storage_verification']
 assert len(t['routines'])==24 and t['instruction_rows']==1011
 assert reports['movie_packet_flash_connection']['compiled_source_count']==196
 assert direct['cases_passed']==360 and direct['independent_checks']==2368
 assert errors['cases_passed']==63 and errors['independent_checks']==393
 assert retained['total_original_native_comparisons']==711
 assert backend['continuous_service_comparisons']==338 and backend['independent_checks']==1772
 assert digest(ROOT/'work/sonic-adventure/build/flash_storage.dll')==backend['dll_sha256']
 dll=digest(ROOT/'work/sonic-adventure/build/movie_packet_flash.dll')
 assert all(r['dll_sha256']==dll for r in (direct,errors,retained))
 executed=set(errors['executed_rows']);edges=set(map(tuple,errors['executed_outcomes']));coverage=[]
 for row in direct['coverage_by_routine']:
  missing=[a for a in row['missing_rows'] if a not in executed]
  missing_edges=[a for a in row['missing_outcomes'] if tuple(a) not in edges]
  coverage.append(row|dict(missing_rows=missing,missing_outcomes=missing_edges,covered_rows=row['static_rows']-len(missing),covered_outcomes=row['conditional_outcomes']-len(missing_edges)))
 assert not any(r['missing_rows'] for r in coverage)
 assert sum(len(r['missing_outcomes']) for r in coverage)==3
 assert all(not r['missing_outcomes'] for r in coverage if r['address']!='0x8c64bddc')
 complete=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines'] if r['address'] in COMPLETE]
 assert len(complete)==8 and sum(r['instruction_rows'] for r in complete)==610 and sum(len(r['conditional_edges']) for r in complete)==98
 partial=next(r for r in coverage if r['address']=='0x8c64bddc')
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for r in t['routines']:
  if r['address'] in COMPLETE or r['address']==partial['address']:continue
  canonical=next(row for row in registry if row.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and row['address']==r['address'])
  reuse.append({k:canonical[k] for k in ('address','name','source','verification','status')})
  files.extend((P/canonical['source'],P/canonical['verification']))
 assert len(reuse)==15
 limitations=[
  'Eight original bodies qualify610own instruction rows/98conditional outcomes. All24integrated bodies execute1011rows. Original settings64bddc executes113rows/25of28outcomes and remains partial/unregistered; read-pending,read-poll-failure and write-request-error outcomes are not qualified. Fifteen existing canonical bodies receive zero duplicate credit; their source/proof/name/status fields remain unchanged.',
  '423fresh direct original/native comparisons and711fresh earlier request/runtime/regression/ABI comparisons use the196-source DLL.338additional authored flash-service comparisons receive zero original-code credit. Actual raw flash metadata/bytes,range/header/version/CRC errors,atomic program rejection and prior successful bitmap writes are tested without supplying helper return values.',
  'The standalone host flash service accepts explicit mutable bytes and disjoint partition extents. It is authored support,not a recovered BIOS. Zero-to-one requests fail atomically before writes; physical programming failure/timing/protection and persistent filesystem saving are unmodeled. No default console identity/game settings are manufactured by the Cservice.',
  'Direct settings fixtures optionally invoke authored SYSINFO initialization on both sides before either execution/memory snapshot. These are explicit unit fixtures,not natural startup proof. All masks and two genuine pending write stages before completed compaction are checked. Production polling remains original/unbounded; research read/write guards stop at2/3continuing tests. The write guard admits both legitimate pending phases.',
  'The verification model suppresses duplicate function-entry events when original bottom-tested byte helpers revisit their entry internally. All original instructions still execute; native code is unchanged by this event-accounting correction. Optional platform priming was moved before both runs and memory baselines,not used to patch a reached game result.',
  'This qualification covers direct/retained domains only. Separate configured earlier-startup evidence,erased-flash retries and fresh cold startup must be assessed in their own reports; no ongoing capture or old full-copy result substitutes for completed evidence. Full reset/boot,physical timing,ARM/audio/pixels/gameplay remain unfinished.',
  'Full-source/publicGitHub/documentation,PC60/120/240FPS/up to8K,Android/stereoscopic3DS,N64/PlayStation adaptations,SUPIRHDtoggle/texturepercent/render/aspect goals remain unfinished. TextureOff must use vertex colors. Overall10% is weighted milestones,not measured code coverage.'
 ]
 report=dict(routines=complete,partial_settings=partial,canonical_reuse=reuse,coverage_by_routine=coverage,newly_registered_entries=8,newly_qualified_rows=610,qualified_conditional_outcomes=98,executed_integration_rows=1011,reused_canonical_entries=15,reused_additional_credit=0,partial_settings_additional_credit=0,compiled_source_count=196,direct_original_native_comparisons=423,retained_cases_replayed_fresh=711,total_original_native_comparisons=1134,independent_direct_checks=2761,authored_flash_service_cases=338,authored_flash_independent_checks=1772,authored_support_recovery_credit=0,dll_sha256=dll,sha256={str(p):digest(p) for p in files},limitations=limitations,scope='Eight complete original flash-packet bodies; real native storage bytes and caller/control paths. Partial settings outcomes,existing canonical bodies and authored platform support receive no additional retail credit. Earlier/full startup evidence is separate.')
 (P/'analysis/movie_packet_flash_qualification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Qualified8neworiginals610rows98outcomes;1134freshoriginal/native;338authoredcases;settings3outcomesremainpartial')
if __name__=='__main__':main()
