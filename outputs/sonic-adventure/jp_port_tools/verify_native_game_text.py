"""Original game debug-text bodies, packets, formatter and real native state.
Only store-queue/PREF writes are observed; no original game function is hooked.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Surface
from native_video_test_support import Video,Context
from print_format_reference import make_reference
U=C.c_uint32;I=C.c_int32;P=C.c_void_p;S=C.c_size_t;F=C.c_float;D=C.c_double
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(values):return struct.pack('<'+'I'*len(values),*[v&0xffffffff for v in values])
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','font-input','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes());assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure');out.mkdir()
    assert sha(a.upstream/'src/draw/text.c')==b['original_source_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;video=Video(lib);checks=calls=packets=defined=0;seen={};rng=random.Random(0x54455854)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);reset=bind(lib,'jp_queue_reset_frame',I);status=bind(lib,'jp_queue_status',I);data=bind(lib,'jp_queue_data',P,U);used=bind(lib,'jp_queue_committed_bytes',S,U);ps=bind(lib,'jp_print_status',I);clearprint=bind(lib,'jp_print_clear_error',None)
    snapshot=bind(lib,'jp_game_text_snapshot',None,P);init=bind(lib,'text_style_init',None,P,U,U);setsize=bind(lib,'text_set_size',None,C.c_uint16);setcolor=bind(lib,'text_set_color',None,U);step=I.in_dll(lib,'frame_step')
    printers={n:bind(lib,n,None,*types) for n,types in {'text_print':[I,P],'text_print_front':[I,P],'text_print_number':[I,I,I],'text_print_binary':[I,U,I],'text_print_hex':[I,U,I],'text_print_float':[I,F,I],'text_printf':[I,P]}.items()}
    check(qi(32<<20)==0 and bind(lib,'jp_frames_initialize',I,U,U,S)(8,16,32<<20)==0 and t.init(4,16,8<<20)==0 and video.init(320,128,0xff000000,1)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'real shared native owners')
    bind(lib,'njInitPrint',None,P,U,U)(None,0,0);check(ps()==0,'real original SDK font initialization')
    nativefont=Surface.from_address(bind(lib,'jp_print_font_surface',P)());fontword=t.surface_word(C.byref(nativefont));rawfont=(a.font_input/'debug-font-128x128-argb1555-twiddled.bin').read_bytes();check(C.string_at(t.surface_data(fontword),len(rawfont))==rawfont,'real authentic default font payload')
    contexts=(Context*bind(lib,'jp_queue_context_count',U)()).in_dll(lib,'_nj_context_');contextbytes=bytes(contexts)
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,refhash=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;CURSORS=0x8cd00000;TEXT=0x8cd02000;FMT=0x8cd10000;STR=0x8cd18000;LIST=0x8ce00000
    def store(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def read(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def iw(at,values):store(at,words(values))
    def word(at):return struct.unpack('<I',read(at,4))[0]
    iw(table['_nj_vertex_buffer_'],[CURSORS]);iw(CURSORS,[LIST]*5)
    for i,ctx in enumerate(contexts):store(table['_nj_context_']+i*192,bytes(ctx.prefix)+words([0])+bytes(ctx)[Context.face.offset:])
    class MMIO(dict):
        def __contains__(self,key):return 0xe0000000<=key<0xe4000000 or dict.__contains__(self,key)
        def __getitem__(self,key):return dict.get(self,key,0)
    mmio=MMIO({0xff000038:0,0xff00003c:0});captured=[]
    def pref(at):at&=~31;captured.append([mmio[at+i*4] for i in range(8)])
    entries={table[n]:n for n in [*b['original_functions_sha256'],'strlen','sprintf','vsprintf','_fmtout']}
    def trace(pc,*unused):
        if pc in entries:seen[entries[pc]]=seen.get(entries[pc],0)+1
    def original(name,args=(),fwords=(),stack=()):
        nonlocal calls
        fpu=Fpu();fpu.fpscr=0x40001|((calls%2)<<21);s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
        for i,value in enumerate(fwords):fpu.banks[(fpu.fpscr>>21)&1][4+i]=value
        if stack:iw(s.r[15],stack)
        reference(ram,s,fpu=fpu,mmio=mmio,trace=trace,cache_prefetch=pref,instruction_budget=4000000);calls+=1;check(s.r[15]==0x8cf00000,'whole original stack restored')
    original('text_style_init',[0,0,0]);init(None,0,0)
    state_names=['text_color','text_size','text_global_param','text_isp_param','text_tsp_param','text_tex_param']
    def state():outwords=(U*6)();snapshot(outwords);return list(outwords)
    check(state()==[word(table[n]) for n in state_names],'original style init and context-header cache')
    def compare(name,args,sz=16,col=0xffbfbfbf,frame_step=0):
        nonlocal packets,defined
        check(reset()==0,'empty native recording');iw(CURSORS+8,[LIST]);captured.clear();step.value=frame_step;iw(table['frame_step'],[frame_step]);setsize(sz);setcolor(col);original('text_set_size',[sz]);original('text_set_color',[col]);prior=state()
        nativeargs=list(args);guestargs=list(args);keep=[]
        if name in ['text_print','text_print_front']:
            payload=args[1];store(TEXT,payload+b'\0');guestargs[1]=TEXT;buf=C.create_string_buffer(payload);keep.append(buf);nativeargs[1]=C.cast(buf,P)
        if name=='text_print_float':original(name,[args[0],args[2]],fwords=[fw(args[1])]);nativeargs[1]=F(args[1])
        else:original(name,guestargs)
        printers[name](*nativeargs);check(ps()==status()==0,'real text transport succeeds');blob=C.string_at(data(1),used(1)) if used(1) else b''
        rows=[list(struct.unpack_from('<8I',blob,i)) for i in range(0,len(blob),32)]
        check(len(rows)==len(captured) and len(blob)==word(CURSORS+8)-LIST,'original packet count and descriptor advancement')
        for i,(actual,want) in enumerate(zip(rows,captured)):
            n=4 if i==0 else 7;check(actual[:n]==want[:n],('whole original defined packet words',name,sz,args,i,actual,want));check(actual[n:]==[0]*(8-n),'deterministic native padding');defined+=n
        check(state()==prior and bytes(contexts)==contextbytes,'draw preserves original style and all Ninja contexts');check(state()==[word(table[n]) for n in state_names],'native style state equals original');packets+=len(rows)
    for sz,col,loc in [(16,0xffbfbfbf,0),(0,0x01234567,0x7fff8000),(65535,0xffffffff,0x80007fff),(257,0x7fa04080,0x7fff7fff),(8,0xff40c0ff,0x10001)]:
        for name in ['text_print','text_print_front']:compare(name,[C.c_int32(loc).value,bytes(range(1,256))],sz,col)
    compare('text_print',[0,b'']);compare('text_print_front',[0,b''])
    for value in [0,1,-1,1234567,-999999999,2147483647,-2147483648]:
        for digits in [0,1,3,10,12]:compare('text_print_number',[0x10001,value,digits],257)
    for name,limits in [('text_print_binary',[0,1,7,31,32]),('text_print_hex',[0,1,3,7,8])]:
        for value in [0,1,0x80000000,0xffffffff,0xabcdef01]:
            for digits in limits:compare(name,[0x7fff8000,value,digits],65535)
    for value in [0.,-0.,.125,-.75,123.456,9999.999,1e-7]:
        for digit in [5,8,13]:compare('text_print_float',[0x10001,value,digit],16)
    for _ in range(20):compare('text_print_float',[0x10001,F(rng.uniform(-9999,9999)).value,8],257)
    for fmt,values in [(b'NO. %d',[('i',42)]),(b'NPC [%s]',[('s',b'SONIC')]),(b'VALUE %.2f / %08X',[('f',.125),('i',0xdeadbeef)]),(b'%d/%f/%s/%08X/%d/%f/%u/%c',[('i',1),('f',-1.25),('s',b'SONIC'),('i',0xdeadbeef),('i',-2),('f',.125),('i',0xffffffff),('i',65)])]:
        check(reset()==0,'empty formatted recording');captured.clear();iw(CURSORS+8,[LIST]);store(FMT,fmt+b'\0');guest=[];native=[];keep=[]
        for kind,value in values:
            if kind=='s':store(STR,value+b'\0');guest.append(STR);buf=C.create_string_buffer(value);keep.append(buf);native.append(C.cast(buf,P))
            elif kind=='f':guest.append(fw(value));native.append(D(value))
            else:guest.append(value&0xffffffff);native.append(I(value))
        original('text_printf',[0x10001],stack=[FMT,*guest]);printers['text_printf'](0x10001,C.create_string_buffer(fmt),*native);check(ps()==status()==0,'real shared original formatter succeeds')
        blob=C.string_at(data(1),used(1));rows=[list(struct.unpack_from('<8I',blob,i)) for i in range(0,len(blob),32)];check(len(rows)==len(captured),'formatted packet count')
        for i,(actual,want) in enumerate(zip(rows,captured)):n=4 if not i else 7;check(actual[:n]==want[:n],('original variadic glyph words',fmt,i,actual,want));defined+=n
        packets+=len(rows)
    for name,args in [('text_print',[0,b'NO DRAW']),('text_print_front',[0,b'NO DRAW']),('text_print_number',[0,123,5]),('text_print_binary',[0,123,8]),('text_print_hex',[0,123,8]),('text_print_float',[0,1.25,5])]:compare(name,args,frame_step=1)
    # Invalid pointers are never touched while frame-step suppression applies.
    check(reset()==0,'suppression reset');step.value=1;before=state();printers['text_print'](0,None);printers['text_printf'](0,None);check(used(1)==0 and state()==before and ps()==0,'suppressed strings/format skip argument access')
    step.value=0;check(reset()==0,'invalid-input recording reset')
    for name,args in [('text_print',[0,None]),('text_print_binary',[0,1,33]),('text_print_hex',[0,1,9]),('text_print_number',[0,-1,-1])]:
        printers[name](*args);check(ps()==1 and used(1)==0,'bounded invalid argument rejects before queue mutation');clearprint()
    original('text_exit');bind(lib,'text_exit',None)();check(t.surface_data(fontword) and C.string_at(t.surface_data(fontword),len(rawfont))==rawfont,'text exit preserves Ninja-owned font')
    check(set(b['original_functions_sha256'])<=set(seen),'all thirteen original family bodies executed')
    bind(lib,'jp_queue_shutdown',None)();bind(lib,'njExitPrint',None)();t.shutdown();check(bind(lib,'jp_frames_shutdown',I)()==0,'real resources drained');video.shutdown()
    result=dict(status='complete_original_game_debug_text_packets_state_and_formatter_verified',checks=checks,original_entry_calls=calls,original_functions_seen=seen,original_game_or_format_function_hooks_used=False,defined_original_packet_words=defined,packet_blocks_compared=packets,frame_step_suppression_verified=True,original_unsafe_float_padding_rejected_by_existing_boundary=True,retail_font_private=True,source_binary_sha256=sha(a.binary),original_source_sha256=b['original_source_sha256'],build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),reference_hashes=refhash,whole_gameplay_or_localization_verified=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',calls,'original entries',flush=True)
if __name__=='__main__':main()
