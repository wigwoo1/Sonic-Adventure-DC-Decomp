"""Compare SDK-facing native queue contracts to pinned Japanese machine code.

The historical subset executor remains untouched. This test adds explicit PREF
observation and routes MOV.L predecrement stores through its MMIO service. It does not
model GPU processing, cache timing, store-queue arbitration or rasterization.
Native addresses and guest store-queue aliases are compared by list and offset.
"""
import argparse, ctypes as C, hashlib, inspect, json, random, re, struct, sys
from pathlib import Path

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for name in ['workspace','upstream','build','binary','report']:
        ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure'
    sys.path.insert(0,str(p))
    import verify_startup as executor
    historical=json.loads((p/'analysis/jp_ninja_math_v2_verification.json').read_text())
    assert sha(a.binary)==historical['original_binary_sha256']
    symbols=a.upstream/'config/1st_read/symbols.txt'
    assert sha(symbols)==historical['original_symbols_sha256']
    table={name:int(address,16) for address,name in re.findall(
        r'^(0x[0-9a-fA-F]+)\s+(\S+)',symbols.read_text(),re.M)}
    build=json.loads((a.build/'build_verification.json').read_text())
    for name,expected in build['authored_source_sha256'].items():assert sha(r/name)==expected,name
    dll_path=a.build/'jp_native_queue.dll'
    assert sha(dll_path)==build['artifacts_sha256']['jp_native_queue.dll']
    original=inspect.getsource(executor.reference)
    signature='cache_purge=None, trace=None):'
    anchor='        elif word & 0xF0FF == 0x00B3:'
    assert original.count(signature)==original.count(anchor)==1
    extended=original.replace(signature,'cache_purge=None, trace=None, cache_prefetch=None):')
    extended=extended.replace(anchor,
        "        elif word & 0xF0FF == 0x0083:\n"
        "            assert cache_prefetch is not None, 'PREF requires explicit service'\n"
        "            cache_prefetch(regs[n])\n"+anchor)
    # The historical predecrement long store directly addresses RAM. Preserve
    # its register/value semantics while using the existing RAM/MMIO dispatcher.
    store_anchor="            struct.pack_into('<I', ram, index(regs[n]), regs[m])"
    assert extended.count(store_anchor)==1
    extended=extended.replace(store_anchor,'            write32(regs[n],regs[m])')
    namespace=dict(vars(executor));exec(compile(extended,'<queue-pref-reference>','exec'),namespace)
    reference=namespace['reference'];ram=bytearray(16<<20);binary=a.binary.read_bytes()
    ram[0x10000:0x10000+len(binary)]=binary
    BASE=0x8c000000;CTX=0x8cd00000;CURSORS=0x8cd00200
    def store(address,values):struct.pack_into('<'+'I'*len(values),ram,address-BASE,*values)
    def word(address):return struct.unpack_from('<I',ram,address-BASE)[0]
    # The guest SDK header has 36 four-byte fields before GLOBALPARAMBUFFER.
    # The original load/store instruction addresses are tested against this
    # layout on every Get/Set call; the native ABI intentionally differs.
    CTX_HEADER=CTX+36*4
    param=table['_nj_parameterkm_']
    store(table['_nj_current_ctx_'],[CTX]);store(table['_nj_vertex_buffer_'],[CURSORS])
    lib=C.CDLL(str(dll_path))
    def bind(name,result,*args):
        f=getattr(lib,name);f.restype=result;f.argtypes=list(args);return f
    u32=C.c_uint32;ptr=C.c_void_p
    init=bind('jp_queue_initialize',C.c_int,C.c_size_t)
    reset=bind('jp_queue_reset_frame',C.c_int)
    reserve=bind('jp_queue_reserve',C.c_int,C.c_uint,u32)
    setctx=bind('jp_queue_set_context',C.c_int,C.c_uint,C.POINTER(u32))
    getctx=bind('jp_queue_get_context',C.c_int,C.c_uint,C.POINTER(u32))
    getparam=bind('jp_queue_get_parameter',C.c_int,C.POINTER(u32))
    data=bind('jp_queue_data',ptr,C.c_uint)
    cursor=bind('jp_queue_parameter_cursor',ptr,C.c_uint)
    descriptor=bind('jp_queue_descriptor_cursor',ptr,C.c_uint)
    status=bind('jp_queue_status',C.c_int)
    get=bind('njGetkmPointer',None);put=bind('njSetkmPointer',None)
    attr=bind('njSetkmModelAttr',None,u32);end=bind('njEndVertex',None,ptr)
    names=['njStartVertexNonTexOpaque','njStartVertexNonTexTrans',
           'njStartVertexTexOpaque','njStartVertexTexTrans']
    starts=[bind(n,ptr) for n in names]
    counts={n:0 for n in ['njGetkmPointer','njSetkmPointer','njSetkmModelAttr','njEndVertex',*names]}
    checks=0;instruction_addresses=set();rng=random.Random(0x1007cc)
    mmio={0xff000038:0,0xff00003c:0};prefetches=[]
    def compare(condition,description):
        nonlocal checks
        checks+=1;assert condition,description
    # Compare every source/destination-register combination against the old
    # executor on RAM, then test the new MMIO route against the same result.
    for n in range(16):
        for m in range(16):
            fixture=bytearray(4096);struct.pack_into('<HHH',fixture,0,0x2006|(n<<8)|(m<<4),11,9)
            old=executor.State();old.r[:]=[0x12340000+i for i in range(16)]
            old.r[n]=0x8c000204;old.pc=0x8c000000;old.pr=0x8c000006
            new=executor.State.from_buffer_copy(bytes(old));mapped=executor.State.from_buffer_copy(bytes(old))
            first=bytearray(fixture);second=bytearray(fixture);third=bytearray(fixture)
            executor.reference(first,old,instruction_budget=3)
            reference(second,new,instruction_budget=3)
            mapped_mmio={0x8c000200:0}
            reference(third,mapped,mmio=mapped_mmio,instruction_budget=3)
            compare(bytes(old)==bytes(new)==bytes(mapped),'MOV.L predecrement register preservation')
            compare(first==second,'MOV.L predecrement RAM preservation')
            compare(mapped_mmio[0x8c000200]==struct.unpack_from('<I',first,0x200)[0],
                    'MOV.L predecrement MMIO value')
            compare(third==fixture,'MOV.L MMIO does not write RAM')
    def pref(address):
        assert address&31==0
        prefetches.append((address,[mmio[address+4*i] for i in range(8)]))
    def trace(pc,*rest):instruction_addresses.add(pc)
    def original_call(name,arg=0):
        state=executor.State();state.r[:]=[0x22440000+i for i in range(16)]
        state.r[4]=arg;state.r[15]=0x8cf00000
        state.pc=table[name];state.pr=0x8cfed000;state.sr=0x70000000
        reference(ram,state,mmio=mmio,instruction_budget=2000,cache_prefetch=pref,trace=trace)
        counts[name]+=1;return state.r[0]
    compare(init(1<<20)==0,'initialize')
    indices=[0,2,4,1,3]
    for iteration in range(256):
        compare(reset()==0,'frame reset')
        for i in range(5):compare(reserve(i,8)==0,'reserve')
        headers=[rng.getrandbits(32) for _ in range(4)]
        guest_pointers=[0x8cd10000+i*0x1000+(iteration%8)*32 for i in range(5)]
        store(CTX_HEADER,headers)
        for i,index in enumerate(indices):store(CURSORS+4*index,[guest_pointers[i]])
        compare(setctx(0,(u32*4)(*headers))==0,'set context')
        original_call('njGetkmPointer');get();compare(status()==0,'Get status')
        native_header=(u32*4)();compare(getparam(native_header)==0,'read parameters')
        compare(list(native_header)==[word(param+4*i) for i in range(4)],'Get headers')
        for i in range(5):
            compare(word(param+16+4*i)==guest_pointers[i],'Get original list index')
            compare(cursor(i)==data(i),'Get native list index')
        attributes=rng.getrandbits(32)
        original_call('njSetkmModelAttr',attributes);attr(attributes)
        compare(getparam(native_header)==0,'read material parameters')
        compare(list(native_header)==[word(param+4*i) for i in range(4)],'material bits')
        mode=iteration%4;li=mode%2;textured=mode>=2
        address=(guest_pointers[li]&0x03ffffff)|0xe0000000
        for i in range(8):mmio[address+4*i]=0xde000000+i
        prefetches.clear();guest_vertex=original_call(names[mode]);native_vertex=starts[mode]()
        compare(status()==0,'start status')
        compare(guest_vertex==address+32,'original start offset')
        compare(native_vertex==data(li)+32,'native start offset')
        compare(word(table['_nj_listtype_'])==param+16+4*li,'original selected list')
        compare(word(table['_nj_sq_base_'])==guest_pointers[li]&0xfc000000,'original list top')
        compare(mmio[0xff000038]==mmio[0xff00003c]==((guest_pointers[li]>>24)&0x1c),'QACR mapping')
        compare(len(prefetches)==1 and prefetches[0][0]==address,'one header PREF')
        native_words=struct.unpack('<8I',C.string_at(data(li),32))
        defined=4 if textured else 3
        compare(list(native_words[:defined])==prefetches[0][1][:defined],'defined header words')
        compare(list(native_words[defined:])==[0]*(8-defined),'documented native padding')
        compare(prefetches[0][1][defined:]==[0xde000000+i for i in range(defined,8)],'original padding untouched')
        vertices=iteration%7
        original_call('njEndVertex',guest_vertex+vertices*32);end(native_vertex+vertices*32)
        compare(status()==0,'end status')
        for i in range(5):
            compare(word(param+16+4*i)-guest_pointers[i]==cursor(i)-data(i),'End list-relative offset')
        original_call('njSetkmPointer');put();compare(status()==0,'Set status')
        compare(getctx(0,native_header)==0,'read context')
        compare(list(native_header)==[word(CTX_HEADER+4*i) for i in range(4)],'Set context')
        for i,index in enumerate(indices):
            compare(word(CURSORS+index*4)-guest_pointers[i]==descriptor(i)-data(i),'Set descriptor-relative offset')
    lib.jp_queue_shutdown()
    report=dict(status='native_queue_Japanese_function_contracts_verified',comparisons=checks,
                original_function_contract_comparisons=checks-1024,
                original_calls=sum(counts.values()),calls_by_function=counts,
                original_instruction_addresses_reached=len(instruction_addresses),
                original_binary_sha256=sha(a.binary),original_symbols_sha256=sha(symbols),
                original_executor_file_sha256=sha(Path(executor.__file__)),
                original_reference_function_sha256=hashlib.sha256(original.encode()).hexdigest(),
                PREF_extended_reference_sha256=hashlib.sha256(extended.encode()).hexdigest(),
                verifier_sha256=sha(Path(__file__)),dll_sha256=sha(dll_path),
                build_report_sha256=sha(a.build/'build_verification.json'),
                PREF_scope='explicit header capture; no timing or arbitration model',
                MOV_L_predecrement_extension_checks=1024,
                historical_executor_modified=False,
                native_padding_policy='zero undefined header words; compare original defined words',
                native_safety_errors_compared_to_original=False,
                hardware_validation=False,complete_renderer_linked=False,gameplay_verified=False,
                overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],checks,'comparisons;',sum(counts.values()),'original calls')

if __name__=='__main__':main()
