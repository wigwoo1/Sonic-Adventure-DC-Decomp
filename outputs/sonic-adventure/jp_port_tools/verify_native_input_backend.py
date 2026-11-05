"""Actual Windows converter/provider and mutable original-SDK record contracts.
Controlled samples do not establish physical controller/keyboard coverage.
"""
import argparse,ctypes as C,hashlib,json,random,threading
from pathlib import Path
from native_packet_test_support import bind
U=C.c_uint32;I=C.c_int;P=C.c_void_p
class Sample(C.Structure):_fields_=[('connected',U),('buttons',U)]+[(n,C.c_int32) for n in ['x1','y1','x2','y2']]+[('left',U),('right',U)]
class Packet(C.Structure):_fields_=[('buttons',C.c_uint16),('left',C.c_uint8),('right',C.c_uint8)]+[(n,C.c_int16) for n in ['lx','ly','rx','ry']]
class Peripheral(C.Structure):
    _fields_=[(n,U) for n in ['id','support','on','off','press','release']]+[('r',C.c_uint16),('l',C.c_uint16)]+[(n,C.c_int16) for n in ['x1','y1','x2','y2']]+[('name',P),('extend',P),('old',U),('info',P)]
class Info(C.Structure):_fields_=[('polls',C.c_uint64)]+[(n,U) for n in ['status','source_kind','focused','connected_mask','xinput_available','os_error','record_bytes']]
Source=C.CFUNCTYPE(I,P,C.POINTER(Sample))
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists();b=json.loads((d/'build_verification.json').read_bytes())
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    lib=C.CDLL(b['DLL']);checks=0;groups={};errors=[];rng=random.Random(0x494e5055)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    init=bind(lib,'jp_input_initialize',I,P,U,P,P);shutdown=bind(lib,'jp_input_shutdown',I);poll=bind(lib,'jp_input_poll',I);status=bind(lib,'jp_input_status',I)
    clear=bind(lib,'jp_input_clear_error',None);snapshot=bind(lib,'jp_input_snapshot',I,P);record=bind(lib,'jp_input_record',P,U)
    peripheral=bind(lib,'pdGetPeripheral',P,U);ninja=bind(lib,'njGetPeripheral',P,U);description=bind(lib,'pdGetPeripheralInfo',P,U);server=bind(lib,'pdExecPeripheralServer',None);exit_sdk=bind(lib,'pdExitPeripheral',None)
    convert=bind(lib,'jp_input_map_xinput',I,P,P);keyboard=bind(lib,'jp_input_apply_keys',I,P,P)
    check(bind(lib,'jp_input_info_size',C.c_size_t)()==C.sizeof(Info) and bind(lib,'jp_input_record_size',C.c_size_t)()==C.sizeof(Peripheral),'native original-SDK record and info ABI')
    check(status()==4 and poll()==4 and not record(0) and shutdown()==0,'uninitialized owner fails explicitly')
    check(Peripheral.from_address(peripheral(0)).support==0,'uninitialized SDK reads stable disconnected record')
    for flags in [0,4,0xffffffff]:check(init(None,flags,None,None)==1,'invalid stock initialization flags')
    check(init(P(1),1,None,None)==1,'keyboard capture rejects invalid window')
    mappings={0x1000:4,0x2000:2,0x4000:1024,0x8000:512,0x10:8,1:16,2:32,4:64,8:128,0x100:1<<17,0x200:1<<16}
    for mask in range(65536):
        p=Packet(mask,0,0,0,0,0,0);out=Sample();convert(C.byref(p),C.byref(out))
        expected=sum(target for source,target in mappings.items() if mask&source)
        check(out.connected==1 and out.buttons==expected and (out.x1,out.y1,out.x2,out.y2,out.left,out.right)==(0,0,0,0,0,0),'all XInput button masks map through actual collector conversion')
    for value in range(-32768,32768):
        p=Packet(0,30,31,value,value,value,value);out=Sample();convert(C.byref(p),C.byref(out));flipped=max(-32768,min(32767,-value))//256
        check((out.x1,out.x2,out.y1,out.y2)==(value//256,value//256,flipped,flipped) and out.left==30 and out.right==31 and out.buttons==1<<16,'all signed axes and trigger threshold retain original units/signs')
    guard=Sample();C.memset(C.byref(guard),0x5a,C.sizeof(guard));before=bytes(guard)
    check(convert(None,C.byref(guard))==1 and bytes(guard)==before and convert(C.byref(Packet()),None)==1,'invalid pure converter arguments do not mutate output')
    keys=(C.c_uint8*256)();single={32:4,90:2,88:1024,67:512,13:8,38:16,40:32,37:64,39:128,81:1<<17,69:1<<16,27:4|2|1024|512|8}
    for key in range(256):
        keys[key]=1;out=Sample();keyboard(keys,C.byref(out));keys[key]=0
        check(out.connected==1 and out.buttons==single.get(key,0) and out.x1==(-128 if key==65 else 127 if key==68 else 0) and out.y1==(-128 if key==87 else 127 if key==83 else 0) and out.left==(255 if key==81 else 0) and out.right==(255 if key==69 else 0),'each keyboard binding and unrelated key')
    keys[65]=keys[68]=keys[87]=keys[83]=1;out=Sample(1,4,80,90,3,4,30,40);keyboard(keys,C.byref(out))
    check((out.x1,out.y1,out.x2,out.y2,out.buttons,out.left,out.right)==(0,0,3,4,4,30,40),'opposed keyboard directions cancel while other controller channels remain')
    keys[1]=2;before=bytes(out);check(keyboard(keys,C.byref(out))==1 and bytes(out)==before,'invalid key bitsets fail atomically');keys[1]=0
    groups['actual_Windows_mapping_functions_without_hardware_injection']=checks
    samples=(Sample*4)();source_ok=[True];reenter=[False];nested=[]
    @Source
    def source(context,out):
        try:
            if reenter[0]:nested.append((poll(),shutdown()))
            C.memmove(out,samples,C.sizeof(samples));return int(source_ok[0])
        except BaseException as error:errors.append(str(error));return 0
    check(init(None,0,C.cast(source,P),None)==0 and status()==0,'explicit controlled source initializes real provider')
    check(init(None,0,C.cast(source,P),None)==2,'second initialization cannot replace active ownership')
    pointers=[record(i) for i in range(4)];check(len(set(pointers))==4 and all(pointers),'four stable distinct native SDK records')
    old=[0]*4;allowed=sum(1<<v for v in [0,1,2,3,4,5,6,7,8,9,10,16,17])
    for iteration in range(1024):
        for i in range(4):
            samples[i]=Sample(rng.randrange(2),rng.getrandbits(18)&allowed,*[rng.randrange(-128,128) for _ in range(4)],rng.randrange(256),rng.randrange(256))
        if iteration%2:server()
        else:check(poll()==0,'actual provider polling')
        check(not errors and status()==0,'controlled source has no callback or owner error')
        for i in range(4):
            ptr=record(i);s=samples[i];actual=Peripheral.from_address(ptr);on=s.buttons if s.connected else 0
            check(ptr==pointers[i] and peripheral(i*6)==ninja(i*6)==ptr,'SDK port aliases retain exact native record identity')
            check((actual.old,actual.on,actual.off,actual.press,actual.release)==(old[i],on,(~on)&0xffffffff,on&~old[i],old[i]&~on),'original SDK active/negative/edge masks')
            check((actual.x1,actual.y1,actual.x2,actual.y2,actual.l,actual.r)==((s.x1,s.y1,s.x2,s.y2,s.left,s.right) if s.connected else (0,0,-32768,-32768,0x8000,0x8000)),'connected axes or original disconnected sentinels')
            check(bool(actual.support)==bool(s.connected) and bool(actual.info)==bool(actual.name)==bool(s.connected) and description(i*6)==actual.info,'actual connection metadata follows physical/source availability')
            old[i]=on
        state=Info();snapshot(C.byref(state));check(state.polls==iteration+1 and state.connected_mask==sum(samples[i].connected<<i for i in range(4)),'committed sample sequence and connection mask')
    # The original recorder may overlay per_info; physical edge history stays owned.
    for i in range(4):Peripheral.from_address(pointers[i]).on=0xffffffff
    check(poll()==0,'sampling after game-side peripheral overlay')
    check(all(Peripheral.from_address(pointers[i]).old==old[i] and Peripheral.from_address(pointers[i]).press==0 for i in range(4)),'demo overlay does not fabricate physical button edges')
    for bad_field,bad_value in [('connected',2),('buttons',0x80000000),('x1',128),('y1',-129),('x2',128),('y2',-129),('left',256),('right',256)]:
        samples[3]=Sample(1,4,0,0,0,0,0,0);setattr(samples[3],bad_field,bad_value)
        before=[C.string_at(ptr,C.sizeof(Peripheral)) for ptr in pointers];state=Info();snapshot(C.byref(state));count=state.polls
        check(poll()==1 and status()==1 and poll()==1,'invalid sample fails with sticky error')
        after=Info();snapshot(C.byref(after));check(after.polls==count and before==[C.string_at(ptr,C.sizeof(Peripheral)) for ptr in pointers],'failure in last port does not partially publish earlier ports')
        clear()
    samples[3]=Sample();source_ok[0]=False;before=[C.string_at(ptr,C.sizeof(Peripheral)) for ptr in pointers]
    check(poll()==5 and status()==5 and before==[C.string_at(ptr,C.sizeof(Peripheral)) for ptr in pointers],'source failure retains committed records');clear();source_ok[0]=True
    reenter[0]=True;check(poll()==0 and nested[-1]==(7,7),'reentrant poll/shutdown rejected without destroying live provider');reenter[0]=False
    thread_results=[]
    def foreign():thread_results.extend([poll(),shutdown(),snapshot(C.byref(Info())),status(),record(0)])
    thread=threading.Thread(target=foreign);thread.start();thread.join();check(thread_results==[3,3,3,3,None] and status()==0,'foreign thread cannot operate or poison owner')
    check(not record(4) and status()==1,'invalid native slot query rejected');clear()
    check(Peripheral.from_address(peripheral(1)).support==0 and Peripheral.from_address(peripheral(23)).support==0 and status()==0,'expansion ports expose stable noncontroller state')
    check(Peripheral.from_address(peripheral(24)).support==0 and status()==1,'invalid SDK port returns safe record and explicit error');clear()
    exit_sdk();check(status()==4 and shutdown()==0,'original SDK exit drains real peripheral owner')
    groups['real_peripheral_records_transitions_ownership_and_failure_paths']=checks-sum(groups.values())
    # Real Windows service smoke test in an owned hidden window: no key injection.
    user=C.WinDLL('user32',use_last_error=True);kernel=C.WinDLL('kernel32',use_last_error=True)
    create=user.CreateWindowExW;create.argtypes=[U,C.c_wchar_p,C.c_wchar_p,U,I,I,I,I,P,P,P,P];create.restype=P
    user.DestroyWindow.argtypes=[P];user.DestroyWindow.restype=I;kernel.GetModuleHandleW.argtypes=[C.c_wchar_p];kernel.GetModuleHandleW.restype=P
    window=create(0,'STATIC','Native input verification',0,0,0,16,16,None,None,kernel.GetModuleHandleW(None),None);check(bool(window),'owned hidden Win32 input window')
    live_modes=[]
    for flags in [1,2,3]:
        rc=init(window,flags,None,None)
        if rc==6:
            check(flags&2 and status()==4,'unavailable XInput is explicit and leaves no owner');live_modes.append(dict(flags=flags,available=False));continue
        check(rc==0,'actual Windows source initialized')
        for _ in range(8):check(poll()==0,'actual Windows keyboard/XInput polling succeeds or reports real disconnection')
        state=Info();check(snapshot(C.byref(state))==0 and state.source_kind==2 and state.focused==0,'actual Windows focus gate and source identity')
        check(all(Peripheral.from_address(record(i)).on==0 and Peripheral.from_address(record(i)).press==0 for i in range(4)),'unfocused source publishes no held/pressed controls')
        if flags&1:check(state.connected_mask&1,'configured native keyboard supplies logical player-zero pad')
        if flags&2:check(state.xinput_available==1,'Windows system XInput entry actually loaded')
        live_modes.append(dict(flags=flags,available=True,polls=state.polls,xinput_available=state.xinput_available));check(shutdown()==0,'actual Windows module/input owner drained')
    check(bool(user.DestroyWindow(window)),'owned hidden window destroyed')
    groups['real_Windows_services_and_unfocused_polling_smoke']=checks-sum(groups.values())
    result=dict(status='authored_Windows_input_mapping_real_SDK_records_atomic_edges_and_service_smoke_verified',checks=checks,groups=groups,all_XInput_button_masks=65536,all_signed_axis_values=65536,controlled_four_port_updates=1024,live_Windows_modes=live_modes,keyboard_events_injected=False,focused_physical_keyboard_events_verified=False,physical_controller_events_verified=False,original_pad_scan_executed=False,frame_wait_trace_replaced=False,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))


if __name__=='__main__':main()
