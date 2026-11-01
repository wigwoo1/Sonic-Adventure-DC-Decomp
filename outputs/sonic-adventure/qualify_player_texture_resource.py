"""Accept only three complete initializer bodies; retain the texture-link frontier."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_resource_qualification.json';assert not target.exists()
 paths=[P/f'analysis/player_texture_resource_{suffix}.json' for suffix in ('verification','compile_checks')];v,c=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
 for report in (v,c):
  for path,value in report['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 for path,value in c['source_sha256'].items():assert sha(path)==value,path
 assert (v['original_native_comparisons'],v['checks'],v['independent_exact_oracles'])==(66,357,155)
 assert c['source_count']==282 and c['compiler_exit_code']==0 and c['original_instruction_rows_byte_checked']==2813
 assert v['next_dependency']=='0x8c64c374' and v['frontier_status']==-2 and v['partial_frontier_execution_discarded']
 assert v['supplied_allocator_results']==v['supplied_game_callee_outputs']==v['supplied_private_execution_frames']==0
 wanted={'0x8c607264','0x8c64e5e0','0x8c660e58'};routines=[]
 for prefix in ('player_texture_load','player_texture_table_initialize','player_texture_heap_initialize'):
  mp=P/f'analysis/{prefix}_translation.json';t=json.loads(mp.read_text());files.append(mp)
  for b in t['routines']:
   if b['address'] not in wanted:continue
   cov=next(x for x in v['coverage_by_routine'] if x['address']==b['address']);assert not cov['missing_rows'] and not cov['unresolved_outcomes']
   assert cov['executed_rows']==b['instruction_rows'] and cov['executed_outcomes']==len(b['conditional_edges'])
   row={k:b[k] for k in ('name','address','end_exclusive','retail_span_sha256','instruction_rows')};row.update(module='1ST_READ.BIN',source=t['source'],coverage=cov);routines.append(row)
 assert len(routines)==3 and sum(x['instruction_rows'] for x in routines)==64 and sum(x['coverage']['executed_outcomes'] for x in routines)==6
 prior=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(prior)==1642 and not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address'] in wanted for x in prior)
 dll=R/'work/sonic-adventure/build/player_texture_driver_defaults.dll';linked=inventory(dll,prior+routines);assert linked['confirmed_linked_unique_routines_by_name']==689
 limits=[
  'Three complete public initializer bodies are software-qualified in documented input domains:607264 descriptor-name array,64e5e0 texture table,660e58 SDK heap. All64original rows and6conditional outcomes execute across both FPU banks and nearest/zero rounding. Guarded independent bytes/fields and full retail/native state plus ordered boundaries match.',
  '66fresh comparisons/357checks include155independent exact oracles. Original calloc,filesystem wrapper,real-disc mount/directory/open/size/close,format,read,decompression and decoder execute from ordinary entries. No allocator/callee output or private execution frame is supplied. Retail input remains local.',
  'EFF_REGULAR.PRS124234compressed bytes and358304decoded bytes match independent preserved SHA256 values. Resource load returns normally after61sector reads and stages138numeric texture IDs. These IDs are not linked descriptors. Original608c82/64c34c reaches missing64c374 with matched status-2;partial fault state is discarded. Visible textured rendering remains unqualified.',
  'Caller stack8cfff000 and original calloc workspaces avoid loader fixed CD0/CEE scratch aliases. Existing host instruction_budget is25000000 for this large asset;earlier wrapper default was2000000. CPU code,native wait guards,numerical/bus ABI and original instruction ordering are unchanged.',
  'Four additive279/280/281/282-source research builds retain historical stages. All282sources pass strict Wall/Wextra/Werror;50original bodies/2813rows are byte-checked,including7canonical reuses/127rows with zero duplicate credit. Only3initializers receive new credit;other loading/decoding/default callbacks remain unregistered proposals.',
  'Real-disc reads use the existing software disc adapter. Physical Dreamcast/GPU,full startup/scheduler,remaining effect update,all texture formats,linked descriptors,visible pixels and complete game/ports/settings remain unqualified. Builds are incomplete research artifacts,not a playable port.',
  'Whole-project estimate remains approximately10%weighted milestones,not measured recovered-code coverage. Goal stays active. Next reverse-engineering task is64c374 with SolHigh. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR and settings remain planned;TextureOff disables texture sampling and uses vertex/material colours.'
 ]
 report=dict(status='three_complete_texture_initializers_software_qualified_real_resource_decode_verified_linker_unrecovered',routines=routines,new_original_routine_credit=3,newly_qualified_original_rows=64,executed_conditional_outcomes=6,unresolved_conditional_outcomes=0,fresh_original_native_comparisons=66,fresh_checks=357,independent_exact_oracles=155,compiled_source_count=282,fresh_native_link_performed=True,additive_native_builds=4,added_build_files=8,original_bodies_byte_checked=50,original_rows_byte_checked=2813,canonical_reused_rows=127,canonical_duplicate_credit=0,result_registry_entries=1645,result_unique_module_address_pairs=1642,linked_routine_inventory=linked,dll_sha256=sha(dll),compressed_bytes=v['compressed_bytes'],compressed_sha256=v['compressed_sha256'],decoded_bytes=v['decoded_bytes'],decoded_sha256=v['decoded_sha256'],real_resource_reads=61,numeric_ids_staged=138,next_dependency='0x8c64c374',visible_textured_rendering_qualified=False,full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured recovered-code coverage',goal_paused=False,limitations=limits,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED3initializers/64rows/6outcomes;66comparisons/357checks;689confirmedlinks;next64c374',flush=True)
if __name__=='__main__':main()
