"""Qualify six original non-root graphics controls without changing native code."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
NEW=('0x8c656860','0x8c665ae0','0x8c641c16','0x8c65614e','0x8c656660','0x8c657760')
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('movie_graphics_init_translation','movie_graphics_init_connection','movie_graphics_framebuffers_verification','movie_graphics_reset_verification','movie_graphics_mode_controls_verification','movie_graphics_mode_infeasible_verification','movie_graphics_modes_rebuild_verification','movie_graphics_buffers_qualification','movie_graphics_init_startup_verification');files=[Path(__file__)];reports={}
 for name in names:
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports[names[0]];frame=reports[names[2]];reset=reports[names[3]];mode=reports[names[4]];proof=reports[names[5]];rebuild=reports[names[6]];prior=reports[names[7]];dll=digest(ROOT/'work/sonic-adventure/build/movie_graphics_init.dll')
 assert (frame['cases_passed'],frame['independent_checks'])==(119,3094) and (reset['cases_passed'],reset['independent_checks'])==(68,1670) and (mode['cases_passed'],mode['independent_checks'])==(180,2807)
 assert frame['dll_sha256']==reset['dll_sha256']==mode['dll_sha256']==prior['dll_sha256']==dll and prior['total_evidence_original_native_comparisons']==3179
 assert rebuild['fresh_build_and_link_passed'] and rebuild['all_other_pe_bytes_equal'] and rebuild['published_dll_sha256']==dll and proof['own_infeasible_rows']==2 and proof['own_infeasible_outcomes']==3
 selected=[]
 for address in NEW:
  source=reset if address=='0x8c656860' else frame if address in ('0x8c665ae0','0x8c641c16') else mode
  row=next(r for r in source['coverage_by_routine'] if r['address']==address);selected.append(row)
  assert row['missing_rows']==proof['proven_infeasible_rows'].get(address,[])
  assert row['missing_outcomes']==proof['proven_infeasible_edges'].get(address,[])
 assert sum(r['static_rows'] for r in selected)==1024 and sum(r['covered_rows'] for r in selected)==1022 and sum(r['conditional_outcomes'] for r in selected)==98 and sum(r['covered_outcomes'] for r in selected)==95
 bodies=[r for r in t['routines'] if r['address'] in NEW];assert len(bodies)==6 and sum(r['instruction_rows'] for r in bodies)==1024
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in ('0x8c6574b8','0x8c657240','0x8c665cc0','0x8c663320','0x8c6634a0','0x8c664352','0x8c6578e0','0x8c657960','0x8c657968','0x8c651bc0','0x8c653444','0x8c653476','0x8c666706'):
  old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);reuse.append(old);files.extend((P/old['source'],P/old['verification']))
 limitations=[
  'Six original own bodies qualify1024static instruction rows:1022execute andtwoareprovedinfeasible.95conditional outcomes execute,threeareprovedinfeasible. Reset656860,framebuffers665ae0,pairedcontext641c16,listconfiguration65614e,textureconfiguration656660,dispatcher657760. Allactualnative providers execute;13existing canonical provider entries receive zero additionalcredit. Root656940 and memory-manager constructor663140 remain partial/unregistered;no completegraphics/gameclaim.',
  '367fresh original/native comparisons/7571independent checks pass:119frame/context cases3094checks,68reset cases1670checks,180configuration/dispatcher cases2807checks. CompleteCPU/FPU/RAM/VRAM/Sound/devices/orderedboundaries compare. Independent record/pool/cache/display/MMIO/copy/zero-word/count/timeout/listspace/min/capacity/allocator/result contracts supplement instruction decoding. Directmetadatafixtures are explicit,withmappedaligned distinctstack/records/backing;not a resumedinternalguestcheckpoint or substitutedchildresult.',
  'Reset invokes real651bc0softwareinterruptcallback. Originaltimeout clearsbusy afterthreshold+1callbacks;counterlimit stopsat11. Unknowncallback retainsgenuine-2andprefix. This does not establishphysicalIRQacceptance,clocktiming,arbitrarycallback/concurrency or fullhardware behavior.',
  'Original framebuffer clear writes complete2560bytechunks,including a lastchunk beyondrecordsize whennonmultiple;tests retainthat behavior. Original16wordlist canbecomezeroafterheaderreservation whileenabled,causing actualnullcopy-3at652544 afterpriorallocation;tests preservefaultingmachine/memoryprefix. Deferred656660returnsincomingR12withoutinitializingit whenstage|100!=b00;dispatcher maypropagate thatvalue. FourcallerR12valuesaretested;no normalizationtozero/success is introduced.',
  'Checkedretail instruction/literal bytes,explicitdelay-slotCFG/dominators and register-write sets prove pointer-builder normalreturnR0zero,counts1or2versusinitialzero pretests,andtwo deadreturnrows. Proof assumesfullentry,immutablecode/literals,normalSH4callee-savedABI,validdistinctstack/data andno asynchronous mutation. Runtimecode/branches/helperreturns unchanged;no infeasibleedge is forced to gaincoverage.',
  'All201native source/publishedbuild bytes unchanged. Freshisolatedbuild/link matchescompletePEimageexcept4byteCOFFtimestamp;strictunboundedsyntaxchecks retained inworkflow. Earlier3179comparisons andconfiguredfullstartupcaptures are retained/hashchecked,NOT rerun asfresh. Combinedevidence3546comparisons. Startupfrontier608d88unchanged;no newstartupadvancement/pixels/audio/playableport claim.',
  'FullgamePC60/120/240FPS8K,Android/stereoscopic3DS,N64/PS1,SUPIRHD/texturepercent/render/aspectfeatures remain unfinished. TextureOff disables textures andusesvertexcolors. Originalallrequirements/source/proofs/builds remainpreserved;overall10%weighted milestones is not measuredcodecoverage;goal stays active.'
 ]
 q=dict(routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in bodies],canonical_reuse=reuse,newly_registered_entries=6,newly_qualified_rows=1024,executed_rows=1022,proved_infeasible_rows=2,executed_conditional_outcomes=95,proved_infeasible_conditional_outcomes=3,qualified_conditional_outcomes=98,compiled_source_count=201,native_source_or_build_changed=False,fresh_original_native_comparisons=367,fresh_independent_checks=7571,retained_original_native_comparisons=3179,total_evidence_original_native_comparisons=3546,remaining_unqualified_graphics_manifest_bodies=1,reused_additional_credit=0,dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_graphics_modes_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified6mode/reset/frame controls:1022executed+2infeasible rows,95+3outcomes;367fresh comparisons')
if __name__=='__main__':main()
