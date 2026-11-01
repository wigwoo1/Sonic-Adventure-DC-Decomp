"""Qualify actual renderer/matrix producers in one scoped textured draw chain."""
import ast,csv,hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]


def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_context_producer_qualification.json';assert not target.exists()
 paths=[P/'analysis'/name for name in ('renderer_context_producer_state_verification.json','renderer_context_producer_matrix_verification.json','renderer_context_producer_frontier.json','renderer_context_owner_compile_checks.json')]
 state,scene,negative,compiler=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
 for d in (state,scene,negative,compiler):
  for path,value in d['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 for path,value in compiler['source_sha256'].items():assert sha(path)==value,path
 assert state['original_native_comparisons']==12 and state['checks']==76 and state['independent_exact_oracles']==40
 assert scene['original_native_comparisons']==23 and scene['checks']==487 and scene['independent_exact_oracles']==438
 assert scene['renderer_context_from_original_startup'] and scene['matrix_stack_from_original_initializer'] and not scene['renderer_context_is_declared_input']
 assert scene['native_DLL_unchanged'] and scene['CPU_executor_unchanged'] and not scene['native_wait_guards_changed']
 assert scene['game_callee_outputs_supplied']==scene['allocator_outputs_supplied']==scene['private_execution_frames_supplied']==0
 assert scene['valid_texture_descriptors']==138 and len({x['pointer'] for x in scene['descriptors']})==136
 assert all(x['error']==0 and x['references']>0 for x in scene['descriptors'])
 assert not scene['retail_pixel_trace_values_published'] and not scene['physical_GPU_pixels_qualified'] and not scene['full_game_qualified']
 phases=scene['phases'];context=phases[2];draw=next(x for x in phases if x['label']=='actual stored visible particle draw with loaded textures')
 assert all(x['status']==0 and x['full_state_match'] and x['ordered_boundaries_match'] for x in phases)
 assert context['polygon_context_root']==draw['polygon_context_root']=='0x8c8a33c0'
 boundary=next(b for b in context['boundaries'] if b['target']=='0x8c656940');record=bytes(boundary['graphics_input_record']);owner=int.from_bytes(record[36:40],'little')&0x8fffffff
 alloc=next(b for b in context['boundaries'] if b['target']=='0x8c6021d6');requested=alloc['arguments'][0];assert requested==4*(100+0x20000)+0x160
 buffers=draw['sprite_command_buffers'];assert len(buffers)==1 and buffers[0]['bytes']==160;start=int(buffers[0]['ram_address'],16)
 assert start==owner+32==context['polygon_context_words'][2]&0x8fffffff
 assert owner<=start<start+160<=owner+requested and draw['polygon_context_words'][2]&0x8fffffff==start+160
 assert [buffers[0]['words'][8+8*i] for i in range(4)]==[0xe0000000]*3+[0xf0000000]
 assert draw['graphics_output']['qacr']==[12,12]
 for phase in phases:
  if phase is draw:assert len(phase['vertex_events'])==50 and {x[0] for x in phase['vertex_events']}<={1,2,3,5}
  else:assert not phase['vertex_events']
 assert negative['frontier_status']==-2 and negative['frontier_pc']=='0x0' and negative['frontier_label']=='original filesystem mount' and negative['partial_fault_state_discarded']
 rawpath=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=rawpath.read_bytes();assert sha(rawpath)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 listings=[P/'analysis'/folder/f'{address}_instructions.tsv' for folder,address in [('movie-startup-driver-frontier','8c604702'),('movie-main-setup','8c604dce'),('movie-command-handoff-next','8c656940'),('renderer-matrix-initializer','8c638fe4')]]
 rows=0
 for listing in listings:
  for row in csv.DictReader(listing.open(),delimiter='\t'):
   a=int(row['address'],16);value=bytes.fromhex(row['bytes']);assert len(value)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==value;rows+=1
 assert rows==1163
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1652
 canonical=next(x for x in registry if x['address']=='0x8c638fe4');assert canonical['name']=='sa_frame_setup_8c638fe4'
 dll=R/'work/sonic-adventure/build/renderer_context_owner.dll';linked=inventory(dll,registry);assert linked['registry_unique_module_address_pairs']==1649 and linked['confirmed_linked_unique_routines_by_name']==696
 scripts=sorted(P.glob('*renderer_context_producer*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 files += [*scripts,*listings,rawpath,dll,P/'source/sa_draw_frame_dependencies.c',P/'analysis/draw_frame_setup_verification.json',P/'analysis/renderer-matrix-context/references.tsv',P/'analysis/renderer-matrix-context/resources.tsv',P/'ghidra/export_renderer_matrix_initializer.ps1',P/'ghidra/export_renderer_matrix_context_references.ps1',P/'ghidra/ExportResourceReferences.java']
 limits=[
  'Actual original602128 system heap,604702 display/config record and604dce/656940 allocation create polygon root88fcd4 from null. No polygon root,cursor orview dimensions are supplied. The record is88fdb4,its+20hex field contains the root output address,andoriginal allocation creates the uncached owned buffer.',
  '12 producer comparisons/76 checks/40 independent oracles cover both FPU banks and nearest/zero rounding. Independent formulas verify normalized counts,allocation bytes,descending heap position,allocated/free32-byte units,uncached alias,record output field andactual command cursor.',
  'The complete scoped resource/player/particle chain passes23 comparisons/487 checks/438 independent oracles. Original display setup is not repeated after texture loading;real allocator state andall8MiB graphics state persist. All138 texture entries/136 distinct descriptors have zero error andexact reference counts. Actual160-byte command buffer8cb7fd20lies inside the actual owned allocation andadvances the original context cursor by160 bytes.',
  'The raw no-BIOS-vector control returns a matched null-target stop during filesystem mount. The successful run explicitly binds the existing external host-disc service vector8c0000bc to8c00f100. This is a declared software platform contract,not recovered BIOS or a game-helper result.',
  'Removing the old geometry fixture also exposed absent matrix-stack initialization. At particle spawn,original execution stops at639bbe after unsigned depth underflow;the old unchecked legacy native pop continues. Preserve this invalid/uninitialized-domain mismatch and do not count it as equivalent execution. The successful run uses an actual calloc buffer andthe already registered/connected canonical638fe4 initializer before spawning. No fake stack cursor/depth/capacity ormatrix is installed.',
  'The current284-source DLL andevery existing C source remain unchanged;retained strict Wall/Wextra/Werror evidence andsource hashes are validated.1163 static original instruction rows of604702/604dce/656940/638fe4 are byte-checked;this does not claim full dynamic branch coverage ornew routine credit. Registry1652entries/1649unique pairs/696confirmed exact-name links remain unchanged. A redundant unpublished matrix recovery proposal is privately preserved withhashes andexcluded from public selection.',
  'GPU identity,display-mode,scan/IRQ samples,host-disc policy,caller arena/counts andscene inputs remain declared. External timing/sample services are per-call scenario inputs,not complete physical-platform continuity. Geometry is staged inRAM;no frame is consumed ordisplayed. Broad initializer domains,invalid legacy memory guards,original reset/scene scheduling,physical GPU andcomplete game/ports remain unfinished. Retail assets/raw pixel traces remain private.',
  'Whole-project estimate remains approximately10% by weighted milestones,not measured recovered-code coverage. Goal active;continue Sol High. Next trace original frame submission/consumption fromthe actual context and160-byte buffer. Retain the matrix-underflow guard obligation. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR/settings remain planned;Texture Off disables sampling anduses vertex/material colours.'
 ]
 result=dict(status='actual_renderer_and_matrix_producers_integrated_into_original_textured_particle_RAM_command_path',new_original_routine_credit=0,result_registry_entries=1652,result_unique_module_address_pairs=1649,linked_routine_inventory=linked,primary_original_native_comparisons=35,producer_comparisons=12,producer_checks=76,producer_independent_oracles=40,scene_comparisons=23,scene_checks=487,scene_independent_oracles=438,static_original_rows_byte_checked=1163,full_initializer_dynamic_domains_qualified=False,renderer_context_from_original_startup=True,matrix_stack_from_original_initializer=True,canonical_matrix_initializer='sa_frame_setup_8c638fe4',compiled_source_count=284,fresh_native_compile_performed=False,native_DLL_preserved=True,added_build_files=0,valid_texture_entries=138,unique_texture_descriptors=136,ram_command_bytes=160,command_buffer=buffers[0],actual_owned_pointer=hex(owner),actual_allocation_requested_bytes=requested,uploaded_texture_vram_sha256=scene['uploaded_texture_vram_sha256'],persistent_vram_bytes=8388608,raw_BIOS_vector_negative_control_retained=True,uninitialized_matrix_stack_native_reference_mismatch_remains_outside_success_domain=True,physical_GPU_pixels_qualified=False,full_game_qualified=False,retail_pixel_trace_values_published=False,overall_completion_percent=10,estimate_method='weighted whole-project milestones;not measured recovered-code coverage',goal_paused=False,recommended_model_next_task='Sol High',next_task='Trace actual original frame submission/consumption from real polygon context8a33c0andowned command buffer8cb7fd20. Preserve all prior comparisons andthe separately documented invalid matrix-stack guard obligation;do not claim displayed pixels or reset boot.',limitations=limits,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('QUALIFIED35 comparisons;actual context/matrix producers plus23-call textured draw;zero newroutine credit',flush=True)


if __name__=='__main__':main()
