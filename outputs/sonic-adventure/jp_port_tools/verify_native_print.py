"""Original print execution, exact defined packets, and actual font glyph GPU.
Allocator and VRAM-copy boundaries are explicit guest services for init only.
No print/character/strlen/context function hooks. Private original-font pixels.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Surface
from native_video_test_support import Video,Context
from native_gpu_test_support import GPU,Config,Report,Failure
from primitive_reference import make_reference
from native_pvm_test_support import image_oracle
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','font-input','output']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.upstream/'config/1st_read/symbols.txt')==pin['files_sha256']['config/1st_read/symbols.txt']
    rawfont=(a.font_input/'debug-font-128x128-argb1555-twiddled.bin').read_bytes();assert hashlib.sha256(rawfont).hexdigest()==b['private_font_payload_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);checks=calls=packets=defined=images=pixels=0;seen=set();groups={};shots=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);status=bind(lib,'jp_queue_status',I);clear=bind(lib,'jp_queue_clear_error',None)
    data=bind(lib,'jp_queue_data',P,U);used=bind(lib,'jp_queue_committed_bytes',S,U);param=bind(lib,'jp_queue_parameter_cursor',P,U);desc=bind(lib,'jp_queue_descriptor_cursor',P,U)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexNonTexTrans',P);end=bind(lib,'njEndVertex',None,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q);pinned=bind(lib,'jp_frames_pinned_bytes',S);orphan=bind(lib,'jp_frames_orphaned_bytes',S)
    init=bind(lib,'njInitPrint',None,P,U,U);exitprint=bind(lib,'njExitPrint',None);size=bind(lib,'njPrintSize',None,C.c_uint16);color=bind(lib,'njPrintColor',None,U);ps=bind(lib,'jp_print_status',I);pc=bind(lib,'jp_print_clear_error',None)
    printers={'njPrintC':bind(lib,'njPrintC',None,I,P),'njPrintD':bind(lib,'njPrintD',None,I,I,I),'njPrintB':bind(lib,'njPrintB',None,I,U,I),'njPrintH':bind(lib,'njPrintH',None,I,U,I)}
    fontptr=bind(lib,'jp_print_font_surface',P);contexts=(Context*bind(lib,'jp_queue_context_count',U)()).in_dll(lib,'_nj_context_');current=P.in_dll(lib,'_nj_current_ctx_');bank=U.in_dll(lib,'_nj_current_texture_bank')
    getparam=bind(lib,'jp_queue_get_parameter',I,P)
    check(qi(32<<20)==0 and fi(8,16,32<<20)==0 and t.init(4,16,8<<20)==0 and v.init(320,96,0xff000000,1)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'real shared native owners/default contexts')
    # Original RAM plus explicit store-queue/PREF observations; existing
    # frozen instruction executor is extended by the qualified primitive model.
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import multiply,round_dyadic,truncate_i32
    reference,refhash=make_reference(executor)
    table={n:int(x,16) for x,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    CURSORS=0x8cd00000;TEXT=0x8cd02000;LIST=0x8ce00000
    def store(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def read(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def word(at):return struct.unpack('<I',read(at,4))[0]
    def iw(at,v):store(at,words(v))
    iw(table['_nj_vertex_buffer_'],[CURSORS]);iw(CURSORS,[LIST]*5)
    for i,ctx in enumerate(contexts):
        # Convert the one widened pointer and its alignment padding; all
        # other fields remain 32-bit SDK values in the same original order.
        store(table['_nj_context_']+i*192,bytes(ctx.prefix)+words([0])+bytes(ctx)[Context.face.offset:])
    sqbase=(LIST&0x3ffffff)|0xe0000000
    class MMIO(dict):
        def __contains__(self,key):return 0xe0000000<=key<0xe4000000 or dict.__contains__(self,key)
        def __getitem__(self,key):return dict.get(self,key,0)
    mmio=MMIO({0xff000038:0,0xff00003c:0});captured=[];services={'kmiTXalloc':0,'kmiCopyToFrameBuffer':0}
    entries={table[n]:n for n in [*b['print_original_function_sha256'],'strlen','kmCreateTextureSurface','kmLoadTexture','kmProcessVertexRenderState']}
    def trace(pc,*rest):
        if pc in entries:seen.add(entries[pc])
    def pref(at):
        assert at&0xf0000000==0xe0000000,hex(at)
        at&=~31;captured.append([mmio[at+i*4] for i in range(8)])
    original_font=[None];loaded=[]
    def alloc(ram,regs,sr):
        services['kmiTXalloc']+=1;original_font[0]=regs[5];check(regs[4]==word(regs[5]+20),'guest allocation uses exact described byte size');iw(regs[5]+28,[8]);regs[0]=0;return sr
    def copy(ram,regs,sr):
        services['kmiCopyToFrameBuffer']+=1;loaded.append(read(regs[5],regs[6]));check(regs[4]==8,'guest copy targets allocated VRAM offset');regs[0]=0;return sr
    init_hooks={table['kmiTXalloc']:alloc,table['kmiCopyToFrameBuffer']:copy}
    def original(name,*args,hooks=None):
        nonlocal calls
        f=Fpu();f.fpscr=0x40001|((calls%2)<<21);s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,val in enumerate(args):s.r[4+i]=val&0xffffffff
        reference(ram,s,fpu=f,mmio=mmio,hooks=hooks,trace=trace,cache_prefetch=pref,instruction_budget=3000000);calls+=1;check(s.r[15]==0x8cf00000,'whole original stack restored');return s
    original('njInitPrint',0,0,0,hooks=init_hooks);init(None,0,0)
    nativefont=Surface.from_address(fontptr());fontword=t.surface_word(C.byref(nativefont))
    check(ps()==status()==t.status()==0 and loaded[-1]==rawfont and C.string_at(t.surface_data(fontword),32768)==rawfont,'genuine init uses exact authentic built-in font')
    check(bytes(nativefont)[:28]==read(original_font[0],28),'all font descriptor value fields equal original describe/create calls')
    guestctx=read(table['_nj_context_']+4*192,192);nativewords=list(contexts[4].words[:4]);guestwords=list(struct.unpack_from('<4I',guestctx,144));guestwords[3]=(guestwords[3]&~0x1fffff)|fontword
    check(nativewords==guestwords,('complete original context processing and native font header match with resource-address relocation',list(map(hex,nativewords)),list(map(hex,guestwords))))
    check(read(original_font[0]+28,4)==words([8]) and t.live()==1,'one direct font allocation, native owner resource ID replaces VRAM offset')
    groups['genuine_initialization_and_explicit_allocator_copy_boundaries']=checks
    # Independent value oracle across the signed grid and uint16 size domain,
    # including inexact products near +/-2^31, followed by signed word wrap.
    coord=bind(lib,'jp_print_coordinate',C.c_int16,C.c_int16,C.c_uint16);advance=bind(lib,'jp_print_advance',C.c_int16,C.c_int16,C.c_uint16);rng=random.Random(0x5052494e)
    cases=[(x,s) for x in [-32768,-32767,-12345,-1,0,1,12345,32766,32767] for s in [0,1,7,8,255,256,32767,32768,65534,65535]]+[(rng.randrange(-32768,32768),rng.randrange(65536)) for _ in range(8192)]
    for x,sz in cases:
        product=multiply(round_dyadic(x,0,1)[0],round_dyadic(sz,0,1)[0],1)[0];want=C.c_int16(truncate_i32(product)[0]).value
        check(coord(x,sz)==want and advance(x,sz)==C.c_int16(x+sz).value,'independent RZ multiply/FTRC/EXTS.W and exact pen advance')
    groups['coordinate_and_pen_value_policy']=checks-sum(groups.values())
    def snapshot():
        parameters=(U*4)();check(getparam(parameters)==0,'read actual Ninja parameter cache')
        return current.value,bank.value,bytes(contexts),bytes(parameters),tuple((param(i) or 0)-(data(i) or 0) for i in range(5)),tuple(desc(i) for i in range(5))
    def compare(name,args,sz,col):
        nonlocal packets,defined
        check(reset()==0,'clear actual native queue');iw(CURSORS+8,[LIST]);captured.clear();size(sz);color(col);original('njPrintSize',sz);original('njPrintColor',col)
        prior=snapshot();guestargs=list(args);nativeargs=list(args);keep=None
        if name=='njPrintC':
            raw=args[1];store(TEXT,raw+b'\0');guestargs[1]=TEXT;keep=C.create_string_buffer(raw);nativeargs[1]=C.addressof(keep)
        original(name,*guestargs);printers[name](*nativeargs);after=snapshot()
        check(ps()==status()==0,'real native print succeeds');blob=C.string_at(data(1),used(1));rows=[list(struct.unpack_from('<8I',blob,i)) for i in range(0,len(blob),32)]
        check(len(rows)==len(captured) and len(blob)==word(CURSORS+8)-LIST,'exact packet count and original descriptor advancement')
        for i,(actual,want) in enumerate(zip(rows,captured)):
            n=4 if not i else 7
            if not i:want=list(want);want[3]=(want[3]&~0x1fffff)|fontword
            check(actual[:n]==want[:n],('whole original defined packet words',name,i,actual,want));defined+=n
            check(actual[n:]==[0]*(8-n),'native deterministic padding')
        check(prior[:5]==after[:5] and all(prior[5][i]==after[5][i] for i in [0,2,3,4]),'print preserves selected context, bank, all context bytes, parameters and all parameter cursors; other descriptors untouched')
        check(after[5][1]==data(1)+len(blob) and t.current.value==0xfffffffe and word(table['_nj_current_texture'])==0xfffffffe,'only translucent descriptor advances and original texture sentinel is -2')
        packets+=len(rows);return blob
    # All unsigned character codes, zero-size geometry, large coordinates,
    # pen wrap, signed decimals including INT_MIN, binary and hexadecimal.
    for sz,col,loc in [(8,0xffbfbfbf,0),(0,0x01234567,0x7fff8000),(65535,0xffffffff,0x80007fff),(257,0x7fa04080,0x7fff7fff),(16,0xff40c0ff,0x00010001)]:
        compare('njPrintC',[C.c_int32(loc).value,bytes(range(1,256))],sz,col)
    compare('njPrintC',[0,b''],8,0xffffffff)
    for val in [0,1,-1,1234567,-999999999,2147483647,-2147483648]:
        for digits in [0,1,3,10,12]:compare('njPrintD',[0x00010001,val,digits],65535 if digits==3 else 8,0xffabcdef)
    for name,limits in [('njPrintB',[0,1,7,31,32]),('njPrintH',[0,1,3,7,8])]:
        for val in [0,1,0x80000000,0xffffffff,0xabcdef01]:
            for digits in limits:compare(name,[0x7fff8000,val,digits],257,0xff123456)
    check(set(b['print_original_function_sha256'])-{'njExitPrint'}<=seen,'genuine print init/size/color/character/C/D/B/H and real strlen/context services executed')
    groups['whole_original_packet_and_renderer_state_comparisons']=checks-sum(groups.values())
    # Native invalid inputs reject before queue/context/font changes.
    check(reset()==0,'clear queue for invalid inputs');before=snapshot();font_before=bytes(nativefont)
    for name,args in [('njPrintC',[0,None]),('njPrintB',[0,0,33]),('njPrintB',[0,0,-1]),('njPrintH',[0,0,9]),('njPrintD',[0,-1,-1])]:
        printers[name](*args);check(ps()==1 and used(1)==0 and snapshot()==before and bytes(nativefont)==font_before,'invalid string/digit count rejects without stream or renderer changes');pc()
    longtext=C.create_string_buffer(b'A'*32768);printers['njPrintC'](0,longtext);check(ps()==1 and used(1)==0 and snapshot()==before,'long string beyond signed SDK length rejected');pc()
    bind(lib,'njCharacter',None,C.c_int16,C.c_int16,C.c_char)(0,0,b'A');check(ps()==2 and used(1)==0,'bare character outside print transaction is rejected');pc()
    custom=C.create_string_buffer(bytes(128))
    for dim,attr in [(0,256),(7,256),(9,256),(2048,256),(8,0),(8,259),(8,0x10000|256)]:
        init(custom,dim,attr);check(ps()==1 and bytes(nativefont)==font_before and snapshot()==before,'unsupported native font formats/dimensions preserve original font/state');pc()
    # Failed raw queue append must undo the pending resource pin. Alias inputs
    # are rejected before an allocation could invalidate them.
    append=bind(lib,'jp_queue_append_direct_translucent',I,P,P,U);header=(U*4)(*nativewords)
    check(append(header,None,1)==1 and used(1)==0,'missing direct packet rejects');clear()
    check(append(header,C.c_void_p(0x1000),0xffffffff)==1 and used(1)==0,'overflow block count rejects before read');clear()
    check(append(header,C.c_void_p(0x1000),10000000)==4 and used(1)==0 and pinned()==0,'budget failure rolls back actual pending font reference');clear()
    check(append(header,None,0)==0 and used(1)==32 and pinned()==32768,'zero-glyph original header is an actual retained transaction')
    check(append(C.c_void_p(data(1)),None,0)==1 and used(1)==32,'aliased direct header rejected before growth');clear();check(reset()==0 and pinned()==0,'queue reset releases unsubmitted font reference')
    groups['native_invalid_bounds_budget_alias_and_rollback']=checks-sum(groups.values())
    # Check the whole original init/create/describe/load/header path for every
    # accepted native font size and 16-bit colour format. Guest services still
    # replace only the allocator and VRAM copy, never those original bodies.
    for dim in [8,16,32,64,128,256,512,1024]:
        for fmt in [0,1,2]:
            payload=bytes([dim&255,fmt+128])*(dim*dim);buf=C.create_string_buffer(payload);store(0x8ca00000,payload)
            original('njInitPrint',0x8ca00000,dim,0x100|fmt,hooks=init_hooks);init(buf,dim,0x100|fmt)
            nativefont=Surface.from_address(fontptr());fontword=t.surface_word(C.byref(nativefont))
            check(ps()==t.status()==0 and bytes(nativefont)[:28]==read(original_font[0],28) and loaded[-1]==payload and C.string_at(t.surface_data(fontword),len(payload))==payload,('custom font metadata and all copied bytes equal whole original create/describe/load path',dim,fmt,ps(),t.status(),list(struct.unpack('<7I',bytes(nativefont)[:28])),list(struct.unpack('<7I',read(original_font[0],28))),len(loaded[-1])))
            guestwords=list(struct.unpack('<4I',read(table['_nj_context_']+4*192+144,16)));guestwords[3]=(guestwords[3]&~0x1fffff)|fontword
            check(list(contexts[4].words[:4])==guestwords,'custom font header equals whole original header builder')
    init(None,0,0);nativefont=Surface.from_address(fontptr());fontword=t.surface_word(C.byref(nativefont));check(ps()==0 and C.string_at(t.surface_data(fontword),32768)==rawfont,'restore authentic default font')
    groups['all_supported_font_initializers_and_payloads']=checks-sum(groups.values())
    # Render a full original-font character grid at exact texel centres.
    # Each character is printed by original njPrintC; no synthetic glyph draw.
    check(reset()==0 and v.init(128,128,0xff000000,1)==0,'native atlas text queue and actual video viewport');size(8);color(0xffffffff)
    for row in range(16):
        # Code zero cannot be passed in a C string: leave its known blank cell.
        first=max(1,row*16);raw=bytes(range(first,(row+1)*16));buf=C.create_string_buffer(raw)
        printers['njPrintC']((((1 if row==0 else 0)<<16)|row),buf)
    fid=Q();check(ps()==status()==0 and submit(C.byref(fid))==0,'capture real original glyph print frame')
    gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;cfg=Config.make(128,128,1);cfg.logical_width=cfg.logical_height=128
    # Video logical dimensions are separately retained in this actual frame.
    buffer=C.create_string_buffer(128*128*4);report=Report();err=Failure();bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0,('real original glyph GPU draw',err.code,err.batch,err.hresult))
    # Preserve the observed image before asserting the independent image so
    # any sampling/layout disagreement has an inspectable actual artifact.
    png(out/'glyph-grid-observed.png',buffer.raw,128,128)
    (out/'observed-frame.json').write_text(json.dumps(dict(logical_width=v.frame_snapshot(fid.value).logical_width,logical_height=v.frame_snapshot(fid.value).logical_height,clear_argb=hex(v.frame_snapshot(fid.value).clear_argb),first_pixel=list(buffer.raw[:4]),context_header=list(map(hex,contexts[4].words[:4]))),indent=2))
    expected=image_oracle(dict(kind=0x100,w=128,h=128,pixels=rawfont),128,128)
    # Font texels are binary black/white with binary alpha. Transparent pixels
    # retain the opaque black background in the real blended frame.
    expected=bytearray(expected)
    for i in range(0,len(expected),4):
        if not expected[i+3]:expected[i:i+4]=b'\0\0\0\xff'
    # NUL's cell is intentionally omitted above.
    for y in range(8):
        for x in range(8):expected[(y*128+x)*4:(y*128+x)*4+4]=b'\0\0\0\xff'
    check(buffer.raw==expected,('all actual printed glyph pixels equal independent font oracle',sum(x!=y for x,y in zip(buffer.raw,expected))))
    images+=1;pixels+=16384
    window=P();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,128,128,1,C.byref(window),C.byref(err))==0,'actual original glyph window')
    check(bind(gl,'jp_window_render_frame',I,P,Q,P,P,P)(window,fid,C.byref(cfg),C.byref(report),C.byref(err))==0,'actual original glyph backbuffer')
    actual=C.create_string_buffer(len(buffer));check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,actual,len(actual),C.byref(err))==0 and actual.raw==buffer.raw,'actual glyph backbuffer matches verified D3D pixels');shot=out/'original-printed-glyphs.png';png(shot,actual.raw,128,128);shots.append(dict(path=str(shot),sha256=sha(shot),actual_window_backbuffer=True,original_print_functions=True,retail_derived=True,gameplay=False))
    check(bind(gl,'jp_window_present',I,P,U,P)(window,0,C.byref(err))==0,'actual original glyph Present');bind(gl,'jp_window_close',None,P)(window)
    # Submitted font bytes remain valid after genuine SDK table reset and exit.
    bind(lib,'njInitTexture',None,P,U)(None,0);check(t.status()==0 and t.live()==1,'genuine SDK record reset preserves active print font');exitprint();check(t.live()==0 and orphan()==pinned()==32768,'real exit retires font through frame owner')
    check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0 and buffer.raw==expected,'actual printed glyphs survive exit through submitted frame');images+=1;pixels+=16384
    cfg.textures=0;check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0 and report.texture_uploads==report.decoded_texels==report.shader_texture_bindings==report.shader_sampler_bindings==0,'Texture Off samples no font texture');images+=1
    check(release(fid.value)==0 and pinned()==orphan()==0,'last genuine font frame fully releases payload')
    # A readable progress capture produced by the genuine text callers. This
    # is a native text test scene, not a game menu or gameplay screenshot.
    check(reset()==0 and v.init(320,96,0xff000000,1)==0,'real text scene video viewport');init(None,0,0);size(16);color(0xffffffff)
    lines=[(1,0,b'SONIC ADVENTURE'),(1,1,b'Native text test')]
    for x,y,text in lines:printers['njPrintC']((x<<16)|y,C.create_string_buffer(text))
    printers['njPrintD']((1<<16)|3,120,3);printers['njPrintB']((5<<16)|3,0xaa,8);printers['njPrintH']((1<<16)|4,0xdeadbeef,8)
    lines += [(1,3,b'120'),(5,3,b'10101010'),(1,4,b'DEADBEEF')]
    text_id=Q();check(ps()==status()==0 and submit(C.byref(text_id))==0,'real C/D/B/H text scene frame')
    cfg=Config.make(640,192,1);buffer=C.create_string_buffer(640*192*4)
    check(bound(gpu.handle,text_id,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0,'actual readable text scene GPU draw')
    atlas=image_oracle(dict(kind=0x100,w=128,h=128,pixels=rawfont),128,128);expected=bytearray(b'\0\0\0\xff'*(640*192))
    for x,y,text in lines:
        for i,ch in enumerate(text):
            for py in range(32):
                for px in range(32):
                    sx=(ch%16)*8+px//4;sy=(ch//16)*8+py//4;source=(sy*128+sx)*4;target=((y*32+py)*640+(x+i)*32+px)*4
                    if atlas[source+3]:expected[target:target+4]=atlas[source:source+4]
    check(buffer.raw==expected,'every readable scene pixel matches independent glyph placement/scale oracle');images+=1;pixels+=640*192
    window=P();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,640,192,1,C.byref(window),C.byref(err))==0 and bind(gl,'jp_window_render_frame',I,P,Q,P,P,P)(window,text_id,C.byref(cfg),C.byref(report),C.byref(err))==0,'actual readable scene window/backbuffer')
    actual=C.create_string_buffer(len(buffer));check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,actual,len(actual),C.byref(err))==0 and actual.raw==buffer.raw,'actual readable scene backbuffer equals oracle');shot=out/'native-original-text-scene.png';png(shot,actual.raw,640,192);shots.append(dict(path=str(shot),sha256=sha(shot),actual_window_backbuffer=True,original_print_functions=True,retail_derived=True,gameplay=False))
    check(bind(gl,'jp_window_present',I,P,U,P)(window,0,C.byref(err))==0,'actual readable scene Present');bind(gl,'jp_window_close',None,P)(window)
    exitprint();check(release(text_id.value)==0 and pinned()==orphan()==0,'text scene and font release completely')
    gpu.close(gpu.handle);qs();t.shutdown();check(fs()==0,'shared owners fully drained');v.shutdown()
    groups['actual_original_font_glyph_GPU_and_exit_lifetime']=checks-sum(groups.values())
    result=dict(status='genuine_print_init_character_string_integer_packets_and_actual_glyph_pixels_verified',checks=checks,original_entry_calls=calls,original_functions_seen=sorted(seen),explicit_guest_init_service_calls=services,print_character_string_integer_or_context_hooks_used=False,defined_original_packet_words=defined,packet_blocks_compared=packets,coordinate_cases=len(cases),actual_GPU_images=images,pixels_compared=pixels,actual_Presents=len(shots),actual_screenshots=shots,font_pixels_private=True,Texture_Off_zero_sampling_verified=True,genuine_float_and_variadic_formatting_implemented=False,whole_system_boot_or_gameplay_verified=False,playable=False,overall_completion_percent=10,reference_hashes=refhash,source_original_binary_sha256=sha(a.binary),source_symbols_sha256=sha(a.upstream/'config/1st_read/symbols.txt'),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),groups=groups)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
if __name__=='__main__':main()
