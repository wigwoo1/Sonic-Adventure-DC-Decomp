import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_info.dll'))
fn=lib.sa_sound_driver_info;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
logger=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_frame_debug_log;logger.argtypes=abi
cases=0
for bank,mask,version in itertools.product([0,0x200000],[0,7,15],[(1,0),(1,1),(0,0)]):
 result=0
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 struct.pack_into('<I',ram,0x67c1d0,0x8cb08000)
 def lit(a):return struct.unpack_from('<h',binary,a-0x8c010000)[0]
 struct.pack_into('<I',ram,0xb08000+lit(0x8c65ab62),0xa0801000)
 values={0xa0801020:version[0],0xa0801021:version[1],0xa0801022:0x7f,0xa0801023:0x80}
 word_pairs=[(0x8c65ac6a,0x8c65ac68),(0x8c65ac6e,0x8c65ac6c),(0x8c65ac72,0x8c65ac70),(0x8c65ac76,0x8c65ac74),(0x8c65ac7a,0x8c65ac78),(0x8c65ac7e,0x8c65ac7c)]
 for i,(a,b) in enumerate(word_pairs):values[0xa0801020+lit(a)]=0x89abcdef+i
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(t,m,r,events):
  n=1 if t==0x8c667188 else 4
  events.append((t,r[4],r[5]));v=values[r[4]];at=r[5]&0xffffff
  for i in range(n):m[at+i]=(v>>(8*i))&255
  r[0]=0
 hooks={}
 for t in [0x8c667188,0x8c667150]:
  def hook(m,r,sr,t=t):service(t,m,r,expected);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c65a2fc:return logger(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t in hooks:
   try:service(t,m,r,actual)
   except Exception as exc:errors.append(repr(exc));return -9
   return 0
  errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65aaf4;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<2I',native,0x67c1c8)==(0xabcdef01,0x12345678)
 third=0x8c65ab68 if version==(1,0) else 0x8c65ac64
 wanted=[(0x8c667188,0xa0801020,0x8cb08000+lit(0x8c65ab64)),(0x8c667188,0xa0801021,0x8cb08000+lit(0x8c65ab66)),(0x8c667188,0xa0801023 if version==(1,0) else 0xa0801022,0x8cb08000+lit(third))]
 wanted += [(0x8c667150,0xa0801020+lit(a),0x8cb08000+lit(b)) for a,b in word_pairs]
 assert actual==wanted
 for t,a,b in wanted:
  n=1 if t==0x8c667188 else 4
  assert int.from_bytes(native[b&0xffffff:(b&0xffffff)+n],'little')==values[a]
 assert struct.unpack_from('<I',native,0xb08000+lit(0x8c65ab62)-28)[0]==0xa0801020
 assert native[0xb08000+lit(0x8c65ac66)]==0
 cases+=1
report={'cases_passed':cases,'instructions_recovered':167,'scope':'Full original RAM/CPU/SR/PR/FPU/getter-event equality for context present, three version combinations/both FPU banks/three interrupt masks. Native disabled logger; independent info base, three byte/six word read sources/destinations, version branch, values, reset byte, zero return, stack/R14. Byte/word getters modeled; hardware unverified.'}
(out/'analysis/sound_driver_info_context_present_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'driver info context comparisons passed')
