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
reset_translation=json.loads((PROJECT/'analysis/arm_sound_reset_services_translation.json').read_text())
entries=reset_translation['entries_by_driver']
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
for driver,fill,flags,entry_index,pair in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000),range(7),((0,0),(0x16000,4),(0x16000,16))):
    entry=entries[driver][entry_index]
    entry=int(entry,16);image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    original[0x8c8]={0:0,0x20000000:47,0x90000000:255,0xf0000000:1}[flags]
    struct.pack_into('<II',original,0x14288,*pair)
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE;nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
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
    cases.append({'driver':driver,'entry':hex(entry),'fill':fill,'flags':hex(flags),'pair':pair,'mmio_events':len(native_events)})
report={'cases_passed':len(cases),'entries_by_driver':entries,'cases':cases,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_reset_services.c'),
 'verifier_sha256':sha(Path(__file__)),'translation_sha256':sha(PROJECT/'analysis/arm_sound_reset_services_translation.json'),
 'boot_evidence_sha256':sha(PROJECT/'analysis/arm_sound_boot_native_verification.json'),
 'scope':'Seven complete reset callees/dependencies per driver, no external callbacks: full RAM/active CPU/CPSR/MMIO and ordered bus equality against original ARM execution across fill/flag/clear-range cases.',
 'limitations':'SVC ARM state, stable two MiB RAM and modeled MMIO. Reference CPU model is not a hardware-timing oracle; no audio/playback claim.'}
(PROJECT/'analysis/arm_sound_reset_services_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'complete native ARM reset-service comparisons passed')
