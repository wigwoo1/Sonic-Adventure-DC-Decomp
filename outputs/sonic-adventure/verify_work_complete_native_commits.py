import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_complete.dll')).sa_sound_stream_work_complete
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
split=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_split_range.dll')).sa_sound_driver_split_range;split.argtypes=fn.argtypes
copylib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_pool_queue_initialize.dll'))
helpers={0x8c668af4:split}
for address,name in [(0x8c10cd38,'sa_engine_record_copy_dispatch'),(0x8c10cd8e,'sa_engine_record_copy8_tail')]:
    helpers[address]=getattr(copylib,name);helpers[address].argtypes=fn.argtypes
for address,stem,name in [(0x8c6676ae,'sound_stream_channel_count_get','sa_sound_stream_channel_count_get'),(0x8c667796,'sound_stream_decode_acknowledge','sa_sound_stream_decode_acknowledge'),(0x8c644c7e,'file_decode_completion_acknowledge','sa_file_decode_completion_acknowledge')]:
    helpers[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),name);helpers[address].argtypes=fn.argtypes
for address,stem,name in [(0x8c65e58a,'sound_source_commit','sa_sound_source_commit'),(0x8c65e644,'sound_source_return','sa_sound_source_return'),(0x8c65e368,'sound_source_lock_enter','sa_sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit','sa_sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder','sa_integer_remainder'),(0x8c10d25c,'byte_move','sa_byte_move')]:
    helpers[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),name);helpers[address].argtypes=fn.argtypes
services=[0x8c6676f6,0x8c6677b6,0x8c6677d8,0x8c010300];cases=0
for total,consumed,channels,limit,callback,bank in itertools.product([5,32,100],[0,4],[0,1,2],[0xffffffff,0,50],[0,0x8c010300],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;work=0x8ca60000
    ram[0xa60000:0xa60090]=bytes([0x55])*144
    struct.pack_into('<4I',ram,0xa60004,0x8ca70000,0x8ca80000,0x8ca80100,0x8ca80200)
    for offset,value in [(0x2c,7),(0x30,9),(0x34,consumed),(0x3c,limit),(0x40,11),(0x44,13),(0x48,callback),(0x4c,0x12345678)]:struct.pack_into('<I',ram,0xa60000+offset,value)
    for address in [0xa80000,0xa80100,0xa80200]:struct.pack_into('<I',ram,address,0x8ca90000)
    struct.pack_into('<2I',ram,0xa9001c,0x8c65e644,0x8c65e58a)
    objects_before=[]
    for index,address in enumerate([0xa80000,0xa80100,0xa80200]):
        base=0x8ca10000+index*0x10000
        struct.pack_into('<11I',ram,address+4,1,0,0,0,0 if index==0 else 100,64 if index==0 else 0,base,256,0,0,0)
        objects_before.append(bytes(ram[address:address+48]))
        ram[base&0xffffff:(base&0xffffff)+512]=bytes([0x5a])*512
    for offset,base,length in [(0x14,0x8ca10000,64),(0x1c,0x8ca20000,100),(0x24,0x8ca30000,100)]:struct.pack_into('<2I',ram,0xa60000+offset,base,length)
    ram[0xa70000:0xa70100]=bytes([0x77])*256;ram[0xa7000e]=channels
    struct.pack_into('<I',ram,0xa70004,3);struct.pack_into('<I',ram,0xa70008,0x8caa0000)
    ram[0xaa0000:0xaa0040]=bytes([0x66])*64;struct.pack_into('<I',ram,0xaa000c,3)
    stream_before=bytes(ram[0xa70000:0xa70100]);handle_before=bytes(ram[0xaa0000:0xaa0040])
    before=bytes(ram[0xa60000:0xa60090]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    chosen=min(total-consumed,32);splits=[]
    def service(address,mem,r,trace):
        trace.append((address,tuple(r[i] for i in range(4,8))))
        if address==0x8c6676f6:r[0]=total
        elif address==0x8c6677b6:r[0]=36
        elif address==0x8c6677d8:r[0]=32
        elif address==0x8c6676ae:r[0]=channels
        elif address==0x8c668af4:
            assert r[4] in [work+0x14,work+0x1c,work+0x24]
            assert r[5]==(36 if r[4]==work+0x14 else chosen*2&0xffffffff)
            for pointer in [r[6],r[7]]:
                offset=pointer&0xffffff
                for i,v in enumerate(struct.pack('<2I',r[4],r[5])):mem[offset+i]=v
        elif address==0x8c010300:assert r[4]==0x12345678
        elif address in [0x8c010100,0x8c010200]:
            encoded=r[4]==0x8ca80000
            if encoded:base=0x8ca10000;length=64;first=36;head=r[5]==0
            else:
                index=(r[4]-0x8ca80100)//256;assert index in [0,1]
                base=0x8ca20000+index*0x10000;length=100
                signed=ctypes.c_int32(chosen*2).value;first=min(length,signed)&0xffffffff;head=r[5]==1
            expected_descriptor=(base,first) if head else ((base+first)&0xffffffff,(length-first)&0xffffffff)
            offset=r[6]&0xffffff
            descriptor=tuple(sum(int(mem[offset+i*4+j])<<(j*8) for j in range(4)) for i in range(2))
            assert descriptor==expected_descriptor,(descriptor,expected_descriptor)
        else:assert address==0x8c667796
    def hook(address):
        def run(mem,r,sr):service(address,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address in helpers:return helpers[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        service(address,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=work;s.r[15]=0x8cf00000;s.pc=0x8c65ba22;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in services},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    wanted=bytearray(before)
    for offset,value in [(0x2c,7+chosen),(0x30,45),(0x34,consumed+chosen),(0x40,11+chosen),(0x44,49)]:struct.pack_into('<I',wanted,offset,value&0xffffffff)
    assert native[0xa60000:0xa60090]==wanted and regs[15]==0x8cf00000
    signed_limit=ctypes.c_int32(limit).value;notify=bool(callback and signed_limit>=0 and 11+chosen>=signed_limit)
    assert [a for a,args in actual]==[0x8c6676f6,0x8c6677b6,0x8c6677d8]+([0x8c010300] if notify else [])
    wanted_stream=bytearray(stream_before);struct.pack_into('<I',wanted_stream,4,0);struct.pack_into('<I',wanted_stream,0x9c,0)
    wanted_handle=bytearray(handle_before);struct.pack_into('<I',wanted_handle,0xc,0)
    assert native[0xa70000:0xa70100]==wanted_stream and native[0xaa0000:0xaa0040]==wanted_handle
    for index,address in enumerate([0xa80000,0xa80100,0xa80200]):
        wanted_object=bytearray(objects_before[index])
        if index==0:
            struct.pack_into('<I',wanted_object,0x10,36)
            struct.pack_into('<I',wanted_object,0xc,28)
            struct.pack_into('<I',wanted_object,0x18,36)
        elif index<=channels:
            published=chosen*2
            struct.pack_into('<I',wanted_object,0xc,published)
            struct.pack_into('<I',wanted_object,0x10,100-published)
            struct.pack_into('<I',wanted_object,0x14,published)
        assert native[address:address+48]==wanted_object
        base=(0x8ca10000+index*0x10000)&0xffffff
        assert native[base:base+512]==bytes([0x5a])*512
    cases+=1
(p/'analysis/work_complete_native_commits_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='216 retail RAM/CPU/FPU/SR/PR/service-trace completion comparisons65ba22..65bb4e with native range splitting/copying,source commit/return,interrupt save/restore,remainder,byte move,channel getter and stream/file acknowledgment. Total/byte/sample getters and notification callback modeled. Independent min(decoded32,total-consumed),complete144-byte work counters,256-byte stream and64-byte decoder states; encoded object consumes36 of64 reserved bytes and returns28; each active channel publishes twice trimmed sample count of100 reserved bytes and returns remainder. Entire three source objects and512-byte buffers checked,ring rollback positions asserted. Totals5/32/100,consumed0/4,channels0/1/2,limits-1/0/50,callback presence,both banks. Negative remaining already separately tested; current valid reservations avoid negative copy counts. Actual asset final trimming/full playback/fullgame unverified.'),indent=2)+'\n');print(cases,'native commits work completion comparisons passed')
