import ctypes,itertools,json,struct,math,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from sh4_binary32 import multiply,multiply_add,truncate_i32,divide,add_sub
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
kernel=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')).sa_sound_decode_block_stereo
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_process.dll')).sa_file_decode_request_process
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
kernel.argtypes=fn.argtypes
setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_stereo_request.dll')).sa_file_decode_stereo_request
activate=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_activate.dll')).sa_file_decode_request_activate
setup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def trig_oracle(angle):
    x=angle&0x7fffffff;index=int(struct.unpack('<f',struct.pack('<I',divide(x,0x3f490fdb)[0]))[0])
    remainder=add_sub(x,multiply(raw(float(index)),0x3f490fdb)[0],True)[0];octant=index&7
    argument=add_sub(remainder,0xbf490fdb)[0] if octant&1 else remainder
    cosine=octant in [0,3,4,7];table=0x8c160444 if cosine else 0x8c160460
    coefficients=struct.unpack_from('<7I',binary,table-0x8c010000);square=multiply(argument,argument)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if not cosine:answer=multiply(argument,answer)[0]
    if octant in [1,2,3,4]:answer^=0x80000000
    return struct.unpack('<f',struct.pack('<I',answer))[0]
assets=json.loads((out/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[];cases=0
for asset,bank in itertools.product(assets,[0,1]):
    path=ROOT/asset['path']
    with path.open('rb') as file:
        header=file.read(64);offset=struct.unpack_from('>H',header,2)[0]+4
        file.seek(offset);payload=file.read(288)
    assert len(payload)==288 and header.hex()==asset['header_hex'] and header[7]==2
    pattern=0
    count=16;terminal=bool(payload[0]&128);history=(0,0)
    scale=(int.from_bytes(payload[:2],'big')&8191)+1
    rate=struct.unpack_from('>I',header,8)[0];extension=struct.unpack_from('>h',header,16)[0]
    pi2=struct.unpack('<f',struct.pack('<I',0x40c90fda))[0]
    angle=raw(f32(f32(f32(float(extension))*pi2)/f32(float(rate))));trig=trig_oracle(angle)
    root2=f32(math.sqrt(2.));center=f32(root2-trig);denominator=f32(root2-1.)
    coefficient=f32(f32(center-f32(math.sqrt(f32(f32(center-denominator)*f32(center+denominator)))))/denominator)
    coefficients=(ctypes.c_int16(int(f32(coefficient*8192.))&65535).value,ctypes.c_int16(int(-f32(f32(coefficient*coefficient)*4096.))&65535).value)
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(payload)]=payload
    ram[0xa20000:0xa20300]=bytes([0x55])*768;struct.pack_into('<2h',ram,0xa30000,*history)
    ram[0xa40000:0xa40300]=bytes([0x55])*768;struct.pack_into('<2h',ram,0xa50000,0,0)
    struct.pack_into('<2I2i',ram,0xf00000,0x8ca40008,0x8ca50000,*coefficients)
    ram[0xa60000:0xa60040]=bytes([0x55])*64
    struct.pack_into('<I',ram,0xa6000c,0)
    struct.pack_into('<4h2h',ram,0xa60028,0,0,0,0,*coefficients)
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca60000,0x8ca10000,count,0x8ca20008];s.r[15]=0x8cf00000;s.pc=0x8c644cb0;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address==0x8c65b238,address
        return kernel(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    for entry,function in [(0x8c644c38,setup),(0x8c644c56,activate)]:
        s.pc=entry
        reference(ram,s,fpu=f,special=special)
        assert function(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        nf.fpul=fpul.value
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
        if entry==0x8c644c38:
            assert regs[0]==1 and struct.unpack_from('<5I',native,0xa60014)==(2,0x8ca10000,count,0x8ca20008,0x8ca40008)
        else:assert struct.unpack_from('<2I',native,0xa6000c)==(1,0)
    s.pc=0x8c644cb0
    reference(ram,s,fpu=f,special=special)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert code==0,(count,terminal,pattern,history,code)
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    decoded=0 if terminal else count
    channel_blocks=decoded//2
    assert struct.unpack_from('<2I',native,0xa6000c)==(3,decoded) and regs[15]==0x8cf00000
    assert native[0xa20000:0xa20008]==bytes([0x55])*8 and native[0xa20008+channel_blocks*64:0xa20300]==bytes([0x55])*(760-channel_blocks*64)
    assert native[0xa10000:0xa10000+len(payload)]==payload
    if terminal and count:assert native[0xa30000:0xa30004]==struct.pack('<2h',*history)
    assert native[0xa40000:0xa40008]==bytes([0x55])*8 and native[0xa40008+channel_blocks*64:0xa40300]==bytes([0x55])*(760-channel_blocks*64)
    if terminal and count:assert native[0xa50000:0xa50004]==struct.pack('<2h',0,0)
    for channel,initial,destination,histaddr in [(0,history,0xa20008,0xa60028),(1,(0,0),0xa40008,0xa6002c)]:
        newest,older=[raw(float(v*65536)) for v in initial]
        c1,c2=[raw(v/4096.) for v in coefficients];samples=[]
        for pair in range(channel_blocks):
            block=pair*2+channel
            scale=(int.from_bytes(payload[block*18:block*18+2],'big')&8191)+1
            for packed in payload[block*18+2:block*18+18]:
                for nibble in [packed>>4,packed&15]:
                    signed=nibble-16 if nibble&8 else nibble
                    prediction=multiply_add(c1,newest,multiply(c2,older)[0])[0]
                    sample=multiply_add(raw(float(scale)),raw(float(signed*65536)),prediction)[0]
                    samples.append((truncate_i32(sample)[0]>>16)&65535)
                    older,newest=newest,sample
        assert native[destination:destination+len(samples)*2]==struct.pack('<'+'H'*len(samples),*samples)
        if not (terminal and count):
            expected_history=struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
            assert native[histaddr:histaddr+4]==expected_history
    records.append(dict(path=asset['path'],offset=offset,pair_sha256=hashlib.sha256(payload).hexdigest(),bank=bank,coefficients=coefficients,decoded_blocks=decoded))
    cases+=1
(out/'analysis/file_decode_request_sequence_verification.json').write_text(json.dumps(dict(cases_passed=cases,original_files=len(assets),assets=records,scope='218 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL native stereo request setup, activation, file dispatcher and stereo decoder sequence comparisons using first288-byte encoded prefix (eight consecutive pairs) from109 original two-channel magic8000 files,both banks. Original data offsets,header-derived predictor coefficients,distinct original left/right bytes and per-block scales. Independent per-channel exact binary32 signed-nibble recurrence/saturating conversion checks every sample/final history. Pair hashes,source/guards,persistent RAM/register sequence idle setup->activation phase1->decode phase3/result storage,stack/return checks,no modeled calls. Zero starting histories;pairs beyond prefix,fullpayload,odd-count/termination branches,physical playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'native audio request sequence comparisons passed')
