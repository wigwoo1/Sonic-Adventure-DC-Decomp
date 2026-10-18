import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/input_channel_records.dll';lib=ctypes.CDLL(str(dll));counts={};errors=[]
 @Call
 def unexpected(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
 for name,entry in [('state_get',0x8c647ca4),('event_set',0x8c647cb4),('request_set',0x8c647cc4)]:
  fn=getattr(lib,'sa_input_channel_'+name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for channel,value,bank in itertools.product(range(8),[0,1,0xffffffff],[0,0x200000]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;offset=0x894138+channel*128
   ram[0x894138:0x894538]=bytes([0x55])*1024;struct.pack_into('<I',ram,offset+4,value);struct.pack_into('<3I',ram,0xf00000,value,value^0xa5a5a5a5,0x12345678)
   native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[channel,value,value^0x87654321,0xabcdef01];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
   reference(ram,s,{},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,unexpected)==0
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and bytes(nf)==bytes(f)
   assert r[4]==0x8c894138+channel*128
   if name=='state_get':assert r[0]==value
   elif name=='event_set':assert struct.unpack_from('<I',native,offset+8)[0]==value
   else:
    assert struct.unpack_from('<6I',native,offset+0x14)==(value,value^0x87654321,0xabcdef01,value,value^0xa5a5a5a5,0x12345678)
    assert struct.unpack_from('<I',native,offset+4)[0]==1
   count+=1
  counts[name]=count
 assert not errors
 report=dict(cases_passed=counts,instruction_counts={'state_get':8,'event_set':8,'request_set':18},binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={name:hashlib.sha256((out/'source'/('sa_input_channel_'+name+'.c')).read_bytes()).hexdigest() for name in counts},scope='Retail full-state comparisons with no injected calls; independent 128-byte channel stride, state, event and six request argument checks. Channel processing/backend interpretation incomplete.')
 (out/'analysis/input_channel_records_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'input channel record comparisons passed')
if __name__=='__main__':main()
