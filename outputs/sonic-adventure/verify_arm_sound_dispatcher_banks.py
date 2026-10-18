"""Successful constructed SMSB sequence banks, priority replacement and cleanup."""
from pathlib import Path
P=Path(__file__).resolve().parent
baseline=P/'verify_arm_sound_dispatcher.py'
exec(compile(baseline.read_text().split('special=lambda')[0],str(baseline),'exec'))
from arm_legacy_alignment_reference import install
bank_cases=[];guard_cases=[];budget_cases=[]
def compare(driver,flags,part,bank_index,program,kind,budget=100000,fault=None):
    prepared,hw=bootstrap(driver);original=bytearray(prepared);hardware=bytearray(hw)
    def put(a,v,n=4):original[a:a+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
    def word(a):return struct.unpack_from('<I',original,a)[0]
    source=word(0x4b2c+driver*8);original[source:source+512]=bytes(512)
    bank=0x1c000;size=0x200;original[bank:bank+size]=bytes(size)
    original[bank:bank+4]=b'SMSB';put(bank+4,1);put(bank+8,size);put(bank+12,3)
    offsets=(0x40,0x80,0xc0);clocks=(16,32,48);periods=(16,32,64)
    for i,offset in enumerate(offsets):
        put(bank+16+i*4,offset);original[bank+offset:bank+offset+4]=b'SMSD'
        put(bank+offset+4,clocks[i]);put(bank+offset+8,periods[i])
        original[bank+offset+12:bank+offset+18]=bytes((0,60+i,64,1,4,0x83))
    original[bank+size-4:bank+size]=b'ENDB';put(bank+28,0x44534d53)
    put(0x14000+bank_index*8,bank);put(0x14004+bank_index*8,size)
    priority={'start':64,'replace':64,'force':0,'lower':8,'higher':128,'zero_old':8,'masked_equal':71,'masked_force':7}.get(kind,64)
    active=kind in ('replace','force','lower','higher','zero_old','masked_equal','masked_force')
    group=0x10000+part*64;channel=0xc000+part*128;other=(part+1)&7
    pending=word(0x48cc+driver*8)
    command=0x01000000|(part<<20)|(60<<8)|64;other_command=0x01000000|(other<<20)|(61<<8)|64
    if active:
        put(group,0 if kind=='zero_old' else 64,1);put(group+1,1,1)
        for offset,value in ((4,0x1e040),(8,16),(0xc,16),(0x10,0x1e00c),(0x14,0x1e000),(0x24,17),(0x34,16),(0x38,2)):
            put(group+offset,value)
        put(channel+0x28,1,1)
        struct.pack_into('<IIIIII',original,pending,command,1,16,other_command,2,16)
        put(0x13004,0);put(0x13000,8);put(0xb000,command);put(0xb004,other_command)
        for i in range(48):
            put(0xe800+i*64,0x81,1);put(0xe800+i*64+4,part if i%2==0 else other,1)
            struct.pack_into('<I',hardware,i*128,0x4123+i)
    if kind=='bad_magic':original[bank:bank+4]=b'FAIL'
    if kind=='bad_version':put(bank+4,2)
    if kind=='bad_trailer':original[bank+size-4:bank+size]=b'FAIL'
    if kind=='bad_count':put(bank+12,0)
    requested_program=3 if kind=='bad_program' else program
    original[source:source+6]=bytes((1,0,part,bank_index,requested_program,priority))
    native=bytearray(original);nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    entry=0x48e8+driver*8;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    expected_events=[];native_events=[];snapshots=[];reference_count=[0];native_count=[0]
    machine=cpu(original,hardware,initial,expected_events);adapted=install(machine,unicorn,arm,expected_events,MMIO,MMIO_SIZE)
    if fault:
        def before_bus(uc,access,address,size,value,user):
            if ('read' if access==unicorn.UC_MEM_READ else 'write')!=fault[0]:return
            reference_count[0]+=1
            if reference_count[0]==fault[1]:
                snapshots.append((bytes(state_of(uc)),bytes(uc.mem_read(0,SIZE)),bytes(uc.mem_read(MMIO,MMIO_SIZE)),len(expected_events)-1));uc.emu_stop()
        machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,before_bus,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(entry,STOP,count=budget);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    def fail(direction):
        if fault and fault[0]==direction:
            native_count[0]+=1;return native_count[0]==fault[1]
        return False
    @Read
    def read(user,address,size,out):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('read',hex(address)));return -9
        if fail('read'):return -11
        out[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little');native_events.append(('read',address,size,out[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('write',hex(address)));return -9
        if fail('write'):return -11
        value&=(1<<(size*8))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little');native_events.append(('write',address,size,value));return 0
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,budget)
    completed=expected.r[15]==STOP
    context=(driver,part,bank_index,program,kind,budget,fault)
    if fault:
        assert snapshots,('actual hardware access not reached',context)
        state,ram,hardware_before,n=snapshots[0]
        assert rc==-11 and bytes(actual)==state and native==ram and nhardware==hardware_before,(context,'fault state',rc)
        assert native_events==expected_events[:n]
    else:
        assert rc==(0 if completed else -3) and not errors,(context,rc,hex(actual.r[15]),hex(expected.r[15]),errors)
        assert bytes(actual)==bytes(expected),(context,'CPU',list(actual.r),list(expected.r),hex(actual.cpsr),hex(expected.cpsr))
        assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(context,'memory')
        assert native_events==expected_events,(context,'bus effects')
        if completed:
            def result(a):return struct.unpack_from('<I',native,a)[0]
            control=word(0x48c4+driver*8)
            if kind in ('bad_magic','bad_version','bad_trailer'):
                error={'bad_magic':2,'bad_version':8,'bad_trailer':4}[kind]
                assert native[channel+0x2a]&error and native[channel+0x28]&128,'independent bank rejection'
            elif kind in ('bad_count','bad_program'):assert native[channel+0x29]&4 and native[channel+0x28]&128 and native[control]==0,'program-count status delivered to channel'
            elif kind=='lower':
                assert native[group:group+64]==original[group:group+64] and native[channel+0x29]&2 and native[channel+0x28]&128 and native[control]==0,('lower priority retained program and delivered channel status',context)
                assert native[pending:pending+24]==original[pending:pending+24] and native[0xb000:0xb008]==original[0xb000:0xb008]
                assert not native_events,'blocked replacement had no hardware writes'
            else:
                start=bank+offsets[program];end=bank+(offsets[program+1] if program<2 else size-4)
                expected_fields={4:end,8:clocks[program],0xc:clocks[program]*periods[program]//16,0x10:start+12,0x14:start,0x34:periods[program],0x38:periods[program]}
                assert all(result(group+offset)==value for offset,value in expected_fields.items()),('independent SMSB program initialization',context)
                assert native[group]==priority&0xf8 and native[group+1]&1,'independent masked priority/active state'
                if active:
                    assert native[pending:pending+12]==bytes(12) and native[pending+12:pending+24]==original[pending+12:pending+24],'matching pending notes removed only'
                    assert result(0xb000)==0 and result(0xb004)==other_command,'matching ring commands removed only'
                    mask=word(0x17fc+driver*12)
                    for i in range(48):
                        expected_voice=1 if i%2==0 else 0x81
                        assert native[0xe800+i*64]==expected_voice,'matching voice release only'
                        before=struct.unpack_from('<I',hardware,i*128)[0];after=struct.unpack_from('<I',nhardware,i*128)[0]
                        assert after==((before&mask)|0x8000 if i%2==0 else before),'independent hardware key-off mask'
    return {'driver':driver,'flags':hex(flags),'part':part,'bank':bank_index,'program':program,'kind':kind,'budget':budget,'fault':fault,'completed':completed,'alignment_corrections':len(adapted),'mmio_events':len(native_events)}
for params in itertools.product(range(2),(0,0xf0000000),(0,3,7),(0,7,31),range(3),('start','replace','force','lower','higher','zero_old','masked_equal','masked_force')):
    bank_cases.append(compare(*params))
for driver,flags,part,bank,kind in itertools.product(range(2),(0,0xf0000000),(0,3,7),(0,7,31),('bad_magic','bad_version','bad_trailer','bad_count','bad_program')):
    bank_cases.append(compare(driver,flags,part,bank,0,kind))
for driver,part,direction,ordinal in itertools.product(range(2),(0,7),('read','write'),(1,24)):
    guard_cases.append(compare(driver,0xf0000000,part,7,2,'force',fault=(direction,ordinal)))
for driver,kind,budget in itertools.product(range(2),('start','replace','lower'),(1,16,64,256,512,1024)):
    budget_cases.append(compare(driver,0xf0000000,3,7,1,kind,budget))
names=re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_dispatcher.ps1').read_text())
paths=[Path(__file__),baseline,P/'verify_arm_sound_reset_services.py',dll,P/'arm_legacy_alignment_reference.py',P/'build_arm_sound_dispatcher.ps1']+[P/'source'/name for name in names]
paths += [P/'source/sa_arm_sound_boot_native.c',P/'build_arm_sound_boot_native.ps1'] if '--connected' in sys.argv else []
report={'bank_cases_passed':len(bank_cases),'mmio_failure_cases_passed':len(guard_cases),'budget_cases_passed':len(budget_cases),'bank_cases':bank_cases,'guards':guard_cases,'budgets':budget_cases,
 'reference_alignment_corrections':sum(c['alignment_corrections'] for c in bank_cases+guard_cases+budget_cases),
 'sha256':{str(path):sha(path) for path in paths},
 'connected':'--connected' in sys.argv,
 'scope':'Separate dispatcher overlay: constructed valid/invalid SMSB sequence banks, three programs with terminal SMSD sentinel, bank/part indexes and priority policies, active pending/ring/voice cleanup. No ARM callbacks. Full CPU/RAM/modeled MMIO/ordered effects versus original ARM with explicit pre-ARMv6 unaligned-load adapter; independent program/rejection/priority/cleanup/hardware assertions. Not retail bank playback, streamed-audio/DSP/all-command validation or main-driver integration.'}
(P/('analysis/arm_sound_dispatcher_banks_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_dispatcher_banks_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(bank_cases),'SMSB bank/priority cases,',len(guard_cases),'MMIO failures and',len(budget_cases),'budget comparisons passed')
