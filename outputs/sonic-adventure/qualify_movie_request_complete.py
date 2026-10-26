"""Close the original request using new branch proof and unchanged retained evidence."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_request_complete_verification','movie_transfer_runtime_qualification','movie_transfer_runtime_verification','movie_transfer_runtime_regression_verification','movie_transfer_request_closure_verification','movie_bulk_family_translation','movie_transfer_wait_translation','movie_transfer_runtime_connection'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 new=reports['movie_request_complete_verification'];prior=reports['movie_transfer_runtime_qualification']
 assert new['cases_passed']==12 and new['independent_checks']==270 and prior['total_comparisons']==699
 assert reports['movie_transfer_runtime_connection']['compiled_source_count']==193
 dll=digest(ROOT/'work/sonic-adventure/build/movie_transfer_runtime.dll');assert dll==new['dll_sha256']==prior['dll_sha256']
 definition=next(r for r in reports['movie_bulk_family_translation']['routines'] if r['address']=='0x8c65a5dc');body=set(definition['reachable_addresses']);wanted=set(map(tuple,definition['conditional_edges']))
 evidence=[reports[n] for n in ('movie_request_complete_verification','movie_transfer_runtime_verification','movie_transfer_runtime_regression_verification','movie_transfer_request_closure_verification')]
 seen=set().union(*(set(r['request_executed_rows']) for r in evidence));edges=set().union(*(set(map(tuple,r['request_executed_branch_outcomes'])) for r in evidence))
 assert len(body)==len(body&seen)==139 and len(wanted)==len(wanted&edges)==26
 newly=set(map(tuple,new['request_executed_branch_outcomes']));missing=set(map(tuple,prior['partial_request']['unverified_outcomes']));assert len(missing)==4 and missing<=newly
 source=P/'source/sa_movie_transfer_wait.c';routine=definition|dict(module='1ST_READ.BIN',name='sa_movie_transfer_wait_8c65a5dc',source=source.relative_to(P).as_posix(),source_sha256=digest(source))
 image=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=image.read_bytes();start=int(routine['address'],16)-0x8c010000;end=int(routine['end_exclusive'],16)-0x8c010000
 assert hashlib.sha256(raw[start:end]).hexdigest()==routine['retail_span_sha256'];files.extend((source,image))
 limitations=[
 'Complete139-row original request and all26conditional outcomes qualify in the initialized software CPU/bus domain. Four newly exercised outcomes use actual queue-full rejection, existing current request, real queued child-state2 SDK start error and actual non-FFFFFFFF asynchronous callback. No helper result, completion flag, guest checkpoint or original instruction is replaced.',
 'Twelve fresh comparisons across three FPU modes add270independent checks. The unchanged193-source DLL and immutable source/proof hashes retain699previous original/native comparisons and59authored-platform cases; those older suites were not rerun for this proof-only extension. Combined evidence covers711original/native comparisons. Published historical partial reports remain intact and are superseded only for this request own-body qualification.',
 'Previously verified32continuous requests complete through real data and the original vector/context/ASIC/generic/handler/SDK/RTE chain at the native untimed yield boundary. Explicit initial vector/registration setup and ordinary ABI state carry remain declared. Authored runtime and existing callees receive zero duplicate recovered-game credit.',
 'Asynchronous return is not completed DMA: without a running platform the descriptor remains active, completion0 and SoundRAM untouched. The native platform still schedules at this particular blocking-wait yield point. Broader asynchronous scheduling, physical timing/security/suspend/external triggers, uninterrupted earliest startup, ARM execution, audio and gameplay remain unfinished.',
 'The research binary bounds unavailable completion at the genuine65a6ecwait after two continued tests; production source preserves its original unbounded wait. The separate bootstrapac000010dependency remains unresolved. Prior early-caller/full-copy/video-clear evidence is retained historically, not rerun. All requested game/port/display/SUPIR/TextureOff vertex-color goals remain unfinished; overall10% is weighted milestones, not measured code coverage.'
 ]
 report=dict(routines=[routine],newly_registered_entries=1,newly_qualified_rows=139,qualified_conditional_outcomes=26,executed_rows=139,executed_conditional_outcomes=26,missing_rows=[],unverified_outcomes=[],newly_verified_outcomes=[list(x) for x in sorted(missing)],compiled_source_count=193,fresh_original_native_comparisons=12,fresh_independent_checks=270,retained_original_native_comparisons=699,combined_original_native_comparisons=711,retained_authored_platform_cases=59,retained_continuous_requests_returned=32,production_code_changed=False,reused_additional_credit=0,retained_default_frontier='0x8c65a6ec',partial_bootstrap_frontier='0xac000010',dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Full original request own-body qualification through four real queue/callback states; retained proofs and native binary are byte-identical. One existing translated body becomes complete, with no duplicate callee/platform credit.',limitations=limitations)
 (P/'analysis/movie_request_complete_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified original REQUEST139rows/26outcomes;12new+699retained comparisons;native code unchanged')
if __name__=='__main__':main()
