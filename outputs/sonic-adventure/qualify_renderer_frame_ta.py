"""Qualify authored TA DMA capture,retaining the real frame-completion frontier."""
import ast,hashlib,json,re
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_frame_ta_qualification.json';assert not target.exists()
 paths=[P/'analysis'/f'{n}.json' for n in ('renderer_frame_ta_connection','renderer_frame_ta_dma_verification','renderer_frame_ta_state_frontier','renderer_frame_ta_scene_frontier')]
 connection,core,empty,scene=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
 for d in (connection,core,empty,scene):
  for p,h in d['sha256'].items():assert sha(p)==h,p;files.append(Path(p))
 assert core['cases']==40 and core['checks']==387 and core['independent_exact_oracles']==185
 assert empty['comparisons']==4 and scene['original_native_comparisons']==23
 assert empty['frontier_status']==scene['frontier_status']==-4 and empty['frontier_pc']==scene['frontier_pc']=='0x8c64207e'
 assert empty['partial_fault_state_discarded'] and scene['partial_fault_state_discarded']
 assert scene['checks']==485 and scene['independent_exact_oracles']==437
 assert scene['renderer_context_from_original_startup'] and scene['matrix_stack_from_original_initializer']
 assert scene['native_game_C_unchanged'] and scene['native_backend_is_additive_extension'] and not scene['native_wait_guards_changed']
 assert scene['game_callee_outputs_supplied']==scene['allocator_outputs_supplied']==scene['private_execution_frames_supplied']==0
 assert not scene['retail_pixel_trace_values_published'] and not scene['physical_GPU_pixels_qualified'] and not scene['full_game_qualified']
 phases=scene['phases'];assert all(x['status']==0 for x in phases[:-1]) and phases[-1]['status']==-4
 assert all(x['full_state_match'] and x['ordered_boundaries_match'] for x in phases)
 draw=next(x for x in phases if x['label']=='actual stored visible particle draw with loaded textures');frame=phases[-1];packets=frame['command_packets'];dma=[x for x in packets if x[1]==2]
 assert len(packets)==9 and len(dma)==7 and len(empty['phases'][-1]['command_packets'])==4
 words=[w for x in dma for w in x[2]];buffer=draw['sprite_command_buffers'][0];assert buffer['bytes']==160 and int(buffer['ram_address'],16)==0x8cb7fd20
 assert words[8:48]==buffer['words'] and words[-8:]==[0]*8 and words[0]==0x82000000
 assert [x[0] for x in dma]==list(range(0x10000000,0x100000e0,32))
 calls=frame['boundaries'];transfer=next(x for x in calls if x['target']=='0x8c650bc8');assert transfer['arguments'][:4]==[0x10000000,0xacb7fd00,224,16]
 assert any(x['target']=='0x8c642440' for x in calls) and any(x['target']=='0x8c641f00' for x in calls)
 assert frame['graphics_output']['pvr_pending']==1<<19 and frame['graphics_output']['pvr_active']==0
 assert draw['graphics_input']['vram_sha256']==draw['graphics_output']['vram_sha256']==frame['graphics_output']['vram_sha256']=='9b6b5a3309cb53d20bc01b77d4c7e2884f468f64e32de8da88261ed884bdcdc1'
 for phase in phases:
  if phase is draw:assert len(phase['vertex_events'])==50 and {x[0] for x in phase['vertex_events']}<={1,2,3,5}
  else:assert not phase['vertex_events']
 build=P/'build_renderer_frame_ta.ps1';recipe=build.read_text();sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",recipe)];assert len(sources)==len(set(sources))==284
 assert all(x in recipe for x in ('-shared','-Wall','-Wextra','-Werror','SA_BOUND_RETAIL_STALLS=1'))
 prior=json.loads((P/'analysis/renderer_context_owner_compile_checks.json').read_text())['source_sha256'];old=set(prior);current=set(map(str,sources));assert old-current=={str(P/'source/frame_command_bus.c'),str(P/'source/pvr_dma_bus.c')}
 assert current-old=={str(P/'source/frame_ta_command_bus.c'),str(P/'source/pvr_ta_dma_bus.c')}
 for p in old&current:assert sha(p)==prior[p],p
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];dll=R/'work/sonic-adventure/build/renderer_frame_ta.dll';lib=dll.with_suffix('.lib');assert dll.read_bytes()[:2]==b'MZ' and lib.read_bytes()[:8]==b'!<arch>\n'
 linked=inventory(dll,registry);assert linked['registry_entries']==1652 and linked['registry_unique_module_address_pairs']==1649 and linked['confirmed_linked_unique_routines_by_name']==696
 scripts=sorted(P.glob('*renderer_frame*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 files += [build,dll,lib,*sources,*scripts,P/'analysis/renderer_context_owner_compile_checks.json']
 limits=[
  'Authored untimed TA DMA endpoint captures contiguous32-byte command packets at10000000..107fffff. Native/ref transfer state,MMIO order,payload bytes,partial failures,capacity andboundary guards pass40cases/387checks/185independent oracles. This is platform I/O support,not newly decompiled game routines orTA rasterization.',
  'A fresh284-source DLL/import library builds withWall/Wextra/Werror. Only two authored backend source selections change:frame_ta_command_bus.c andpvr_ta_dma_bus.c. The282shared C sources,all historical source/proof/build bytes andgame routines remain unchanged. CPU/SQ counters andQACR are not changed byDMA. Packet observation route2 meansDMA;0/1retainCPU store-queue routes. Binary struct layout/call signatures remain unchanged.',
  'Original ordinary602128/604702/604dce producers create the context. Four empty-frame comparisons and23 real texture/player/particle comparisons match complete RAM,CPU,FPU,hidden graphics state andevery ordered boundary. The two frame-wrapper calls stop at the actual bounded wait64207e withstatus-4after accepted transfers;their partial states are discarded,not resumed. These27comparisons include25 normal returns andtwo matching typedwait stops,not27 completed frames.',
  'The real scene retains138 independently checked texture entries andboth complete retail hashes. New code preserves the exact8MiBVRAM hash through texture-backed draw andframe transfer. Original642440list finalization,641f00submission and650bc8enqueue emit two SQ packets andsevenDMA packets. DMA sourceacb7fd00/length224contains the exact generated160-byte quad atoffset32,plus anoriginal32-byte list header and32-byte zero terminator. No retail pixel trace values/assets are published.',
  'The existing DMA service raises pending bit19after accepted bytes. TA list-complete events andtheir original interrupt/callback delivery are not implemented inthis stage;641f00still waits andthe frame doesnot complete. No wait guard,game flag,callback result orprivate frame is changed tosatisfy the wait. No GPU image,physical timing,reset boot,scene scheduling orplayable port is claimed. The64-packet research capture bound remains explicit.',
  'The original compact event list lackedextend whenfirst reaching TA-prefetch. An additive adapter appends every supplied event in order;all existing compact clear spans/fullstate comparisons are preserved. A separate short global-clock control honestly stops filesystem mount at602612. Correct frame-clock inputs apply only atentry6050b0,leaving retained filesystem clocks unchanged.',
  'Registry1652entries/1649unique(module,address) pairs/696confirmed exact-name links remain unchanged;zero new routine credit. Whole-project estimate remains approximately10%weighted milestones,not measured recovered-code coverage. Goal active;continue Sol High for TA list completion andactualoriginal IRQ/callback delivery. Keep the invalid/uninitialized legacy matrix-stack guard obligation explicit. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR/settings remain planned;Texture Off disables sampling anduses vertex/material colours.'
 ]
 report=dict(status='TA_DMA_packet_transfer_software_qualified_whole_frame_completion_pending',new_original_routine_credit=0,result_registry_entries=1652,result_unique_module_address_pairs=1649,linked_routine_inventory=linked,compiled_source_count=284,fresh_native_compile_performed=True,strict_compile_flags=['-Wall','-Wextra','-Werror'],preserved_shared_C_sources=282,authored_backend_replacements=2,original_game_C_changes=0,added_build_files=2,backend_contract_cases=40,backend_contract_checks=387,backend_independent_oracles=185,original_native_comparisons=27,normal_original_call_returns=25,matched_typed_wait_stops=2,empty_frame_comparisons=4,scene_comparisons=23,scene_checks=485,scene_independent_oracles=437,valid_texture_entries_asserted=138,ram_command_bytes=160,TA_DMA_bytes=224,TA_DMA_packets=7,total_TA_packets=9,exact_quad_payload_preserved=True,actual_original_DMA_arguments=transfer['arguments'][:4],DMA_pending=1<<19,TA_list_interrupts_implemented=False,whole_frame_returns=False,frontier_pc='0x8c64207e',frontier_status=-4,physical_GPU_pixels_qualified=False,full_game_qualified=False,retail_pixel_trace_values_published=False,uploaded_texture_vram_sha256=frame['graphics_output']['vram_sha256'],primary_contracts=connection['primary_contracts']+['https://kos-docs.dreamcast.wiki/asic_8h.html'],overall_completion_percent=10,estimate_method='weighted whole-project milestones;not measured recovered-code coverage',goal_paused=False,recommended_model_next_task='Sol High',next_task='Implement/qualify TA list completion fromactual accepted commands anddeliver events throughactualoriginal handlers/IRQ/RTE/callbacks. Preserve exact normal/partial states andallwait guards;do not directly clear8a30c0orclaimpixels frompacket capture. Keep legacy matrix-underflow typed-guard obligation.',limitations=limits,source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED TA DMA40cases;27original/native comparisons include2matching waitfrontiers;framecompletion pending',flush=True)


if __name__=='__main__':main()
