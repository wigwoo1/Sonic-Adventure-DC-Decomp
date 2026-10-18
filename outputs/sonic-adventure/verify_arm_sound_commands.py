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
translation=json.loads((PROJECT/'analysis/arm_sound_commands_translation.json').read_text())['drivers']
def compare(driver,entry,command=0,fill=0,flags=0,active=0,group=1,part=2,note=60,read_offset=None,write_offset=None,queue=None,stop_pc=STOP,expected_rc=0,pitch=None):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    memory=bytearray([fill])*SIZE;memory[:len(image)]=image
    memory[0xe800:0xf400]=bytes(0xc00)
    memory[0x1a000:0x1a200]=bytes(0x200)
    struct.pack_into('<I',memory,0x1a010,0x8000)
    memory[0x1a126]=60;memory[0x1a127]=0xf0 if fill else 0
    if active:
        for slot,g,c,n,mode in ((0,group,part,note,0x80),(1,group,part,note,0xc0),(2,(group+1)&7,part,note,0x80),(3,group,(part+1)&15,note,0x80),(4,group,part,(note+1)&127,0x80)):
            at=0xe800+slot*64;memory[at]=mode;memory[at+3]=0x12
            memory[at+4:at+7]=bytes((g,c,n));memory[at+0xa:at+0xc]=bytes((0x20,0x40))
            struct.pack_into('<III',memory,at+0x10,0x1a100,0x8000,0x1a000)
            struct.pack_into('<I',memory,at+0x38,0x12345678);memory[at+0x3f]=1
    if read_offset is not None:struct.pack_into('<II',memory,0x13000,write_offset,read_offset)
    if queue is not None:
        for i,word in enumerate(queue):struct.pack_into('<I',memory,0xb000+((read_offset+4*i)&0xfff),word)
    before=bytes(memory);native=bytearray(memory)
    hardware=bytearray([fill])*MMIO_SIZE;nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[1]=command;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    if pitch is not None:initial.r[5]=0xe800;initial.r[6]=0x1a100;initial.r[7]=pitch;initial.r[8]=MMIO
    expected_events=[];native_events=[]
    machine=cpu(memory,hardware,initial,expected_events)
    machine.emu_start(entry,stop_pc,timeout=5000000,count=1000000)
    expected=state_of(machine);assert expected.r[15]==stop_pc,(driver,hex(entry),hex(expected.r[15]))
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
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,1000000)
    assert rc==expected_rc and not errors,(driver,hex(entry),hex(command),rc,errors)
    assert bytes(actual)==bytes(expected),(driver,hex(entry),hex(command),'CPU',hex(actual.cpsr),hex(expected.cpsr))
    assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,hex(command),'memory')
    assert native_events==expected_events,(driver,hex(command),'bus')
    return native,nhardware,actual,native_events,before
command_cases=0;reader_cases=0;batch_cases=0;frontier_cases=0;pitch_cases=0
for driver,kind,fill,flags,active,pair,data in itertools.product(range(2),(0,2,5,6,7),(0,0x55),(0,0xf0000000),(0,1),((0,0),(1,2),(7,15)),(0,64,127)):
    group,part=pair;command=(kind<<24)|(group<<20)|(part<<16)|(data<<8)|data|(0xf8000000 if flags else 0)
    memory,hardware,state,events,before=compare(driver,0x9d0,command,fill,flags,active,group,part,data if kind==0 else 60)
    if kind==0 and active and command:
        assert memory[0xe800]==0 and memory[0xe840]==0x40 and memory[0xe900]==0x80
        assert memory[0xe803]==0xff and memory[0xe83f]==0 and struct.unpack_from('<I',memory,0xe838)[0]==0
        assert len(events)==2 and struct.unpack_from('<I',hardware,0)[0]&0x8000
    if kind==6:
        delta=(((command&127)<<7)|((command>>8)&127))-0x2000
        at=0xc400+group*0x480+part*0x48
        assert struct.unpack_from('<I',memory,at+4)[0]==delta&0xffffffff
        if active:
            gain=0x40 if delta<0 else 0x20
            expected_delta=(((gain*delta)&0xffffffff)>>5)&0xffff
            for slot in (0,1,4):assert struct.unpack_from('<I',memory,0xe800+slot*64+0xc)[0]==expected_delta
    command_cases+=1
for driver,fill,flags,offsets,word in itertools.product(range(2),(0,0x55),(0,0xf0000000),((0,0),(0,4),(0xffc,0),(0xffc,0xffc),(0xff8,0xffc),(0,8)),(0,0x12345678,0xffffffff)):
    read,write=offsets;entry=int(translation[driver]['queue_reader'],16)
    memory,hardware,state,events,before=compare(driver,entry,fill=fill,flags=flags,read_offset=read,write_offset=write,queue=[word])
    assert state.r[1]==(0 if read==write else word)
    assert struct.unpack_from('<II',memory,0x13000)==(write,read if read==write else (read+4)&0xfff)
    assert memory[0xb000:0xc000]==before[0xb000:0xc000] and not events
    reader_cases+=1
for driver,fill,flags,active,scenario in itertools.product(range(2),(0,0x55),(0,0xf0000000),(0,1),((0,0),(0xffc,1),(0xff8,4),(0xff4,6),(0,8),(0x100,2))):
    start,count=scenario;types=(2,0,6,5,7)
    words=[0 if i==0 and count>4 else (types[i%5]<<24)|(1<<20)|(2<<16)|(60<<8)|64|(0xf8000000 if flags else 0) for i in range(count)]
    memory,hardware,state,events,before=compare(driver,0x9a4,fill=fill,flags=flags,active=active,read_offset=start,write_offset=(start+count*4)&0xfff,queue=words)
    assert struct.unpack_from('<II',memory,0x13000)==((start+count*4)&0xfff,(start+min(4,count)*4)&0xfff)
    assert memory[0xb000:0xc000]==before[0xb000:0xc000]
    batch_cases+=1
for driver,kind,fill,flags,pair in itertools.product(range(2),(),(0,0x55),(0,0xf0000000),((0,0),(1,2),(7,15))):
    group,part=pair;command=(kind<<24)|(group<<20)|(part<<16)|(64<<8)|64|(0xf8000000 if flags else 0)
    frontier=int(translation[driver]['external_handlers'][(1,3,4).index(kind)],16)
    compare(driver,0x9d0,command,fill,flags,group=group,part=part,stop_pc=frontier,expected_rc=-8)
    frontier_cases+=1
for driver,fill,flags,pitch in itertools.product(range(2),(0,0x55),(0,0xf0000000),(0,0x80,0x100,0x7ff,0xff00,0xffff0000)):
    compare(driver,int(translation[driver]['pitch_helper'],16),fill=fill,flags=flags,active=1,pitch=pitch)
    pitch_cases+=1
report={'command_cases_passed':command_cases,'queue_reader_cases_passed':reader_cases,'batch_cases_passed':batch_cases,'frontier_cases_passed':frontier_cases,'pitch_helper_cases_passed':pitch_cases,
 'dll_sha256':sha(dll),'source_sha256':sha(PROJECT/'source/sa_arm_sound_commands.c'),
 'verifier_sha256':sha(Path(__file__)),'translation_sha256':sha(PROJECT/'analysis/arm_sound_commands_translation.json'),
 'boot_evidence_sha256':sha(PROJECT/'analysis/arm_sound_boot_native_verification.json'),
 'scope':'Native queue reader/intake/dispatch and command types 0/2/5/6/7, full CPU/CPSR/RAM/MMIO/ordered bus equality against original ARM; independent ring consumption, four-command cap, unchanged queue words, voice clearing and pitch delta fields. All command handlers are native; types 1/3/4 have separate comprehensive suites and mixed-stream verification.',
 'limitations':'Constructed queued words and modeled MMIO only. Most other main-loop services remain unrecovered native C; command types are covered by native suites. No audio synthesis, hardware timing or playback proof.'}
(PROJECT/'analysis/arm_sound_commands_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(command_cases,'command,',reader_cases,'reader,',batch_cases,'batch,',frontier_cases,'frontier and',pitch_cases,'pitch comparisons passed')
