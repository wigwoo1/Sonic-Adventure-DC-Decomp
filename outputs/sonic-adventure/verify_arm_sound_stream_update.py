"""Compare the complete native stream update and its helpers with original ARM."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
connected='--connected' in sys.argv
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if connected else 'work/sonic-adventure/build/arm_sound_stream_update_native.dll')
library=ctypes.CDLL(str(dll));fn=library.sa_arm_sound_boot_native if connected else library.sa_arm_sound_stream_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write]+([Service] if connected else [])+[U,U];fn.restype=ctypes.c_int
cases=[];budgets=[];faults=[];flag_cases=[]
activity=(0,1,5,0x11,0x15,0x14)
def run(driver,fill,flags,active,pattern,budget=20000,fault=None,single=None):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    def literal(a):return struct.unpack_from('<I',original,a+driver*8)[0]
    table=literal(0x70f8);streams=literal(0x70fc);bus=literal(0x73a8)
    def put(a,v,n=4):original[a:a+n]=(v&((1<<(n*8))-1)).to_bytes(n,'little')
    for i in range(16):
        put(table+i*12,0x55555555);put(table+i*12+4,0xaaaaaaaa);put(table+i*12+8,MMIO+i*128)
    for i in range(24):
        s=streams+i*0x58;put(s,active,1);put(s+3,i%16,1)
        put(s+0xc,(pattern*0x800+i*31)&0x7fff);put(s+0x10,(i-12)*13)
        for reload,step,target,delay,current in ((0x14,0x15,0x18,0x1c,0x1e),(0x16,0x17,0x19,0x1d,0x1f)):
            put(s+reload,(pattern+i)%3,1);put(s+step,(1,3,127,255)[(pattern+i)%4],1)
            put(s+target,(0,1,64,127,254,255)[pattern],1)
            put(s+current,(255,254,127,64,1,0)[pattern],1);put(s+delay,(pattern+i)%3,1)
        put(s+0x24,(pattern*53+i*11)&255,1);put(s+0x28,(pattern+i)%32,1)
        put(s+0x27,i%3,1);put(s+0x23,2,1);put(s+0x29,7,1)
        put(s+0x30,0x12340000+i);put(s+0x38,i%3)
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE
    status=(0,0x4000,0x8000,0x10000,0x18000,0xffffffff)[pattern]
    struct.pack_into('<II',hardware,bus-MMIO+0x10,status,(0,1,32,0x7fffffff,0x80000000,0xffffffff)[pattern])
    for i in range(16):struct.pack_into('<I',hardware,i*128,0x4200 if i%2 else 0x4000)
    nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    entry=0x70a4+driver*8;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.cpsr=flags|0x13
    if single:
        kind,value=single
        entry={'gt':0x71c4+driver*8,'lt':0x7d94+driver*4,'cc':0x7168+driver*8,'rsb':0x71ac+driver*8,'shift':0x7364+driver*8}[kind]
        for register in (1,2,6):initial.r[register]=value
    initial.r[15]=entry
    expected_events=[];native_events=[];snapshots=[];ref_count=[0];native_count=[0]
    machine=cpu(original,hardware,initial,expected_events)
    if fault:
        def before_bus(uc,access,address,size,value,user):
            if ('read' if access==unicorn.UC_MEM_READ else 'write')!=fault[0]:return
            ref_count[0]+=1
            if ref_count[0]==fault[1]:
                snapshots.append((bytes(state_of(uc)),bytes(uc.mem_read(0,SIZE)),bytes(uc.mem_read(MMIO,MMIO_SIZE)),len(expected_events)-1));uc.emu_stop()
        machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,before_bus,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(entry,STOP,count=budget);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    def fail(direction):
        if fault and fault[0]==direction:
            native_count[0]+=1;return native_count[0]==fault[1]
        return False
    @Read
    def read(user,address,size,value):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('read',address));return -9
        if fail('read'):return -11
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('write',address));return -9
        if fail('write'):return -11
        value&=(1<<(size*8))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    args=[view,ctypes.byref(actual),driver,None,read,write]+([Service()] if connected else [])+[STOP,budget]
    rc=fn(*args)
    if fault:
        assert snapshots,'retail MMIO fault not reached'
        state,ram,hw,n=snapshots[0]
        assert rc==-11 and bytes(actual)==state and native==ram and nhardware==hw,(driver,fault,'fault state')
        assert native_events==expected_events[:n]
    else:
        completed=expected.r[15]==STOP
        assert rc==(0 if completed else -3) and not errors,(driver,active,pattern,budget,rc,errors)
        assert bytes(actual)==bytes(expected),(driver,active,pattern,budget,single,'CPU')
        assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,active,pattern,'memory')
        assert native_events==expected_events
        if single:
            kind,value=single;n=bool(flags&0x80000000);z=bool(flags&0x40000000);c=bool(flags&0x20000000);v=bool(flags&0x10000000)
            if kind=='gt':assert actual.r[2]==(255 if not z and n==v else value) and actual.cpsr==initial.cpsr
            if kind=='lt':assert actual.r[6]==(0 if n!=v else value) and actual.cpsr==initial.cpsr
            if kind=='cc':assert actual.r[15]==(0x7180+driver*8 if not c else entry+4) and actual.cpsr==initial.cpsr
            if kind=='rsb':assert actual.r[2]==(256-value)&0xffffffff and actual.cpsr==initial.cpsr
            if kind=='shift':
                result=value>>15;wanted=(flags&0x10000000)|(0x40000000 if not result else 0)|(((value>>14)&1)<<29)|0x13
                assert actual.r[1]==result and actual.cpsr==wanted,'independent shifted MOVS carry'
        elif completed:
            for i in range(24):
                s=streams+i*0x58
                if active==0:assert native[s:s+0x58]==original[s:s+0x58],'inactive record preserved'
                if i>=8 and active&1:
                    assert native[s+0x23]==(status>>15)&255,'independent status extraction'
                if i<8 and active&1 and status&0x8000:
                    released=(i%2==0 or i%3==1)
                    assert bool(native[s]&1)==(not released),'independent release decision'
                    if released:
                        for offset in (0x3c,0x38,0x30,0x28,0xc,0x24):assert native[s+offset:s+offset+4]==bytes(4) if offset!=0x28 else native[s+0x28]==0,'release cleanup'
                        assert native[s+0x23]==2 and native[s+0x29]==7,'release retained metadata'
    return {'driver':driver,'fill':fill,'flags':hex(flags),'active':active,'pattern':pattern,'budget':budget,'fault':fault,'single':single,'mmio_events':len(native_events)}
for params in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000),activity,range(6)):cases.append(run(*params))
for driver,budget in itertools.product(range(2),(1,4,16,64,128,256,512,1024)):
    budgets.append(run(driver,0x55,0xf0000000,0x15,4,budget))
for driver,direction,ordinal in itertools.product(range(2),('read','write'),(1,2,8)):
    faults.append(run(driver,0x55,0xf0000000,0x15,4,fault=(direction,ordinal)))
for driver,flag_bits,kind,value in itertools.product(range(2),range(16),('gt','lt','cc','rsb','shift'),(0,1,0x4000,0xffffffff)):
    flag_cases.append(run(driver,0,flag_bits<<28,0,0,budget=1,single=(kind,value)))
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_stream_update.c',P/'source/sa_arm_sound_stream_update_fixture.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_stream_update.py',P/'create_arm_sound_controls.py',P/'source/sa_arm_sound_snapshot_fixture.c',P/'build_arm_sound_stream_update.ps1',P/'analysis/arm_sound_stream_update_translation.json']
report={'cases_passed':len(cases),'budget_cases_passed':len(budgets),'mmio_failure_cases_passed':len(faults),'flag_cases_passed':len(flag_cases),'cases':cases,'budgets':budgets,'faults':faults,'flag_cases':flag_cases,
 'connected':connected,'native_service_callback':False,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':('Connected driver' if connected else 'Standalone fixture')+': complete 24-stream update and direct helper paths, no ARM callbacks. Full active CPU/CPSR/two MiB RAM/modeled MMIO/ordered bus comparisons; independent release, status and flag assertions. No physical timing/synthesis/playback proof.'}
(P/('analysis/arm_sound_stream_update_connected_verification.json' if connected else 'analysis/arm_sound_stream_update_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'stream comparisons,',len(budgets),'budgets,',len(faults),'MMIO faults and',len(flag_cases),'condition/shift vectors passed')
