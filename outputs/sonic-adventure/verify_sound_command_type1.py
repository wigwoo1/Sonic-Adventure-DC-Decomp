import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_commit.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for a,n in [(0x8c10cd44,'sa_engine_record_copy_loaded_dispatch'),(0x8c10cd82,'sa_sound_request_copy20'),(0x8c65acac,'sa_engine_error_record')]:
  h=getattr(lib,n);h.argtypes=abi;functions[a]=h
 functions[0x8c65a464]=lib.sa_sound_request_commit;functions[0x8c65a464].argtypes=abi
 wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_command_type1.dll'))
 fn=wrapper.sa_sound_command_type1;fn.argtypes=abi;cases=0
 for present,valid,pending,cursor,bank,mask,identifier,value,channel in itertools.product([0,1],[0,1],[0,256],[0,255],[0,0x200000],[0],[0,255],[0,0xffffffff],[-1,0,15,16,128,256]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca03000 if present else 0)
  struct.pack_into('<5I',ram,0xa03164,0x8ca04000,256,pending,cursor,7)
  data=bytearray((i*13+7)&255 for i in range(20));ram[0x890614:0x890628]=data;ram[0xa02001]=identifier
  channel_byte=channel&255;channel_valid=channel_byte<16
  if present and valid and channel_valid:
   struct.pack_into('<I',data,0,1);data[4:8]=bytes([identifier,value&255,((value>>8)&255),(channel_byte<<3)&255])
  ram[0xa04000:0xa05c00]=bytes([0x55])*0x1c00
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   return functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca02000 if valid else 0;s.r[5]=value;s.r[6]=value>>8;s.r[7]=channel&0xffffffff;s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c643b08
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  success=present and valid and channel_valid and pending!=256
  assert r[0]==(0 if success else 0x0d000001 if not present else 0x07000103 if not valid else 0x03000001 if not channel_valid else 0x0a000001)
  assert struct.unpack_from('<3I',native,0xa0316c)==(pending+int(success),(cursor+1)%256 if success else cursor,7)
  expected=bytearray([0x55])*0x1c00
  if success:expected[cursor*28:cursor*28+20]=data;struct.pack_into('<I',expected,cursor*28+20,1)
  assert native[0xa04000:0xa05c00]==expected
  assert native[0x890614:0x890628]==data
  assert ((sr.value>>4)&15)==mask
  cases+=1
 (root/'analysis/sound_command_type1_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native 53-instruction type-1 sound command wrapper calling recovered producer/copy/error routines. Full retail RAM/CPU/FPU/SR/PR/MACL equality plus payload, flag, producer cursor wrap, pending count, full queue/null context errors and interrupt mask restoration. Independent low-byte parameters, signed channel-byte validation and shifted channel, preserved command scratch contents, pool record payload, flags and counters; null context/object and full queue paths. Fixture context, no audio playback.'},indent=2)+'\n')
 print(cases,'type-1 sound command comparisons passed')
if __name__=='__main__':main()
