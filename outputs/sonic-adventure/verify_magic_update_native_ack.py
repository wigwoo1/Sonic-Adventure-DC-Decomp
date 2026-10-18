import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_magic_update.dll')).sa_sound_stream_magic_update
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_phase_get.dll')).sa_file_decode_phase_get;getter.argtypes=fn.argtypes
ack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_completion_acknowledge.dll')).sa_file_decode_completion_acknowledge;ack.argtypes=fn.argtypes
services=[0x8c667844,0x8c644cb0,0x8c667978,0x8c010100,0x8c010200];cases=0
for phase,decoder_phase,finish,bank in itertools.product([0,1,2,3],[0,1,3],[2,3],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa70000:0xa70100]=bytes([0x66])*256;ram[0xa60000:0xa60040]=bytes([0x55])*64
    struct.pack_into('<2I',ram,0xa70004,phase,0x8ca60000);struct.pack_into('<I',ram,0xa6000c,decoder_phase)
    struct.pack_into('<4I',ram,0xa70088,0x8c010100,0x12345678,0x8c010200,0x87654321)
    before=bytes(ram[0xa70000:0xa70100]);handle_before=bytes(ram[0xa60000:0xa60040]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def write(mem,offset,values):
        for index,value in enumerate(values):
            for byte in range(4):mem[offset+index*4+byte]=(value>>(byte*8))&255
    def service(address,mem,r,trace):
        trace.append((address,tuple(r[i] for i in range(4,8))))
        if address==0x8c644c16:r[0]=int.from_bytes(bytes(mem[0xa6000c:0xa60010]),'little')
        elif address==0x8c010100:write(mem,0xa70070,[7,65,64])
        elif address==0x8c667844:write(mem,0xa6000c,[1])
        elif address==0x8c644cb0:write(mem,0xa6000c,[finish])
        elif address==0x8c667978:write(mem,0xa700a0,[32,72])
        elif address==0x8c644c7e:write(mem,0xa6000c,[0])
        else:assert address==0x8c010200
    def hook(address):
        def run(mem,r,sr):service(address,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address==0x8c644c16:return getter(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        if address==0x8c644c7e:return ack(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        service(address,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca70000;s.r[15]=0x8cf00000;s.pc=0x8c667a5c;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in services},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    wanted=bytearray(before);handle=bytearray(handle_before);addresses=[];active=phase
    if phase==1:
        addresses.append(0x8c644c16)
        if decoder_phase==0:
            addresses.extend([0x8c010100,0x8c667844]);struct.pack_into('<3I',wanted,0x70,7,65,64);active=2;struct.pack_into('<I',handle,12,1)
    if active==2:
        addresses.extend([0x8c644cb0,0x8c644c16]);struct.pack_into('<I',handle,12,finish)
        if finish==3:
            addresses.extend([0x8c667978,0x8c644c7e,0x8c010200]);struct.pack_into('<2I',wanted,0xa0,32,72);struct.pack_into('<I',handle,12,0);active=3
    struct.pack_into('<I',wanted,4,active)
    assert [a for a,_ in actual]==[a for a in addresses if a not in [0x8c644c16,0x8c644c7e]] and native[0xa70000:0xa70100]==wanted and native[0xa60000:0xa60040]==handle and regs[15]==0x8cf00000
    for address,args in actual:
        if address==0x8c010100:assert args==(0x12345678,0x8ca70070,0x8ca70074,0x8ca70078)
        if address==0x8c010200:assert args[:3]==(0x87654321,72,32)
    cases+=1
(out/'analysis/magic_update_native_ack_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='48 full retail RAM/CPU/FPU/SR/PR/service trace comparisons of60-instruction magic stream update667a5c..667ad4. Stream phases0/1/2/3,decoder phases0/1/3,finished/pending status,both FPU banks. Native decoder phase getter644c16 and completion acknowledgment644c7e; other downstream bodies and two callbacks modeled; independent state/memory/call-order oracle verifies idle start, same-update decoding/completion/acknowledgment, input pointer arguments and output byte/sample callback arguments. Further native integration and continuous playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'magic stream native-acknowledgment comparisons passed')
