"""Whole original formatter/float print, native varargs/bounds and GPU output."""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Surface
from native_video_test_support import Video,Context
from native_gpu_test_support import GPU,Config,Report,Failure
from native_pvm_test_support import image_oracle
from print_format_reference import make_reference
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64;F=C.c_float;D=C.c_double
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def floatword(v):return F.from_buffer_copy(words([v]))
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','font-input','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['format_external_sources_sha256'].items():assert sha(a.upstream/n)==h,n
    for n in ['frames_DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.upstream/'config/1st_read/symbols.txt')==pin['files_sha256']['config/1st_read/symbols.txt']
    font=(a.font_input/'debug-font-128x128-argb1555-twiddled.bin').read_bytes();assert hashlib.sha256(font).hexdigest()==b['private_font_payload_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);checks=calls=packets=defined=images=pixels=0;seen={};groups={};shots=[];rng=random.Random(0x464d5446)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);status=bind(lib,'jp_queue_status',I);data=bind(lib,'jp_queue_data',P,U);used=bind(lib,'jp_queue_committed_bytes',S,U)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    init=bind(lib,'njInitPrint',None,P,U,U);size=bind(lib,'njPrintSize',None,C.c_uint16);color=bind(lib,'njPrintColor',None,U);pf=bind(lib,'njPrintF',None,I,F,I);pv=bind(lib,'njPrint',None,I,P);ps=bind(lib,'jp_print_format_status',I);fmt=bind(lib,'jp_print_format',I,P,S,P)
    contexts=(Context*bind(lib,'jp_queue_context_count',U)()).in_dll(lib,'_nj_context_')
    check(qi(32<<20)==0 and fi(8,16,32<<20)==0 and t.init(4,16,8<<20)==0 and v.init(320,96,0xff000000,1)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'actual queue/frame/font/video owners')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_widen_reference import widen
    from sh4_binary32 import round_dyadic
    reference,refhash=make_reference(executor)
    table={n:int(x,16) for x,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    CURSORS=0x8cd00000;FMT=0x8cd02000;ARGV=0x8cd04000;DEST=0x8cd08000;STR=0x8cd10000;COUNT=0x8cd18000;LIST=0x8ce00000
    def store(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def read(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def iw(at,raw):store(at,words(raw))
    def word(at):return struct.unpack('<I',read(at,4))[0]
    iw(table['_nj_vertex_buffer_'],[CURSORS]);iw(CURSORS,[LIST]*5)
    for i,ctx in enumerate(contexts):store(table['_nj_context_']+i*192,bytes(ctx.prefix)+words([0])+bytes(ctx)[Context.face.offset:])
    class MMIO(dict):
        def __contains__(self,key):return 0xe0000000<=key<0xe4000000 or dict.__contains__(self,key)
        def __getitem__(self,key):return dict.get(self,key,0)
    mmio=MMIO({0xff000038:0,0xff00003c:0});captured=[];init_services={'kmiTXalloc':0,'kmiCopyToFrameBuffer':0}
    tracked=['njInitPrint','njPrintF','njPrint','njPrintC','njCharacter','sprintf','vsprintf','_fmtout','_xtostr','_xti','_its','_log10','_unpack','_calcnpw','_power','_pow10','_pow5','_div64','_mult64','_mult','_rnd','strlen','memcpy','memcmp']
    entries={table[n]:n for n in tracked}
    def trace(pc,*rest):
        if pc in entries:n=entries[pc];seen[n]=seen.get(n,0)+1
    def pref(at):
        assert at&0xf0000000==0xe0000000,hex(at)
        at&=~31;captured.append([mmio[at+i*4] for i in range(8)])
    def alloc(ram,regs,sr):init_services['kmiTXalloc']+=1;iw(regs[5]+28,[8]);regs[0]=0;return sr
    def copy(ram,regs,sr):init_services['kmiCopyToFrameBuffer']+=1;check(read(regs[5],regs[6])==font,'actual guest original font copy input');regs[0]=0;return sr
    hooks={table['kmiTXalloc']:alloc,table['kmiCopyToFrameBuffer']:copy}
    def original(name,args=(),fwords=(),stack=(),hooks=None):
        nonlocal calls
        f=Fpu();f.fpscr=0x40001|((calls%2)<<21);s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,val in enumerate(args):s.r[4+i]=val&0xffffffff
        for i,val in enumerate(fwords):f.banks[(f.fpscr>>21)&1][4+i]=val
        if stack:iw(s.r[15],stack)
        reference(ram,s,fpu=f,mmio=mmio,hooks=hooks,trace=trace,cache_prefetch=pref,instruction_budget=4000000);calls+=1;check(s.r[15]==0x8cf00000,'whole original stack restored');return s
    original('njInitPrint',(0,0,0),hooks=hooks);init(None,0,0);nativefont=Surface.from_address(bind(lib,'jp_print_font_surface',P)());fontword=t.surface_word(C.byref(nativefont));check(t.status()==status()==0,'genuine native default font')
    narrow=bind(lib,'jp_print_format_narrow_word',U,Q);wide=bind(lib,'jp_print_format_widen_word',Q,U);scaled=bind(lib,'jp_print_format_scaled_exponent',I,I)
    inputs=[0,0x80000000,1,0x7fffff,0x800000,0x7f7fffff,0x7f800000,0xff800000,0x7f800001,0x7fc00000,0xffffffff]+[sign|e<<23|frac for sign in [0,0x80000000] for e in range(256) for frac in [0,1,0x3fffff,0x7fffff]]+[rng.getrandbits(32) for _ in range(4096)]
    for raw in inputs:
        f=Fpu();f.fpscr=0x40001|0x80000;check(widen(f,raw,4)==0 and wide(raw)==(f.banks[0][4]<<32)|f.banks[0][5],'native widening equals independent existing manual-based FCNVSD DN1 oracle')
    def narrow_oracle(raw):
        exp=(raw>>52)&2047;frac=raw&0xfffffffffffff;sgn=raw>>63
        if exp==2047:return 0x7fbfffff if frac else (sgn<<31)|0x7f800000
        result=round_dyadic((-1 if sgn else 1)*(frac if not exp else frac+(1<<52)),-1074 if not exp else exp-1075,1)[0]
        if result&0x7fffffff<0x800000:result=sgn<<31
        return result
    double_inputs=[0,1,0x8000000000000000,0x7fefffffffffffff,0x7ff0000000000000,0xfff0000000000000,0x7ff0000000000001,0x7ff8000000000000]+[rng.getrandbits(64) for _ in range(8192)]
    for raw in double_inputs:check(narrow(raw)==narrow_oracle(raw),'independent integer dyadic RZ binary64->single/DN1 value')
    for exp in range(-125,129):
        raw=struct.unpack('<Q',struct.pack('<d',2.**(exp-1)))[0];s=original('_log10',fwords=[raw>>32,raw&0xffffffff]);check(scaled(exp)==C.c_int32(s.r[0]).value,'all possible widened-single frexp exponents match whole original binary64 scale estimate')
    groups['native_value_conversions_and_complete_original_exponent_domain']=checks
    # Test full original _fmtout directly with ample caller-owned output storage.
    # Native argument pointers are relocated to corresponding real guest RAM.
    format_cases=[]
    for spec in [b'%d',b'%i',b'%u',b'%o',b'%x',b'%X',b'%+08d',b'%-05d',b'%08.3d',b'%#.5x',b'%#06x',b'%.0d',b'%hd',b'%hu',b'%ld',b'%lu']:
        for value in [0,1,42,0xffffffff,0x80000000,0x7fffffff]:
            if spec==b'%#.5x' and value==0:continue
            format_cases.append((spec,[('i',value)]))
    for spec in [b'%f',b'%.2f',b'%08.2f',b'%+8.2f',b'%e',b'%.4E',b'%g',b'%#g',b'% .4g',b'%-012.5f']:
        for value in [0.,-0.,0.125,-1.5,0.0001,0.00001,123.456,1e10,1e-20]:format_cases.append((spec,[('f',fw(value))]))
    for value in [0x00800000,0x7f7fffff,1,0x80000001,0x7f800000,0xff800000,0x7fbfffff,0x7fc00001]:
        for spec in [b'%e',b'%g',b'%f']:format_cases.append((spec,[('f',value)]))
    format_cases += [(b'NPC [%s]',[('s',b'SONIC')]),(b'%-08.3s',[('s',b'TAILS')]),(b'%c:%5%', [('i',0x80)]),(b'%*.*f',[('i',12),('i',5),('f',fw(0.125))]),(b'%*s',[('i',-8),('s',b'A')]),(b'%p',[('p',0xdeadbeef)]),(b'COUNT%n/%08X',[('n',0),('i',0x89abcdef)]),(b'GINDEX : %08X(%d)',[('i',0xdeadbeef),('i',-123)])]
    for text,values in format_cases:
        store(FMT,text+b'\0');guest=[];native=[];keep=[];string_slot=0;native_count=I(-1);iw(COUNT,[0xffffffff])
        for kind,value in values:
            if kind=='s':
                at=STR+string_slot*2048;string_slot+=1;store(at,value+b'\0');buf=C.create_string_buffer(value);keep.append(buf);guest.append(at);native.append(C.cast(buf,P))
            elif kind=='f':guest.append(value);native.append(D(floatword(value).value))
            elif kind=='n':guest.append(COUNT);native.append(C.cast(C.byref(native_count),P))
            elif kind=='p':guest.append(value);native.append(P(value))
            else:guest.append(value&0xffffffff);native.append(I(value))
        iw(ARGV,guest);store(DEST,b'\xa5'*2048);s=original('_fmtout',(1,0,DEST,FMT),stack=[ARGV]);length=C.c_int32(s.r[0]).value;arena=C.create_string_buffer(b'\xa5'*2080);pointer=C.addressof(arena)+16
        count=fmt(pointer,2048,C.create_string_buffer(text),*native)
        check(count==length and ps()==0 and C.string_at(pointer,count+1)==read(DEST,length+1),('complete original/native formatter text/count',text,values,count,length,C.string_at(pointer,max(0,count)+1),read(DEST,max(0,length)+1)))
        check(arena.raw[:16]==b'\xa5'*16 and arena.raw[16+count+1:2080]==b'\xa5'*(2080-16-count-1),'complete caller output guards/padding preserved')
        if any(k=='n' for k,_ in values):check(native_count.value==C.c_int32(word(COUNT)).value,'original %n count stored through native-width argument pointer')
    groups['complete_original_formatter_strings_integer_float_and_varargs']=checks-sum(groups.values())
    # Entire genuine njPrintF and njPrint include real formatter and character
    # dependencies. Compare every defined packet word and descriptor movement.
    def packet_check(label):
        nonlocal packets,defined
        blob=C.string_at(data(1),used(1));rows=[list(struct.unpack_from('<8I',blob,i)) for i in range(0,len(blob),32)];check(status()==0 and len(rows)==len(captured) and len(blob)==word(CURSORS+8)-LIST,'complete original/native print packet count')
        for i,(actual,want) in enumerate(zip(rows,captured)):
            n=4 if not i else 7
            if not i:want=list(want);want[3]=(want[3]&~0x1fffff)|fontword
            check(actual[:n]==want[:n],('complete original formatted print packet',label,i,actual,want));defined+=n
        packets+=len(rows)
    fixed_cases=[]
    for raw in [fw(0),0x80000000,fw(.125),fw(-.75),fw(123.456),fw(9999.999),fw(0.0000001),1,0x80000001,0x00800000]:
        for digit in [5,8,13]:fixed_cases.append((raw,digit))
    fixed_cases += [(fw(rng.uniform(-9999,9999)),8) for _ in range(64)]
    for raw,digit in fixed_cases:
        check(reset()==0,'fixed print native reset');captured.clear();iw(CURSORS+8,[LIST]);size(257);color(0xff40c0ff);original('njPrintSize',(257,));original('njPrintColor',(0xff40c0ff,));original('njPrintF',(0x7fff8000,digit),fwords=[raw]);pf(0x7fff8000,floatword(raw),digit);check(ps()==0,'valid native fixed print');packet_check((hex(raw),digit))
    variadic_cases=[(b'NO. %d',[('i',42)]),(b'NPC [%s]',[('s',b'SONIC')]),(b'VALUE %.2f / %08X',[('f',fw(.125)),('i',0xdeadbeef)]),(b'%d/%f/%s/%08X/%d/%f/%u/%c',[('i',1),('f',fw(-1.25)),('s',b'SONIC'),('i',0xdeadbeef),('i',-2),('f',fw(.125)),('i',0xffffffff),('i',65)])]
    for text,values in variadic_cases:
        check(reset()==0,'variadic native reset');captured.clear();iw(CURSORS+8,[LIST]);store(FMT,text+b'\0');guest=[];native=[];keep=[]
        for kind,value in values:
            if kind=='s':store(STR,value+b'\0');guest.append(STR);buf=C.create_string_buffer(value);keep.append(buf);native.append(C.cast(buf,P))
            elif kind=='f':guest.append(value);native.append(D(floatword(value).value))
            else:guest.append(value&0xffffffff);native.append(I(value))
        original('njPrint',(0x7fff8000,),stack=[FMT,*guest]);pv(0x7fff8000,C.create_string_buffer(text),*native);check(ps()==0,'native Windows varargs print');packet_check(text)
    groups['genuine_complete_float_and_variadic_print_packets']=checks-sum(groups.values())
    # Invalid original domains get bounded native errors and no queue output.
    check(reset()==0,'invalid format queue reset')
    for raw,digit in [(fw(123.4),1),(fw(1),0),(fw(1),14),(0x7f800000,5),(0xff800000,5),(0x7fc00000,5),(0x7f7fffff,13)]:
        pf(0,floatword(raw),digit);check(ps()!=0 and used(1)==0 and status()==0,'invalid fixed float padding/size/class produces no draw')
    valid_a=C.create_string_buffer(b'A')
    for text,args in [(b'%128s',[C.cast(valid_a,P)]),(b'%',[]),(b'%*c',[I(-2147483648),I(65)]),(b'%#.5x',[I(0)]),(b'%s',[P()]),(b'%q',[])]:
        buffer=C.create_string_buffer(160);check(fmt(C.byref(buffer,16),128,C.create_string_buffer(text),*args)==-1 and ps()!=0,'invalid or overlong format rejected with bounded output')
    pv(0,C.create_string_buffer(b'A'*128));check(ps()!=0 and used(1)==0,'128-byte variadic print overflow produces no draw')
    for capacity in [1,2,4,128]:
        arena=C.create_string_buffer(b'\xa5'*(capacity+32));pointer=C.addressof(arena)+16;text=b'A'*(capacity-1)
        check(fmt(pointer,capacity,C.create_string_buffer(text))==capacity-1 and C.string_at(pointer,capacity)==text+b'\0' and arena.raw[:16]==arena.raw[16+capacity:32+capacity]==b'\xa5'*16,'exact output capacity includes NUL and preserves both guards')
        check(fmt(pointer,capacity,C.create_string_buffer(text+b'A'))==-1 and ps()!=0 and arena.raw[:16]==arena.raw[16+capacity:32+capacity]==b'\xa5'*16,'one byte beyond capacity rejects without guard writes')
    arena=C.create_string_buffer(b'\xa5'*32);check(fmt(arena,0,C.create_string_buffer(b'A'))==-1 and arena.raw[:32]==b'\xa5'*32,'zero capacity rejects before any destination write')
    groups['native_undefined_domains_and_fixed_buffer_bounds']=checks-sum(groups.values())
    # Native-window screenshot with real original F/variadic caller output.
    check(reset()==0 and v.init(320,96,0xff000000,1)==0,'actual formatted text scene reset and explicit native black background');size(8);color(0xffffffff);pf((1<<16)|1,F(-.125),3);pv((1<<16)|3,C.create_string_buffer(b'VALUE %.2f  HEX %08X'),D(.125),I(0xdeadbeef));pv((1<<16)|5,C.create_string_buffer(b'NPC [%s] / NO. %d'),C.cast(C.create_string_buffer(b'SONIC'),P),I(42))
    lines=[(1,1,b'-000.12500'),(1,3,b'VALUE 0.13  HEX DEADBEEF'),(1,5,b'NPC [SONIC] / NO. 42')]
    fid=Q();check(ps()==status()==0 and submit(C.byref(fid))==0,'genuine F/variadic scene frame')
    gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;cfg=Config.make(640,192,1);buffer=C.create_string_buffer(640*192*4);report=Report();err=Failure();bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0,'actual formatted scene GPU')
    atlas=image_oracle(dict(kind=0x100,w=128,h=128,pixels=font),128,128);expected=bytearray(b'\0\0\0\xff'*(640*192))
    for x,y,text in lines:
        for i,ch in enumerate(text):
            for py in range(16):
                for px in range(16):
                    source=(((ch//16)*8+py//2)*128+(ch%16)*8+px//2)*4;target=((y*16+py)*640+(x+i)*16+px)*4
                    if atlas[source+3]:expected[target:target+4]=atlas[source:source+4]
    png(out/'formatted-scene-observed.png',buffer.raw,640,192)
    check(buffer.raw==expected,('every formatted scene pixel equals independent literal text/font oracle',sum(x!=y for x,y in zip(buffer.raw,expected))));images+=1;pixels+=640*192
    window=P();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,640,192,1,C.byref(window),C.byref(err))==0 and bind(gl,'jp_window_render_frame',I,P,Q,P,P,P)(window,fid,C.byref(cfg),C.byref(report),C.byref(err))==0,'actual formatted scene window/backbuffer');actual=C.create_string_buffer(len(buffer))
    check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,actual,len(actual),C.byref(err))==0 and actual.raw==buffer.raw,'actual formatted backbuffer equals oracle');shot=out/'native-original-formatted-text.png';png(shot,actual.raw,640,192);shots.append(dict(path=str(shot),sha256=sha(shot),actual_window_backbuffer=True,original_float_variadic_print_functions=True,retail_derived=True,gameplay=False));check(bind(gl,'jp_window_present',I,P,U,P)(window,0,C.byref(err))==0,'actual formatted scene Present');bind(gl,'jp_window_close',None,P)(window)
    cfg.textures=0;check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0 and report.texture_uploads==report.decoded_texels==report.shader_texture_bindings==report.shader_sampler_bindings==0,'formatted Textures Off samples no font');images+=1
    check(release(fid.value)==0,'formatted frame released');gpu.close(gpu.handle);bind(lib,'njExitPrint',None)();qs();t.shutdown();check(fs()==0,'all actual owners drained');v.shutdown()
    imports=subprocess.check_output([str(Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')),'--coff-imports',b['frames_DLL']],text=True);check(not re.search(r'Symbol:\s+(?:sprintf|vsprintf|snprintf|vsnprintf)\s*\(',imports),'no host CRT formatter imported')
    groups['actual_formatted_GPU_scene_Off_and_original_formatter_binding']=checks-sum(groups.values())
    result=dict(status='whole_original_formatter_float_and_native_variadic_print_verified',checks=checks,original_entry_calls=calls,original_dependency_calls=seen,guest_init_service_calls=init_services,original_formatter_or_print_function_hooks_used=False,format_cases=len(format_cases),complete_fixed_print_cases=len(fixed_cases),complete_variadic_print_cases=len(variadic_cases),defined_original_packet_words=defined,packet_blocks_compared=packets,widen_cases=len(inputs),narrow_cases=len(double_inputs),complete_original_scaled_exponent_cases=254,actual_GPU_images=images,pixels_compared=pixels,actual_Presents=len(shots),actual_screenshots=shots,Texture_Off_zero_sampling_verified=True,host_CRT_formatter_imported=False,fixed_RZ_DN1_value_policy=True,FPSCR_flags_traps_hardware_or_whole_game_context_verified=False,reference_hashes=refhash,original_binary_sha256=sha(a.binary),original_symbols_sha256=sha(a.upstream/'config/1st_read/symbols.txt'),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),reference_tool_sha256=sha(Path(__file__).with_name('print_format_reference.py')),whole_system_boot_or_gameplay_verified=False,playable=False,overall_completion_percent=10,groups=groups)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
if __name__=='__main__':main()
