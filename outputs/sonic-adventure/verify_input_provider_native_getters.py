import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_input_provider.dll')).sa_sound_stream_input_provider
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
channel_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_channel_count_get.dll')).sa_sound_stream_channel_count_get;channel_get.argtypes=fn.argtypes
position_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_position_get.dll')).sa_sound_stream_position_get;position_get.argtypes=fn.argtypes
services=[0x8c65e798,0x8c010100];cases=0
for channels,capacity,maximum,remaining,bank in itertools.product([0,1,2],[0,128,256],[0,100,0xffffffff],[0xffffffff,100],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa60000:0xa60090]=bytes([0x55])*144
    struct.pack_into('<I',ram,0xa60004,0x8ca70000);struct.pack_into('<2I',ram,0xa6000c,0x8ca40000,0x8ca40100)
    struct.pack_into('<2I',ram,0xa6001c,0x8ca10028,capacity);struct.pack_into('<3I',ram,0xa60038,maximum,remaining,7)
    for address in [0xa40000,0xa40100]:struct.pack_into('<I',ram,address,0x8ca50000)
    struct.pack_into('<I',ram,0xa50018,0x8c010100);ram[0xa20000:0xa20020]=bytes([0x66])*32
    ram[0xa70000:0xa70100]=bytes([0x77])*256;ram[0xa7000e]=channels;struct.pack_into('<I',ram,0xa7003c,99);stream_before=bytes(ram[0xa70000:0xa70100])
    before=bytes(ram[0xa60000:0xa60090]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def write(mem,offset,values):
        for index,value in enumerate(values):
            for byte in range(4):mem[offset+index*4+byte]=(value>>(byte*8))&255
    def service(address,mem,r,trace):
        trace.append((address,tuple(r[i] for i in range(4,8))))
        if address==0x8c6676ae:r[0]=channels
        elif address==0x8c65e798:assert r[4]==0x8ca40000;r[0]=0x8ca10000
        elif address==0x8c6676a0:assert r[4]==0x8ca70000;r[0]=99
        else:
            index=(r[4]-0x8ca40000)//256
            assert r[4]==0x8ca40000+index*256 and index<channels and r[5]==0 and r[6]==0x4000 and r[7]==0x8ca6001c+index*8
            write(mem,0xa6001c+index*8,[0x8ca10028+index*8,capacity+index*16])
    def hook(address):
        def run(mem,r,sr):service(address,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target={0x8c6676ae:channel_get,0x8c6676a0:position_get}.get(address)
        if target:return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        service(address,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca60000,0x8ca20004,0x8ca20008,0x8ca2000c];s.r[15]=0x8cf00000;s.pc=0x8c65b88c;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in services},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    signed_max=ctypes.c_int32(maximum).value;wanted_capacity=min(capacity//2,signed_max)&0xffffffff;wanted_remaining=0x1fffffff if remaining&0x80000000 else (remaining-7)&0xffffffff
    assert native[0xa20000:0xa20020]==bytes([0x66])*4+struct.pack('<3I',20,wanted_capacity,wanted_remaining)+bytes([0x66])*16
    wanted=bytearray(before)
    for index in range(channels):struct.pack_into('<2I',wanted,0x1c+index*8,0x8ca10028+index*8,capacity+index*16)
    assert native[0xa60000:0xa60090]==wanted and regs[0]==99 and regs[15]==0x8cf00000
    assert [a for a,args in actual]==[0x8c010100]*channels+[0x8c65e798]
    assert native[0xa70000:0xa70100]==stream_before
    cases+=1
(out/'analysis/input_provider_native_getters_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='108 full retail RAM/CPU/FPU/SR/PR/service trace input provider comparisons65b88c..65b930. Channel count0/1/2,query capacities0/128/256,signed maxima0/100/-1,finite/negative remaining,both FPU banks. Native signed channel-count getter6676ae and position getter6676a0; source-buffer getter65e798 and virtual channel queries modeled. Independent offset20,logical half-capacity signed minimum,remaining-limit sentinel1fffffff,full144-byte work object/output guards,query argument/order and tail-return99 checked. Remaining source getter/query integration,physical input/fullstream playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'input provider native-getter comparisons passed')
