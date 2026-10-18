import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_tikal_position_native import Vector

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes();binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_display_rotations.dll')).sa_tikal_display_rotations
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call,Vector]
 events=[];actual=[];ve=[];va=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  b=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),fp.banks[b][4],r[15],pr)
  if mutate and target==0x8c056926:struct.pack_into('<2I',mem,0xa00120,0x8ca00800,0x8ca00900)
  if mutate and target==0x8c608ccc:
   struct.pack_into('<I',mem,0xa00238,0x8ca00d00);struct.pack_into('<H',mem,0xa00520,2)
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
 def arithmetic(w,fp,trace):
  trace.append((w,bytes(fp)[:132]));b=(fp.fpscr>>21)&1
  if w==0xf0fd:fp.banks[b][0:2]=[0x3f000001,0x3f800002]
  elif w in [0xf1fd,0xf5fd]:
   start=(w>>8)&12
   for i,v in enumerate([0x3e800000,0xbf000000,0x3f800000,0x3f800000]):fp.banks[b][start+i]=v
  else:raise AssertionError(hex(w))
 @Vector
 def vector(ctx,w,fp):
  try:arithmetic(w,fp.contents,ve)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 count=0
 for bank,mode,hidden,special,index,angles,mutate in itertools.product(range(2),[0,2,0xffff],[0,1,0xfffe,0xffff],range(2),[0,7,0xffff],[(0,0,0x8000),(1,2,3)],range(2)):
  for mem in (ram,native):
   mem[0xa00000:0xa01000]=bytes(4096);mem[0xf00000-64:0xf00000]=bytes(64);mem[0xa08000:0xa08200]=bytes(512)
   struct.pack_into('<2I',mem,0xa00120,0x8ca00200,0x8ca00300);struct.pack_into('<I',mem,0xa00300,0x8ca00400)
   struct.pack_into('<H',mem,0xa00206,hidden);struct.pack_into('<3I',mem,0xa00214,*angles)
   struct.pack_into('<2I',mem,0xa00238,0x8ca00b00,0x8ca00f00)
   struct.pack_into('<I',mem,0xa00b0c,0x8ca00c00);struct.pack_into('<I',mem,0xa00d0c,0x8ca00e00);struct.pack_into('<I',mem,0xa00f40,special)
   struct.pack_into('<3H',mem,0xa00520,mode,index,(index+1)&65535);struct.pack_into('<I',mem,0xa00530,0x3f000000);struct.pack_into('<I',mem,0xa00540,0x8c900000);struct.pack_into('<I',mem,0xa00548,0x8ca00a00)
   for i in [index,(index+1)&65535]:struct.pack_into('<I',mem,0x900000+i*16,0x12345678+i)
   for a in [0xa00c08,0xa00e08]:struct.pack_into('<3I',mem,a,0x3f800000,0xbf800000,0x80000000)
   struct.pack_into('<2I',mem,0x88f698,16,2);struct.pack_into('<I',mem,0x88f5f8,0x8ca08000)
  f=Fpu();f.fpscr=0x7f07c|(bank<<21);f.fpul=0x12345678
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*37
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[15]=0x8cf00000;s.pc=0xcb008a0;s.pr=0x8c000100;s.sr=0x700083f3 if index&1 else 0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear();ve.clear();va.clear()
  targets=[0x8c056926,0x8c05694a,0x8c038cec,0x8c608ccc,0x8c04b57c,0x8c040942,0x8c056938]
  reference(ram,s,{t:hook(t) for t in targets},f,vector_instruction=lambda w,fp:arithmetic(w,fp,va))
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,vector)==0
  nf.fpul=fpul.value
  assert events==actual and ve==va and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(bank,mode,hidden,special,index,angles,mutate)
  count+=1
 assert not errors,errors
 report=dict(cases_passed=count,module_sha256=hashlib.sha256(module).hexdigest(),source_sha256=hashlib.sha256((out/'source/tikal_display.c').read_bytes()).hexdigest(),scope='Whole display callback with native axis rotations and matrix push/pop/translation, original bodies unhooked. Hidden flag, special model, current/previous mode/index selection including65535, zero/mixed angles and 0x8000 cancellation, cached work/entity despite task callback mutation and model/mode mutation before render. All registers/SR/PR/FPU/FPUL/stack/RAM/call FR4 compared. State/material/draw services and FSCA/FTRV numerical outputs injected. PR=SZ=0; controller connection pending; no native host renderer.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','tikal_display_rotations.c', 'tikal_display.c', 'model_matrix_stack_push.c', 'model_matrix_stack_pop.c', 'model_matrix_translate.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_display_rotations.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_display_rotations_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'Tikal native-display comparisons passed')
if __name__=='__main__':main()
