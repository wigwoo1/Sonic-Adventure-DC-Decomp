"""Original ARM and independent byte-parser/ring checks with scripted MMIO input."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if '--connected' in sys.argv else 'work/sonic-adventure/build/arm_sound_input_native.dll')
fn=ctypes.CDLL(str(dll)).sa_arm_sound_input_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,U,U];fn.restype=ctypes.c_int
cases=[];budgets=[];faults=[];coverage=[set(),set()]
def oracle(sequence):
    status=None;first=None;commands=[]
    for word in sequence:
        if word&0x100:break
        value=word&255
        if value&128:
            status=value if value>>4!=15 else None;first=None
        elif status is not None:
            if status>>4 in (12,13):data1=value;data2=0
            elif first is None:first=value;continue
            else:data1=first;data2=value;first=None
            commands.append(((0xf8|((status>>4)&7))<<24)|((0x70|(status&15))<<16)|(data1<<8)|data2)
    state=0 if status is None else 0x1680 if status>>4 in (12,13) else 0x163c if first is None else 0x164c
    return commands,state,status,first

def compare(driver,flags,sequence,position=0,fill=0,budget=10000,fault=None,seed=None,history=(),entry_override=None,capture=False,direct=None):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();memory=bytearray([fill])*SIZE;memory[:len(image)]=image
    shift=driver*12;entry=0x1598+shift;port=struct.unpack_from('<I',memory,0x1814+driver*8)[0]
    if seed is None:
        struct.pack_into('<I',memory,0x13000,position);struct.pack_into('<I',memory,0x15f4+shift,0)
    else:memory=bytearray(seed[0])
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)];initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    if seed is not None:initial=State.from_buffer_copy(seed[1]);initial.r[15]=entry
    if entry_override is not None:entry=entry_override;initial.r[15]=entry
    if direct is not None:initial.r[0],initial.r[1],initial.r[2]=direct
    sequence=list(sequence)+[0x100];ri=[0];ni=[0];expected_events=[];native_events=[];faulted=[False]
    machine=unicorn.Uc(unicorn.UC_ARCH_ARM,unicorn.UC_MODE_ARM);machine.ctl_set_cpu_model(arm.UC_CPU_ARM_TI925T)
    machine.mem_map(0,SIZE);machine.mem_write(0,bytes(memory));machine.mem_map(MMIO,MMIO_SIZE)
    machine.reg_write(arm.UC_ARM_REG_CPSR,initial.cpsr)
    for reg,value in zip(registers,initial.r):machine.reg_write(reg,value)
    def trace(uc,address,size,user):
        coverage[driver].add(address)
        if fault and address==0x159c+shift and ri[0]+1==fault:
            expected_events.append(('fault',port,4));faulted[0]=True;uc.emu_stop()
    def bus(uc,access,address,size,value,user):
        assert access==unicorn.UC_MEM_READ and address==port and size==4
        value=sequence[min(ri[0],len(sequence)-1)];ri[0]+=1
        uc.mem_write(port,value.to_bytes(4,'little'));expected_events.append(('read',port,4,value))
    machine.hook_add(unicorn.UC_HOOK_CODE,trace)
    machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,bus,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(entry,STOP,count=budget);expected=state_of(machine)
    native=bytearray(memory);actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    @Read
    def read(user,address,size,out):
        assert address==port and size==4
        if fault and ni[0]+1==fault:native_events.append(('fault',port,4));return -9
        value=sequence[min(ni[0],len(sequence)-1)];ni[0]+=1;out[0]=value;native_events.append(('read',port,4,value));return 0
    @Write
    def write(user,address,value,size):errors.append(('unexpected MMIO write',address));return -9
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,STOP,budget)
    completed=expected.r[15]==STOP
    assert rc==(0 if completed else -9 if faulted[0] else -3),(driver,sequence,rc,hex(expected.r[15]))
    assert bytes(actual)==bytes(expected) and native==machine.mem_read(0,SIZE),(driver,sequence,'CPU/RAM')
    assert expected_events==native_events and not errors,(driver,sequence,'bus')
    if completed:
        commands,state,status,first=oracle(list(history)+sequence)
        if direct is not None:
            header,data1,data2=direct
            commands=[((0xf8|((header>>4)&7))<<24)|((0x70|(header&15))<<16)|((data1&255)<<8)|(data2&255)]
        assert struct.unpack_from('<I',native,0x13000)[0]==(position+4*len(commands))&0xfff,'ring write position'
        for i,command in enumerate(commands):assert struct.unpack_from('<I',native,0xb000+((position+4*i)&0xfff))[0]==command,'independent packed command'
        assert struct.unpack_from('<I',native,0x15f4+shift)[0]==(state+shift if state else 0),'running-status continuation'
        if status is not None:assert native[0x15fc+shift]==status,'saved status'
        if first is not None:assert native[0x15fd+shift]==first,'saved first data byte'
    result={'driver':driver,'flags':flags,'sequence':sequence,'position':position,'fill':fill,'budget':budget,'fault':fault,'reads':ni[0],'completed':completed,'direct':direct}
    return (result,(bytes(native),bytes(actual))) if capture else result
for driver,flags,position,status in itertools.product(range(2),(0,0xf0000000),(0,0xffc),range(128,256)):
    cases.append(compare(driver,flags,[status,0,127,1,64,status,126,2],position,0x55 if flags else 0))
for driver,value in itertools.product(range(2),range(128)):cases.append(compare(driver,0,[value]))
sequences=([0x90,60,100,61,101],[0xc7,0,127,64],[0x90,60,0xd2,12,0xf0,40,0x81,13,14],[0x80000090,0xffff003c,0x7f],[0x90,60,0x100,100])
for driver,flags,sequence in itertools.product(range(2),(0,0xf0000000),sequences):cases.append(compare(driver,flags,sequence,0xffc))
for driver,budget in itertools.product(range(2),(1,2,4,8,16,32,64,128)):budgets.append(compare(driver,0xf0000000,[0x90,60,100,61,101],0xffc,budget=budget))
for driver,ordinal in itertools.product(range(2),range(1,7)):faults.append(compare(driver,0xf0000000,[0x90,60,100,61,101],0xffc,fault=ordinal))
for driver,flags in itertools.product(range(2),(0,0xf0000000)):cases.append(compare(driver,flags,[],entry_override=0x1768+driver*12))
direct_cases=[]
for driver,flags,header,data in itertools.product(range(2),(0,0xf0000000),(0,0x80,0x90,0xc0,0xff,0xffffffff),(0,127,255,0xffffffff)):
    direct_cases.append(compare(driver,flags,[],0xffc,entry_override=0x16a0+driver*12,direct=(header,data,data^0xffffffff)))
flows=[]
chunks=([0x90],[60],[100,61],[101,0xc7],[127,64,0xf0],[40,0x81,13],[14])
for driver,flags,position in itertools.product(range(2),(0,0xf0000000),(0,0xffc)):
    seed=None;history=[];steps=[]
    for chunk in chunks:
        result,seed=compare(driver,flags,chunk,position,seed=seed,history=history,capture=True)
        history+=chunk;steps.append(result)
    flows.append({'driver':driver,'flags':flags,'position':position,'steps':steps})
atomic_guards=0
ram=bytearray(SIZE);view=(ctypes.c_uint8*SIZE).from_buffer(ram)
state=State();state.r[15]=0x1598;state.cpsr=0x13
@Read
def guard_read(user,address,size,out):raise AssertionError('atomic guard reached MMIO')
@Write
def guard_write(user,address,value,size):raise AssertionError('atomic guard reached MMIO')
for index,replacement in ((0,None),(1,None),(2,2),(4,Read()),(5,Write())):
    args=[view,ctypes.byref(state),0,None,guard_read,guard_write,STOP,100];args[index]=replacement
    saved=bytes(state);assert fn(*args)==-2 and bytes(state)==saved and ram==bytearray(SIZE);atomic_guards+=1
for mode in (0x10,0x12,0x30,0x33):
    state.cpsr=mode;saved=bytes(state)
    assert fn(view,ctypes.byref(state),0,None,guard_read,guard_write,STOP,100)==-2 and bytes(state)==saved;atomic_guards+=1
translation=json.loads((P/'analysis/arm_sound_input_translation.json').read_text())
missing=[]
for driver,rows in enumerate(translation['drivers']):missing.append([r for r in rows if int(r['address'],16) not in coverage[driver]])
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_input.c',P/'source/sa_arm_sound_input_fixture.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_input.py',P/'create_arm_sound_controls.py',P/'build_arm_sound_input.ps1',P/'analysis/arm_sound_input_translation.json']
if '--connected' in sys.argv:
    import re
    paths += [P/'build_arm_sound_boot_native.ps1']+[P/'source'/n for n in re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_boot_native.ps1').read_text())]
report={'connected':'--connected' in sys.argv,'cases_passed':len(cases),'producer_cases_passed':len(direct_cases),'producer_cases':direct_cases,'persistent_flows_passed':len(flows),'persistent_checkpoints_passed':sum(len(f['steps']) for f in flows),'flows':flows,'atomic_guards_passed':atomic_guards,'budget_cases_passed':len(budgets),'mmio_fault_cases_passed':len(faults),'cases':cases,'budgets':budgets,'faults':faults,'covered_sites':[len(s) for s in coverage],'uncovered_rows':missing,'sha256':{str(p):sha(p) for p in paths},'scope':'Selected-library static native input parser and ring producer; original TI925T instruction reference and independent status/packed-command/ring/continuation assertions. Scripted MMIO words at original input port, not physical device timing or real input delivery. Exceptions/interrupts, complete host event scheduling and audible synthesis remain outside proof.'}
(P/('analysis/arm_sound_input_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_input_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'parser cases,',len(direct_cases),'producer cases,',len(flows),'persistent flows,',len(budgets),'budgets,',len(faults),'MMIO faults passed; uncovered sites',*[len(x) for x in missing])
