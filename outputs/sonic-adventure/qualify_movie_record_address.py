"""Qualify two canonical integrations; preserve the existing registry byte-for-byte."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 reports={};files=[Path(__file__)]
 for name in ('movie_record_address_translation','movie_record_address_connection','movie_record_address_verification','movie_record_address_retained_verification','movie_record_address_startup_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_record_address_translation'];d=reports['movie_record_address_verification'];retained=reports['movie_record_address_retained_verification'];startup=reports['movie_record_address_startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_record_address.dll')
 assert len(t['routines'])==2 and t['instruction_rows']==9 and reports['movie_record_address_connection']['compiled_source_count']==212
 assert (d['cases_passed'],d['independent_checks'],d['executed_rows'],d['executed_outcomes'])==(352,6336,9,0)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(35,335)
 assert (startup['cases_passed'],startup['independent_checks'],startup['completed_entry'],startup['current_warm_boundaries'])==(1,68,'0x8c0546c0',4702)
 assert not startup['fresh_cold_profile'] and not retained['historical_cold_profile']['freshly_rerun']
 assert all(r['dll_sha256']==dll for r in (d,retained,startup))
 assert all(not r['missing_rows'] for r in d['coverage_by_routine'])
 registry_path=P/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text())['routines'];assert len(registry)==1469;canonical=[]
 for routine in t['routines']:
  matches=[r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)&0x1fffffff==int(routine['address'],16)&0x1fffffff];assert len(matches)==1
  old=matches[0];assert old['retail_span_sha256']==routine['retail_span_sha256'];files.extend((P/old['source'],P/old['verification']));canonical.append(dict(address=routine['address'],canonical_name=old['name'],additional_recovery_credit=0))
 prior=P/'analysis/movie_record_register_qualification.json';previous=json.loads(prior.read_text());assert previous['total_evidence_original_native_comparisons']==8036;files.extend((prior,registry_path))
 report=dict(newly_registered_entries=0,newly_qualified_rows=0,reused_additional_credit=0,canonical_reuse=canonical,qualified_integration_rows=9,executed_integration_rows=9,executed_conditional_outcomes=0,compiled_source_count=212,coverage_by_routine=d['coverage_by_routine'],new_direct_original_native_comparisons=352,new_direct_independent_checks=6336,fresh_retained_original_native_comparisons=35,fresh_retained_independent_checks=335,fresh_warm_startup_comparisons=1,fresh_warm_startup_checks=68,fresh_original_native_comparisons=388,fresh_independent_checks=6739,retained_previous_evidence_original_native_comparisons=8036,total_evidence_original_native_comparisons=8424,registry_entries_unchanged=1469,registry_sha256=digest(registry_path),completed_entry='0x8c0546c0',completion_domain='Fresh uninterrupted configured warm earlier-caller entry only; not a full game boot.',declared_return_pc='0x8cfed000',warm_boundaries=4702,historical_cold_profile=retained['historical_cold_profile'],fresh_cold_profile=False,next_task='Expand to earlier caller0x8c053ac0 and its dependency chain.',partial_unregistered_dispatcher='0x8c652210',remaining_unqualified_graphics_root_outcomes=previous['remaining_unqualified_graphics_root_outcomes'],dll_sha256=dll,limitations=['Two previously canonical routines, zero new registry/instruction credit. All9 integration rows execute. Independent modulo32 arithmetic/overflow/MACL/MACH/GBR/SR/GPR/FPU/fullRAM and ordered-boundary expectations pass for352 cases. Native MACL is observed after execution, never substituted.','Thirty-two context vectors andthree reached SR/FD fault prefixes replay unchanged earlier oracles in the212-source DLL. Legacy model REGISTER_ROWS guards, provider/runtime/API layouts and old sources/proofs remain intact. Other historical cases are not all rerun.','One fresh configured0546c0 warm startup returns normally after4702 boundaries, preserving exact prior211 recorded fields and full original/native state. Previous211 full cold profile585993 boundaries is retained unchanged, not rerun or relabeled as212. No full-game/reset/hardware/gameplay claim.','Whole game/ports/FPS8K/SUPIR/settings unfinished; Texture Off must disable sampling/use vertex colors. Goal ACTIVE about10% weighted milestones, not code coverage.'],sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_record_address_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED canonical2/9rows/zero extra credit;388 freshcases6739checks;warmcaller returns')
if __name__=='__main__':main()
