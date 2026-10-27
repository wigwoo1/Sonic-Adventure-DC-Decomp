"""Qualify actual response bodies/APIs; preserve the real submission frontier."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 names=('leaves','responses','controls','notifications','subdevices','subdevice_edges','api','swaps','publication','retained');reports={};files=[Path(__file__)]
 expected={'leaves':(192,2304),'responses':(832,13536),'controls':(400,4676),'notifications':(448,6240),'subdevices':(192,3360),'subdevice_edges':(24,240),'api':(160,2968),'swaps':(12,192),'publication':(18,276),'retained':(323,2607)}
 for name in names:
  path=P/f'analysis/movie_input_pump_{name}_verification.json';r=json.loads(path.read_text());reports[name]=r;files.append(path);assert (r['original_native_comparisons'],r['independent_checks'])==expected[name],name
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 for name in ('translation','connection','audit'):
  path=P/f'analysis/movie_input_pump_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 t=reports['translation'];c=reports['connection'];audit=reports['audit'];dll=digest(ROOT/'work/sonic-adventure/build/movie_input_pump.dll')
 assert (t['instruction_rows'],t['canonical_rows'],t['proposed_rows'],len(t['canonical_reuse']),c['compiled_source_count'])==(1035,370,665,6,223)
 assert all(r['dll_sha256']==dll for n,r in reports.items() if n in names and n!='retained')
 dead=reports['subdevice_edges']['structurally_unreachable_edges'];assert len(dead)==1 and dead[0]['edge']==['0x8c660114','0x8c66012a']
 parent_path=P/'analysis/movie_channel_io_translation.json';parent=json.loads(parent_path.read_text());files.append(parent_path)
 all_bodies=list(t['routines'])+[r for r in parent['routines'] if r['address'] in ('0x8c646982','0x8c6469c8')];coverage=[]
 samples=[r for r in reports.values() if 'coverage_by_routine' in r]
 for body in all_bodies:
  available=[next(x for x in r['coverage_by_routine'] if x['address']==body['address']) for r in samples if any(x['address']==body['address'] for x in r['coverage_by_routine'])];assert available
  missing=set.intersection(*(set(x['missing_rows']) for x in available));missing_edges=set.intersection(*(set(map(tuple,x['missing_outcomes'])) for x in available))
  unreachable={tuple(x['edge']) for x in dead if x['source_routine']==body['address']};assert missing_edges==unreachable and not missing,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges'])-len(unreachable),structurally_unreachable_outcomes=sorted(unreachable),missing_executable_rows=[],missing_executable_outcomes=[]))
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1493,1502);previous=registry[:1493];qualified=[];canonical=[]
 for body in all_bodies:
  old=next((r for r in previous if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==body['address']),None)
  if old:canonical.append(old);continue
  cv=next(r for r in coverage if r['address']==body['address']);row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
  source=parent['source'] if body['address'] in ('0x8c646982','0x8c6469c8') else t['source']
  row.update(module='1ST_READ.BIN',source=source,instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes'],structurally_unreachable_outcomes=cv['structurally_unreachable_outcomes']);qualified.append(row);files.append(P/source)
 assert len(canonical)==6 and len(qualified)==9 and sum(r['instruction_rows'] for r in qualified)==706 and sum(r['executed_outcomes'] for r in qualified)==95
 strict_path=P/'analysis/movie_input_pump_unbounded.json';strict=json.loads(strict_path.read_text());assert strict['source_count']==223 and strict['exit_code']==0
 for pin,h in strict['source_sha256'].items():assert digest(pin)==h
 files.append(strict_path);correction_path=P/'analysis/movie_input_pump_metadata_correction.json';correction=json.loads(correction_path.read_text());assert correction['native_source_bridge_probe_build_returns_byte_identical'] and correction['all_behavioral_case_records_unchanged'];files.append(correction_path)
 for path,row in correction['preserved_before_files'].items():assert digest(P/row['preserved_copy'])==row['before_sha256']==row['copy_sha256'];files.append(P/row['preserved_copy'])
 catalog_path=P/'analysis/movie_input_pump_catalog_baseline_correction.json';catalog=json.loads(catalog_path.read_text());assert catalog['native_source_bridge_probe_build_returns_byte_identical'] and catalog['all_behavioral_case_records_unchanged'];files.append(catalog_path)
 for path,row in catalog['preserved_before_files'].items():assert digest(P/row['preserved_copy'])==row['before_sha256']==row['copy_sha256'];files.append(P/row['preserved_copy'])
 previous_q=P/'analysis/movie_channel_request_qualification.json';previous_data=json.loads(previous_q.read_text());assert previous_data['total_evidence_original_native_comparisons']==14626;files.append(previous_q)
 comparisons=sum(n for n,ch in expected.values());checks=sum(ch for n,ch in expected.values());assert (comparisons,checks)==(2601,36399)
 next_addresses=('0x8c65efda','0x8c65eeda');next_canonical=[r for r in previous if r['address'] in next_addresses and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN']
 for row in next_canonical:files.extend((P/row['source'],P/row['verification']))
 limitations=[
  '223-source integration:13new guarded adapters1035originalrows=370rows fromsixregisteredcanonicalhelpers plus665rows insevenpreviouslyunregisteredresponsedrafts. Existing6461ae/65f50c/6602d2/6607a8 routesretained. Nine newlyqualifiedentries706rows/95executableoutcomes includethesevenresponses plusold646982/6469c8 outerAPIs41rows. Canonicalhelpers zero extra credit.',
  'All1076integration/outerAPIinstructionrows execute. Of148staticconditionaloutcomes,147execute;one660114->66012aedge isproveninfeasible bybytecheckedCFGdominance/no-call/disjoint-store proof. Sourcebranch remainspresent. Proofdomain isserialguestexecutionwithcaller-ownedrecordunmodifiedconcurrently. Thisisnotphysicalcache/TLB/host-threadatomicity/controllerhardware qualification.',
  'Actualresponsewalker/portmask/presence/header/primary/sub/metadata/byte-swap/statuspublication/rearm codeexecutes. IndependententireRAM/fields/stack/state/packet/projection oracles plusoriginalinstruction-model/fullmachine/device/boundary comparisons. RawRXbuffers,pastinflighttokens,presence/context,display-samples andcallbackpointersaredeclaredinputs. Completedtoken/publication/metadata/callbackresultsarenotstubbed. All48callbackslotsmatch/miss andrealNOP/unknowncallbacks;actualgame-modepredicateexercisesfalse callbackpath. NOPpreservesR0,notanassumedfalse result.',
  'Automaticpump APIs nowexecuteactualpump andpreservequeuedserial includingbusy/negativepreflightpaths. Queueingaloneleavesinflight/completedtokenszero. The real65efda submitbody,currentlyunconnected,containsoriginalcopyrecord+18->+1c andcommand+6->+7 beforehardwarehandoff. Itmustbeintegratedwithactualcalleesbeforeclaimingqueuedtask->naturalresponsecompletion orsuccessfulactiveheaderparser/codecbehavior. Testsneverfillcompletedtokens/savedtaskcontexts toforceprogress.',
  'Nativecodecompiledfirsttimeandunchangedthroughverification. ReferenceCPUonlyaddsTASpurge65f1d2;flat originalexecutioncode unchanged. Boundary-observationdataonlysuppresseslocal668b32->668b24loopasnewcall;everyoriginalinstruction/memory/state check remains. Independent1/2/31wordtestsproveonelogicalcall/allloopiterations. Originalzero-count2^32iterationbyte-swap behaviorisnotcompletedorboundedbynewsource. No newsourcewaitlimits.',
  '2601freshoriginal/nativecomparisons36399independentchecks;14626previouscomparisonsretainedhistorical/notallrerun;17227combinedevidencecomparisons,notuniquecases orcodecoverage. Retainedqueuedtask/softwareIRQ/contextSRFD controls fresh223;authoredIRQadmissionsarenotoriginalcomparisoncredit. No fresh223warm/coldstartup;earlierwarm/cold remainhistorical. Overallabout10%weightedmilestones,fullgame/portsunfinished;goalactive.',
  'A descriptive1035rowcountscope typo was correctedwithbeforefiles/hashes preserved andexplicitmetadata-only receipt;allnative/source/build/returnbytesandbehavioralcase recordsunchanged. Proofinputidentitypinsupdatedonlyafter checkinggeneratedmetadataequivalence. Verifierfixes(delayedbranchcoverage,wordloopcalllogging,twoheaderprojectionbytes,actualselectorliteral/getteralias/NOP-preservesR0) retaincompletecomparisons;noneare nativecodepatchesorrelaxedboundaries.'
 ]
 q=dict(status='partially_verified_input_pump_and_response_integration',compiled_source_count=223,newly_registered_entries=9,newly_qualified_rows=706,qualified_conditional_outcomes=95,routines=qualified,canonical_reuse=canonical,canonical_duplicate_credit=0,new_guarded_adapter_rows=1035,canonical_adapter_rows=370,proposed_response_rows=665,old_outer_api_rows=41,executed_integration_rows=1076,executed_integration_conditional_outcomes=147,structurally_unreachable_edges=dead,coverage_by_routine=coverage,fresh_original_native_comparisons=comparisons,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=14626,total_evidence_original_native_comparisons=17227,prior_registry_entries=1493,result_registry_entries=1502,result_unique_module_address_pairs=1499,flat_return_guard_sites=audit['guard_call_return_sites'],new_resume_exports=0,qualified_nonlocal_resume_routes_total=8,remaining_unqualified_resume_routes=74,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=previous_data['historical_warm_profile'],historical_cold_profile=previous_data['historical_cold_profile'],next_canonical_dependencies=next_canonical,next_original_dependencies=list(next_addresses),next_task='Integrate the preserved65efda submit/65eeda commands-prepare family and hardware handoff into current guarded ABI, withzero duplicatecreditwherealreadyregistered;qualifyrealqueued->inflighttoken transition thenactualresponse->taskresume withoutcompleted-token/parser-result injection.',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_input_pump_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED9newentries706rows/95outcomes;',comparisons,'fresh/',checks,'checks;goalactive10%')
if __name__=='__main__':main()
