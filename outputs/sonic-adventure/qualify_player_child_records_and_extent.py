"""Qualify complete list primitives/cleanup and collision extent; retain frontiers."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_child_records_and_extent_qualification.json';assert not target.exists()
 paths=[P/'analysis'/n for n in ('player_collision_attach_verification.json','player_child_record_exhaustion_verification.json','player_collision_attach_compile_checks.json')]
 lifecycle,exhaustion,compiler=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
 for report in (lifecycle,exhaustion,compiler):
  for path,value in report['sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 for path,value in compiler['source_sha256'].items():assert sha(path)==value,path;files.append(Path(path))
 assert (lifecycle['original_native_comparisons'],lifecycle['checks'],lifecycle['independent_exact_oracles'],lifecycle['allocated_child_lifecycles'])==(186,1012,265,16)
 assert (exhaustion['original_native_comparisons'],exhaustion['checks'])==(29,116)
 assert compiler['source_count']==278 and compiler['compiler_exit_code']==0 and compiler['original_instruction_rows_byte_checked']==201
 assert all(x['supplied_allocator_results']==x['supplied_game_callee_outputs']==x['supplied_private_execution_frames']==0 for x in (lifecycle,exhaustion))
 wanted={'0x8c02f400','0x8c0b33a0','0x8c0b33ce','0x8c0b3afc'};routines=[]
 for prefix in ('player_collision_attach','player_child_update'):
  mp=P/f'analysis/{prefix}_translation.json';t=json.loads(mp.read_text());files.append(mp)
  for b in t['routines']:
   if b['address'] not in wanted:continue
   cov=next(x for x in lifecycle['coverage_by_routine'] if x['address']==b['address'])
   missing_rows=set(cov['missing_rows'])-set(exhaustion['executed_rows'])
   missing_edges={tuple(x) for x in cov['unresolved_outcomes']}-{tuple(x) for x in exhaustion['executed_conditional_outcomes']}
   assert not missing_rows and not missing_edges,(b['address'],missing_rows,missing_edges)
   row={k:b[k] for k in ('name','address','end_exclusive','retail_span_sha256','instruction_rows')}
   row.update(module='1ST_READ.BIN',source=t['source'],coverage=dict(executed_rows=b['instruction_rows'],static_rows=b['instruction_rows'],executed_outcomes=len(b['conditional_edges']),static_outcomes=len(b['conditional_edges']),missing_rows=[],unresolved_outcomes=[]));routines.append(row)
 assert len(routines)==4 and sum(x['instruction_rows'] for x in routines)==142 and sum(x['coverage']['executed_outcomes'] for x in routines)==22
 prior=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(prior)==1636
 assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address'] in wanted for x in prior)
 dll=R/'work/sonic-adventure/build/player_collision_attach.dll';linked=inventory(dll,prior+routines);assert linked['confirmed_linked_unique_routines_by_name']==684
 limits=[
  'Four complete originals are software-qualified in the tested domains: 88-row collision extent 02f400,23-row record allocation 0b33a0,16-row unlink/free 0b33ce,and15-row cleanup 0b3afc. All142rows and22conditional outcomes execute. This is not a claim about all possible inputs or complete game coverage.',
  '186full-state/ordered-boundary comparisons and1012checks include265independent exact oracles and16original allocated-child lifecycles. Original heap,pool,parent,constructor,collision allocation/attachment,stored child initializer andupdate are executed with declared scene inputs andhost phase order. Actor collision andprovider pointers are actual original allocator/attachment outputs. Eight collision kinds andtwo player slots execute across the retained FPU bank/rounding combinations.',
  '29additional comparisons/116checks exercise genuine heap exhaustion across four bank/rounding combinations. Null allocation preserves an existing actual list;released node storage is reusable. No allocator/callee result orprivate execution frame is supplied. Original list links,copy bytes,kind/count/flag/owner fields,default25andcomputed extent bits are checked independently.',
  'The complete106-row collision attachment 02f4b0remains unregistered:94rows/13outcomes execute,including genuine second-allocation failure. Its first-header-allocation failure enters12retained diagnostic-loop rows andthree unresolved outcomes. No new wait bound is introduced. Registered seven-row byte copy604500is reused with zero duplicate credit.',
  'The complete481-row effect update remains unregistered:223rows/9outcomes execute in this suite. Spawning,nonempty-particle update/draw,remaining geometry/renderer paths,retail scheduler/scene setup,physical hardware andcomplete game/ports/settings remain unfinished. Extent tests cover PR=0/SZ=0,two register banks andround-to-nearest/zero;unusual inputs/modes are not generally qualified.',
  'The byte-copy local BF/S backedge604506withdelay604508is independently byte-checked and classified through the existing internal-entry observer facility. It is not a new call anddoes not write PR. All actual call/tail-entry boundaries andexactfullstate checks remain. Existing CPU/FPU/bus/ABI/guest limits andprevious source/proof/build bytes are unchanged.',
  '278sources compile/link andpass strict unbounded Wall/Wextra/Werror;all201newly emitted original rows byte-match RevA. The pinned connection report inherited277inits count field;the additive compile report derives andcompiles the actual278distinct source arguments andrecords the metadata correction. Nohistorical report is rewritten.',
  'Whole-project estimate remains10%weighted milestones,not measured code coverage. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIRandsettings remain planned. TextureOff must disable sampling anduse vertex/material colours. Goal active;continue SolHigh.'
 ]
 result=dict(status='complete_original_collision_extent_and_child_record_primitives_software_qualified_in_declared_domains',routines=routines,new_original_routine_credit=4,newly_qualified_original_rows=142,executed_conditional_outcomes=22,unresolved_conditional_outcomes=0,fresh_original_native_comparisons=215,fresh_checks=1128,independent_exact_oracles=265,actual_allocated_child_lifecycles=16,compiled_source_count=278,new_emitted_original_rows=201,canonical_reused_entries=['0x8c604500'],canonical_reused_original_rows=7,canonical_duplicate_credit=0,result_registry_entries=1640,result_unique_module_address_pairs=1637,linked_routine_inventory=linked,dll_sha256=sha(dll),collision_attachment_qualified=False,complete_effect_update_qualified=False,full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Exercise original particle spawning andnonempty update/draw from the actual initialized child;retain original collision attachment failure-loop andretail scene/scheduler obligations.',limitations=limits,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('QUALIFIED4originals/142rows/22outcomes;215comparisons/1128checks;684confirmedlinks',flush=True)
if __name__=='__main__':main()
