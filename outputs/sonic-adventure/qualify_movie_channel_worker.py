"""Record partial worker coverage without granting premature registry credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','verification','pool_verification','irq_verification','retained_verification','contract_audit'):
  path=P/f'analysis/movie_channel_worker_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 d=reports['verification'];pool=reports['pool_verification'];irq=reports['irq_verification'];retained=reports['retained_verification'];t=reports['translation'];audit=reports['contract_audit']
 dll=digest(ROOT/'work/sonic-adventure/build/movie_channel_worker.dll');assert all(r['dll_sha256']==dll for r in (d,pool,irq,retained))
 assert t['instruction_rows']==272 and len(t['routines'])==9 and reports['connection']['compiled_source_count']==219
 assert d['original_native_comparisons']==296 and d['original_admissions']==36 and len(d['commands'])==100
 assert (pool['lifecycles_passed'],pool['original_native_comparisons'],pool['independent_checks'])==(8,48,1072)
 assert (irq['lifecycles_passed'],irq['original_native_comparisons'],irq['independent_checks'])==(8,48,424)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(35,335)
 assert (audit['instruction_rows_byte_checked'],audit['dispatch_slots'],audit['unique_dispatch_targets'])==(272,21,19)
 coverage=d['coverage_by_routine'];partial=next(r for r in coverage if r['address']=='0x8c647980');assert partial['missing_rows'] and partial['missing_outcomes'] and not partial['complete']
 assert sum(r['complete'] for r in coverage)==8
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1485
 reused=[r for r in t['routines'] if any(old['address']==r['address'] and old.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for old in registry)]
 assert len(reused)==1 and reused[0]['address']=='0x8c66086a'
 canonical=next(r for r in registry if r['address']=='0x8c66086a' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN')
 assert canonical['retail_span_sha256']==reused[0]['retail_span_sha256']
 files.extend((P/canonical['source'],P/canonical['verification']))
 prior=P/'analysis/movie_task_owner_qualification.json';old=json.loads(prior.read_text());assert old['total_evidence_original_native_comparisons']==10518;files.append(prior)
 comparisons=d['original_native_comparisons']+pool['original_native_comparisons']+irq['original_native_comparisons']+retained['total_original_native_comparisons'];checks=sum(r['independent_checks'] for r in (d,pool,irq,retained))
 next_entries=['0x8c6482a0','0x8c64839a','0x8c64850a'];exports=P/'analysis/movie-channel-decoders'
 for name in next_entries:
  for suffix in ('_instructions.tsv','_draft.c'):files.append(exports/(name[2:]+suffix))
 files.append(exports/'diagnostics.tsv')
 limitations=[
  '272 bytechecked originalrows translated asstaticC:8proposedentries269rows plusalreadyregistered66086a canonicalyieldthunk3rows adaptedtoexistingstate/busABI withzero duplicatecredit. Shared647982/idle647b74nonfunctionresume routes.7selector wrappers andyieldthunk arefullyexecuted,while255row worker hasremainingunexecutedpostdecoder/callback rows/outcomes. Newregistrycredit deliberatelyzero;1485registryentries/1482uniquepairs unchanged.',
  '32 actualsingle-channel originaladmission lifecycles createall8selectors underFR0/FR1 andtwoSRbankprofiles;fourreal scheduler/yield/resume cycles each thenoriginalteardown.8moreactualinterleavedlifecycles use0/4/8tasks. Originalconstructorsinitializeeverytaskcontext/stack;no taskslots/contexts/counters patchedafteradmission.',
  'All21commands plus4unsignedout-of-range values testedforchannel0/channel7 andFR0/FR1. Command/payloadwordsareexplicitprefixfixtures;payload meaning/resource validity fordecoders isnotqualified.18nonidlecommands stopatexactreal missingdecoderaddresses,withoriginalargument/stack/storeeffects. No callback/decoderresult supplied. Commandpostprocessing/callback pathnotprovedcomplete.',
  'RuntimeRAM signedhalfword tableload and BRAFtargetretained;all21slots/19targets matchretaildata. Sharedinit andidle continuationbypassfunction-entryobservation andnative resumptiondoesnotreruninit. Worker addsno researchloopguard. Priorcontext/SR/FD/exception/runtime/device/ABIbehavior unchanged;originalowner+20c zero/decrementwrap preserved.',
  '35fresh retainedcontext/SR/FDcontrols and8softwareIRQlifecycles48comparisons pass.16authoredIRQadmissions receivezerooriginalcodecomparisoncredit. No fresh219warm/coldstartup;216warm/211cold evidencehistorical/notrerun. No physicalcontrolleracquisition,hardwaretiming/IRQfrequency,automaticframewaitpump,fullboot/gameplay/fullportproof.',
  'Next recover6482a0(command1),64839a(command18),64850a(sentinel cleanup);read-onlyoriginalexports preserved. FirsttwocalleesinterpretR4asindex-derivedstate inatleasttheirprefixes;do notassumeallcommandsconsumeasinglepointertype. Other16commandcalleesremainunrecovered. Additionaldecoder return/nonlocalcontinuations maybeneeded;onlyactualidle647b74routequalifiedhere.',
  'Goalactiveabout10%weightedmilestones,notmeasuredcodecoverage. Fullgame/PC/Android/3DS/N64/PS1/FPS8K/SUPIR/settingsunfinished. TextureOff disablestexturesampling/usesvertexcolors.'
 ]
 report=dict(status='partially_verified',compiled_source_count=219,proposed_function_entries=8,translated_instruction_rows=272,proposed_new_instruction_rows=269,canonical_integration_rows=3,reused_additional_credit=0,canonical_reuse=[canonical],executed_translated_rows=sum(r['executed_rows'] for r in coverage),executed_new_rows=sum(r['executed_rows'] for r in coverage)-3,worker_fully_qualified=False,newly_registered_entries=0,newly_qualified_rows=0,result_registry_entries=1485,result_unique_module_address_pairs=1482,coverage_by_routine=coverage,fresh_worker_original_native_comparisons=d['original_native_comparisons'],fresh_worker_independent_checks=d['independent_checks'],fresh_pool_original_native_comparisons=48,fresh_pool_independent_checks=1072,fresh_irq_original_native_comparisons=48,fresh_irq_independent_checks=424,fresh_retained_original_native_comparisons=35,fresh_retained_independent_checks=335,fresh_original_native_comparisons=comparisons,fresh_independent_checks=checks,authored_irq_admission_cases=16,retained_previous_evidence_original_native_comparisons=10518,total_evidence_original_native_comparisons=10518+comparisons,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=retained['historical_cold_profile'],next_original_dependencies=next_entries,next_task='Recover original command1/18 decoder state paths6482a0/64839a andsentinel cleanup64850a;qualifyactualresource/index inputs anddecoderreturn/callbackpaths withoutservice resultsubstitutes. Preservefull21slot worker andcontinueidle647b74 state correctly.',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_channel_worker_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('PARTIAL CHANNEL WORKER',comparisons,'freshcomparisons',checks,'checks;',report['executed_new_rows'],'executedrows;zero registrycredit')
if __name__=='__main__':main()
