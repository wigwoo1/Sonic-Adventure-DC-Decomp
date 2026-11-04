"""Compare every intensity format and culled-batch cursor contract to JP code.

Complete original SDK header/Get/Set/End bodies execute without hooks. Native
discard models skipping End; zeroed discarded storage is a native ownership
policy, not a claim about console store-queue residue or GPU/cache timing.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from primitive_reference import make_reference

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    pin_path=p/'jp_port_tools/jp_polygon2d_external_inputs.json';pin=json.loads(pin_path.read_text())
    assert sha(a.binary)==pin['original_binary_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=d/'jp_native_queue.dll';assert sha(dll)==b['artifacts_sha256'][dll.name]
    sys.path.insert(0,str(p));import verify_startup as executor
    reference,reference_hashes=make_reference(executor)
    symbols={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    # These five names are absent from the 1ST_READ symbol list. Their entry
    # addresses are explicit in the pinned, matching njkm.c source comments.
    additional_entries={'njStartVertexTexPunch':0x8c658aa0,'njStartVertexTexPunchIntens':0x8c658afa,
        'njStartVertexTexPunchIntensH':0x8c658b6c,'njStartVertexTexOpaqueIntensH':0x8c658dd2,'njStartVertexTexTransIntensH':0x8c658e60}
    for n,addr in additional_entries.items():assert n not in symbols;symbols[n]=addr
    lib=C.CDLL(str(dll));U=C.c_uint32;P=C.c_void_p;I=C.c_int;S=C.c_size_t
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_queue_initialize',I,S);shutdown=bind('jp_queue_shutdown',None);reset=bind('jp_queue_reset_frame',I)
    reserve=bind('jp_queue_reserve',I,U,U);status=bind('jp_queue_status',I);clear=bind('jp_queue_clear_error',None)
    used=bind('jp_queue_committed_bytes',S,U);data=bind('jp_queue_data',P,U);cursor=bind('jp_queue_parameter_cursor',P,U);descriptor=bind('jp_queue_descriptor_cursor',P,U)
    setctx=bind('jp_queue_set_context',I,U,C.POINTER(U));getctx=bind('jp_queue_get_context',I,U,C.POINTER(U));select=bind('jp_queue_select_context',I,U)
    hints=bind('jp_queue_cache_hint_count',C.c_uint64);last=bind('jp_queue_last_cache_hint',P);discard=bind('jp_queue_discard_active',I)
    get=bind('njGetkmPointer',None);put=bind('njSetkmPointer',None);end=bind('njEndVertex',None,P)
    # list, texture, intensity, two-block header. Punch/plain is included.
    formats=[('njStartVertexNonTexOpaqueIntens',0,False,True,False),('njStartVertexNonTexTransIntens',1,False,True,False),
        ('njStartVertexTexOpaqueIntens',0,True,True,False),('njStartVertexTexTransIntens',1,True,True,False),
        ('njStartVertexTexOpaqueIntensH',0,True,True,True),('njStartVertexTexTransIntensH',1,True,True,True),
        ('njStartVertexTexPunch',2,True,False,False),('njStartVertexTexPunchIntens',2,True,True,False),('njStartVertexTexPunchIntensH',2,True,True,True)]
    starts={n:bind(n,P,*([P,P] if high else [P] if intensity else [])) for n,li,tex,intensity,high in formats}
    plain=bind('njStartVertexNonTexOpaque',P)
    rng=random.Random(0x494e5445);checks=0;calls={n:0 for n,*rest in formats};seen=set();sections={};cancellations=0;compared_words=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    BASE=0x8c000000;CTX=0x8cd00000;CURSORS=0x8cd00200;COL=0x8cd00400;OFF=0x8cd00500
    bases=[0x8cd10000+i*0x1000 for i in range(5)];indices=[0,2,4,1,3]
    def iw(addr,values):struct.pack_into('<'+'I'*len(values),ram,addr-BASE,*values)
    def word(addr):return struct.unpack_from('<I',ram,addr-BASE)[0]
    iw(symbols['_nj_current_ctx_'],[CTX]);iw(symbols['_nj_vertex_buffer_'],[CURSORS])
    mmio={0xff000038:0,0xff00003c:0};captured=[]
    for base in bases:
        at=(base&0x3ffffff)|0xe0000000
        mmio.update({addr:0xde000000+(addr&31) for addr in range(at,at+1024,4)})
    def pref(addr):captured.append((addr,[mmio[addr+4*i] for i in range(8)]))
    addresses={symbols[n]:n for n in [*starts,'njGetkmPointer','njSetkmPointer','njEndVertex']}
    def trace(pc,*rest):
        if pc in addresses:seen.add(addresses[pc])
    def original(n,a0=0,a1=0):
        s=executor.State();s.r[4]=a0;s.r[5]=a1;s.r[15]=0x8cf00000;s.pc=symbols[n];s.pr=0x8cfed000;s.sr=0x70000000
        reference(ram,s,mmio=mmio,cache_prefetch=pref,trace=trace,instruction_budget=5000);return s.r[0]
    def frame():
        check(reset()==0,'reset and guards')
        for li,j in enumerate(indices):iw(CURSORS+4*j,[bases[li]])
    check(init(1<<20)==0,'initialize')
    for iteration in range(128):
        for name,li,texture,intensity,high in formats:
            frame();blocks=2 if high else 1;vertices=iteration%4
            check(reserve(li,blocks+4)==0,'complete reservation')
            head=[rng.getrandbits(32) for _ in range(4)];colors=[rng.getrandbits(32) for _ in range(4)];offsets=[rng.getrandbits(32) for _ in range(4)]
            if iteration<16:
                classes=[0,0x80000000,0x7f800000,0xff800000,0x7fc12345,0x7f800001,1,0x80000001]
                colors=[classes[(iteration+j)%8] for j in range(4)];offsets=colors[::-1]
            ca=(U*4)(*colors);oa=(U*4)(*offsets);shared=iteration%7==0
            iw(COL,colors);iw(OFF,offsets);iw(CTX+144,head);check(setctx(0,(U*4)(*head))==0,'set context')
            get();original('njGetkmPointer');before_hint=hints();captured.clear()
            guest=original(name,COL if intensity else 0,COL if shared else OFF);calls[name]+=1
            native=starts[name](*([ca,ca if shared else oa] if high else [ca] if intensity else []))
            check(status()==0 and native==data(li)+32*blocks,'native first vertex/full pointer')
            check(native>0xffffffff and guest==((bases[li]&0x3ffffff)|0xe0000000)+32*blocks,'64-bit host, guest alias offset')
            check(hints()==before_hint+blocks and last()==data(li)+32*(blocks-1),'one/two native header hints')
            check(len(captured)==blocks,'original one/two header PREF')
            host_words=struct.unpack('<'+'I'*(8*blocks),C.string_at(data(li),32*blocks))
            for block,(addr,guest_words) in enumerate(captured):
                check(addr==((bases[li]&0x3ffffff)|0xe0000000)+32*block,'header block order')
                defined=set(range(4 if texture else 3))| (set(range(4,8)) if intensity and not high else set()) if block==0 else set(range(8))
                for j in range(8):
                    if j in defined:check(host_words[8*block+j]==guest_words[j],('defined header word',name,iteration,block,j));compared_words+=1
                    else:check(host_words[8*block+j]==0,'undefined native header words zero')
            check(bytes(ca)==struct.pack('<4I',*colors) and bytes(oa)==struct.pack('<4I',*offsets),'raw color/offset payloads preserved')
            if iteration%2:
                C.memset(native,0x6b,4*32);before=[used(i) for i in range(5)]
                check(discard()==0,'discard active batch');cancellations+=1
                check([used(i) for i in range(5)]==before,'discard has no commit')
                check(C.string_at(data(li),(blocks+4)*32)==bytes((blocks+4)*32),'discard clears uncommitted reservation')
                # The original culled path intentionally omits njEndVertex.
            else:
                if vertices:C.memset(native,0x7a,vertices*32)
                end(native+vertices*32);original('njEndVertex',guest+vertices*32)
                check(used(li)==32*(blocks+vertices),'exact header plus vertex commit')
            put();original('njSetkmPointer');check(status()==0,'commit/discard then Set')
            out=(U*4)();check(getctx(0,out)==0 and list(out)==[word(CTX+144+4*i) for i in range(4)],'context retained')
            for i,j in enumerate(indices):
                check(word(CURSORS+4*j)-bases[i]==used(i),'original/native cursor offset after skip/End')
                if data(i):check(cursor(i)==descriptor(i)==data(i)+used(i),'native descriptor/parameter cursor')
    sections['original_header_calls']=sum(calls.values());sections['culled_path_cancellations']=cancellations
    # A canceled batch must preserve earlier committed bytes and pending Set.
    frame();check(reserve(0,4)==0 and reserve(1,4)==0,'mixed reservations');get();v=plain();payload=bytes(range(32));C.memmove(v,payload,32);end(v+32)
    prefix=C.string_at(data(0),64);v=starts['njStartVertexTexTransIntensH']((U*4)(1,2,3,4),(U*4)(5,6,7,8));C.memset(v,0x77,64)
    check(discard()==0 and C.string_at(data(0),64)==prefix,'cancel preserves other committed list')
    check(used(0)==64 and used(1)==0,'mixed list used lengths')
    check(select(1)==2,'previous committed batch still needs Set');clear();put();check(status()==0 and descriptor(0)==cursor(0),'pending dirty state survives cancellation')
    check(reset()==0,'mixed discard recovered')
    # Whole-format preflight, sticky errors and guard recovery.
    check(discard()==2,'discard without active batch');clear()
    check(reserve(0,1)==0,'one-block reservation');get();before=C.string_at(data(0),32);ca=(U*4)(1,2,3,4);oa=(U*4)(5,6,7,8)
    check(not starts['njStartVertexTexOpaqueIntensH'](ca,oa) and status()==3,'H requires both header blocks')
    check(C.string_at(data(0),32)==before,'H bounds failure writes nothing');clear()
    check(not starts['njStartVertexTexOpaqueIntens'](None) and status()==1,'null color rejected');clear()
    check(not starts['njStartVertexTexOpaqueIntensH'](ca,None) and status()==1,'null offset rejected');clear()
    shutdown();check(init(4096)==0,'fresh exact allocation for guard fixture')
    check(reserve(0,4)==0,'guard reservation');get();v=starts['njStartVertexTexOpaqueIntensH'](ca,oa)
    suffix=C.c_ubyte.from_address(data(0)+128);saved=suffix.value;suffix.value^=1
    check(discard()==6 and used(0)==0,'guard failure does not commit');clear();suffix.value=saved
    check(discard()==0,'restore guard then discard');check(discard()==2,'double discard rejected');clear();put();check(reset()==0,'recover after all failures')
    shutdown();check(discard()==7,'uninitialized discard rejected')
    check(seen==set(addresses.values()),'every original SDK dependency body reached')
    report=dict(status='all_intensity_headers_and_culled_batch_contracts_verified',checks=checks,original_header_calls=sum(calls.values()),calls_by_function=calls,
        defined_header_words_compared=compared_words,culled_path_cancellations=cancellations,sections=sections,original_function_bodies_executed=sorted(seen),original_dependency_hooks_used=False,
        original_binary_sha256=sha(a.binary),external_symbols_sha256=sha(a.upstream/'config/1st_read/symbols.txt'),external_SDK_header_source_sha256=sha(a.upstream/'sdk/ninja/njkm.c'),
        additional_original_entry_addresses={n:hex(v) for n,v in additional_entries.items()},additional_address_provenance='explicit entry addresses in pinned matching sdk/ninja/njkm.c comments',
        build_report_sha256=sha(d/'build_verification.json'),DLL_sha256=sha(dll),verifier_sha256=sha(Path(__file__)),reference_hashes=reference_hashes,primitive_reference_sha256=sha(Path(__file__).with_name('primitive_reference.py')),
        all_eight_intensity_variants=True,punch_plain=True,raw_color_words_not_float_converted=True,defined_words_match_original=True,undefined_native_words_zero=True,
        cancellation_scope='same cursor/context outcome as original header start followed by Set without End; discarded host bytes are cleared',
        mixed_batch_commit_state_preserved=True,one_native_queue_owner=True,hardware_validation=False,GPU_timing_verified=False,complete_sprite_producers_verified=False,
        full_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks;',sum(calls.values()),'original header calls;',cancellations,'original culled-path contracts')

if __name__=='__main__':main()
