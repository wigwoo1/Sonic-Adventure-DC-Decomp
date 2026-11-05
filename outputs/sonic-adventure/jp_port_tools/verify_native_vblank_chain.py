"""Original chain/node contracts, native logical ticks and genuine njBlank.
Guest platform callback registration and synthetic user handlers are explicit
test services. Original list and Ninja function bodies execute without hooks.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from native_clock_test_support import Clock,Counter
from native_video_test_support import Video
from model_reference import make_reference
U=C.c_uint32;I=C.c_int;P=C.c_void_p;Q=C.c_uint64;Callback=C.CFUNCTYPE(None,P);VoidCallback=C.CFUNCTYPE(None);Idle=C.CFUNCTYPE(I,P,U)
class Record(C.Structure):_fields_=[('priority',U),('next',U),('callback',C.c_size_t),('argument',C.c_size_t)]
class Snapshot(C.Structure):_fields_=[(n,Q) for n in ['ticks','callbacks','idle_calls','last_wall','next_deadline']]+[(n,U) for n in ['rate_numerator','rate_denominator','max_catchup','status']]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for n,h in b['chain_external_sources_sha256'].items():assert sha(a.upstream/n)==h,n
    for n,h in b['kept_presentation_objects_sha256'].items():assert sha(n)==h,n
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256']
    C.CDLL(b['frames_DLL']);C.CDLL(b['GPU_DLL']);clock=Clock(Path(b['present_DLL']));lib=clock.lib
    assert sha(b['present_DLL'])==json.loads((Path(b['frame_build'])/'build_verification.json').read_bytes())['DLL_sha256']
    add=bind(lib,'syChainAddHandler',U,C.c_int16,P,U,P);delete=bind(lib,'syChainDeleteHandler',None,U);reset_chain=bind(lib,'syChainInit',None);count=bind(lib,'jp_vblank_chain_count',U);status=bind(lib,'jp_vblank_chain_status',I);clear=bind(lib,'jp_vblank_chain_clear_error',None);describe=bind(lib,'jp_vblank_chain_describe',I,U,P);mask=bind(lib,'jp_vblank_chain_mask',U)
    init=bind(lib,'jp_wait_initialize',I,U,U,U,P,P);shutdown=bind(lib,'jp_wait_shutdown',I);setter=bind(lib,'jp_wait_set_vsync',I,P);pump=bind(lib,'jp_wait_pump',I,P);wait_status=bind(lib,'jp_wait_status',I);snapshot=bind(lib,'jp_wait_snapshot',I,P);arm=bind(lib,'jp_vblank_arm_ninja_blank',I);wait_blank=bind(lib,'jp_vblank_wait_ninja_blank',I);eor=bind(lib,'njSetEORflag',None,P)
    ninja_count=I.in_dll(lib,'_nj_blank_count_');render_flag=I.in_dll(lib,'_nj_render_flag_');eor_flag=I.in_dll(lib,'_nj_eor_flag_');ninja_pointer=C.cast(bind(lib,'njBlank',None,P),P).value
    checks=0;groups={};counter=Counter();idle_mode=1;native_events=[];guest_events=[];callback_errors=[];actions={};handles={};registration=[];calls={};cookies=(U*2048)(*range(2048));rng=random.Random(0x56424c4b);owner_thread=threading.get_ident();threads=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    @Idle
    def idle(context,ms):
        if idle_mode<=0:return idle_mode
        if ms:counter.tick+=(ms*781250+999)//1000
        return 1
    @Callback
    def observe(argument):
        try:
            cookie=U.from_address(argument).value;native_events.append((cookie,mask()));threads.append(threading.get_ident())
            if cookie in actions:delete(handles[actions[cookie]])
        except BaseException as error:callback_errors.append(str(error))
    native_callback=C.cast(observe,P).value
    def snap():
        value=Snapshot();check(snapshot(C.byref(value))==0,'actual wait snapshot');return value
    def configure(num=60,den=1,maximum=7):
        nonlocal idle_mode
        check(shutdown()==0,'previous single wait owner shutdown');counter.tick=0;counter.ok=True;idle_mode=1
        check(counter.start(clock,781250)==0 and init(num,den,maximum,C.cast(idle,P),None)==0,'one actual clock/wait owner initialized');native_events.clear();guest_events.clear();actions.clear();handles.clear();callback_errors.clear()
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,refhash=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;POOL=table['isrPool'];KEY=table['keyTbl']+0x90*4;ARGS=0x8ce40000;USER=0x8cfed100
    def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
    def read(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def word(at):return struct.unpack('<I',read(at,4))[0]
    def iw(at,value):put(at,struct.pack('<I',value&0xffffffff))
    for i in range(2048):iw(ARGS+i*4,i)
    def platform(ram,regs,sr):registration.append((regs[4],regs[5]));regs[0]=0;return sr
    def guest_user(ram,regs,sr):
        cookie=word(regs[4]);guest_events.append((cookie,sr>>4&15))
        if cookie in actions:original('syChainDeleteHandler',[handles[actions[cookie]]],nested=True)
        return sr
    entered={};entries={table[n]:n for n in ['syChainInit','syChainAddHandler','syChainDeleteHandler','syChainMasterHandlerPVR_V','njBlank','njSetEORflag']}
    def trace(pc,*unused):
        if pc in entries:entered[entries[pc]]=entered.get(entries[pc],0)+1
    def original(name,args=(),nested=False):
        state=executor.State();state.pc=table[name];state.pr=0x8cfed000;state.sr=0x70000000|(0x60 if nested else 0);state.r[15]=0x8cf10000 if nested else 0x8cf00000
        for i,val in enumerate(args):state.r[4+i]=val&0xffffffff
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,state,fpu=fpu,hooks={table['kmSetVSyncCallback']:platform,USER:guest_user},trace=trace,instruction_budget=500000);calls[name]=calls.get(name,0)+1;check(state.r[15]==(0x8cf10000 if nested else 0x8cf00000),'whole original entry restores stack');return state
    def same():
        live=[]
        for handle in range(1,256):
            callback,next_pointer,argument,priority=struct.unpack('<4I',read(POOL+handle*16,16))
            if not priority&0x80000000:continue
            live.append(handle);record=Record();check(describe(handle,C.byref(record))==0,'actual active native node')
            check(record.priority==priority and record.next==((next_pointer-POOL)//16 if next_pointer else 0),'original priorities and full linked-list order')
            if callback==USER:check(record.callback==native_callback and record.argument==C.addressof(cookies)+(argument-ARGS),'relocated 64-bit user callback and argument')
            else:check(callback==table['njBlank'] and record.callback==ninja_pointer and record.argument==argument==0,'genuine Ninja callback pointer')
        check(count()==len(live),'original active pool population');return live
    def tick():
        native_events.clear();guest_events.clear();counter.tick=snap().next_deadline;processed=U(99);check(pump(C.byref(processed))==0 and processed.value==1,'one actual logical vblank tick');original('syChainMasterHandlerPVR_V',[0]);check(native_events==guest_events and not callback_errors and mask()==0,'original callback order/arguments and interrupt-context restoration');same()
    def add_pair(cookie,priority):
        old=original('syChainAddHandler',[0x900,USER,priority,ARGS+cookie*4]).r[0];new=add(0x900,observe,priority,C.addressof(cookies)+cookie*4);check(new==old and new!=0,'original lowest free handle');handles[cookie]=new;return new
    check(add(0x900,observe,88,None)==0 and status()==2,'uninitialized chain fails explicitly');configure();original('syChainInit');same()
    for cookie in range(1,256):add_pair(cookie,[88,0,88,0x7fffffff,cookie%7][cookie%5])
    check(original('syChainAddHandler',[0x900,USER,88,ARGS]).r[0]==add(0x900,observe,88,C.addressof(cookies))==0 and status()==6,'full original 255-slot pool');same();tick()
    for cookie in [1,128,255,64,32]:original('syChainDeleteHandler',[handles[cookie]]);delete(handles[cookie]);same()
    for cookie in range(1000,1005):add_pair(cookie,88)
    tick();reset_chain();original('syChainInit');same();handles.clear()
    for step in range(360):
        live=same()
        if not live or (len(live)<80 and rng.randrange(3)):
            cookie=step+1;add_pair(cookie,rng.choice([0,1,87,88,88,89,0x7fffffff,rng.getrandbits(31)]))
        else:
            handle=rng.choice(live);original('syChainDeleteHandler',[handle]);delete(handle)
        if step%17==0:tick()
    groups['whole_original_pool_priorities_handles_order_and_reuse']=checks
    configure();original('syChainInit');add_pair(1,5);add_pair(2,10);add_pair(3,15);actions[1]=1;actions[2]=3;tick();check(native_events==[(1,6),(2,6)] and count()==1,'self-deletion and deleting pending callback');actions.clear()
    before=snap();live=count()
    for event in [0,0x320,0x920,0xa00,-1]:check(add(event,observe,88,None)==0 and status()==5 and count()==live,'unsupported event source rejected without list mutation');clear()
    check(add(0x900,None,88,None)==0 and status()==1,'null callback rejected');clear()
    for handle in [0,256,0xffffffff]:
        delete(handle);check(status()==1 and count()==live,'invalid handle rejected');clear()
    foreign=[]
    def foreign_thread():
        foreign.extend([add(0x900,observe,1,None),status()]);delete(2);foreign.append(status())
    thread=threading.Thread(target=foreign_thread);thread.start();thread.join();check(foreign==[0,3,3] and status()==0 and count()==live and bytes(snap())==bytes(before),'foreign thread cannot mutate callback owner')
    configure();reentrant=[]
    @Callback
    def recursive(argument):
        value=U(77);reentrant.extend([add(0x900,observe,1,None),status()]);reset_chain();reentrant.append(status());reentrant.extend([pump(C.byref(value)),value.value]);clear()
    check(add(0x900,recursive,1,None)!=0,'native recursive fixture');counter.tick=snap().next_deadline;processed=U();check(pump(C.byref(processed))==0 and reentrant==[0,4,4,4,77] and count()==1,'callback additions/reset/pump reentrancy explicitly rejected')
    groups['native_boundaries_and_safe_callback_mutation']=checks-sum(groups.values())
    configure();order=[]
    @Callback
    def a_callback(arg):order.append(1)
    @Callback
    def b_callback(arg):order.append(2)
    @VoidCallback
    def game():order.append(3)
    check(add(0x900,a_callback,88,None)==1 and setter(C.cast(game,P))==0 and add(0x900,b_callback,88,None)==3,'game callback shares original pool and priority')
    counter.tick=snap().next_deadline;processed=U();check(pump(C.byref(processed))==0 and order==[2,3,1],'new equal-priority callbacks precede game callback')
    order.clear();check(setter(C.cast(game,P))==0,'game callback reinstall');counter.tick=snap().next_deadline;check(pump(C.byref(processed))==0 and order==[3,2,1],'game callback reinstall reorders at equal priority')
    check(setter(None)==0 and count()==2,'removing game callback retains registered engine callbacks');reset_chain();check(count()==0,'chain reset removes all callbacks')
    for i in range(255):check(add(0x900,observe,i,C.addressof(cookies)+i*4)!=0,'pool capacity fixture')
    saved=bytes(snap());check(setter(C.cast(game,P))==9 and bytes(snap())==saved and count()==255,'full pool rejects game callback atomically');reset_chain();check(setter(C.cast(game,P))==0 and count()==1,'game callback recovery after pool reset')
    groups['legacy_game_callback_priority_replacement_and_capacity']=checks-sum(groups.values())
    # The blank handle is a private original static. Locate its original MOV.L
    # literal among the real njBlank instructions, excluding named dependencies.
    literals=[];pc=table['njBlank']
    for i in range(64):
        ins=struct.unpack('<H',read(pc,2))[0]
        if ins>>12==13:literals.append(word(((pc+4)&~3)+(ins&255)*4))
        pc+=2
        if ins==0x000b:break
    candidates=set(literals)-{table['_nj_render_flag_'],table['_nj_blank_count_'],table['syChainDeleteHandler']};check(len(candidates)==1,'one original static blank-handle literal');blank_handle=candidates.pop()
    for initial in [0,1,0x7fffffff,0xffffffff]:
        configure();original('syChainInit');ninja_count.value=C.c_int(initial).value;render_flag.value=0;eor_flag.value=1
        iw(table['_nj_blank_count_'],initial);iw(table['_nj_render_flag_'],0);iw(table['_nj_eor_flag_'],1)
        handle=original('syChainAddHandler',[0x900,table['njBlank'],88,0]).r[0];iw(blank_handle,handle);check(arm()==0 and arm()==1,'real Ninja callback armed once');clear();same();tick()
        check(ninja_count.value==C.c_int(word(table['_nj_blank_count_'])).value and (ninja_count.value&0xffffffff)==(initial+1)&0xffffffff,'whole original njBlank counter including wrap')
        original('njSetEORflag',[0]);eor(None);check(render_flag.value==1 and eor_flag.value==0,'genuine end-of-render flag setter');tick();check(count()==0 and ninja_count.value==C.c_int(word(table['_nj_blank_count_'])).value,'genuine njBlank self-retires after completed render')
    configure();ninja_count.value=render_flag.value=0;check(arm()==0 and wait_blank()==0 and ninja_count.value==1 and snap().ticks==1,'startup blank wait advances through the real logical clock');eor(None);check(wait_blank()==0 and ninja_count.value==2 and count()==0,'post-render blank retires without synthetic counter updates')
    configure();check(arm()==0,'failure-wait fixture armed');idle_mode=0;check(wait_blank()==5 and wait_status()==5,'closed host ends startup wait explicitly');configure();check(arm()==0,'failed host fixture armed');idle_mode=-1;check(wait_blank()==7,'failed host ends startup wait explicitly')
    check(all(t==owner_thread for t in threads),'all synthetic callbacks execute on original game thread');check(shutdown()==0 and clock.status()==0,'wait owner shutdown');clock.shutdown()
    groups['genuine_Ninja_blank_EOR_counter_and_native_startup_wait']=checks-sum(groups.values())
    check(entered['njBlank']==8,'all original Ninja callback bodies observed')
    result=dict(status='original_vblank_chain_contracts_and_native_clock_Ninja_startup_wait_verified',checks=checks,original_calls_by_function=calls,original_entry_calls=sum(calls.values()),original_dependencies_entered=entered,original_chain_or_Ninja_function_hooks_used=False,guest_platform_callback_registration_hooks_used=True,guest_synthetic_user_callback_hooks_used=True,guest_platform_callback_registration_calls=len(registration),original_priority_pool_and_handle_contracts_verified=True,pool_usable_slots=255,SDK_equal_priorities_newest_first=True,synthetic_native_callback_count=len(threads),all_callbacks_on_game_thread=True,safe_self_and_pending_handler_deletion=True,native_callback_add_reset_reentrancy_rejected=True,legacy_game_callback_original_priority_and_shared_pool_verified=True,non_vblank_event_sources_explicitly_unsupported=True,original_njBlank_static_handle_address=hex(blank_handle),genuine_Ninja_blank_and_EOR_bodies_executed=True,real_logical_wait_not_manual_counter_advancement=True,physical_IRQs_hblank_DMA_or_GPU_EOR_integration_verified=False,whole_system_boot_or_gameplay_executed=False,reference_hashes=refhash,original_binary_sha256=sha(a.binary),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),groups=groups,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ['reference_hashes','groups']}),flush=True)
if __name__=='__main__':main()
