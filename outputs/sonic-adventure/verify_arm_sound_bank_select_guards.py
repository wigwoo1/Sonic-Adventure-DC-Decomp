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
from arm_legacy_alignment_reference import install
budget_cases=0;mmio_cases=0
for driver,present,budget in itertools.product(range(2),(False,True),(1,10,20,30,40)):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray(SIZE);memory[:len(image)]=image
    context=0xc400+0x480+2*0x48;memory[context+1]=3
    struct.pack_into('<III',memory,context+0x10,0x1b000,0x1b080,0x1b100)
    table=struct.unpack_from('<I',image,0x1800 if driver==0 else 0x180c)[0]
    struct.pack_into('<I',memory,table+24,0x1a000 if present else 0)
    struct.pack_into('<III',memory,0x1a010,0x20,3,0x60);struct.pack_into('<I',memory,0x1a020,0x80)
    native=bytearray(memory);hardware=bytearray(MMIO_SIZE)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[1]=(4<<24)|(1<<20)|(2<<16)|32;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x9d0;initial.cpsr=0xf0000013
    events=[];machine=cpu(memory,hardware,initial,events);install(machine,unicorn,arm,events,MMIO,MMIO_SIZE)
    machine.emu_start(0x9d0,STOP,count=budget);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);calls=[]
    @Read
    def read(user,a,n,v):calls.append(True);return -99
    @Write
    def write(user,a,v,n):calls.append(True);return -99
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,budget)==-3
    assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE) and not calls
    budget_cases+=1
for driver,offset,kind in itertools.product(range(2),(1,2,3),('read','write')):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray(SIZE);memory[:len(image)]=image
    entry,halt,literal=(0x634,0x638,0x95c) if kind=='read' else (0x650,0x658,0x8f0)
    struct.pack_into('<I',memory,literal,MMIO+offset)
    native=bytearray(memory);hardware=bytearray(MMIO_SIZE)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=0xf0000013
    machine=cpu(memory,hardware,initial,[]);machine.emu_start(entry,halt,count=1000);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);calls=[]
    @Read
    def read(user,a,n,v):calls.append(('read',a,n));return -52
    @Write
    def write(user,a,v,n):calls.append(('write',a,n));return -51
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,1000)==(-52 if kind=='read' else -51)
    assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE)
    assert calls==[(kind,MMIO,4)]
    mmio_cases+=1
report={'instruction_budget_cases_passed':budget_cases,'aligned_mmio_failure_cases_passed':mmio_cases,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_bank_select.c'),
 'header_sha256':sha(PROJECT/'source/arm_sound_memory.h'),'verifier_sha256':sha(Path(__file__)),
 'bank_select_evidence_sha256':sha(PROJECT/'analysis/arm_sound_bank_select_verification.json'),
 'adapter_sha256':sha(PROJECT/'arm_legacy_alignment_reference.py'),
 'scope':'Exact partial bank-selection CPU/RAM across budgets, valid/missing banks; physical aligned MMIO addresses and unchanged destination state on failing unaligned word reads/writes.'}
(PROJECT/'analysis/arm_sound_bank_select_guards_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(budget_cases,'bank-selection budget and',mmio_cases,'aligned MMIO-failure comparisons passed')
