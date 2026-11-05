"""Qualify real five-list allocation, rollback and retained-frame ownership.
Production DLL is tested directly. A separate diagnostic build injects only
CRT allocation failure to exercise each staged rollback, never the renderer.
"""
import argparse, ctypes as C, hashlib, json, random, subprocess
from pathlib import Path
from native_packet_test_support import bind
from native_video_test_support import Video
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists();d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    U=C.c_uint32;S=C.c_size_t;P=C.c_void_p;I=C.c_int;Q=C.c_uint64
    checks=0;groups={};rng=random.Random(0x5155414c)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def exercise(path,diagnostic=False):
        lib=C.CDLL(str(path));pre=bind(lib,'jp_queue_preallocate',I,C.POINTER(S));cap=bind(lib,'jp_queue_capacity_bytes',S,U);data=bind(lib,'jp_queue_data',P,U)
        init=bind(lib,'jp_queue_initialize',I,S);stop=bind(lib,'jp_queue_shutdown',None);clear=bind(lib,'jp_queue_clear_error',None);status=bind(lib,'jp_queue_status',I);cost=bind(lib,'jp_queue_allocated_bytes',S)
        parameter=bind(lib,'jp_queue_parameter_cursor',P,U);descriptor=bind(lib,'jp_queue_descriptor_cursor',P,U);reserve=bind(lib,'jp_queue_reserve',I,U,U);reset=bind(lib,'jp_queue_reset_frame',I)
        context=bind(lib,'jp_queue_set_context',I,U,P);select=bind(lib,'jp_queue_select_context',I,U);getcontext=bind(lib,'jp_queue_get_context',I,U,P);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None)
        begin=bind(lib,'njStartVertexNonTexOpaque',P);end=bind(lib,'njEndVertex',None,P);discard=bind(lib,'jp_queue_discard_active',I)
        fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,C.POINTER(Q));view=bind(lib,'jp_frame_list',P,Q,U,C.POINTER(S));release=bind(lib,'jp_frame_release',I,Q)
        def allocate(values):return pre((S*5)(*values))
        def snapshot():
            return (cost(),tuple((cap(i),data(i),parameter(i),descriptor(i),C.string_at(data(i)-32,cap(i)+64) if data(i) else b'') for i in range(5)))
        def failed(values,error):
            before=snapshot();check(allocate(values)==error,'explicit rejection');check(snapshot()==before,'all five pools/cursors/guards unchanged');clear();check(status()==0,'recoverable sticky error')
        check(allocate([32]*5)==7,'uninitialized rejected');check(init(1<<20)==0 and fi(8,8,1<<20)==0 and Video(lib).init(64,64,0xff000000,1)==0,'real queue, frame and video owners')
        words=(U*4)(0x80000002,0x20800000,0x12345678,0)
        check(context(2,words)==0 and select(2)==0,'distinct selected context');get()
        check(pre(None)==1,'null request');clear()
        minima=[32,96,64,128,160];check(allocate(minima)==0,'allocate five real lists');check(cost()==sum(minima)+5*64,'guard-inclusive accounting')
        for i,n in enumerate(minima):
            check(cap(i)==n and data(i) and data(i)%32==0 and data(i)>0xffffffff,'native aligned list allocation')
            check(parameter(i)==descriptor(i)==data(i),'existing parameter and SDK descriptor share allocation')
            check(C.string_at(data(i)-32,32)==b'\xa5'*32 and C.string_at(data(i)+n,32)==b'\x5a'*32,'both actual queue guards')
        got=(U*4)();check(getcontext(2,got)==0 and bytes(got)==bytes(words),'context header preserved')
        unchanged=snapshot();check(allocate([0]*5)==0 and snapshot()==unchanged,'zero minima retain reusable buffers')
        for index in range(5):
            values=minima.copy();values[index]=33;failed(values,1)
            values[index]=(1<<64)-32;failed(values,1)
        # Actual recorded geometry blocks reconfiguration until it is submitted.
        check(reserve(0,4)==0,'normal producer growth');get();ptr=begin();check(bool(ptr),'real SDK-facing nontextured start')
        failed([512]*5,2);C.memmove(ptr,rng.randbytes(96),96);end(ptr+96)
        failed([512]*5,2);put();failed([512]*5,2)
        recorded=C.string_at(data(0),128);fid=Q();check(submit(C.byref(fid))==0,'actual immutable frame submitted')
        check(allocate([512]*5)==0,'empty recording can allocate with submitted frame live')
        size=S();raw=view(fid.value,0,C.byref(size));check(size.value==128 and C.string_at(raw,128)==recorded,'submitted frame independent of queue reallocation')
        check(release(fid.value)==0,'submitted frame released')
        # Source request aliases a pool which this same request replaces.
        values=(S*5)(1024,32,32,32,32);C.memmove(data(0),values,C.sizeof(values))
        check(pre(C.cast(data(0),C.POINTER(S)))==0 and cap(0)==1024,'aliased request copied before old buffer is freed')
        for i in range(5):
            address=data(i)-1;old=C.c_ubyte.from_address(address).value;C.c_ubyte.from_address(address).value=0
            failed([2048]*5,6);C.c_ubyte.from_address(address).value=old
            address=data(i)+cap(i);old=C.c_ubyte.from_address(address).value;C.c_ubyte.from_address(address).value=0
            failed([2048]*5,6);C.c_ubyte.from_address(address).value=old
        check(reset()==0,'restored guards accepted')
        # A late-list budget failure must not install earlier eligible pools.
        check(init(512)==0,'tight transient budget');check(allocate([32,0,0,0,0])==0,'old allocation')
        failed([64,64,128,0,0],4)
        check(allocate([64,64,96,0,0])==0 and cost()==416,'exact transient-budget boundary commits all lists')
        before=snapshot();check(allocate([32,32,32,0,0])==0 and snapshot()==before,'minimum reuse never shrinks')
        check(reserve(4,1)==0 and cap(4)==32,'zero configured minimum still permits real producer')
        check(cap(5)==cap(0xffffffff)==0,'invalid capacity queries')
        stop();check(cost()==0 and all(cap(i)==0 and data(i) is None for i in range(5)),'shutdown frees real pools');check(fs()==0,'frame cleanup')
        if diagnostic:
            fail=bind(lib,'jp_test_fail_at',None,I);live=bind(lib,'jp_test_live',I)
            for failure in range(1,6):
                fail(0);check(init(1<<20)==0 and allocate([32]*5)==0 and live()==5,'diagnostic owns five old allocations')
                fail(failure);before=snapshot();check(allocate([64]*5)==5 and snapshot()==before,'staged allocation failure preserves old pools')
                check(live()==5,'all temporary allocations freed on rollback');clear();fail(0)
                check(allocate([64]*5)==0 and live()==5,'allocation retry replaces and frees old pools');stop();check(live()==0,'diagnostic allocation lifetime balanced')
    exercise(Path(b['frames_DLL']));groups['production_owner']=checks
    # Replace just the queue TU in a separate private diagnostic frame DLL.
    source=d/'allocation_failure_probe.c';source.write_text('''#include <malloc.h>
#include <stddef.h>
static void *tracked[32];static int calls,fail_at;
static void *probe_alloc(size_t size,size_t alignment){
    if(++calls==fail_at)return NULL;
    void *p=_aligned_malloc(size,alignment);
    if(p)for(int i=0;i<32;i++)if(!tracked[i]){tracked[i]=p;break;}
    return p;
}
static void probe_free(void *p){
    if(p)for(int i=0;i<32;i++)if(tracked[i]==p){tracked[i]=NULL;break;}
    _aligned_free(p);
}
__declspec(dllexport) void jp_test_fail_at(int at){calls=0;fail_at=at;}
__declspec(dllexport) int jp_test_live(void){int n=0;for(int i=0;i<32;i++)n+=tracked[i]!=NULL;return n;}
#define JP_QUEUE_PREALLOCATE_ALLOC probe_alloc
#define _aligned_free probe_free
#include "jp_native_queue_allocation.c"
''')
    compile=list(b['commands'][0]['command']);compile[compile.index('-c')+1]=str(source);compile[-1]=str(d/'allocation_failure_probe.obj')
    link=list(next(x['command'] for x in b['commands'] if x['command'][-1]==b['frames_DLL']));link[link.index(b['queue_owner_object'])]=compile[-1];link[-1]=str(d/'jp_native_frames_diagnostic.dll')
    commands=[]
    for i,cmd in enumerate([compile,link]):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);check(run.returncode==0 and not log.stat().st_size,'diagnostic compile/link warning free');commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
    before=checks;exercise(Path(link[-1]),True);groups['diagnostic_owner_and_five_allocator_failure_rollbacks']=checks-before
    result=dict(status='real_five_list_allocation_and_failure_atomicity_verified',checks=checks,groups=groups,build_report_sha256=sha(a.build/'build_verification.json'),frames_DLL_sha256=sha(b['frames_DLL']),verifier_sha256=sha(__file__),commands=commands,production_functions_replaced=False,diagnostic_allocation_failure_only=True,all_five_failure_positions_tested=True,retained_actual_frame_verified=True,console_memory_aliases_emulated=False,njInitVertexBuffer_integrated=False,whole_system_boot_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
