"""Complete original model morph/normal executions vs native valid fixtures."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_model_preflight import Model,Mesh,Vec
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','build','binary','numeric-dll','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(a.build/'build_verification.json');pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json');assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 sys.path.insert(0,str(p));import verify_startup as executor
 from model_reference import make_reference
 from verify_fpu_cleanup_transfer import Fpu
 reference,reference_hashes=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
 lib=C.CDLL(b['frames_DLL']);numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',C.c_int,C.c_uint32,C.c_void_p,C.c_uint32)
 def service(op,f):assert numeric(op,C.byref(f),f.fpul)==0
 initial=bytearray(16<<20);raw=a.binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw;ram=bytearray(initial);rng=random.Random(0x4d4f444c);host=(C.c_ubyte*0x8000)();at=C.addressof(host);base=0x8cd00000;records=[];checks=0
 names=('model_blend','model_calc_face_normals','model_calc_point_normals','points_blend','points_blend_sine','points_blend_square')
 for bank in range(2):
  for kind,strips in ((0,'plain'),(0x4000,'plain'),(0xc000,'plain'),(0xc000,'reversed'),(0xc000,'two')):
   count=3 if kind==0 else 4;indices=[0,1,2] if kind==0 else [0,1,2,3] if kind==0x4000 else [-32764,0,1,2,3] if strips=='reversed' else [4,0,1,2,3]
   if strips=='two':indices += [4,3,2,1,0]
   mesh_count=2 if strips=='two' else 1;faces=2*mesh_count if kind==0xc000 else 1
   for name in names:
    for sample,blend in enumerate((0.,0.125,0.5,0.875,1.)):
     for alias in ('separate','from','to') if name.startswith('points_') or name=='model_blend' else ('separate',):
      arena=bytearray(b'\xa5'*0x8000);coords=[(0.,0.,0.),(2.,0.3,0.),(0.,2.,0.2),(2.,2.,0.7)][:count]
      if kind==0x4000:coords=[(0.,0.,0.),(2.,0.,0.),(2.,2.,0.3),(0.,2.,0.)]
      for off,shift in ((0x1000,0.),(0x2000,0.6),(0x3000,0.2)):
       values=[v+shift*(j+1)+rng.uniform(-0.05,0.05) for xyz in coords for j,v in enumerate(xyz)];struct.pack_into('<'+'f'*len(values),arena,off,*values)
      struct.pack_into('<'+'h'*len(indices),arena,0x6000,*indices)
      # Well-defined face normals for the standalone point-normal stage.
      struct.pack_into('<'+'f'*(faces*3),arena,0x5000,*([0.1,0.2,0.9]*faces))
      target=0x1000 if alias=='from' else 0x2000 if alias=='to' else 0x3000
      point_offsets=(0x1000,0x2000,target);models=(Model*3)();mesh=Mesh();mesh.kind=kind;mesh.count=mesh_count;mesh.indices=C.cast(at+0x6000,C.POINTER(C.c_int16));mesh.normals=at+0x5000
      for j,off in enumerate(point_offsets):
       models[j].points=C.cast(at+off,C.POINTER(Vec));models[j].normals=C.cast(at+0x4000,C.POINTER(Vec));models[j].point_count=count;models[j].radius=1
       if j==2:models[j].meshes=C.pointer(mesh);models[j].mesh_count=1
       struct.pack_into('<IIiIIHH4f',arena,0x100+j*0x100,base+off,base+0x4000,count,base+0x700 if j==2 else 0,0,1 if j==2 else 0,0,0.,0.,0.,1.)
      struct.pack_into('<HHIIIII',arena,0x700,kind,mesh_count,base+0x6000,0,base+0x5000,0,0)
      ram[0xd00000:0xd08000]=arena;C.memmove(host,bytes(arena),len(arena));descriptors=bytes(models)+bytes(mesh);fp=Fpu();fp.fpscr=0x40001|(bank<<21);fp.banks[bank][4]=struct.unpack('<I',struct.pack('<f',blend))[0];s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000
      if name=='model_blend':s.r[4:7]=[base+0x100,base+0x200,base+0x300];fn=bind(lib,name,None,C.c_void_p,C.c_void_p,C.c_void_p,C.c_float);args=[C.byref(models[0]),C.byref(models[1]),C.byref(models[2]),blend]
      elif name.startswith('model_calc_'):s.r[4]=base+0x300;fn=bind(lib,name,None,C.c_void_p);args=[C.byref(models[2])]
      else:s.r[4:8]=[base+0x1000,base+0x2000,base+target,count];fn=bind(lib,name,None,C.c_void_p,C.c_void_p,C.c_void_p,C.c_float,C.c_int);args=[at+0x1000,at+0x2000,at+target,blend,count]
      fmac=[]
      def trace(pc,word,regs,sr,pr,fpu):
       if word&0xf00f==0xf00e:fmac.append(hex(pc))
      reference(ram,s,fpu=fp,vector_instruction=service,special={'gbr':0,'r_bank':[0]*8},trace=trace,instruction_budget=200000);fn(*args)
      expected=bytes(ram[0xd00800:0xd08000]);actual=bytes(host)[0x800:];diff=[i+0x800 for i,(x,y) in enumerate(zip(expected,actual)) if x!=y]
      (d/'last_case.json').write_text(json.dumps(dict(name=name,bank=bank,kind=kind,blend=blend,alias=alias,differences=[dict(offset=x,original=bytes(ram[0xd00000+(x&~3):0xd00004+(x&~3)]).hex(),native=bytes(host)[x&~3:(x&~3)+4].hex()) for x in diff[:40]],original_fmac_PC=fmac,initial_point_words={hex(x):bytes(arena[x:x+12]).hex() for x in (0x1000,0x2000,target)}),indent=2)+'\n')
      assert not diff,(name,bank,hex(kind),blend,alias,diff[:20]);checks+=1
      assert bytes(models)+bytes(mesh)==descriptors and s.r[15]==0x8cf00000 and bytes(ram[0xd00000:0xd00800])==bytes(arena[:0x800]);checks+=1
      records.append(dict(name=name,bank=bank,kind=kind,strips=strips,blend=blend,alias=alias,output_sha256=hashlib.sha256(actual).hexdigest()))
 out=dict(status='complete_original_model_morph_and_face_point_normals_match_native_valid_mesh_fixtures',checks=checks,original_complete_calls=len(records),records=records,triangle_quad_strip_meshes=True,in_place_source_target_point_aliases=True,whole_data_arena_and_guards_compared=True,original_model_or_mesh_descriptors_modified=False,reference_hashes=reference_hashes,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_binary_sha256=sha(a.binary),original_game_dependency_hooks_used=False,genuine_character_toe_models_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
 (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;',len(records),'complete original calls',flush=True)
if __name__=='__main__':main()
