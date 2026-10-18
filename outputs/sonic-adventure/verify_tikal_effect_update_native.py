import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_tikal_position_native import Vector

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes();binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_effect_update_native.dll')).sa_tikal_effect_update_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call,Vector]
 events=[];actual=[];ve=[];va=[];errors=[];states=set();targets=set()
 def effect(mem,r,sr,fp,target,pr):
  bank=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),fp.banks[bank][4],fp.banks[bank][5],r[15],pr);targets.add(target)
  if event:struct.pack_into('<I',mem,0x78c608+owner*4,0x8ca01a00)
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
  if w==0xf06d:fp.banks[b][0]=0x3f800000
  elif w==0xf0fd:fp.banks[b][0:2]=[0x3f000001,0x3f800002]
  elif w in [0xf1fd,0xf5fd]:
   start=(w>>8)&12
   for i,v in enumerate([0x3e800000,0xbf000000,0x3f800000,0]):fp.banks[b][start+i]=v
  elif w in [0xf0ed,0xf37d]:fp.banks[b][3]=0x3f800000
  else:raise AssertionError(hex(w))
 @Vector
 def vector(ctx,w,fp):
  try:arithmetic(w,fp.contents,ve)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 count=0
 for owner,initial,present,event,collision,link,bank in itertools.product([0,7,255],[0,1,2,3,4,127,128,255],range(2),range(2),[0,1,2,3,255],range(3),range(2)):
  for mem in (ram,native):
   mem[0xa00000:0xa02000]=bytes(8192);mem[0xf00000-96:0xf00000]=bytes(96);mem[0xa08000:0xa08200]=bytes(512)
   struct.pack_into('<2I',mem,0xa00120,0x8ca00200,0x8ca00400);mem[0xa00200]=initial;mem[0xa00208]=owner
   struct.pack_into('<3I',mem,0xa00458,0x8ca00600,0x8ca00700,0x8ca00800)
   for k,a in enumerate([0xa00614,0xa00714,0xa00814]):struct.pack_into('<3I',mem,a,0x3f800000+k,0xbf800000+k,0x80000000)
   struct.pack_into('<I',mem,0x78c608+owner*4,0x8ca01200 if present else 0);struct.pack_into('<I',mem,0x78c628+owner*4,0x8ca01400);struct.pack_into('<I',mem,0x754c10+owner*52,0x100 if event else 0)
   struct.pack_into('<I',mem,0xa01218,0xffffffff);struct.pack_into('<2I',mem,0xa01238,0x8ca01600,0x8ca01500);mem[0xa01504]=collision
   struct.pack_into('<I',mem,0xa01420,0x7fffffff);struct.pack_into('<I',mem,0xa0160c,0x8ca01700);struct.pack_into('<3I',mem,0xa01708,0x3f800000,0xbf800000,0x80000000)
   struct.pack_into('<2I',mem,0xa0150c,0x7fffffff,0xffffffff);struct.pack_into('<3I',mem,0xa01514,0x3f800000,0xbf800000,0x80000000);struct.pack_into('<I',mem,0xa01520,0x8ca01800 if link else 0);struct.pack_into('<I',mem,0xa01820,0x8ca01900 if link==2 else 0)
   struct.pack_into('<3I',mem,0xa01914,1,2,3);struct.pack_into('<3I',mem,0xa01920,0x3f800000,0xbf800000,0x80000000)
   struct.pack_into('<I',mem,0x18b7ec,0x8ca01b00);struct.pack_into('<3I',mem,0xa01b20,0x3f800000,0xbf800000,0x80000000)
   struct.pack_into('<2I',mem,0x88f698,16,2);struct.pack_into('<I',mem,0x88f5f8,0x8ca08000);struct.pack_into('<I',mem,0x890070,event)
   for i in range(16):struct.pack_into('<I',mem,0x67c640+i*4,0x3f800000 if i%5==0 else 0)
  f=Fpu();f.fpscr=0x7f07c|(bank<<21);f.fpul=0x12345678
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*37
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[15]=0x8cf00000;s.pc=0xcb0101c;s.pr=0x8c000100;s.sr=0x700083f3 if owner&1 else 0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear();ve.clear();va.clear()
  services=[0x8c098786]
  reference(ram,s,{t:hook(t) for t in services},f,vector_instruction=lambda w,fp:arithmetic(w,fp,va))
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,vector)==0
  nf.fpul=fpul.value
  assert events==actual and ve==va and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(owner,initial,present,event,collision,link,bank)
  states.add(ram[0xa00200]);count+=1
 assert not errors,errors
 report=dict(cases_passed=count,module_sha256=hashlib.sha256(module).hexdigest(),source_sha256=hashlib.sha256((out/'source/tikal_effect_update.c').read_bytes()).hexdigest(),states_reached=sorted(states),services_visited=[hex(x) for x in sorted(targets)],scope='Full407-instruction effect callback with native primary/secondary angle updates, angle approach/channel-difference, camera vector transform, vector-to-angle (sqrt service, native atan2), vector add/subtract, axis rotation, matrix push/pop and direction transform, original bodies unhooked. Initialization, missing owner destruction, state/event/collision branches, nested child null/present, callback owner-table mutation, high/wrapped angle values, both banks, normalization. All regs/SR/PR/FPU/FPUL/stack/RAM/call order compared. Sqrt, task services and vector instruction numerical outputs injected. PR=SZ=0, finite vector scalar scope. ')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['atan2_full_native.c', 'sa_atan2_entry.c', 'sa_atan2_quadrants.c', 'atan_core_native.c', 'sa_atan_core.c', 'sa_atan_reduced.c', 'sa_atan_classifier.c', 'sa_atan_scaling.c', 'sh4_divide.c', 'sh4_float_i32.c', 'sh4_compare_bits.h','scalar_sqrt_wrapper.c','sa_camera_position.c','camera_position_native.c','camera_matrix_prepare.c','sa_vector_to_angles.c','sh4_truncate_i32.c','sa_vector_add.c','sa_vector_subtract.c','sh4_add_sub.c','sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','sa_angle_approach.c', 'sa_effect_primary_angles.c', 'sa_effect_secondary_angles.c', 'effect_angles_native.c', 'tikal_effect_update_native.c', 'tikal_effect_update.c', 'model_channel_difference.c', 'model_matrix_stack_push.c', 'model_matrix_stack_pop.c', 'model_direction_transform.c', 'sh4_multiply.c', 'sh4_fmac.c']}
 report['dll_sha256']={name:hashlib.sha256((ROOT/f'work/sonic-adventure/build/{name}.dll').read_bytes()).hexdigest() for name in ['tikal_effect_update_native']}
 (out/'analysis/tikal_effect_update_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'Tikal effect-update comparisons passed')
if __name__=='__main__':main()
