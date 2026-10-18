import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_feed.dll')).sa_sound_stream_work_feed
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
capacity_fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_capacity.dll')).sa_sound_source_capacity;capacity_fn.argtypes=fn.argtypes
helpers={}
for address,stem in [(0x8c65e4b8,'sound_source_query'),(0x8c65e644,'sound_source_return'),(0x8c65e368,'sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder'),(0x8c66771c,'sound_stream_source_set'),(0x8c66776e,'sound_stream_start'),(0x8c10ca5c,'signed_division'),(0x8c6676a4,'sound_stream_format_get'),(0x8c6676ee,'sound_stream_unit_count_get'),(0x8c6676f6,'sound_stream_total_samples_get'),(0x8c65bc6c,'sound_work_unit_count_get'),(0x8c65bc84,'sound_work_total_samples_get')]:
    helpers[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),'sa_'+stem);helpers[address].argtypes=fn.argtypes
cases=0
for stop,source_capacity,fmt,length,marker,exhausted,capacity,bank in itertools.product([0,1],[0,8],[0,1],[1,36],[False,True],[False,True],[6,8],[0,1]):
        source_capacity=length if source_capacity else 0
        length=source_capacity
        stack=0x8cf00000
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        ram[0xa60000:0xa60100]=bytes([0x55])*256;ram[0xa60003]=stop
        struct.pack_into('<3I',ram,0xa60004,0x8ca70000,0x8ca80000,0x8ca80100)
        struct.pack_into('<I',ram,0xa60034,100 if exhausted else 0)
        for address in [0xa80000,0xa80100]:struct.pack_into('<I',ram,address,0x8ca90000)
        for offset,target in [(0x18,0x8c65e4b8),(0x24,0x8c65e462),(0x1c,0x8c65e644)]:struct.pack_into('<I',ram,0xa90000+offset,target)
        struct.pack_into('<11I',ram,0xa80004,1,0,source_capacity,0,0,0,0x8ca10000,256,0,0,0)
        struct.pack_into('<I',ram,0xa80110,capacity)
        ram[0xa70000:0xa70100]=bytes([0x77])*256
        struct.pack_into('<H',ram,0xa700a8,fmt)
        struct.pack_into('<I',ram,0xa70004,0)
        ram[0xa7000d:0xa70010]=bytes([16,2,18])
        struct.pack_into('<I',ram,0xa70010,4);struct.pack_into('<I',ram,0xa70018,100)
        stream_before=bytes(ram[0xa70000:0xa70100])
        object_before=[bytes(ram[a:a+48]) for a in [0xa80000,0xa80100]]
        ram[0xa10000:0xa10002]=bytes.fromhex('8001' if marker else '1234')
        native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
        def service(address,mem,r,trace):
            trace.append((address,tuple(r[i] for i in range(4,8))))
            if address==0x8c010200:
                assert r[4] in [0x8ca80000,0x8ca80100]
                r[0]=source_capacity if r[4]==0x8ca80000 else capacity
            elif address==0x8c010100:
                assert (r[4],r[5],r[6],r[7])==(0x8ca80000,1,0x7fffffff,0x8ca60014)
                for i,v in enumerate(struct.pack('<2I',0x8ca10000,length)):mem[0xa60014+i]=v
            elif address==0x8c6676a4:r[0]=fmt
            elif address==0x8c65bc84:r[0]=100
            elif address==0x8c65bc6c:r[0]=4
            elif address==0x8c010300:assert (r[4],r[5],r[6])==(0x8ca80000,1,0x8ca60014)
            elif address==0x8c66771c:assert (r[4],r[5],r[6])==(0x8ca70000,0x8ca10000,length)
            else:assert address==0x8c66776e and r[4]==0x8ca70000
        def hook(address):
            def run(mem,r,sr):service(address,mem,r,expected);return sr
            return run
        @Call
        def call(ctx,address,mem,f,sr,r,pr):
            if address==0x8c65e462:return capacity_fn(mem,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
            if address in helpers:return helpers[address](mem,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
            raise AssertionError('Unexpected modeled call '+hex(address))
        s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca60000;s.r[15]=stack;s.pc=0x8c65b930;s.pr=0x8c000100;s.sr=0x70000063
        regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
        reference(ram,s,{},fpu=f,special={'macl':0})
        assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
        early=stop==1 and source_capacity==0
        terminal=fmt==0 and length>=2 and marker
        ended=early or terminal or exhausted
        assert actual==expected==[]
        assert native[0xa60001]==(3 if ended else 0x55) and regs[15]==stack
        started=not ended and capacity//2>=4
        wanted_object=bytearray(object_before[0])
        if started:
            struct.pack_into('<I',wanted_object,0xc,0)
            struct.pack_into('<I',wanted_object,0x18,length)
        assert native[0xa80000:0xa80030]==wanted_object
        assert native[0xa80100:0xa80130]==object_before[1]
        if not early:assert struct.unpack_from('<2I',native,0xa60014)==(0x8ca10000,length)
        wanted_stream=bytearray(stream_before)
        started=not ended and capacity//2>=4
        if started:
            for offset,value in [(4,1),(0x50,0x8ca10000),(0x54,length//(18 if fmt==0 else 4)),(0x84,0),(0xa0,0),(0xa4,0)]:struct.pack_into('<I',wanted_stream,offset,value)
        assert native[0xa70000:0xa70100]==wanted_stream
        cases+=1
(p/'analysis/feed_native_reservations_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='256 full retail RAM/CPU/FPU/SR/PR and ordered call comparisons of source feeder65b930..65ba22. Stop flag/source capacity,format,descriptor length,original 0x8001 marker,total exhaustion,sample capacity,banks. Independent state3 termination, reservation return versus source/start call decisions and restored stack assertions. Source capacity method executes natively in both source and sample modes; entire source objects independently unchanged. Format/unit/total getters and work wrappers also native, Source setter/start and signed division native; entire256 stream independently checked for phase1,source pointer,derived units and reset counters on start, unchanged on rejection. Encoded query/return, interrupt helpers and signed remainder also native: no modeled calls. Source reservations independently exhausted and write position advanced on start, fully rolled back on rejection; sample object unchanged. Zero/one/36-byte reservations, controlled source words; actual file streaming/full playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'native reservation feeder comparisons passed')
