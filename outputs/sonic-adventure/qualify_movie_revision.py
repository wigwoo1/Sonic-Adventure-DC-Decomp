"""Qualify closed services; retain legacy driver and controller/RTC frontiers."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
SELECTED={'0x8c6024fa','0x8c6458fc','0x8c6461f0','0x8c65262c','0x8c651384'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_revision_translation','movie_revision_connection','movie_revision_verification','movie_revision_platform_evidence','movie_clear_qualification')]
 translation,connection,direct,platform,prior=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,platform,prior):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert connection['compiled_source_count']==166 and translation['instruction_rows']==497
 assert direct['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_revision.dll')
 assert direct['cases_passed']==50 and direct['independent_checks']==202 and direct['status_counts']=={'0':32,'-1':2,'-2':10,'-4':3,'-3':3}
 rows=direct['coverage_by_routine']
 for row in rows:
  if row['address'] in SELECTED|{'0x8c6461ae'}:assert not row['missing_rows'] and not row['missing_edges'],row
 assert sum(r['static_rows'] for r in rows if r['address'] in SELECTED)==211
 assert sum(r['conditional_edges_covered'] for r in rows if r['address'] in SELECTED)==8
 assert all(c['callback_word_before']==c['callback_word_after']=='0x8c000000' for c in direct['cases'])
 early=next(c for c in direct['cases'] if c['input']['entry']==0x8c010180 and c['input']['mode']==0)
 assert early['status']==-2 and early['stop_pc']=='0x8c65f558' and early['heap_root_at_stop']=='0x8c88ef1c' and early['context_root_at_stop']=='0x8c7ac7a0'
 assert early['clear_native']['writes']==0x200000
 low=[c for c in direct['cases'] if c['input']['entry']==0x8c604702 and c['input']['startup_overrides'][str(0x5f689c)]<10]
 assert len(low)==2 and all(c['status']==-2 and c['stop_pc']=='0x8c000000' for c in low)
 returned=[c for c in direct['cases'] if c['input']['entry']==0x8c604702 and c['status']==0];assert len(returned)==19
 mode=[c for c in direct['cases'] if c['input']['entry']==0x8c65262c];assert len(mode)==5 and all(c['status']==0 for c in mode)
 driver=next(r for r in rows if r['address']=='0x8c604702');assert driver['covered_rows']==246 and driver['static_rows']==255 and len(driver['missing_edges'])==3
 parent=P/'analysis/movie_clear_translation.json';old=json.loads(parent.read_text());files.append(parent)
 routines=[]
 for report in (translation,old):
  for r in report['routines']:
   if r['address'] in SELECTED:routines.append(dict(r,module='1ST_READ.BIN',source=report['source'],source_sha256=digest(P/report['source'])))
 assert len(routines)==5
 frontier=P/'analysis/movie-revision-maple-frontier'
 files += [Path(__file__),P/'ghidra/export_movie_revision_frontier.ps1',frontier/'diagnostics.tsv']+list(frontier.glob('*_instructions.tsv'))+list(frontier.glob('*_draft.c'))
 report=dict(routines=routines,newly_registered_entries=5,qualified_rows=211,qualified_conditional_outcomes=8,reused_existing_entries=['0x8c6461ae'],reused_additional_credit=0,driver_additional_credit=0,other_parent_additional_credit=0,
  compiled_source_count=166,direct_comparisons=50,independent_checks=202,coverage_by_routine=rows,supported_driver_returns=19,mode_selector_profiles=5,legacy_fallback_profiles=2,
  revision_profile=platform['reference_revision'],platform_source=platform['source_url'],callback_word_unchanged=True,actual_early_heap_root='0x8c88ef1c',actual_early_context_root='0x8c7ac7a0',actual_next_dependency='0x8c65f558',rtc_next_dependency='0x8c10be78',
  next_task='Recover original controller-context producer 8c65f558 and its actual lower services; then the RTC read helper 8c10be78. Keep the legacy callback and unsupported precision domains explicit.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Five closed original event/device/controller-registration/renderer mode/reset bodies qualify. An explicit system-bus revision profile lets the original driver return and earlier startup initialize its real device context. Driver, controller-buffer and RTC parents remain partial.',
  limitations=['Revision 11 is an explicit raw profile supported by the pinned Flycast source, not a measurement of the user console or a guarantee covering every hardware revision. Earlier platform defaults are unchanged.',
   'The original code compares the low system-bus revision byte with ten. Revision-zero/nine controls still reach the unchanged original callback value 8c000000. No callback code, target or return value is substituted; the legacy path remains unresolved.',
   'No WRITE reference to callback word 8c6734c4 occurs in this seeded Ghidra inventory. Computed or external writers are not ruled out. The explicit revision profile changes a host observation, not original code or the callback slot.',
   'The driver returns in 19 profiles but is not granted recovery credit: 246/255 rows and 27/30 conditional outcomes are covered, and the legacy callback remains unresolved. Renderer mode and reset own bodies are qualified separately.',
   'Controller parent 8c601a34 reaches all 79 rows but lacks one outcome and stops in its supplied-buffer path at real unconnected 8c65f558. Other controller/RTC parents also remain partial. Their fresh exports are research evidence, not qualified C.',
   'The actual precision-zero early caller creates heap and context through original code, clears all eight MiB and stops at 8c65f558. Precision-one controls stop with host -1 at reached unsupported floating-point operations. No blanket double-precision support is claimed.',
   'The existing table-copy body 8c6461ae has a new complete-state adapter and explicit source-record fixture but retains its canonical source/proof/name/status and receives no duplicate credit.',
   'Full-clear checking and explicit finite raw IRQ/FIFO/scan/clock inputs are retained. Software storage is not physical controller responses, DMA, interrupts, timing, rendered pixels, natural complete boot or playable ports. Whole-project weighted estimate remains ten percent.'])
 (P/'analysis/movie_revision_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified five original services: 211 rows / eight outcomes; supported-profile driver returns, legacy/controller/RTC parents remain partial')
if __name__=='__main__':main()
