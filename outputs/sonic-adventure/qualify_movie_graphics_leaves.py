"""Qualify four complete leaves; keep the larger graphics family explicitly partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
NEW=('0x8c656f50','0x8c657096','0x8c6573c4','0x8c66563e')
REUSED=('0x8c6578e0','0x8c657960','0x8c657968','0x8c664b12','0x8c664b52')
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_graphics_init_translation','movie_graphics_init_connection','movie_graphics_leaf_configuration_verification','movie_graphics_init_all_retained_verification','movie_graphics_init_startup_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_graphics_init_translation'];d=reports['movie_graphics_leaf_configuration_verification'];ret=reports['movie_graphics_init_all_retained_verification'];start=reports['movie_graphics_init_startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_graphics_init.dll')
 assert len(t['routines'])==22 and t['instruction_rows']==2956 and reports['movie_graphics_init_connection']['compiled_source_count']==201
 assert d['cases_passed']==226 and d['independent_checks']==2433 and ret['total_original_native_comparisons']==2245
 assert start['cases_passed']==2 and start['completed_profiles']==['warm','cold'] and start['actual_next_frontier']=='0x8c608d88'
 assert d['dll_sha256']==ret['dll_sha256']==start['dll_sha256']==dll
 assert all(not r['missing_rows'] and not r['missing_outcomes'] and r['static_rows']==r['covered_rows'] and r['conditional_outcomes']==r['covered_outcomes'] for r in d['coverage_by_routine'])
 bodies=[r for r in t['routines'] if r['address'] in NEW];assert len(bodies)==4 and sum(r['instruction_rows'] for r in bodies)==352 and sum(len(r['conditional_edges']) for r in bodies)==74
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in REUSED:
  old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);reuse.append(old)
  adapter=next(r for r in t['routines'] if r['address']==address);assert old['retail_span_sha256']==adapter['retail_span_sha256']
  files.extend((P/old['source'],P/old['verification']))
 limitations=[
  'Only four complete leaves qualify: list enable mask656f50, packed list control657096, list space sizing6573c4 and VRAM address mapping66563e. All352own instructions/74conditional outcomes execute. Five already canonical global/list adapters have55executed rows and receive zero duplicate credit; original canonical records stay intact.',
  '226fresh direct comparisons/2433independent checks cover bitfields, force/format variants, compactness/order selection, wrapping arithmetic, size multipliers, mapped address boundaries, global stores and linked-list effects. WholeCPU/FPU/RAM/VRAM/Sound/devices and ordered boundaries compare. Tests use declared configuration-data fixtures and valid mapped aligned caller storage; physical unaligned/address-error behavior and arbitrary concurrency are not established.',
  'All2245previous direct/retained transfer/runtime/request/flash/audio/packing/MIDI cases replay freshly in201-source integration. Old17/18/19/20argument native APIs and actual data/IRQ/RTE completion paths are retained; no completion flags, child results, ARM acknowledgements or guest checkpoints are injected.',
  'Two fresh uninterrupted configured0546c0startup comparisons execute the larger graphics initializer and reach608d88 after its normal return. Cold retains the full8MiB video clear, two2MiB Sound fills, two33376-byte transfers with real interrupt/return paths and1065282Sound events without drops. All39queue records and old200-source boundary prefixes compare. Explicit raw flash and platform observations remain declared; this is not reset boot or hardware timing proof.',
  'The remaining13new larger bodies are prototypes with incomplete per-body branch/ABI coverage and get zero recovery credit. Startup success for a declared profile does not qualify these callers. The optional16MiB setting appears structurally unreachable through the original constant-return option selector; that inference is not yet a qualified infeasibility proof. Large unsigned-size exploration retains genuine address failures rather than forcing success.',
  'Game rendering/pixels, ARM/audio output, broad asynchronous/hardware behavior, settings/erasedflash/resource/download/bootstrap and the full game/ports remain unfinished. PC60/120/240FPS8K, Android/stereoscopic3DS, N64/PlayStation adaptations, SUPIRHD/texturepercent/render/aspect settings remain pending. TextureOff disables textures and uses vertexcolors. Overall10% is weighted milestones,not measured recovered-code coverage;goal stays active.'
 ]
 q=dict(routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in bodies],canonical_reuse=reuse,newly_registered_entries=4,newly_qualified_rows=352,qualified_conditional_outcomes=74,reused_adapter_rows=55,reused_additional_credit=0,unqualified_new_larger_bodies=13,compiled_source_count=201,direct_original_native_comparisons=226,independent_direct_checks=2433,retained_cases_replayed_fresh=2245,completed_startup_comparisons=2,total_original_native_comparisons=2473,dll_sha256=dll,scope=d['scope'],limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_graphics_leaves_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified four graphics leaves:352rows/74outcomes,2473fresh comparisons;larger callers remain partial')
if __name__=='__main__':main()
