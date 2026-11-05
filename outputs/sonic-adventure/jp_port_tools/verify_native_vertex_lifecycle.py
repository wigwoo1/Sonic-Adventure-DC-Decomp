"""Build the higher lifecycle module and exercise its actual native owners."""
import argparse,ctypes as C,hashlib,json,subprocess
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_clock_test_support import Clock,Counter
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','presentation','gpu','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.baseline/'verification.json').read_bytes())
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists();d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['kept_objects_sha256'].items():assert sha(n)==h,n
    header=(r/'outputs/sonic-adventure/native/jp_native_vertex_lists.h').read_text();start=header.index('struct jp_vertex_lists {');end=header.index('};',start)+2
    imports=d/'jp_vertex_lifecycle_imports.h';imports.write_text(header[start:end]+'''
__declspec(dllimport) int jp_vertex_lists_configure(int32_t,int32_t,int32_t,int32_t,int32_t,struct jp_vertex_lists *);
__declspec(dllimport) int jp_vblank_arm_ninja_blank(void);
__declspec(dllimport) int jp_vblank_wait_ninja_blank(void);
__declspec(dllimport) int jp_vblank_cancel_ninja_blank(void);
__declspec(dllimport) uint32_t jp_rz_scalar32_multiply(uint32_t,uint32_t);
__declspec(dllimport) uint32_t jp_rz_scalar32_divide(uint32_t,uint32_t);
__declspec(dllimport) void jp_texture_cache_maximum(uint32_t);
''')
    source=r/'outputs/sonic-adventure/native/jp_native_vertex_lifecycle.c';common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    cache=r/'outputs/sonic-adventure/native/jp_native_texture_cache.c'
    commands=[common+['-c',str(cache),'-o',str(d/'texture_cache.obj')]]
    frame_link=list(b['commands'][-1]['command']);old=next(n for n in frame_link if Path(n).name=='texture_space.obj');frame_link[frame_link.index(old)]=str(d/'texture_cache.obj');frame_link[-1]=str(d/'jp_native_frames.dll');commands.append(frame_link)
    pb=json.loads((a.presentation.parent/'build_verification.json').read_bytes());wait_source=r/'outputs/sonic-adventure/native/jp_native_vblank_cancel.c'
    wait_compile=list(pb['commands'][0]['command']);wait_compile[wait_compile.index('-c')+1]=str(wait_source);wait_compile[-1]=str(d/'wait_cancel.obj');commands.append(wait_compile)
    present_link=list(next(x['command'] for x in pb['commands'] if x['command'][-1]==str(a.presentation.resolve())));previous=next(n for n in present_link if Path(n).name=='vblank_wait_owner.obj');present_link[present_link.index(previous)]=str(d/'wait_cancel.obj');present_link[-1]=str(d/'jp_native_present.dll');commands.append(present_link)
    commands.append(common+['-I'+str(d),'-c',str(source),'-o',str(d/'lifecycle.obj')])
    exports=['njInitVertexBuffer','_nj_SystemConfigStruct_','_nj_vertex_2v_type_']+['_nj_vertex_buffer_'+n+'_' for n in ['op','om','tp','tm','pt','size']]
    export_args=['-Wl,/export:njInitVertexBuffer']+[item for n in exports[1:] for item in ['-Xlinker','/export:'+n+',DATA']]
    commands.append([common[0],'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'lifecycle.obj'),str(d/'jp_native_frames.lib'),str(d/'jp_native_present.lib'),*export_args,'-o',str(d/'jp_native_vertex_lifecycle.dll')])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    t=Runtime(d/'jp_native_frames.dll');C.CDLL(str(a.gpu.resolve()));clock=Clock(d/'jp_native_present.dll');counter=Counter();assert counter.start(clock,781250)==0
    Idle=C.CFUNCTYPE(C.c_int,C.c_void_p,C.c_uint32)
    idle_state={'closed':False}
    @Idle
    def idle(context,ms):
        if idle_state['closed']:return 0
        counter.tick+=(ms*781250+999)//1000;return 1
    assert bind(clock.lib,'jp_wait_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_uint32,C.c_void_p,C.c_void_p)(60,1,7,idle,None)==0
    lib=C.CDLL(str(d/'jp_native_vertex_lifecycle.dll'));initialize=bind(lib,'jp_vertex_lifecycle_initialize',C.c_int,*([C.c_int32]*5));sdk=bind(lib,'njInitVertexBuffer',None,*([C.c_int32]*5));qi=bind(t.dll,'jp_queue_initialize',C.c_int,C.c_size_t);cap=bind(t.dll,'jp_queue_capacity_bytes',C.c_size_t,C.c_uint32);checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    check(initialize(-1,0,131072,0,32768)==3,'real missing texture owner rejected')
    check(t.init(8,16,1<<20)==0 and qi(4<<20)==0,'actual owners initialized')
    check(initialize(-1,0,131072,0,32768)==0,'actual Ninja wait followed by real queue configuration')
    check(cap(0)==416 and cap(1)==655360,'main real queue capacities')
    check(C.c_uint32.in_dll(lib,'_nj_vertex_buffer_tp_').value==163840 and C.c_uint32.in_dll(lib,'_nj_vertex_buffer_size_').value==163940 and C.c_int.in_dll(lib,'_nj_vertex_2v_type_').value==1,'SDK scalar globals committed')
    check(C.c_uint32.in_dll(t.dll,'_nj_tex_max_free_size').value==1048576-t.allocated(),'actual original texture query refreshes shared maximum')
    check(C.c_int.in_dll(clock.lib,'_nj_blank_count_').value>=1,'genuine original Ninja callback ran')
    saved=C.c_uint32.in_dll(lib,'_nj_vertex_buffer_size_').value
    check(initialize(0,0,0,0,0)==2 and C.c_uint32.in_dll(lib,'_nj_vertex_buffer_size_').value==saved,'invalid request preserves SDK globals')
    bind(clock.lib,'njSetEORflag',None,C.c_void_p)(None);check(bind(clock.lib,'jp_vblank_wait_ninja_blank',C.c_int)()==0,'real EOR retires first handler')
    sdk(80,0,160,0,40);check(bind(lib,'jp_vertex_lifecycle_status',C.c_int)()==0 and C.c_uint32.in_dll(lib,'_nj_vertex_buffer_size_').value==280,'SDK entry calls actual lifecycle')
    cancel=bind(clock.lib,'jp_vblank_cancel_ninja_blank',C.c_int);count=bind(clock.lib,'jp_vblank_chain_count',C.c_uint32)
    check(cancel()==0 and count()==0,'explicit cancellation removes only owned blank')
    check(qi(128)==0,'small real queue budget')
    oldsize=C.c_uint32.in_dll(lib,'_nj_vertex_buffer_size_').value
    check(initialize(-1,0,131072,0,32768)==6 and count()==0,'failed actual allocation cleans newly armed blank')
    check(C.c_uint32.in_dll(lib,'_nj_vertex_buffer_size_').value==oldsize,'allocation failure preserves scalar globals')
    check(qi(4<<20)==0 and initialize(-1,0,131072,0,32768)==0,'actual initialization retries after allocation failure')
    check(cancel()==0,'retry handler cleanup')
    Callback=C.CFUNCTYPE(None,C.c_void_p);reentered=[]
    @Callback
    def reentry(context):reentered.append(initialize(80,0,160,0,40))
    check(bind(clock.lib,'jp_wait_set_vsync',C.c_int,C.c_void_p)(reentry)==0,'actual game callback installed')
    check(initialize(80,0,160,0,40)==0 and reentered and all(x==1 for x in reentered),'reentry rejected while outer lifecycle completes')
    check(cancel()==0,'reentry case cleans blank')
    check(bind(clock.lib,'jp_wait_set_vsync',C.c_int,C.c_void_p)(None)==0,'game callback removed')
    # Verify slot identity: cancelling a retired Ninja slot must not delete a
    # different callback which subsequently acquired the same numeric slot.
    bind(clock.lib,'njSetEORflag',None,C.c_void_p)(None)
    check(initialize(80,0,160,0,40)==0 and count()==0,'render-completed blank retires itself')
    @Callback
    def unrelated(context):pass
    add=bind(clock.lib,'syChainAddHandler',C.c_uint32,C.c_int16,C.c_void_p,C.c_uint32,C.c_void_p)
    handle=add(0x900,unrelated,1,None);check(handle==1 and cancel()==0 and count()==1,'recycled unrelated numeric slot survives cancellation')
    bind(clock.lib,'syChainDeleteHandler',None,C.c_uint32)(handle)
    idle_state['closed']=True
    check(initialize(80,0,160,0,40)==5 and count()==0,'terminal wait failure still cleans actual Ninja handler')
    check(bind(clock.lib,'jp_wait_status',C.c_int)()==5,'cleanup preserves terminal wait failure status')
    check(bind(clock.lib,'jp_wait_shutdown',C.c_int)()==0,'wait shutdown');clock.shutdown();t.shutdown();bind(t.dll,'jp_queue_shutdown',None)()
    result=dict(status='native_Ninja_vertex_initialization_lifecycle_and_failure_retry_verified',checks=checks,commands=results,source_sha256=sha(source),cache_source_sha256=sha(cache),wait_source_sha256=sha(wait_source),verifier_sha256=sha(__file__),imports_sha256=sha(imports),baseline_proof_sha256=sha(a.baseline/'verification.json'),DLL=str(d/'jp_native_vertex_lifecycle.dll'),DLL_sha256=sha(d/'jp_native_vertex_lifecycle.dll'),actual_wait_and_queue_and_texture_owners_used=True,original_sdk_entry_adapted=True,allocation_failure_retry_and_reentry_tested=True,terminal_failure_cleanup_tested=True,recycled_unrelated_slot_preserved=True,full_kmSetSystemConfiguration_or_parent_boot_verified=False,artifact_publishable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks')
if __name__=='__main__':main()
