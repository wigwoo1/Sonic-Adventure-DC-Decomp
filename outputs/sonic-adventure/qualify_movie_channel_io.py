"""Qualify readiness and bounded actual nested failure/resumption behavior."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 names=('translation','connection','units_verification','nested_verification','prefixes_verification','ordinary_regression','worker_regression','irq_verification','retained_verification','contract_audit')
 for name in names:
  path=P/f'analysis/movie_channel_io_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 t=reports['translation'];units=reports['units_verification'];nested=reports['nested_verification'];prefix=reports['prefixes_verification'];ordinary=reports['ordinary_regression'];worker=reports['worker_regression'];irq=reports['irq_verification'];retained=reports['retained_verification'];audit=reports['contract_audit']
 dll=digest(ROOT/'work/sonic-adventure/build/movie_channel_io.dll');assert all(r['dll_sha256']==dll for r in (units,nested,prefix,ordinary,worker,irq,retained))
 assert (len(t['routines']),t['instruction_rows'],reports['connection']['compiled_source_count'])==(12,391,221)
 expected={'units_verification':(208,2224),'nested_verification':(528,3416),'prefixes_verification':(32,400),'ordinary_regression':(240,2424),'worker_regression':(48,1072),'irq_verification':(48,424),'retained_verification':(35,335)}
 for name,(n,checks) in expected.items():r=reports[name];assert (r.get('original_native_comparisons',r.get('total_original_native_comparisons')),r['independent_checks'])==(n,checks)
 assert (nested['admission_bases'],nested['finite_lifecycles'],nested['pending_prefix_lifecycles'],nested['negative_return_lifecycles'])==(8,48,8,8)
 assert (audit['instruction_rows_byte_checked'],audit['explicit_nonfunction_continuation_routes'],audit['guard_allowed_call_returns'])==(1332,82,1920)
 coverage=[]
 for body in t['routines']:
  samples=[next(r for r in report['coverage_by_routine'] if r['address']==body['address']) for report in (units,nested,prefix)]
  missing=set.intersection(*(set(r['missing_rows']) for r in samples));missing_edges=set.intersection(*(set(map(tuple,r['missing_outcomes'])) for r in samples))
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1488,1489);previous=registry[:1488];canonical=[];proposed=[]
 for body in t['routines']:
  old=next((r for r in previous if r['address']==body['address'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'),None)
  if old:assert old['retail_span_sha256']==body['retail_span_sha256'];canonical.append(old)
  else:proposed.append(body)
 assert len(canonical)==8 and len(proposed)==4
 body=next(r for r in proposed if r['address']=='0x8c660dec');c=next(r for r in coverage if r['address']==body['address']);assert (c['static_rows'],c['executed_rows'],c['executed_outcomes'])==(38,38,6) and not c['missing_rows'] and not c['missing_outcomes']
 routine=dict(module='1ST_READ.BIN',name=body['name'],address=body['address'],end_exclusive=body['end_exclusive'],source=t['source'],retail_span_sha256=body['retail_span_sha256'],instruction_rows=38,executed_rows=38,executed_outcomes=6)
 for r in canonical:
  c=next(x for x in coverage if x['address']==r['address']);assert not c['missing_rows'] and not c['missing_outcomes']
 prior=P/'analysis/movie_channel_decoder_qualification.json';old=json.loads(prior.read_text());assert old['total_evidence_original_native_comparisons']==11716;files.append(prior)
 comparisons=sum(n for n,c in expected.values());checks=sum(c for n,c in expected.values());assert (comparisons,checks)==(1139,10295)
 for folder,addresses in [('movie-channel-io-request',('8c660494','8c6604e6','8c646654')),('movie-channel-io-request-lower',('8c6604c0','8c660518'))]:
  files.append(P/'analysis'/folder/'diagnostics.tsv')
  for address in addresses:
   for suffix in ('_instructions.tsv','_draft.c'):files.append(P/'analysis'/folder/(address+suffix))
 files.append(P/'ghidra/export_movie_channel_io_request_lower.ps1')
 limitations=[
  'Newreadiness660dec38rows/sixconditionaloutcomes fullyqualifiedwithincoherentRAM/globalobject/retailindex-map/header/status/flagdomains. No physicalinputacquisition/IRQfrequency/timing orsuccessfulheaderparseclaim. Othernew6608a4/646982/6469c8 remainpartial;8canonicalhelpers96rows integratewithzero duplicatecredit.',
  '391originalrows in12integrationbodies:295newproposedrows+96canonicalrows.941olderdecoder/workerrows adaptedforcallcontinuations withzeroadditionalretailcredit.82nativeafter-callcontinuationexports areNOTnewretailfunctions. Onlysixnewnonlocalreturn-chainroutes arebehaviorqualified here:660a68,64756e,64831c,648400,647a08,6479fa;existing668e48/647b74yield routesremain. Arbitrarycallback/resumegraphs andtheother76newresumeexportsremainunqualified.',
  'ResumedRTS capturesoriginaltargetbeforeitsdelayslot andusesexactflatmodel1920call-returnset pluscapturedouterentryPRhostmetadata. OrdinaryfunctionentryPRchecks unchanged;fourconnectedwrappercontexts capturetheirentryPRwithoutchangingexternal17/18/19/20argumentABIs orarchitecturalstate. GuardmembershipdoesNOTproveall1920nativecontinuationgraphs orarbitrarydirectroot-returnlanding. Bad-saved-returnfixture stops-3at660a7abeforedelay,notweakenedintounknownjump.',
  'Actualoriginal6477cc/660842/668bee constructors,647c50/647c6c callbackregistration,647cc4payloadsubmission,647cb4commandposting and668e60scheduler executecommands1/18. Headerinactivefailure yieldsinside6608a4;latercontextrestore resumesIO/parser/decoder/worker withallnestedSP/PR/deadline/GPR/FPU/special/device/boundary state matched. Budgets0/2 cause1/3actualIOyields thenoriginal-255completion/cleanup/callback/idle/teardown. Budget-1 stayspendingfor3observedschedulerreturns thenactualownercancellation. Eightseparatefaultfixtures corruptonesavedPR solelytoverifyguard;no normal taskcontext/counter/stackrepairs or parser/callbackresultsubstitutes.',
  'Initialcoherentinactiveheader/input-object/descriptor/globalcontrolfixtures andunmodifiedretailmappingareexplicit;callerretryconfigurationisABIinput. Thesearestandalonetasklifecycles,NOTprovednaturalresourceacquisition/fullboot. Actualheader1/flags2path postsoriginalrequestmarkerthenstopsatrealmissing660494. Wrapper6469c8reachesmissing6604e6;no requestid/parserresult injected. TAS/requestsub-bodies6604c0/660518 andpump646654 exportsare researchonly.',
  '1139freshoriginal/nativecomparisons10295checks:208units2224,528nested3416,32prefix400,240ordinary2424,48worker1072,48IRQ424,35contextFD335.11716previouscomparisonsremainhistorical/notallrerun;12855combined.16authoredIRQadmissions nooriginalcomparisoncredit. No fresh221warm/coldstartup;216warm/211cold retainhistoricalstatus. Oldworker/source/proofs remainpreserved,whilecurrentcompileddecoder/workerFORKSaddcontinuations;pastcoverageisnotrelabeledassource-identical/fresh221coverage.',
  '221strictDLLandall221unbounded-fsyntax-only-Wall-Wextra-Werror pass. Modelinstructionexecutioncode/SRFDguards/runtimeblocks/externalABIs unchanged;bridgeinversecheckrestoresoldexecutabletextafterdeclaredadditions. Old2673buildbytespreserved. Readiness/unit/canonical results deriveactualcode;metadataoutputs haveindependentfullRAM/stack/registeroracles. No newsourcewaitlimit. Owner+20c semanticsremainundetermined andoriginalzero/decrementwrap persists.',
  'Nextrecoveractualrequestbackends660494/6604e6,TAS.b andtheir6604c0/660518 callees,plusoptional646654pump. Keeporiginalmutex/diagnostic/requesteffects;donotmanufactureids/completion/headerdata. Thenqualifyactiveheader/packet/metadata parsingandsuccessfuldecoderreturns. Fullgame/PC/Android/3DS/N64/PS1/FPS8K/SUPIR/settings remainunfinished;TextureOff disablessampling/usesvertexcolors. Goalactiveabout10%weightedmilestones,notmeasuredcodecoverage.'
 ]
 q=dict(status='partially_verified_io_family',newly_registered_entries=1,newly_qualified_rows=38,qualified_conditional_outcomes=6,compiled_source_count=221,routines=[routine],new_integration_rows=391,new_proposed_original_rows=295,canonical_integration_rows=96,canonical_reuse=canonical,reused_additional_credit=0,adapted_previous_rows=941,adapted_additional_credit=0,explicit_native_nonfunction_continuations=82,qualified_nonlocal_resume_routes=['0x8c660a68','0x8c64756e','0x8c64831c','0x8c648400','0x8c647a08','0x8c6479fa'],remaining_unqualified_new_resume_routes=76,flat_return_guard_sites=1920,prior_guard_sites=1883,internal_root_pr_metadata=True,ordinary_entry_guards_preserved=True,arbitrary_resume_graphs_qualified=False,coverage_by_routine=coverage,executed_integration_rows=sum(r['executed_rows'] for r in coverage),partial_entries=[r['address'] for r in proposed if r['address']!='0x8c660dec'],fresh_original_native_comparisons=comparisons,fresh_independent_checks=checks,authored_irq_admission_cases=16,retained_previous_evidence_original_native_comparisons=11716,total_evidence_original_native_comparisons=11716+comparisons,prior_registry_entries=1488,result_registry_entries=1489,result_unique_module_address_pairs=1486,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=retained['historical_cold_profile'],next_original_dependencies=['0x8c660494','0x8c6604e6','0x8c6604c0','0x8c660518','0x8c646654'],next_task='RecoveractualTAS/requestbackend660494/6604e6 andtheir6604c0/660518callees,optional646654pump;preservemutex/diagnostic/requestsemantics andqualifyoriginalactiveheaderparserwithoutfakeids/results/completions.',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_channel_io_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED READINESS38/6;IOPARTIAL;',comparisons,'fresh/',checks,'checks;6nonlocalroutesqualified')
if __name__=='__main__':main()
