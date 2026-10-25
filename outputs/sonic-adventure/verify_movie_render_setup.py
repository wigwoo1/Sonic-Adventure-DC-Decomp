"""Retain original-instruction comparison while connecting renderer setup."""
import collections,csv,ctypes,hashlib,itertools,json,types
from pathlib import Path
import verify_movie_render_config as old
P=old.P;ROOT=old.ROOT;previous=old.previous
MP=P/'analysis/movie_render_setup_translation.json';manifest=json.loads(MP.read_text())
NEW={int(r['address'],16) for r in manifest['routines']};ROWS=dict(old.ROWS);PER_BODY={}
reused=[r for r in json.loads((P/'analysis/boot_event_translation.json').read_text())['routines'] if r['address'] in ('0x8c6412c2','0x8c6411b0')]
assert len(reused)==2
EVIDENCE_ROUTINES=manifest['routines']+reused
for routine in EVIDENCE_ROUTINES:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
exec(compile(old.code,'renderer_setup_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_render_setup.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
native_fn=effects.LIB.sa_movie_render_setup_connected;native_fn.argtypes=old.native_fn.argtypes;native_fn.restype=ctypes.c_int
initialized_fn=effects.LIB.sa_movie_render_setup_initialized_connected;initialized_fn.argtypes=native_fn.argtypes;initialized_fn.restype=ctypes.c_int;effects.sequence=False
def invoke(*args):return (initialized_fn if effects.sequence else native_fn)(*args)
effects.fn=invoke
for name in ('sa_transfer_context_bus_init','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes
effects.LIB.sa_hardware_transfer_set_samples.restype=ctypes.c_int;effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
exec(compile(old.compare_code,'renderer_setup_comparison','exec'),dict(old._compare.__globals__,effects=effects,namespace=namespace),local:={});_compare=local['compare']
def compare(m,s,f,c):effects.sequence=c.get('initialize_context',False);return _compare(m,s,f,c)
load=old.load;store=old.store
DECREMENT,PRIMARY,INDEX5,INDEX8,INSTALL,DEFAULT=(0x8c653386,0x8c64133e,0x8c64162a,0x8c64167e,0x8c6412c2,0x8c6411b0)
DRIVER=0x8cd90000
def prepare(ram,c):
 m,s,f=old.prepare(ram,c)
 if c['entry']==DECREMENT:s.r[4]=c.get('value',0)
 elif c['entry'] in (PRIMARY,INDEX5,INDEX8,INSTALL,DEFAULT):
  store(m,0x8c67c004,c.get('driver',DRIVER));m[DRIVER&0xffffff:(DRIVER&0xffffff)+0x300]=bytes([0xa5])*0x300
  if c['entry'] in (PRIMARY,INDEX5,INDEX8):s.r[4]=c.get('callback',0x8c641698);s.r[5]=c.get('argument',0x12345678)
  else:s.r[4]=c.get('driver',DRIVER);s.r[5]=c.get('index',0);s.r[6]=c.get('callback',0x8c641698);s.r[7]=c.get('argument',0x12345678)
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f
def cases():
 for mode,value in itertools.product((0,4),(0,1,2,16,0x7fffffff,0x80000000,0xffffffff)):yield dict(entry=DECREMENT,mode=mode,value=value)
 for entry,mode,callback,sr in itertools.product((PRIMARY,INDEX5,INDEX8),(0,4),(0,0x8c641698),(0,0x3f3,0x700003f3)):
  yield dict(entry=entry,mode=mode,callback=callback,sr=sr)
 for entry,mode,index,callback in itertools.product((INSTALL,DEFAULT),(0,4),tuple(range(11))+(0x20000000,0x20000001,0xffffffff),(0,0x8c641698)):
  yield dict(entry=entry,mode=mode,index=index,callback=callback,sr=0x700003a2)
 for entry in (PRIMARY,INDEX5,INDEX8,INSTALL):
  yield dict(entry=entry,mode=0,driver=0)
  yield dict(entry=entry,mode=0,bad_stack=True)
def semantics(m,a,before,s,record,c):
 if record['status']:return 0
 entry=c['entry'];checks=0
 if entry==DECREMENT:
  assert s.r[0]==0 and s.r[4]==(c['value']-1)&0xffffffff and s.r[3]==load(m,0x8c6533d0)
  assert load(a,s.r[3])==(c['value']-1)&0xffffffff and s.r[15]==before.r[15] and s.sr==before.sr
  checks=7
 else:
  index=c.get('index',{PRIMARY:0,INDEX5:5,INDEX8:8}.get(entry,0));callback=c.get('callback',0x8c641698)
  expected=(DRIVER+0x230+((index<<3)&0xffffffff))&0xffffffff
  defaults=(0x8c641698,0x8c64169c,0x8c6416a0,0x8c6416a4,0x8c6416e0,0x8c6416e4,0x8c6416e8,0x8c6416ec,0x8c6416f0)
  if entry==DEFAULT or not callback:
   if index<9:assert load(a,DRIVER+0x230+index*8)==defaults[index];checks+=1
   else:assert a[(DRIVER&0xffffff):(DRIVER&0xffffff)+0x300]==m[(DRIVER&0xffffff):(DRIVER&0xffffff)+0x300];checks+=1
  else:
   assert load(a,expected)==callback and load(a,expected+4)==(0 if entry==INDEX8 else c.get('argument',0x12345678));checks+=2
  assert s.r[15]==before.r[15] and s.sr&0xf0==before.sr&0xf0;checks+=2
 return checks
def main():
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases():
   m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s)
   record,path,a,s,f=compare(m,s,f,c)
   expected=-3 if c.get('bad_stack') or c.get('driver')==0 else (-4 if c['entry'] in (PRIMARY,INDEX5,INDEX8,INSTALL) and 'sr' in c and not c['sr']&0x40000000 else 0)
   assert record['status']==expected,(c,record['status'],record['stop_pc'])
   record['independent_checks']=semantics(m,a,before,s,record,c);records.append(record);coverage.update(path)
   for j,pc in enumerate(path):
    text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
    if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_render_setup_connection.json']
 coverage_rows=[]
 for routine in EVIDENCE_ROUTINES:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(map(lambda x:int(x,16),e)) for e in routine['conditional_edges']}
  coverage_rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],
                            conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 report=dict(cases_passed=len(records),independent_checks=sum(r['independent_checks'] for r in records),status_counts=dict(collections.Counter(r['status'] for r in records)),covered_addresses=[hex(x) for x in sorted(coverage)],coverage_by_routine=coverage_rows,
             routines=manifest['routines'],dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records)
 (P/'analysis/movie_render_setup_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Renderer setup direct comparisons:',len(records))
if __name__=='__main__':main()
