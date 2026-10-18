import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_magic_extension_word.dll')).sa_sound_magic_extension_word;fn.argtypes=abi
cases=0
for magic,size,offset,value,bank in itertools.product([0x8000,0x8001],[17,18,0xffffffff],[13,14,65535],[0,0x8001,65535],[0,0x200000]):
 data=struct.pack('>HH12xH',magic,offset,value)
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10012]=data;ram[0xa20000:0xa20008]=bytes([0x55])*8
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca10000,size,0x8ca20000];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65af64
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(*args):raise AssertionError('Unexpected service')
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 valid=size==18 and magic==0x8000 and offset>13
 expected=value.to_bytes(2,'little')+bytes([0x55])*6 if valid else bytes([0x55])*8
 assert native[0xa20000:0xa20008]==expected and r[0]==(0 if valid else 0xfffffffe if size==18 and magic!=0x8000 else 0xffffffff)
 assert native[0xa10000:0xa10012]==data;cases+=1
(out/'analysis/sound_magic_extension_word_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native51-instruction magic header extension-word getter65af64,no calls.108 full retail RAM/CPU/FPU/SR/PR/MACL cases. Magic8000/8001,signed size17/18/ffffffff,BE header offset13/14/ffff,BE extension word0/8001/ffff,banks. Independent size/signature/offset rejection,preserved output/guards,BE16 read at16 toLE output and source preservation. Extension field semantic meaning/initializer integration/actual assets/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'magic extension word comparisons passed')
