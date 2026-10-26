"""Qualify six graphics builders and their descriptor provider;seven callers stay partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
NEW=('0x8c657240','0x8c6574b8','0x8c665cc0','0x8c664352','0x8c663320','0x8c6634a0')
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('movie_graphics_init_translation','movie_graphics_init_connection','movie_graphics_buffers_verification','movie_graphics_allocations_verification','movie_graphics_display_infeasible_verification','movie_graphics_buffer_abi_verification','movie_graphics_leaves_qualification','movie_graphics_init_startup_verification','movie_graphics_descriptor_verification','movie_graphics_buffers_rebuild_verification');files=[Path(__file__)];reports={}
 for name in names:
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports[names[0]];b=reports[names[2]];a=reports[names[3]];proof=reports[names[4]];prior=reports['movie_graphics_leaves_qualification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_graphics_init.dll')
 assert b['cases_passed']==508 and b['independent_checks']==6881 and a['cases_passed']==168 and a['independent_checks']==2520
 assert b['dll_sha256']==a['dll_sha256']==prior['dll_sha256']==dll and prior['total_original_native_comparisons']==2473
 assert proof['proven_infeasible_conditional_outcomes']==[['0x8c665d48','0x8c665d80']]
 coverage=b['coverage_by_routine']+a['coverage_by_routine'];assert len(coverage)==6
 for row in coverage:
  assert row['static_rows']==row['covered_rows'] and not row['missing_rows']
  assert row['missing_outcomes']==(proof['proven_infeasible_conditional_outcomes'] if row['address']=='0x8c665cc0' else [])
 assert sum(r['covered_rows'] for r in coverage)==896 and sum(r['covered_outcomes'] for r in coverage)==145 and sum(r['conditional_outcomes'] for r in coverage)==146
 bodies=[r for r in t['routines'] if r['address'] in NEW];assert len(bodies)==6 and sum(r['instruction_rows'] for r in bodies)==896
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in ('0x8c65253e','0x8c664b12','0x8c664b52','0x8c66563e'):
  old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);reuse.append(old);files.extend((P/old['source'],P/old['verification']))
 descriptor=reports['movie_graphics_descriptor_verification'];assert descriptor['cases_passed']==30 and descriptor['independent_checks']==330 and descriptor['executed_rows']==22 and descriptor['executed_conditional_outcomes']==5 and descriptor['qualified_conditional_outcomes']==6 and descriptor['dll_sha256']==dll
 rebuild=reports['movie_graphics_buffers_rebuild_verification'];assert rebuild['fresh_build_and_link_passed'] and rebuild['all_other_pe_bytes_equal'] and rebuild['published_dll_sha256']==dll and rebuild['compiled_source_count']==201
 limitations=[
  'Six original builders896instructions plus their22instruction descriptor provider664ac0 qualify918own instructions,150executed conditional outcomes and2proved infeasible outcomes. Header replication657240,list pointers6574b8,display sizing665cc0,pool allocation664352,cached buffers663320,paired texture allocation6634a0 and descriptor scanning664ac0 now qualify. Existing copy/list/address providers execute actual native code and receive zero duplicate credit. Seven larger graphics bodies remain partial with zero recovery credit.',
  '706fresh original/native comparisons and9731independent byte/pointer/arithmetic/list/size/register-shadow checks pass. Header masks/format overrides,count0..4,distinct/aliased RAM andVRAM destinations,FPU modes;pointer five size slots/count0..2,wrap/alignment/force variants;display six formats/default,video flags/custom wrapping height andtotals;bothpoolbanks,firstfit,exact/split,alllistpositions,descriptor exhaustion,unsupportedtypes preserving real partial mutations,signedsize quirks,cache reuse and address mapping are tested. Thirty descriptor controls cover first/second/later/last/none free flags,all98304poolbytes,ignoredarguments andthreeFPmodes.',
  'Actual pointer-builder ABI uses three caller stack words: fifth size atSP0,unusedwordSP4,countSP8. Checkedoriginalprologue/rootcall andunusedwordvariations support this layout;Ghidra draft six-argument signature is not authoritative. Larger counts,corrupt/aliased caller metadata,arbitrary concurrency andphysical alignment exceptions are outside qualified domains. No child return or allocation result is supplied.',
  'Display branch665d48->665d80 is infeasible under checkedimmutable code/fullentry/no asynchronous register modification:mov1,r5 dominates tst r5,r5,which clearsT. All113displayinstructions and17otheroutcomes execute. Descriptor initial branch664acc->664ae6 is infeasible because checked fixedbase8c8a83e4+18000 does not wrap and exceedsbase;later genuine exhaustion executes. Native source is unchanged;no result/branch is forced to manufacture coverage.',
  'This is proof-only recovery of already connected original bodies. All201native source/build bytes remain unchanged. Earlier2473comparisons,including two configured fullstartupcaptures,are retained andhashchecked,not rerun or counted as fresh. Combined evidence3179comparisons. Retained startup reaches608d88after656940;its3600post-clear tile writes andfullclear/Sound/IRQ/queue checks remain intact. Neither startup profile qualifies the remaining seven larger bodies.',
  'WholeCPU/FPU/RAM/VRAM/Sound/devices/orderedboundary states compare. Directfixtures are declared initial configuration/list/pool metadata,not guest checkpoints. Register shadows and allocation metadata do not establish GPU pixels,ARM/audio,hardware timing,resetboot,fullgame orplayable ports. PC60/120/240FPS8K,Android/stereoscopic3DS,N64/PS1,SUPIRHD anddisplaysettings remain pending;TextureOff disables textures andusesvertexcolors. Overall10%weighted milestones is not measured recovered-code coverage;goal stays active.'
 ]
 q=dict(routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in bodies]+[descriptor['routine']],canonical_reuse=reuse,newly_registered_entries=7,newly_qualified_rows=918,executed_conditional_outcomes=150,proven_infeasible_conditional_outcomes=2,qualified_conditional_outcomes=152,compiled_source_count=201,native_source_or_build_changed=False,fresh_original_native_comparisons=706,fresh_independent_checks=9731,retained_original_native_comparisons=2473,total_evidence_original_native_comparisons=3179,remaining_unqualified_larger_bodies=7,reused_additional_credit=0,dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_graphics_buffers_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified7graphics/descriptor builders918rows/150executed+2infeasible outcomes;706freshcomparisons')
if __name__=='__main__':main()
