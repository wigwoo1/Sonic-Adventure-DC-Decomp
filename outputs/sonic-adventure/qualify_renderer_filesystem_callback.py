"""Qualify the original gate, real tail integration and exact firmware observation."""
import ast,csv,hashlib,json,re
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_filesystem_callback_qualification.json';assert not target.exists()
 names=['renderer_filesystem_callback_translation','renderer_filesystem_callback_connection','renderer_filesystem_irq_observer_connection','renderer_filesystem_irq_verification','renderer_filesystem_irq_delivery_verification','renderer_filesystem_irq_controls_verification','renderer_filesystem_irq_event_bus_verification','renderer_filesystem_irq_scene_frontier']
 paths=[P/'analysis'/f'{n}.json' for n in names];t,c,o,unit,delivery,controls,events,scene=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),*paths]
 for d in (t,c,o,unit,delivery,controls,events,scene):
  for path,h in d['sha256'].items():assert sha(path)==h,path;files.append(Path(path))
 assert t['instruction_rows']==7 and t['full_Ghidra_rows_byte_checked']==334 and t['canonical_shared_rows_reused']==327
 assert unit['original_native_comparisons']==40 and unit['gate_cases']==32 and unit['checks']==352 and unit['gate_instruction_rows_executed']==7 and unit['gate_conditional_outcomes_executed']==2
 assert unit['context_from_original_allocation_and_mount'] and not unit['backend_results_supplied']
 assert unit['game_callee_outputs_supplied']==unit['allocator_outputs_supplied']==unit['private_execution_frames_supplied']==0
 assert delivery['original_native_comparisons']==11 and delivery['complete_IRQs']==delivery['IRQ_firmware_calls']==2 and delivery['following_ordinary_firmware_calls']==1
 assert not delivery['guest_execution_changed'] and not delivery['comparisons_removed'] and delivery['host_observer_uses_live_CPU']
 assert controls['original_native_comparisons']==14 and controls['checks']==64 and len(controls['cases'])==9
 assert [x for x in controls['cases'] if x['kind']=='valid'][0]['runtime_state'][8]==2
 assert all(x['watchdog_calls']==0 for x in controls['cases'])
 assert events['cases_passed']==53 and events['checks']==357 and not events['game_code_executed']
 assert scene['original_native_comparisons']==24 and scene['checks']==487 and scene['independent_exact_oracles']==437
 assert scene['frontier_status']==-3 and scene['frontier_pc']=='0x8c601eb8' and scene['partial_fault_state_discarded']
 assert all(x['status']==0 for x in scene['phases'][:-1]) and all(x['full_state_match'] and x['ordered_boundaries_match'] for x in scene['phases'])
 assert scene['expected_texture_count']==138 and scene['renderer_context_from_original_startup'] and scene['matrix_stack_from_original_initializer']
 assert not scene['retail_pixel_trace_values_published'] and not scene['native_wait_guards_changed'] and not scene['physical_GPU_pixels_qualified'] and not scene['full_game_qualified']
 assert scene['game_callee_outputs_supplied']==scene['allocator_outputs_supplied']==scene['private_execution_frames_supplied']==0
 frame=scene['phases'][-1];assert frame['runtime_state'][8]==2 and frame['runtime_state'][9:11]==[0,0] and frame['frame_busy_flag']==0
 assert frame['IRQ_vector_calls']==frame['graphics_handler_calls']==frame['VBlank_handler_calls']==2 and frame['SDK_watchdog_calls']==0 and frame['list_callback_calls']==1
 assert frame['frame_events_state']==[9,0,2,0,3,4,2,0,1]
 for target_pc in ('0x8c5ffcf0','0x8c6005c2','0x8c00f100'):assert sum(b['target']==target_pc for b in frame['boundaries'])==2
 assert all(b['pr']=='0x8c6452de' for b in frame['boundaries'] if b['target']=='0x8c00f100')
 draw=next(x for x in scene['phases'] if x['label']=='actual stored visible particle draw with loaded textures')
 assert draw['sprite_command_buffers'][0]['bytes']==160 and len(frame['command_packets'])==9
 assert [w for address,route,words in frame['command_packets'] if route==2 for w in words][8:48]==draw['sprite_command_buffers'][0]['words']
 for phase in scene['phases']:
  if phase is draw:assert len(phase['vertex_events'])==50 and {x[0] for x in phase['vertex_events']}<={1,2,3,5}
  else:assert not phase['vertex_events']
 raw=R/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=raw.read_bytes();assert sha(raw)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 full=P/'analysis/renderer-filesystem-callback/8c5ffcf0_instructions.tsv';rows=list(csv.DictReader(full.open(),delimiter='\t'));assert len(rows)==334
 for row in rows:
  a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==int(row['length'])==2 and binary[a-0x8c010000:a-0x8c010000+2]==value
 call=0x8c6452da;word=int.from_bytes(binary[call-0x8c010000:call-0x8c010000+2],'little');assert word&0xf0ff==0x400b
 recipe=P/'build_renderer_filesystem_callback.ps1';build=recipe.read_text();sources={P/'source'/n for n in re.findall(r"'([^']+\.c)'",build)};assert len(sources)==287
 assert all(flag in build for flag in ('-shared','-O2','-Wall','-Wextra','-Werror','SA_BOUND_RETAIL_STALLS=1'))
 previous={P/'source'/n for n in re.findall(r"'([^']+\.c)'",(P/'build_renderer_list_callbacks.ps1').read_text())}
 assert len(previous&sources)==284 and previous-sources=={P/'source/sa_renderer_list_callbacks_connected.c',P/'source/renderer_list_callbacks_probe.c'}
 assert sources-previous=={P/'source/sa_renderer_filesystem_callback_connected.c',P/'source/renderer_filesystem_callback_probe.c',P/'source/sa_renderer_filesystem_callback.c'}
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 dll=R/'work/sonic-adventure/build/renderer_filesystem_callback.dll';lib=dll.with_suffix('.lib');assert dll.read_bytes()[:2]==b'MZ' and lib.read_bytes()[:8]==b'!<arch>\n'
 limits=[
  'Complete7-row5ffcf0 gate isqualified forvalid callerrecords anddeclared idle-mounted filesystem inputs. Gate1 returnswithoutRAMwrites;allotherwordvalues tested executeactual existing6005c2 worker.32cases covertwo callerrecords,0/1/2/ffffffff,twoFPUbanks andbothTvalues. Originalallocators andreal21-sectormount produceglobalcontext/tables. No backendresult,allocatoroutput orprivate execution frame supplied.',
  'Ghidra334-rowbody isretained andbyte-checked.327sharedrows areidentical toalreadytranslated6005c2 evidence. Thetail isdelegated tothat realnativeworker;only7newentryrows andone newroutine receivecredit. Pending-I/O branches ofsharedworker arenot newlyqualified bytheidle-context tests.',
  '287-source DLL compiles withWall/Wextra/Werror andpreserves284 shared compilation inputs.89original/nativecomparisons match:80normalreturns and9typedstops. Gate/scene/control checks total903;thetexture scene retains437independent exactoracles.53 authoredTA-event contractcases pass357checks onthissameDLL.',
  'A pre-existing reference-observer defect used thesuspended framePR forfirmware callbacks duringrecursiveIRQ execution. Theadditive correctedobserver reads thelive enteredCPU PR andrestores borrowedobserver selection intry/finally. No guestregister,gameflag,callee result,waitguard orcomparison ischanged/ignored. Actualjsr6452da givesPR6452de. TwoIRQfirmwarecalls plusafollowingordinaryworker call verifyRTEandobserver restoration.',
  'Actual138-texture/player/particle path passes23normalcalls,transfers224bytes includingtheexact160-bytequad,andcompletes twooriginalcopiedvector/context/SDK/filesystemgate/worker/firmware/RTE paths. Busyflagclears throughoriginalcode withoutSDKwatchdog. Full6050b0 stillstops at601eb8becauseinputcontext isuninitialized;thepartialfault isdiscarded,not resumed.',
  'Masks,BL,disabledservice andmissingVBlank retainactualwait64207e;damagedvectors/handler retainfault8cfef000. Validordinarysubmission returnsaftertwoIRQs. Firmwareobserver qualification concerns thetestednormal firmware path;itdoesnot assertalllegacystub observers orarbitrarynestedinterrupt/timing domains.',
  'TAparameterbuffer/current-pointer production,automaticVBlanktiming,rasterization anddisplayedpixels remainunfinished. VBlankandclockobservations aredeclared externalinputs. Originalbackgroundmetadata VRAMwrites match thesoftwareprofile,not proof ofphysicalGPUallocation orunchangedtexturememory acrossIRQprocessing. Float/volume/64-byteTAformats retain explicitunsupported domains.',
  'Oldsource/proof/buildhistory ispreserved. Earliercallbackreference observers andoldmock-backed callbacksource remainhistoricalevidence,not theacceptedIRQintegration. Matching retailinputs/assets/pixeltraces remainprivate. No playableport orfullgame qualification. Approximately10%weightedwhole-projectmilestones,not measured recovered-codecoverage. Goalactive;continueSolHigh.'
 ]
 body=t['routines'][0];record=dict(module='1ST_READ.BIN',name=body['name'],address=body['address'],end_exclusive=body['end_exclusive'],source=t['source'],retail_span_sha256=body['retail_span_sha256'],verification='analysis/renderer_filesystem_callback_qualification.json',status='Complete original filesystem callback gate software-qualified in declared valid domains;real327-rowworker reused withoutduplicatecredit. Fullframe/input/GPUunfinished.',limitations=limits)
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1655 and len({(x.get('module','1ST_READ.BIN'),x['address']) for x in registry})==1652
 assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==record['address'] for x in registry)
 linked=inventory(dll,registry+[record]);assert linked['registry_entries']==1656 and linked['registry_unique_module_address_pairs']==1653 and linked['confirmed_linked_unique_routines_by_name']==700
 scripts=sorted({*P.glob('*renderer_filesystem_callback*.py'),*P.glob('*renderer_filesystem_irq*.py')})
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 files += [recipe,dll,lib,raw,full,*sources,*scripts]
 result=dict(status='one_original_filesystem_callback_and_live_IRQ_firmware_observation_software_qualified',new_original_routine_credit=1,new_routine_records=[record],result_registry_entries=1656,result_unique_module_address_pairs=1653,linked_routine_inventory=linked,original_native_comparisons=89,normal_returns=80,matched_typed_stops=9,gate_scene_control_checks=903,scene_independent_exact_oracles=437,platform_contract_cases=53,platform_contract_checks=357,new_original_instruction_rows=7,gate_conditional_outcomes=2,full_Ghidra_rows_byte_checked=334,shared_canonical_rows_reused=327,shared_tail_new_credit=0,compiled_source_count=287,preserved_previous_shared_C_inputs=284,fresh_native_compile_performed=True,source_matched_research_build='work/sonic-adventure/build/renderer_filesystem_callback.dll',corrected_reference='renderer_filesystem_irq_reference.py',scene_frontier_pc='0x8c601eb8',scene_frontier_status=-3,scene_complete_IRQs=2,scene_real_filesystem_callback_calls=2,scene_real_firmware_calls=2,qualified_whole_frame=False,physical_GPU_pixels_qualified=False,full_game_qualified=False,retail_pixel_trace_values_published=False,limitations=limits,next_task='Trace theoriginal producer ofMaple/inputcontext usedby65f3a0/646868 before601e2a. Initializeit throughoriginalallocators/startup routines,then repeatwholetextureframefromnormal-returnproducers. Neverwriteinputpointer/gameflag,skip601e2a orresumeprivatefaultstate. TAparameter/currentpointer,VBlanktiming,pixels,andlegacy639b98invalid-popguard remain obligations.',overall_completion_percent=10,estimate_method='weighted whole-project milestones;not measured recovered-code coverage',goal_paused=False,recommended_model_next_task='Sol High',sha256={str(path):sha(path) for path in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('QUALIFIED1 originalgate;1656entries/1653unique/700exact-name links',flush=True)


if __name__=='__main__':main()
