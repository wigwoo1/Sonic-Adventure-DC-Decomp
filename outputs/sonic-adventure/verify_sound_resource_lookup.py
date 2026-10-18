import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_resource_lookup.dll')).sa_sound_resource_lookup;fn.argtypes=abi
tags={0x42534d53:0x1e4,0x42504d53:0x264,0x42534f53:0x2e4,0x52535053:0x364,0x42504653:0x3e4,0x424f4653:0x464,0x57504653:0x46c}
cases=0
for mode,tag,selector,bank,alias in itertools.product(range(4),list(tags)+[0x12345678],[0,1,15,16,127,128,255],[0,0x200000],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,0 if mode==0 else 0x8ca08000)
 for off in range(0x1e4,0x474,4):struct.pack_into('<I',ram,0xa08000+off,0x99800000+off)
 ram[0xa20000:0xa20020]=bytes([0x55])*32
 output1=0 if mode==1 else 0x8ca20004;output2=0 if mode==2 else 0x8ca20004 if alias else 0x8ca20010
 before=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append(a);return -1
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[tag,selector,output1,output2];s.r[15]=0x8cf00000;s.pc=0x8c65a900;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and not calls
 signed=ctypes.c_int8(selector).value;expected=bytearray(before[0xa20000:0xa20020])
 code=0x0d000001 if mode==0 else 0x03000001 if mode in [1,2] else 0x08000003 if signed<0 or tag in tags and signed>=(16 if tag in list(tags)[:4] else 1) else 0x09000001 if tag not in tags else 0
 if mode==3 and code in [0,0x09000001]:
  off=tags.get(tag,0)+(signed*8 if tag in list(tags)[:4] else 0)
  first,second=(0,0xffffffff) if code else struct.unpack_from('<2I',before,0xa08000+off)
  struct.pack_into('<I',expected,output1-0x8ca20000,first);struct.pack_into('<I',expected,output2-0x8ca20000,second)
 assert r[0]==code and native[0xa20000:0xa20020]==expected
 cases+=1
assert cases==896
scope='896 full retail RAM/CPU/FPU/SR/PR comparisons of resource lookup65a900. Seven native tag mappings plus unknown tag;seven signed-index boundary fixtures,disabled/null outputs,distinct and aliased outputs,both banks. Independent tag-to-context offsets,index limits,eight-byte stride,error precedence,unknown-tag zero/ffffffff writes,alias second-write precedence,and32-byte guards. No modeled calls. Physical hardware and full game integration unverified.'
(p/'analysis/sound_resource_lookup_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'resource lookup comparisons passed')
