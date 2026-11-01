"""Qualify a scoped original texture/particle command path without new routine credit."""
import ast,hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_draw_qualification.json';assert not target.exists()
 paths=[P/'analysis'/n for n in ('player_texture_draw_state_verification.json','player_texture_draw_verification.json','player_texture_draw_commands_verification.json','player_texture_link_compile_checks.json')];state,scene,commands,compiler=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
 for report in (state,scene,commands,compiler):
  for path,value in report['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 for path,value in compiler['source_sha256'].items():assert sha(path)==value,path
 assert state['comparisons']==3 and state['preserved_vram_bytes']==8388608 and state['exact_payload_bytes']==128
 assert scene['original_native_comparisons']==commands['original_native_comparisons']==20 and scene['checks']==468 and scene['independent_exact_oracles']==425
 assert commands['checks']==472 and commands['independent_exact_oracles']==429
 assert scene['valid_texture_descriptors']==commands['valid_texture_descriptors']==138 and compiler['source_count']==283
 assert commands['persistent_graphics_device_state'] and not commands['physical_GPU_pixels_qualified'] and not commands['full_game_qualified']
 for d in commands['descriptors']:assert d['error']==0 and d['references']>0
 draw=next(x for x in commands['phases'] if x['label']=='actual stored visible particle draw with loaded textures');buffers=draw['sprite_command_buffers'];assert len(buffers)==1 and buffers[0]['bytes']==160
 assert [buffers[0]['words'][8+8*i] for i in range(4)]==[0xe0000000]*3+[0xf0000000]
 assert any(x['target']=='0x8c606a88' and x['arguments'][0]==132 for x in draw['boundaries'])
 assert draw['status']==0 and draw['graphics_input']['vram_sha256']==draw['graphics_output']['vram_sha256']==commands['uploaded_texture_vram_sha256']
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1648
 dll=R/'work/sonic-adventure/build/player_texture_link.dll';linked=inventory(dll,registry);assert linked['confirmed_linked_unique_routines_by_name']==692
 scripts=sorted(P.glob('*player_texture_draw*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 files+=scripts
 limits=[
  'One ordinary-entry original resource/player/child/particle command path is software-qualified forplayer0,character0,platform mode0,FPU bank0/nearest rounding,declared shape(0,1,10),scene flag2/height100,and640x480view. This is not all scene inputs,all sprite flags,all texture formats orcomplete renderer qualification.',
  'Original heap/calloc/filesystem mount/directory,SDK table andheap initialization,graphics memory-manager663140,driver default installation,and099750/0997bc/608134execute. Complete compressed/decoded retail hashes match;independent PVMH109 parsing verifies all138global IDs,136distinct descriptors,zero errors andexact duplicate-ID reference counts.',
  'The previous per-call checker recreated video memory. An additive test adapter now carries exactly matched normal-return graphics state,including all8MiBVRAM,GPU registers,store queues,DMA and sound payloads. Original CPU/native execution,all fullstate/boundary comparisons andnative wait guards are unchanged. The independent3-comparison128-byte VRAM/register producer test proves continuity. External timing/sample services retain their declared scenario inputs;this is not a complete physical-platform continuity claim.',
  'The retained first scene run20comparisons/468checks andcurrent read-only command-capture run20comparisons/472checks both pass. Primary current evidence is23comparisons:20scene plus3continuity tests,with472counted scene checks/429independent scene oracles. Do not add the earlier20scene comparisons asdistinct gameplay coverage. Actual original task/player/collision/child/spawn/view/stored draw/cleanup execute;no allocator or game-callee result orprivate execution frame is supplied.',
  'Stored draw0b3f2cpasses the former606a9etexture fault,binds actual EFF_REGULARentry132/global index184f,and writes one160-byte RAM command buffer:32-byte header plusfour32-byte vertices,threeE0000000commands andoneF0000000strip end. Every RAM byte andordered boundary matches independently decoded original instructions. Uploaded VRAM remains unchanged throughout player/view/draw. Cleanup consumes the actual allocated particle list.',
  'This is RAM command staging with actual QACR12/12,not a submitted GPU frame ordisplayed image. The renderer command-buffer context,matrix andscene inputs aredeclared fixture inputs;full original frame-buffer/context producers,submission/consumption,scheduler,physical GPU pixels andcomplete game/ports remain unfinished. Scene/camera producers andthe full09e778/09e880sprite-family domains remain obligations.',
  'No new original routine is credited andno C source/DLL is changed. The283-source build andall source hashes/retained strict compile evidence arepreserved;new Python code parses. Registry remains1648entries/1645unique module-address pairs/692confirmed exact-name links. No new native build is claimed.',
  'Whole-project estimate remains approximately10%weighted milestones,not measured recovered-code coverage. Incomplete research artifacts,not playable. Goalactive/continueSolHigh. Next trace the original command-buffer producer andframe submission/consumption fromreal callers. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR/settings remain planned;TextureOff disables sampling anduses vertex/material colours.'
 ]
 report=dict(status='original_texture_resource_and_allocated_particle_RAM_command_path_software_qualified_in_declared_domain',new_original_routine_credit=0,result_registry_entries=1648,result_unique_module_address_pairs=1645,linked_routine_inventory=linked,primary_original_native_comparisons=23,current_scene_comparisons=20,current_scene_checks=472,current_scene_independent_oracles=429,continuity_comparisons=3,additional_retained_scene_comparisons=20,additional_retained_scene_checks=468,valid_texture_entries=138,unique_texture_descriptors=len({x['pointer'] for x in commands['descriptors']}),zero_error_texture_entries=138,actual_sprite_texture_index=132,actual_sprite_global_index='0x184f',ram_command_bytes=160,vertices=4,command_buffer=buffers[0],uploaded_texture_vram_sha256=commands['uploaded_texture_vram_sha256'],persistent_vram_bytes=8388608,software_particle_command_path_qualified=True,physical_GPU_pixels_qualified=False,full_game_qualified=False,renderer_buffer_context_is_declared_input=True,scene_environment_producers_qualified=False,compiled_source_count=283,fresh_native_compile_performed=False,native_DLL_preserved=True,added_build_files=0,overall_completion_percent=10,estimate_method='weighted milestones;not measured recovered-code coverage',goal_paused=False,next_task='Trace actual original renderer command-buffer/context setup andframe submission/consumption using callers of6065b0/605e0aandglobal890104;compose their ordinary producers before claiming display output. Keep full sprite-family/scene scheduler qualification obligations.',limitations=limits,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED original138-texture/particle RAM command path;23primary comparisons;zero newroutine credit',flush=True)
if __name__=='__main__':main()
