"""Original allocator instruction trace, bounded arenas, native ABI and locks."""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('arena','base','reservation')]+[(n,U) for n in ('span','usable','units','live','requested','capacity','free_bytes','largest_free','owned','status','unit_bytes','header_bytes','initialized','reserved')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['original_source_sha256'].items():assert sha(u/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    d.mkdir();lib=C.CDLL(b['DLL']);create=bind(lib,'jp_sdk_heap_create',I,U);initialize=bind(lib,'jp_sdk_heap_initialize',I,P,U);shutdown=bind(lib,'jp_sdk_heap_shutdown',I)
    allocate=bind(lib,'jp_sdk_heap_allocate',P,S);zero=bind(lib,'jp_sdk_heap_callocate',P,S,S);resize=bind(lib,'jp_sdk_heap_reallocate',P,P,S);free=bind(lib,'jp_sdk_heap_free',I,P)
    statistics=bind(lib,'jp_sdk_heap_statistics',I,P,P);snapshot=bind(lib,'jp_sdk_heap_snapshot',I,P);status=bind(lib,'jp_sdk_heap_status',I)
    checks=[];groups={}
    def check(ok,label):assert ok,label;checks.append(label)
    def state():v=Info();assert snapshot(C.byref(v))==0;return v
    def stats():x=U();y=U();assert statistics(C.byref(x),C.byref(y))==0;return x.value,y.value
    check(not allocate(1) and status()==2 and free(None)==0 and shutdown()==0,'cold allocator fails allocation explicitly and permits harmless NULL free/idempotent close')
    check(all(not hasattr(lib,n) for n in ('malloc','free','realloc','calloc','_builtin_set_imask','_builtin_get_imask')),'SDK heap does not export/interpose native CRT or console interrupt primitives')
    for invalid in (0,1,63,64*1024*1024+1,0xffffffff):check(create(invalid)==1 and state().initialized==0,'invalid arena spans reject without creating native state')
    # A real C caller uses the original SDK declarations and actual import lib.
    peer=d/'sdk_heap_client.c';peer.write_text('''#include <shinobi.h>
__declspec(dllexport) void client_init(void *p,Uint32 n){syMallocInit(p,n);}
__declspec(dllexport) void *client_alloc(Uint32 n){return syMalloc(n);}
__declspec(dllexport) void client_free(void *p){syFree(p);}
__declspec(dllexport) void client_stat(Uint32 *a,Uint32 *b){syMallocStat(a,b);}
__declspec(dllexport) void client_finish(void){syMallocFinish();}
''')
    host=d/'heap_page_query.c';host.write_text('''#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
__declspec(dllexport) void page_query(uintptr_t address,unsigned *out){
 MEMORY_BASIC_INFORMATION info;SIZE_T n=VirtualQuery((void *)address,&info,sizeof info);
 out[0]=n?info.State:0;out[1]=n?info.Protect:0;out[2]=n?info.AllocationProtect:0;
}
''')
    compiler=b['commands'][0]['command'][0];commands=[
        [compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-include',str(r/'outputs/sonic-adventure/native/jp_host_types.h'),'-I'+str(u/'sdk/include'),'-c',str(peer),'-o',str(d/'sdk_client.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(host),'-o',str(d/'page_query.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sdk_client.obj'),str(d/'page_query.obj'),str(a.build/'jp_native_sdk_heap.lib'),'-Wl,kernel32.lib','-o',str(d/'sdk_heap_client.dll')]]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);(d/f'peer_build_{i}.txt').write_bytes(run.stdout+run.stderr);check(not run.returncode and not run.stdout and not run.stderr,'actual original-SDK ABI caller and native page query compile/link warning-free')
    client=C.CDLL(str(d/'sdk_heap_client.dll'));client_init=bind(client,'client_init',None,P,U);client_alloc=bind(client,'client_alloc',P,U);client_free=bind(client,'client_free',None,P);client_finish=bind(client,'client_finish',None);client_stat=bind(client,'client_stat',None,P,P);query=bind(client,'page_query',None,S,P)
    def page(at):v=(U*3)();query(at,v);return tuple(v)
    high_pointer=False
    for offset in range(32):
        buffer=(C.c_ubyte*8192)();C.memset(buffer,0xa5,len(buffer));start=C.addressof(buffer)+64+offset;span=1024+offset
        client_init(start,span);check(status()==0,'actual SDK syMallocInit accepts native borrowed arena')
        info=state();expected=(start+32)&~31;usable=(span-(expected-start))&~31
        check((info.arena,info.base,info.usable,info.units,info.owned,info.unit_bytes,info.header_bytes)==(start,expected,usable,usable//32,0,32,16),'all 32 starting alignments preserve original units/native header and bounded usable extent')
        p=client_alloc(1);high_pointer|=bool(p and p>0xffffffff)
        check(p and p%32==0 and expected+32<=p<expected+usable,'real SDK caller returns aligned full-width arena pointer')
        C.memset(p,0x57,1);client_free(p);x=U();y=U();client_stat(C.byref(x),C.byref(y))
        check(status()==0 and (x.value,y.value)==(usable-32,usable-32),'actual SDK free/stat coalesce complete borrowed arena')
        client_finish();check(status()==0 and state().initialized==0,'SDK finish tears down its native owner')
        raw=bytes(buffer);prefix=expected-C.addressof(buffer);end=prefix+usable
        check(raw[:prefix]==b'\xa5'*prefix and raw[end:]==b'\xa5'*(len(raw)-end),'every unused prefix/tail byte and both surrounding canaries remain unchanged')
    check(high_pointer,'actual C caller exchanged allocator pointers above 4GiB')
    groups['borrowed_alignment_canaries_and_real_SDK_C_caller']=len(checks)
    check(create(16384)==0,'actual guarded native heap creation');native=state();check(native.owned and native.base>0xffffffff and native.initialized,'real owned arena has native-width addresses')
    before=page(native.arena-1);after=page(native.arena+native.span);middle=page(native.arena)
    check(before[0]==after[0]==0x2000 and before[2]&255==after[2]&255==1 and middle[0]==0x1000 and middle[1]&255==4,'actual Windows committed RW arena is bounded by inaccessible reserved guard pages')
    check(create(4096)==3 and initialize(P(native.arena),native.span)==3 and state().base==native.base,'live native arena cannot be silently replaced')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,_=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
    original_names=('syMallocInit','syMallocStat','syMalloc','malloc','syFree','free','calloc');assert all(n in table for n in original_names)
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;guest=0x8ce00000;guest_base=guest+32;calls=0;seen=set()
    def put(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def read(at,n):return bytes(ram[at&0xffffff:(at&0xffffff)+n])
    def trace(pc,*unused):
        for n in original_names:
            if pc==table[n]:seen.add(n)
    def original(name,*args):
        nonlocal calls
        cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        for i,x in enumerate(args):cpu.r[4+i]=x&0xffffffff
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,trace=trace,instruction_budget=200000);calls+=1
        assert cpu.r[15]==0x8cf00000 and cpu.sr&0xf0==0,(name,hex(cpu.sr))
        return cpu.r[0]
    put(guest,b'\xa5'*native.span);original('syMallocInit',guest,native.usable)
    def original_stats():
        original('syMallocStat',0x8cf10000,0x8cf10004);return struct.unpack('<II',read(0x8cf10000,8))
    check(stats()==original_stats()==(native.usable-32,native.usable-32),'complete unhooked original init/stat use the same effective size with alignment headroom')
    rng=random.Random(0x53484d41);live={};serial=0;events=[]
    for step in range(400):
        if live and (len(live)>24 or rng.randrange(100)<43):
            key=rng.choice(list(live));p,g,size,capacity,pattern=live.pop(key)
            original('syFree',g);check(free(p)==0,'native free matches complete original free/coalescing entry');events.append(['free',key])
        else:
            calloc=rng.randrange(3)==0
            if calloc:count=rng.randrange(9);size=rng.choice((0,1,2,3,4,5,7,15,17,31));requested=count*size;p=zero(count,size);g=original('calloc',count,size)
            else:requested=rng.choice((0,1,2,3,4,31,32,33,63,64,65,127,255,511,1024,2048,8192));p=allocate(requested);g=original('syMalloc',requested)
            check(bool(p)==bool(g),'native/original allocation and real exhaustion agree')
            if p:
                capacity=((requested+31)//32)*32
                check(p-native.base==g-guest_base and p%32==0,'native and original first-fit/end-split allocation offsets agree')
                if calloc:
                    rounded=(requested+3)&~3;check(C.string_at(p,rounded)==read(g,rounded)==bytes(rounded),'complete original calloc rounded word clearing agrees within allocated capacity')
                serial+=1;pattern=bytes([serial%251+1])*capacity
                if capacity:C.memmove(p,pattern,capacity);put(g,pattern)
                live[serial]=(p,g,requested,capacity,pattern);events.append(['calloc' if calloc else 'allocate',serial,requested,p-native.base])
            else:check(status()==6,'native exhaustion is explicit and does not fabricate an allocation');events.append(['exhausted',requested])
        check(stats()==original_stats(),'all original free-byte/largest-block accounting agrees after every trace operation')
        info=state();check((info.live,info.requested,info.capacity)==(len(live),sum(x[2] for x in live.values()),sum(x[3] for x in live.values())),'independent native live ownership and requested/capacity totals')
        for p,g,size,capacity,pattern in live.values():check(C.string_at(p,capacity)==read(g,capacity)==pattern,'all live payload bytes survive other allocations/frees and coalescing')
    for p,g,size,capacity,pattern in live.values():original('syFree',g);check(free(p)==0,'actual remaining allocations released')
    check(stats()==original_stats()==(native.usable-32,native.usable-32),'full original/native arena coalesces after complete trace retirement')
    check(set(original_names).issubset(seen),'all seven original entry/dependency bodies executed without function hooks')
    reservation=native.reservation;check(shutdown()==0 and page(reservation)[0]==0x10000,'actual native reservation is released after its last allocation')
    groups['unhooked_original_machine_trace_and_actual_guarded_arena']=len(checks)-sum(groups.values())
    check(create(4096)==0,'real heap for safe resize and error cases');a0=allocate(128);b0=allocate(64);pattern=bytes(range(128));C.memmove(a0,pattern,128);C.memset(b0,0x6d,64)
    check(resize(a0,32)==a0 and C.string_at(a0,32)==pattern[:32] and state().requested==96,'safe in-place shrink preserves data and actual capacity')
    moved=resize(a0,400);check(moved and moved!=a0 and C.string_at(moved,32)==pattern[:32] and C.string_at(b0,64)==b'\x6d'*64,'native repaired growth preserves old requested prefix and neighboring allocation')
    check(free(a0)==8,'old moved allocation is no longer owned')
    old=stats();check(not resize(moved,4096) and status()==6 and stats()==old and C.string_at(moved,32)==pattern[:32],'failed native growth retains the original allocation/data/accounting')
    check(not resize(moved,C.c_size_t(-1).value) and status()==7 and C.string_at(moved,32)==pattern[:32],'resize arithmetic overflow preserves original pointer/data')
    check(not zero(C.c_size_t(-1).value,2) and status()==7 and not allocate(C.c_size_t(-1).value) and status()==7,'native size/product overflow cannot wrap into a small allocation')
    check(shutdown()==4 and state().initialized and state().live==2,'live pointers prevent teardown without losing native ownership')
    other=(C.c_ubyte*32)();check(free(C.addressof(other))==8 and free(moved+1)==8 and C.string_at(b0,64)==b'\x6d'*64,'foreign and interior pointers reject without corrupting live payloads')
    info=state();out=U(0x12345678);before=C.string_at(info.arena,info.span)
    check(statistics(None,C.byref(out))==1 and out.value==0x12345678 and statistics(C.byref(out),C.byref(out))==1 and out.value==0x12345678,'invalid/overlapping stat outputs preserve caller values')
    check(statistics(P(info.base),C.byref(out))==1 and snapshot(P(info.base))==1 and C.string_at(info.arena,info.span)==before,'output records cannot overwrite the allocator arena or its headers')
    check(not resize(moved,0) and status()==0 and free(b0)==0 and free(b0)==8,'zero-size resize frees safely and double free rejects')
    z=resize(None,0);check(z and free(z)==0,'NULL zero-size resize follows the declared zero-size allocation contract')
    foreign=[];thread=threading.Thread(target=lambda:foreign.append(shutdown()));thread.start();thread.join();check(foreign==[10] and state().initialized,'foreign thread cannot tear down the creating thread\'s heap')
    check(shutdown()==0,'safe repair/error cases close with no outstanding owners')
    groups['native_realloc_repairs_and_invalid_input_preservation']=len(checks)-sum(groups.values())
    check(create(1<<20)==0,'actual shared SDK heap for concurrent clients');failures=[];barrier=threading.Barrier(5)
    def worker(number):
        try:
            local=random.Random(number+123);barrier.wait()
            for step in range(120):
                size=local.randrange(1,1025);p=allocate(size);assert p
                data=bytes([number+1])*size;C.memmove(p,data,size)
                if step%3==0:
                    n=size+1024;q=resize(p,n);assert q and C.string_at(q,size)==data;p=q
                assert C.string_at(p,size)==data and free(p)==0
        except BaseException as error:failures.append(repr(error))
    workers=[threading.Thread(target=worker,args=(i,)) for i in range(4)]
    for thread in workers:thread.start()
    barrier.wait()
    for thread in workers:thread.join()
    info=state();check(not failures and info.live==info.requested==info.capacity==0 and stats()==(info.usable-32,info.usable-32),'four real concurrent native clients preserve data, allocator topology and complete coalescing')
    check(shutdown()==0,'concurrent arena teardown succeeds after workers retire')
    check(create((1<<20)+32)==0 and state().usable==1<<20 and stats()==((1<<20)-32,(1<<20)-32) and shutdown()==0,'32 bytes of real extra backing preserve the original full one-MiB usable heap without its end overrun')
    groups['real_native_lock_and_concurrent_callers']=len(checks)-sum(groups.values())
    # Corrupt free-list links/size and a live allocation header deliberately.
    # The validator must reject before traversing an invalid host pointer.
    for kind in ('link','free_size','allocated_size'):
        check(create(4096)==0,'fresh actual arena for malformed-header guard')
        info=state();p=allocate(32) if kind=='allocated_size' else None
        if kind=='link':P.from_address(info.base).value=1
        elif kind=='free_size':U.from_address(info.base+8).value=0xffffffff
        else:U.from_address(p-32+8).value=1
        check(not allocate(1) and status()==9,'malformed native link/header is rejected before unsafe original traversal')
        if p:
            # Restore the exact original header to make the client's ownership
            # releasable in a fresh process; corruption stays terminal by design.
            U.from_address(p-32+8).value=2
            check(free(p)==9 and shutdown()==4,'terminal corrupted live ownership is retained rather than silently discarded')
            # This case is last: OS process teardown releases its test arena.
        else:check(shutdown()==0,'empty corrupted arena can be safely retired without traversing its free list')
    groups['corrupt_header_and_link_rejection']=len(checks)-sum(groups.values())
    result=dict(status='original_SDK_heap_machine_trace_bounded_native_arenas_safe_repairs_ABI_and_real_lock_verified',checks=len(checks),groups=groups,
        complete_original_machine_calls=calls,original_function_entries_seen=sorted(seen),original_function_hooks_used=False,trace_operations=400,trace_events=events,
        all_32_borrowed_start_alignments_and_canaries=True,real_SDK_C_client_and_pointers_above_4GiB=True,actual_reserved_guard_pages_and_release_verified=True,
        safe_realloc_preserves_data_and_old_allocation_on_failure=True,unsafe_original_realloc_executed=False,native_overflow_double_free_alias_and_header_guards=True,
        concurrent_threads=4,concurrent_alloc_free_iterations=480,concurrent_growing_resizes=160,real_native_SRW_lock=True,
        terminal_corrupted_live_test_arena_left_for_test_process_teardown=True,peer_commands=commands,peer_source_sha256=sha(peer),host_query_source_sha256=sha(host),peer_DLL_sha256=sha(d/'sdk_heap_client.dll'),
        build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),general_exit_or_system_boot_heap_integration_verified=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',calls,'original calls',flush=True)
if __name__=='__main__':main()
