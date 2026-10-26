"""Qualify the MIDI sender and connect an existing handoff without duplicate credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 names=('translation','connection','verification','retained_verification','packet_regression_summary','mode_regression_verification','controls_regression_verification','packing_regression_verification')
 for suffix in names:
  name='movie_midi_commands_'+suffix;path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[suffix]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['translation'];d=reports['verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_midi_commands.dll')
 assert t['instruction_rows']==54 and len(t['routines'])==2 and reports['connection']['compiled_source_count']==200
 assert d['cases_passed']==258 and d['independent_checks']==5262
 assert all(r['static_rows']==r['covered_rows'] and r['conditional_outcomes']==r['covered_outcomes'] and not r['missing_rows'] and not r['missing_outcomes'] for r in d['coverage_by_routine'])
 expected=(('retained_verification','total_original_native_comparisons',711,None),('packet_regression_summary','total_original_native_comparisons',423,2761),('mode_regression_verification','cases_passed',177,3450),('controls_regression_verification','cases_passed',244,5466),('packing_regression_verification','cases_passed',432,9363))
 for name,key,count,checks in expected:
  r=reports[name];assert r[key]==count and r['dll_sha256']==dll
  if checks is not None:assert r['independent_checks']==checks
 assert d['dll_sha256']==dll
 packing=reports['packing_regression_verification'];assert packing['aligned_stack_covered_rows']==147 and packing['experimental_unaligned_stack_cases']==36
 body=next(r for r in t['routines'] if r['address']=='0x8c643b9c');adapter=next(r for r in t['routines'] if r['address']=='0x8c608c14')
 assert body['instruction_rows']==43 and len(body['conditional_edges'])==6 and adapter['instruction_rows']==11 and not adapter['conditional_edges']
 rows=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in ('0x8c608c14','0x8c65a464','0x8c10cd44','0x8c65acac'):
  old=next(r for r in rows if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  reuse.append({k:old[k] for k in ('address','name','source','verification','status')});files.extend((P/old['source'],P/old['verification']))
  if address=='0x8c608c14':assert old['retail_span_sha256']==adapter['retail_span_sha256']
 limitations=[
  'One original MIDI sender qualifies43new ownrows/6outcomes. The11-row masked global-pair setter608c14 is already canonical:its new complete-state startup adapter receives zero additional recovery credit. Existing setter/SDK/indexedcopy/error canonical fields/source/proofs remain unchanged.',
  '258fresh direct comparisons/5262independent checks cover actual command86/header/sparse packet stores6..9,aliases/forward overlap,full/wrap/overflow queues,ignored nonzero object pointer values,allIMASKs,T/S,threeFPmodes,Sound memory and partialsource/root/queue/stack/privilege failures. Handoff pointer/value stores,size&fffff800,Tclear and fullRAM/register/FPU invariants compare;it records metadata,not allocation.',
  '1987earlier mode/control/packetpacking/flash/runtime/request/ABI comparisons execute freshly in200-source integration. WholeCPU/RAM/FPU/banked/special/device/Sound/runtime/flash/ordered-boundary state compares. Independent byte/pipelinedcopy/ring/error/mask/Sound oracles supplement instruction decoding. No command bytes/helper returns/checkpoint/ARMack are supplied.',
  'The retained MIDI packing suite explicitly separates aligned ABI from36experimental byte-backed unaligned-stack cases;physicalSH4unaligned/address-error behavior remains unqualified. Earlier mathematical infeasibility proof and canonical source/proofs remain unchanged.',
  'Fresh configured0546c0startup comparisons are separate evidence. Warm startup now enqueues32actual MIDI commands through original packing/sender/SDK/copy code and connects the existing buffer metadata helper,before larger656940. Queue/producer state is not ARM processing,audibleoutput,physicaltiming,resetboot or gameplay proof.',
  'Full game-code/source publishing,PC60/120/240FPS8K,Android/stereoscopic3DS,N64/PlayStation adaptations,SUPIRHD/texturepercent/render/aspect remain unfinished. TextureOff disables textures and uses vertexcolors.10% is weighted milestones,not measured code coverage. Settings/bootstrap/erasedflash/resource/download and largerinitializer dependencies remain unfinished.'
 ]
 r=dict(routines=[body|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source']))],connected_existing_adapters=[adapter],canonical_reuse=reuse,newly_registered_entries=1,newly_qualified_rows=43,qualified_conditional_outcomes=6,executed_integration_rows=54,reused_adapter_rows=11,reused_additional_credit=0,shared_fragment_additional_credit=0,compiled_source_count=200,direct_original_native_comparisons=258,independent_direct_checks=5262,retained_cases_replayed_fresh=1987,total_original_native_comparisons=2245,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope=d['scope'],limitations=limitations)
 (P/'analysis/movie_midi_commands_qualification.json').write_text(json.dumps(r,indent=2)+'\n');print('Qualified1newMIDI sender43rows6outcomes;11existingadapterrows0credit;2245freshcomparisons')
if __name__=='__main__':main()
