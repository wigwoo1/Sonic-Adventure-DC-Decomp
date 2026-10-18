import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from sh4_binary32 import multiply,multiply_add,truncate_i32
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')).sa_sound_decode_block_mono
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
for count,terminal,pattern,history,bank,coefficients,scale in itertools.product([1],[False],[0,16,112,128,240],[(0,0),(123,-456)],[0,1],[(0,0),(1000,-500),(-300,200)],[1,4,8192]):
    payload=bytearray()
    for block in range(2):payload+=(b'\x80\x00' if terminal and block==0 else (scale-1).to_bytes(2,'big'))+bytes(range(pattern,pattern+16))
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(payload)]=payload
    ram[0xa20000:0xa20100]=bytes([0x55])*256;struct.pack_into('<2h',ram,0xa30000,*history)
    struct.pack_into('<2i',ram,0xf00000,*coefficients)
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca10000,count,0x8ca20008,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c65b120;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    @Call
    def call(*args):raise AssertionError('Unexpected call')
    reference(ram,s,fpu=f,special=special)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert code==0,(count,terminal,pattern,history,code)
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    decoded=0 if terminal else count
    assert regs[0]==decoded and regs[15]==0x8cf00000
    assert native[0xa20000:0xa20008]==bytes([0x55])*8 and native[0xa20008+decoded*64:0xa20100]==bytes([0x55])*(248-decoded*64)
    assert native[0xa10000:0xa10000+len(payload)]==payload
    if terminal and count:assert native[0xa30000:0xa30004]==struct.pack('<2h',*history)
    newest,older=[raw(float(v*65536)) for v in history]
    c1,c2=[raw(v/4096.) for v in coefficients];samples=[]
    for block in range(decoded):
        for packed in payload[block*18+2:block*18+18]:
            for nibble in [packed>>4,packed&15]:
                signed=nibble-16 if nibble&8 else nibble
                prediction=multiply_add(c1,newest,multiply(c2,older)[0])[0]
                sample=multiply_add(raw(float(scale)),raw(float(signed*65536)),prediction)[0]
                samples.append((truncate_i32(sample)[0]>>16)&65535)
                older,newest=newest,sample
    assert native[0xa20008:0xa20008+decoded*64]==struct.pack('<'+'H'*len(samples),*samples)
    if not (terminal and count):
        expected_history=struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
        assert native[0xa30000:0xa30004]==expected_history
    cases+=1
(out/'analysis/sound_decode_mono_scales_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='180 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL mono block comparisons,no modeled calls. Scales1/4/8192,packed sample ranges00..0f/10..1f/70..7f/80..8f/f0..ff,two histories,three predictor pairs,both banks. Independent signed-nibble exact binary32 recurrence and saturating conversion checks every output sample and final history,source/output guards,return and stack. PR/SZ/RM0; original payloads,stereo,other modes/fullgame unverified.'),indent=2)+'\n')
print(cases,'mono decoder scale comparisons passed')
