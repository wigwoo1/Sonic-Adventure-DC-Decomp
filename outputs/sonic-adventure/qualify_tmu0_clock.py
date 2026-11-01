"""Qualify the timer foundation without claiming frame scheduling or recovery credit."""
import ast,hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/tmu0_clock_qualification.json';assert not target.exists()
 paths=[P/'analysis/tmu0_clock_verification.json',P/'analysis/tmu0_original_helpers_verification.json']
 core,helpers=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),*paths]
 for d in (core,helpers):
  for path,h in d['sha256'].items():assert sha(path)==h,path;files.append(Path(path))
 assert core['random_operations']==27000 and core['checks']==90677 and core['large_interval_partition_cases']==1000 and core['reload_boundary_profiles']==80
 assert helpers['original_native_comparisons']==helpers['normal_returns']==68 and helpers['checks']==780 and helpers['original_rows_byte_checked']==24
 assert helpers['initialization_from_original_instructions'] and helpers['original_native_game_DLL_unchanged'] and not helpers['finite_clock_samples_used']
 assert not core['reads_advance_time'] and not helpers['reads_advance_time']
 assert helpers['game_callee_outputs_supplied']==helpers['allocator_outputs_supplied']==helpers['private_execution_frames_supplied']==0
 registry=P/'analysis/recovered_routines.json';routines=json.loads(registry.read_text())['routines']
 game=R/'work/sonic-adventure/build/renderer_input_tick_mmio.dll';linked=inventory(game,routines)
 assert linked['registry_entries']==1658 and linked['registry_unique_module_address_pairs']==1655 and linked['confirmed_linked_unique_routines_by_name']==702
 old=json.loads((P/'analysis/renderer_input_tick_qualification.json').read_text())
 for name,h in old['sha256'].items():assert sha(name)==h,name
 for p in (P/'tmu0_clock_native.py',P/'tmu0_clock_reference.py',P/'verify_tmu0_clock.py',P/'verify_tmu0_original_helpers.py',Path(__file__)):ast.parse(p.read_text(),filename=str(p))
 dll=R/'work/sonic-adventure/build/tmu0_clock.dll'
 files += [dll,dll.with_suffix('.lib'),P/'build_tmu0_clock.ps1',P/'analysis/renderer_input_tick_qualification.json']
 result=dict(status='elapsed_PCLK_TMU0_foundation_and_original_leaf_helpers_qualified',new_original_routine_credit=0,linked_routine_inventory=linked,native_platform_core_sources=1,native_game_DLL_unchanged=True,random_operations=27000,platform_checks=90677,large_interval_partition_cases=1000,original_native_comparisons=68,normal_returns=68,original_helper_checks=780,original_rows_byte_checked=24,reads_advance_time=False,finite_clock_samples_used_for_helper_tests=False,whole_scene_timer_integration_qualified=False,automatic_VBlank=False,physical_clock_rate_qualified=False,guest_instruction_cycle_assignment_qualified=False,previous_declared_profile_frame_proof_preserved=True,reference_document=dict(url='https://www.renesas.com/en/document/mah/sh7750-sh7750s-sh7750r-group-users-manual-hardware',document='R01UH0456EJ0702 Rev 7.02',sections='12.2-12.5',printed_pages='341-356'),limitations=core['limitations']+['The full existing frame pipeline still uses declared timer/VBlank profiles. This foundation is not the completed automatic timing or frame-loop integration.','Stopped-to-running clock phase is an explicit virtual clock-domain reset profile, not a physical board measurement. Unsupported operations return research status -2; this is not an assertion that real hardware raises that fault.','No game routine is newly recovered or linked by adding authored platform support. Existing 1658 registry entries / 1655 unique pairs / 702 exact-name links remain unchanged. No playable port, pixel, frame-rate or resolution feature is qualified.'],next_task='Bind the persistent TMU0 state into the complete native/original scene comparison. Assign elapsed PCLK from a documented scheduler clock domain rather than timer read count, preserve old exhaustion controls, implement scan/VBlank progression from original display registers, and verify consecutive original frame/draw calls before pixel claims.',overall_completion_percent=10,estimate_method='weighted whole-project milestones; not measured code coverage',goal_paused=False,recommended_model_next_task='Sol High',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('QUALIFIED TMU0 foundation: 90677 platform checks + 68 original comparisons; zero new routine credit',flush=True)

if __name__=='__main__':main()
