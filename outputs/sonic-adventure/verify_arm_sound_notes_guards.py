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
fixtures={};fixture_hashes=[]
def native_io(hardware,events):
    @Read
    def read(user,a,n,v):
        if not MMIO<=a<a+n<=MMIO+MMIO_SIZE:errors.append(('read',hex(a)));return -9
        v[0]=int.from_bytes(hardware[a-MMIO:a-MMIO+n],'little');events.append(('read',a,n,v[0]));return 0
    @Write
    def write(user,a,v,n):
        if not MMIO<=a<a+n<=MMIO+MMIO_SIZE:errors.append(('write',hex(a)));return -9
        v&=(1<<(n*8))-1;hardware[a-MMIO:a-MMIO+n]=v.to_bytes(n,'little');events.append(('write',a,n,v));return 0
    return read,write
for driver,fill in itertools.product(range(2),(0,0x55)):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray([fill])*SIZE;memory[:len(image)]=image
    hardware=bytearray([fill])*MMIO_SIZE;native=bytearray(memory);nhardware=bytearray(hardware)
    initial=State();initial.cpsr=0x13;initial.r[15]=0;initial.r[14]=STOP
    events=[];machine=cpu(memory,hardware,initial,events);install(machine,unicorn,arm,events,MMIO,MMIO_SIZE)
    machine.emu_start(0,0x194,count=2000000)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);nevents=[];read,write=native_io(nhardware,nevents)
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),0x194,2000000)==0
    assert bytes(actual)==bytes(state_of(machine)) and native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE) and events==nevents
    for active in (0,1):
        if active:
            for group,part,note in ((1,2,60),(1,2,64),(2,2,60),(1,3,60)):
                machine.reg_write(arm.UC_ARM_REG_R1,(1<<24)|(group<<20)|(part<<16)|(note<<8)|64)
                machine.reg_write(arm.UC_ARM_REG_LR,STOP)
                machine.emu_start(0x9d0,STOP,timeout=5000000,count=1000000)
                assert machine.reg_read(arm.UC_ARM_REG_PC)==STOP
        memory=bytes(machine.mem_read(0,SIZE));hardware=bytes(machine.mem_read(MMIO,MMIO_SIZE))
        slots=sum(bool(memory[0xe800+i*64]&0x80) for i in range(48));assert slots==(4 if active else 0)
        fixtures[driver,fill,active]=(memory,hardware)
        fixture_hashes.append({'driver':driver,'fill':fill,'active':active,'active_slots':slots,'ram_sha256':hashlib.sha256(memory).hexdigest(),'mmio_sha256':hashlib.sha256(hardware).hexdigest()})
# Build full and one-free pools using original driver commands only.
for driver,fill in itertools.product(range(2),(0,0x55)):
    raw,mmio=fixtures[driver,fill,0];memory=bytearray(raw);hardware=bytearray(mmio)
    state=State();state.cpsr=0x13;state.r[13]=0xb000;state.r[14]=STOP;state.r[15]=0x9d0
    machine=cpu(memory,hardware,state,[]);install(machine,unicorn,arm,[],MMIO,MMIO_SIZE)
    for note in range(48):
        machine.reg_write(arm.UC_ARM_REG_R1,(1<<24)|(1<<20)|(2<<16)|(note<<8)|64)
        machine.reg_write(arm.UC_ARM_REG_LR,STOP);machine.emu_start(0x9d0,STOP,count=1000000)
        assert machine.reg_read(arm.UC_ARM_REG_PC)==STOP
    full=bytes(machine.mem_read(0,SIZE));hw=bytes(machine.mem_read(MMIO,MMIO_SIZE))
    assert sum(bool(full[0xe800+i*64]&0x80) for i in range(48))==48
    fixtures[driver,fill,'full']=(full,hw)
    machine.reg_write(arm.UC_ARM_REG_R1,(1<<20)|(2<<16)|64)
    machine.reg_write(arm.UC_ARM_REG_LR,STOP);machine.emu_start(0x9d0,STOP,count=1000000)
    free=bytes(machine.mem_read(0,SIZE));hw=bytes(machine.mem_read(MMIO,MMIO_SIZE))
    assert sum(bool(free[0xe800+i*64]&0x80) for i in range(48))==47 and not free[0xe800]&0x80
    fixtures[driver,fill,'one_free']=(free,hw)
budget_cases=0;bus_cases=0;cmn_cases=0;reference_corrections=0
for driver,pool,budget in itertools.product(range(2),(0,1,'full'),(1,16,32,64,128,512)):
    raw,mmio=fixtures[driver,0,pool];memory=bytearray(raw);hardware=bytearray(mmio);native=bytearray(memory);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[1]=(1<<24)|(1<<20)|(2<<16)|(60<<8)|64;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x9d0;initial.cpsr=0xf0000013
    events=[];nevents=[];machine=cpu(memory,hardware,initial,events);install(machine,unicorn,arm,events,MMIO,MMIO_SIZE)
    machine.emu_start(0x9d0,STOP,count=budget);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);read,write=native_io(nhardware,nevents)
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,budget)==(0 if expected.r[15]==STOP else -3)
    assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE)
    assert events==nevents and not errors,errors;budget_cases+=1
for driver,pool,kind in itertools.product(range(2),(0,1),('read','write')):
    raw,mmio=fixtures[driver,0,pool];memory=bytearray(raw);hardware=bytearray(mmio);native=bytearray(memory);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[1]=(1<<24)|(1<<20)|(2<<16)|(60<<8)|64;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x9d0;initial.cpsr=0xf0000013
    events=[];machine=cpu(memory,hardware,initial,events);adapted=install(machine,unicorn,arm,events,MMIO,MMIO_SIZE);captured=[]
    def fault(uc,access,address,size,value,user):
        if (access==unicorn.UC_MEM_READ)==(kind=='read') and not captured:
            captured.append((bytes(state_of(uc)),bytes(uc.mem_read(0,SIZE)),bytes(uc.mem_read(MMIO,MMIO_SIZE)),address,size));uc.emu_stop()
    machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,fault,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(0x9d0,STOP,count=2000000);assert len(captured)==1,(driver,pool,kind,len(captured),events[-5:])
    reference_corrections+=len(adapted)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);failed=[]
    @Read
    def read(user,a,n,v):
        if kind=='read':failed.append((a,n));return -52
        v[0]=int.from_bytes(nhardware[a-MMIO:a-MMIO+n],'little');return 0
    @Write
    def write(user,a,v,n):
        if kind=='write':failed.append((a,n));return -51
        nhardware[a-MMIO:a-MMIO+n]=(v&((1<<(n*8))-1)).to_bytes(n,'little');return 0
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,2000000)==(-52 if kind=='read' else -51)
    snapshot,ram,mmio,address,size=captured[0]
    assert bytes(actual)==snapshot and native==ram and nhardware==mmio and failed==[(address,size)];bus_cases+=1
for flags,value in itertools.product((0,0xf0000000),(0,1,3,0x7fffffff,0x80000000,0xfffffffd,0xfffffffe,0xffffffff)):
    raw,mmio=fixtures[1,0,0];memory=bytearray(raw);hardware=bytearray(mmio);native=bytearray(memory);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[0]=value;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0xb14;initial.cpsr=flags|0x13
    machine=cpu(memory,hardware,initial,[]);machine.emu_start(0xb14,STOP,count=1);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);nevents=[];read,write=native_io(nhardware,nevents)
    assert fn(view,ctypes.byref(actual),1,None,read,write,Service(),STOP,1)==-3
    assert bytes(actual)==bytes(expected) and native==memory and not nevents;cmn_cases+=1
report={'instruction_budget_cases_passed':budget_cases,'mmio_failure_cases_passed':bus_cases,'cmn_flag_cases_passed':cmn_cases,'reference_alignment_corrections':reference_corrections,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_notes.c'),'verifier_sha256':sha(Path(__file__)),
 'scope':'Exact bounded/partial note and allocation state in empty/active/full pools, first MMIO read/write rejection, and CMN +1 carry/overflow/zero/sign edge values.',
 'limitations':'Modeled MMIO rejection, not a hardware data-abort/exception timing proof.'}
(PROJECT/'analysis/arm_sound_notes_guards_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(budget_cases,'note budget,',bus_cases,'MMIO-failure and',cmn_cases,'CMN-flag comparisons passed')
