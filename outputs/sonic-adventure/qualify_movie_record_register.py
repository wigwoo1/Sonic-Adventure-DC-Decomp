"""Qualify seven original registration/context bodies; retain canonical credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
OWN={'0x8c602b68','0x8c6477cc','0x8c010f22','0x8c602bf0','0x8c66082c','0x8c660842','0x8c110954'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 reports={};files=[Path(__file__)]
 for name in ('movie_record_register_translation','movie_record_register_connection','movie_record_register_verification','movie_record_register_returns_proof','movie_record_context_vectors_verification','movie_record_register_retained_verification','movie_record_register_startup_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_record_register_translation'];direct=reports['movie_record_register_verification'];proof=reports['movie_record_register_returns_proof'];vectors=reports['movie_record_context_vectors_verification'];retained=reports['movie_record_register_retained_verification'];startup=reports['movie_record_register_startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_record_register.dll')
 assert len(t['routines'])==21 and t['instruction_rows']==865 and reports['movie_record_register_connection']['compiled_source_count']==211
 assert direct['cases_passed']==167 and (vectors['cases_passed'],vectors['independent_checks'])==(32,320)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(1013,14027)
 assert (len(startup['cases']),startup['independent_checks'],startup['actual_next_frontier'])==(2,146,'0x8c601874')
 assert all(r['dll_sha256']==dll for r in (direct,vectors,retained,startup))
 missing_rows=set();missing_edges=set();executed=0;executed_edges=0
 for row in direct['coverage_by_routine']:
  if row['address'] in OWN:
   missing_rows.update(row['missing_rows']);missing_edges.update(map(tuple,row['missing_outcomes']));executed+=row['executed_rows'];executed_edges+=row['executed_outcomes']
 assert missing_rows==set(proof['infeasible_rows'])=={'0x8c647840','0x8c647842'}
 assert missing_edges==set(map(tuple,proof['infeasible_conditional_outcomes']))=={('0x8c64783e','0x8c647840'),('0x8c602bc0','0x8c602bce')}
 assert (executed,executed_edges)==(308,20)
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];previous=registry[:1462];canonical=[]
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();files.append(binary)
 for routine in t['routines']:
  address=routine['address'];matches=[r for r in previous if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)&0x1fffffff==int(address,16)&0x1fffffff]
  if address in OWN:assert not matches,address
  else:
   assert len(matches)==1,address
   old=matches[0];start=(int(old['address'],16)&0x1fffffff)-0x0c010000;size=int(old['end_exclusive'],16)-int(old['address'],16)
   assert start>=0 and size>0 and hashlib.sha256(raw[start:start+size]).hexdigest()==old['retail_span_sha256'],address
   entry=dict(address=address,canonical_name=old['name'],additional_recovery_credit=0,historical_end_exclusive=old['end_exclusive'],current_end_exclusive=routine['end_exclusive'],historical_span_sha256=old['retail_span_sha256'],current_span_sha256=routine['retail_span_sha256'],historical_span_bytes_verified_against_same_original_binary=True)
   if old['retail_span_sha256']!=routine['retail_span_sha256']:
    assert old['end_exclusive']!=routine['end_exclusive'],address
    entry['span_note']='The earlier registered span and the new Ghidra body envelope have different endpoints. The old byte interval/hash is independently checked unchanged against the same original binary; the new instruction ranges are separately byte-checked. Literal gaps can make instruction-byte counts differ from a contiguous envelope. This remains the same physical entry and receives zero additional credit; the historical report is not rewritten or relabeled.'
   canonical.append(entry);files.extend((P/old['source'],P/old['verification']))
 assert len(canonical)==14
 routines=[dict(module='1ST_READ.BIN',source=t['source'],**r) for r in t['routines'] if r['address'] in OWN]
 assert sum(r['instruction_rows'] for r in routines)==310 and sum(len(r['conditional_edges']) for r in routines)==22
 prior_path=P/'analysis/movie_slot_initialize_qualification.json';prior=json.loads(prior_path.read_text());assert prior['total_evidence_original_native_comparisons']==6822;files.append(prior_path)
 files.extend((P/'analysis/record_context_register_primary_evidence.json',P/'source/sa_input_context_save.c'))
 fresh=direct['cases_passed']+vectors['cases_passed']+retained['total_original_native_comparisons']+len(startup['cases']);checks=direct['independent_checks']+vectors['independent_checks']+retained['independent_checks']+startup['independent_checks'];assert fresh==1214
 limitations=[
 'Seven newly qualified original entries310 instruction rows:308 executed andtwo checked infeasible rows,20 executed/two proved infeasible conditional outcomes. Fourteen earlier canonical adapters, including startup wrapper010b00, receive zero duplicate credit. All coverage is reported per body; not all865 integration rows are claimed executed. The15-row callback preserves its shared indirect tail with a guarded actual target, not a synthetic callee result.',
 '167 direct cases exercise shifts, mask bits, actual queue registration, null/real callbacks, input modes, buffers, rejection/full/free slots, SR masks/banks and fault prefixes. Independent tagged-buffer/job/header/callback/SR/context expectations and complete original/native states/boundaries compare. Later callback writes correctly populate the initially cleared record region; no original behavior is changed to satisfy the oracle.',
 'Thirty-two extra context vectors use nonzero FPUL/MACH/MACL/GBR and sticky FPSCR flags across all eight PR/SZ/FR modes, including exact overlapping wide stores. Prior SZ0 context prototype source remains unchanged and had no registry entry; this is its first complete original entry qualification. STS FPU-control reads respect FD; authored host fault-4 occurs at the reached instruction and preserves earlier effects. Primary CPU manual corroborates semantics. Physical exception-vector delivery/timing remain outside this software proof.',
 '1013 retained cases14027 checks rerun previous slot/system/float/upload/IRQ/flash oracles in211-source DLL. Earlier6822 evidence cases are retained, not all rerun. No fresh432-case word-copy grid or old platform-only proof is claimed.',
 'Two fresh full configured0546c0 startups:4695 warm/585993 cold boundaries,146 checks, reach missing601874. Exact prior210 recorded boundary prefixes and complete original/native CPU/FPU/RAM/VRAM/Sound/platform/flash/runtime comparisons pass. Full8MiB clear/two2MiB Sound fills/two33376-byte transfers/G2IRQ/RTE/REQUEST/all1065282 Sound events zero drops/39 SDK records1092B/32 MIDI sends/32768B VRAM upload remain independently checked. Not physical boot/gameplay proof.',
 'Immutable code/literals, coherent mapped buffers and disjoint private frames apply. Callee return/reset proofs account for impossible failure rows/outcomes. No host helper return substitution or guest checkpoint. Broader652210 dispatcher/656940 root remain partial; full game/ports/FPS/8K/SUPIR/settings unfinished. Texture Off must disable sampling and use vertex colors. Goal ACTIVE about10% weighted milestones, not measured code coverage.'
 ]
 report=dict(routines=routines,newly_registered_entries=7,newly_qualified_rows=310,executed_rows=308,proved_infeasible_rows=2,qualified_conditional_outcomes=22,executed_conditional_outcomes=20,proved_infeasible_conditional_outcomes=2,compiled_source_count=211,integrated_entries=21,static_integration_rows=865,measured_executed_integration_rows=sum(r['executed_rows'] for r in direct['coverage_by_routine']),canonical_reuse=canonical,reused_additional_credit=0,coverage_by_routine=direct['coverage_by_routine'],new_direct_original_native_comparisons=167,new_direct_independent_checks=direct['independent_checks'],additional_context_vector_comparisons=32,additional_context_vector_checks=320,fresh_retained_original_native_comparisons=1013,fresh_retained_independent_checks=14027,fresh_full_startup_comparisons=2,fresh_startup_independent_checks=146,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=6822,total_evidence_original_native_comparisons=6822+fresh,actual_next_frontier='0x8c601874',partial_unregistered_dispatcher='0x8c652210',remaining_unqualified_graphics_root_outcomes=prior['remaining_unqualified_graphics_root_outcomes'],preexisting_unregistered_context_prototype='source/sa_input_context_save.c',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_record_register_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED7/310/22;',fresh,'freshcases',checks,'checks')
if __name__=='__main__':main()
