import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/input_channel_callbacks.dll';lib=ctypes.CDLL(str(dll));counts={}
 for name,entry,slot in [('callback',0x8c647bb8,0x74),('secondary_callback',0x8c647bde,0x78)]:
  fn=getattr(lib,'sa_input_channel_'+name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for channel,present,payload,bank in itertools.product(range(8),[0,1],[0,0xabcdef01],[0,0x200000]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;offset=0x894138+channel*128
   struct.pack_into('<I',ram,offset+slot,0x8c010200 if present else 0);struct.pack_into('<I',ram,offset+0x7c,payload)
   native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
   def effect(mem,r,sr,pr):
    trace=(tuple(r[4:8]),r[14],r[15],pr);struct.pack_into('<I',mem,offset+slot,0)
    for i in range(4):r[i]=0x34560000+i
    return trace,sr^0x31
   @Call
   def call(ctx,t,m,f,sr,r,pr):
    if t!=0x8c010200:errors.append(hex(t));return -9
    trace,sr[0]=effect(native,r,sr[0],pr[0]);actual.append(trace);return 0
   def hook(mem,r,sr):trace,sr=effect(mem,r,sr,s.pr);expected.append(trace);return sr
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
   reference(ram,s,{0x8c010200:hook},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
   assert not errors and len(actual)==present
   if present:assert actual[0]==((channel,0x22340005,0x22340006,payload if slot==0x74 else 0x22340007),0x2234000e,0x8cf00000,0x8c000100)
   else:assert r[0]==slot
   count+=1
  counts[name]=count
 report=dict(cases_passed=counts,instruction_counts={'callback':19,'secondary_callback':17},binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={name:hashlib.sha256((out/'source'/('sa_input_channel_'+name+'.c')).read_bytes()).hexdigest() for name in counts},scope='Full retail state comparisons; independent nullable callback slots, userdata forwarding and restored stack/PR/r14 at tail call. Callback implementations injected.')
 (out/'analysis/input_channel_callbacks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'input channel callback comparisons passed')
if __name__=='__main__':main()
