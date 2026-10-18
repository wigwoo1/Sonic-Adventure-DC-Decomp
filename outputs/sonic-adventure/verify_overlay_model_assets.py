"""Original decoded overlay model streams through the native main renderer."""
import ctypes,gc,hashlib,inspect,itertools,json,math,struct,sys
from pathlib import Path
import verify_main_model_assets as main_assets
from overlay_model_assets import OverlayImage,canonical,digest
PROJECT=Path(__file__).resolve().parent
combined=main_assets.combined;previous=main_assets.previous
base,current,mesh,services=main_assets.base,main_assets.current,main_assets.mesh,main_assets.services
load,store=main_assets.load,main_assets.store
DLL=base.ROOT/'work/sonic-adventure/build/overlay_model_assets.dll';LIB=ctypes.CDLL(str(DLL))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in combined.SPECS.items()}
for name,fn in FUNCTIONS.items():fn.argtypes=combined.FUNCTIONS[name].argtypes;fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(combined.LIB,name).argtypes;fn.restype=getattr(combined.LIB,name).restype
BINARY=main_assets.BINARY;MAIN_IMAGE=OverlayImage(BINARY.read_bytes(),0x8c010000)
INVENTORY=PROJECT/'analysis/overlay_model_assets_inventory.json'
asset_spans=main_assets.asset_spans

# Extend the established frozen fixture locally. Overlay bytes are copied at
# their original mapped address; runtime texture records move outside the bank.
source=inspect.getsource(main_assets.prepare)
source=source.replace("m,s,f=previous.previous.prepare(fixture,n);s.r[4]=int(model['address'],16)","m,s,f=previous.previous.prepare(fixture,n);s.r[4]=int(model['address'],16)\n assert image.end<0x8cc00000\n m[image.base&0xffffff:(image.base&0xffffff)+len(image.data)]=image.data")
source=source.replace("axis=max(range(3),key=lambda j:max(axes[j])-min(axes[j])) if c['camera']=='crossing' else 2", "axis=c.get('axis',max(range(3),key=lambda j:max(axes[j])-min(axes[j]))) if c['camera']=='crossing' else 2")
source=source.replace('table,records=0x8cb00000,0x8cb10000','table,records=0x8c700000,0x8c720000').replace('image.u32(0x8c620b80)','MAIN_IMAGE.u32(0x8c620b80)')
exec(compile(source,'overlay_asset_prepare','exec'),globals())
source=inspect.getsource(main_assets.check)
old=" def bounded_reference(*args,**kwargs):kwargs['instruction_budget']=500000;return original_reference(*args,**kwargs)"
new=""" strip_reads=[];reference_inputs=[];native_inputs=[]
 def bounded_reference(*args,**kwargs):
  kwargs['instruction_budget']=500000;outer=kwargs['trace'];mem=args[0]
  def trace(pc,w,r,sr,pr,fp):
   if pc==0x8c61ef72:
    pointers=[r[i] for i in (12,14,13)]
    strip_reads.append(dict(pointers=[hex(a) for a in pointers],words=[[load(mem,a+j*4) for j in range(3)] for a in pointers],normalizer_index=len(reference_inputs)))
   if pc==previous.NORMALIZE:reference_inputs.append(tuple(load(mem,r[4]+j*4) for j in range(3)))
   outer(pc,w,r,sr,pr,fp)
  kwargs['trace']=trace
  return original_reference(*args,**kwargs)
 real_mesh=FUNCTIONS['mesh']
 def wrapped_mesh(*args):
  outer=args[-2]
  @base.OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   if target==previous.NORMALIZE:native_inputs.append(tuple(load(mem,r[4]+j*4) for j in range(3)))
   outer(ctx,target,mem,fp,sr,r,pr)
  return real_mesh(*args[:-2],observe,args[-1])
 execute.__globals__['FUNCTIONS']=dict(FUNCTIONS,mesh=wrapped_mesh)"""
assert source.count(old)==1;source=source.replace(old,new)
source=source.replace('finally:base.previous.reference=original_reference',"finally:base.previous.reference=original_reference;execute.__globals__['FUNCTIONS']=FUNCTIONS")
source=source.replace(' assert status==0 and external==0,'," assert native_inputs==reference_inputs,(c,'normalizer inputs')\n assert status==0 and external==0,")
source=source.replace(' return dict(input=c,'," return dict(module=c['module'],strip_reads=strip_reads,normalizer_inputs=reference_inputs,input=c,")
exec(compile(source,'overlay_asset_check','exec'),globals())


def selection(inventory):
 modules=[json.loads((PROJECT/m['report']).read_text()) for m in inventory['modules']]
 chosen={}
 def add(module,model,reason):chosen.setdefault((module['module'],model['address']),dict(module=module,model=model,reasons=[]))['reasons'].append(reason)
 for module in modules:
  candidates=[m for m in module['models'] if m['object_node_references']]
  if candidates:add(module,min(candidates,key=lambda m:(m['vertex_count'],sum(s['corner_count'] for s in m['segments']))),'module representative')
 for family in combined.FAMILIES:
  candidates=[(module,m) for module in modules for m in module['models'] if m['object_node_references'] and any(s['family']==family for s in m['segments'])]
  module,model=min(candidates,key=lambda pair:(pair[1]['vertex_count'],sum(s['corner_count'] for s in pair[1]['segments']),pair[0]['module']))
  add(module,model,'family '+family)
 for module in modules:
  for finding in module['face_nonalternate_strip_candidates']:add(module,next(m for m in module['models'] if m['address']==finding['model']),'face 8000 finding')
 return [chosen[k] for k in sorted(chosen)]


def main():
 baseline,allowed,execute=combined.setup();inventory=json.loads(INVENTORY.read_text())
 for p,h in inventory['sha256'].items():assert digest(Path(p))==h,p
 selected=selection(inventory);records=[];images={};cases=[]
 for item in selected:
  module=item['module']['module']
  for camera,bank,offset in itertools.product(('front','crossing'),range(2),(0,32)):
   cases.append((item,dict(module=module,camera=camera,mode=bank<<21,cursor_offset=offset)))
  if 'face 8000 finding' in item['reasons']:
   for axis,bank,offset in itertools.product(range(3),range(2),(0,32)):
    cases.append((item,dict(module=module,camera='crossing',axis=axis,mode=bank<<21,cursor_offset=offset)))
 if '--smoke' in sys.argv:cases=[(i,c) for i,c in cases if c['mode']==0 and c['cursor_offset']==0 and ('face 8000 finding' in i['reasons'] or c['camera']=='front')]
 for n,(item,c) in enumerate(cases):
  module=item['module'];name=module['module']
  if name not in images:images[name]=OverlayImage(Path(module['source_path']).read_bytes(),int(module['load_address'],16))
  record=check(c,n,item['model'],images[name],allowed,execute)
  for read in record['strip_reads']:
   pointers=[canonical(int(a,16)) for a in read['pointers']]
   target=next(s for s in item['model']['segments'] if s['topology']=='0x8000')
   assert pointers==[canonical(int(target['address'],16)),mesh.STATE,canonical(int(target['material_address'],16))],(c,'real overlay strip pointers',read)
   normalizer=record['normalizer_inputs'][read['normalizer_index']]
   for j in range(3):
    values=[current.value(row[j]) for row in read['words']]
    if any(math.isnan(v) for v in values):assert math.isnan(current.value(normalizer[j]))
    else:
     rounded=lambda v:current.value(current.bits(v))
     assert normalizer[j]==current.bits(rounded(rounded(values[0]+values[1])+values[2]))
  records.append(record)
  if (n+1)%40==0:print(n+1,'decoded overlay native-model comparisons passed',flush=True)
  if (n+1)%20==0:gc.collect()
 if '--smoke' in sys.argv:print('Overlay-model smoke passed:',len(records),'strip reads:',sum(len(c['strip_reads']) for c in records));return
 totals={k:sum(r[k] for r in records) for k in ('boundary_comparisons','numerical_operations','external_service_boundaries','queue_flushes','queue_events','near_invocations')}
 read_cases=[r for r in records if r['strip_reads']]
 assert {r['module'] for r in read_cases}=={'AL_MAIN.BIN','AL_RACE.BIN'}
 files=[Path(__file__),PROJECT/'overlay_model_assets.py',Path(main_assets.__file__),Path(combined.__file__),INVENTORY,PROJECT/'build_overlay_model_assets.ps1',PROJECT/'analysis/main_dispatch_mesh_families_verification.json']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256']);files.extend(Path(p) for p in inventory['sha256'])
 result=dict(cases_passed=len(records),selected_models=len(selected),modules_exercised=len(images),selection=[dict(module=i['module']['module'],model=i['model']['address'],reasons=i['reasons']) for i in selected],independent_projection_cases=sum(r['independent_projection_and_stream_count'] for r in records),original_face_strip_read_cases=len(read_cases),original_face_strip_reads=sum(len(r['strip_reads']) for r in records),**totals,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(BINARY),newly_recovered_entries=0,scope='Unmodified mapped overlay model/material/vertex/normal/index/UV data through the native main renderer against original instructions, constructed cameras/runtime descriptors, both FPU banks and cursor halves. Independent front projection/index counts, original/native normalizer inputs and two real overlay face-strip inherited-register observations.',limitations=['Structural selections and constructed runtime cameras, not complete scene/gameplay execution. The same five-index face strip occurs in two modules; these are distinct stored occurrences rather than two proven unique artworks. Texture images/GPU pixels, actual scene reachability and full frame ordering remain unverified.','Only self-contained bounded basic model records in existing mapped modules are inventoried. Cross-bank references, relocated offsets, alternative formats, unmapped binaries, physical timing and hardware numerical precision remain open.'],cases=records)
 (PROJECT/'analysis/overlay_model_assets_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','selected_models','modules_exercised','independent_projection_cases','original_face_strip_read_cases','original_face_strip_reads',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
