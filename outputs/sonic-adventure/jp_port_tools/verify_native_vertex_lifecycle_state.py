"""Real live-font preservation and late callback failure atomicity."""
import argparse,ctypes as C,hashlib,json
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Surface
from native_clock_test_support import Clock,Counter
from verify_native_vertex_lifecycle_original import Config
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','gpu','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=json.loads((a.build/'verification.json').read_bytes());assert sha(b['DLL'])==b['DLL_sha256']
    t=Runtime(a.build/'jp_native_frames.dll');C.CDLL(str(a.gpu.resolve()));clock=Clock(a.build/'jp_native_present.dll');counter=Counter();assert counter.start(clock,781250)==0
    I=C.c_int;U=C.c_uint32;P=C.c_void_p;Idle=C.CFUNCTYPE(I,P,U);Callback=C.CFUNCTYPE(None,P)
    @Idle
    def idle(context,ms):counter.tick+=(ms*781250+999)//1000;return 1
    assert bind(clock.lib,'jp_wait_initialize',I,U,U,U,P,P)(60,1,7,idle,None)==0
    lib=C.CDLL(b['DLL']);initialize=bind(lib,'jp_vertex_lifecycle_initialize',I,*([C.c_int32]*5));cancel=bind(clock.lib,'jp_vblank_cancel_ninja_blank',I);count=bind(clock.lib,'jp_vblank_chain_count',U);setter=bind(clock.lib,'jp_wait_set_vsync',I,P)
    qi=bind(t.dll,'jp_queue_initialize',I,C.c_size_t);data=bind(t.dll,'jp_queue_data',P,U);cap=bind(t.dll,'jp_queue_capacity_bytes',C.c_size_t,U);clear=bind(t.dll,'jp_queue_clear_error',None);get=bind(t.dll,'njGetkmPointer',None);put=bind(t.dll,'njSetkmPointer',None);reserve=bind(t.dll,'jp_queue_reserve',I,U,U);begin=bind(t.dll,'njStartVertexNonTexOpaque',P);discard=bind(t.dll,'jp_queue_discard_active',I)
    replace=bind(t.dll,'jp_texture_replace_standalone_surface',I,C.POINTER(Surface),P,C.c_size_t)
    config=Config.in_dll(lib,'_nj_SystemConfigStruct_');checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    check(qi(4<<20)==0 and t.init(8,16,1<<20)==0,'actual owners')
    font=Surface();font.nWidth=font.nHeight=8;font.TextureSize=128;raw=C.create_string_buffer(bytes(range(128)));check(replace(C.byref(font),raw,128)==0,'actual standalone font')
    address=font.pSurface;word=t.surface_word(C.byref(font))
    for i in range(24):
        check(cancel()==0 and initialize(*([(-1,0,131072,0,32768),(200,3,-1,4,10),(200,3,100,4,10)][i%3]))==0,'real repeated normal/bypass lifecycle')
        check(font.pSurface==address and t.surface_word(C.byref(font))==word and C.string_at(address,128)==raw.raw[:128],'font allocation identity and all encoded bytes preserved')
        check(U.in_dll(t.dll,'_nj_tex_max_free_size').value==1048576-t.allocated(),'live font participates in final original cache query')
    def snapshot():return (bytes(config),tuple(U.in_dll(lib,'_nj_vertex_buffer_'+n+'_').value for n in ['op','om','tp','tm','pt','size']),U.in_dll(t.dll,'_nj_tex_max_free_size').value,tuple((data(i),cap(i)) for i in range(5)))
    cancel();check(reserve(0,4)==0,'actual active recording reservation');get();check(bool(begin()),'actual active batch');before=snapshot()
    check(initialize(10,0,20,0,0)==6 and count()==0 and snapshot()==before,'active batch rejects reconfiguration and cleans blank without state commit');clear();check(discard()==0,'actual pending reference cancelled');put()
    # A genuine game callback can invalidate texture ownership during the wait.
    old=snapshot();events=[]
    @Callback
    def unload(context):events.append(1);t.shutdown()
    check(setter(unload)==0,'actual callback installed');check(initialize(10,0,20,0,0)==3 and events and count()==1,'late owner failure retires Ninja blank and retains game callback')
    after=snapshot();check(after[0:2]==old[0:2] and after[3]==old[3],'late texture failure preserves config/scalars/queue storage')
    check(setter(None)==0 and t.init(8,16,1<<20)==0 and initialize(10,0,20,0,0)==0,'real recovery after callback invalidation')
    cancel();bind(clock.lib,'jp_wait_shutdown',I)();clock.shutdown();t.shutdown();bind(t.dll,'jp_queue_shutdown',None)()
    result=dict(status='real_font_lifetime_active_batch_and_late_texture_failure_verified',checks=checks,build_proof_sha256=sha(a.build/'verification.json'),verifier_sha256=sha(__file__),native_functions_replaced=False,actual_font_payload_bytes_checked=24*128,actual_reconfiguration_calls=28,actual_game_callback_invalidated_texture_owner=True,retail_inputs_used=False,full_gameplay_verified=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
