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
word_cases=0;store_cases=0;byte_cases=0;corrections_total=0
patterns=((0x89abcdef,(0x89abcdef,0xef89abcd,0xcdef89ab,0xabcdef89)),(0x01234567,(0x01234567,0x67012345,0x45670123,0x23456701)))
for driver,kind,offset,pattern,flags in itertools.product(range(2),('load','store','byte'),range(4),patterns,(0,0xf0000000)):
    if kind!='load' and pattern!=patterns[0]:continue
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray([0xaa])*SIZE;memory[:len(image)]=image
    address=0x16000+offset
    if kind=='load':entry=0x634;literal=0x95c
    elif kind=='store':entry=0x650;literal=0x8f0
    else:entry=0x89c;literal=0x99c;memory[0x8c8]=47
    struct.pack_into('<I',memory,literal,address);struct.pack_into('<I',memory,0x16000,pattern[0])
    before=bytes(memory);native=bytearray(memory);hardware=bytearray(MMIO_SIZE);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    if kind=='load' and offset:
        raw=cpu(memory,hardware,initial,[]);raw.emu_start(entry,STOP,count=100000)
        assert raw.reg_read(arm.UC_ARM_REG_R1)!=pattern[1][offset]
    expected_events=[];native_events=[]
    machine=cpu(memory,hardware,initial,expected_events)
    adapted=install(machine,unicorn,arm,expected_events,MMIO,MMIO_SIZE)
    machine.emu_start(entry,STOP,count=1000000);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    @Read
    def read(user,a,n,v):
        if not MMIO<=a<a+n<=MMIO+MMIO_SIZE:errors.append(('read',hex(a)));return -9
        v[0]=int.from_bytes(nhardware[a-MMIO:a-MMIO+n],'little');native_events.append(('read',a,n,v[0]));return 0
    @Write
    def write(user,a,v,n):
        if not MMIO<=a<a+n<=MMIO+MMIO_SIZE:errors.append(('write',hex(a)));return -9
        v&=(1<<(n*8))-1;nhardware[a-MMIO:a-MMIO+n]=v.to_bytes(n,'little');native_events.append(('write',a,n,v));return 0
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,1000000)==0 and not errors,errors
    assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE)
    assert nhardware==machine.mem_read(MMIO,MMIO_SIZE) and expected_events==native_events
    if kind=='load':assert actual.r[1]==pattern[1][offset];word_cases+=1
    elif kind=='store':
        assert native[0x16000:0x16004]==bytes(4) and native[0x16004:0x16008]==before[0x16004:0x16008]
        store_cases+=1
    else:
        expected_bytes=bytearray(before[0x16000:0x16008]);expected_bytes[offset]=0
        assert native[0x16000:0x16008]==expected_bytes;byte_cases+=1
    assert len(adapted)==(4 if kind=='load' and offset else 1 if kind=='store' and offset else 0)
    corrections_total+=len(adapted)
report={'word_load_cases_passed':word_cases,'word_store_cases_passed':store_cases,'byte_store_cases_passed':byte_cases,'reference_corrections':corrections_total,
 'dll_sha256':sha(dll),'header_sha256':sha(PROJECT/'source/arm_sound_memory.h'),
 'verifier_sha256':sha(Path(__file__)),'adapter_sha256':sha(PROJECT/'arm_legacy_alignment_reference.py'),
 'scope':'All four word offsets, two independent hardcoded rotate tables, aligned physical stores, untouched adjacent bytes and byte-addressed stores; original payload instructions and manual-derived reference corrections. Unadapted Unicorn differs for the tested unaligned word reads.',
 'reference':'ARM Architecture Reference Manual ARM DDI 0100I A2.8, https://e2e.ti.com/cfs-file/__key/communityserver-discussions-components-files/1023/ARM-Architecture.pdf',
 'limitations':'Unicorn is augmented only for unaligned single-word transfers. Word-aligned block-transfer stacks remain a caller contract; physical AICA timing and exception entry are not tested.'}
(PROJECT/'analysis/arm_sound_alignment_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(word_cases,'word-load,',store_cases,'word-store and',byte_cases,'byte-store architecture comparisons passed')
