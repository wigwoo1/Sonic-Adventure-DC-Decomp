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
cases=0;batches=0;corrections_total=0
modes=(('present',0),('present',1),('present',3),('missing',0))
for driver,fill,flags,pair,bank,mode,program,batch in [
    (d,f,c,p,b,m,i,False) for d,f,c,p,b,m,i in itertools.product(range(2),(0,0x55),(0,0xf0000000),((0,0),(1,2),(7,15)),(0,3,15),modes,(0,1,2,127))
]+[(d,f,c,(1,2),3,m,i,True) for d,f,c,m,i in itertools.product(range(2),(0,0x55),(0,0xf0000000),modes,(0,1,2,127))]:
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray([fill])*SIZE;memory[:len(image)]=image
    group,part=pair;context=0xc400+group*0x480+part*0x48
    memory[context]=0x33;memory[context+1]=bank
    struct.pack_into('<III',memory,context+0x10,0x1b000,0x1b080,0x1b100)
    table=struct.unpack_from('<I',image,0x1800 if driver==0 else 0x180c)[0]
    pointer=0x1a000 if mode[0]=='present' else 0
    struct.pack_into('<I',memory,table+bank*8,pointer)
    memory[0x1a000:0x1a200]=bytes(0x200)
    struct.pack_into('<III',memory,0x1a010,0x20,mode[1],0x60)
    struct.pack_into('<III',memory,0x1a020,0x80,0xa0,0xc0)
    command=(4<<24)|(group<<20)|(part<<16)|(program<<8)|32|(0xf8000000 if flags else 0)
    if batch:
        struct.pack_into('<II',memory,0x13000,0,0xffc);struct.pack_into('<I',memory,0xbffc,command)
    before=bytes(memory);native=bytearray(memory);hardware=bytearray(MMIO_SIZE);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[1]=command;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=0x9a4 if batch else 0x9d0;initial.cpsr=flags|0x13
    events=[];machine=cpu(memory,hardware,initial,events)
    adapted=install(machine,unicorn,arm,events,MMIO,MMIO_SIZE)
    machine.emu_start(initial.r[15],STOP,timeout=5000000,count=1000000)
    expected=state_of(machine);assert expected.r[15]==STOP
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native);calls=[]
    @Read
    def read(user,address,size,value):calls.append(('read',address));return -9
    @Write
    def write(user,address,value,size):calls.append(('write',address));return -9
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,1000000)==0,(driver,mode,program)
    assert bytes(actual)==bytes(expected),(driver,mode,program,'CPU')
    assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,mode,program,'memory')
    assert not calls and not events
    assert struct.unpack_from('<I',native,0x13dc if driver==0 else 0x13e8)[0]==0x1b000
    if mode[0]=='present':
        if program<mode[1]:
            assert native[context]==program
            assert struct.unpack_from('<III',native,context+0x10)==(0x1a000,0x1a080+0x20*program,0x1a060)
        elif driver==0:
            assert native[context]==0x33 and struct.unpack_from('<III',native,context+0x10)==(0x1b000,0x1b080,0x1b100)
        else:
            assert native[context]==0xff and struct.unpack_from('<III',native,context+0x10)==(0xffffffff,0x1b080,0x1b100)
    elif driver==0:
        fallback=struct.unpack_from('<I',image,0x1804)[0]
        assert struct.unpack_from('<I',native,context+0x10)[0]==(fallback if program<2 else 0x1b000)
    else:
        assert native[context]==program and struct.unpack_from('<I',native,context+0x10)[0]==0xffffffff
    assert len(adapted)==(4 if driver==1 and mode[0]=='missing' else 0)
    corrections_total+=len(adapted)
    if batch:
        assert struct.unpack_from('<II',native,0x13000)==(0,0)
        assert native[0xb000:0xc000]==before[0xb000:0xc000];batches+=1
    else:cases+=1
report={'cases_passed':cases,'native_batch_cases_passed':batches,'reference_alignment_corrections':corrections_total,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_bank_select.c'),
 'header_sha256':sha(PROJECT/'source/arm_sound_memory.h'),'verifier_sha256':sha(Path(__file__)),
 'adapter_sha256':sha(PROJECT/'arm_legacy_alignment_reference.py'),
 'translation_sha256':sha(PROJECT/'analysis/arm_sound_bank_select_translation.json'),
 'alignment_evidence_sha256':sha(PROJECT/'analysis/arm_sound_alignment_verification.json'),
 'scope':'Complete type-4 bank/program selection and native queued invocation: valid/missing banks, zero/one/three programs, accepted/equal/out-of-range indices, driver-specific failure behavior, full active CPU/CPSR/RAM and preserved queue comparison. No external ARM service callbacks.',
 'limitations':'Unicorn reference is explicitly corrected for documented pre-ARMv6 unaligned word accesses; missing MASARU bank path retains its original -1 pointer and wrapped low-RAM reads. No hardware timing, synthesis or playback proof.'}
(PROJECT/'analysis/arm_sound_bank_select_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'bank/program and',batches,'native batch comparisons passed;',corrections_total,'architectural reference corrections')
