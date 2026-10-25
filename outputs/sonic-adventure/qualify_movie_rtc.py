"""Qualify six original clock bodies with checked conditional-unreachability."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
SELECTED={'0x8c10be78','0x8c10c018','0x8c10bd96','0x8c10ba48','0x8c10be0c','0x8c10ba80'}
DEAD={
 '0x8c10be78':(['0x8c10be9c','0x8c10be9e','0x8c10bea0','0x8c10bea2','0x8c10bea4'],[['0x8c10be98','0x8c10be9c']]),
 '0x8c10bd96':(['0x8c10bdc2'],[['0x8c10bdc0','0x8c10bdc2'],['0x8c10bdd6','0x8c10bdf0'],['0x8c10bdec','0x8c10bddc']]),
 '0x8c10be0c':(['0x8c10be36','0x8c10be38'],[['0x8c10be2a','0x8c10be2e'],['0x8c10be30','0x8c10be20'],['0x8c10be34','0x8c10be36'],['0x8c10be46','0x8c10be5a']])}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_rtc_translation','movie_rtc_connection','movie_rtc_verification','movie_maple_qualification','movie_revision_translation')]
 translation,connection,direct,prior,parent=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,prior,parent):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert connection['compiled_source_count']==169 and translation['instruction_rows']==269
 assert direct['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_rtc.dll')
 assert direct['cases_passed']==91 and direct['independent_checks']==571 and direct['backend_checks']==43
 assert direct['status_counts']=={'0':78,'-3':3,'-4':7,'-2':3}
 rows=[r for r in direct['coverage_by_routine'] if r['address'] in SELECTED];assert len(rows)==6
 for r in rows:
  missing,edges=DEAD.get(r['address'],([],[]));assert r['missing_rows']==missing and r['missing_edges']==edges,r
 assert sum(r['static_rows'] for r in rows)==217 and sum(r['covered_rows'] for r in rows)==209
 assert sum(r['conditional_edges_covered'] for r in rows)==30 and sum(len(r['missing_edges']) for r in rows)==8
 raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();listing={}
 for report in (translation,parent):
  for routine in report['routines']:
   if routine['address'] in SELECTED|{'0x8c10d460'}:
    for row in csv.DictReader(Path(routine['listing']).open(),delimiter='\t'):
     a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes']);listing[a]=row['instruction'].lstrip('_')
 expected={
  0x8c10d586:'mov #0x0,r0',0x8c10d588:'add #0x8,r15',0x8c10d58a:'lds.l @r15+,PR',0x8c10d58c:'mov.l @r15+,r9',0x8c10d58e:'mov.l @r15+,r10',0x8c10d590:'mov.l @r15+,r11',0x8c10d592:'mov.l @r15+,r12',0x8c10d594:'mov.l @r15+,r13',0x8c10d596:'rts',0x8c10d598:'mov.l @r15+,r14',
  0x8c10be92:'jsr @r2',0x8c10be96:'tst r0,r0',0x8c10be98:'bt/s 0x8c10bea6',0x8c10be9a:'add #0x8,r15',0x8c10beca:'mov #0x0,r0',0x8c10bed0:'rts',
  0x8c10bdbe:'tst r0,r0',0x8c10bdc0:'bt 0x8c10bdc4',0x8c10bda6:'mov r15,r11',0x8c10bda8:'add #0x4,r11',0x8c10bdaa:'mov r11,r13',0x8c10bdb0:'mov.l r13,@r15',0x8c10bdcc:'mov r11,r14',0x8c10bdce:'mov.l @r15,r4',0x8c10bdd0:'add #0x4,r14',0x8c10bdd2:'cmp/hs r14,r4',0x8c10bdd6:'bt/s 0x8c10bdf0',0x8c10bde8:'add #0x4,r4',0x8c10bdea:'cmp/hs r14,r4',0x8c10bdec:'bf/s 0x8c10bddc',
  0x8c10be1a:'mov #0x0,r12',0x8c10be1c:'mov #0x1,r13',0x8c10be1e:'mov r12,r14',0x8c10be26:'mov r0,r4',0x8c10be28:'tst r4,r4',0x8c10be2a:'bf 0x8c10be2e',0x8c10be2c:'mov r13,r14',0x8c10be2e:'tst r14,r14',0x8c10be30:'bt 0x8c10be20',0x8c10be32:'tst r14,r14',0x8c10be34:'bf 0x8c10be3a',0x8c10be44:'tst r0,r0',0x8c10be46:'bf 0x8c10be5a'}
 for a,text in expected.items():assert listing[a]==text,(hex(a),listing[a])
 transfer_rows={int(r['address'],16):r for r in csv.DictReader(Path(next(r for r in translation['routines'] if r['address']=='0x8c10d460')['listing']).open(),delimiter='\t')}
 assert [a for a,r in transfer_rows.items() if r['instruction']=='rts']==[0x8c10d596]
 for a,r in transfer_rows.items():
  for destination in r['flows'].split():
   d=int(destination,16)
   if 0x8c10d588<=d<=0x8c10d598:assert 0x8c10d586<=a<=0x8c10d598
 literal=lambda a:int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')
 assert literal(0x8c10bfd0)==0x8c10d460 and literal(0x8c10bfcc)==0xa0710000
 assert literal(0x8c6000e0)==0x8c601618
 proof=dict(checked_original_instruction_rows=len(listing),critical_instructions=len(expected),unexecuted_rows=DEAD,
  assumptions=['Immutable original code/literals; no asynchronous RAM mutation.','Normal original entry/return ABI and valid mapped stack scratch; host faults stop rather than become guest return values.','Diagnostic output storage is disabled or separate from active frames, control globals and code; the direct tests declare those buffers explicitly.'],
  arguments=[
   'The synchronized transfer has one original RTS. All ordinary epilogue entries pass mov #0,r0 at 8c10d586; subsequent restores do not write R0. RTC read uses its pinned literal target. Host errors propagate out before the caller resumes, so the guest error-return arm at 8c10be9c cannot execute in this connected domain.',
   'The RTC reader likewise sets R0=0 at 8c10beca immediately before its normal epilogue. Its logging calls execute before that assignment. Therefore the coherence fallback store at 8c10bdc2 and the two stable-reader guest-error branch outcomes cannot execute.',
   'After a successful first read, stable-reader R14 becomes one and is preserved through the original callee ABI. Its initial retry outcome and zero-R14 fallback arm cannot execute. The two fallback rows remain in C; genuine unstable samples still exercise the separate equality/retry loop.',
   'In the two-sample coherence comparator, initial R4 is stack-scratch+4 and R14 is scratch+8. The initial unsigned >= outcome is false. After the sole equal comparison R4 advances by four and equals R14, so the backwards unsigned < outcome is false. Valid mapped scratch avoids 32-bit wrap. The unequal-sample path retains the supplied earlier timestamp.'])
 early=next(c for c in direct['cases'] if c['input']['entry']==0x8c010180)
 assert early['status']==-2 and early['stop_pc']=='0x8c600032' and early['rtc_snapshot']==[1,32,12] and early['clear_native']['writes']==0x200000
 assert any(b['target']=='0x8c64594c' and b['pr']=='0x8c10ba68' for b in early['boundaries'])
 routines=[]
 for report in (translation,parent):
  for r in report['routines']:
   if r['address'] in SELECTED:routines.append(dict(r,module='1ST_READ.BIN',source=report['source'],source_sha256=digest(P/report['source'])))
 assert len(routines)==6
 frontier=P/'analysis/movie-rtc-next-startup'
 files += [Path(__file__),P/'ghidra/export_movie_rtc_next.ps1',frontier/'diagnostics.tsv']+list(frontier.glob('*_instructions.tsv'))+list(frontier.glob('*_draft.c'))
 target=P/'analysis/movie-rtc-next-target';files += [P/'ghidra/export_movie_rtc_target.ps1',target/'diagnostics.tsv']+list(target.glob('*_instructions.tsv'))+list(target.glob('*_draft.c'))
 report=dict(routines=routines,newly_registered_entries=6,qualified_rows=217,executed_rows=209,proven_unreachable_rows=8,qualified_conditional_outcomes=38,executed_conditional_outcomes=30,proven_unreachable_conditional_outcomes=8,conditional_unreachability=proof,
  reused_existing_entries=['0x8c10d460'],reused_additional_credit=0,compiled_source_count=169,direct_comparisons=91,independent_checks=571,backend_checks=43,coverage_by_routine=direct['coverage_by_routine'],actual_next_dependency='0x8c600032',next_thunk_target='0x8c601618',next_task='Recover original initializer thunk 8c600032, its actual indirect-jump target 8c601618 and reached subsystem dependencies. The adjacent 8c600038 export is separate research, not that thunk target. Keep finite hardware input exhaustion explicit.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Six original RTC/diagnostic/consistency/stability/update/initialization bodies qualify within the declared non-aliasing software model. Of 217 rows and 38 branch outcomes, 209/30 execute and 8/8 are justified unreachable under explicit assumptions. Retained C includes those arms; the hardware transfer adapter earns zero extra credit.',
  limitations=[
   'Borrowed full-width raw words are finite explicit inputs. The original code selects their low 16 bits and performs the retries; the backend supplies no OS clock, calendar conversion, generated stable value, fake helper result or extrapolation.',
   'Both original RTC addresses are modeled for aligned 32-bit reads at P0/P1/P2 aliases. Unconfigured input stops -2, exhausted input stops -4, bad widths/null output stop -3 without consuming a sample. Writes are unsupported. This is not an electrical/timing specification or console measurement.',
   'The six qualified bodies retain their original integer branches and error arms. Conditional-unreachability depends on the stated connected callee ABI, immutable code, valid scratch and separate diagnostic storage; it does not cover arbitrary buffer corruption or asynchronous mutation.',
   'The synchronized transfer was already registered. Its new separate adapter permits RTC accesses without modifying the older canonical source/proof/build. This suite covers 101/148 adapter rows, not a second complete qualification; prior canonical evidence remains retained.',
   'The actual early caller executes original heap/device/controller producers, full eight-MiB clear, twelve supplied raw RTC reads and original clock callback registration before stopping at 8c600032. Earlier caller/driver remain partial. No startup service or callback return is substituted.',
   'Three retained controller constructor/packet profiles pass through the new build. Full accumulated-suite rerun, physical RTC/controller/DMA/IRQ behavior, complete natural boot, rendered pixels, gameplay and playable ports/settings remain unverified. Whole-project estimate remains ten percent.'])
 (P/'analysis/movie_rtc_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified six clock bodies: 217 rows / 38 outcomes, including 8 conditionally unreachable rows/outcomes')
if __name__=='__main__':main()
