"""Qualify the actual lock/packet backend; keep response/pump/parser work open."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 names=('translation','connection','verification','api_verification','tasks_verification','retained_verification','audit');reports={};files=[Path(__file__)]
 for name in names:
  path=P/f'analysis/movie_channel_request_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 t=reports['translation'];c=reports['connection'];audit=reports['audit'];dll=digest(ROOT/'work/sonic-adventure/build/movie_channel_request.dll')
 assert t['instruction_rows']==125 and c['compiled_source_count']==222 and audit['guard_call_return_sites']==1928
 assert reports['verification']['original_native_comparisons']==576 and reports['api_verification']['original_native_comparisons']==88
 assert reports['tasks_verification']['original_native_comparisons']==240 and reports['tasks_verification']['admission_bases']==24
 assert reports['retained_verification']['original_native_comparisons']==867
 for name in ('verification','api_verification','tasks_verification'):assert reports[name]['dll_sha256']==dll
 comparison=sum(reports[n]['original_native_comparisons'] for n in ('verification','api_verification','tasks_verification','retained_verification'));checks=sum(reports[n]['independent_checks'] for n in ('verification','api_verification','tasks_verification','retained_verification'))
 assert (comparison,checks)==(1771,23451)
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1489,1493)
 prior_registry=registry[:1489];qualified=[]
 for body in t['routines']:
  if body['address']=='0x8c65f348':
   old=next(r for r in prior_registry if r['address']==body['address'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN');assert old['retail_span_sha256']==body['retail_span_sha256'];continue
  assert not any(r['address']==body['address'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in prior_registry)
  coverage=next(r for r in reports['verification']['coverage_by_routine'] if r['address']==body['address']);assert not coverage['missing_rows'] and not coverage['missing_outcomes']
  row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],executed_rows=coverage['executed_rows'],executed_outcomes=coverage['executed_outcomes']);qualified.append(row)
 assert len(qualified)==4 and sum(r['instruction_rows'] for r in qualified)==89 and sum(r['executed_outcomes'] for r in qualified)==4
 previous=P/'analysis/movie_channel_io_qualification.json';old=json.loads(previous.read_text());assert old['total_evidence_original_native_comparisons']==12855;files.append(previous)
 strict=P/'analysis/movie_channel_request_unbounded.json';receipt=json.loads(strict.read_text());assert receipt['source_count']==222 and receipt['exit_code']==0
 for path,h in receipt['source_sha256'].items():assert digest(path)==h
 files.append(strict)
 limitations=[
  'Four original request bodies660494/6604e6/6604c0/660518 qualify89rows/fourconditionaloutcomes within the declared coherent-RAM, single guest executor domain. The actual original packet builder65f810 and canonical diagnostics65f348/10d400/10dbc4 execute; no command serial, completion, diagnostic output, or callback result is substituted. Canonical report36rows gets zero extra recovery credit.',
  'TAS byte semantics and mandatory purge are explicit. All256 old byte values perentry, byte-neighbor preservation, SR.T, packet words/endianness/mapping/bank/padding/busy/serial rollover and actual cache-purge sequence have independent expectations. The flat original CPU model gains only mandatory purge events at the two new TAS PCs. Guest TAS runs without an instruction interrupt; concurrent host access to RAM, physical cache/TLB exceptions and controller hardware remain unqualified.',
  '24 actual-constructor/task-admission lifecycles: channels0/7,FR0/FR1,commands1/18 and request-completion/device-busy/mutex-busy wait prefixes. Each is observed across three scheduler returns then original owner cancellation. New observed nonlocal resumes66091a/660906 preserve nested guest stack/PR/owner deadlines; successful queueing happens once while completion remains pending. Completion serial and response bytes are never installed to make the parser finish. Input/global/descriptor/packet resources are explicit coherent fixtures; natural resource acquisition/full startup is not proved.',
  'Enabled automatic pump reaches actual missing646654 after real packet creation, unlock and saved return; disabled pump APIs complete. The two outer APIs remain partial. Active diagnostic uses real37-byte retail message through original formatting plus null/NOP/unknown callback. The report unaligned-stack branch is not newly qualified. Arbitrary callback/nonlocal graphs, physical response processing, successful header/metadata parse, codecs and playable game remain unfinished.',
  '1771 fresh original/native comparisons23451 independent checks. 12855 prior comparisons retained historical, not all rerun;14626 combined evidence comparisons, not unique test cases or code coverage. 222-source strict DLL and unbounded syntax checks pass. Prior source/proof/build hashes remain preserved; old runtime/SRFD/externalABI/decoder-worker continuation code unchanged. No fresh222 warm/cold startup; earlier warm/cold evidence remains historical.',
  'Only eight of the82 existing after-call continuation routes now have observed nonlocal qualification across the prior and new task graphs; other74/arbitrarygraphs remain unqualified. Return membership1928sites does not prove all destinations. Original owner+20c zero/decrement wrap semantics stay undetermined and unchanged. Next recover646654 pump and its actual response-processing callees, preserving real readiness/diagnostic/error effects. Full PC/Android/3DS/N64/PS1/FPS8K/SUPIR/settings remain unfinished;goal activeabout10%weightedmilestones,notmeasuredcodecoverage.'
 ]
 q=dict(status='partially_verified_request_integration',newly_registered_entries=4,newly_qualified_rows=89,qualified_conditional_outcomes=4,compiled_source_count=222,routines=qualified,new_integration_rows=125,canonical_diagnostic_rows=36,canonical_duplicate_credit=0,coverage_by_routine=reports['verification']['coverage_by_routine'],fresh_original_native_comparisons=comparison,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=12855,total_evidence_original_native_comparisons=12855+comparison,prior_registry_entries=1489,result_registry_entries=1493,result_unique_module_address_pairs=1490,flat_return_guard_sites=1928,new_qualified_nonlocal_resume_routes=['0x8c660906','0x8c66091a'],qualified_existing_resume_routes_total=8,remaining_unqualified_resume_routes=74,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_original_dependencies=['0x8c646654','0x8c65f1aa'],next_task='Recover original optional input pump646654 and reached response-processing dependencies without fabricating completion serials or parsed payloads.',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 q['next_canonical_dependencies']=[next(r for r in prior_registry if r['address']==address and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN') for address in ('0x8c646654','0x8c65f1aa')]
 q['next_task']='Connect the already registered646654/65f1aa canonical bodies and relevant input_service_cleanup_native.c callees to the current guarded ABI. Preserve their existing proofs with zero duplicate credit; replace reached isolated-test stub boundaries with actual code and qualify real response/completion paths.'
 q['limitations'][-1]=q['limitations'][-1].replace('Next recover646654 pump and its actual response-processing callees','Next integrate the already registered646654/65f1aa bodies and recover/connect their actual response-processing callees')
 (P/'analysis/movie_channel_request_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED4requests89rows/4outcomes;',comparison,'fresh/',checks,'checks;goalactive10%')
if __name__=='__main__':main()
