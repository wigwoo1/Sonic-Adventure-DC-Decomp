import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert struct.unpack_from('<4H',binary,0x634c90)==(0xd206,0xe301,0x000b,0x2232)
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_odd_flag_set.dll')).sa_file_decode_odd_flag_set
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for initial,bank,sr0 in itertools.product([0,1,0x80000000,0xffffffff],[0,1],[0,0x70000063]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x673b74:0x673b80]=bytes([0x55])*12;struct.pack_into('<I',ram,0x673b78,initial)
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c644c90;s.pr=0x8c000100;s.sr=sr0
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(*args):raise AssertionError('Unexpected call')
    reference(ram,s,fpu=f)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert native[0x673b74:0x673b80]==bytes([0x55])*4+b'\x01\0\0\0'+bytes([0x55])*4 and regs[2]==0x8c673b78 and regs[3]==1
    cases+=1
(out/'analysis/file_decode_odd_flag_set_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='16 full retail RAM/CPU/FPU/SR/PR comparisons of4-instruction odd-decode flag setter644c90..644c98,no calls. Original opcodes checked,4 prior flag words,both banks,two SR values. Independent global673b78=1,neighbor guards,R2pointer/R3=1 and preserved stack/state. Dispatcher integration/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'odd decode flag comparisons passed')
