"""Qualify original spawning/draw wrapper with explicit rendering frontiers."""
import ast,hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_child_particles_qualification.json';assert not target.exists()
 paths=[P/'analysis'/n for n in ('player_child_particles_verification.json','player_child_particles_compile_checks.json','player_child_particle_texture_frontier.json')]
 verification,compiler,frontier=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
 for report in (verification,compiler,frontier):
  for path,value in report['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 for path,value in compiler['source_sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 assert (verification['original_native_comparisons'],verification['checks'],verification['independent_exact_oracles'],verification['allocated_child_lifecycles'])==(192,1118,336,8)
 assert compiler['source_count']==278 and compiler['compiler_exit_code']==0 and compiler['original_instruction_rows_byte_checked']==347
 assert frontier['runtime_witnesses']==8 and frontier['exported_original_loader_rows']==229 and frontier['new_native_loader_implementations']==0
 assert verification['supplied_allocator_results']==verification['supplied_game_callee_outputs']==verification['supplied_private_execution_frames']==0
 mp=P/'analysis/player_child_update_translation.json';t=json.loads(mp.read_text());files.append(mp);wanted={'0x8c0b37f2','0x8c0b3f2c'};routines=[]
 for b in t['routines']:
  if b['address'] not in wanted:continue
  cov=next(x for x in verification['coverage_by_routine'] if x['address']==b['address'])
  assert not cov['missing_rows'] and not cov['unresolved_outcomes'] and cov['executed_rows']==b['instruction_rows'] and cov['executed_outcomes']==len(b['conditional_edges'])
  row={k:b[k] for k in ('name','address','end_exclusive','retail_span_sha256','instruction_rows')};row.update(module='1ST_READ.BIN',source=t['source'],coverage=cov);routines.append(row)
 assert len(routines)==2 and sum(x['instruction_rows'] for x in routines)==347 and sum(x['coverage']['executed_outcomes'] for x in routines)==14
 prior=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(prior)==1640 and not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address'] in wanted for x in prior)
 dll=R/'work/sonic-adventure/build/player_collision_attach.dll';linked=inventory(dll,prior+routines);assert linked['confirmed_linked_unique_routines_by_name']==686
 scripts=sorted(P.glob('*player_child_particle*.py'))
 for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
 files+=scripts
 limits=[
  'Complete286-row particle spawning0b37f2and61-row draw wrapper0b3f2care software-qualified in the documented domains:all347rows and14conditional outcomes execute. This doesnot assert all inputs orfully working rendering.',
  '192fresh original/native full-state andordered-boundary comparisons/1118checks include336independent exact oracles. Eight real heap/pool/parent/constructor/collision/child lifecycles covertwo player slots,two FPU banks andround-to-nearest/zero. Declared scene flag,height,shape position andscreen dimensions are inputs. Original allocation,spawning,list handling,view initialization andstored callbacks execute;no allocator/callee result orprivate frame is supplied.',
  'Absent scene flag,lower/upper height rejection,two actual particle creations,genuine heap exhaustion/release/reuse,nonempty update,cleanup,empty andsuppressed drawing match. Independent LCG steps/call counts,list links,scene-height copy,andactual caller packet position/dimension/word/header/flags are checked. The test prepares each initial matrix in the correct opposite FPU bank before original initialization.',
  'Normal draw qualification covers clipped andsuppressed particles. Eight visible witnesses run the actual original view builder from640x480dimensions andadvance through original sprite geometry,then both executions stop-3at606a9eon an uninitialized EFF_REGULARtexture descriptor. Fault execution is compared anddiscarded,never re-entered as a private-frame fixture. Visible textured rendering andphysical GPU pixels remain unqualified.',
  'The retail assetSONICADV/EFF_REGULAR.PRSis present andits124234compressed bytes match historical hash2c62f8a4035181cb5366a8ea5332a793ae304b0a9546c43faa186e35c6d04096. Historical PVMHmetadata records138textures,matching original list1c3068;the tested sprite uses index132. Resource batch0990deandloader099750/file-decode0997bcare identified;their229original rows are exported/byte-checked drafts,not native implementations andreceivezero credit. No texture descriptor is hand-filled andno retail asset is published.',
  'The481-row effect update remains unregistered:352rows/19outcomes execute in this suite. Geometry,other sprite/renderer paths,original scene-state producers,texture loading,retail startup/scheduler,physical hardware andcomplete game/ports/settings remain unfinished.',
  'The preserved278-source build is reused without relinking. Current278sources pass strict unbounded Wall/Wextra/Werror andretain their hashes;347particle rows arebyte-checked andnew Python scripts parse. No new CPU/FPU/bus/ABI/guest limit orwaitbound is introduced. Previoussource/proof/build bytes arepreserved except four established append-preserving status files.',
  'Whole-project estimate remains10%weighted milestones,not measured code coverage. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR anddisplay/texture settings remain planned. TextureOff disables sampling anduses vertex/material colours. Goal active;continueSolHigh.'
 ]
 report=dict(status='complete_original_particle_spawn_and_draw_wrapper_software_qualified_in_declared_domains',routines=routines,new_original_routine_credit=2,newly_qualified_original_rows=347,executed_conditional_outcomes=14,unresolved_conditional_outcomes=0,fresh_original_native_comparisons=192,fresh_checks=1118,independent_exact_oracles=336,actual_allocated_child_lifecycles=8,compiled_source_count=278,fresh_native_link_performed=False,existing_build_preserved=True,particle_rows_byte_checked=347,loader_draft_rows_byte_checked=229,loader_draft_routine_credit=0,result_registry_entries=1642,result_unique_module_address_pairs=1639,linked_routine_inventory=linked,dll_sha256=sha(dll),visible_texture_frontiers=verification['visible_texture_frontiers'],complete_effect_update_qualified=False,visible_textured_rendering_qualified=False,full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Recover/integrate original099750and0997bcresource loaders andtheir real607264/10d360/04c5e0/608134dependencies for localEFF_REGULAR.PRS;execute actual texture descriptor producer before resuming visible child drawing. Keep scene-state producer andremaining update/geometry obligations.',limitations=limits,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED2originals/347rows/14outcomes;192comparisons/1118checks;686confirmedlinks;visibletexturefrontierretained',flush=True)
if __name__=='__main__':main()
