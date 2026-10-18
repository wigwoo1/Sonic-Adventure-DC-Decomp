import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_swap_words.dll')).sa_input_response_word_swap;fn.argtypes=abi
cases=0
for count,pattern,bank,sr0 in itertools.product([1,2,8,64],[0,1,2],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 original=bytes(((i*37+pattern*91)&255) for i in range(count*4))
 ram[0xa08000:0xa08000+len(original)]=original
 ram[0xa08000+len(original):0xa08000+len(original)+8]=b'neighbor'
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  raise AssertionError(('unexpected service',a))
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=count;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c668b24
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 expected=b''.join(original[i:i+4][::-1] for i in range(0,len(original),4))
 assert native[0xa08000:0xa08000+len(original)+8]==expected+b'neighbor'
 assert r[0]==0 and r[4]==0x8ca08000+4*count and r[5]==0
 cases+=1
(out/'analysis/sound_driver_swap_words_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native10-instruction in-place32-bit byte swap668b24. Full retail RAM/CPU/FPU/SR/PR comparison and independent byte reversal, neighboring bytes and final count/pointer/return. Positive counts1/2/8/64, three deterministic byte patterns, both FPU banks/SR masks. Zero count has no initial guard and is excluded; full game unverified.'),indent=2)+'\n')
print(cases,'word swap comparisons passed')
