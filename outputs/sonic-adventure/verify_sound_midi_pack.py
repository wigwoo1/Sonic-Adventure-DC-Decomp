import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll'))
 support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for address,name in [(0x8c65a2fc,'sa_engine_frame_debug_log'),(0x8c65acac,'sa_engine_error_record')]:
  helper=getattr(support,name);helper.argtypes=abi;functions[address]=helper
 fn=lib.sa_sound_midi_pack;fn.argtypes=abi;cases=0
 for present,valid,status,identifier,note,value,bank in itertools.product([0,1],[0,1],[0x80,0x9f,0xa3,0xb7,0xc2,0xd4,0xe8,0xff],[0,15],[0,255],[0,255],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca03000 if present else 0)
  ram[0xa00001]=identifier;ram[0xa02000:0xa02008]=bytes([0x55])*8
  struct.pack_into('<3I',ram,0xf00000,status,note,value)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   return functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000 if valid else 0;s.r[5]=0x8ca02000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c643c18
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  expected=bytearray([0x55])*8
  if present and valid:
   if status&0xf0==0xf0:expected[:4]=bytes(4)
   else:expected[:4]=bytes([0 if status&0xf0 in (0xc0,0xa0) else value,note,(status&15)|(identifier<<4),(status>>4)&7])
  assert native[0xa02000:0xa02008]==expected,(status,identifier,note,value,native[0xa02000:0xa02008],expected)
  assert r[0]==(0x0d000001 if not present else 0x07000103 if not valid else 0)
  cases+=1
 (root/'analysis/sound_midi_pack_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native 162-instruction MIDI packer, debug-disabled and native error helpers; all high-nibble message classes 8..F, stack varargs, data0/255 and channel0/15, null object/context, FPU banks. Full retail RAM/CPU/FPU/SR/PR/MACL equality and independent packed bytes/preserved neighbors/errors. Logging formatter and physical sound execution not covered.'},indent=2)+'\n')
 print(cases,'MIDI packing comparisons passed')
if __name__=='__main__':main()
