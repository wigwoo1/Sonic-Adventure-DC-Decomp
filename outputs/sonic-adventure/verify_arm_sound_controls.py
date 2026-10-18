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
def compare(driver,fill,active,flags,control,data,batch=False):
    raw,mmio=fixtures[driver,fill,active];memory=bytearray(raw);hardware=bytearray(mmio)
    command=(3<<24)|(1<<20)|(2<<16)|(control<<8)|data|(0xf8000000 if flags else 0)
    if batch:struct.pack_into('<II',memory,0x13000,0,0xffc);struct.pack_into('<I',memory,0xbffc,command)
    before=bytes(memory);native=bytearray(memory);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[1]=command;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x9a4 if batch else 0x9d0;initial.cpsr=flags|0x13
    events=[];nevents=[];machine=cpu(memory,hardware,initial,events);adapted=install(machine,unicorn,arm,events,MMIO,MMIO_SIZE)
    machine.emu_start(initial.r[15],STOP,timeout=5000000,count=2000000)
    assert machine.reg_read(arm.UC_ARM_REG_PC)==STOP,(driver,active,control,data,'reference stop')
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);read,write=native_io(nhardware,nevents)
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,2000000)
    assert rc==0 and not errors,(driver,fill,active,control,data,rc,hex(actual.r[15]),errors)
    expected=state_of(machine)
    assert bytes(actual)==bytes(expected),(driver,fill,active,control,data,'CPU',hex(actual.cpsr),hex(expected.cpsr))
    assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,fill,active,control,data,'memory')
    assert events==nevents,(driver,fill,active,control,data,'events')
    if batch:assert struct.unpack_from('<II',native,0x13000)==(0,0) and native[0xb000:0xc000]==before[0xb000:0xc000]
    return len(events),len(adapted),hashlib.sha256(native).hexdigest()
control_cases=0;batch_cases=0;corrections=0;mutations=set()
for driver,fill,active,flags,control,data in itertools.product(range(2),(0,0x55),(0,1),(0,0xf0000000),range(128),(0,64,127)):
    events,adapted,digest=compare(driver,fill,active,flags,control,data)
    control_cases+=1;corrections+=adapted;mutations.add(digest)
    if control_cases%768==0:print('Control cases passed:',control_cases,flush=True)
for driver,active,control in itertools.product(range(2),(0,1),range(128)):
    events,adapted,digest=compare(driver,0,active,0xf0000000,control,64,True)
    batch_cases+=1;corrections+=adapted
report={'control_cases_passed':control_cases,'native_batch_cases_passed':batch_cases,'reference_alignment_corrections':corrections,
 'fixture_hashes':fixture_hashes,'distinct_final_ram_hashes':len(mutations),
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_controls.c'),
 'verifier_sha256':sha(Path(__file__)),'generator_sha256':sha(PROJECT/'create_arm_sound_controls.py'),
 'translation_sha256':sha(PROJECT/'analysis/arm_sound_controls_translation.json'),
 'adapter_sha256':sha(PROJECT/'arm_legacy_alignment_reference.py'),'header_sha256':sha(PROJECT/'source/arm_sound_memory.h'),
 'scope':'All 128 type-3 control indices with zero/mid/max data, both drivers/fills/flag metadata and inactive/four-active-voice states prepared by original reset/note commands. Full CPU/CPSR/RAM/MMIO/ordered bus equality; all native runs use null ARM callbacks. Queued invocation covers every control index with empty/active voices.',
 'limitations':'Original type-1 note commands are used only to prepare input voice fixtures, not inside native control runs. Modeled MMIO, active SVC state, no sound synthesis/hardware timing/playback claim.'}
(PROJECT/'analysis/arm_sound_controls_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(control_cases,'control and',batch_cases,'native queued-control comparisons passed')
