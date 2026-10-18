"""Real header acquisition against original instructions and independent packets."""
import ctypes,gc,hashlib,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_vertex_workers as previous
PROJECT=Path(__file__).resolve().parent
services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
ACQUIRE=0x8C605E68;PACKETS=0x8CDC0000
SERVICES=previous.SERVICES-{ACQUIRE}
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_acquisition.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,acquire=(ACQUIRE,'sa_material_acquire_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 *_,allowed,execute=previous.setup()
 manifest_path=PROJECT/'analysis/main_dispatch_acquisition_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_vertex_workers_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 base.TRACKED.add(ACQUIRE)
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 m,s,f=previous.prepare(c,n)
 cursor=c.get('cursor',PACKETS)
 store(m,services.HEADER+16,cursor)
 # The draw wrapper copies this renderer cursor into the working header.
 store(m,particle.STATE_B,cursor)
 if c['entry']=='acquire':s.r[4]=0xDEADC0DE
 return m,s,f

def compare_queue(native,ref):
 actual=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert actual==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096)
 assert list(native.queue)==ref.queue and list(native.qacr)==ref.qacr
 assert (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates)

def cases():
 for cursor,header,mode in itertools.product((PACKETS,PACKETS+32,0xACDC0000,0xACDC0020),
    ((0,0,0),(0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF),(0x12345678,0x87654321,0xA5A55A5A),(0x80000000,1,0x01000000)),range(8)):
  yield dict(entry='acquire',cursor=cursor,header=header,mode=mode<<19,sequence=3)
 for bank,normals,clip,materials in itertools.product(range(2),(False,True),(False,True),((0,),(0,0,1))):
  yield dict(entry='mesh',mode=bank<<21,normals=normals,clip=clip,materials=materials)
 for entry,variant,bank in itertools.product(('draw','primary'),range(2),range(2)):
  yield dict(entry=entry,graph='retail',variant=variant,mode=bank<<21)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 rows=set();records=[];counts={};totals=dict(stage_comparisons=0,boundary_comparisons=0,numerical_operations=0,queue_flushes=0,queue_events=0,independent_packets=0,external_service_boundaries=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[selected[i] for i in (0,31,64,95,127,128,143,144,151)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  # Nonzero contents expose the original partial-header write: words 3..7 persist.
  for i in range(16):native.queue[i]=ref.queue[i]=0xD0D00000+i*0x101
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  local=[];cursor=c.get('cursor',PACKETS);macl=0xFEED1234
  for step in range(c.get('sequence',1)):
   before=bytes(m);old_queue=list(native.queue);old_flush=native.flushes
   native.trace_count=0;native.trace_dropped=0;ref.events=[]
   path,status,b,ops,macl,calls=execute(c,m,s,f,c['entry'],macl,allowed)
   assert status==0,(c,status);compare_queue(native,ref)
   rows.update(set(path)&set(range(ACQUIRE,0x8C605EB4,2)))
   totals['stage_comparisons']+=1;totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops
   totals['queue_flushes']+=native.flushes-old_flush;totals['queue_events']+=native.trace_count
   totals['external_service_boundaries']+=calls
   if c['entry']=='acquire':
    q=(cursor>>5)&1;words=[load(before,services.HEADER),load(before,services.HEADER+4),(load(before,services.HEADER+8)&0x03EFFFFF)|0x20080000]
    expected=words+old_queue[q*8+3:q*8+8]
    assert [load(m,(cursor&0x1FFFFFFF)+i*4) for i in range(8)]==expected
    assert list(native.queue[q*8:q*8+8])==expected
    assert list(native.queue[(1-q)*8:(1-q)*8+8])==old_queue[(1-q)*8:(1-q)*8+8]
    assert list(native.qacr)==[(cursor>>24)&28]*2
    assert s.r[0]==s.r[4]==((cursor&0x03FFFFFF)|0xE0000000)+32
    assert load(m,0x8C890128)==cursor&0xFC000000
    assert load(m,0x8C89012C)==services.HEADER+16
    assert load(m,services.HEADER+16)==cursor
    assert native.flushes==old_flush+1
    totals['independent_packets']+=1
    # Use the real finish routine, then acquire the next 32-byte header.
    s.r[4]=s.r[0]
    native.trace_count=0;ref.events=[]
    fp,status,fb,fo,macl,fc=execute(dict(c,entry='finish'),m,s,f,'finish',macl,allowed)
    assert status==0 and fc==0 and not ref.events;compare_queue(native,ref)
    cursor=(cursor+32)&0xFFFFFFFF
    assert load(m,services.HEADER+16)==cursor
    totals['stage_comparisons']+=1;totals['boundary_comparisons']+=fb;totals['numerical_operations']+=fo
   local.append(dict(steps=len(path),status=status,full_state_match=True,queue_match=True))
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,stages=local,full_state_match=True,queue_match=True))
  if (n+1)%40==0:print(n+1,'native acquisition cases passed',flush=True)
  if (n+1)%20==0:gc.collect()
 if '--smoke' in sys.argv:print('Acquisition smoke checks passed:',len(records));return
 assert rows==set(range(ACQUIRE,0x8C605EB4,2))
 files=[Path(__file__),Path(previous.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_acquisition.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_acquisition_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=0,call_sites_covered=0,**totals,
  sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),
  scope='Real header acquisition and finish sequence, connected mesh/draw/particle callers, full original state/RAM and independent queue events/packet/cursor invariants.',
  limitations=['Valid aligned coherent-RAM/MMU-off destinations only; remaining 21 acquisition/primitive services use fixtures. No GPU FIFO, pixels, MMU-on, hardware cache timing or fault proof.','Existing scalar/vector numerical scope and guards retained. No full gameplay/frame-driver or completed port proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_acquisition_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
