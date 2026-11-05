"""Original initialization oracle, actual SDK ABI and real VMU attachment I/O."""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from verify_native_input_backend import Source,Sample,Peripheral,Info
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;P=C.c_void_p;S=C.c_size_t
Metadata=C.CFUNCTYPE(I,P,U,P,S)
class Description(C.Structure):
    _fields_=[('type',U),('reserved',U*3),('root',C.c_uint8),('area',C.c_uint8),('direction',C.c_uint8*2),('product',C.c_char*32),('license',C.c_char*64),('standby',C.c_uint16),('maximum',C.c_uint16)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'];lib=C.CDLL(b['frames_DLL']);checks=0;groups={};rng=random.Random(0x5044494e)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def group(n):groups[n]=checks-sum(groups.values())
    # A real peer uses unmodified SDK declarations/import signatures.
    source=d/'sdk_peripheral_client.c';source.write_text('''#include <shinobi.h>
__declspec(dllexport) void client_init(Sint32 logic,void *recv,void *send){pdInitPeripheral(logic,recv,send);}
__declspec(dllexport) void client_exit(void){pdExitPeripheral();}
__declspec(dllexport) const PDS_PERIPHERAL *client_port(Uint32 port){return pdGetPeripheral(port);}
__declspec(dllexport) Sint32 client_error(Uint32 port){return pdGetPeripheralError(port);}
__declspec(dllexport) unsigned long client_info_bytes(void){return sizeof(PDS_PERIPHERALINFO);}
''')
    ib=json.loads((Path(b['input_build'])/'build_verification.json').read_bytes());common=ib['commands'][0]['command'];common=common[:common.index('-c')];compiler=common[0]
    commands=[common+['-c',str(source),'-o',str(d/'client.obj')],[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'client.obj'),str(a.build/'jp_native_frames.lib'),'-o',str(d/'sdk_peripheral_client.dll')]]
    for j,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);(d/f'client_build_{j}.txt').write_bytes(run.stdout+run.stderr);check(not run.returncode and not run.stdout and not run.stderr,'real SDK input client compiles/links warning-free')
    client_source=source
    peer=C.CDLL(str(d/'sdk_peripheral_client.dll'));sdkinit=bind(peer,'client_init',None,I,P,P);sdkclose=bind(peer,'client_exit',None);port=bind(peer,'client_port',P,U);pererror=bind(peer,'client_error',I,U)
    configure=bind(lib,'jp_input_configure',I,P,U,P,P);configure_metadata=bind(lib,'jp_input_configure_expansions',I,P,P);bootstrap=bind(lib,'jp_input_bootstrap_status',I);status=bind(lib,'jp_input_status',I)
    poll=bind(lib,'jp_input_poll',I);server=bind(lib,'pdExecPeripheralServer',None);close=bind(lib,'jp_input_shutdown',I);clear=bind(lib,'jp_input_clear_error',None);snapshot=bind(lib,'jp_input_snapshot',I,P);description=bind(lib,'pdGetPeripheralInfo',P,U)
    def info():out=Info();check(snapshot(C.byref(out))==0,'actual input snapshot');return out
    check(bind(peer,'client_info_bytes',U)()==C.sizeof(Description)==120,'actual SDK metadata ABI is 120 bytes')
    sdkinit(0,None,None);check(bootstrap()==2 and status()==4,'SDK void initializer reports missing real host configuration')
    check(configure(P(1),1,None,None)==1 and status()==4,'invalid actual keyboard window cannot initialize SDK input')
    samples=(Sample*4)();bad=False;recursive=False;recursive_results=[]
    @Source
    def source(context,out):
        if recursive:recursive_results.extend([poll(),close()])
        C.memmove(out,samples,C.sizeof(samples));return 0 if bad else 1
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,reference_hashes=make_reference(executor)
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    ram=bytearray(16<<20);image=a.binary.read_bytes();ram[0x10000:0x10000+len(image)]=image;calls={};entered={}
    entries={table[n]:n for n in ('pdInitPeripheral','clear_peripheral','pdGetPeripheral','pdGetPeripheralError')}
    def trace(pc,*unused):
        if pc in entries:entered[entries[pc]]=entered.get(entries[pc],0)+1
    def read(at,size):at&=0xffffff;return bytes(ram[at:at+size])
    def original(name,*args):
        cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        for j,value in enumerate(args):cpu.r[4+j]=value&0xffffffff
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,trace=trace,instruction_budget=200000);calls[name]=calls.get(name,0)+1
        check(cpu.r[15]==0x8cf00000,'original peripheral instruction call restores stack');return cpu.r[0]
    previous=[0]*4;buffers=(C.c_ubyte*128)(*([0x5a]*128));buffer_before=bytes(buffers)
    for logic in (0,1):
        check(configure(None,0,C.cast(source,P),None)==0 and configure_metadata(None,None)==0,'explicit controlled source configures existing owner')
        sdkinit(logic,C.addressof(buffers),C.addressof(buffers)+64);check(bootstrap()==0 and status()==0 and bytes(buffers)==buffer_before,'SDK initializer starts actual provider without touching obsolete console DMA arguments')
        original('pdInitPeripheral',logic,0,0);addresses=[port(j) for j in range(24)]
        check(len(set(addresses))==24 and all(addresses),'all 24 SDK ports have stable distinct full-width records')
        for j in range(24):
            guest=original('pdGetPeripheral',j);values=struct.unpack('<6I2H4h4I',read(guest,52));actual=Peripheral.from_address(addresses[j])
            names=('id','support','on','off','press','release','r','l','x1','y1','x2','y2')
            check(tuple(getattr(actual,n) for n in names)==values[:12] and actual.old==values[14] and not actual.extend and not actual.info,'native SDK reset matches complete original initializer fields at every port and both polarities')
            check(C.string_at(actual.name)==read(values[12],32).split(b'\0')[0]==b'(no device)' and pererror(j)==original('pdGetPeripheralError',j)==0,'original reset name/error semantics with relocated native pointers')
        previous=[0]*4
        for step in range(128):
            for j in range(4):samples[j]=Sample(rng.randrange(2),rng.getrandbits(18)&0x307ff,*[rng.randrange(-128,128) for _ in range(4)],rng.randrange(256),rng.randrange(256))
            server();check(status()==0,'SDK server polls same real native owner')
            for j in range(4):
                actual=Peripheral.from_address(port(j*6));on=samples[j].buttons if samples[j].connected else 0;old=previous[j]
                expected=(old,on,(~on)&0xffffffff,on&~old,old&~on)
                if logic:expected=((~old)&0xffffffff,(~on)&0xffffffff,on,(~(on&~old))&0xffffffff,(~(old&~on))&0xffffffff)
                check(tuple(getattr(actual,n) for n in ('old','on','off','press','release'))==expected,'both original button polarities preserve independent physical edge history')
                check(port(j*6)==addresses[j*6] and pererror(j*6)==0,'SDK record identity remains stable while sampled')
                previous[j]=on
        check(info().polls==128,'all actual SDK polls accounted');sdkclose();check(status()==4,'SDK exit closes actual input owner')
    group('complete_original_software_initializer_and_native_positive_negative_sampling')
    # Metadata callbacks are staged with root input before any record commits.
    fail_metadata=False;invalid_metadata=False;metadata_reenter=False;metadata_nested=[];metadata_calls=0
    @Metadata
    def metadata(context,p,out,size):
        nonlocal metadata_calls
        metadata_calls+=1
        if metadata_reenter:metadata_nested.extend([poll(),close()])
        if p==20 and fail_metadata:return -1
        if p==1:
            value=Description();value.type=64 if invalid_metadata else 14;value.product=b'VMU metadata fixture';C.memmove(out,C.byref(value),C.sizeof(value));return 1
        return 0
    check(configure(None,0,C.cast(source,P),None)==0 and configure_metadata(C.cast(metadata,P),None)==0,'configure explicit metadata provider before SDK startup');sdkinit(0,None,None)
    check(bootstrap()==0 and poll()==0 and Description.from_address(description(1)).type==14,'actual expansion record receives typed metadata without replacing root record')
    addresses=[port(j) for j in range(24)];before=[C.string_at(address,C.sizeof(Peripheral)) for address in addresses];old_info=info().polls;fail_metadata=True;samples[0].buttons^=4
    check(poll()==5 and info().polls==old_info and before==[C.string_at(address,C.sizeof(Peripheral)) for address in addresses],'metadata failure cannot partially commit controller records')
    # Access records through previously owned pointers while sticky status
    # intentionally blocks new public peripheral lookups.
    clear();fail_metadata=False;invalid_metadata=True;before=[C.string_at(port(j),C.sizeof(Peripheral)) for j in range(24)]
    check(poll()==1 and before==[C.string_at(address,C.sizeof(Peripheral)) for address in addresses],'unsupported keyboard/root type cannot masquerade as a VMU or modify records');clear();invalid_metadata=False
    metadata_reenter=True;recursive=True;check(poll()==0 and recursive_results[-2:]==[7,7] and set(metadata_nested)=={7},'controller and metadata callbacks cannot recursively poll or close owner');recursive=False;metadata_reenter=False
    foreign=[];thread=threading.Thread(target=lambda:foreign.extend([configure(None,0,C.cast(source,P),None),configure_metadata(None,None),close(),bootstrap()]));thread.start();thread.join()
    check(foreign==[3,3,3,3] and status()==0,'pending configuration and live input remain on creator thread');sdkclose()
    before_calls=metadata_calls
    check(bind(lib,'jp_input_initialize',I,P,U,P,P)(None,0,C.cast(source,P),None)==0 and poll()==0 and metadata_calls==before_calls and close()==0,'shutdown cancels borrowed metadata configuration before an independent later native initialization')
    check(configure(None,0,C.cast(source,P),None)==0,'cold pending configuration has an explicit owner')
    foreign=[];thread=threading.Thread(target=lambda:foreign.extend([bind(lib,'jp_input_initialize',I,P,U,P,P)(None,0,C.cast(source,P),None),close()]));thread.start();thread.join()
    check(foreign==[3,3] and close()==0,'foreign initialization/shutdown cannot steal or cancel pending borrowed callbacks')
    sdkinit(0,None,None);check(bootstrap()==2 and status()==4,'cancelled pending configuration cannot be reused by SDK initialization');group('atomic_metadata_failures_reentrancy_and_thread_ownership')
    media=d/'media';media.mkdir();store_init=bind(lib,'jp_vmu_store_initialize',I,C.c_wchar_p);store_create=bind(lib,'jp_vmu_store_create',I,U);store_open=bind(lib,'jp_vmu_store_open',I,U);store_close=bind(lib,'jp_vmu_store_close',I,U)
    check(bind(lib,'jp_input_use_virtual_cards',I)()==0 and configure(None,0,C.cast(source,P),None)==0,'configure actual shared VMU metadata bridge');sdkinit(0,None,None)
    check(bootstrap()==0 and poll()==0 and not description(1),'no initialized VMU owner advertises no card')
    check(store_init(str(media))==0 and bind(lib,'jp_lcd_initialize',I,U)(255)==0,'real disk-backed VMU store and existing LCD owner initialized')
    for drive in range(8):check(store_create(drive)==0 and store_open(drive)==0,'create/open actual fresh virtual card under exclusive storage ownership')
    hashes={f.name:sha(f) for f in media.glob('*.VMU')};check(len(hashes)==8,'eight actual 128KB card images exist')
    check(poll()==0,'real metadata refresh after card insertions')
    for drive in range(8):
        p=drive//2*6+drive%2+1;pointer=description(p);actual=Peripheral.from_address(port(p))
        check(pointer and actual.info==pointer and Description.from_address(pointer).type==14 and actual.id==actual.support==0,'SDK discovers actual storage/LCD/timer attachment in correct expansion port')
    check(store_close(3)==0 and poll()==0 and not description(8) and description(7),'actual card removal clears precisely its own SDK expansion record')
    sdkclose();check(bind(lib,'jp_vmu_store_status',I)()==0 and bind(lib,'jp_lcd_service',I)()==0,'controller shutdown leaves independent VMU store/LCD owners alive')
    for drive in range(8):check(store_close(drive)==0,'release actual test card lock')
    check(bind(lib,'jp_lcd_shutdown',I)()==0 and bind(lib,'jp_vmu_store_shutdown',I)()==0,'VMU owners shut down explicitly after input consumer')
    check(hashes=={f.name:sha(f) for f in media.glob('*.VMU')},'metadata polling preserves every actual card image byte');group('actual_shared_eight_VMU_cards_and_safe_removal')
    # SDK startup with a real owned Windows window, without key injection.
    user=C.WinDLL('user32');kernel=C.WinDLL('kernel32');kernel.GetModuleHandleW.restype=P
    user.CreateWindowExW.restype=P;user.CreateWindowExW.argtypes=[U,C.c_wchar_p,C.c_wchar_p,U,I,I,I,I,P,P,P,P];user.DestroyWindow.argtypes=[P]
    hwnd=user.CreateWindowExW(0,'STATIC','Native SDK peripheral check',0x80000000,0,0,64,64,None,None,kernel.GetModuleHandleW(None),None)
    check(bool(hwnd) and configure_metadata(None,None)==0 and configure(hwnd,3,None,None)==0,'real local HWND and Windows keyboard/XInput configuration')
    sdkinit(0,None,None);check(bootstrap()==0 and status()==0,'SDK entry actually opens Windows input services');check(poll()==0 and info().source_kind==2 and info().xinput_available,'real Windows collector executes through SDK-created owner')
    sdkclose();check(status()==4 and user.DestroyWindow(hwnd),'SDK shutdown releases real input before owned test window');group('real_Windows_SDK_startup_and_shutdown_smoke')
    result=dict(status='SDK_peripheral_startup_original_reset_polarities_atomic_metadata_and_actual_VMU_cards_verified',checks=checks,groups=groups,
        original_complete_machine_calls=sum(calls.values()),original_calls=calls,original_entries=entered,original_instruction_function_hooks_used=False,
        original_DMA_start_branch_executed=False,native_DMA_arguments_read_or_written=False,reference_hashes=reference_hashes,
        actual_virtual_cards=8,actual_card_images_sha256=hashes,metadata_polling_modifies_card_images=False,
        actual_Windows_keyboard_XInput_service_opened=True,physical_keyboard_or_controller_events_verified=False,
        client_source_sha256=sha(client_source),client_DLL_sha256=sha(d/'sdk_peripheral_client.dll'),build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
        full_sbInitSystem_audio_gameplay_or_VMU_program_verified=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',sum(calls.values()),'original calls',flush=True)
if __name__=='__main__':main()
