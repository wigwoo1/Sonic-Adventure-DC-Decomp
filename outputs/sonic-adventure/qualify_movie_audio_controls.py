"""Qualify three complete original controls without crediting reused providers."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_audio_controls_translation','movie_audio_controls_connection','movie_audio_controls_verification','movie_audio_controls_retained_verification','movie_audio_controls_packet_regression_summary','movie_audio_controls_mode_regression_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_audio_controls_translation'];direct=reports['movie_audio_controls_verification'];retained=reports['movie_audio_controls_retained_verification'];packets=reports['movie_audio_controls_packet_regression_summary'];mode=reports['movie_audio_controls_mode_regression_verification']
 assert t['instruction_rows']==147 and len(t['routines'])==3 and reports['movie_audio_controls_connection']['compiled_source_count']==198
 assert direct['cases_passed']==244 and direct['independent_checks']==5466
 assert all(r['static_rows']==r['covered_rows'] and r['conditional_outcomes']==r['covered_outcomes'] and not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
 assert retained['total_original_native_comparisons']==711 and packets['total_original_native_comparisons']==423 and packets['independent_checks']==2761
 assert mode['cases_passed']==177 and mode['independent_checks']==3450
 dll=digest(ROOT/'work/sonic-adventure/build/movie_audio_controls.dll');assert all(r['dll_sha256']==dll for r in (direct,retained,packets,mode))
 routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines']]
 assert sum(r['instruction_rows'] for r in routines)==147 and sum(len(r['conditional_edges']) for r in routines)==26
 rows=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in ('0x8c65a464','0x8c10cd44','0x8c65acac'):
  old=next(r for r in rows if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  reuse.append({k:old[k] for k in ('address','name','source','verification','status')});files.extend((P/old['source'],P/old['verification']))
 limitations=[
  'Three complete original command controls qualify147rows/26outcomes. SDK sender/indexed copy/logger reuse receives zero duplicate credit. Resource/download exports643904/643970 remain unqualified dependencies.',
  '244fresh direct original/native cases/5466independent checks cover seven sequential source bytes including aliases and7mappedSoundreads; signed low-byte index/arguments,revision-dependent arithmetic shifts/bias wrapping; two genuine sequential commands and late queue-full rejection preserving the first command.',
  'WholeCPU/RAM/FPU/banked/special/device/Sound/runtime/flash/ordered-boundary state compares. Source/stack/queue faults and privilege failures preserve reached partial memory/tail/count/interrupt-mask state; no rollback,helper return,guestcheckpoint or ARMack is supplied.',
  '1311earlier mode/SDK/runtime/request/packet/CRC/error cases execute freshly in the198-source integration. Canonical original source/proofs and17/18/19/20native APIs remain intact. Completed configured earlier-startup captures are separately qualified.',
  'Command opcodes88/83/84/82 and observed data fields are identified; semantic audio labels beyond observed production are not asserted. Ringready/count are producer state,not ARM consumption,audible output,physical timing or playable-game proof.',
  'Full project remains unfinished: full decompilation/source publishing,PC60/120/240FPS/up to8K,Android/stereoscopic3DS,N64/PlayStation adaptations,SUPIRHDtoggle/texturepercent/render/aspect. TextureOff must use vertexcolors. Overall10% is weighted milestones,not measured code coverage;partial settings64bddc andbootstrapac000010 remain unresolved.'
 ]
 q=dict(routines=routines,canonical_reuse=reuse,newly_registered_entries=3,newly_qualified_rows=147,qualified_conditional_outcomes=26,executed_integration_rows=147,reused_additional_credit=0,shared_fragment_additional_credit=0,compiled_source_count=198,direct_original_native_comparisons=244,independent_direct_checks=5466,retained_cases_replayed_fresh=1311,total_original_native_comparisons=1555,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=limitations)
 (P/'analysis/movie_audio_controls_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified3originals147rows26outcomes;1555freshoriginal/native')
if __name__=='__main__':main()
