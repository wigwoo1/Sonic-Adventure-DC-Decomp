import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_feed.dll')).sa_sound_stream_work_feed
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
capacity_fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_capacity.dll')).sa_sound_source_capacity;capacity_fn.argtypes=fn.argtypes
cases=0
for stop,source_capacity,fmt,length,marker,exhausted,capacity,bank in itertools.product([0,1],[0,8],[0,1],[1,36],[False,True],[False,True],[6,8],[0,1]):
        stack=0x8cf00000
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        ram[0xa60000:0xa60100]=bytes([0x55])*256;ram[0xa60003]=stop
        struct.pack_into('<3I',ram,0xa60004,0x8ca70000,0x8ca80000,0x8ca80100)
        struct.pack_into('<I',ram,0xa60034,100 if exhausted else 0)
        for address in [0xa80000,0xa80100]:struct.pack_into('<I',ram,address,0x8ca90000)
        for offset,target in [(0x18,0x8c010100),(0x24,0x8c65e462),(0x1c,0x8c010300)]:struct.pack_into('<I',ram,0xa90000+offset,target)
        struct.pack_into('<I',ram,0xa8000c,source_capacity)
        struct.pack_into('<I',ram,0xa80110,capacity)
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
            service(address,mem,r,actual);return 0
        s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca60000;s.r[15]=stack;s.pc=0x8c65b930;s.pr=0x8c000100;s.sr=0x70000063
        regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
        reference(ram,s,{a:hook(a) for a in [0x8c010100,0x8c010300,0x8c6676a4,0x8c65bc84,0x8c65bc6c,0x8c66771c,0x8c66776e]},fpu=f,special={'macl':0})
        assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
        early=stop==1 and source_capacity==0
        terminal=fmt==0 and length>=2 and marker
        ended=early or terminal or exhausted
        addresses=[]
        if not early:
            addresses += [0x8c010100,0x8c6676a4]
            if not terminal:addresses += [0x8c65bc84]
            if not terminal and not exhausted:addresses += [0x8c65bc6c]
            addresses += [0x8c010300] if terminal or exhausted or capacity//2<4 else [0x8c66771c,0x8c66776e]
        assert [a for a,args in actual]==addresses
        assert native[0xa60001]==(3 if ended else 0x55) and regs[15]==stack
        for index,a in enumerate([0xa80000,0xa80100]):assert native[a:a+48]==object_before[index]
        cases+=1
(p/'analysis/feed_native_capacity_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='256 full retail RAM/CPU/FPU/SR/PR and ordered call comparisons of source feeder65b930..65ba22. Stop flag/source capacity,format,descriptor length,original 0x8001 marker,total exhaustion,sample capacity,banks. Independent state3 termination, reservation return versus source/start call decisions and restored stack assertions. Source capacity method executes natively in both source and sample modes; entire source objects independently unchanged. Other subordinate methods modeled; full native feeding/full playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'native capacity feeder comparisons passed')
