"""Qualify two reached original leaves with complete measured instruction coverage."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','verification','retained_verification','startup_verification'):
  path=P/f'analysis/movie_float_initialize_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['translation'];direct=reports['verification'];retained=reports['retained_verification'];startup=reports['startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_float_initialize.dll')
 assert len(t['routines'])==2 and t['instruction_rows']==27 and reports['connection']['compiled_source_count']==207
 assert (direct['cases_passed'],direct['independent_checks'],direct['executed_rows'])==(128,2240,27) and not direct['missing_rows'] and direct['conditional_outcomes']==0
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(101,2127)
 assert (startup['cases_passed'],startup['independent_checks'],startup['boundary_comparisons'],startup['actual_next_frontier'])==(1,47,4376,'0x8c5ff39c')
 assert all(r['dll_sha256']==dll for r in (direct,retained,startup))
 prior_path=P/'analysis/movie_buffer_initialize_qualification.json';prior=json.loads(prior_path.read_text());files.append(prior_path)
 for path,h in prior['sha256'].items():assert digest(path)==h,(prior_path,path)
 assert prior['total_evidence_original_native_comparisons']==5165
 routines=[r|dict(kind='original_routine',module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines'] if r['address']=='0x8c60edb0']
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c60d1f2')
 adapter=next(r for r in t['routines'] if r['address']=='0x8c60d1f2');assert adapter['retail_span_sha256']==canonical['retail_span_sha256'];files.extend((P/canonical['source'],P/canonical['verification']))
 limitations=[
 'One neworiginalownbody:60edb0drawingstateinitializer24rows;inferredsemanticname. Canonicalsa_global_setter_8c60d1f2 isconnectedthrough3-rowadapterwithzeroadditionalcredit;originalcanonicalsource/proof/rowretained. All27integrationrows execute;nooriginalconditionalbranches orcallees. 128directcases2240checks include96completeinitializer/16unsupportedPRprefix/16settercases. RawGPR/SR/PR/stack/FPSCR/FPUL/bothFPUbanks/entireRAM independentoracles;fulloriginal/nativeCPU/RAM/VRAM/Sound/platform/orderedboundaries also compare.',
 'InitializerPR-clear modeledFMOV/FR/SZ byteeffects verified. Odd-registerpairedload usesoppositephysicalbank;overlapping64bitstoresleavefinalFR5wordbeyondordinaryfourfloatrange. PR-setstops-1atFLDI0afteractualearlierpointer/sizestores;no fakeunwind/helperreturn. Physicalillegal-instruction/alignmentexceptiondeliveryunfinished;pairedbyteeffects arenotaphysicalexceptionproof. Setterstoresrawuint32includingnull/unalignedwordswithoutdereferencingthem;actualRTSdelayslotwrite executes.',
 '101fresh retainedupload/IRQ/wait regressioncases2127checks in207-sourceDLL;previousreports/source unchanged. Onefresh uninterruptedconfigured0546c0warmstartup47checks/4376boundaries retains exact206-sourceprefix through60edb0;bothnewleavesexecute,existing5ff2ccruns,parentreachesunsupported5ff39c. Pointer/size/reset/fourfloat/setterstateindependentlychecked;fullclear/VRAM/flash/39SDKrecords/32MIDIsends retained.',
 'Prior206-source fullcoldproof retainedandhashchecked,notrerun orrelabeledas207-sourcecoldstartup. Itsfull8MiBclear/twoSoundfills/two33376Btransfers/originalG2IRQ/RTE/REQUEST/1065282Soundevents0drops evidence remains scopedto206-sourcebuild. Freshwarm/directtests andexactbridgeextensionchecksupportthisnewleafmilestone;no207coldclaim.',
 'Earlier1453canonicalrows/source/proof/builds preserved. Broadgraphicsdispatcher652210 remains partial/unregistered92/259rows16/50outcomes;root656940stillpartialcf4->d06/e90->f38. AuthoredVRAM/CH2/ISTERRpoliciesunchanged,zeroadditionaloriginalcredit. ExistingoldaccountingA400outputdomainstillunsupported. Allports/gameplay/pixels/audio/physicaltiming/resetboot/fullgamecode/FPS8K/SUPIR/settingsunfinished. TextureOffmustdisablesamplingandusevertexcolors. GoalACTIVE10%weighted milestones,notcodecoverage.'
 ]
 q=dict(routines=routines,newly_registered_entries=1,newly_qualified_rows=24,executed_rows=24,integrated_entries=2,executed_integration_rows=27,canonical_reuse=[dict(address=canonical['address'],canonical_name=canonical['name'],additional_recovery_credit=0)],executed_conditional_outcomes=0,qualified_conditional_outcomes=0,compiled_source_count=207,new_direct_original_native_comparisons=128,new_direct_independent_checks=2240,fresh_retained_original_native_comparisons=101,fresh_retained_independent_checks=2127,fresh_warm_startup_comparisons=1,fresh_warm_startup_independent_checks=47,new_cold_startup_comparisons=0,retained_cold_source_count=206,fresh_original_native_comparisons=230,fresh_independent_checks=4414,retained_206_source_original_native_comparisons=5165,total_evidence_original_native_comparisons=5395,actual_next_frontier='0x8c5ff39c',partial_unregistered_dispatcher='0x8c652210',remaining_unqualified_graphics_root_outcomes=prior['remaining_unqualified_graphics_root_outcomes'],reused_additional_credit=0,dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_float_initialize_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED1/24/0;canonicalsetter0additionalcredit;230freshcases4414checks')
if __name__=='__main__':main()
