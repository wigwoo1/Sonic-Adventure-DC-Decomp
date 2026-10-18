import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
Vector=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu))
def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes();binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/camera_position_native.dll')).sa_camera_position_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call,Vector]
 events=[];actual=[];ve=[];va=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  trace=(target,tuple(r[4:8]),r[15],pr)
  if mutate:struct.pack_into('<I',mem,0xa00138,0x8ca00d00)
  r[0]=0x76543210;r[1]=0x11223344;r[2]=0x22334455;r[3]=0x33445566
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);events.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):
   trace,sr=effect(mem,r,sr,f,t,s.pr);actual.append(trace);return sr
  return body
 def arithmetic(word,fp,trace):
  trace.append((word,bytes(fp)[:132]));bank=(fp.fpscr>>21)&1
  if word==0xf0fd:fp.banks[bank][0:2]=[0x3f000001,0x3f800002]
  elif word in [0xf1fd,0xf5fd]:
   start=(word>>8)&12
   for i,v in enumerate([0x3e800000,0xbf000000,0x3f800000,0]):fp.banks[bank][start+i]=v
  elif word in [0xf0ed,0xf37d]:fp.banks[bank][3]=0x3f800000
  else:raise AssertionError(hex(word))
 @Vector
 def vector(ctx,word,fp):
  try:arithmetic(word,fp.contents,ve)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 count=0
 for bank,rm,sr0,angles,normalize,mutate,phase in itertools.product(range(2),range(2),[0,0x700083f3],[(0,0,0),(1,0,0),(0,1,0),(0,0,1),(0x80000000,0xffffffff,0x1234)],range(2),[0],[0]):
  for mem in (ram,native):
   mem[0xa00000:0xa01000]=bytes(4096);mem[0xf00000-64:0xf00000]=bytes(64);mem[0xa08000:0xa08200]=bytes(512)
   struct.pack_into('<I',mem,0xa00138,0x8ca00b00);struct.pack_into('<I',mem,0xa00b0c,0x8ca00c00)
   struct.pack_into('<3I',mem,0xa00114,*angles);struct.pack_into('<3I',mem,0xa00120,0x3f800000,0xbf800000,0x80000000)
   struct.pack_into('<3I',mem,0xa00380,0x3f800000,0xbf000000,0x3e800000);struct.pack_into('<I',mem,0xa0031c,phase);struct.pack_into('<2I',mem,0x88f698,16,2);struct.pack_into('<I',mem,0x88f5f8,0x8ca08000);struct.pack_into('<I',mem,0x890070,normalize)
   for i in range(16):struct.pack_into('<I',mem,0x67c640+i*4,0x3f800000 if i%5==0 else 0)
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x12345678
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*37
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[5]=0x8ca00380;s.r[6]=0x8ca00200;s.r[15]=0x8cf00000;s.pc=0x8c078eb0;s.pr=0x8c000100;s.sr=sr0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear();ve.clear();va.clear()
  reference(ram,s,{t:hook(t) for t in []},f,vector_instruction=lambda w,fp:arithmetic(w,fp,va))
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,vector)==0
  nf.fpul=fpul.value
  assert events==actual and ve==va,(bank,rm,sr0,angles,normalize,mutate,phase,'calls')
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(bank,rm,sr0,angles,normalize,mutate,phase)
  count+=1
 prep=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/camera_position_native.dll')).sa_camera_matrix_prepare
 prep.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P];prepare_count=0
 for depth,limit,bank in itertools.product([0,1,2,15,16,0xffffffff,0x80000000],[0,1,16,0x7fffffff,0xffffffff],range(2)):
  for mem in (ram,native):
   struct.pack_into('<2I',mem,0x88f698,limit,depth);struct.pack_into('<I',mem,0x88f5f8,0x8ca08000);mem[0xa08000:0xa08200]=bytes(512)
  f=Fpu();f.fpscr=bank<<21;f.fpul=0x12345678
  for b in range(2):f.banks[b][:]=[0x12340000+b*16+i for i in range(16)]
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c057b40;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);reference(ram,s,fpu=f)
  assert prep(view,ctypes.byref(nf),ctypes.byref(sr),r)==0
  assert list(r)==list(s.r) and sr.value==s.sr and bytes(nf)==bytes(f) and ram==native,(depth,limit,bank)
  prepare_count+=1
 assert not errors,errors
 report=dict(cases_passed=count,prepare_cases_passed=prepare_count,module_sha256=hashlib.sha256(module).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_camera_position.c').read_bytes()).hexdigest(),scope='Whole camera vector helper with native matrix identity prepare, axis rotation, matrix pop and direction transform, original unhooked bodies executed. All registers/SR/PR/FPU/FPUL/stack/RAM/calls compared. Axis rotation helper bodies native; FSCA/FTRV/FIPR/FSRRA numerical outputs injected, scalar add/multiply native. Zero/mixed/high angles, normalize branches,  banks/RM0/1, input vector overwritten after transformation. PR=SZ=0 finite scalar scope; controller connection pending.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','camera_position_native.c', 'sa_camera_position.c','camera_matrix_prepare.c', 'model_matrix_stack_pop.c', 'model_direction_transform.c', 'sh4_add_sub.c', 'sh4_multiply.c', 'sh4_fmac.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/camera_position_native.dll').read_bytes()).hexdigest()
 (out/'analysis/camera_position_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'Camera vector comparisons passed')
if __name__=='__main__':main()
