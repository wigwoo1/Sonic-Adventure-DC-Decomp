"""Exercise real native queue storage, SDK cursor protocol and failure recovery."""
import argparse, ctypes as C, hashlib, json, random, struct
from pathlib import Path

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve(); b=a.build.resolve()
    build=json.loads((b/'build_verification.json').read_text())
    assert build['status']=='native_queue_linked_behavior_unverified'
    for name, expected in build['authored_source_sha256'].items():
        assert sha(r/name)==expected, name
    assert sha(b/'jp_native_queue.dll')==build['artifacts_sha256']['jp_native_queue.dll']
    dll=C.CDLL(str(b/'jp_native_queue.dll'))
    def bind(name, result, *args):
        f=getattr(dll,name); f.restype=result; f.argtypes=list(args); return f
    size=C.c_size_t; u32=C.c_uint32; ptr=C.c_void_p; integer=C.c_int
    init=bind('jp_queue_initialize',integer,size)
    shutdown=bind('jp_queue_shutdown',None)
    reset=bind('jp_queue_reset_frame',integer)
    reserve=bind('jp_queue_reserve',integer,C.c_uint,u32)
    status=bind('jp_queue_status',integer)
    clear=bind('jp_queue_clear_error',None)
    allocated=bind('jp_queue_allocated_bytes',size)
    used=bind('jp_queue_committed_bytes',size,C.c_uint)
    data=bind('jp_queue_data',ptr,C.c_uint)
    capacity=bind('jp_queue_active_capacity_blocks',size)
    setctx=bind('jp_queue_set_context',integer,C.c_uint,C.POINTER(u32))
    select=bind('jp_queue_select_context',integer,C.c_uint)
    getctx=bind('jp_queue_get_context',integer,C.c_uint,C.POINTER(u32))
    getparam=bind('jp_queue_get_parameter',integer,C.POINTER(u32))
    contexts=bind('jp_queue_context_count',C.c_uint)
    cursor=bind('jp_queue_parameter_cursor',ptr,C.c_uint)
    descriptor=bind('jp_queue_descriptor_cursor',ptr,C.c_uint)
    hints=bind('jp_queue_cache_hint_count',C.c_uint64)
    hint=bind('jp_queue_last_cache_hint',ptr)
    get=bind('njGetkmPointer',None); put=bind('njSetkmPointer',None)
    attr=bind('njSetkmModelAttr',None,u32)
    end=bind('njEndVertex',None,ptr)
    names=['njStartVertexNonTexOpaque','njStartVertexNonTexTrans',
           'njStartVertexTexOpaque','njStartVertexTexTrans']
    starts=[bind(n,ptr) for n in names]
    checks=0; groups={}; rng=random.Random(0x53415155)
    def check(condition):
        nonlocal checks
        checks+=1
        if not condition: raise AssertionError(f'queue check {checks}')
    def words(): return (u32*4)(*(rng.getrandbits(32) for _ in range(4)))
    def readctx(index):
        v=(u32*4)(); check(getctx(index,v)==0); return list(v)
    def readparam():
        v=(u32*4)(); check(getparam(v)==0); return list(v)

    check(status()==7); check(init(0)==1); clear(); check(status()==7)
    # A reserved block includes one header; exact-end writes must remain valid.
    check(init(128)==0); check(reserve(0,2)==0); check(allocated()==128)
    before=(data(0),cursor(0),descriptor(0),allocated())
    check(reserve(0,3)==4)
    check((data(0),cursor(0),descriptor(0),allocated())==before)
    check(reserve(1,1)==4); clear()
    get(); vertex=starts[0](); check(vertex==data(0)+32); check(capacity()==1)
    payload=bytes(range(32)); C.memmove(vertex,payload,32)
    check(reserve(0,1)==2); clear()
    end(vertex+1); check(status()==3); check(used(0)==0); clear()
    end(vertex-32); check(status()==3); clear()
    end(vertex+64); check(status()==3); clear()
    end(vertex+32); check(status()==0); check(used(0)==64)
    check(C.string_at(data(0)+32,32)==payload)
    check(descriptor(0)==before[2]); check(cursor(0)==vertex+32)
    get(); check(status()==2); clear(); put(); check(status()==0)
    check(descriptor(0)==cursor(0)); get(); check(status()==0)
    end(cursor(0)); check(status()==2); clear()
    check(reset()==0); check(used(0)==0); check(C.string_at(data(0),64)==bytes(64))
    check(init(192)==0);check(reserve(0,2)==0)
    before=(data(0),cursor(0),descriptor(0),allocated())
    # A 160-byte replacement fits the final budget, but old+new would not.
    check(reserve(0,3)==4)
    check((data(0),cursor(0),descriptor(0),allocated())==before)
    clear();check(reserve(1,0xffffffff)==4);clear()
    groups['budget_exact_end_invalid_pointer_and_recovery']=checks

    # Interleave both polygon lists and contexts; compare all committed bytes
    # to independently assembled packets through repeated real reallocations.
    start_checks=checks; check(init(2**20)==0)
    expected=[bytearray() for _ in range(5)]
    native_addresses=[]; expected_hints=0
    for iteration in range(256):
        mode=iteration%4; list_index=mode%2; opaque=list_index==0
        textured=mode>=2; context=iteration%contexts()
        header=words(); check(setctx(context,header)==0); check(select(context)==0)
        get(); check(status()==0); check(readparam()==list(header))
        material=rng.getrandbits(32); attr(material)
        # Material flags select blend, flip, clamp, filter, supersampling, D.
        fields=[(26,6),(17,2),(15,2),(13,2),(12,1),(8,4)]
        tsp=int(header[2])
        for shift,width in fields:
            mask=((1<<width)-1)<<shift
            tsp=(tsp&~mask)|(material&mask)
        header[2]=tsp
        n=iteration%7
        check(reserve(list_index,n+1)==0)
        # Reserve may relocate the descriptor; capture it after growth.
        old_descriptor=descriptor(list_index)
        previous_base=data(list_index)
        check(C.string_at(previous_base,len(expected[list_index]))==expected[list_index])
        vertex=starts[mode](); native_addresses.append(vertex)
        check(vertex>0xffffffff); check(vertex%32==0)
        check(vertex==previous_base+len(expected[list_index])+32)
        check(capacity()==n)
        payload=rng.randbytes(n*32)
        if payload:C.memmove(vertex,payload,len(payload))
        check(starts[mode]() is None); check(status()==2); clear()
        check(reset()==2); clear()
        end(vertex+len(payload)); check(status()==0)
        tsp_out=tsp
        if opaque:
            # Set SRC=ONE, DST=ZERO, ALPHA=0, IGNORE_ALPHA=1.
            tsp_out=(tsp_out&((1<<26)-1))|(1<<29)
            tsp_out=(tsp_out&~(1<<20))|(1<<19)
        packet=struct.pack('<8I',header[0],header[1],tsp_out,
                           header[3] if textured else 0,0,0,0,0)+payload
        expected[list_index]+=packet
        check(used(list_index)==len(expected[list_index]))
        check(C.string_at(data(list_index),used(list_index))==expected[list_index])
        check(descriptor(list_index)==old_descriptor)
        check(cursor(list_index)==vertex+len(payload))
        expected_hints+=1; check(hints()==expected_hints)
        check(hint()==vertex-32)
        put(); check(status()==0); check(readctx(context)==list(header))
        check(descriptor(list_index)==cursor(list_index))
        get(); check(readparam()==list(header))
    check(reset()==0)
    for i in range(5):
        check(used(i)==0); check(cursor(i)==data(i)); check(descriptor(i)==data(i))
        if expected[i]:check(C.string_at(data(i),len(expected[i]))==bytes(len(expected[i])))
    groups['packet_context_cursor_and_growth_checks']=checks-start_checks

    start_checks=checks
    # All five lists have separately owned storage, even though only the four
    # selected model-renderer start functions are implemented at this stage.
    for i in range(5): check(reserve(i,2)==0)
    addresses=[data(i) for i in range(5)]
    check(len(set(addresses))==5)
    for i in range(5):check(addresses[i]%32==0)
    check(reserve(5,1)==1); clear()
    check(select(contexts())==1); clear()
    get(); vertex=starts[0](); check(vertex is not None)
    # Corrupt then restore a byte of allocated guard storage, never unowned RAM.
    prefix=C.c_ubyte.from_address(data(0)-1); saved=prefix.value; prefix.value=0
    end(vertex); check(status()==6); clear(); prefix.value=saved
    end(vertex); check(status()==0); put(); check(reset()==0)
    suffix=C.c_ubyte.from_address(data(2)+64); saved=suffix.value; suffix.value=0
    check(reset()==6); clear(); suffix.value=saved; check(reset()==0)
    shutdown(); check(status()==7); check(allocated()==0)
    check(reserve(0,1)==7); clear(); check(status()==7)
    check(init(1024)==0); get(); check(starts[0]() is None); check(status()==3)
    clear(); check(reserve(0,0)==0); check(starts[0]() is None); check(status()==3)
    shutdown()
    groups['list_isolation_guards_and_shutdown_checks']=checks-start_checks
    report=dict(status='native_queue_host_contracts_verified',checks=checks,groups=groups,
                packet_batches=256,pointer_bits=C.sizeof(ptr)*8,
                all_producer_pointers_above_4GB=all(p>0xffffffff for p in native_addresses),
                dll_sha256=sha(b/'jp_native_queue.dll'),build_report_sha256=sha(b/'build_verification.json'),
                verifier_sha256=sha(Path(__file__)),original_instruction_comparison=False,
                complete_renderer_linked=False,gameplay_verified=False,
                raw_writers_require_pre_reserved_producer_bound=True,
                overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],checks,'checks')

if __name__=='__main__':main()
