"""Qualify three fully covered queries, retaining canonical adapters and old policy proofs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
OWN={'0x8c5ff39c','0x8c5ff3fa','0x8c5ff494'};REUSED={'0x8c5ff396','0x8c04fe40'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('movie_system_state_translation','movie_system_state_connection','movie_system_state_verification','movie_system_state_retained_verification','movie_system_state_startup_verification','console_sysinfo_status_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_system_state_translation'];direct=reports['movie_system_state_verification'];retained=reports['movie_system_state_retained_verification'];startup=reports['movie_system_state_startup_verification'];platform=reports['console_sysinfo_status_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_system_state.dll')
 assert len(t['routines'])==5 and t['instruction_rows']==89 and reports['movie_system_state_connection']['compiled_source_count']==209
 assert (direct['cases_passed'],direct['independent_checks'],direct['executed_integration_rows'],direct['executed_integration_outcomes'])==(121,2009,89,16)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(289,4839)
 assert (len(startup['cases']),startup['independent_checks'],startup['actual_next_frontier'])==(2,146,'0x8c109cfe')
 assert (platform['cases_passed'],platform['independent_checks'],platform['corrected_guest_SYSINFO_INIT_R0'])==(30,272,0)
 assert all(r['dll_sha256']==dll for r in (direct,retained,startup,platform))
 for row in direct['coverage_by_routine']:assert not row['missing_rows'] and not row['missing_outcomes']
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in sorted(REUSED):
  old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);adapter=next(r for r in t['routines'] if r['address']==address);assert old['retail_span_sha256']==adapter['retail_span_sha256'];files.extend((P/old['source'],P/old['verification']));canonical.append(dict(address=address,canonical_name=old['name'],additional_recovery_credit=0))
 routines=[r|dict(kind='original_routine',module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines'] if r['address'] in OWN]
 assert len(routines)==3 and sum(r['instruction_rows'] for r in routines)==81 and sum(len(r['conditional_edges']) for r in routines)==16
 prior_path=P/'analysis/movie_float_initialize_qualification.json';prior=json.loads(prior_path.read_text());files.append(prior_path)
 for path,h in prior['sha256'].items():assert digest(path)==h,(prior_path,path)
 assert prior['total_evidence_original_native_comparisons']==5395
 evidence=P/'analysis/console_sysinfo_status_primary_evidence.json';files.extend((evidence,ROOT/'work/sonic-adventure/system-state-primary-redream-syscalls.c'))
 limitations=[
 'Three new original ownbodies:47-rowsettingsquery5ff39c,21-rowbytequery5ff3fa,13-rowinfolookup5ff494. All81rows16conditionaloutcomes execute. Two earliercanonicaladapters subtract5ff396/modepredicate04fe40 add8integrationrows andzeroadditionalrecoverycredit. No physicalalias duplicate matches exist for thenewownentries inpreviousregistry.',
 '121direct original/native cases2009independentchecks cover forcedsettings/signedbytes/ASCIIoffset/default1/nulls/missingrecord/readonlyerror/invalidoutputs/unconfiguredhostprefixes acrossFPUmodes. Initialglobal67340cisactualimplicitR5packetbuffer,notaboolean. Realnativeflash/CRC/consoleproviders execute. DefensivehelperbranchesuseexplicitinitialSYSINFOvectorstotheunchangedoriginalmodepredicateanddeclaredmodeword0or16;actualoriginalguestcodeexecutes,nothostreturnstubs orphysicalBIOSfailureclaims. Mappedcoherentdisjointbuffers/metadata/private stack;destructivealiases/selfmodification/arbitrarycyclesunfinished.',
 'DocumentedSYSINFO_INIT successreturnsR0zero. OlderhostservicepreservedR0(handleraddress);newadditiveconsole_sysinfo_status_service delegatesolddata/state/guards thenwritesonlycorrectfirmwarereturnstatus. OldserviceAPI/state layout/source/proofs retainedunderoldpolicy. Independent primarydocs/pinnedRedreamcodefacts corroborateABI;no emulatorimplementationcopied/called. Thirtyplatformonlycases272checks compareold/newentire16MiBRAM/state/counters/pass-through/errors,zerooriginalroutinecredit.',
 '289freshretainedcomparisons4839checks replayfloat128/upload78/IRQ23/flashsettings60. Unchangedoldfixture/oracles andsource/proofs preserved. Flashsubset excludes432word-copygridcases anddoesnotclaimfreshcompletecopycoverage;newmanifestcoverageinthatregressionhasnonewquerycredit. Other5395historicalevidencecases retained,notallrerun.',
 'Twofresh uninterruptedconfigured0546c0startups:4598warm/585896coldboundaries,146checks. AllCPU/FPU/RAM/VRAM/Sound/platform/runtime/flash/orderedboundaries compareagainstoriginalinstructionsunder correctedfirmwareABI. Recordedold207warm/206coldboundaryfieldprefixes match;notclaimingold/newR0/scratchRAMidentityacrossdifferenthostpolicies. Full8MiBclear/twoSoundfills/two33376Btransfers/G2IRQ/RTE/REQUEST/1065282Soundevents0drops/39SDKrecords1092B/32MIDIsends/32768BVRAMupload retained. Actualqueryusesbuffer8c900000 andadds1info/4reads256B;final2info/9reads514B,nowrites. No checkpoint/helperresult/priming/shortenedcopy/historiccoldsubstitution.',
 'Firstrootpilotused1forbufferpointerandhonestlystopped603c40. OriginalR5dataflow identified;correcteddirectfixture,notnativegamecode. Earliercoldcancelledbeforemodelsourcechange;finalfreshwarm/coldrerun. Missingrecordoracleinitiallyexpectedfailure;original603f8cacceptserasedFFFFslot,soactualdefaultfallbacksuccessisretained;addedgenuineblankreadonlyfailure. Allfinaltests rerun;failedrunsnotpassingcredit. Retainedflashsubsetharnessinitiallyassertedexcludedwordcopycoverage;removedonlythatunearnedclaim,newfullrunnerpassed.',
 'Broadgraphicsdispatcher652210stillpartial/unregistered92/259rows16/50outcomes;root656940stillpartialcf4->d06/e90->f38. Fullgame/ports/FPS8K/SUPIR/settings/pixels/audibleoutput/physicaltiming/resetboot/gameplayunfinished. TextureOffmustdisablesamplingandusevertexcolors. GoalACTIVE10%weighted milestones,notmeasuredcodecoverage.'
 ]
 q=dict(routines=routines,newly_registered_entries=3,newly_qualified_rows=81,executed_rows=81,qualified_conditional_outcomes=16,executed_conditional_outcomes=16,compiled_source_count=209,integrated_entries=5,executed_integration_rows=89,canonical_reuse=canonical,reused_additional_credit=0,coverage_by_routine=direct['coverage_by_routine'],new_direct_original_native_comparisons=121,new_direct_independent_checks=2009,fresh_retained_original_native_comparisons=289,fresh_retained_independent_checks=4839,fresh_full_startup_comparisons=2,fresh_startup_independent_checks=146,fresh_original_native_comparisons=412,fresh_independent_checks=6994,platform_only_cases=30,platform_only_independent_checks=272,retained_207_source_original_native_comparisons=5395,total_evidence_original_native_comparisons=5807,actual_next_frontier='0x8c109cfe',partial_unregistered_dispatcher='0x8c652210',remaining_unqualified_graphics_root_outcomes=prior['remaining_unqualified_graphics_root_outcomes'],dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_system_state_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED3/81/16;412freshcases6994checks')
if __name__=='__main__':main()
