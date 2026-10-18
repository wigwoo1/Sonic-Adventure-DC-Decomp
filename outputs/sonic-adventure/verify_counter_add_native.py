import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/stage_position_restore_saved.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[0x8c0940da,0x8c09852e]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[0],r[1],r[15],pr)
  if target==0x8c10ca5c:
   num=r[1] if r[1]<0x80000000 else r[1]-0x100000000
   den=r[0] if r[0]<0x80000000 else r[0]-0x100000000
   quotient=abs(num)//abs(den);r[0]=(-quotient if (num<0)!=(den<0) else quotient)&0xffffffff
  else:r[0]=alias&1;r[1]=0x12340001;r[2]=0x12340002;r[3]=0x12340003
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);actual.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);expected.append(trace);return sr
  return body
 for name,entry in [('sa_counter_add_native',0x8c08a198)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product([0,1,99,100,0xffff,0x10001,0xffffffff,0x80000000],[0,99,100,32767,32768,65535],[0,1,2,3],[0,0x200000,0x380003]):
   for mem in [ram,native]:
    mem[0xa00000:0xa00400]=bytes(1024);mem[0xefffc0:0xf00000]=bytes(64)
    struct.pack_into('<H',mem,0x78c590,stage);mem[0x78c592:0x78c595]=bytes([255,128,7]);struct.pack_into('<I',mem,0x78c598,0xdeadbeef)
    for off in [0x78c59c,0x78c5a8,0xa00100,0xa00200]:struct.pack_into('<3I',mem,off,0x12345678,0x80000000,0xffffffff)
   for mem in [ram,native]:
    struct.pack_into('<H',mem,0x752be4,stage);struct.pack_into('<H',mem,0x7493ba,26 if alias&1 else 1);mem[0x752be0]=127 if alias&2 else 0
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=[0x8ca00100,0x8c78c59c,0x8c78c5a0,0x8ca00100][alias];s.r[5]=[0x8ca00200,0x8c78c5a8,0x8c78c5ac,0x8ca00100][alias];s.r[4]=variant;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   assert struct.unpack_from('<H',native,0x752be4)[0]==(stage+variant)&0xffff
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in ['sa_signed_division.c','stage_position_restore_saved.c','sa_counter_add.c','sa_counter_award.c','sa_stage_mode_query.c']},cases_passed=counts,instruction_count=41,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source/sa_counter_add.c').read_bytes()).hexdigest() for n in counts},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Whole counter addition body, signed16 pre/post values and wrapped16-bit store, hundred-boundary comparison, subordinate award dispatch, conditional task allocation tail call, stack restoration. Independent counter formula plus full registers/SR/PR/FPU/FPUL/stack/RAM and exact service args/order against retail instructions. Award and mode query execute native recovered bodies; Signed division native; sound/allocator services injected. Actual award-byte writes and allocator branch behavior compared.')
 (out/'analysis/counter_add_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'counter addition comparisons passed')
if __name__=='__main__':main()
