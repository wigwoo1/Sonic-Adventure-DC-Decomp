"""Qualify closed helpers; keep disc/path/initializer parents explicitly partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
SELECTED={'0x8c601618','0x8c5ffc98','0x8c5ff9e6','0x8c5ff9c0','0x8c60041a','0x8c64530a','0x8c60167e'}
REUSED={'0x8c600032','0x8c600dd0','0x8c5ffcfe','0x8c64535c','0x8c645468','0x8c600ddc','0x8c10d974'}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_disc_init_translation','movie_disc_init_connection','movie_disc_init_verification','movie_disc_init_sector_verification','movie_rtc_qualification')]
 translation,connection,direct,sector,prior=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,sector,prior):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert translation['instruction_rows']==951 and len(translation['routines'])==17 and connection['compiled_source_count']==170
 assert direct['cases_passed']==82 and direct['independent_checks']==243 and direct['version_backend_checks']==34
 assert direct['status_counts']=={'0':78,'-3':4}
 assert sector['cases_passed']==3 and sector['independent_checks']==16 and sector['dll_sha256']==direct['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_disc_init.dll')
 rows=[]
 for row in direct['coverage_by_routine']:
  updated=dict(row)
  if row['address']=='0x8c5ffc98':
   routine=next(r for r in translation['routines'] if r['address']==row['address']);body=set(routine['reachable_addresses']);covered=body-set(row['missing_rows'])|set(sector['covered_addresses'])
   missing=set(tuple(x) for x in row['missing_edges'])-set(tuple(x) for x in sector['covered_edges'])
   updated.update(covered_rows=len(covered),missing_rows=sorted(body-covered),conditional_edges_covered=len(routine['conditional_edges'])-len(missing),missing_edges=[list(x) for x in sorted(missing)])
  rows.append(updated)
 for row in rows:
  if row['address'] in SELECTED|REUSED:assert not row['missing_rows'] and not row['missing_edges'],row
 assert sum(r['static_rows'] for r in rows if r['address'] in SELECTED)==144
 assert sum(r['conditional_edges_covered'] for r in rows if r['address'] in SELECTED)==10
 parents=[r for r in rows if r['address'] in {'0x8c600038','0x8c6454fe','0x8c6002a2'}]
 assert [(r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in parents]==[(409,479,30),(92,99,9),(69,113,15)]
 warm,cold=[c for c in direct['cases'] if c['input']['entry']==0x8c010180]
 assert warm['filesystem_root_before']=='0x8cc80000' and warm['filesystem_initialized_flag']==1 and not warm['disc_read_attempts']
 assert cold['input']['cold_filesystem'] and cold['status']==0 and cold['guest_r0']=='0x0' and cold['stop_pc']=='0x8cfed000'
 assert cold['filesystem_root_before']=='0x0' and cold['filesystem_root_after']=='0x8c6946e0' and cold['filesystem_initialized_flag']==1
 assert cold['disc_read_attempts']==72 and cold['boundary_comparisons']==56648 and cold['steps']==1345913 and cold['clear_native']['writes']==0x200000
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in direct['cases']+sector['cases'])
 routines=[dict(r,module='1ST_READ.BIN',source=translation['source'],source_sha256=digest(P/translation['source'])) for r in translation['routines'] if r['address'] in SELECTED];assert len(routines)==7
 files += [Path(__file__),P/'disc_init_platform_reference.py',P/'boot_disc_reference.py',P/'source/native_disc_queue.c',P/'ghidra/export_movie_disc_init_clock_refs.ps1',P/'analysis/movie-disc-init-clock-references/references.tsv']
 report=dict(routines=routines,newly_registered_entries=7,qualified_rows=144,qualified_conditional_outcomes=10,reused_existing_entries=sorted(REUSED),reused_additional_credit=0,compiled_source_count=170,direct_comparisons=85,independent_checks=259,version_backend_checks=34,coverage_by_routine=rows,partial_parent_additional_credit=0,
  cold_early_return=True,cold_filesystem_root='0x8c6946e0',cold_filesystem_reads=72,cold_boundary_comparisons=56648,cold_original_steps_excluding_clear_decoder=1345913,full_clear_writes=0x200000,
  earlier_warm_filesystem_fixture='0x8cc80000',reference_model_correction='New versioned model copies exactly sixteen native identity bytes; older model/source/proof bytes remain retained. No native firmware/transport implementation changed.',
  next_task='Qualify remaining initializer, directory-loader and path-resolution branches using original code and explicit media/failure inputs. Then join cold initialization to its original caller and the existing MOVIE boot flow; do not claim complete reset-to-gameplay from this returning initializer.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Seven closed wrapper, sector, signature, endian, lookup and version bodies qualify: every own row and outcome executes across main/descriptor comparisons. Seven canonical helpers are reused without extra credit. The original cold earlier caller initializes its filesystem from real media and returns; three larger parents remain partial.',
  limitations=[
   'The direct profiles declare their own contexts, descriptors, strings, directory records, buffers and software hardware observations. Actual cold initialization clears only the two filesystem/interface roots before executing original producers; no filesystem header, directory result, callback or success value is inserted.',
   'The earlier common RAM seed contains a prepared filesystem at 8cc80000. Its warm profile reads no media and returns guest -8; it is retained separately, not presented as fresh filesystem initialization. Cold filesystem/interface roots are explicitly zero, matching their original word values.',
   'The cold earlier caller creates filesystem root 8c6946e0, performs 72 real track-sector reads, sets its original initialized flag and returns guest zero after 1345913 ordinary original steps plus the retained full-clear decoder. All 56648 ordinary service boundaries and final complete state match. These are explicit profiles, not physical Dreamcast timing or complete natural reset-to-main ordering.',
   'Direct small-capacity contexts legitimately return guest -12 with two reads; misaligned contexts return -14, injected transport failures return -4 and already-initialized contexts return zero. Host zero means the original routine returned normally, not that its guest operation succeeded.',
   'The older Python disc model included NUL when assigning a sixteen-byte response slice, growing simulated RAM by one byte. The versioned model fixes that path to match the unchanged C sixteen-byte copy. Three aliases, guards, queue/status state and a historical failure witness pass 34 focused checks. The response remains documented native host identity, not copied Dreamcast firmware.',
   'Initializer 8c600038 covers 409/479 rows and 28/58 conditional outcomes. Directory loader 8c6454fe covers 92/99 and 21/30; path helper 8c6002a2 covers 69/113 and 7/22. These parents receive no completion credit, and missing outcomes are not labeled unreachable without proof.',
   'Existing thunk/record/store/byte-copy/directory-parser/zero-call/firmware-init wrappers retain canonical source/proof/name/status and earn zero duplicate credit. The descriptor-free control retains the prior finite wait stop at 8c600f7e.',
   'Full accumulated-suite rerun, physical disc/controller/IRQ/DMA behavior, rendered pixels, complete boot/gameplay, frame-rate/display/texture settings and all playable ports remain unfinished. Whole-project weighted estimate remains ten percent.'])
 (P/'analysis/movie_disc_init_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified seven helpers: 144 rows / 10 outcomes; cold earlier initialization returns after 72 real sector reads; three parents remain partial')
if __name__=='__main__':main()
