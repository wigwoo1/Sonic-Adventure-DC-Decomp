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
errors=[];cases=[]
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
budget_cases=0;bus_cases=0
for driver,which,budget in [(d,'batch',b) for d,b in itertools.product(range(2),(1,3,4,10,20,32))]+[(d,'reader',b) for d,b in itertools.product(range(2),(1,3,4,6,8))]:
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray(SIZE);memory[:len(image)]=image
    struct.pack_into('<II',memory,0x13000,4,0xffc)
    struct.pack_into('<I',memory,0xbffc,2<<24);struct.pack_into('<I',memory,0xb000,5<<24)
    native=bytearray(memory);hardware=bytearray(MMIO_SIZE);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x9a4 if which=='batch' else (0x1558 if driver==0 else 0x1564);initial.cpsr=0xf0000013
    machine=cpu(memory,hardware,initial,[]);machine.emu_start(initial.r[15],STOP,count=budget)
    expected=state_of(machine);assert expected.r[15]!=STOP
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);calls=[]
    @Read
    def read(user,address,size,value):calls.append(True);return -99
    @Write
    def write(user,address,value,size):calls.append(True);return -99
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,budget)==-3
    assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE) and not calls
    budget_cases+=1
for driver,fail_on in itertools.product(range(2),('read','write')):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray(SIZE);memory[:len(image)]=image
    memory[0xe800]=0x80;memory[0xe804:0xe807]=bytes((1,2,60));memory[0xe803]=0x12;memory[0xe83f]=1
    struct.pack_into('<I',memory,0xe838,0x12345678)
    native=bytearray(memory);hardware=bytearray(MMIO_SIZE);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[1]=(1<<20)|(2<<16)|(60<<8)|64;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x9d0;initial.cpsr=0xf0000013
    stop=0xaa8 if fail_on=='read' else 0xab8
    machine=cpu(memory,hardware,initial,[]);machine.emu_start(0x9d0,stop,count=100000)
    expected=state_of(machine);assert expected.r[15]==stop
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);calls=[]
    @Read
    def read(user,address,size,value):
        assert address==MMIO and size==4
        if fail_on=='read':calls.append(('read',address));return -52
        value[0]=0;return 0
    @Write
    def write(user,address,value,size):
        assert fail_on=='write' and address==MMIO and size==4
        calls.append(('write',address));return -51
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,100000)==(-52 if fail_on=='read' else -51)
    assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE)
    assert len(calls)==1 and machine.mem_read(MMIO,MMIO_SIZE)==nhardware
    bus_cases+=1
report={'instruction_budget_cases_passed':budget_cases,'mmio_failure_cases_passed':bus_cases,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_commands.c'),
 'verifier_sha256':sha(Path(__file__)),'commands_evidence_sha256':sha(PROJECT/'analysis/arm_sound_commands_verification.json'),
 'scope':'Exact partial CPU/CPSR/RAM for interrupted queue batch/reader instruction budgets and failed voice-clear MMIO reads/writes after prior state mutations; both original drivers.'}
(PROJECT/'analysis/arm_sound_commands_guards_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(budget_cases,'command budget and',bus_cases,'MMIO-failure comparisons passed')
