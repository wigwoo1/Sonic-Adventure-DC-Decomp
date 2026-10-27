"""Qualify five complete bodies; retain explicit partial and canonical domains."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','verification','irq_verification','retained_verification','startup_verification'):
  path=P/f'analysis/movie_countdown_{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
  for pinned,h in report['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['translation'];d=reports['verification'];irq=reports['irq_verification'];retained=reports['retained_verification'];startup=reports['startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_countdown.dll')
 assert len(t['routines'])==18 and t['instruction_rows']==592 and reports['connection']['compiled_source_count']==215
 assert (d['cases_passed'],d['independent_checks'])==(390,4583)
 assert (irq['lifecycles_passed'],irq['original_native_comparisons'],irq['independent_checks'],irq['authored_irq_admission_cases'])==(8,48,408,16)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(35,335)
 assert (startup['cases_passed'],startup['independent_checks'],startup['actual_wait_pc'],startup['frame_counter_unchanged'])==(1,79,'0x8c051798',2)
 assert all(r['dll_sha256']==dll for r in (d,irq,retained,startup)) and not startup['fresh_cold_profile']
 cov={r['address']:r for r in d['coverage_by_routine']}
 qualified={'0x8c643f98':(44,10),'0x8c602df8':(30,2),'0x8c602e34':(30,2),'0x8c05180c':(2,0),'0x8c652554':(49,0)}
 canonical={0x8c051810,0x8c04f7e0,0x8c10d428,0x8c10d444,0x8c647908,0x8c647bb8,0x8c66085a,0x8c647cb4,0x8c647cc4}
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())['routines'];previous=registry[:1472];assert len(previous)==1472 and len(registry) in (1472,1477)
 reused=[];partial=[];routines=[];binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();files.append(binary)
 for body in t['routines']:
  address=int(body['address'],16);matches=[r for r in previous if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)&0x1fffffff==address&0x1fffffff]
  if address in canonical:
   assert len(matches)==1;old=matches[0];start=int(old['address'],16);end=int(old['end_exclusive'],16);assert hashlib.sha256(raw[start-0x8c010000:end-0x8c010000]).hexdigest()==old['retail_span_sha256'];files.extend((P/old['source'],P/old['verification']));reused.append(dict(address=body['address'],canonical_name=old['name'],additional_credit=0,historical_end_exclusive=old['end_exclusive'],integration_end_exclusive=body['end_exclusive'],historical_span_checked_against_same_binary=True,coverage=cov[body['address']]))
  else:
   assert not matches
   if body['address'] in qualified:
    row=cov[body['address']];n,o=qualified[body['address']];assert (row['static_rows'],row['executed_rows'],row['executed_outcomes'])==(n,n,o) and not row['missing_rows'] and not row['missing_outcomes']
    routines.append(dict(module='1ST_READ.BIN',name=body['name'],address=body['address'],end_exclusive=body['end_exclusive'],source=t['source'],retail_span_sha256=body['retail_span_sha256'],instruction_rows=n,executed_rows=n,executed_outcomes=o))
   else:partial.append(cov[body['address']]|dict(registered=False))
 assert len(reused)==9 and len(partial)==4 and len(routines)==5
 assert sum(r['instruction_rows'] for r in routines)==155 and sum(r['executed_outcomes'] for r in routines)==14
 assert sum(r['coverage']['static_rows'] for r in reused)==134 and sum(r['static_rows'] for r in partial)==303
 assert (cov['0x8c051810']['executed_rows'],cov['0x8c051810']['executed_outcomes'])==(55,8) and d['shad051836_executed']
 prior=P/'analysis/movie_frame_wait_qualification.json';old_q=json.loads(prior.read_text());assert old_q['total_evidence_original_native_comparisons']==9124;files.append(prior)
 limitations=['Five newly qualified bodies:643f98 audio44rows/10outcomes,602df8 and602e34 request variants30rows/two outcomes each,05180c real RTS/NOP2rows,652554 FPUcallback wrapper49rows. All155rows/14outcomes execute. Finite coherent standalone fixtures,immutable code/literals,disjoint private frames;not exhaustive hardware/game ABI proof.',
 'Nine canonical adapters134rows receive zero extra credit;canonical051810 tick55rows/eight outcomes including actual SHAD execute. Signed16 packing93vectors,wrap/decrement/table bounds and real static callback table checked. Static dispatch table never rewritten;actual05180c or genuinely missing target supplied as callback ABI arguments.',
 'FPUwrapper32raw-pattern/bank/width vectors,two reachedFD-entry fault prefixes,three real missing callbacks. Raw FR/FPSCR and fullCPU/RAM/72byte stack independently checked. Additional reached-FPU host-4 guard preserves inherited SR/FD policy;does not claim physical fault-vector delivery. Primary SH4 instruction evidence pinned.',
 'Eight untimed softwareIRQ lifecycles initialize actual copied vectors,install ASIC handler,register original coherent hardware-slot callback and countdown event,admit two IRQs,run acknowledgement/FPUwrapper/event traversal/tick,returnRTE with interrupted CPU/FPU/hidden-bank/special state restored. Counter2->1->0;48original/native comparisons and16authored admission cases. Driver object derived from actual static slot literals;slots populated by original64135a,not hand-filled.',
 'Input602cb8,SDKpump65a378,mask648234,status660dac remain partial/unregistered. Inactive input/null interface/gameMode0 only completes configured IRQ fixtures. Active input reaches actual missing668e60. SDKpump executes real error0x0b000003 under metadata guard;audio caller ignores actual error,not mocked result. Larger652210/656940 and earlier053ac0/051790/queue/dispatch callers remain partial.',
 'No automatic/uninterrupted frame-wait IRQ pumping,physical IRQ timing/frequency,full boot/gameplay proof. Fresh configured warm startup4709boundaries stops-4 at051798 with counter2/reload2 unchanged;exact previous214 recorded boundary fields retained. Historical211cold not rerun. Existing G2/CH2 runtime and ABI unchanged.',
 '474fresh original/native comparisons5405independent checks:390direct4583,48IRQ408,35retained335,one warm79. Previous9124comparisons retained historical/not all rerun;9598combined. Authored IRQ admissions not counted as original/native comparisons.',
 'Whole game/ports/FPS8K/SUPIR/settings unfinished. TextureOff disables texture sampling/uses vertex colors. Goal ACTIVE about10% weighted milestones,not measured code coverage.']
 report=dict(newly_registered_entries=5,newly_qualified_rows=155,executed_new_rows=155,qualified_conditional_outcomes=14,compiled_source_count=215,routines=routines,canonical_reuse=reused,canonical_integration_rows=134,reused_additional_credit=0,partial_entries=partial,coverage_by_routine=d['coverage_by_routine'],new_direct_original_native_comparisons=390,new_direct_independent_checks=4583,fresh_irq_original_native_comparisons=48,fresh_irq_independent_checks=408,authored_irq_admission_cases=16,fresh_retained_original_native_comparisons=35,fresh_retained_independent_checks=335,fresh_warm_startup_comparisons=1,fresh_warm_startup_checks=79,fresh_original_native_comparisons=474,fresh_independent_checks=5405,retained_previous_evidence_original_native_comparisons=9124,total_evidence_original_native_comparisons=9598,prior_registry_entries=1472,result_registry_entries=1477,result_unique_module_address_pairs=1474,actual_wait_pc='0x8c051798',frame_counter_unchanged=2,warm_boundaries=4709,fresh_cold_profile=False,historical_cold_profile=retained['historical_cold_profile'],next_task='Recover actual active-input dependency668e60;preserve partial frame/handler domains and do not fake progress.',remaining_unqualified_graphics_root_outcomes=old_q['remaining_unqualified_graphics_root_outcomes'],partial_unregistered_dispatcher='0x8c652210',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_countdown_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED countdown5/155rows/14outcomes;474fresh/5405checks')
if __name__=='__main__':main()
