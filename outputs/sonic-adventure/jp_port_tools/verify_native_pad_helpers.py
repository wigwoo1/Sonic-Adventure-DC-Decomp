"""Execute original Japanese pad helpers and their complete atan dependency chain.
The full pad_scan/recording_update path is deliberately not substituted.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_task_test_support_v2 import Work
from verify_native_input_backend import Sample,Source,Peripheral
from camera_reference_numeric import service_for
U=C.c_uint32;I=C.c_int;P=C.c_void_p
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();p=r/'outputs/sonic-adventure';assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_bytes());pin=json.loads((p/'jp_port_tools/jp_camera_external_inputs.json').read_bytes())
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    lib=C.CDLL(b['frames_DLL']);sys.path.insert(0,str(p));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=I;numeric.argtypes=[U,C.POINTER(Fpu),U];service=service_for(numeric)
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;BASE=0x8c000000
    def put(address,data):ram[address-BASE:address-BASE+len(data)]=data
    def read(address,size):return bytes(ram[address-BASE:address-BASE+size])
    def word(address,value):put(address,struct.pack('<I',value&0xffffffff))
    checks=calls=0;groups={};seen=set();callback_errors=[];rng=random.Random(0x50414453)
    trace_names=['pad_read_sticks','atan2f','_in_atan2s','_in_atans','_app_atans','_in_fmods','_in_ldexps'];entries={table[n]:n for n in trace_names}
    def trace(pc,op,registers,sr,pr,fpu):
        if pc in entries:seen.add(entries[pc])
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def original(name,bank=0,arg=0):
        nonlocal calls
        f=Fpu();f.fpscr=0x40001|(bank<<21)
        for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
        s=State();s.r[15]=0x8cf00000;s.r[4]=arg;s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000
        reference(ram,s,fpu=f,vector_instruction=service,instruction_budget=18000,trace=trace)
        check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),('original helper stack and FPU mode restored',name));calls+=1
    samples=(Sample*4)()
    @Source
    def source(context,out):
        try:C.memmove(out,samples,C.sizeof(samples));return 1
        except BaseException as error:callback_errors.append(str(error));return 0
    initialize=bind(lib,'jp_input_initialize',I,P,U,P,P);shutdown=bind(lib,'jp_input_shutdown',I);poll=bind(lib,'jp_input_poll',I);record=bind(lib,'jp_input_record',P,U)
    ports=bind(lib,'jp_pad_bind_test_ports',None);sticks=bind(lib,'jp_pad_test_sticks',None);buttons=bind(lib,'jp_pad_test_buttons',None,I);quit_count=bind(lib,'jp_pad_test_quit',None);quit_pressed=bind(lib,'pad_quit_pressed',I,I);reset=bind(lib,'jp_pad_test_reset',None)
    enable=bind(lib,'pad_enable',None);disable=bind(lib,'pad_disable',None);enable_player=bind(lib,'pad_enable_player',None,C.c_uint8);disable_player=bind(lib,'pad_disable_player',None,C.c_uint8)
    check(initialize(None,0,C.cast(source,P),None)==0,'actual integrated native input owner')
    camera=P.in_dll(lib,'camera_work');camera_value=Work()
    arrays={n:(U*count).in_dll(lib,n) for n,count in [('pad_sticks',16),('pad_sticks_read',16),('pad_held',4),('pad_press_repeat',4),('pad_held_live',4),('pad_press_live',4)]}
    def update():
        check(poll()==0 and not callback_errors,'real provider accepts controlled physical-unit samples');ports()
        for i in range(4):
            address=0x8cd10000+i*64;put(address,bytes(Peripheral.from_address(record(i)))[:36]+bytes(16));word(table['per_info']+4*i,address)
    def fresh():
        reset();put(table['pads_on'],b'\x01');put(table['player_pads_on'],b'\x01\x01');put(table['quit_pressed'],b'\0');put(table['quit_held_scans'],b'\0\0')
        for n in ['pad_held','pad_press_repeat','pad_held_live','pad_press_live','pad_repeat_scans']:put(table[n],bytes(16))
    axes=[(-128,-128),(-128,0),(-128,127),(0,-128),(0,0),(0,127),(127,-128),(127,0),(127,127)]
    axes += [(x,y) for x in [-13,-12,-11,0,11,12,13] for y in [-13,-12,-11,0,11,12,13]]
    for case in range(384):
        fresh();a0=axes[case] if case<len(axes) else (rng.randrange(-128,128),rng.randrange(-128,128));a1=axes[-case-1] if case<len(axes) else (rng.randrange(-128,128),rng.randrange(-128,128))
        for i in range(4):samples[i]=Sample(1,0,*(a0 if i==0 else a1 if i==1 else (0,0)),0,0,0,0)
        update();on=case%4!=0
        if not on:disable();put(table['pads_on'],b'\0')
        bits=[case%3!=0,case%5!=0]
        for i,active in enumerate(bits):
            if not active:disable_player(i)
        put(table['player_pads_on'],bytes(bits))
        if case%3:
            angle=[0,1,-1,0x4000,0x7fff,0x8000,0xffff,0x12345678,-0x12345678][case%9];camera_value.ang.y=angle;camera.value=C.addressof(camera_value);word(table['camera_work'],0x8cd20000);put(0x8cd20000,bytes(64));word(0x8cd20018,angle)
        else:camera.value=None;word(table['camera_work'],0)
        original('pad_read_sticks',case%2);sticks()
        for n in ['pad_sticks_read','pad_sticks']:check(bytes(arrays[n])==read(table[n],64),('whole original eight-player analog array',n,case,a0,a1))
    check(set(trace_names)<=seen,('complete original atan2/runtime dependency chain executed without hooks',sorted(seen)))
    groups['original_eight_player_analog_RZ_values_and_camera_relative_angles']=checks
    # Repeat/enable masks operate on actual stable peripheral records.
    fresh();camera.value=None;word(table['camera_work'],0)
    directions=[(0,79,0),(0,78,0),(0,-79,0),(79,0,0),(-79,0,0),(0,0,16),(0,0,4),(127,127,0)]
    for case in range(128):
        if case%16==0:fresh()
        x,y,mask=directions[(case//16)%len(directions)]
        if case%16 in [14,15]:mask^=4
        for i in range(4):samples[i]=Sample(1,mask,x,y,0,0,0,0)
        update()
        if case%11==0:disable();put(table['pads_on'],b'\0')
        else:enable();put(table['pads_on'],b'\x01')
        for slot in range(4):
            original('pad_read_buttons',case%2,slot);buttons(slot)
            for n in ['pad_held','pad_press_repeat','pad_held_live','pad_press_live']:check(bytes(arrays[n])==read(table[n],16),('whole original repeat/held/live mask array',n,case,slot))
    # Continuous held directions exercise original scan-31 and six-scan repeats.
    fresh()
    for case in range(80):
        for i in range(4):samples[i]=Sample(1,16,0,0,0,0,0,0)
        update();original('pad_read_buttons',case%2,0);buttons(0)
        check(bytes(arrays['pad_press_repeat'])==read(table['pad_press_repeat'],16),'original continuous directional repeat cadence')
    groups['original_button_threshold_repeat_and_enable_masks']=checks-sum(groups.values())
    fresh();combo=4|2|1024|512|8
    for case in range(128):
        for i in range(4):samples[i]=Sample(1,(combo if i==case%4 and case%3!=0 else combo&~(1<<(case%2+1))),0,0,0,0,0,0)
        update();original('pad_count_quit',case%2);quit_count()
        expected=read(table['quit_pressed'],1)[0];check(quit_pressed(0)==expected and quit_pressed(99)==expected,'original all-port quit combination and press-edge behavior')
    groups['original_quit_combo_on_all_four_ports']=checks-sum(groups.values())
    camera.value=None;check(shutdown()==0,'real native peripheral owner drained')
    result=dict(status='original_Japanese_pad_helpers_native_input_RZ_analog_repeat_quit_and_complete_atan_chain_verified',checks=checks,groups=groups,original_helper_calls=calls,original_analog_parent_calls=384,original_button_parent_calls=592,original_quit_parent_calls=128,original_runtime_dependencies_executed=sorted(seen),original_dependency_hooks_used=False,original_mode='single_RZ_DN1_traps_disabled_both_register_banks',analog_axis_domain='original controller units -128..127',original_GCC_two_player_bounds_repair_retained=True,native_negative_shift_UB_removed=True,original_pad_scan_executed=False,original_pad_scan_remaining_imports=b['full_pad_link_audit']['remaining_imports'],frame_wait_input_trace_replaced=False,physical_keyboard_or_controller_events_verified=False,original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))


if __name__=='__main__':main()
