"""Atomic API guards, instruction budgets and failing MMIO services."""
import ctypes,hashlib,itertools,json,sys
from pathlib import Path
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';size=0x200000;stop=0x1ff000
sys.path.insert(0,str(ROOT/'work/sonic-adventure/toolchain/arm-validation-python'))
import unicorn
from unicorn import arm_const as arm
class State(ctypes.Structure):_fields_=[('r',ctypes.c_uint32*16),('cpsr',ctypes.c_uint32)]
U=ctypes.c_uint32;BP=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,ctypes.POINTER(U))
Write=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,U)
Service=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,BP,ctypes.POINTER(State),U)
registers=[getattr(arm,'UC_ARM_REG_R'+str(i)) for i in range(13)]+[arm.UC_ARM_REG_SP,arm.UC_ARM_REG_LR,arm.UC_ARM_REG_PC]
dll=ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll'
fn=ctypes.CDLL(str(dll)).sa_arm_sound_boot_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,Service,U,U]
calls=[]
@Read
def fail_read(user,address,length,value):calls.append(('read',address,length));return -52
@Write
def fail_write(user,address,value,length):calls.append(('write',address,length,value));return -51
ram=bytearray(size);view=(ctypes.c_uint8*size).from_buffer(ram)
state=State();state.cpsr=0x13;state.r[15]=0x43c
args=[view,ctypes.byref(state),0,None,fail_read,fail_write,Service(),stop,100]
def snapshot():return hashlib.sha256(ram).digest(),bytes(state),tuple(calls)
atomic=0
for index,value in ((0,None),(1,None),(2,2),(4,Read()),(5,Write())):
    supplied=list(args);supplied[index]=value;before=snapshot()
    assert fn(*supplied)==-2 and snapshot()==before
    atomic+=1
for cpsr in (0x10,0x33):
    state.cpsr=cpsr;before=snapshot();assert fn(*args)==-2 and snapshot()==before;atomic+=1
state.cpsr=0x13;supplied=list(args);supplied[-1]=0;before=snapshot()
assert fn(*supplied)==-3 and snapshot()==before;atomic+=1
state.r[15]=0xdead;before=snapshot();assert fn(*args)==-8 and snapshot()==before;atomic+=1
payloads=json.loads((project/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
budgets=0;bus_errors=0
for driver,budget,entry,halt,want in [
    (d,b,0x43c,None,-3) for d,b in itertools.product(range(2),(1,3,4,10,64,1000))
]+[(d,b,0x5d54+8*d,None,-3) for d,b in itertools.product(range(2),(1,3,4,10,64,1000))]+[(d,100,e,h,w) for d,(e,h,w) in itertools.product(range(2),((0x634,0x638,-52),(0x650,0x658,-51)))]+[(d,100,e+8*d,h+8*d,w) for d,(e,h,w) in itertools.product(range(2),((0x62b0,0x62bc,-52),(0x5d78,0x5d88,-51)))]:
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    ram=bytearray([0x55])*size;ram[:len(image)]=image
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=stop;initial.r[15]=entry;initial.cpsr=0xa0000013
    machine=unicorn.Uc(unicorn.UC_ARCH_ARM,unicorn.UC_MODE_ARM)
    machine.ctl_set_cpu_model(arm.UC_CPU_ARM_TI925T)
    machine.mem_map(0,size);machine.mem_write(0,bytes(ram))
    machine.mem_map(0x800000,0x10000);hardware=bytearray(0x10000)
    machine.reg_write(arm.UC_ARM_REG_CPSR,initial.cpsr)
    for register,value in zip(registers,initial.r):machine.reg_write(register,value)
    machine.emu_start(entry,halt if halt is not None else stop,count=budget)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*size).from_buffer(ram);calls.clear()
    @Read
    def read(user,address,length,value):
        if not 0x800000<=address<address+length<=0x810000:return -9
        value[0]=int.from_bytes(hardware[address-0x800000:address-0x800000+length],'little');return 0
    @Write
    def write(user,address,value,length):
        if not 0x800000<=address<address+length<=0x810000:return -9
        hardware[address-0x800000:address-0x800000+length]=(value&((1<<(length*8))-1)).to_bytes(length,'little');return 0
    assert fn(view,ctypes.byref(actual),driver,None,read if halt is None else fail_read,write if halt is None else fail_write,Service(),stop,budget)==want
    assert list(actual.r)==[machine.reg_read(r) for r in registers]
    assert actual.cpsr==machine.reg_read(arm.UC_ARM_REG_CPSR) and ram==machine.mem_read(0,size)
    assert hardware==machine.mem_read(0x800000,0x10000)
    if halt is None:assert not calls;budgets+=1
    else:assert len(calls)==1;bus_errors+=1
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
report={'atomic_guards_passed':atomic,'instruction_budget_cases_passed':budgets,'mmio_failure_cases_passed':bus_errors,
 'dll_sha256':sha(dll),'source_sha256':sha(project/'source/sa_arm_sound_boot_native.c'),
 'reset_services_source_sha256':sha(project/'source/sa_arm_sound_reset_services.c'),
 'verifier_sha256':sha(Path(__file__)),'native_evidence_sha256':sha(project/'analysis/arm_sound_boot_native_verification.json'),
 'scope':'Atomic missing-service/driver/mode/entry guards; exact partial CPU/RAM versus original ARM execution for exhausted instruction budgets and rejected first MMIO read/write.'}
(project/'analysis/arm_sound_boot_native_guards_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(atomic,'atomic guards,',budgets,'instruction-budget and',bus_errors,'MMIO-failure comparisons passed')
