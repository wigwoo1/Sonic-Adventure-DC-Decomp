"""Qualify fully evidenced own bodies; canonical adapters get zero extra credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
OWN={'0x8c109cfe','0x8c108808','0x8c1087d2','0x8c602ab4','0x8c602998'}
REUSED={'0x8c107ec0','0x8c646c8a'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 reports={};files=[Path(__file__)]
 for name in ('movie_slot_initialize_translation','movie_slot_initialize_connection','movie_slot_initialize_verification','movie_slot_initialize_bounds_proof','movie_slot_initialize_retained_verification','movie_slot_initialize_startup_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_slot_initialize_translation'];direct=reports['movie_slot_initialize_verification'];bounds=reports['movie_slot_initialize_bounds_proof'];retained=reports['movie_slot_initialize_retained_verification'];startup=reports['movie_slot_initialize_startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_slot_initialize.dll')
 assert len(t['routines'])==7 and t['instruction_rows']==266 and reports['movie_slot_initialize_connection']['compiled_source_count']==210
 assert (direct['cases_passed'],direct['independent_checks'])==(603,7179)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(410,6848)
 assert (len(startup['cases']),startup['independent_checks'],startup['actual_next_frontier'])==(2,146,'0x8c010b00')
 assert all(r['dll_sha256']==dll for r in (direct,retained,startup))
 proven=set(map(tuple,bounds['infeasible_conditional_outcomes']));missing=set()
 for row in direct['coverage_by_routine']:
  assert not row['missing_rows'];missing.update(map(tuple,row['missing_outcomes']))
 assert missing==proven=={('0x8c109d24','0x8c109d54'),('0x8c10881a','0x8c10882c')}
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in sorted(REUSED):
  old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);adapter=next(r for r in t['routines'] if r['address']==address);assert old['retail_span_sha256']==adapter['retail_span_sha256'];files.extend((P/old['source'],P/old['verification']));canonical.append(dict(address=address,canonical_name=old['name'],additional_recovery_credit=0))
 previous=registry[:1457]
 assert all(not any(r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)&0x1fffffff==int(address,16)&0x1fffffff for r in previous) for address in OWN)
 routines=[dict(module='1ST_READ.BIN',source=t['source'],**r) for r in t['routines'] if r['address'] in OWN]
 assert sum(r['instruction_rows'] for r in routines)==199 and sum(len(r['conditional_edges']) for r in routines)==30
 prior_path=P/'analysis/movie_system_state_qualification.json';prior=json.loads(prior_path.read_text());files.append(prior_path);assert prior['total_evidence_original_native_comparisons']==5807
 limitations=[
 'Five newly qualified original bodies: fixed four-record initializer, batch/single-record refresh, metadata predicate and two-input status table getter. All199 own instructions execute. Twenty-eight own conditional outcomes execute; two initial loop exits are proved infeasible by checked immutable literal/arithmetic evidence. Both canonical clear and metadata decoder adapters receive zero additional recovery credit. All266 integration rows execute.',
 '603 direct comparisons/7179 independent checks verify full RAM effects outside only the declared private stack; the full original/native comparison includes all stack bytes and complete CPU/FPU/platform state. Actual original driver getters, metadata bit locator and decoder execute. Metadata short-circuit success/failures, special indices, table inputs, wraparound no-write clear, callee preservation and three FPU modes are covered. Mapped disjoint records and private stack; original literal words are immutable. Self-modification, destructive aliases and arbitrary invalid guest records are outside qualification.',
 '410 retained cases/6848 checks rerun unchanged system-query, float, upload, interrupt and60 flash-settings oracles in the new210-source DLL. The432-case old word-copy grid is not freshly rerun. Prior5807 evidence cases are retained, not all rerun.',
 'Two fresh uninterrupted configured0546c0 startups reach missing010b00 with4648 warm/585946 cold ordered boundaries. Exact prior209 boundary-field prefixes are preserved. All original/native CPU/FPU/RAM/VRAM/Sound/platform/flash/runtime states compare. Full8MiB clear, two2MiB Sound fills, two33376-byte transfers/G2IRQ/RTE/REQUEST,1065282 Sound events zero drops,39 SDK records1092B,32 MIDI sends and32768B VRAM upload remain verified. Explicit configured model, not physical timing/reset boot/gameplay evidence.',
 'Broader652210 dispatcher and656940 root remain partial. Full decompilation, playable native ports/FPS/8K/SUPIR/settings remain unfinished. Texture Off must disable sampling and use vertex colors. Goal ACTIVE, about10% weighted milestones, not measured code coverage.'
 ]
 report=dict(routines=routines,newly_registered_entries=5,newly_qualified_rows=199,executed_rows=199,qualified_conditional_outcomes=30,executed_conditional_outcomes=28,proved_infeasible_conditional_outcomes=2,compiled_source_count=210,integrated_entries=7,executed_integration_rows=266,canonical_reuse=canonical,reused_additional_credit=0,coverage_by_routine=direct['coverage_by_routine'],new_direct_original_native_comparisons=603,new_direct_independent_checks=7179,fresh_retained_original_native_comparisons=410,fresh_retained_independent_checks=6848,fresh_full_startup_comparisons=2,fresh_startup_independent_checks=146,fresh_original_native_comparisons=1015,fresh_independent_checks=14173,retained_previous_evidence_original_native_comparisons=5807,total_evidence_original_native_comparisons=6822,actual_next_frontier='0x8c010b00',partial_unregistered_dispatcher='0x8c652210',remaining_unqualified_graphics_root_outcomes=prior['remaining_unqualified_graphics_root_outcomes'],dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_slot_initialize_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED5/199/30;1015freshcases14173checks')
if __name__=='__main__':main()
