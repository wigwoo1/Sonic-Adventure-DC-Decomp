"""Qualify only closed original driver dependencies, preserving partial parents."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
SELECTED={'0x8c6045b4','0x8c6510a4','0x8c641160','0x8c66120c','0x8c665a00','0x8c666706','0x8c653640','0x8c663080','0x8c650aa0','0x8c665e00'}
REUSED={'0x8c6045a8','0x8c64241a','0x8c653440','0x8c6666c0','0x8c651520','0x8c664aec','0x8c665440'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_driver_translation','movie_driver_connection','movie_driver_verification','driver_signal_bus_verification','movie_driver_prefix_verification','movie_startup_qualification')]
 translation,connection,direct,bus,prefix,prior=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,bus,prefix,prior):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert direct['dll_sha256']==bus['dll_sha256']==prefix['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_driver.dll')
 assert connection['compiled_source_count']==164 and translation['instruction_rows']==1808
 assert direct['cases_passed']==90 and direct['independent_checks']==337 and direct['status_counts']=={'0':49,'-2':5,'-3':5,'-4':31}
 assert bus['checks_passed']==98 and bus['samples_borrowed_unchanged'] and bus['aliases']==3
 assert prefix['cases_passed']==6 and prefix['independent_checks']==120 and prefix['status_counts']=={'-3':6}
 assert prefix['next_dependencies']==['0x8c10d4bc','0x8c6021fe']
 assert all(c['heap_root_at_stop']=='0x0' for c in prefix['cases'])
 rows=direct['coverage_by_routine']
 for row in rows:
  if row['address'] in SELECTED|REUSED:assert not row['missing_rows'] and not row['missing_edges'],row
 assert sum(r['static_rows'] for r in rows if r['address'] in SELECTED)==934
 assert sum(r['conditional_edges_covered'] for r in rows if r['address'] in SELECTED)==50
 driver=next(r for r in rows if r['address']=='0x8c604702');assert driver['covered_rows']==233 and driver['static_rows']==255
 callers=[c for c in direct['cases'] if c['input']['entry']==0x8c010180];assert len(callers)==2
 for c in callers:assert c['stop_pc']=='0x8c665998' and c['heap_root_at_stop']=='0x8c88ef1c' and c['library_root_at_stop']=='0x8c8a2d54'
 unknown=[c for c in direct['cases'] if c['stop_pc']=='0x8c000000'];assert len(unknown)==1 and unknown[0]['status']==-2
 partial=[r for r in rows if r['address'] not in SELECTED|REUSED];assert len(partial)==6
 routines=[dict(r,module='1ST_READ.BIN',source=translation['source'],source_sha256=digest(P/translation['source'])) for r in translation['routines'] if r['address'] in SELECTED]
 assert len(routines)==10
 files.append(Path(__file__))
 report=dict(routines=routines,coverage_by_routine=rows,newly_registered_entries=10,qualified_rows=934,qualified_conditional_outcomes=50,reused_existing_entries=sorted(REUSED),reused_additional_credit=0,
  partial_bodies=partial,partial_additional_credit=0,compiled_source_count=164,direct_comparisons=90,independent_direct_checks=337,platform_api_checks=98,
  retained_late_boot_profiles=6,retained_late_boot_checks=120,actual_early_caller_profiles=2,actual_early_heap_root='0x8c88ef1c',actual_early_library_root='0x8c8a2d54',actual_early_research_stop='0x8c665998',unresolved_callback_target='0x8c000000',
  next_task='Verify the original 2,097,152-call VRAM clear efficiently without changing its instructions or bypassing its helper; then close reset/mode/driver paths and resolve the actual callback producer.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Ten complete original synchronization/library/default-table/frame-layout/setup helpers qualify. Seven canonical bodies gain new complete-state adapters without duplicate credit. Six additional bodies, including driver, renderer reset and pool allocator, remain partial.',
  limitations=['Finite raw IRQ-status, FIFO, scan and clock observations plus raw register storage are explicit software platform inputs. No hardware completion, physical IRQ delivery, timer progression, DMA execution, display or pixels are established.',
   'DMA operation register ffa00040 is software storage only, with new trace events 29/30. Status stream reads preserve old W1C acknowledgement and stop with -4 when exhausted; input arrays remain borrowed and unchanged.',
   'With SA_BOUND_RETAIL_STALLS, self-loop 8c665968 stops on its second visit, and the original finite 2,097,152-call VRAM clear stops before its second continued loop test at 8c665998. No completion credit is assigned to 8c6656e2 or its parents. Without the macro both original loops remain.',
   'The original early caller initializes heap and library through real code but remains partial at the bounded clear. Earlier runtime/VBR fixtures are still explicit; cold library tests restore its zero root. This is not natural full boot.',
   'Driver input R4 contains mode/size flags, R5 selects the pixel format, R6 is the additional configuration word and R7 is the extra input. An invalid display-mode profile follows its real error return and later stops at unresolved callback 8c000000; no callback result is supplied.',
   'Pool allocator 8c664ac0 reaches all 22 rows but lacks one branch outcome; it is deliberately uncredited here. Memory-manager constructor 8c663140 also remains partial. Canonical earlier source/proof/status fields and builds are preserved.',
   'The six retained late-boot controls remain separate from unfinished early startup and keep their genuine zero-heap/context faults. Full-game decompilation, playable ports and requested display/texture settings remain unfinished.'])
 (P/'analysis/movie_driver_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified ten original dependencies: 934 rows / 50 outcomes; seven reused and six partial bodies receive no extra credit')
if __name__=='__main__':main()
