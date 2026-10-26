"""Qualify the original memory-manager constructor with checked unreachable paths."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=['movie_driver_translation','movie_graphics_constructor_verification','movie_graphics_constructor_infeasible_verification','movie_graphics_constructor_rebuild_verification','movie_graphics_root_workflow_verification'];reports={};files=[Path(__file__)]
 for name in names:
  path=P/f'analysis/{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
  for pinned,h in report['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t,replay,proof,rebuild,prior=(reports[name] for name in names)
 assert (replay['cases_passed'],replay['independent_checks'])==(28,588)
 coverage=replay['coverage'];assert (coverage['static_rows'],coverage['covered_rows'],coverage['conditional_outcomes'],coverage['covered_outcomes'])==(159,124,12,7)
 assert coverage['missing_rows']==proof['constructor_infeasible_rows'] and coverage['missing_outcomes']==proof['constructor_infeasible_outcomes'] and (proof['constructor_proved_infeasible_rows'],proof['constructor_proved_infeasible_outcomes'])==(35,5)
 assert proof['option_replay']['result']==0 and proof['first_two_descriptor_addresses']==['0x8c8a83e4','0x8c8a83fc'] and proof['cleared_descriptor_count']==4096
 assert rebuild['fresh_build_and_link_passed'] and rebuild['all_other_pe_bytes_equal'] and rebuild['published_dll_sha256']==replay['dll_sha256']==prior['dll_sha256'] and prior['total_evidence_original_native_comparisons']==3612
 body=next(r for r in t['routines'] if r['address']=='0x8c663140');registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in ('0x8c665a80','0x8c10d3c8','0x8c10db1c','0x8c653444','0x8c663080','0x8c664ac0','0x8c665440'):
  old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);reuse.append(old);files.extend((P/old['source'],P/old['verification']))
 limitations=[
 'One original constructor159static rows qualifies:124execute,35areprovedinfeasible;7conditional outcomes execute,5areprovedinfeasible. Native201source/DLL unchanged. Existing canonical callees earnzero duplicatecredit. Graphicsroot656940 stillpartial with2unqualifiedoutcomes;startup608d88unchanged.',
 '28fresh fullentry original/native comparisons588independentchecks covermode1/other,empty/finiteoccupied small/cache cleanup lists,allfree/allbusy/ffffflagpatterns andthreeFPUprofiles. Entire98304byte smallpool/77824byte cachepool/40bytebank metadata/globalcapacity bytes andreal provider/MMIOarguments haveindependentoracles. CompleteCPU/FPU/RAM/VRAM/Sound/device/orderedboundaries compare;no helperreturns/checkpoints supplied.',
 'Bothoriginalconstructorbranches actuallycall665a80 withfixedimmutableHARDWARE/EnableTexture16MB strings anddefault0. AnolderGhidradraft misleadingly names665a00 foronecall;checkedinstructions/literals andoriginalhelper replayconfirm665a80. No productionchange wasneeded. Fixed-input111stepverification-only replayreturns0,withunknownlive-ins rejected andprivate-stack-onlywrites.',
 'Originalclear writeszero toall4096descriptorflags. Its onlylatercache releasewrites flags ina disjoint76byte recordpool; exactcheckedscanner thereforefindsfirstandsecondfree descriptors. Constructorinterveningpayload/metadata stores do not consumeanotherdescriptor. Explicitdelay-slotCFG/dominators prove35deadrows/5infeasibleoutcomes. Runtimebranches intact;faultingcallees stopbeforelaterguesttests,notguessedreturns.',
 'Proofdomain:fullentry,immutablecode/literals/strings,normalSH4callee-savedABI,mappedalignedprivatestack andfinitecoherentcleanup lists/disjointpool-metadata-outputstorage,no asynchronousmutation. Corrupt/cyclic/arbitraryaliases,patchedselectors,physicalIRQ/timing andgeneralconcurrency remainunfinished. Finitelistcleanup mayretain other payloadbytes exactly;wholepooloracle checks them.',
 'Freshisolated201sourcecompile/link matchescompletePEexcept4byteCOFFtimestamp;strictunboundedsyntaxchecks includedinworkflow. Earlier3612runtime/startupcomparisons retained/hashchecked,NOT rerunfresh;3640combinedevidence. No newstartup/pixels/audio/gameplay/playableport claim. Allgame/ports/FPS8K/SUPIR/settingsrequirements persist;TextureOffusesvertexcolors;goalACTIVE10%weightedmilestones,notmeasuredcodecoverage.'
 ]
 q=dict(routines=[body|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source']))],canonical_reuse=reuse,newly_registered_entries=1,newly_qualified_rows=159,executed_rows=124,proved_infeasible_rows=35,executed_conditional_outcomes=7,proved_infeasible_conditional_outcomes=5,qualified_conditional_outcomes=12,compiled_source_count=201,native_source_or_build_changed=False,fresh_original_native_comparisons=28,fresh_independent_checks=588,retained_original_native_comparisons=3612,total_evidence_original_native_comparisons=3640,reused_additional_credit=0,graphics_root_complete=False,remaining_unqualified_root_outcomes=[['0x8c656cf4','0x8c656d06'],['0x8c656e90','0x8c656f38']],dll_sha256=replay['dll_sha256'],limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_graphics_constructor_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified constructor:124executed+35infeasible rows;7+5outcomes;28freshcomparisons')
if __name__=='__main__':main()
