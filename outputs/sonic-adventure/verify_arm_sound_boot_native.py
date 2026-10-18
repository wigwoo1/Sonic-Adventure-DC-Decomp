"""Compare static native ARM recovery with Unicorn executing original bytes."""
import ctypes,hashlib,itertools,json,struct,sys
from pathlib import Path
from verify_startup import ROOT
PROJECT=ROOT/'outputs/sonic-adventure';SIZE=0x200000;MMIO=0x800000;MMIO_SIZE=0x10000;STOP=0x1ff000
PACKAGE=ROOT/'work/sonic-adventure/toolchain/arm-validation-python'
sys.path.insert(0,str(PACKAGE))
import unicorn
from unicorn import arm_const as arm
assert unicorn.__version__=='2.1.4'
class State(ctypes.Structure):_fields_=[('r',ctypes.c_uint32*16),('cpsr',ctypes.c_uint32)]
BP=ctypes.POINTER(ctypes.c_uint8);U=ctypes.c_uint32
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,ctypes.POINTER(U))
Write=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,U)
Service=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,BP,ctypes.POINTER(State),U)
registers=[getattr(arm,'UC_ARM_REG_R'+str(i)) for i in range(13)]+[arm.UC_ARM_REG_SP,arm.UC_ARM_REG_LR,arm.UC_ARM_REG_PC]
dll=ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll'
fn=ctypes.CDLL(str(dll)).sa_arm_sound_boot_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,Service,U,U]
payloads=json.loads((PROJECT/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
entries=json.loads((PROJECT/'analysis/arm_sound_boot_static_translation.json').read_text())['leaf_entries']
errors=[];cases=[];boot_cases=[];boot_snapshots={};loop_cases=[]
def cpu(memory,hardware,state,events,trace=None):
    machine=unicorn.Uc(unicorn.UC_ARCH_ARM,unicorn.UC_MODE_ARM)
    machine.ctl_set_cpu_model(arm.UC_CPU_ARM_TI925T)
    machine.mem_map(0,SIZE);machine.mem_write(0,bytes(memory))
    machine.mem_map(MMIO,MMIO_SIZE);machine.mem_write(MMIO,bytes(hardware))
    machine.reg_write(arm.UC_ARM_REG_CPSR,state.cpsr)
    for reg,value in zip(registers,state.r):machine.reg_write(reg,value)
    def bus(uc,access,address,size,value,user):
        if access==unicorn.UC_MEM_READ:value=int.from_bytes(uc.mem_read(address,size),'little')
        events.append(('read' if access==unicorn.UC_MEM_READ else 'write',address,size,value))
    machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,bus,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    if trace:machine.hook_add(unicorn.UC_HOOK_CODE,trace)
    return machine

def state_of(machine):
    state=State();state.r[:]=[machine.reg_read(reg) for reg in registers]
    state.cpsr=machine.reg_read(arm.UC_ARM_REG_CPSR);return state

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
for driver,fill,flags,entry in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000),entries):
    entry=int(entry,16);image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    original[0x8c8]={0:0,0x20000000:47,0x90000000:255,0xf0000000:1}[flags]
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE;nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    expected_events=[];native_events=[]
    machine=cpu(original,hardware,initial,expected_events)
    machine.emu_start(entry,STOP,count=1000000)
    expected=state_of(machine);assert expected.r[15]==STOP,hex(expected.r[15])
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    @Read
    def read(user,address,size,value):
        try:
            assert MMIO<=address and address+size<=MMIO+MMIO_SIZE
            value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
            native_events.append(('read',address,size,value[0]));return 0
        except BaseException as exc:errors.append(repr(exc));return -9
    @Write
    def write(user,address,value,size):
        try:
            assert MMIO<=address and address+size<=MMIO+MMIO_SIZE
            value&=(1<<(size*8))-1
            nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
            native_events.append(('write',address,size,value));return 0
        except BaseException as exc:errors.append(repr(exc));return -9
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,1000000)
    assert rc==0 and not errors,(driver,hex(entry),rc,errors)
    assert bytes(expected)==bytes(actual),(driver,hex(entry),'CPU',list(expected.r),list(actual.r),hex(expected.cpsr),hex(actual.cpsr))
    assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,hex(entry),'memory')
    assert expected_events==native_events,(driver,hex(entry),'MMIO events')
    cases.append({'driver':driver,'entry':hex(entry),'fill':fill,'flags':hex(flags),'mmio_events':len(native_events)})
# Complete reset now uses native C for every subordinate call.
for driver,fill in itertools.product(range(2),(0,0x55)):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE;nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[15]=0;initial.r[14]=STOP;initial.cpsr=0xa0000013 if fill==0 else 0xf0000053
    expected_events=[];native_events=[]
    machine=cpu(original,hardware,initial,expected_events)
    machine.emu_start(0,0x194,timeout=5000000,count=2000000)
    expected=state_of(machine);assert expected.r[15]==0x194
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    @Read
    def read(user,address,size,value):
        if not MMIO<=address<address+size<=MMIO+MMIO_SIZE:errors.append(('read',hex(address)));return -9
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<address+size<=MMIO+MMIO_SIZE:errors.append(('write',hex(address)));return -9
        value&=(1<<(size*8))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),0x194,2000000)
    assert rc==0 and not errors,(driver,fill,rc,errors)
    assert bytes(expected)==bytes(actual),(driver,fill,'CPU')
    assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,fill,'memory')
    assert expected_events==native_events
    assert not actual.cpsr&0x40 and actual.r[13]==0xb000
    boot_snapshots[driver]=(bytes(native),bytes(nhardware),bytes(actual))
    boot_cases.append({'driver':driver,'fill':fill,'fully_native':True,'stop_pc':hex(actual.r[15]),'service_calls':0})
# Run one main-loop iteration from initialized state through real original ARM
# subordinates. Only boundaries originating at recovered BL sites are logged.
translation=json.loads((PROJECT/'analysis/arm_sound_boot_static_translation.json').read_text())
service_targets={int(a,16) for a in translation['service_targets']}- {0x9a4,0x6d0,0x768,0x615c,0x6164,0x63f8,0x6400,0x6620,0x6628,0x6640,0x6648,0x66e0,0x66e8,0x6780,0x6788,0x684c,0x6854,0x68f8,0x6900,0x69a8,0x69b0,0x6a5c,0x6a64,0x6bd4,0x6bdc,0x70a4,0x70ac}
service_targets-={0x4210,0x4218,0x48e8,0x48f0}
recovered_sites={int(a,16) for a in translation['instruction_addresses']}
for driver,tick,fast,enabled in itertools.product(range(2),(0,1),(0,1),(0,1)):
    print('Main loop case',driver,tick,fast,enabled,flush=True)
    memory,mmio,saved=boot_snapshots[driver]
    original=bytearray(memory);original[0x22c]=tick;original[0x22e]=fast;original[0x13400]=enabled
    native=bytearray(original);hardware=bytearray(mmio);nhardware=bytearray(mmio)
    initial=State.from_buffer_copy(saved);initial.r[15]=0x194
    expected_events=[];native_events=[];reference_services=[];native_services=[]
    seen_start=[False];previous=[None]
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    native_calls={a for a in recovered_sites if int.from_bytes(image[a:a+4],'little')&0x0f000000==0x0b000000}
    def trace(uc,address,size,user):
        prior=previous[0];previous[0]=address
        if address==0x194:
            if seen_start[0]:uc.emu_stop();return
            seen_start[0]=True
        if address in service_targets and prior in native_calls:
            reference_services.append((address,bytes(state_of(uc)),hashlib.sha256(uc.mem_read(0,SIZE)).hexdigest()))
    machine=cpu(original,hardware,initial,expected_events,trace)
    machine.emu_start(0x194,STOP,timeout=5000000,count=2000000)
    expected=state_of(machine);assert expected.r[15]==0x194
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    @Read
    def read(user,address,size,value):
        if not MMIO<=address<address+size<=MMIO+MMIO_SIZE:errors.append(('read',hex(address)));return -9
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<address+size<=MMIO+MMIO_SIZE:errors.append(('write',hex(address)));return -9
        value&=(1<<(size*8))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    @Service
    def service(user,memory,state,target):
        try:
            assert target in service_targets
            native_services.append((target,bytes(state.contents),hashlib.sha256(native).hexdigest()))
            child=cpu(native,nhardware,state.contents,native_events)
            child.emu_start(target,state.contents.r[14],timeout=5000000,count=2000000)
            updated=state_of(child);assert updated.r[15]==state.contents.r[14],hex(updated.r[15])
            native[:]=child.mem_read(0,SIZE);nhardware[:]=child.mem_read(MMIO,MMIO_SIZE)
            ctypes.memmove(state,ctypes.byref(updated),ctypes.sizeof(updated));return 0
        except BaseException as exc:errors.append(repr(exc));return -9
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),0x194,2000000)==0 and not errors,errors
    assert bytes(expected)==bytes(actual),(driver,tick,fast,enabled,'CPU')
    assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,tick,fast,enabled,'memory')
    assert expected_events==native_events and reference_services==native_services,(driver,tick,fast,enabled,'events')
    loop_cases.append({'driver':driver,'tick':tick,'fast':fast,'enabled':enabled,'service_calls':len(native_services)})
report={'leaf_cases_passed':len(cases),'boot_cases_passed':len(boot_cases),'loop_cases_passed':len(loop_cases),'leaf_entries':entries,'cases':cases,'boot_cases':boot_cases,'loop_cases':loop_cases,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_boot_native.c'),
 'reset_services_source_sha256':sha(PROJECT/'source/sa_arm_sound_reset_services.c'),'reset_translation_sha256':sha(PROJECT/'analysis/arm_sound_reset_services_translation.json'),
 'commands_source_sha256':sha(PROJECT/'source/sa_arm_sound_commands.c'),'commands_translation_sha256':sha(PROJECT/'analysis/arm_sound_commands_translation.json'),
 'bank_select_source_sha256':sha(PROJECT/'source/sa_arm_sound_bank_select.c'),'memory_header_sha256':sha(PROJECT/'source/arm_sound_memory.h'),
 'controls_source_sha256':sha(PROJECT/'source/sa_arm_sound_controls.c'),'controls_translation_sha256':sha(PROJECT/'analysis/arm_sound_controls_translation.json'),
 'notes_source_sha256':sha(PROJECT/'source/sa_arm_sound_notes.c'),'notes_translation_sha256':sha(PROJECT/'analysis/arm_sound_notes_translation.json'),
 'status_source_sha256':sha(PROJECT/'source/sa_arm_sound_status.c'),'status_translation_sha256':sha(PROJECT/'analysis/arm_sound_status_translation.json'),
 'update_source_sha256':sha(PROJECT/'source/sa_arm_sound_update.c'),'update_translation_sha256':sha(PROJECT/'analysis/arm_sound_update_translation.json'),
 'snapshot_source_sha256':sha(PROJECT/'source/sa_arm_sound_snapshot.c'),'snapshot_translation_sha256':sha(PROJECT/'analysis/arm_sound_snapshot_translation.json'),
 'ramps_source_sha256':sha(PROJECT/'source/sa_arm_sound_ramps.c'),'ramps_translation_sha256':sha(PROJECT/'analysis/arm_sound_ramps_translation.json'),
 'effects_source_sha256':sha(PROJECT/'source/sa_arm_sound_effects.c'),'effects_translation_sha256':sha(PROJECT/'analysis/arm_sound_effects_translation.json'),
 'stream_source_sha256':sha(PROJECT/'source/sa_arm_sound_stream_update.c'),'stream_translation_sha256':sha(PROJECT/'analysis/arm_sound_stream_update_translation.json'),
 'sequence_source_sha256':sha(PROJECT/'source/sa_arm_sound_sequencer.c'),'sequence_translation_sha256':sha(PROJECT/'analysis/arm_sound_sequencer_translation.json'),
 'dispatcher_source_sha256':sha(PROJECT/'source/sa_arm_sound_dispatcher.c'),
 'builder_sha256':sha(PROJECT/'build_arm_sound_boot_native.ps1'),'verifier_sha256':sha(Path(__file__)),
 'inventory_evidence_sha256':sha(PROJECT/'analysis/arm_sound_driver_inventory_verification.json'),
 'payload_evidence_sha256':sha(PROJECT/'analysis/arm_sound_driver_payloads.json'),
 'translation_metadata_sha256':sha(PROJECT/'analysis/arm_sound_boot_static_translation.json'),
 'reference_version':unicorn.__version__,'reference_model':'UC_CPU_ARM_TI925T',
 'reference_files':{str(path.relative_to(ROOT)).replace('\\','/'):sha(path) for path in PACKAGE.rglob('*') if path.is_file() and path.suffix in ('.dll','.py')},
 'scope':'Twelve fully native ARM leaf helpers for each original driver, complete RAM/active CPU/CPSR/MMIO event equality against Unicorn executing actual ARM bytes. Complete reset through 194 executes fully in native C with no external ARM services, for both drivers and two memory/MMIO fills.',
 'limitations':'SVC ARM state, two MiB RAM and modeled MMIO only; ARM core model is TI925T instruction reference, not a timing/hardware oracle. Main-loop dispatch is compared using original ARM subroutines as explicit validation-only services; those callees remain unrecovered C. No driver execution/playback completion claim.'}
(PROJECT/'analysis/arm_sound_boot_native_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'native ARM helper comparisons and',len(boot_cases),'reset-prefix and',len(loop_cases),'main-loop comparisons passed')
