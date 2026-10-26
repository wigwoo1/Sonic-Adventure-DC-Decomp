"""Qualify three complete original controls without crediting reused providers."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_midi_packet_translation','movie_midi_packet_connection','movie_midi_packet_verification','movie_midi_packet_retained_verification','movie_midi_packet_packet_regression_summary','movie_midi_packet_mode_regression_verification','movie_midi_packet_controls_regression_verification','movie_midi_packet_alignment_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_midi_packet_translation'];direct=reports['movie_midi_packet_verification'];retained=reports['movie_midi_packet_retained_verification'];packets=reports['movie_midi_packet_packet_regression_summary'];mode=reports['movie_midi_packet_mode_regression_verification'];controls=reports['movie_midi_packet_controls_regression_verification'];alignment=reports['movie_midi_packet_alignment_verification']
 assert t['instruction_rows']==162 and len(t['routines'])==1 and reports['movie_midi_packet_connection']['compiled_source_count']==199
 assert direct['cases_passed']==432 and direct['independent_checks']==9363
 assert all(r['static_rows']==r['covered_rows'] and r['conditional_outcomes']==r['covered_outcomes'] and not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
 assert retained['total_original_native_comparisons']==711 and packets['total_original_native_comparisons']==423 and packets['independent_checks']==2761
 assert mode['cases_passed']==177 and mode['independent_checks']==3450
 assert controls['cases_passed']==244 and controls['independent_checks']==5466
 assert alignment['abi_rows_executed']==147 and alignment['abi_conditional_outcomes_executed']==17 and len(alignment['impossible_abi_rows'])==15 and len(alignment['impossible_abi_edges'])==5
 dll=digest(ROOT/'work/sonic-adventure/build/movie_midi_packet.dll');assert all(r['dll_sha256']==dll for r in (direct,retained,packets,mode,controls))
 routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines']]
 assert sum(r['instruction_rows'] for r in routines)==162 and sum(len(r['conditional_edges']) for r in routines)==22
 rows=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in ('0x8c65a2fc','0x8c65acac'):
  old=next(r for r in rows if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  reuse.append({k:old[k] for k in ('address','name','source','verification','status')});files.extend((P/old['source'],P/old['verification']))
 limitations=[
  'One original stack-argument MIDI packing body qualifies162rows/22outcomes within the declared memory model and aligned-stack ABI. Original trace65a2fc/error65acac source/proofs retain canonical fields and receive zero duplicate credit.',
  '432fresh direct original/native cases/9363independent checks cover all256messagebytes,A0/C0singledata,F0zero,byte truncation/fieldpacking,source/stack aliases,mappedSound,enabled tracing and actual partial faults. No packed bytes or callee returns are injected.',
  '147rows/17outcomes execute under a four-aligned caller stack. Exact checked pointer/prologue arithmetic proves15alignment-alternative rows/5outcomes unreachable under that ABI.36additional experimental misaligned byte-backed RAM fixtures execute these alternatives. They do not establish physicalSH4unaligned loads/stores or address-error exceptions;that platform behavior remains unfinished. Original alternatives stay intact in native source.',
  '1555earlier mode/SDK/control/runtime/request/packet/CRC/error comparisons execute freshly in199-source integration. CompleteCPU/RAM/FPU/banked/special/device/Sound/runtime/flash/ordered-boundary state compares. Independent packet/ring/error/mask/callee-saved/Sound oracles supplement original decoding.',
  'A0/C0/F0 behavior is derived from original literals/instructions,not assumed from a MIDI standard. Header rereads and the saved source byte may change through aliases after earlier packet stores. Source/data/destination/stack faults preserve reached partial bytes. No helper result,ARMack,guestcheckpoint or rollback supplied.',
  'Completed full configured earlier-startup captures are separately qualified. Packet production and trace execution do not establish ARM consumption,audibleoutput,physicaltiming,reset/bootROM or gameplay.',
  'Full game-code/source publishing,PC60/120/240FPS8K,Android/stereoscopic3DS,N64/PlayStation adaptations,SUPIRHD/texturepercent/render/aspect remain unfinished. TextureOff must use vertexcolors.10% is weighted milestones,not measured code coverage. Settings64bddc/bootstrapac000010/erasedflash and exportedresource/download dependencies remain unresolved.'
 ]
 q=dict(routines=routines,canonical_reuse=reuse,newly_registered_entries=1,newly_qualified_rows=162,qualified_conditional_outcomes=22,executed_integration_rows=162,aligned_abi_executed_rows=147,aligned_abi_executed_outcomes=17,aligned_abi_infeasible_rows=15,aligned_abi_infeasible_outcomes=5,experimental_unaligned_cases=36,hardware_unaligned_exceptions_qualified=False,reused_additional_credit=0,shared_fragment_additional_credit=0,compiled_source_count=199,direct_original_native_comparisons=432,independent_direct_checks=9363,retained_cases_replayed_fresh=1555,total_original_native_comparisons=1987,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=limitations)
 (P/'analysis/movie_midi_packet_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified1original162rows22outcomes;1987freshoriginal/native;alignedABIplusinfeasibilityproof')
if __name__=='__main__':main()
