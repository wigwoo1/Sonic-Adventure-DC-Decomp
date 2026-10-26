"""Qualify two original producer bodies; preserve shared copy/support credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_audio_mode_translation','movie_audio_mode_connection','movie_audio_mode_verification','movie_audio_mode_retained_verification','movie_audio_mode_packet_regression_summary'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['movie_audio_mode_translation'];direct=reports['movie_audio_mode_verification'];retained=reports['movie_audio_mode_retained_verification'];packets=reports['movie_audio_mode_packet_regression_summary']
 assert t['instruction_rows']==131 and len(t['routines'])==3 and reports['movie_audio_mode_connection']['compiled_source_count']==197
 assert direct['cases_passed']==177 and direct['independent_checks']==3450
 assert all(r['static_rows']==r['covered_rows'] and r['conditional_outcomes']==r['covered_outcomes'] and not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
 assert retained['total_original_native_comparisons']==711 and packets['total_original_native_comparisons']==423 and packets['independent_checks']==2761
 dll=digest(ROOT/'work/sonic-adventure/build/movie_audio_mode.dll');assert all(r['dll_sha256']==dll for r in (direct,retained,packets))
 routines=[r|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines'] if r['kind']=='original_routine']
 fragments=[r for r in t['routines'] if r['kind']=='shared_copy_fragment']
 assert len(routines)==2 and sum(r['instruction_rows'] for r in routines)==120 and sum(len(r['conditional_edges']) for r in routines)==16
 assert len(fragments)==1 and fragments[0]['address']=='0x8c10cd82' and fragments[0]['instruction_rows']==11
 rows=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];reuse=[]
 for address in ('0x8c10cd44','0x8c65acac'):
  old=next(r for r in rows if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  reuse.append({k:old[k] for k in ('address','name','source','verification','status')});files.extend((P/old['source'],P/old['verification']))
 limitations=[
  'Two original routines qualify120own rows/16conditional outcomes:MONO/STRO command construction and SDK20-byte ring producer. The existing11-row copy tail is a shared overlapping fragment,not a newly recovered routine. Existing indexed dispatcher/error logger source/proof/name/status fields remain unchanged;zero duplicate credit.',
  '177fresh direct original/native cases/3450independentchecks compare whole machine/RAM/FPU/banked/special/bus/Sound/runtime/flash/boundary state. Actual command payloads,readywords,counts/tail wrap,count==256 rejection,above-limit and overflow count behavior,aliases,all16IMASKvalues,T/S and three FPU modes are checked. Real mappedSound output performs6writes/24bytes;RAM-only guard rejection was corrected to preserve actual bus writes.',
  'Source/queue/root/stack faults and unprivileged LDC retain reached partial state. A failed copy can leave the advanced tail/ready word/partial high payload words and IMASKf0 while count remains unchanged. Successful/error returns restore the input interrupt mask before actual diagnostic stores;no rollback or helper result is supplied.',
  '711earlier request/runtime/ABI comparisons and423packet/settings/CRC/compaction/failure comparisons execute freshly in the197-source integration. Canonical earlier proofs are preserved. Shared copy82 gets an explicit native route;reference observation suppresses its sequential fallthrough during larger existing copies,without skipping original instructions.',
  'These are explicit direct-unit and retained domains. Separate full earlier-startup captures must complete before they support startup claims;no prior full-copy result or ongoing run is substituted. Ring readiness is producer state,not ARM acknowledgement,command consumption,audible output,physical timing or playable game proof.',
  'Existing indexed dispatcher still has its previous input-domain guards;the new20-byte mappedSound-output path is tested,not every arbitrary copy size/source bus. Production code and previous native ABIs17/18/19/20 are retained. No new authored device results/default settings are added.',
  'Full-source/publicGitHub/documentation,PC60/120/240FPS/up to8K,Android/stereoscopic3DS,N64/PlayStation adaptations,SUPIRHDtoggle/texturepercent/render/aspect remain unfinished. TextureOff must use vertex colors. Overall10% is weighted milestones,not measured code coverage. Partial settings64bddc and bootstrapac000010 remain unresolved.'
 ]
 report=dict(routines=routines,shared_fragments=fragments,canonical_reuse=reuse,newly_registered_entries=2,newly_qualified_rows=120,qualified_conditional_outcomes=16,executed_integration_rows=131,shared_copy_fragment_rows=11,shared_fragment_additional_credit=0,reused_additional_credit=0,compiled_source_count=197,direct_original_native_comparisons=177,independent_direct_checks=3450,retained_cases_replayed_fresh=1134,total_original_native_comparisons=1311,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Original audio-mode/SDK command production with real20-byte copies and original ready/count/tail/mask/error state. No helper return or ARM acknowledgement. Two new bodies qualify;shared copy and prior canonical support receive zero extra credit. Startup/ARM/audio/gameplay proof is separate.',limitations=limitations)
 (P/'analysis/movie_audio_mode_qualification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Qualified2originals120rows16outcomes;1311freshoriginal/native;shared11rows0credit')
if __name__=='__main__':main()
