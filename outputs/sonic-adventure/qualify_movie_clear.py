"""Qualify the original full clear; keep decoder/probe and parent credit separate."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_clear_translation','movie_clear_connection','movie_clear_verification','movie_driver_qualification')]
 translation,connection,direct,prior=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,prior):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert connection['compiled_source_count']==165 and not connection['verification_interpreter_is_game_code']
 assert direct['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_clear.dll')
 assert direct['cases_passed']==19 and direct['status_counts']=={'0':11,'-3':4,'-4':3,'-2':1}
 assert len(direct['calibration'])==6 and [r['iterations'] for r in direct['calibration']]==[1,2,3,17,257,4096]
 assert all(r['instructions']==r['iterations']*16 and r['cpu_and_memory_match'] and r['ordered_cpu_and_write_hashes_match'] for r in direct['calibration'])
 assert len(direct['retail_loop_and_helper_rows_checked'])==16 and direct['retail_literal_words_checked']==3
 row=next(r for r in direct['coverage_by_routine'] if r['address']=='0x8c6656e2')
 assert row['static_rows']==234 and row['covered_rows']==233 and row['missing_rows']==['0x8c66597c']
 assert row['conditional_edges_covered']==31 and row['missing_edges']==[['0x8c665972','0x8c66597c']]
 routine=next(r for r in translation['routines'] if r['address']=='0x8c6656e2')
 assert 'bounded_research_waits' not in routine and routine['bounded_research_self_stalls']==[dict(pc='0x8c665968',max_visits=2,status=-4)]
 listing=Path(routine['listing']);rows={int(r['address'],16):r for r in csv.DictReader(listing.open(),delimiter='\t')}
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 expected={0x8c665960:'mov.l 0x8c6659e8,r3',0x8c665962:'mov.l @r3,r0',0x8c665964:'cmp/eq #0x6,r0',0x8c665966:'bt 0x8c66596c',0x8c665968:'bra 0x8c665968',0x8c66596a:'nop',0x8c66596c:'mov.l 0x8c6659e8,r2',0x8c66596e:'mov.l @r2,r0',0x8c665970:'cmp/eq #0x6,r0',0x8c665972:'bf/s 0x8c66597c',0x8c665974:'mov #0x7c,r4'}
 for a,text in expected.items():
  assert rows[a]['instruction'].lstrip('_')==text
  assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(rows[a]['bytes'])
 assert int.from_bytes(raw[0x8c6659e8-0x8c010000:0x8c6659ec-0x8c010000],'little')==0x8c8a2d58
 proof=dict(unreachable_row='0x8c66597c',unreachable_edge=['0x8c665972','0x8c66597c'],checked_instructions=len(expected),global_word='0x8c8a2d58',argument='Reaching 8c66596c requires the first load of this word to equal six. The intervening path has no store or call; the second load is from the same word and therefore also equals six. The following BF/S cannot branch under the declared single-threaded, no-asynchronous-mutation model. Its delay slot still executes.',assumptions=['Immutable original code/literals.','No asynchronous CPU or RAM mutation.','Software register/callback policies stated in the verification report.'])
 complete=[c for c in direct['cases'] if c['input']['entry']==0x8c6656e2 and c['status']==0];assert len(complete)==10
 failures=[c for c in direct['cases'] if c['input']['entry']==0x8c6656e2 and c['status']==-3];assert len(failures)==4
 assert [c['clear_native']['writes'] for c in failures]==[0,1,0x100000,0x1fffff]
 for c in direct['cases']:
  n,ref=c['clear_native'],c['clear_reference']
  assert n['calls']==ref['calls'] and n['writes']==ref['writes'] and n['attempts']==ref['attempts']
  assert not n['code_errors'] and not n['fpu_errors'] and c['full_state_match'] and c['ordered_boundaries_match'] and c['full_vram_bytes_compared']==0x800000
 for c in complete:
  assert c['clear_native']['calls']==c['clear_native']['writes']==0x200000 and c['clear_reference']['steps']==33554432 and c['clear_reference']['coverage']==0xffff
 mode=next(c for c in direct['cases'] if c['input']['entry']==0x8c65262c);assert mode['status']==0
 early=next(c for c in direct['cases'] if c['input']['entry']==0x8c010180);assert early['status']==-2 and early['stop_pc']=='0x8c000000' and early['clear_native']['writes']==0x200000
 files += [listing,binary,Path(__file__)]
 report=dict(routines=[dict(routine,module='1ST_READ.BIN',source=translation['source'],source_sha256=digest(P/translation['source']))],newly_registered_entries=1,qualified_rows=234,executed_rows=233,proven_unreachable_rows=1,executed_conditional_outcomes=31,proven_unreachable_conditional_outcomes=1,unreachable_proof=proof,
  direct_comparisons=19,complete_direct_clear_profiles=10,failure_profiles=4,hardware_self_loop_profiles=3,calibration_profiles=6,full_clear_bytes=0x800000,full_clear_helper_calls=0x200000,decoded_instructions_per_complete_clear=33554432,
  compiled_source_count=165,verification_interpreter_recovery_credit=0,reused_copy_helper_additional_credit=0,parent_additional_credit=0,mode_selector_observed_return=0,early_caller_next_dependency='0x8c000000',
  next_task='Trace the producer and runtime ownership of callback word 8c6734c4, which still resolves to 8c000000 in the actual early-caller profile; separately qualify remaining driver/reset/mode parents.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Original 8c6656e2 executes its full finite eight-MiB clear and qualifies under the documented software-platform domain. Native code performs every original helper call/write. Verification-only decoding executes every original loop/helper instruction and earns no game-code recovery credit.',
  limitations=['Native helper boundaries check exact register-sequence, address/value/width and FPU/FPUL invariants on every iteration. Counts, endpoints and streaming fingerprints also match the independently decoded execution. Fingerprints alone are not collision-free proofs.',
   'The constant clear event run is stored compactly in the reference trace. All native writes and all reference instructions execute; this is trace compression, not a bulk-fill replacement. Full final 16-MiB RAM, eight-MiB VRAM, machine/FPU/special/banked state and prefix/suffix ordered boundaries are compared.',
   'Nonzero patterned initial VRAM exposes missed or partial writes. Verification-only bus faults at zero, one, half and last-word completion retain exact faulting CPU/memory state. These hooks cannot supply game-service results.',
   'Only the finite clear bound is removed in the new source/DLL. The hardware-failure infinite self-loop still stops on its second visit under SA_BOUND_RETAIL_STALLS; normal source preserves that infinite loop. Hardware storage/ID/status/timer observations remain explicit host inputs.',
   'One row/outcome is proved unreachable under immutable literals and no asynchronous RAM mutation, rather than falsely counted as executed. No physical hardware timing, DMA, interrupt delivery, pixels or natural full boot is established.',
   'The mode-selector profile now returns and the actual early caller clears all VRAM before reaching unresolved callback 8c000000. Parent recovery is not inferred from these profiles. All prior source/proof/canonical fields and build bytes remain preserved.',
   'The complete game, playable ports, FPS/resolution settings, SUPIR textures and other targets remain unfinished; whole-project weighted estimate remains ten percent.'])
 (P/'analysis/movie_clear_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified full original clear: 233 executed rows / one proved unreachable; 2,097,152 actual calls and eight MiB per complete profile')
if __name__=='__main__':main()
