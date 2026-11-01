"""Qualify original callback bodies and the explicitly bounded software platform."""
import ast,csv,hashlib,json,re
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_list_callbacks_qualification.json';assert not target.exists()
 names=['renderer_list_callbacks_translation','renderer_list_callbacks_connection','renderer_list_callbacks_verification','renderer_list_callbacks_flags_verification','renderer_list_callbacks_controls_verification','renderer_list_callbacks_state_frontier','renderer_list_callbacks_scene_frontier','renderer_list_callbacks_event_bus_verification']
 paths=[P/'analysis'/f'{n}.json' for n in names];translation,connection,unit,flags,controls,empty,scene,events=[json.loads(p.read_text()) for p in paths]
 files=[Path(__file__),*paths]
 for d in (translation,connection,unit,flags,controls,scene,events):
  for name,h in d['sha256'].items():assert sha(name)==h,name;files.append(Path(name))
 assert unit['original_native_comparisons']==44 and unit['instruction_rows_executed']==114 and unit['conditional_edges_executed']==10 and unit['checks']==362
 assert flags['original_native_comparisons']==13 and flags['checks']==96
 assert controls['original_native_comparisons']==14 and controls['checks']==64
 valid=next(c for c in controls['cases'] if c['kind']=='valid');assert valid['status']==0 and valid['runtime_state'][8]==2 and valid['watchdog_calls']==0 and valid['busy_flag']==0 and valid['list_callback_calls']==1
 assert {c['status'] for c in controls['cases'] if c['kind']!='valid'}=={-2,-4}
 assert empty['comparisons']==5 and empty['frontier_pc']=='0x8c601eb8' and empty['frontier_status']==-3 and empty['partial_fault_state_discarded']
 assert empty['phases'][-1]['SDK_watchdog_calls']==0 and empty['phases'][-1]['busy_flag']==0
 assert scene['original_native_comparisons']==24 and scene['checks']==487 and scene['independent_exact_oracles']==437
 assert scene['frontier_pc']=='0x8c5ffcf0' and scene['frontier_status']==-2 and scene['partial_fault_state_discarded']
 assert all(p['status']==0 for p in scene['phases'][:-1])
 assert all(p['full_state_match'] and p['ordered_boundaries_match'] for p in scene['phases'])
 assert scene['renderer_context_from_original_startup'] and scene['matrix_stack_from_original_initializer'] and scene['expected_texture_count']==138
 assert not scene['retail_pixel_trace_values_published'] and not scene['physical_GPU_pixels_qualified'] and not scene['full_game_qualified']
 assert scene['game_callee_outputs_supplied']==scene['allocator_outputs_supplied']==scene['private_execution_frames_supplied']==0
 assert not scene['native_wait_guards_changed']
 frame=scene['phases'][-1];assert frame['SDK_watchdog_calls']==0 and frame['list_callback_calls']==1 and frame['frame_events_state']==[9,0,2,0,3,4,2,0,1]
 assert frame['IRQ_vector_calls']==frame['graphics_handler_calls']==frame['VBlank_handler_calls']==1 and frame['runtime_state'][8]==0
 assert frame['command_packets'][-1][2]==[0]*8
 draw=next(p for p in scene['phases'] if p['label']=='actual stored visible particle draw with loaded textures')
 assert draw['sprite_command_buffers'][0]['bytes']==160
 assert [w for d,q,words in frame['command_packets'] if q==2 for w in words][8:48]==draw['sprite_command_buffers'][0]['words']
 for phase in scene['phases']:
  if phase is draw:assert len(phase['vertex_events'])==50 and {x[0] for x in phase['vertex_events']}<={1,2,3,5}
  else:assert not phase['vertex_events']
 assert events['cases_passed']==53 and events['checks']==357 and not events['game_code_executed'] and not events['physical_timing']
 raw=R/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=raw.read_bytes();assert sha(raw)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 count=0
 for routine in translation['routines']:
  for row in csv.DictReader(Path(routine['listing']).open(),delimiter='\t'):
   a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==int(row['length'])==2 and binary[a-0x8c010000:a-0x8c010000+2]==value;count+=1
 assert count==114 and translation['canonical_reused_entries']==['0x8c604546']
 recipe=P/'build_renderer_list_callbacks.ps1';build=recipe.read_text();sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build)];assert len(sources)==len(set(sources))==286
 assert all(x in build for x in ('-shared','-O2','-Wall','-Wextra','-Werror','SA_BOUND_RETAIL_STALLS=1'))
 old_recipe=(P/'build_renderer_frame_ta.ps1').read_text();old_sources={P/'source'/n for n in re.findall(r"'([^']+\.c)'",old_recipe)}
 assert len(old_sources&set(sources))==282
 assert old_sources-set(sources)=={P/'source/sa_renderer_context_owner_connected.c',P/'source/renderer_context_owner_probe.c'}
 assert set(sources)-old_sources=={P/'source/sa_renderer_list_callbacks_connected.c',P/'source/renderer_list_callbacks_probe.c',P/'source/sa_renderer_list_callbacks.c',P/'source/frame_events_bus_eol.c'}
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/translation['source']).read_text()
 dll=R/'work/sonic-adventure/build/renderer_list_callbacks.dll';lib=dll.with_suffix('.lib');assert dll.read_bytes()[:2]==b'MZ' and lib.read_bytes()[:8]==b'!<arch>\n'
 limits=[
  'Original651f1e copies192bytes to valid mapped VRAM and updates list state/ring/flags/elapsed raw downcounter;604568 clears its global;604570 stores its argument. All114 group rows/10conditional outcomes execute,including canonical604546 withrealoptionalcallback execution. Canonical604546 isreused withzero duplicatecredit.',
  '100 original/native comparisons onthe286-source DLL:90normalreturns and10matched typed stops. Directcallback cases checkall4slots,bothgates/selectors andskip/active states;targetedclear-bit controls independently verifydeferredbit32. All8MiBVRAM andfull machine/device/boundary state remain compared.',
  '53 authored platform-contract cases/357checks onthe sameDLL qualify accepted32-byteTA packets,zero-control EOL withignored stale tail,listbits7/8/9/10/21,acknowledgments andexplicitunsupported domains. Packed/intensity32-byte ordinary vertices only;float/volume headers andsprite/modifier vertices remain unsupported. No ISP parameter generation,rasterization orphysicaltiming.',
  'Original645de0 copiesvectors;originaldisplay startup registers652210atVBR+224andmask6930. Atactual6416ecRTS/delay returns,theauthoredplatform delivers declaredexternalnormal-status observations anddevicepending throughoriginalvector/context/SDKcallbacks/RTE. VBlank observations remain explicitinputs configured beforeeach entry;automatictiming isunfinished.',
  'Originalempty finalization/submission returns aftertwoIRQs withoutSDKwatchdog. Sixmissing-VBlank/mask/BL/disabled controls retain64207e wait;twodamagedvector/handler controls retainfault8cfef000. Originalwholeempty6050b0 nowreachesuninitialized inputread601eb8. This isnotwhole-frame success.',
  'Actual138-texture/player/particle chain passes23normalcalls,thenoriginal6050b0 transfers theexact224bytes/160-bytequad andexecutes listcompletion. Itstops honestly atmissing filesystemcallback5ffcf0 insidegenericIRQdispatch. Handlerhasnotreturned;noIRQdelivery credit forthat unfinishedcycle. Partialfaultstate isdiscarded,never resumed.',
  'The software TA model doesnot produce ISP parameter-buffer contents/current-pointer advancement. The original callback nowwrites background parameter metadata intoVRAM under thedeclared registerprofile;VRAMchanges areobserved andcompared,notproof ofcorrectphysicalGPU allocation orpixels. Preserveprior textureproof;do notclaim texturememory isunchanged acrossIRQhandling.',
  'Standalone callback qualification covers declared valid slots,alignedRAM andmappedVRAM. Malformedmetadata andout-of-domain MMIO/faultvectoring areunqualified. Prior broad/strict-EOL/VRAM-incomplete proposals andtheir researchbuilds remain historical unqualified artifacts. Onlythis qualification identifies theaccepted source/DLL.',
  'Allpreviouspublished source/proof/build bytes arepreserved exceptappend-preservingstatus andregistry additions. No retailinputs/assets orrawpixeltraces included. Approximately10%weightedwhole-projectmilestones,not measured recovered-codecoverage. Fullgame/PCports/FPS8K/Android/stereo3DS/N64/PS1/SUPIR/settings remain unfinished. Goalactive;continueSolHigh.'
 ]
 records=[dict(module='1ST_READ.BIN',name=b['name'],address=b['address'],end_exclusive=b['end_exclusive'],source=translation['source'],retail_span_sha256=b['retail_span_sha256'],verification='analysis/renderer_list_callbacks_qualification.json',status='Complete original callback software-qualified in declared valid domains;all114group rows/10outcomes. Wholeframe andphysicalGPUremain unfinished.',limitations=limits) for b in translation['routines'] if not b['canonical_registered_reuse']]
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1652 and len(records)==3
 pairs={(x.get('module','1ST_READ.BIN'),x['address']) for x in registry};assert len(pairs)==1649 and all((x['module'],x['address']) not in pairs for x in records)
 linked=inventory(dll,registry+records);assert linked['registry_entries']==1655 and linked['registry_unique_module_address_pairs']==1652 and linked['confirmed_linked_unique_routines_by_name']==699
 scripts=sorted({*P.glob('*renderer_list_callbacks*.py'),*P.glob('*renderer_frame*irq*.py'),*P.glob('*renderer_frame*signal*.py'),*P.glob('*renderer_frame_events*.py'),*P.glob('frame_*reference*.py')})
 for p in scripts:ast.parse(p.read_text(encoding='utf-8-sig'),filename=str(p))
 files += [recipe,dll,lib,raw,*sources,*scripts]
 result=dict(status='three_original_callbacks_and_untimed_TA_list_delivery_software_qualified',new_original_routine_credit=3,canonical_reused_entries=['0x8c604546'],new_routine_records=records,result_registry_entries=1655,result_unique_module_address_pairs=1652,linked_routine_inventory=linked,original_native_comparisons=100,normal_returns=90,matched_typed_stops=10,callback_and_scene_checks=1009,scene_independent_exact_oracles=437,platform_contract_cases=53,platform_contract_checks=357,complete_original_instruction_rows=114,complete_conditional_outcomes=10,compiled_source_count=286,preserved_previous_shared_C_inputs=282,fresh_native_compile_performed=True,scene_frontier_pc='0x8c5ffcf0',empty_whole_frame_frontier_pc='0x8c601eb8',whole_frame_qualified=False,physical_GPU_pixels_qualified=False,full_game_qualified=False,retail_pixel_trace_values_published=False,limitations=limits,next_task='Recover5ffcf0filesystemcallbackwithoutstubs andtraceitsactual6005c2tail. Repeatactualtextureframe;theninitializeoriginalinputproducerfor601eb8. ImplementTAparameter/currentpointer state andautomaticVBlanktiming beforephysicalframe/pixelclaims. Preservealltypedcontrols andlegacy639b98invalid-domain obligation.',overall_completion_percent=10,estimate_method='weighted whole-project milestones;not measured recovered-code coverage',goal_paused=False,recommended_model_next_task='Sol High',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('QUALIFIED3 originalcallbacks;1655entries/1652uniquepairs/699exactlinks',flush=True)


if __name__=='__main__':main()
