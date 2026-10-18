import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from sh4_binary32 import multiply,multiply_add,truncate_i32
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
kernel=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')).sa_sound_decode_block_stereo
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_process.dll')).sa_file_decode_request_process
flag=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_odd_flag_set.dll')).sa_file_decode_odd_flag_set
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
kernel.argtypes=fn.argtypes;flag.argtypes=fn.argtypes
cases=0
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
assets=[a for a in json.loads((out/'analysis/magic_audio_endings_inventory.json').read_text())['assets'] if a['first_word_after_end']=='0x8001']
for asset,terminal_position,history,bank,coefficients in itertools.product(assets,[0,2],[(0,0),(123,-456)],[0,1],[(0,0),(1000,-500),(-300,200)]):
    count=4;pattern=0
    path=ROOT/asset['path']
    with path.open('rb') as file:
        file.seek(asset['declared_encoded_end']-36);final_pair=file.read(36);marker=file.read(18)
    assert final_pair.hex()==asset['final_pair_hex'] and marker[:2]==b'\x80\x01'
    assert marker.hex()==asset['after_declared_end_hex'][:36]
    # Bytes after the terminating block are explicit guard data, never decoded.
    payload=(final_pair+marker+bytes(18)) if terminal_position==2 else marker+bytes(54)
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(payload)]=payload
    ram[0xa20000:0xa20100]=bytes([0x55])*256;struct.pack_into('<2h',ram,0xa30000,*history)
    ram[0xa40000:0xa40100]=bytes([0x55])*256;struct.pack_into('<2h',ram,0xa50000,-321,654)
    struct.pack_into('<2I2i',ram,0xf00000,0x8ca40008,0x8ca50000,*coefficients)
    ram[0xa60000:0xa60040]=bytes([0x55])*64
    struct.pack_into('<6I',ram,0xa6000c,1,0x55555555,2,0x8ca10000,count,0x8ca20008)
    struct.pack_into('<I4h2h',ram,0xa60024,0x8ca40008,*history,-321,654,*coefficients)
    ram[0x673b74:0x673b80]=bytes([0x55])*12;struct.pack_into('<I',ram,0x673b78,0x12345678)
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca60000;s.r[15]=0x8cf00000;s.pc=0x8c644cb0;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target={0x8c65b238:kernel,0x8c644c90:flag}[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,fpu=f,special=special)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert code==0,(count,terminal_position,pattern,history,code)
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    decoded=terminal_position
    left_blocks=(decoded+1)//2;right_blocks=decoded//2
    assert struct.unpack_from('<2I',native,0xa6000c)==(3,decoded) and regs[15]==0x8cf00000
    wanted=1 if decoded&1 else 0x12345678
    assert native[0x673b74:0x673b80]==bytes([0x55])*4+wanted.to_bytes(4,'little')+bytes([0x55])*4
    assert native[0xa60028:0xa60030]==struct.pack('<4h',*history,-321,654)
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
                    scale=(int.from_bytes(payload[block*18:block*18+2],'big')&8191)+1
                    sample=multiply_add(raw(float(scale)),raw(float(signed*65536)),prediction)[0]
                    samples.append((truncate_i32(sample)[0]>>16)&65535)
                    older,newest=newest,sample
        assert native[destination:destination+len(samples)*2]==struct.pack('<'+'H'*len(samples),*samples)
        if False:
            expected_history=struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
            assert native[histaddr:histaddr+4]==expected_history
    cases+=1
(out/'analysis/original_audio_terminal_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='2592 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL native file dispatcher/stereo decoder/global flag comparisons using actual terminal block8001 from108 original magic8000 files. Terminal block first or after actual final stereo pair,two controlled predictor histories,three coefficient pairs,both FPU banks. No modeled calls. Independent every emitted sample using original per-block scales,phase3/result0or2,unchanged stored histories on termination,complete outputs/source/global guards. Original tail bytes and pair hashes matched inventory. Starting histories/coefficients controlled,not reconstructed from full preceding files. Final valid-sample trimming,SSRACING inconsistent header,looping,fullpayload/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'original terminal comparisons passed')
