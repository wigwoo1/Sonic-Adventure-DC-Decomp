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
translation=json.loads((PROJECT/'analysis/arm_sound_controls_translation.json').read_text())['drivers']
cases=0;successes=0
scenarios=('valid','missing','bad_magic','bad_version','bad_trailer','zero_count','pool_zero','pool_low','too_small','skip_size')
for driver,fill,flags,scenario,selector,kind in itertools.product(range(2),(0,0x55),(0,0xf0000000),scenarios,(0,1,2),range(4)):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray([fill])*SIZE;memory[:len(image)]=image
    memory[0x1a000:0x1a200]=bytes(0x200);header=0x1a000
    struct.pack_into('<I',memory,header,0x42504653);memory[header+4]=1;struct.pack_into('<I',memory,header+8,0x100)
    memory[header+0xc]=2;struct.pack_into('<II',memory,header+0x10,0x40,0x80);struct.pack_into('<I',memory,header+0xfc,0x42444e45)
    for at in (header+0x40,header+0x80):memory[at+0x20]=kind;memory[at+0x23]=0x10 if scenario=='skip_size' else 0
    struct.pack_into('<I',memory,0x14200,0 if scenario=='missing' else header)
    start=0 if scenario=='pool_zero' else 0x17ffc if scenario=='pool_low' else 0x18000
    length=0x100 if scenario in ('too_small','skip_size') else 0x20000
    struct.pack_into('<II',memory,0x14288,start,length)
    struct.pack_into('<II',memory,0x1300c,0x12345678,0xabcdef01);memory[0x13014]=0;memory[0x13420]=0x80
    if scenario=='bad_magic':struct.pack_into('<I',memory,header,0)
    if scenario=='bad_version':memory[header+4]=2
    if scenario=='bad_trailer':struct.pack_into('<I',memory,header+0xfc,0)
    if scenario=='zero_count':memory[header+0xc]=0
    native=bytearray(memory);hardware=bytearray(MMIO_SIZE);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[0]=selector;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=int(translation[driver]['dsp_helper'],16);initial.cpsr=flags|0x13
    events=[];nevents=[];machine=cpu(memory,hardware,initial,events);install(machine,unicorn,arm,events,MMIO,MMIO_SIZE)
    machine.emu_start(initial.r[15],STOP,count=100000);expected=state_of(machine);assert expected.r[15]==STOP
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);read,write=native_io(nhardware,nevents)
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,100000)==0 and not errors,errors
    assert bytes(actual)==bytes(expected),(driver,scenario,selector,kind,'CPU')
    assert native==machine.mem_read(0,SIZE) and hardware==machine.mem_read(MMIO,MMIO_SIZE) and not events and not nevents
    if scenario in ('valid','skip_size') and selector<2:
        assert actual.r[0]==0 and native[0x13014]==1
        assert struct.unpack_from('<II',native,0x1300c)==(header+0x40*(selector+1),0x30|(kind<<13))
        assert native[0x13420]==0x80;successes+=1
    elif scenario!='missing':
        assert native[0x13014]==0 and struct.unpack_from('<II',native,0x1300c)==(0x12345678,0xabcdef01)
    cases+=1
report={'cases_passed':cases,'independently_checked_successes':successes,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_controls.c'),'verifier_sha256':sha(Path(__file__)),
 'translation_sha256':sha(PROJECT/'analysis/arm_sound_controls_translation.json'),'header_sha256':sha(PROJECT/'source/arm_sound_memory.h'),
 'adapter_sha256':sha(PROJECT/'arm_legacy_alignment_reference.py'),
 'scope':'Complete DSP configuration helper: original/default headers, malformed signature/version/trailer, program count/bounds, absent/low/short allocation, four memory classes, explicit size-check bypass flag, both fills/flags/drivers. Full CPU/RAM and independently checked configuration words/pointers/enable flags.',
 'limitations':'Constructed DSP descriptors and modeled memory only; no DSP microprogram execution or audio synthesis proof.'}
(PROJECT/'analysis/arm_sound_dsp_config_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'native DSP-configuration comparisons passed;',successes,'independent successful configurations')
