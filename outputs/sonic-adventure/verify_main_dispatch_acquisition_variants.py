"""Native header variants versus original code and independent packet histories."""
import ctypes,gc,hashlib,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_alternate_strips as previous
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR,ALTERNATE,ALT_CLIP=previous.PLAIN,previous.CLIPPED,previous.NEAR,previous.ALTERNATE,previous.ALT_CLIP
SECONDARY,EXTENDED,EXT_SECONDARY=0x8C605F3C,0x8C6064B0,0x8C6065B0
ACQUISITIONS={0x8C605E68,SECONDARY,EXTENDED,EXT_SECONDARY};SERVICES=previous.SERVICES-ACQUISITIONS
PACKETS,SECOND_PACKETS,INPUT=previous.PACKETS,0x8CDE0000,0x8CD00000
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_acquisition_variants.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,acquire_secondary=(SECONDARY,'sa_material_acquire_secondary_connected'),acquire_extended=(EXTENDED,'sa_material_acquire_extended_connected'),acquire_extended_secondary=(EXT_SECONDARY,'sa_material_acquire_extended_secondary_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype
KINDS=('acquire_secondary','acquire_extended','acquire_extended_secondary')
CURSOR_INDEX={'acquire':4,'acquire_secondary':5,'acquire_extended':4,'acquire_extended_secondary':5}
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 *_,allowed,execute=previous.setup()
 manifest_path=PROJECT/'analysis/main_dispatch_acquisition_variants_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_alternate_strips_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};base.TRACKED|=ACQUISITIONS
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 m,s,f=previous.prepare(c,n)
 if c['entry'] in CURSOR_INDEX or c.get('acquisition_mesh'):
  store(m,services.HEADER+16,PACKETS);store(m,services.HEADER+20,SECOND_PACKETS)
  if c['entry'] in CURSOR_INDEX:store(m,services.HEADER+4*CURSOR_INDEX[c['entry']],c.get('cursor',PACKETS))
  for i,v in enumerate(c.get('payload',(0x12345678,0x87654321,0xA5A55A5A,0x5A5AA5A5))):store(m,INPUT+i*4,v)
 return m,s,f

def cases():
 for entry,cursor,header,mode in itertools.product(KINDS,(PACKETS,PACKETS+32,0xACDC0000,0xACDC0020),
   ((0,0,0,0),(0xFFFFFFFF,)*4,(0x12345678,0x87654321,0xA5A55A5A,0x5A5AA5A5)),range(8)):
  yield dict(entry=entry,cursor=cursor,header=header,mode=mode<<19,repeat=3)
 for entry,rm in itertools.product(KINDS,range(4)):yield dict(entry=entry,rm=rm)
 for bank,reverse in itertools.product(range(2),(False,True)):
  sequence=('acquire','acquire_extended_secondary','acquire_secondary','acquire_extended','acquire_secondary','acquire_extended_secondary','acquire_extended','acquire')
  yield dict(entry='acquire',sequence=sequence[::-1] if reverse else sequence,mode=bank<<21)
 for extra,normals,bank,materials in itertools.product((0,0x100000,0x200000,0x300000),(False,True),range(2),((0,),(0,0,1),(0,1,0))):
  yield dict(entry='mesh',acquisition_mesh=True,material_extra=extra,normals=normals,mode=bank<<21,materials=materials)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();records=[];counts={};totals=dict(stage_comparisons=0,boundary_comparisons=0,numerical_operations=0,queue_flushes=0,queue_events=0,independent_packets=0,independent_mesh_cases=0,external_service_boundaries=0)
 reached={a:0 for a in ACQUISITIONS}
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,cursor=PACKETS+32,repeat=2) for e in KINDS]+[dict(entry='acquire',sequence=('acquire_extended_secondary','acquire','acquire_secondary','acquire_extended'))]+[dict(entry='mesh',acquisition_mesh=True,material_extra=extra) for extra in (0,0x100000,0x200000,0x300000)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);ref=QueueReference();native=Memory();bus=Bus();macl=0xFEED1234;local=[]
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  for i in range(16):native.queue[i]=ref.queue[i]=0xD0D00000+i*0x101
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  sequence=c.get('sequence',(c['entry'],)*c.get('repeat',1))
  def run(entry):
   nonlocal macl
   native.trace_count=0;native.trace_dropped=0;ref.events=[];old_flush=native.flushes
   path,status,b,ops,macl,external=execute(dict(c,entry=entry),m,s,f,entry,macl,allowed)
   assert status==0,(c,entry,status)
   previous.previous.previous.acquisition.compare_queue(native,ref)
   rows.update(set(path)&want)
   for a in reached:reached[a]+=path.count(a)
   totals['stage_comparisons']+=1;totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops
   totals['queue_flushes']+=native.flushes-old_flush;totals['queue_events']+=native.trace_count+native.trace_dropped;totals['external_service_boundaries']+=external
   local.append(dict(entry=entry,steps=len(path),full_state_match=True,queue_match=True))
   return path
  for entry in sequence:
   if entry in CURSOR_INDEX:
    s.r[4]=INPUT;before=bytes(m);index=CURSOR_INDEX[entry];cursor=load(m,services.HEADER+index*4);other=load(m,services.HEADER+(9-index)*4)
    old_queue=list(native.queue);old_flush=native.flushes
    run(entry);q=(cursor>>5)&1
    words=[load(before,services.HEADER+i*4) for i in range(4)]
    if index==4:words[2]=(words[2]&0x03EFFFFF)|0x20080000
    extended=entry in ('acquire_extended','acquire_extended_secondary')
    packet=words+[load(before,INPUT+i*4) for i in range(4)] if extended else words[:3]+old_queue[q*8+3:q*8+8]
    assert [load(m,cursor+i*4) for i in range(8)]==packet,(c,entry,'packet')
    assert list(native.queue[q*8:q*8+8])==packet and list(native.queue[(1-q)*8:(1-q)*8+8])==old_queue[(1-q)*8:(1-q)*8+8]
    assert list(native.qacr)==[(cursor>>24)&28]*2 and native.flushes==old_flush+1
    assert s.r[0]==((cursor&0x03FFFFFF)|0xE0000000)+32
    if extended:assert s.r[4]==INPUT and s.r[5]==s.r[0]
    else:assert s.r[4]==s.r[0]
    assert load(m,0x8C890128)==cursor&0xFC000000 and load(m,0x8C89012C)==services.HEADER+index*4
    assert load(m,services.HEADER+index*4)==cursor and load(m,services.HEADER+(9-index)*4)==other
    totals['independent_packets']+=1
    s.r[4]=s.r[0];run('finish')
    assert load(m,services.HEADER+index*4)==cursor+32 and load(m,services.HEADER+(9-index)*4)==other
   else:
    before=bytes(m);path=run(entry);extra=c['material_extra'];expected={0:0x8C605E68,0x100000:SECONDARY,0x200000:EXTENDED,0x300000:EXT_SECONDARY}[extra]
    materials=c.get('materials',(0,));count=sum(i==0 or material!=materials[i-1] for i,material in enumerate(materials))
    index=5 if extra&0x100000 else 4;start=load(before,services.HEADER+index*4);other=load(before,services.HEADER+(9-index)*4)
    assert path.count(expected)==count and sum(path.count(a) for a in ACQUISITIONS)==count
    assert load(m,services.HEADER+index*4)==start+32*count and load(m,services.HEADER+(9-index)*4)==other
    last=start+32*(count-1);words=[load(m,services.HEADER+i*4) for i in range(4)]
    if index==4:words[2]=(words[2]&0x03EFFFFF)|0x20080000
    if extra&0x200000:words.extend(load(m,mesh.STATE+16+i*4) for i in range(4))
    else:words=words[:3]
    assert [load(m,last+i*4) for i in range(len(words))]==words,(c,'connected last header')
    totals['independent_mesh_cases']+=1
  key='history' if c.get('sequence') else c['entry'];counts[key]=counts.get(key,0)+1
  records.append(dict(input=c,stages=local,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'native acquisition-variant cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Acquisition-variant smoke checks passed:',len(records));return
 assert rows==want and all(reached.values())
 files=[Path(__file__),Path(previous.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_acquisition_variants.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_acquisition_variants_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=0,call_sites_covered=0,**totals,acquisition_invocations={hex(a):n for a,n in sorted(reached.items())},service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Three complete header variants with original full-state/queue comparison, independent packets and finish/cursor histories, all four original mesh acquisition selectors native.',limitations=['Valid aligned coherent-RAM/MMU-off destinations and bounded payloads only. No physical GPU FIFO, pixels, MMU-on/cache timing/fault or full-game proof.','Sixteen remaining primitive emitters use fixtures in connected mesh tests; no visible-rendering claim. Existing numerical guards/oracle limits retained.'],cases=records)
 (PROJECT/'analysis/main_dispatch_acquisition_variants_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered',*totals,'acquisition_invocations')},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
