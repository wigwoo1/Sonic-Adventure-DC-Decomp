"""Original banked exception returns, timer paths and scripted input FIQ calls."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
class Banks(ctypes.Structure):_fields_=[('spsr',U),('svc_r8_r14',U*7),('exception_r8_r14',U*7),('returned',U)]
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if '--connected' in sys.argv else 'work/sonic-adventure/build/arm_sound_exception_native.dll')
fn=ctypes.CDLL(str(dll)).sa_arm_sound_exception_native
fn.argtypes=[BP,ctypes.POINTER(State),ctypes.POINTER(Banks),U,ctypes.c_void_p,Read,Write,U,U];fn.restype=ctypes.c_int
cases=[];budgets=[];faults=[];coverage=[set(),set()]
def compare(driver,cause,flags,countdown,phase,entry=0x1c,mode=0x11,sequence=(),budget=10000,fault=None,counter=0xfffffffe,resume=False):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray(SIZE);memory[:len(image)]=image
    def put(a,v,n=4):memory[a:a+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
    def word(a):return struct.unpack_from('<I',memory,a)[0]
    cause_port=word(0x900);ack_port=word(0x918);port=word(0x1814+driver*8)
    put(word(0x8fc),counter);put(word(0x910),counter);put(word(0x914),counter)
    put(0x320,countdown);put(word(0x90c),phase,1);put(0x22c,0x55,1);put(0x22e,0xaa,1);put(0xfc,0x12345678)
    put(0x13000,0xffc);put(0x15f4+driver*12,0)
    hardware=bytearray(MMIO_SIZE);struct.pack_into('<I',hardware,cause_port-MMIO,0xdeadbe00|cause)
    native=bytearray(memory);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)];initial.r[13]=0x1fa000;initial.r[14]=STOP+4;initial.r[15]=entry;initial.cpsr=0xd0|mode|(flags&0xf0000000)
    banks=Banks();banks.spsr=flags|0x13;banks.svc_r8_r14[:]=[0x87650000+i for i in range(7)];banks.svc_r8_r14[5]=0x1fe000
    banks.exception_r8_r14[:]=[0x77770000+i for i in range(7)]
    svc_seed=tuple(banks.svc_r8_r14)
    expected_banks=Banks.from_buffer_copy(banks);actual=State.from_buffer_copy(initial)
    expected_events=[];native_events=[];ri=[0];ni=[0];failed=[False];read_count=[0];write_count=[0];native_counts={'read':0,'write':0}
    sequence=list(sequence)+[0x100]
    machine=unicorn.Uc(unicorn.UC_ARCH_ARM,unicorn.UC_MODE_ARM);machine.ctl_set_cpu_model(arm.UC_CPU_ARM_TI925T)
    machine.mem_map(0,SIZE);machine.mem_write(0,bytes(memory));machine.mem_map(MMIO,MMIO_SIZE);machine.mem_write(MMIO,bytes(hardware))
    machine.reg_write(arm.UC_ARM_REG_CPSR,banks.spsr)
    for reg,value in zip(registers[8:15],banks.svc_r8_r14):machine.reg_write(reg,value)
    machine.reg_write(arm.UC_ARM_REG_CPSR,initial.cpsr)
    for reg,value in zip(registers,initial.r):machine.reg_write(reg,value)
    machine.reg_write(arm.UC_ARM_REG_SPSR,banks.spsr)
    bus_sites={0x26c:('read',8),0x2b0:('write',8),0x2bc:('write',8),0x354:('write',8),0x358:('write',8),0x35c:('write',8),0x360:('write',8),0x159c+driver*12:('read',0)}
    def trace(uc,address,size,user):
        coverage[driver].add(address)
        if fault and address in bus_sites:
            direction,index=bus_sites[address];count=read_count[0] if direction=='read' else write_count[0]
            if direction==fault[0] and count+1==fault[1]:
                expected_events.append(('fault',direction,uc.reg_read(registers[index]),4));failed[0]=True;uc.emu_stop()
    def bus(uc,access,address,size,value,user):
        if access==unicorn.UC_MEM_READ:
            if address==port:value=sequence[min(ri[0],len(sequence)-1)];ri[0]+=1;uc.mem_write(port,value.to_bytes(4,'little'))
            else:value=int.from_bytes(uc.mem_read(address,size),'little')
            expected_events.append(('read',address,size,value));read_count[0]+=1
        else:expected_events.append(('write',address,size,value));write_count[0]+=1
    machine.hook_add(unicorn.UC_HOOK_CODE,trace)
    machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,bus,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    @Read
    def read(user,address,size,out):
        native_counts['read']+=1
        if fault==('read',native_counts['read']):native_events.append(('fault','read',address,size));return -9
        if address==port:
            value=sequence[min(ni[0],len(sequence)-1)];ni[0]+=1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        else:value=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        out[0]=value;native_events.append(('read',address,size,value));return 0
    @Write
    def write(user,address,value,size):
        native_counts['write']+=1
        if fault==('write',native_counts['write']):native_events.append(('fault','write',address,size));return -9
        nhardware[address-MMIO:address-MMIO+size]=(value&((1<<(8*size))-1)).to_bytes(size,'little');native_events.append(('write',address,size,value));return 0
    view=(ctypes.c_uint8*SIZE).from_buffer(native)
    checkpoints=0
    while True:
        machine.emu_start(machine.reg_read(arm.UC_ARM_REG_PC),STOP,count=budget);expected=state_of(machine);completed=expected.r[15]==STOP
        if completed:
            expected_banks.returned=1;expected_banks.svc_r8_r14[:]=expected.r[8:15]
            saved=expected.cpsr;machine.reg_write(arm.UC_ARM_REG_CPSR,(saved&~31)|mode)
            expected_banks.exception_r8_r14[:]=[machine.reg_read(reg) for reg in registers[8:15]]
            machine.reg_write(arm.UC_ARM_REG_CPSR,saved)
        rc=fn(view,ctypes.byref(actual),ctypes.byref(banks),driver,None,read,write,STOP,budget)
        assert rc==(0 if completed else -9 if failed[0] else -3),(driver,cause,mode,entry,budget,rc,hex(expected.r[15]))
        assert bytes(actual)==bytes(expected),(driver,cause,mode,entry,budget,'CPU',list(actual.r),list(expected.r),hex(actual.cpsr),hex(expected.cpsr))
        assert bytes(banks)==bytes(expected_banks),(driver,cause,mode,entry,budget,'banks',list(banks.exception_r8_r14),list(expected_banks.exception_r8_r14))
        assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),'RAM/MMIO'
        assert native_events==expected_events,'ordered bus effects'
        checkpoints+=1
        if not resume or completed or failed[0]:break
        assert checkpoints<1024,'resume failed to converge'
    if completed:
        out=lambda a:struct.unpack_from('<I',native,a)[0]
        assert actual.cpsr==flags|0x13,'SPSR restored'
        if mode==0x11:assert list(actual.r)==list(initial.r[:8])+list(svc_seed)+[STOP],'FIQ preserves interrupted general registers'
        if entry in (0x1c,0x248):
            assert out(word(0x8fc))==(counter+1)&0xffffffff,'interrupt count wrap'
            assert native_events[-4:]==[('write',ack_port,4,1)]*4,'four unmerged acknowledgement writes'
            if cause==2:
                expected_count=(countdown-1)&0xffffffff
                assert out(0x320)==(word(0x324) if expected_count==0 else expected_count),'timer countdown/reload'
                rollover=expected_count!=0 and ((phase+1)&3)==0
                assert out(word(0x910))==(counter+int(rollover))&0xffffffff,'quarter-rate tick'
                assert native[0x22e]==(0 if rollover else 255),'fast/tick marker'
                assert native[0x22c]==(255 if rollover else 0x55),'main tick flag'
            elif cause==5:
                assert out(word(0x914))==(counter+1)&0xffffffff and out(0xfc)==0xffffffff,'input interrupt marker/count'
                # Chosen fixture is two same-status three-byte commands.
                if list(sequence[:5])==[0x90,60,100,61,101]:
                    assert out(0x13000)==4 and out(0xbffc)==0xf9703c64 and out(0xb000)==0xf9703d65,'input parser ring production'
    return {'driver':driver,'cause':cause,'flags':flags,'countdown':countdown,'phase':phase,'entry':entry,'mode':mode,'budget':budget,'fault':fault,'completed':completed,'resume':resume,'checkpoints':checkpoints,'events':len(native_events),'reads':native_counts['read'],'writes':native_counts['write']}
for driver,cause,flags,countdown,phase in itertools.product(range(2),range(8),(0,0x200000c0,0x90000040,0xf0000080),(0,1,2,0xffffffff),(0,1,2,3)):
    cases.append(compare(driver,cause,flags,countdown,phase,sequence=[0x90,60,100,61,101]))
for driver,flags,pair in itertools.product(range(2),(0,0xf00000c0),((4,0x1b),(8,0x13),(12,0x17),(16,0x17),(20,0x17),(24,0x12),(0x234,0x11))):cases.append(compare(driver,0,flags,1,0,entry=pair[0],mode=pair[1]))
for driver,cause,budget in itertools.product(range(2),(0,2,5),(1,2,4,8,16,32,64,128,256)):budgets.append(compare(driver,cause,0xf00000c0,2,3,budget=budget,sequence=[0x90,60,100,61,101]))
for driver,cause in itertools.product(range(2),(0,2,5)):
    probe=compare(driver,cause,0xf00000c0,2,3,sequence=[0x90,60,100,61,101])
    for direction in ('read','write'):
        for ordinal in sorted({1,probe['reads' if direction=='read' else 'writes']}):faults.append(compare(driver,cause,0xf00000c0,2,3,sequence=[0x90,60,100,61,101],fault=(direction,ordinal)))
resume_cases=[]
for driver,cause,budget in itertools.product(range(2),(0,2,5),(1,7,31)):
    resume_cases.append(compare(driver,cause,0xf00000c0,2,3,budget=budget,sequence=[0x90,60,100,61,101],resume=True))
ram=bytearray(SIZE);view=(ctypes.c_uint8*SIZE).from_buffer(ram);guard_events=[];atomic_guards=0
@Read
def guard_read(user,address,size,out):guard_events.append(address);return -9
@Write
def guard_write(user,address,value,size):guard_events.append(address);return -9
def guard(index=None,replacement=None,mode=0xd1,spsr=0x13,returned=0):
    global atomic_guards
    state=State();state.cpsr=mode;state.r[15]=0x1c
    banks=Banks();banks.spsr=spsr;banks.returned=returned
    args=[view,ctypes.byref(state),ctypes.byref(banks),0,None,guard_read,guard_write,STOP,100]
    if index is not None:args[index]=replacement
    saved=(bytes(state),bytes(banks),bytes(ram))
    assert fn(*args)==-2 and saved==(bytes(state),bytes(banks),bytes(ram)) and not guard_events
    atomic_guards+=1
for index,replacement in ((0,None),(1,None),(2,None),(3,2),(5,Read()),(6,Write())):guard(index,replacement)
for mode in (0x10,0x1f,0x31,0x01000011):guard(mode=mode)
for spsr in (0x10,0x33,0x01000013):guard(spsr=spsr)
guard(returned=1)
translation=json.loads((P/'analysis/arm_sound_exception_translation.json').read_text())
missing=[[r for r in rows if int(r['address'],16) not in coverage[driver]] for driver,rows in enumerate(translation['drivers'])]
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_exception.c',P/'source/sa_arm_sound_exception_fixture.c',P/'source/arm_sound_exception.h',P/'source/sa_arm_sound_input.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_exception.py',P/'create_arm_sound_controls.py',P/'build_arm_sound_exception.ps1',P/'analysis/arm_sound_exception_translation.json']
if '--connected' in sys.argv:
    import re
    paths += [P/'build_arm_sound_boot_native.ps1']+[P/'source'/n for n in re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_boot_native.ps1').read_text())]
report={'connected':'--connected' in sys.argv,'cases_passed':len(cases),'resume_cases_passed':len(resume_cases),'resume_checkpoints_passed':sum(c['checkpoints'] for c in resume_cases),'resume_cases':resume_cases,'atomic_guards_passed':atomic_guards,'budget_cases_passed':len(budgets),'mmio_fault_cases_passed':len(faults),'cases':cases,'budgets':budgets,'faults':faults,'uncovered_rows':missing,'sha256':{str(p):sha(p) for p in paths},'scope':'Selected-library original vectors/generic exception and fast interrupt handler, banked exception-to-SVC/ARM return, scripted input-parser calls. Exact active CPU/CPSR, saved SVC/exception bank snapshots, RAM/MMIO/ordered bus versus original TI925T. Caller supplies already-entered exception context. Finite timer/input/return assertions; no automatic interrupt injection, nested exceptions, physical timing/device scheduling, complete host event scheduling, synthesis or full-game proof.','architecture_reference':'ARM DDI0100I A4.1.22 LDM (3), A2 exception/register-bank model: https://e2e.ti.com/cfs-file/__key/communityserver-discussions-components-files/1023/ARM-Architecture.pdf'}
(P/('analysis/arm_sound_exception_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_exception_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'exception/FIQ cases,',len(resume_cases),'resume flows,',len(budgets),'budgets,',len(faults),'MMIO faults passed; uncovered sites',*[len(s) for s in missing])
