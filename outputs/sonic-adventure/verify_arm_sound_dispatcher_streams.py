"""Positive SOSB streamed-sound and SFpB DSP commands through dispatcher entry."""
from pathlib import Path
P=Path(__file__).resolve().parent
baseline=P/'verify_arm_sound_dispatcher.py'
exec(compile(baseline.read_text().split('special=lambda')[0],str(baseline),'exec'))
from arm_legacy_alignment_reference import install
stream_cases=[];dsp_cases=[];fault_cases=[];budget_cases=[]
def execute(driver,flags,memory,hardware,context,budget=100000,fault=None):
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    entry=0x48e8+driver*8;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    native=bytearray(memory);nhardware=bytearray(hardware)
    events=[];nevents=[];snapshots=[];reference_count=[0];native_count=[0]
    machine=cpu(memory,hardware,initial,events);adapted=install(machine,unicorn,arm,events,MMIO,MMIO_SIZE)
    if fault:
        def before(uc,access,address,size,value,user):
            if ('read' if access==unicorn.UC_MEM_READ else 'write')!=fault[0]:return
            reference_count[0]+=1
            if reference_count[0]==fault[1]:
                snapshots.append((bytes(state_of(uc)),bytes(uc.mem_read(0,SIZE)),bytes(uc.mem_read(MMIO,MMIO_SIZE)),len(events)-1));uc.emu_stop()
        machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,before,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(entry,STOP,count=budget);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    def fail(direction):
        if fault and fault[0]==direction:
            native_count[0]+=1;return native_count[0]==fault[1]
        return False
    @Read
    def read(user,a,n,out):
        if not MMIO<=a<=MMIO+MMIO_SIZE-n:errors.append(('read',a));return -9
        if fail('read'):return -11
        out[0]=int.from_bytes(nhardware[a-MMIO:a-MMIO+n],'little');nevents.append(('read',a,n,out[0]));return 0
    @Write
    def write(user,a,value,n):
        if not MMIO<=a<=MMIO+MMIO_SIZE-n:errors.append(('write',a));return -9
        if fail('write'):return -11
        value&=(1<<(n*8))-1;nhardware[a-MMIO:a-MMIO+n]=value.to_bytes(n,'little');nevents.append(('write',a,n,value));return 0
    rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,budget)
    completed=expected.r[15]==STOP
    if fault:
        assert snapshots,('real bus failure not reached',context,fault)
        state,ram,hw,n=snapshots[0]
        assert rc==-11 and bytes(actual)==state and native==ram and nhardware==hw,(context,fault,'fault state',rc)
        assert nevents==events[:n]
    else:
        assert rc==(0 if completed else -3) and not errors,(context,rc,hex(actual.r[15]),hex(expected.r[15]),errors)
        assert bytes(actual)==bytes(expected),(context,'CPU',list(actual.r),list(expected.r),hex(actual.cpsr),hex(expected.cpsr))
        assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(context,'memory')
        assert nevents==events,(context,'bus effects')
    return native,nhardware,completed,{'context':context,'completed':completed,'budget':budget,'fault':fault,'reads':sum(e[0]=='read' for e in nevents),'writes':sum(e[0]=='write' for e in nevents),'alignment_corrections':len(adapted)}
def stream(driver,flags,slot,bank_index,program,mode,kind,budget=100000,fault=None):
    prepared,hw=bootstrap(driver);memory=bytearray(prepared);hardware=bytearray(hw)
    def literal(a,late=False):return struct.unpack_from('<I',memory,a+driver*(4 if late else 8))[0]
    def put(a,value,n=4):memory[a:a+n]=(value&((1<<(n*8))-1)).to_bytes(n,'little')
    source=literal(0x4b2c);memory[source:source+512]=bytes(512)
    bank=0x1c000;length=0x800;memory[bank:bank+length]=bytes(length)
    memory[bank:bank+4]=b'SOSB';put(bank+4,1);put(bank+8,length);put(bank+12,0 if kind=='single' else 1)
    put(bank+16,0x40);put(bank+20,0x80);memory[bank+length-4:bank+length]=b'ENDB'
    for index,offset in enumerate((0x40,0x80)):
        record=bank+offset;memory[record:record+4]=b'SOSD'
        sample=0x400+index*0x40;put(record+4,((sample<<16)|(sample>>16))&0xffffffff)
        for j,value in enumerate((0x00100020,0x00300040,0x00502000,0x00700080,0x009000a0,0x00b000c0,0x00d000e0,0x00f00100)):put(record+8+j*4,value)
        put(record+4+0x10,0xa5,1);put(record+4+0x12,(0,31)[index],1);put(record+4+0x13,(0,15)[index],1)
        put(record+4+0x15,(0,255)[index],1);put(record+4+0x24,9+index,1)
    put(literal(0x54e4)+bank_index*8,bank);put(literal(0x54e4)+bank_index*8+4,length)
    state=0x11000+slot*0x58;table=literal(0x77e4)
    put(state,0,1);put(state+0x18,0x80808080);put(state+0x1e,128,1);put(state+0x1f,128,1)
    put(state+0x10,0);put(state+0x14,0,1);put(state+0x16,0,1);put(state+0x23,mode,1);put(state+0x34,256)
    active=kind in ('replace','force','lower','higher')
    priority={'force':0,'lower':8,'higher':128}.get(kind,64)
    if active:
        put(state,1,1);put(state+2,8,1);put(state+3,0,1)
        put(table,8,1);put(table+1,1,1);put(table+4,state)
    hardware_address=struct.unpack_from('<I',memory,table+8)[0];assert MMIO<=hardware_address<MMIO+MMIO_SIZE
    if kind=='bad_magic':memory[bank:bank+4]=b'FAIL'
    if kind=='bad_version':put(bank+4,2)
    if kind=='bad_trailer':memory[bank+length-4:bank+length]=b'FAIL'
    requested=2 if kind=='bad_program' else program
    memory[source:source+6]=bytes((0x11,0,slot,bank_index,requested,priority))
    native,nhardware,completed,result=execute(driver,flags,memory,hardware,('SOSB',driver,slot,bank_index,program,mode,kind),budget,fault)
    if completed and not fault:
        def out(a):return struct.unpack_from('<I',native,a)[0]
        display=0x13800+slot*32
        if kind in ('bad_magic','bad_version','bad_trailer'):
            bit={'bad_magic':2,'bad_version':8,'bad_trailer':4}[kind]
            assert native[state]&128 and native[display+0x1f]&bit,'independent SOSB header rejection'
        elif kind=='bad_program':assert native[state]&128 and native[display+0x1c]&4,'inclusive program boundary rejected'
        elif kind=='lower':
            assert native[state]==memory[state]|128 and native[state+1:state+0x58]==memory[state+1:state+0x58],'lower priority preserved record'
            assert native[table:table+16*12]==memory[table:table+16*12] and nhardware==hardware,'blocked stream had no allocation/hardware effects'
            assert native[display+0x1c]&2
        else:
            descriptor=bank+(0x40,0x80)[program]+4;sample=bank+0x400+program*0x40
            assert native[state]&3==1 and native[state+2]==priority>>3 and native[state+3]==0,'independent allocation and active state'
            assert out(table+4)==state and native[table]==priority>>3 and native[table+1]==1,'voice ownership'
            assert out(state+8)==out(state+0x30)==sample and out(state+4)==out(state+0x2c)==0,'sample start/counters'
            h=hardware_address-MMIO
            assert struct.unpack_from('<I',nhardware,h)[0]==(sample>>16)|0xc000 and struct.unpack_from('<I',nhardware,h+4)[0]==sample&0xffff,'sample registers and key-on'
            for j in (0,1,5,6,7):
                value=struct.unpack_from('<I',memory,descriptor+4+j*4)[0]
                assert struct.unpack_from('<II',nhardware,h+8+j*8)==(value&0xffff,value>>16),'packed descriptor register copy'
            volume=memory[descriptor+0x15];pan=memory[descriptor+0x12];send=memory[descriptor+0x13]
            linear=memory[literal(0x824c,True)+pan];encoded=memory[literal(0x8250,True)+min(31,linear)]
            assert native[state+0x20]==nhardware[h+0x29]==volume,'neutral-envelope volume'
            assert native[state+0x21]==nhardware[h+0x24]==encoded,'pan table encoding'
            assert native[state+0x2b]==nhardware[h+0x25]==send and nhardware[h+0x20]==0xa5,'effect controls'
            release=memory[descriptor+0x24];wanted=(release,release+256,release*256//256,256)[mode]
            assert native[state+0x3c]==release and out(state+0x38)==wanted,'release-mode dispatch'
    return result
def dsp(driver,flags,selector,kind,scenario):
    prepared,hw=bootstrap(driver);memory=bytearray(prepared);hardware=bytearray(hw)
    source=struct.unpack_from('<I',memory,0x4b2c+driver*8)[0];memory[source:source+512]=bytes(512);memory[source:source+3]=bytes((0x84,0,selector))
    bank=0x1a000;memory[bank:bank+0x200]=bytes(0x200)
    struct.pack_into('<I',memory,bank,0x42504653);memory[bank+4]=1;struct.pack_into('<I',memory,bank+8,0x100)
    memory[bank+12]=2;struct.pack_into('<II',memory,bank+16,0x40,0x80);struct.pack_into('<I',memory,bank+0xfc,0x42444e45)
    for offset in (0x40,0x80):memory[bank+offset+0x20]=kind;memory[bank+offset+0x23]=0x10 if scenario=='skip_size' else 0
    struct.pack_into('<I',memory,0x14200,bank)
    start=0 if scenario=='pool_zero' else 0x17ffc if scenario=='pool_low' else 0x18000
    length=0x100 if scenario in ('too_small','skip_size') else 0x20000;struct.pack_into('<II',memory,0x14288,start,length)
    struct.pack_into('<II',memory,0x1300c,0x12345678,0xabcdef01);memory[0x13014]=0;memory[0x13420]=0x80
    if scenario=='bad_magic':memory[bank:bank+4]=bytes(4)
    if scenario=='bad_version':memory[bank+4]=2
    if scenario=='bad_trailer':memory[bank+0xfc:bank+0x100]=bytes(4)
    native,_,completed,result=execute(driver,flags,memory,hardware,('SFpB',driver,selector,kind,scenario))
    assert completed
    if scenario in ('valid','skip_size') and selector<2:
        assert native[0x13014]==1 and struct.unpack_from('<II',native,0x1300c)==(bank+0x40*(selector+1),0x30|(kind<<13)),'independent DSP configuration'
        assert native[0x13420]==0x80
    else:assert native[0x13014]==0 and native[0x1300c:0x13014]==memory[0x1300c:0x13014],'invalid DSP configuration retained old pointers'
    return result
for params in itertools.product(range(2),(0,0xf0000000),(0,3,7),(0,7,31),range(2),range(4),('start','replace','force','lower','higher')):stream_cases.append(stream(*params))
for driver,flags,slot,bank,kind in itertools.product(range(2),(0,0xf0000000),(0,7),(0,31),('single','bad_magic','bad_version','bad_trailer','bad_program')):stream_cases.append(stream(driver,flags,slot,bank,0,0,kind))
for driver,flags,selector,kind,scenario in itertools.product(range(2),(0,0xf0000000),range(3),range(4),('valid','bad_magic','bad_version','bad_trailer','pool_zero','pool_low','too_small','skip_size')):dsp_cases.append(dsp(driver,flags,selector,kind,scenario))
for driver in range(2):
    probe=stream(driver,0xf0000000,3,7,1,2,'start')
    for direction in ('read','write'):
        for ordinal in (1,probe['reads' if direction=='read' else 'writes']):fault_cases.append(stream(driver,0xf0000000,3,7,1,2,'start',fault=(direction,ordinal)))
for driver,kind,budget in itertools.product(range(2),('start','replace','lower'),(1,16,64,256,512,1024)):budget_cases.append(stream(driver,0xf0000000,3,7,1,2,kind,budget))
names=re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_dispatcher.ps1').read_text())
paths=[Path(__file__),baseline,P/'verify_arm_sound_reset_services.py',dll,P/'arm_legacy_alignment_reference.py',P/'build_arm_sound_dispatcher.ps1']+[P/'source'/name for name in names]
paths += [P/'source/sa_arm_sound_boot_native.c',P/'build_arm_sound_boot_native.ps1'] if '--connected' in sys.argv else []
report={'stream_cases_passed':len(stream_cases),'dsp_cases_passed':len(dsp_cases),'mmio_failure_cases_passed':len(fault_cases),'budget_cases_passed':len(budget_cases),'stream_cases':stream_cases,'dsp_cases':dsp_cases,'faults':fault_cases,'budgets':budget_cases,
 'sha256':{str(path):sha(path) for path in paths},
 'connected':'--connected' in sys.argv,
 'scope':'Constructed mono SOSB banks and SFpB configurations through dispatcher overlay; null ARM callback and full CPU/RAM/modeled MMIO/ordered-effect equality with original ARM using explicit legacy alignment adapter. Independent sample/register/ownership/priority/pan/volume/release and DSP configuration checks. No real streamed-sample decoding, DSP execution, timing/audio synthesis, all-command or main-driver integration proof.'}
(P/('analysis/arm_sound_dispatcher_streams_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_dispatcher_streams_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(stream_cases),'SOSB stream cases,',len(dsp_cases),'DSP cases,',len(fault_cases),'MMIO failures and',len(budget_cases),'budgets passed')
