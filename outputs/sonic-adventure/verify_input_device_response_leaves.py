import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure'
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 counts={}
 for name,entry in [('presence_decode',0x8c65ff12),('response_header',0x8c65ff5a)]:
  fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/input_device_{name}.dll')),f'sa_input_device_{name}')
  fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
  count=0
  for mode,command,status,bank in itertools.product([0,1,2],[0,1,4],[0,5,8,0xdf,0xe0,0xfe,0xff],[0,0x200000]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
   struct.pack_into('<I',ram,0x8a83b4,0x8ca10000)
   struct.pack_into('<I',ram,0xa110c4,mode)
   ram[0xa20000:0xa2002c]=bytes([0x55])*44
   ram[0xa20007]=command;ram[0xa20008]=status
   header=bytes([status,0x23,0x45,0x67])
   ram[0xa30000:0xa30004]=header[::-1] if mode==1 else header
   struct.pack_into('<I',ram,0xa30004,0x12345678)
   native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
   errors=[]
   @Call
   def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
   s=State();s.r[:]=[0x22340000+i for i in range(16)]
   s.r[4]=0x8ca20000;s.r[5]=0x8ca30000;s.r[15]=0x8cf00000
   s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr)
   fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
   reference(ram,s,fpu=f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert not errors
   assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   if name=='presence_decode':
    expected=int(status==5) if command==1 else int(status not in [0,255])
    assert native[0xa20000]==expected
   else:
    assert native[0xa20008:0xa2000c]==header
    expected=0xffffffff if status==255 else (0x12345678 if status>=0xe0 else 0)
    assert struct.unpack_from('<I',native,0xa20014)[0]==expected
   count+=1
  counts[name]=count
 result=dict(cases_passed=sum(counts.values()),routines=counts,scope='Complete RAM/CPU/FPU comparisons against retail instructions plus independent presence, header byte order and response-value checks. No injected game calls. Does not verify primary/subdevice responses or the full completion routine.')
 (out/'analysis/input_device_response_leaves_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(counts,'response leaf comparisons passed')

if __name__=='__main__':main()
