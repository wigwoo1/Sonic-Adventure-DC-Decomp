"""Qualify the original memory-accounting family in the complete 202-source state."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=['movie_graphics_handoff_translation','movie_graphics_handoff_connection','movie_graphics_handoff_verification','movie_graphics_handoff_retained_verification','movie_graphics_handoff_startup_verification','movie_graphics_constructor_qualification'];reports={};files=[Path(__file__)]
 for name in names:
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t,connection,direct,retained,startup,prior=(reports[name] for name in names);dll=digest(ROOT/'work/sonic-adventure/build/movie_graphics_handoff.dll')
 assert connection['compiled_source_count']==202 and (direct['cases_passed'],direct['independent_checks'])==(137,2181) and (retained['total_original_native_comparisons'],retained['independent_checks'])==(94,1922)
 assert direct['dll_sha256']==retained['dll_sha256']==startup['dll_sha256']==dll and prior['total_evidence_original_native_comparisons']==3640
 assert len(startup['cases'])==2 and startup['actual_next_frontier']=='0x8c6536ec'
 assert sum(r['static_rows'] for r in direct['coverage_by_routine'])==sum(r['covered_rows'] for r in direct['coverage_by_routine'])==265
 assert sum(r['conditional_outcomes'] for r in direct['coverage_by_routine'])==sum(r['covered_outcomes'] for r in direct['coverage_by_routine'])==60
 assert all(not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
 assert {r['address'] for r in t['routines']}=={'0x8c608d88','0x8c653dc0','0x8c663a60'}
 limitations=[
 'Three original own bodies265instructions/60conditional outcomes fullyexecute andqualify:608d88selector,653dc0aggregate,663a60list/cache accounting. StaticC,no runtimeopcode decoding. No new credit forolder providers,authored support,orstillpartialgraphicsroot656940(two outcomes).',
 '137fresh direct original/native comparisons2181independentchecks coverbankfree/non-active totals,signedlargestblock,32bitsumwrap,eightcachetypes/slotcounts/masks,zero/nonzero/unknown type fields,selector/globalbypass,outputaliases,threeFPUprofiles,VRAMaliases/bounds,andpartialbad-outputfailures. CompleteCPU/FPU/RAM/VRAM/Sound/device/orderedboundaries compare;no counts/childreturns/checkpoints injected. Finitecoherentmappedaligned metadata;privatecallee stackdisjoint fromoutputs/inputs. Cycles/destructiveprivate-stackaliases,physicalalignmentexceptions/concurrency remainunfinished.',
 'Initialtestfixturebusyfield collided withlegacyintegerbusy metadata;accountingfieldsnowprefixed. A generatorlocalname was correctedbeforevalidcomparisons; a standalonepreflight incorrectlyexpectedlegacybusy beforeprepare andwasdiscarded. DirectVRAMcaseexposedthenewstage inheritedaccessguardrejectingwordstore supportedbyexistinginterruptbus. Onlynewstageguardnowacceptsaligned32bitphysical05000000..057ffffc andaliases. FourteenfocusedVRAMpilots thenall137casespass;failedruns/pilotsnotdoublecounted. Priorpublishedsource/proofs/builds unchanged.',
 '94freshroot/constructor comparisons1922checks replayunchangedoracles in202-sourceDLL;olderreports preserved. Bothfreshconfigured0546c0startup profiles retainfull8MiBclear/two2MiBSoundfills/two33376bytetransfers/originalIRQ/RTE/REQUESTreturns/all1065282Soundevents0drops/39queue records/32MIDIsends. Exactprior201-sourceboundaryprefixthrough608d88matches. Query executesrealproviders,driverinitializationreturns andparentcalls6406ac;honestnewfrontier6536ec. Aninitialstartupcapturedold201testmodule;discardeddiagnosticnotcounted. Newcaptureguardsmodule/DLL/export/manifest identity.',
 'Alloriginal201component source/proof/build bytes remainpreserved. New202bridge/probe retain17/18/19/20argument APIs byexactprefix-normalizedsource comparison;freshdirectandfullstartup verifyactuallyusedentrypaths. Source/buildtraceinputs explicit;no shortenedclear/copy,consolepriming,guestcheckpoint,historiccoldsubstitution,orARMacknowledgement.',
 'FullgamePC60/120/240FPS8K,Android/stereoscopic3DS,N64/PS1,SUPIR/texturepercent/render/aspectsettings remainunfinished. TextureOffdisablestexturesandusesvertexcolors. No visiblepixels,audibleoutput,physicalIRQtiming,resetboot,gameplayorplayableportclaim. GoalACTIVE10%weighted milestones,notmeasuredcodecoverage.'
 ]
 fresh=137+94+len(startup['cases']);assert fresh==233
 q=dict(routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines']],canonical_reuse=[],newly_registered_entries=3,newly_qualified_rows=265,executed_rows=265,proved_infeasible_rows=0,executed_conditional_outcomes=60,proved_infeasible_conditional_outcomes=0,qualified_conditional_outcomes=60,compiled_source_count=202,new_direct_original_native_comparisons=137,new_direct_independent_checks=2181,fresh_retained_original_native_comparisons=94,fresh_retained_independent_checks=1922,fresh_full_startup_comparisons=2,fresh_startup_independent_checks=startup['independent_checks'],fresh_original_native_comparisons=fresh,retained_201_source_original_native_comparisons=3640,total_evidence_original_native_comparisons=3640+fresh,actual_next_frontier='0x8c6536ec',remaining_unqualified_graphics_root_outcomes=[['0x8c656cf4','0x8c656d06'],['0x8c656e90','0x8c656f38']],reused_additional_credit=0,dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_graphics_handoff_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified3accountingbodies265rows60outcomes;233freshcomparisons,frontier6536ec')
if __name__=='__main__':main()
