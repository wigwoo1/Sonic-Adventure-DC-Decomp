"""Qualify original controller setup and packet construction, not physical I/O."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
REUSED={'0x8c65f44a','0x8c6607c8','0x8c6607be'}
PARENTS={'0x8c601a34','0x8c646304','0x8c65ebb0'}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_maple_translation','movie_maple_connection','movie_maple_verification','movie_revision_qualification','movie_revision_translation')]
 translation,connection,direct,prior,parent=json.loads(paths[0].read_text()),json.loads(paths[1].read_text()),json.loads(paths[2].read_text()),json.loads(paths[3].read_text()),json.loads(paths[4].read_text());files=list(paths)
 for proof in (translation,connection,direct,prior,parent):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert connection['compiled_source_count']==167 and translation['instruction_rows']==693
 assert direct['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_maple.dll')
 assert direct['cases_passed']==80 and direct['independent_checks']==748
 assert direct['status_counts']=={'0':67,'-3':10,'-2':2,'-4':1}
 rows=direct['coverage_by_routine'];assert len(rows)==14
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in rows)
 selected={r['address'] for r in translation['routines']}-REUSED|PARENTS
 assert len(selected)==11
 assert sum(r['static_rows'] for r in rows if r['address'] in selected)==915
 assert sum(r['conditional_edges_covered'] for r in rows if r['address'] in selected)==78
 early=next(c for c in direct['cases'] if c['input']['entry']==0x8c010180)
 assert early['status']==-2 and early['stop_pc']=='0x8c10be78' and early['controller_context_root']=='0x8c8a72b8'
 assert early['clear_native']['writes']==0x200000 and early['clear_reference']['writes']==0x200000
 legacy=next(c for c in direct['cases'] if c['input']['entry']==0x8c604702)
 assert legacy['status']==-2 and legacy['stop_pc']=='0x8c000000'
 routines=[]
 for report in (translation,parent):
  for r in report['routines']:
   if r['address'] in selected:routines.append(dict(r,module='1ST_READ.BIN',source=report['source'],source_sha256=digest(P/report['source'])))
 assert len(routines)==11
 files.append(Path(__file__))
 report=dict(routines=routines,newly_registered_entries=11,qualified_rows=915,qualified_conditional_outcomes=78,
  reused_existing_entries=sorted(REUSED),reused_additional_credit=0,compiled_source_count=167,direct_comparisons=80,independent_checks=748,coverage_by_routine=rows,
  actual_early_controller_context_root='0x8c8a72b8',actual_next_dependency='0x8c10be78',legacy_next_dependency='0x8c000000',
  next_task='Recover the original RTC read helper 8c10be78 and its reached lower services. Retain explicit finite raw inputs and unresolved legacy callback controls.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Eleven complete original controller initialization, context/record and packet/padding bodies qualify, including three previously partial parents. Three existing field/mask helpers retain their canonical evidence and receive no duplicate credit. All 915 rows and 78 conditional outcomes execute.',
  limitations=[
   'Full-state comparisons cover the declared profiles and software platform model, not every possible input or proof of physical hardware behavior. Explicit roots, buffers, counts and record flags in direct tests are preconditions.',
   'The actual early caller uses original producers: it creates heap/library/device/controller context, executes the full eight-MiB clear, and stops at unconnected RTC read 8c10be78. No constructor, callback or clock return is inserted.',
   'The parent uses R4 and R6 as its two buffer arguments. Both aligned and unaligned paths execute and independently checked results round upward to 32-byte boundaries before the original address alias is applied.',
   'Packet construction checks both word orders, two payloads, padding decisions, busy rejection, both buffer banks, last unit and serial wrap. It establishes software packet bytes, not a controller reply, DMA transfer or hardware interrupt.',
   'The forward byte-copy primitive uses R0/R1/R2, preserves R2/R3/R4 and propagates overlapping forward writes. It is not a memmove replacement. Invalid accesses retain the observed partial state and stop.',
   'Revision-eleven remains the earlier explicitly sourced raw profile; original zero-revision fallback still reaches unchanged callback 8c000000. No blanket precision-one or legacy-path completion is claimed.',
   'Three existing flag/field/mask adapters retain their original names, source and proof. Verification probes and decoders earn no game-code credit. Original source/proof/build bytes remain retained.',
   'Natural complete boot, physical input/timing, rendered pixels, gameplay, frame-rate/resolution/HD settings and all playable ports remain unfinished. Whole-project weighted estimate remains ten percent.'])
 (P/'analysis/movie_maple_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified eleven controller/context/packet bodies: 915 rows / 78 outcomes; three helpers reused without extra credit')
if __name__=='__main__':main()
