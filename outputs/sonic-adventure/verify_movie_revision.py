"""Explicit revision observations retain fallback controls and real startup calls."""
import collections,csv,ctypes,hashlib,json,sys,types
from pathlib import Path
import verify_movie_clear as old
P=old.P;ROOT=old.ROOT;driver=old.old
MP=P/'analysis/movie_revision_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(driver.ROWS);PER_BODY=dict(driver.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
exec(compile(old.code,'revision_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_revision.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
observed=effects.LIB.sa_movie_revision_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=old.Stats()
def invoke(*args):return observed(*args,ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
exec(compile(old.compare_code,'revision_comparison','exec'),dict(old._compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_compare=local['compare']
def prepare(ram,c):
 m,s,f=driver.prepare(ram,c)
 if c['entry']==0x8c6461ae:
  root=driver.load(m,0x8c6462b4);driver.store(m,root,0x8cda0000)
  start=0xda0028;m[start:start+24*0x2c]=bytes((i*7+3)&255 for i in range(24*0x2c))
 return m,s,f
def cases(smoke=False):
 base=dict(entry=driver.DRIVER,mode=0,render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},startup_overrides={0x5f689c:0xb},irq_samples=[0,8],fifo_samples=[0]*32,scan_samples=[0,1]*32)
 if smoke:
  selected=[{},dict(entry=0x8c010180,cold_library=True),dict(entry=0x8c6024fa),dict(entry=0x8c601a34,arguments=[0,0,0,0]),dict(entry=0x8c10ba48),dict(entry=0x8c6461ae)]
 else:
  selected=[dict(driver_flags=flags) for flags in (0,1,2,3,0x10,0x20,0x30,0x10000,0x10010,0x10020,0x10030)]
  selected += [dict(pixel_format=x) for x in (1,2,3,4,5)]
  selected += [dict(driver_flags=0x10000,mode=mode) for mode in (1,4)]
  selected += [dict(startup_overrides={0x5f689c:x}) for x in (0,9,10,0x1000b)]
  selected += [dict(entry=driver.MODE,display_mode=x,cold_library=True) for x in (0,1,2,3,0xffffffff)]
  selected += [dict(entry=driver.LIB_RESET)]
  selected += [dict(entry=0x8c010180,cold_library=True,mode=mode) for mode in (0,1)]
  selected += [dict(entry=a) for a in sorted(NEW)]
  selected += [dict(entry=0x8c6458fc,chain=[0x58,0x48]),dict(entry=0x8c601a34,arguments=[0,0,0,0]),dict(entry=0x8c601a34,arguments=[1,0x8cd80000,0x4000,0])]
  selected += [dict(entry=a,sr=0) for a in (0x8c6458fc,0x8c646304)]
  selected += [dict(entry=a,bad_stack=True) for a in (driver.DRIVER,0x8c6024fa,0x8c601a34)]
  selected += [dict(entry=driver.DRIVER,irq_samples=[]),dict(entry=driver.DRIVER,null_bus=True)]
 for c in selected:yield base|c
def semantic(m,a,before,s,r,c):
 entry=c['entry'];checks=0;revision=c['startup_overrides'][0x5f689c]
 if entry==driver.DRIVER:
  assert driver.load(a,0x8c6734c4)==driver.load(m,0x8c6734c4)==0x8c000000;checks+=2
  if not c.get('bad_stack') and not c.get('null_bus') and c['irq_samples']:
   if c['mode']==1 and c.get('driver_flags',0)&0x10000:assert r['status']==-1 and r['stop_pc']=='0x8c604812' and not r['clear_native']['calls'];checks+=3
   elif revision&255<10:assert r['status']==-2 and r['stop_pc']=='0x8c000000';checks+=2
   else:assert r['status']==0 and s.r[15]==before.r[15] and s.pr==before.pr,(c,r['status'],r['stop_pc']);checks+=3
 if entry==driver.MODE:
  assert r['status']==0 and s.r[0]==(0 if c['display_mode'] in (0,1,2) else 2);checks+=2
 if entry==0x8c6458fc and r['status']==0:
  start=driver.load(m,0x8c645a0c);extent=int.from_bytes(m[0x6459fe:0x645a00],'little',signed=True)
  assert all(driver.load(a,start+i+12)==0 for i in range(0,extent,16));checks+=1
  start=driver.load(m,0x8c645a10);extent=int.from_bytes(m[0x645a00:0x645a02],'little',signed=True)
  assert all(driver.load(a,start+i)==0 for i in range(0,extent,4)) and before.sr&0xf0==s.sr&0xf0;checks+=2
 if entry==0x8c6461ae and r['status']==0:
  source=0x8cda0028;dest=driver.load(m,0x8c6462b0)
  for i in range(24):
   source_offset=(source+i*0x2c)&0xffffff;dest_offset=(dest+i*24)&0xffffff
   assert a[dest_offset:dest_offset+8]==m[source_offset:source_offset+4]+m[source_offset+8:source_offset+12];checks+=1
   assert a[dest_offset+8:dest_offset+16]==m[source_offset+16:source_offset+24];checks+=1
   assert a[dest_offset+16:dest_offset+24]==m[dest_offset+16:dest_offset+24];checks+=1
 return checks
def main():
 ram=driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=_compare(m,s,f,c);r['independent_checks']=semantic(m,a,before,s,r,c);r['callback_word_before']=hex(driver.load(m,0x8c6734c4));r['callback_word_after']=hex(driver.load(a,0x8c6734c4));r['heap_root_at_stop']=hex(driver.load(a,0x8c88ef3c));r['context_root_at_stop']=hex(driver.load(a,0x8c5fbb14));records.append(r);coverage.update(path)
  if r['clear_reference']['calls']:
   coverage.update(pc for bit,pc in enumerate(list(range(old.LOOP,old.END,2))+list(range(old.COPY,0x8c65254e,2))) if r['clear_reference']['coverage']&(1<<bit))
  for j,pc in enumerate(path):
   t=ROWS.get(pc,'');distance=2 if t.startswith(('bt/s ','bf/s ')) else 1
   if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],r['clear_native']['calls'],flush=True)
 if '--smoke' in sys.argv:return
 rows=[];routines=list(manifest['routines'])+[r for r in driver.manifest['routines'] if int(r['address'],16) in (driver.DRIVER,driver.MODE,driver.LIB_RESET)]
 for routine in routines:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_revision_connection.json',P/'analysis/movie_clear_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=sum(r['independent_checks'] for r in records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Explicit system-bus revision inputs; original driver/callback word unchanged. Every reached game service executes original code or stops at its real missing boundary. Full-clear checking is inherited unchanged; ordinary CPU/RAM/FPU/peripheral and ordered-boundary comparisons remain.')
 (P/'analysis/movie_revision_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Revision comparisons',len(records),report['status_counts'],report['independent_checks']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
