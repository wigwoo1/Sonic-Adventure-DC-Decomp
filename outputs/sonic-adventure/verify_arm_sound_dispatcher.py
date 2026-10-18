"""Dispatcher baseline command coverage and independent new-instruction checks."""
import re
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if '--connected' in sys.argv else 'work/sonic-adventure/build/arm_sound_dispatcher_native.dll')
fn=ctypes.CDLL(str(dll)).sa_arm_sound_boot_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,Service,U,U];fn.restype=ctypes.c_int
translation=json.loads((P/'analysis/arm_sound_dispatcher_translation.json').read_text())
bootstraps={};cases=[];instruction_cases=[];wait_cases=[]
def bootstrap(driver):
    if driver not in bootstraps:
        image=(ROOT/payloads[driver]['payload_path']).read_bytes();ram=bytearray(SIZE);ram[:len(image)]=image
        hardware=bytearray(MMIO_SIZE);state=State();state.cpsr=0x13
        machine=cpu(ram,hardware,state,[]);machine.emu_start(0,0x194,count=2000000)
        assert machine.reg_read(arm.UC_ARM_REG_PC)==0x194,'original reset fixture failed'
        bootstraps[driver]=(bytes(machine.mem_read(0,SIZE)),bytes(machine.mem_read(MMIO,MMIO_SIZE)))
    return bootstraps[driver]
def run(driver,flags,opcode=None,instruction=None,value=0,hard_wait=False):
    prepared,hw=bootstrap(driver);original=bytearray(prepared);hardware=bytearray(hw)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.cpsr=flags|0x13
    budget=1000000;entry=0x48e8+driver*8
    if opcode is not None:
        source=struct.unpack_from('<I',original,0x4b2c+driver*8)[0]
        original[source:source+512]=bytes(512);original[source]=opcode
        if hard_wait:original[source+4:source+8]=b'SEGA'
    if instruction:
        entry=int(instruction['address'],16);asm=instruction['instruction'];budget=1
        initial.r[1]=value;initial.r[6]=value;initial.r[7]=value
        initial.r[10]=initial.r[12]=0x1a100
        if asm=='rsbs r3,r1,r7':initial.r[7]=value^0xffffffff
        struct.pack_into('<I',original,0x1a104,0x89abcdef);original[0x1a100-31]=0xa7
        if asm.startswith('msr '):initial.r[10]=((flags^0xf0000000)&0xf0000000)|0x13|(0xc0 if value&1 else 0)
        if asm=='ldr r3,[r3,r1,lsl #0x2]':
            initial.r[3]=0x1a200;initial.r[1]=value&3
            for i in range(4):struct.pack_into('<I',original,0x1a200+i*4,0x12340000+i)
    initial.r[15]=entry
    native=bytearray(original);nhardware=bytearray(hardware);actual=State.from_buffer_copy(initial)
    expected_events=[];native_events=[];machine=cpu(original,hardware,initial,expected_events)
    machine.emu_start(entry,STOP,count=budget);expected=state_of(machine)
    @Read
    def read(user,address,size,out):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('read',hex(address)));return -9
        out[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little');native_events.append(('read',address,size,out[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('write',hex(address)));return -9
        value&=(1<<(size*8))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little');native_events.append(('write',address,size,value));return 0
    view=(ctypes.c_uint8*SIZE).from_buffer(native)
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,budget)
    completed=expected.r[15]==STOP
    assert rc==(0 if completed else -3) and not errors,(driver,opcode,instruction,rc,hex(actual.r[15]),hex(expected.r[15]),errors)
    assert bytes(actual)==bytes(expected),(driver,opcode,instruction,'CPU',list(actual.r),list(expected.r),hex(actual.cpsr),hex(expected.cpsr))
    assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,opcode,instruction,'memory')
    assert native_events==expected_events,(driver,opcode,'bus effects')
    if instruction:
        if asm.startswith('mrs '):assert actual.r[10]==initial.cpsr
        elif asm.startswith('msr '):assert actual.cpsr==(initial.cpsr&0x00ffff00)|(initial.r[10]&0xff0000ff)
        elif asm=='mov r7,r7, asr #0x3':
            signed=value if value<0x80000000 else value-0x100000000
            assert actual.r[7]==(signed>>3)&0xffffffff and actual.cpsr==initial.cpsr
        elif asm=='rsbs r3,r1,r7':
            result=(initial.r[7]-initial.r[1])&0xffffffff
            wanted=(flags&0x0fffffff)|(result&0x80000000)|(0x40000000 if not result else 0)|(0x20000000 if initial.r[7]>=initial.r[1] else 0)
            if ((initial.r[7]^initial.r[1])&(initial.r[7]^result)&0x80000000):wanted|=0x10000000
            assert actual.r[3]==result and actual.cpsr==wanted|0x13
        elif asm.startswith('rsb '):assert actual.r[6]==(256-value)&0xffffffff and actual.cpsr==initial.cpsr
        elif ']!' in asm:
            if asm.startswith('ldr '):
                target=int(asm.split(',')[0].split('r')[2]);assert actual.r[target]==0x89abcdef and actual.r[12]==0x1a104
            else:assert actual.r[10]==0x1a0f4 and struct.unpack_from('<I',native,0x1a0f4)[0]==value
        elif asm=='str r1,[r10,#-0xc]':assert actual.r[10]==initial.r[10] and struct.unpack_from('<I',native,0x1a0f4)[0]==value
        elif asm=='ldrbne r7,[r12,#-0x1f]':assert actual.r[7]==(value if flags&0x40000000 else 0xa7)
        elif asm=='ldr r3,[r3,r1,lsl #0x2]':assert actual.r[3]==0x12340000+(value&3)
        else:raise AssertionError(asm)
    if opcode is not None and not hard_wait:assert completed,('baseline command did not finish',driver,opcode,hex(expected.r[15]))
    if hard_wait:assert not completed and expected.r[15] in {a+driver*8 for a in (0x60dc,0x60e0,0x60e4,0x60e8,0x60ec)},'reset wait loop not reached'
    return {'driver':driver,'flags':hex(flags),'opcode':opcode,'instruction':instruction['instruction'] if instruction else None,'value':value,'completed':completed,'wait':hard_wait,'mmio_events':len(native_events)}
special=lambda asm:asm.startswith(('mrs ','msr ','rsbs ')) or asm in ('mov r7,r7, asr #0x3','rsb r6,r6,#0x100','ldr r3,[r3,r1,lsl #0x2]','ldrbne r7,[r12,#-0x1f]','str r1,[r10,#-0xc]') or ']!' in asm
for driver,rows in enumerate(translation['drivers']):
    selected=[r for r in rows if special(r['instruction'])];assert len(selected)==15,(driver,len(selected))
    for row,flag_bits,value in itertools.product(selected,range(16),(0,1,0x7fffffff,0x80000000,0xffffffff)):
        instruction_cases.append(run(driver,flag_bits<<28,instruction=row,value=value))
for driver,flags,opcode in itertools.product(range(2),(0,0xf0000000),range(256)):cases.append(run(driver,flags,opcode))
for driver in range(2):wait_cases.append(run(driver,0,0x8f,hard_wait=True))
builder=P/'build_arm_sound_dispatcher.ps1';names=re.findall(r"'(sa_arm_sound[^']+\.c)'",builder.read_text())
paths=[Path(__file__),support,dll,builder,P/'create_arm_sound_dispatcher.py',P/'create_arm_sound_controls.py',P/'source/sa_arm_sound_boot_native.c',P/'source/arm_sound_memory.h',P/'analysis/arm_sound_dispatcher_translation.json']+[P/'source'/name for name in names]
paths += [P/'source/sa_arm_sound_boot_native.c',P/'build_arm_sound_boot_native.ps1'] if '--connected' in sys.argv else []
report={'instruction_cases_passed':len(instruction_cases),'baseline_command_cases_passed':len(cases),'reset_wait_cases_passed':len(wait_cases),'instruction_cases':instruction_cases,'cases':cases,'wait_cases':wait_cases,
 'sha256':{str(path):sha(path) for path in paths},
 'connected':'--connected' in sys.argv,
 'scope':'Separate dispatcher overlay DLL reuses existing native driver helpers; null ARM callbacks. Single-instruction addressing/status/arithmetic proof and all 256 zero-argument command baselines from original reset state. Full active CPU/CPSR/two MiB RAM/modeled MMIO/ordered effects equal original ARM. Positive bank/active-command paths, varied arguments and main-driver integration remain unverified; no dispatcher/game completion claim.'}
(P/('analysis/arm_sound_dispatcher_baseline_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_dispatcher_baseline_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(instruction_cases),'instruction checks,',len(cases),'zero-argument command baselines and',len(wait_cases),'explicit reset waits passed')
