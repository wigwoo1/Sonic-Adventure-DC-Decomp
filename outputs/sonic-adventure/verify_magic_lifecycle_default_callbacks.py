import ctypes,itertools,json,struct,math,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from sh4_binary32 import multiply,multiply_add,truncate_i32,divide,add_sub
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
kernel=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')).sa_sound_decode_block_stereo
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_update_dispatch.dll')).sa_sound_stream_update_dispatch
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
kernel.argtypes=fn.argtypes
def extra(dll,name):
    target=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);target.argtypes=fn.argtypes;return target
decoder=extra('file_decode_request_process.dll','sa_file_decode_request_process')
magic=extra('sound_stream_magic_update.dll','sa_sound_stream_magic_update')
phase_get=extra('file_decode_phase_get.dll','sa_file_decode_phase_get')
complete=extra('sound_stream_decode_complete.dll','sa_sound_stream_decode_complete')
result_get=extra('file_decode_result_get.dll','sa_file_decode_result_get')
compact=extra('sound_stream_stereo_compact.dll','sa_sound_stream_stereo_compact')
sample_copy=extra('sound_stream_sample_copy.dll','sa_sound_stream_sample_copy')
input_set=extra('sound_stream_input_callback_set.dll','sa_sound_stream_input_callback_set')
output_set=extra('sound_stream_output_callback_set.dll','sa_sound_stream_output_callback_set')
start_stream=extra('sound_stream_start.dll','sa_sound_stream_start')
output_default=extra('sound_stream_default_output.dll','sa_sound_stream_default_output')
input_default=extra('sound_stream_default_input.dll','sa_sound_stream_default_input')
input_provider=extra('sound_stream_input_provider.dll','sa_sound_stream_input_provider')
channel_get=extra('sound_stream_channel_count_get.dll','sa_sound_stream_channel_count_get')
position_get=extra('sound_stream_position_get.dll','sa_sound_stream_position_get')
buffer_get=extra('sound_source_buffer_get.dll','sa_sound_source_buffer_get')
query=extra('sound_source_query.dll','sa_sound_source_query')
lock_enter=extra('sound_source_lock_enter.dll','sa_sound_source_lock_enter')
lock_exit=extra('sound_source_lock_exit.dll','sa_sound_source_lock_exit')
setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_stereo_request.dll')).sa_file_decode_stereo_request
activate=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_activate.dll')).sa_file_decode_request_activate
setup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes
request=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_decode_schedule.dll')).sa_sound_stream_decode_schedule;request.argtypes=fn.argtypes
wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_stereo_request.dll')).sa_sound_stream_stereo_request;wrapper.argtypes=fn.argtypes
division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/signed_division.dll')).sa_signed_division;division.argtypes=fn.argtypes
remainder=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/integer_remainder.dll')).sa_integer_remainder;remainder.argtypes=fn.argtypes
ack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_completion_acknowledge.dll')).sa_file_decode_completion_acknowledge
ack.argtypes=fn.argtypes
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
    ram[0xa70000:0xa70100]=bytes([0x66])*256;struct.pack_into('<I',ram,0xa70008,0x8ca60000);struct.pack_into('<I',ram,0xa70050,0x8ca10000);struct.pack_into('<I',ram,0xa70064,0x8ca20008);struct.pack_into('<2I',ram,0xa7006c,65536,0);
    for field,value in [(0x54,16),(0x58,2),(0x60,32),(0x68,256),(0x74,256),(0x78,256)]:struct.pack_into('<I',ram,0xa70000+field,value)
    struct.pack_into('<I',ram,0xa70004,0);struct.pack_into('<I',ram,0xa70040,256);struct.pack_into('<I',ram,0xa70044,65536);struct.pack_into('<I',ram,0xa7005c,18);struct.pack_into('<H',ram,0xa700a8,0)
    ram[0xa7000e]=2
    struct.pack_into('<I',ram,0xa70018,256);struct.pack_into('<2I',ram,0xa70098,0,0)
    stream_before=bytes(ram[0xa70000:0xa70100]);actual=[];expected=[]
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca70000,count//2,0x22340006,0x22340007];s.r[15]=0x8cf00000;s.pc=0x8c644cb0;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    def service(address,mem,r,trace):
        args=tuple(r[i] for i in range(4,8));trace.append((address,args))
        assert address==0x8c010200 and args[:3]==(0x87654321,288,256)
    def hook(address):
        def run(mem,r,sr):service(address,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address!=0x8c010200,'Modeled output callback forbidden'
        target={0x8c667a5c:magic,0x8c644c16:phase_get,0x8c644cb0:decoder,0x8c667844:request,0x8c667978:complete,0x8c644c8c:result_get,0x8c644c7e:ack,0x8c66792c:compact,0x8c667900:sample_copy,0x8c65b88c:input_provider,0x8c6676ae:channel_get,0x8c6676a0:position_get,0x8c65e798:buffer_get,0x8c65e4b8:query,0x8c65e368:lock_enter,0x8c65e380:lock_exit,0x8c6673a2:output_default,0x8c667384:input_default}.get(address)
        if target:return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        if address in [0x8c10ca5c,0x8c10cbb8,0x8c66780e]:
            target={0x8c10ca5c:division,0x8c10cbb8:remainder,0x8c66780e:wrapper}[address]
            return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        if address in [0x8c644c38,0x8c644c56]:
            target={0x8c644c38:setup,0x8c644c56:activate}[address]
            return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        assert address==0x8c65b238,address
        return kernel(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    for entry,target,arg1,arg2 in [(0x8c66768c,input_set,0x8c667384,0x8ca70000),(0x8c667696,output_set,0x8c6673a2,0x8ca70000),(0x8c66776e,start_stream,0,0)]:
        s.r[4:7]=[0x8ca70000,arg1,arg2]
        for index in range(4,7):regs[index]=s.r[index]
        s.pc=entry
        reference(ram,s,fpu=f,special=special)
        assert target(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        nf.fpul=fpul.value
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert struct.unpack_from('<I',native,0xa70004)[0]==1
    assert struct.unpack_from('<4I',native,0xa70088)==(0x8c667384,0x8ca70000,0x8c6673a2,0x8ca70000)
    stream_before=bytes(native[0xa70000:0xa70100])
    s.pc=0x8c667ad4
    reference(ram,s,{},fpu=f,special=special)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert code==0,(count,terminal,pattern,history,code)
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    decoded=0 if terminal else count
    channel_blocks=decoded//2
    assert struct.unpack_from('<2I',native,0xa6000c)==(0,decoded) and regs[15]==0x8cf00000
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
        compacted=samples
        assert native[destination:destination+len(samples)*2]==struct.pack('<'+'H'*len(compacted),*compacted)
        if not (terminal and count):
            expected_history=struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
            assert native[histaddr:histaddr+4]==expected_history
    assert actual==expected==[]
    wanted_stream=bytearray(stream_before);struct.pack_into('<I',wanted_stream,4,3);struct.pack_into('<2I',wanted_stream,0xa0,256,288)
    initial_counters=struct.unpack_from('<2I',stream_before,0x98)
    struct.pack_into('<2I',wanted_stream,0x98,*[(v+256)&0xffffffff for v in initial_counters])
    assert native[0xa70000:0xa70100]==wanted_stream
    records.append(dict(path=asset['path'],offset=offset,pair_sha256=hashlib.sha256(payload).hexdigest(),bank=bank,coefficients=coefficients,decoded_blocks=decoded))
    cases+=1
(out/'analysis/magic_lifecycle_default_callbacks_verification.json').write_text(json.dumps(dict(cases_passed=cases,original_files=len(assets),assets=records,scope='218 retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons using first288-byte encoded prefixes from109 original two-channel magic8000 assets,both FPU banks. Native default input667384 and output6673a2,callback registration,start,format dispatch,magic update,scheduler,request,decode,completion,zero-length compaction and acknowledgment. No modeled functions. Independent every sample exact binary32 recurrence and final predictor history,output guards,complete stream state and counters+98/+9c increment256. Controlled stream capacity256,position0,total256,zero initial histories; native constructor/header initialization integration,repeated updates,fullpayload,physical playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'default-callback original-audio lifecycle comparisons passed')
