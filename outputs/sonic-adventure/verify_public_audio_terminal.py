import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_update.dll')).sa_sound_stream_work_update
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
capacity_fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_capacity.dll')).sa_sound_source_capacity;capacity_fn.argtypes=fn.argtypes
helpers={}
for address,stem in [(0x8c65bb4e,'sound_stream_work_tick'),(0x8c65b930,'sound_stream_work_feed'),(0x8c667718,'sound_stream_phase_get'),(0x8c667ad4,'sound_stream_update_dispatch'),(0x8c667a5c,'sound_stream_magic_update'),(0x8c644c16,'file_decode_phase_get'),(0x8c65e4b8,'sound_source_query'),(0x8c65e644,'sound_source_return'),(0x8c65e368,'sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder'),(0x8c66771c,'sound_stream_source_set'),(0x8c66776e,'sound_stream_start'),(0x8c10ca5c,'signed_division'),(0x8c6676a4,'sound_stream_format_get'),(0x8c6676ee,'sound_stream_unit_count_get'),(0x8c6676f6,'sound_stream_total_samples_get'),(0x8c65bc6c,'sound_work_unit_count_get'),(0x8c65bc84,'sound_work_total_samples_get')]:
    helpers[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),'sa_'+stem);helpers[address].argtypes=fn.argtypes
cases=0
inventory=json.loads((p/'analysis/magic_audio_endings_inventory.json').read_text())
assets=[a for a in inventory['assets'] if a['first_word_after_end']=='0x8001'];assert len(assets)==108
for asset,marker,bank in itertools.product(assets,[True],[0,1]):
        original=(ROOT/asset['path']).read_bytes();end=asset['declared_encoded_end']
        final_pair=original[end-36:end];terminal_block=original[end:end+18]
        assert hashlib.sha256(final_pair).hexdigest()==asset['final_pair_sha256']
        assert terminal_block[:2]==bytes.fromhex('8001')
        payload=terminal_block if marker else final_pair
        length=len(payload);source_capacity=length;stop=1;fmt=0;exhausted=False;capacity=8
        stack=0x8cf00000
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        ram[0xa60000:0xa60100]=bytes([0x55])*256;ram[0xa60003]=stop;ram[0xa60001]=2
        struct.pack_into('<3I',ram,0xa60004,0x8ca70000,0x8ca80000,0x8ca80100)
        struct.pack_into('<I',ram,0xa60034,100 if exhausted else 0)
        for address in [0xa80000,0xa80100]:struct.pack_into('<I',ram,address,0x8ca90000)
        for offset,target in [(0x18,0x8c65e4b8),(0x24,0x8c65e462),(0x1c,0x8c65e644)]:struct.pack_into('<I',ram,0xa90000+offset,target)
        struct.pack_into('<11I',ram,0xa80004,1,0,source_capacity,0,0,0,0x8ca10000,256,0,0,0)
        struct.pack_into('<I',ram,0xa80110,capacity)
        ram[0xa70000:0xa70100]=bytes([0x77])*256
        struct.pack_into('<H',ram,0xa700a8,fmt)
        struct.pack_into('<I',ram,0xa70004,0)
        struct.pack_into('<I',ram,0xa70008,0x8caa0000)
        ram[0xaa0000:0xaa0040]=bytes([0x66])*64;struct.pack_into('<I',ram,0xaa000c,0)
        handle_before=bytes(ram[0xaa0000:0xaa0040])
        ram[0xa7000d:0xa70010]=bytes([16,2,18])
        struct.pack_into('<I',ram,0xa70010,4);struct.pack_into('<I',ram,0xa70018,100)
        stream_before=bytes(ram[0xa70000:0xa70100])
        object_before=[bytes(ram[a:a+48]) for a in [0xa80000,0xa80100]]
        ram[0xa10000:0xa10200]=bytes([0x5a])*512
        ram[0xa10000:0xa10000+length]=payload
        buffer_before=bytes(ram[0xa10000:0xa10200])
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
        s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca60000;s.r[15]=stack;s.pc=0x8c65bb88;s.pr=0x8c000100;s.sr=0x70000063
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
        assert native[0xa10000:0xa10200]==buffer_before
        assert native[0xaa0000:0xaa0040]==handle_before
        cases+=1
(p/'analysis/public_audio_terminal_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='216 retail RAM/CPU/FPU/SR/PR comparisons of public work update65bb88 through native tick/feed/update dispatch/magic idle check, using actual18-byte terminal markers from108 original valid audio endings,both banks. Original final pair SHA256 and marker bytes validated. Independent work state3, full encoded reservation rollback, entire source/sample objects and512-byte encoded buffer preserved, stream phase0/source fields unchanged, entire64-byte decoder handle unchanged. No modeled calls; no decode requested. Controlled idle stream/handle fixture, full-file histories/streaming/playback/fullgame unverified; SSRACING invalid header ending excluded.'),indent=2)+'\n')
print(cases,'public audio terminal comparisons passed')
