"""Full-state original allocator comparisons with independent free-list checks."""
import collections,csv,ctypes,hashlib,itertools,json,types
from pathlib import Path
import verify_movie_render_setup as old
P=old.P;ROOT=old.ROOT;previous=old.previous;THUNK,ALLOC,ROOT_WORD=0x8c6021d2,0x8c6021d6,0x8c88ef3c
SENTINEL,ARENA,ARENA_SIZE=0x8cd00000,0x8cd00000,0x18000
MP=P/'analysis/movie_heap_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
exec(compile(old.old.code,'heap_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_heap.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
native_fn=effects.LIB.sa_movie_heap_connected;native_fn.argtypes=old.native_fn.argtypes;native_fn.restype=ctypes.c_int
initialized_fn=effects.LIB.sa_movie_heap_initialized_connected;initialized_fn.argtypes=native_fn.argtypes;initialized_fn.restype=ctypes.c_int;effects.sequence=False
def invoke(*args):return (initialized_fn if effects.sequence else native_fn)(*args)
effects.fn=invoke
for name in ('sa_transfer_context_bus_init','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes
effects.LIB.sa_hardware_transfer_set_samples.restype=ctypes.c_int;effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int;effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
exec(compile(old.old.compare_code,'heap_comparison','exec'),dict(old._compare.__globals__,effects=effects,namespace=namespace),local:={});_compare=local['compare']
def compare(m,s,f,c):
 effects.sequence=c.get('initialize_context',False);result=_compare(m,s,f,c)
 if c['entry'] not in NEW:
  record,path,a,s,f=result;record['heap_root_at_stop']=hex(load(a,ROOT_WORD));record['heap_cursor_register_at_stop']=hex(s.r[6])
 return result
load=old.load;store=old.store
def prepare(ram,c):
 m,s,f=old.prepare(ram,c)
 if c['entry'] not in NEW:return m,s,f
 s.r[4]=c.get('allocation_bytes',0);m[ARENA&0xffffff:(ARENA&0xffffff)+ARENA_SIZE]=bytes([0xa5])*ARENA_SIZE
 blocks=[SENTINEL+0x1000+i*0x4000 for i in range(len(c.get('blocks',[4])))];nodes=[SENTINEL]+blocks
 for i,node in enumerate(nodes):
  store(m,node,nodes[(i+1)%len(nodes)]);store(m,node+4,0 if i==0 else c.get('blocks',[4])[i-1])
 store(m,ROOT_WORD,nodes[c.get('cursor',0)])
 if c.get('invalid_root'):store(m,ROOT_WORD,0 if c['invalid_root']=='null' else SENTINEL+1)
 if c.get('invalid_next'):store(m,nodes[c.get('cursor',0)],0)
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f
def cases():
 profiles=((0,[1],0),(1,[2],0),(31,[2],0),(32,[2],0),(33,[3],0),(96,[4],0),(96,[7],0),
           (96,[1,4,2],0),(96,[1,7,2],0),(96,[1,2],0),(128,[],0),(0xffffffff,[1],0),
           (0xffffffe1,[4],0),(0x7fffffff,[2,3],0),(96,[4,1,2],2),(96,[2,6,3],1))
 for entry,mode,sr,profile in itertools.product((THUNK,ALLOC),(0,4),(0x40000000,0x400003f3,0x700003a2),profiles):
  request,blocks,cursor=profile;yield dict(entry=entry,mode=mode,sr=sr,allocation_bytes=request,blocks=blocks,cursor=cursor)
 for entry in (THUNK,ALLOC):
  for kind in ('null','unaligned'):yield dict(entry=entry,mode=0,invalid_root=kind)
  yield dict(entry=entry,mode=0,invalid_next=True)
  yield dict(entry=entry,mode=0,bad_stack=True)
  yield dict(entry=entry,mode=0,sr=0)
def expected_heap(m,request):
 # Independent list-level description: choose the first adequate successor of
 # the current cursor; trim its end or unlink an exact match. No CPU operations
 # or branch labels are used to select the expected result.
 wanted=bytearray(m);units=(((request+31)&0xffffffff)//32)+1;cursor=load(m,ROOT_WORD);prev=cursor;node=load(m,prev);seen=set()
 while True:
  assert node not in seen,'Fixture must form a finite circular list';seen.add(node)
  available=load(m,node+4)
  if available>=units:
   if available==units:store(wanted,prev,load(m,node));header=node
   else:
    remaining=available-units;store(wanted,node+4,remaining);header=node+remaining*32;store(wanted,header+4,units)
   store(wanted,ROOT_WORD,prev);return wanted,(header+32)&0xffffffff,units
  if node==cursor:return wanted,0,units
  prev,node=node,load(m,node)
def semantics(m,a,before,s,record,c):
 if record['status']:return 0
 wanted,result,units=expected_heap(m,c['allocation_bytes'])
 at=ARENA&0xffffff;assert a[at:at+ARENA_SIZE]==wanted[at:at+ARENA_SIZE]
 assert load(a,ROOT_WORD)==load(wanted,ROOT_WORD) and s.r[0]==result and s.r[5]==units
 assert s.r[15]==before.r[15] and s.r[14]==before.r[14] and s.sr&0xf0==before.sr&0xf0
 return 7
def main():
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 def run(m,s,f,c):
  before=effects.State.from_buffer_copy(s);record,path,a,s,f=compare(m,s,f,c)
  expected=-3 if any(c.get(k) for k in ('invalid_root','invalid_next','bad_stack')) else (-4 if 'sr' in c and not c['sr']&0x40000000 else 0)
  assert record['status']==expected,(c,record['status'],record['stop_pc'])
  record['independent_checks']=semantics(m,a,before,s,record,c);records.append(record);coverage.update(path)
  for j,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  return a,s,f
 for c in cases():run(*prepare(ram,c),c)
 for entry,mode in itertools.product((THUNK,ALLOC),(0,4)):
  c=dict(entry=entry,mode=mode,sr=0x700003a2,blocks=[12,7],allocation_bytes=32);m,s,f=prepare(ram,c);live=[]
  for number,request in enumerate((32,96,1,64,128,16,256,0,1024)):
   c=dict(c,allocation_bytes=request,sequence_step=number);s.pc=entry;s.r[4]=request;m,s,f=run(m,s,f,c)
   if s.r[0]:
    span=(s.r[0]-32,s.r[0]-32+(1+(((request+31)&0xffffffff)//32))*32)
    assert all(span[1]<=a or span[0]>=b for a,b in live);live.append(span);records[-1]['independent_checks']+=1
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_heap_connection.json'];rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,
             routines=manifest['routines'],dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,
             scope='Explicit well-formed direct circular-list fixtures, exact/split/later/wrapped-cursor/empty/out-of-memory and request-overflow arithmetic; four retained-state allocation sequences verify no block overlap. Invalid root/link/stack and privilege failures compare complete state. Fixtures do not initialize the startup heap or fabricate allocation results.')
 (P/'analysis/movie_heap_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
