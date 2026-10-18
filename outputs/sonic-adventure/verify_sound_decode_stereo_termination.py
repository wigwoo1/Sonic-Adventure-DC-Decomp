import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from sh4_binary32 import multiply,multiply_add,truncate_i32
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')).sa_sound_decode_block_stereo
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
for count,terminal_position,pattern,history,bank,coefficients in itertools.product([4],range(4),[0,120],[(0,0),(123,-456)],[0,1],[(0,0),(1000,-500),(-300,200)]):
    payload=bytearray()
    for block in range(4):payload+=(b'\x80\x00' if block==terminal_position else b'\x00\x03')+bytes(v^(128 if block&1 else 0) for v in range(pattern,pattern+16))
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(payload)]=payload
    ram[0xa20000:0xa20100]=bytes([0x55])*256;struct.pack_into('<2h',ram,0xa30000,*history)
    ram[0xa40000:0xa40100]=bytes([0x55])*256;struct.pack_into('<2h',ram,0xa50000,-321,654)
    struct.pack_into('<2I2i',ram,0xf00000,0x8ca40008,0x8ca50000,*coefficients)
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca10000,count,0x8ca20008,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c65b238;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    @Call
    def call(*args):raise AssertionError('Unexpected call')
    reference(ram,s,fpu=f,special=special)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert code==0,(count,terminal_position,pattern,history,code)
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    decoded=terminal_position
    left_blocks=(decoded+1)//2;right_blocks=decoded//2
    assert regs[0]==decoded and regs[15]==0x8cf00000
    assert native[0xa20000:0xa20008]==bytes([0x55])*8 and native[0xa20008+left_blocks*64:0xa20100]==bytes([0x55])*(248-left_blocks*64)
    assert native[0xa10000:0xa10000+len(payload)]==payload
    if True:assert native[0xa30000:0xa30004]==struct.pack('<2h',*history)
    assert native[0xa40000:0xa40008]==bytes([0x55])*8 and native[0xa40008+right_blocks*64:0xa40100]==bytes([0x55])*(248-right_blocks*64)
    if True:assert native[0xa50000:0xa50004]==struct.pack('<2h',-321,654)
    for channel,initial,destination,histaddr in [(0,history,0xa20008,0xa30000),(1,(-321,654),0xa40008,0xa50000)]:
        newest,older=[raw(float(v*65536)) for v in initial]
        c1,c2=[raw(v/4096.) for v in coefficients];samples=[]
        for pair in range(left_blocks if channel==0 else right_blocks):
            block=pair*2+channel
            for packed in payload[block*18+2:block*18+18]:
                for nibble in [packed>>4,packed&15]:
                    signed=nibble-16 if nibble&8 else nibble
                    prediction=multiply_add(c1,newest,multiply(c2,older)[0])[0]
                    sample=multiply_add(raw(4.),raw(float(signed*65536)),prediction)[0]
                    samples.append((truncate_i32(sample)[0]>>16)&65535)
                    older,newest=newest,sample
        assert native[destination:destination+len(samples)*2]==struct.pack('<'+'H'*len(samples),*samples)
        if False:
            expected_history=struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
            assert native[histaddr:histaddr+4]==expected_history
    cases+=1
(out/'analysis/sound_decode_stereo_termination_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='96 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL stereo termination comparisons,no modeled calls. Termination at each of four alternating left/right blocks,two packed patterns,two left histories,distinct right history,three coefficient pairs,both banks. Independent per-channel recurrence checks all samples emitted before marker,left/right output extents,return marker index,source/guards and unchanged stored histories despite partial output. Scale4,PR/SZ/RM0;other scales,complete original streams/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stereo decoder termination comparisons passed')
