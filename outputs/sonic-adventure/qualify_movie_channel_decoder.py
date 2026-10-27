"""Qualify complete local bodies and retain explicit active-decoder boundaries."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 names=('translation','connection','leaves_verification','paths_verification','callers_verification','worker_regression','irq_verification','retained_verification','contract_audit')
 for name in names:
  path=P/f'analysis/movie_channel_decoder_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 t=reports['translation'];leaves=reports['leaves_verification'];paths=reports['paths_verification'];callers=reports['callers_verification'];worker=reports['worker_regression'];irq=reports['irq_verification'];retained=reports['retained_verification'];audit=reports['contract_audit']
 dll=digest(ROOT/'work/sonic-adventure/build/movie_channel_decoder.dll');assert all(r['dll_sha256']==dll for r in (leaves,paths,callers,worker,irq,retained))
 assert (len(t['routines']),t['instruction_rows'],reports['connection']['compiled_source_count'])==(9,669,220)
 assert (leaves['original_native_comparisons'],leaves['independent_checks'])==(240,2424)
 assert (paths['original_native_comparisons'],paths['independent_checks'])==(128,1456)
 assert (callers['original_native_comparisons'],callers['independent_checks'],callers['command_lifecycles'])==(272,1496,48)
 assert (worker['original_native_comparisons'],worker['independent_checks'])==(48,1072)
 assert (irq['original_native_comparisons'],irq['independent_checks'])==(48,424)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(35,335)
 assert audit['instruction_rows_byte_checked']==669 and audit['canonical_rows']==17
 coverage=[]
 for body in t['routines']:
  samples=[next(r for r in report['coverage_by_routine'] if r['address']==body['address']) for report in (leaves,paths,callers)]
  missing=set.intersection(*(set(r['missing_rows']) for r in samples));missing_edges=set.intersection(*(set(map(tuple,r['missing_outcomes'])) for r in samples))
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 qualified={'0x8c64850a':(21,2),'0x8c64b4e4':(80,36),'0x8c648470':(57,2)};routines=[]
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1485,1488);previous=registry[:1485]
 for body in t['routines']:
  if body['address'] not in qualified:continue
  c=next(r for r in coverage if r['address']==body['address']);n,outcomes=qualified[body['address']];assert (c['static_rows'],c['executed_rows'],c['executed_outcomes'])==(n,n,outcomes) and not c['missing_rows'] and not c['missing_outcomes']
  assert not any(old['address']==body['address'] and old.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for old in previous)
  routines.append(dict(module='1ST_READ.BIN',name=body['name'],address=body['address'],end_exclusive=body['end_exclusive'],source=t['source'],retail_span_sha256=body['retail_span_sha256'],instruction_rows=c['static_rows'],executed_rows=c['executed_rows'],executed_outcomes=c['executed_outcomes']))
 canonical=next(r for r in previous if r['address']=='0x8c647bde' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN');body=next(r for r in t['routines'] if r['address']==canonical['address']);assert canonical['retail_span_sha256']==body['retail_span_sha256'];cc=next(r for r in coverage if r['address']==canonical['address']);assert not cc['missing_rows'] and not cc['missing_outcomes']
 prior=P/'analysis/movie_channel_worker_qualification.json';old=json.loads(prior.read_text());assert old['total_evidence_original_native_comparisons']==10945;files.append(prior)
 old_worker_source=P/'source/sa_movie_channel_worker.c';assert old['sha256'][str(old_worker_source)]==digest(old_worker_source);files.append(old_worker_source)
 old_worker=next(r for r in old['coverage_by_routine'] if r['address']=='0x8c647980');fresh_worker=next(r for r in callers['coverage_by_routine'] if r['address']=='0x8c647980');missing=set(old_worker['missing_rows'])&set(fresh_worker['missing_rows']);missing_edges=set(map(tuple,old_worker['missing_outcomes']))&set(map(tuple,fresh_worker['missing_outcomes']));assert len(missing)==33 and len(missing_edges)==1
 comparisons=sum(r['original_native_comparisons'] for r in (leaves,paths,callers,worker,irq))+retained['total_original_native_comparisons'];checks=sum(r['independent_checks'] for r in (leaves,paths,callers,worker,irq,retained));assert (comparisons,checks)==(771,7207)
 next_exports=P/'analysis/movie-channel-decoder-io'
 for address in ('8c6608a4','8c660dec'):
  for suffix in ('_instructions.tsv','_draft.c'):files.append(next_exports/(address+suffix))
 files.append(next_exports/'diagnostics.tsv')
 limitations=[
  'Three completeoriginal localbodies158rows/40outcomes qualifywithinexplicitcoherentRAM/index/context/callbackdomains:64850a21/2 cleanup,64b4e480/36 descriptorvalidator,64847057/2 inactive-return/active-tail gateway. Gatewayactivepath executescanonicalcallback then tailtransfersintoactual64b1f0,whichstopsatrealmissing64ae32. This qualifiesgateway instruction/ABIeffects,NOTcompleteactive decoding. Invalidmemory/nonlocalcallback domains remainunqualified.',
  '669rows in9translatedbodies:8newproposals652rows pluscanonical647bde17rows withzeroadditionalcredit. Fiveothernewdecoder bodies remainpartial:6482a0,64839a,64753c,660a7e,64b1f0. Wholefamily executes368/669rows acrossfreshdirect/callercases;17canonicalrowsincluded. Partialcode/source isretainedbutnotregistered.',
  '240directleaf/canonicalcomparisons2424checks includeallcleanup/validatorrows/outcomes,independentfullRAM/cleanupGPR/SR/MACL/return oracles,kind/count/capacity signedboundaries andnull/realNOP/unknowncallbacks.128directdecoderpathcomparisons1456checks:inactiveROOT1/mode1entireRAM/stack/GPR/FPUreturns andactiveprefixoriginalpartialwrites/missingcallee boundaries. Parameter tables differ:command1/mode1 base673ba8,command18base673bec;originalvaluespreserved.',
  '272 actualcallercomparisons1496checks:8originaladmission/idle bases,48command1lifecycles usingoriginal647c50callbacksetter,647cc4payloadsubmission and647cb4commandposting,resourceindices0/7,twoFRprofiles/globalmode0/1. Descriptor/globalobjectinactivefixture setbeforeoriginaltaskconstructors. No taskslot/context/counter patches orcallee results supplied. Returned sentinel-255 triggersactual64850a thenworkercommand/statuswrites,callback andidle/resumption/teardown. Unknownpostcallbackstops-2 afterrealrecordclear/statuswrites;nofakecompletion.',
  'Originalworkerbyte sourceunchanged. Mixedhistorical219/current220evidence increasesworkercoverage186->222of255rows,leaving33rows/oneconditionaloutcomeunqualified. Thesearecombinedsource-identical coveragecounts,NOTa freshcomplete220workerreplay. Freshcaller-onlyworkercoverage isrecordedseparately. Other16commanddecoders/non-sentinelreturns/arbitrarycallbackresumption remainpartial;zero newworker/selector credit.',
  '771freshoriginal/nativecomparisons7207checks:240leaf2424,128paths1456,272callers1496,48worker1072,48IRQ424,35retained335.10945earlierevidencehistorical/notallrerun;11716combined.16authoredIRQadmissionszerooriginalcomparisoncredit. No fresh220warm/cold;216warm/211cold remainhistorical. Strict220DLL/all220unboundedsyntax pass;oldmodelcode/guards/continuations/runtime/ABI/source/proofs/builds preserved.',
  'Actualnextdependencies6608a4(216rows) and660dec(38rows) exportedread-onlyasresearch,zeronativecredit. Theyreach646856/646868/646b36andotherrecord-parser/backend helpers. Futureactualnonlocalyieldpaths neednested6608a4/660a7e/decoder/worker continuations;onlyexistingidle647b74 isqualifiednow. Do notapplyprefixpointerfixtureasvaliddecoderresource orinjectcallback/parser results.',
  'Owner+20c meaning remainsundetermined;originalzero/decrementwrap retained. Physicalcontrolleracquisition/IRQtiming/automaticframewait/fullboot/gameplay/fullgame andPC/Android/3DS/N64/PS1/FPS8K/SUPIR/settings unfinished. TextureOff disablessampling/usesvertexcolors. Goalactiveabout10%weightedmilestones,notmeasuredcodecoverage.'
 ]
 q=dict(status='partially_verified_family',newly_registered_entries=3,newly_qualified_rows=158,executed_new_qualified_rows=158,qualified_conditional_outcomes=40,compiled_source_count=220,routines=routines,translated_instruction_rows=669,executed_translated_rows=sum(r['executed_rows'] for r in coverage),proposed_new_instruction_rows=652,canonical_integration_rows=17,canonical_reuse=[canonical],reused_additional_credit=0,partial_entries=[r['address'] for r in t['routines'] if r['address'] not in qualified and r['address']!=canonical['address']],coverage_by_routine=coverage,previous_worker_coverage=dict(address='0x8c647980',source_identical=True,combined_historical_and_current=True,static_rows=255,executed_rows=255-len(missing),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges),fresh_caller_executed_rows=fresh_worker['executed_rows'],fully_qualified=False),fresh_original_native_comparisons=comparisons,fresh_independent_checks=checks,authored_irq_admission_cases=16,retained_previous_evidence_original_native_comparisons=10945,total_evidence_original_native_comparisons=10945+comparisons,prior_registry_entries=1485,result_registry_entries=1488,result_unique_module_address_pairs=1485,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=retained['historical_cold_profile'],next_original_dependencies=['0x8c6608a4','0x8c660dec'],next_task='Recover actualIO/header/state dependencies6608a4/660dec andtheir646856/646868/646b36backend helpers;qualifyvalidresource/recordinputs andactualnonlocalyield/resume paths without resultsubstitutes. Preservefullworker/decoderexports andexplicitmissingcallees.',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_channel_decoder_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED3 localbodies158rows40outcomes;familyPARTIAL;',comparisons,'fresh/',checks,'checks')
if __name__=='__main__':main()
