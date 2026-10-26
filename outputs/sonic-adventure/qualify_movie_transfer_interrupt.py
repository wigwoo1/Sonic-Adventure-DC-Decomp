"""Qualify the original installed transfer callback in the tested software domain."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_transfer_interrupt_translation','movie_transfer_interrupt_connection','movie_transfer_interrupt_verification','movie_transfer_interrupt_lifecycle_verification','movie_transfer_interrupt_regression_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 t=reports['movie_transfer_interrupt_translation'];connection=reports['movie_transfer_interrupt_connection'];direct=reports['movie_transfer_interrupt_verification'];life=reports['movie_transfer_interrupt_lifecycle_verification'];reg=reports['movie_transfer_interrupt_regression_verification']
 assert t['instruction_rows']==direct['instruction_rows']==direct['covered_rows']==39
 assert direct['conditional_outcomes']==direct['covered_outcomes']==2 and direct['cases_passed']==110 and direct['independent_checks']==1297
 assert connection['compiled_source_count']==188 and life['lifecycles_passed']==12 and life['original_native_comparisons']==24 and life['independent_checks']==324
 assert reg['unchanged_prior_comparisons']==639 and reg['retained_wait_comparisons']==2 and reg['total_comparisons']==641
 dll=digest(ROOT/'work/sonic-adventure/build/movie_transfer_interrupt.dll');assert all(r['dll_sha256']==dll for r in (direct,life,reg))
 raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 def word(a,n=4):return int.from_bytes(raw[a-0x8c010000:a-0x8c010000+n],'little')
 assert word(0x8c10d764)==0x8c10ebe8 and word(0x8c10d768)==0x8c64594c
 assert word(0x8c10ec38)==0x8c5fbb14 and word(0x8c10ec3c)==0x8c5fbb0c and word(0x8c5fbb0c)==0xa05f6900 and word(0x8c10ec36,2)==0xec
 listing=P/'analysis/movie-hardware-initializer/8c10d5a8_instructions.tsv';rows={int(r['address'],16):r for r in csv.DictReader(listing.open(),delimiter='\t')};files.append(listing)
 for a,row in rows.items():assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
 assert rows[0x8c10d6ac]['instruction']=='mov.l 0x8c10d764,r11'
 sites=[0x8c10d6b6,0x8c10d6d0,0x8c10d6ea,0x8c10d708]
 for a in sites:assert rows[a]['instruction']=='jsr @r12' and rows[a+2]['instruction']=='_mov r11,r5'
 canonical=[];registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
 for address in ('0x8c668f40','0x8c65a518','0x8c666e06','0x8c64594c'):
  row=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);canonical.append({k:row[k] for k in ('address','name','source','verification')});files.extend((P/row['source'],P/row['verification']))
 routine=t['routines'][0]|dict(module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source']))
 limitations=['Qualified own handler assumes immutable code/literals, initialized RAM descriptor/root/table/stack and the existing explicit software bus. All39rows and both conditional outcomes execute; arbitrary uninitialized game state is not claimed. Unsupported callback and invalid-pointer stops retain actual preceding writes and delay-slot effects.',
 'The existing normal-status acknowledge bus clears written bits. No new physical hardware model, DMA data movement, event scheduling, CPU exception entry, timing, arbitration or interrupt acceptance is supplied. Two-call lifecycles explicitly invoke the handler ABI after channel-start returns normally; no stopped-guest checkpoint is resumed.',
 'The original handler and original SDK callback set state/completion fields. Existing callback/counter/event registration bodies receive zero duplicate credit. The default mode-one REQUEST still waits at65a6ec without external delivery. Earlier full memory-copy, video-clear and earliest-caller proofs are preserved historically, not rerun in this milestone.',
 'Complete startup, ARM execution/audio, playable game, all native ports/demakes, 60/120/240FPS,8K,SUPIR/display/texture settings remain unfinished. TextureOff must use vertex colors. Whole-project10% is a weighted milestone estimate, not recovered-code coverage.']
 report=dict(routines=[routine],newly_registered_entries=1,qualified_rows=39,executed_rows=39,qualified_conditional_outcomes=2,executed_conditional_outcomes=2,compiled_source_count=188,direct_comparisons=110,independent_direct_checks=1297,lifecycle_comparisons=24,independent_lifecycle_checks=324,regression_comparisons=641,regression_checks=reg['independent_and_record_checks'],total_comparisons=775,canonical_reuse=canonical,reused_additional_credit=0,installed_handler_literal='0x8c10d764',original_registration_call_sites=[hex(a) for a in sites],initializer_rows_byte_checked=len(rows),retained_default_frontier='0x8c65a6ec',dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Complete original39-row installed channel interrupt handler, explicit direct and two-call ABI integration with actual original SDK callback and existing unchanged acknowledge policy. No fabricated helper return/completion or physical event supplied.',limitations=limitations)
 (P/'analysis/movie_transfer_interrupt_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified transfer interrupt:39rows/2outcomes;775comparisons;1newentry')
if __name__=='__main__':main()
