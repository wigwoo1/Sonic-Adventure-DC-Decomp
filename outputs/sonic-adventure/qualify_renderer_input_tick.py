"""Qualify original input producers, two callbacks and the declared frame profile."""
import ast,csv,hashlib,json,re
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/renderer_input_tick_qualification.json';assert not target.exists()
 names=['renderer_input_tick_translation','renderer_input_tick_connection','renderer_input_tick_mmio_connection','renderer_input_tick_constructor_verification','renderer_input_tick_error_controls_verification','renderer_input_tick_event_bus_verification','renderer_input_tick_mmio_scene_frontier','renderer_input_tick_clock_scene_verification']
 paths=[P/'analysis'/f'{name}.json' for name in names]
 t,c,mmio,constructor,controls,events,short,scene=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),*paths]
 for report in (t,c,mmio,constructor,controls,events,short,scene):
  for path,h in report['sha256'].items():assert sha(path)==h,path;files.append(Path(path))
 assert t['instruction_rows']==229 and len(t['routines'])==2 and c['compiled_sources']==288
 assert constructor['original_native_comparisons']==constructor['normal_returns']==30 and constructor['checks']==164 and constructor['controller_channels']==24
 assert not constructor['input_context_pointer_supplied'] and constructor['caller_instruction_rows_byte_checked']==109
 assert controls['original_native_comparisons']==controls['normal_returns']==182 and controls['cases']==177 and controls['checks']==1582
 assert all(not x['missing_rows'] and not x['missing_conditional_outcomes'] for x in controls['coverage'])
 assert events['cases_passed']==53 and events['checks']==357
 assert short['frontier_status']==-4 and short['frontier_pc']=='0x8c602612' and short['partial_fault_state_discarded']
 assert short['original_native_comparisons']==25 and all(p['status']==0 for p in short['phases'][:-1])
 assert short['phases'][-1]['input_update_calls']==1 and short['phases'][-1]['runtime_state'][8]==2
 assert scene['whole_6050b0_frame_control_flow_return_qualified'] and scene['normal_visible_draw_qualified']
 assert scene['original_native_comparisons']==26 and all(p['status']==0 for p in scene['phases'])
 assert all(p['full_state_match'] and p['ordered_boundaries_match'] for p in scene['phases'])
 frame=next(p for p in scene['phases'] if p['entry']=='0x8c6050b0')
 assert frame['input_tick_calls']==frame['input_submission_calls']==frame['IRQ_vector_calls']==frame['graphics_handler_calls']==frame['VBlank_handler_calls']==2
 assert frame['input_update_calls']==1 and frame['runtime_state'][8]==2 and frame['runtime_state'][9:11]==[0,0]
 assert frame['frame_busy_flag']==frame['SDK_watchdog_calls']==0
 assert sum(b['target']=='0x8c646868' for b in frame['boundaries'])==24
 assert scene['expected_texture_count']==138 and scene['independent_exact_oracles']>=477
 for report in (constructor,controls,short,scene):
  assert not report.get('physical_GPU_pixels_qualified',False) and report['game_callee_outputs_supplied']==report['allocator_outputs_supplied']==report['private_execution_frames_supplied']==0
 dll=R/'work/sonic-adventure/build/renderer_input_tick_mmio.dll';lib=dll.with_suffix('.lib')
 recipe=P/'build_renderer_input_tick_mmio.ps1';sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",recipe.read_text())]
 assert len(sources)==len(set(sources))==288
 old_recipe=(P/'build_renderer_filesystem_callback.ps1').read_text();old_sources={P/'source'/name for name in re.findall(r"'([^']+\.c)'",old_recipe)}
 assert len(set(sources)&old_sources)==285
 prior=json.loads((P/'analysis/renderer_filesystem_callback_connection.json').read_text())['sha256']
 for path in set(sources)&old_sources:assert sha(path)==prior[str(path)],str(path)
 old=P/'source/sa_renderer_input_tick.c';accepted=P/'source/sa_renderer_input_tick_mmio.c'
 marker=' if(valid_ram(a,n))return 1;'
 assert accepted.read_text()==old.read_text().replace(marker,marker+'\n if(n==4u && !(a&3u) && (a&0x1fffffffu)==0x005f6908u)return 1;')
 assert not c['wait_guards_changed'] and not mmio['wait_guards_changed'] and mmio['guest_instruction_bodies_unchanged'] and mmio['comparison_checks_unchanged']
 binary=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 for body in t['routines']:
  for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   pc=int(row['address'],16);data=bytes.fromhex(row['bytes']);assert raw[pc-0x8c010000:pc-0x8c010000+len(data)]==data
 limitations=[
  'Original 601a34 initializes all 24 input records using mode zero and the two static-buffer literals from byte-checked 010180. Original 646304/65ebb0/65f558 choose the context and buffers; no pointer, allocator output, game result or stopped frame is supplied. The full 010180 reset/startup wrapper is not run.',
  '229 original rows and all 36 conditional outcomes are covered. 177 explicit controls run actual underlying input pump, submission and real retained 05180c no-op callback. Controlled RAM flags and peripheral error/status inputs are unit-test scenarios, not evidence of a physical controller or automatic device timing.',
  'Accepted C adds only the reached 32-bit aligned external error-status register 5f6908 to the candidate guard. The existing PVR bus still owns reads and write-one-to-clear. Earlier guard-incomplete source and DLL remain unqualified historical artifacts. No wait limit or comparison is relaxed.',
  'The real 138-texture/player/particle sequence completes a normal 6050b0 wrapper return, two original IRQ/RTE paths and all 24 input updates, then actual child cleanup. It checks complete CPU/FPU/RAM/VRAM/devices/boundaries, frame counter and final five-word context publication. This qualifies control flow under the declared software profile, not rasterization or displayed pixels.',
  'A separate four-sample clock profile still stops at 602612 with matching typed exhaustion, and is discarded. The successful run starts afresh with a finite monotonic external timer stream and declared VBlank observations. Automatic clock/VBlank timing, sustained frame scheduling, TA parameter/current-pointer production, real controllers and physical pixels remain unfinished.',
  'Input is the original absent-device status domain; arbitrary malformed contexts, asynchronous controller responses, multithreading and physical cache/interrupt timing are not qualified. Legacy 639b98 invalid matrix-pop partial-state obligation remains open. Retail inputs/assets and pixel traces remain private.',
  'Approximately 10% by weighted whole-project milestones, not measured recovered-code coverage. PC frame-rate/resolution features and future ports/settings remain unfinished. Goal active; continue Sol High.'
 ]
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1656
 records=[dict(module='1ST_READ.BIN',name=b['name'],address=b['address'],end_exclusive=b['end_exclusive'],source='source/sa_renderer_input_tick_mmio.c',retail_span_sha256=b['retail_span_sha256'],verification='analysis/renderer_input_tick_qualification.json',status='Complete original entry software-qualified in declared initialized-input and explicit peripheral-control domains; no full-game or physical-pixel claim.',limitations=limitations) for b in t['routines']]
 assert not ({r['address'] for r in registry}&{r['address'] for r in records})
 linked=inventory(dll,registry+records);assert linked['registry_entries']==1658 and linked['registry_unique_module_address_pairs']==1655 and linked['confirmed_linked_unique_routines_by_name']==702
 scripts=sorted({*P.glob('*renderer_input*.py')})
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 files += [recipe,dll,lib,binary,old,accepted,*sources,*scripts]
 result=dict(status='two_original_input_callbacks_and_declared_textured_frame_control_flow_qualified',new_original_routine_credit=2,new_routine_records=records,result_registry_entries=1658,result_unique_module_address_pairs=1655,linked_routine_inventory=linked,compiled_source_count=288,preserved_previous_shared_C_inputs=285,original_instruction_rows=229,conditional_outcomes=36,original_native_comparisons=sum(d['original_native_comparisons'] for d in (constructor,controls,short,scene)),normal_returns=30+182+24+26,matched_typed_stops=1,gate_scene_control_checks=sum(d['checks'] for d in (constructor,controls,short,scene)),platform_contract_cases=53,platform_contract_checks=357,scene_independent_exact_oracles=scene['independent_exact_oracles'],qualified_6050b0_control_flow_return=True,qualified_whole_frame_pixels=False,physical_GPU_pixels_qualified=False,full_game_qualified=False,automatic_clock_or_VBlank=False,scene_complete_IRQs=2,scene_input_channels=24,short_clock_frontier_pc='0x8c602612',retail_pixel_trace_values_published=False,source_matched_research_build=str(dll.relative_to(R)),accepted_reference='renderer_input_tick_mmio_reference.py',accepted_scene_test='verify_renderer_input_tick_clock_scene.py',limitations=limitations,next_task='Replace finite declared timing samples with a reproducible automatic timer/VBlank progression model tied to execution. Preserve the four-sample exhaustion control and every mask/fault guard. Then verify consecutive original frame/draw calls with actual context/TA parameter/current-pointer state before physical rasterization/pixel claims.',overall_completion_percent=10,estimate_method='weighted whole-project milestones; not measured recovered-code coverage',goal_paused=False,recommended_model_next_task='Sol High',sha256={str(path):sha(path) for path in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('QUALIFIED 2 original callbacks; 1658 entries / 1655 unique pairs / 702 exact-name links',flush=True)

if __name__=='__main__':main()
