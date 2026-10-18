"""Original model streams through the complete native mesh renderer chain."""
import ctypes,gc,hashlib,itertools,json,math,struct,sys
from pathlib import Path
import verify_main_dispatch_mesh_families as combined
from model_asset_inventory import Image,InvalidModel,scan
PROJECT=Path(__file__).resolve().parent
previous=combined.previous;base,current,mesh,services=combined.base,combined.current,combined.mesh,combined.services
load,store=combined.load,combined.store
DLL=base.ROOT/'work/sonic-adventure/build/main_model_assets.dll';LIB=ctypes.CDLL(str(DLL))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in combined.SPECS.items()}
for name,fn in FUNCTIONS.items():fn.argtypes=combined.FUNCTIONS[name].argtypes;fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(combined.LIB,name).argtypes;fn.restype=getattr(combined.LIB,name).restype
INVENTORY=PROJECT/'analysis/main_model_assets_inventory.json'
BINARY=base.ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def selection(inventory):
 models=inventory['models'];chosen={}
 def add(model,reason):chosen.setdefault(model['address'],dict(model=model,reasons=[]))['reasons'].append(reason)
 for family in combined.FAMILIES:
  candidates=[m for m in models if m['object_node_references'] and any(s['family']==family for s in m['segments'])]
  add(min(candidates,key=lambda m:(m['vertex_count'],sum(s['corner_count'] for s in m['segments']))),'family '+family)
 for m in models:
  if any(s['topology']=='0x8000' for s in m['segments']):add(m,'8000 topology')
 for root in inventory['known_particle_roots']:add(next(m for m in models if m['address']==root['model']),'confirmed particle root')
 return [chosen[a] for a in sorted(chosen,key=lambda a:int(a,16))]

def asset_spans(image,model):
 spans=[(int(model['address'],16),40),(int(model['positions'],16),model['vertex_count']*12),(int(model['segment_table'],16),model['segment_count']*24),(int(model['material_table'],16),model['material_count']*20)]
 if int(model['normals'],16):spans.append((int(model['normals'],16),model['vertex_count']*12))
 for s in model['segments']:
  spans.append((int(s['index_stream'],16),s['index_bytes']))
  if int(s['uv_pointer'],16):spans.append((int(s['uv_pointer'],16),s['corner_count']*4))
 return [(a,image.span(a,n)) for a,n in spans]

def prepare(c,n,model,image):
 fixture=dict(entry='mesh',vertex_limit=max(4096,model['vertex_count']),mode=c['mode'],normals=True,clip=False,acquisition_mesh=True,environment_constant_mesh=True,screen_outside=False,material_extra=0,layout=0,cursor_offset=c['cursor_offset'])
 m,s,f=previous.previous.prepare(fixture,n);s.r[4]=int(model['address'],16)
 vertices=struct.unpack('<'+'f'*(model['vertex_count']*3),image.span(int(model['positions'],16),model['vertex_count']*12))
 axes=[vertices[j::3] for j in range(3)]
 axis=max(range(3),key=lambda j:max(axes[j])-min(axes[j])) if c['camera']=='crossing' else 2
 permutation=[j for j in range(3) if j!=axis]+[axis]
 shift=1.-(min(axes[axis])+max(axes[axis]))*0.5 if c['camera']=='crossing' else 2.**math.ceil(math.log2(max(8.,max(abs(v) for v in vertices)*4.+8.)))
 for i in range(16):store(m,0x8ce10000+i*4,0)
 for col,row in enumerate(permutation):store(m,0x8ce10000+(row*4+col)*4,current.bits(1.))
 store(m,0x8ce10000+14*4,current.bits(shift));store(m,0x8ce10000+15*4,current.bits(1.))
 for i in range(16):f.banks[((f.fpscr>>21)&1)^1][i]=load(m,0x8ce10000+i*4)
 store(m,0x8c88f614,current.bits(1e8));store(m,0x8c88f658,0xffffffff);store(m,0x8c88f65c,0xfffffffe)
 # Supply bounded runtime texture descriptors for original material IDs. They
 # stand for loaded textures; actual image pixels/GPU memory are not emulated.
 table,records=0x8cb00000,0x8cb10000;store(m,services.TEXROOT,table)
 ids={image.u32(int(seg['material_address'],16)+12)&image.u32(0x8c620b80) for seg in model['segments'] if int(seg['material_flags'],16)&0x200000}
 assert all(i<4096 for i in ids)
 for k,idx in enumerate(sorted(ids)):
  record=records+k*32;store(m,table+idx*12+8,record)
  for j,v in enumerate((0x11000000+idx,0x22000000+idx,0x13579bdf,0x80000)):store(m,record+j*4,v)
 for a,raw in asset_spans(image,model):assert bytes(m[a&0xffffff:(a&0xffffff)+len(raw)])==raw,(c,'fixture altered asset',hex(a))
 return fixture,m,s,f,permutation,current.value(current.bits(shift)),ids

def check(c,n,model,image,allowed,execute):
 fixture,m,s,f,permutation,shift,ids=prepare(c,n,model,image)
 ref=combined.QueueReference();native=combined.Memory();bus=combined.Bus()
 LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
 for i in range(16):native.queue[i]=ref.queue[i]=combined.SEED[i]
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,queue_reference=ref,bus_pointer=ctypes.byref(bus))
 original_reference=base.previous.reference
 def bounded_reference(*args,**kwargs):kwargs['instruction_budget']=500000;return original_reference(*args,**kwargs)
 base.previous.reference=bounded_reference
 try:path,status,b,ops,_,external=execute(fixture,m,s,f,'mesh',0xfeed1234,allowed)
 finally:base.previous.reference=original_reference
 previous.flat_alternate.previous.previous.acquisition.compare_queue(native,ref)
 assert status==0 and external==0,(c,model['address'],status)
 for a,raw in asset_spans(image,model):assert bytes(m[a&0xffffff:(a&0xffffff)+len(raw)])==raw,(c,'asset modified',hex(a))
 parents={target for f in combined.FAMILIES.values() for target in f[1:3]};observed=[a for a in path if a in parents]
 if c['camera']=='front':
  expected=[combined.FAMILIES[seg['family']][1] for seg in model['segments']]
  assert observed==expected,(c,model['address'],'dispatch',[hex(a) for a in observed],[hex(a) for a in expected])
  assert load(m,0x8c88f658)==(0xffffffff+sum(seg['corner_count'] for seg in model['segments']))&0xffffffff,(c,'index consumption')
  for i in range(model['vertex_count']):
   xyz=[struct.unpack('<f',image.span(int(model['positions'],16)+i*12+j*4,4))[0] for j in range(3)]
   want=(xyz[0]/(xyz[2]+shift),xyz[1]/(xyz[2]+shift),1./abs(xyz[2]+shift))
   got=[current.value(load(m,combined.CACHE+i*32+16+j*4)) for j in range(3)]
   assert load(m,combined.CACHE+i*32+12)==0 and all(math.isclose(a,b,rel_tol=2e-5,abs_tol=2e-6) for a,b in zip(got,want)),(c,'projected vertex',i,got,want)
 return dict(input=c,model=model['address'],status=status,steps=len(path),boundary_comparisons=b,numerical_operations=ops,external_service_boundaries=external,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped,full_state_match=True,queue_match=True,original_asset_spans_preserved=True,independent_projection_and_stream_count=c['camera']=='front',parent_targets=[hex(a) for a in observed],near_invocations=sum(path.count(f[3]) for f in combined.FAMILIES.values()),texture_ids=sorted(ids))

def main():
 baseline,allowed,execute=combined.setup();inventory=json.loads(INVENTORY.read_text());image=Image(BINARY.read_bytes(),0x8c010000)
 for p,h in inventory['sha256'].items():assert digest(Path(p))==h,p
 selected=selection(inventory);records=[]
 cases=[(item,dict(camera=camera,mode=bank<<21,cursor_offset=offset)) for item,camera,bank,offset in itertools.product(selected,('front','crossing'),range(2),(0,32))]
 if '--smoke' in sys.argv:cases=[(item,dict(camera='front',mode=0,cursor_offset=0)) for item in selected if any(r.startswith('family ') for r in item['reasons'])]
 for n,(item,c) in enumerate(cases):
  records.append(check(c,n,item['model'],image,allowed,execute))
  if (n+1)%20==0:print(n+1,'original-model native comparisons passed',flush=True)
  if (n+1)%10==0:gc.collect()
 if '--smoke' in sys.argv:print('Original-model smoke passed:',len(records));return
 totals={k:sum(r[k] for r in records) for k in ('boundary_comparisons','numerical_operations','external_service_boundaries','queue_flushes','queue_events','near_invocations')}
 files=[Path(__file__),PROJECT/'model_asset_inventory.py',Path(combined.__file__),INVENTORY,PROJECT/'build_main_model_assets.ps1',PROJECT/'analysis/main_dispatch_mesh_families_verification.json']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(cases_passed=len(records),selected_models=len(selected),selection=[dict(model=i['model']['address'],reasons=i['reasons']) for i in selected],independent_projection_cases=sum(r['independent_projection_and_stream_count'] for r in records),**totals,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(BINARY),newly_recovered_entries=0,scope='Unmodified original main-executable model/material/index/UV/normal streams through native material dispatch/projection/primitive/clipper/finalizer versus original instructions under constructed front/crossing cameras, both FPU banks and queue halves. Independent front projection and parsed stream-input counters; all input asset spans preserved.',limitations=['Selection covers representatives of nine static material families, the inventoried 8000-topology models and known particle roots, not every candidate or all game assets. Runtime camera and texture descriptors are constructed; loaded texture pixels, overlays, GPU output and actual gameplay reachability remain unverified.','Existing numerical backend, instruction guards, bounded RAM/MMU-off queue and original reference limits remain. No new recovered function credit.'],cases=records)
 (PROJECT/'analysis/main_model_assets_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','selected_models','independent_projection_cases',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
