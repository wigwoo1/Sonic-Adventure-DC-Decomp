"""Original ARM and independent arithmetic checks for all three snapshot loops."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
connected='--connected' in sys.argv
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if connected else 'work/sonic-adventure/build/arm_sound_snapshot_native.dll')
library=ctypes.CDLL(str(dll))
fn=library.sa_arm_sound_boot_native if connected else library.sa_arm_sound_snapshot_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write]+([Service] if connected else [])+[U,U]
fn.restype=ctypes.c_int
cases=[];budgets=[];faults=[];flag_cases=[]
def run(driver,fill,flags,mode,active,budget=10000,fault_read=0,flag_pair=None):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    def literal(a):return struct.unpack_from('<I',original,a+driver*8)[0]
    group3=literal(0x5858);target3=literal(0x5854);target1=literal(0x6638)
    assert group3+16*0x58<=SIZE and target3+16*32<=SIZE and target1+8*32<=SIZE
    for base,count,stride in ((0xc000,8,0x80),(0x10000,8,0x40),(0x11000,8,0x58),(group3,16,0x58)):
        for i in range(count):
            row=base+i*stride
            original[row:row+stride]=bytes((j*29+i*41+mode*17+fill)&255 for j in range(stride))
            if base!=0x10000:
                original[row]=(original[row]&0xfe)|(1 if active==1 or (active==2 and i%2) else 0)
                original[row+3]=(mode+i)%16
                original[row+0x27]=mode%4
                original[row+0x29]=0 if mode%2==0 else 0xff
                original[row+0x2a]=0 if mode%3==0 else 1
    native=bytearray(original)
    hardware=bytearray((i*17+fill+mode)&255 for i in range(MMIO_SIZE));nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    entry=(0x6484 if flag_pair is not None else 0x63f8)+driver*8
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    if flag_pair is not None:initial.r[2],initial.r[5]=flag_pair
    expected_events=[];native_events=[];snapshots=[];reference_reads=[0]
    machine=cpu(original,hardware,initial,expected_events)
    if fault_read:
        def before_read(uc,access,address,size,value,user):
            reference_reads[0]+=1
            if reference_reads[0]==fault_read:
                snapshots.append((bytes(state_of(uc)),bytes(uc.mem_read(0,SIZE)),len(expected_events)-1))
                uc.emu_stop()
        machine.hook_add(unicorn.UC_HOOK_MEM_READ,before_read,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(entry,STOP,count=budget)
    expected=state_of(machine);actual=State.from_buffer_copy(initial)
    view=(ctypes.c_uint8*SIZE).from_buffer(native);native_reads=[0]
    @Read
    def read(user,address,size,value):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('read',hex(address)));return -9
        native_reads[0]+=1
        if native_reads[0]==fault_read:return -11
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('write',hex(address)));return -9
        value&=(1<<(size*8))-1
        nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    args=[view,ctypes.byref(actual),driver,None,read,write]+([Service()] if connected else [])+[STOP,budget]
    rc=fn(*args)
    if fault_read:
        assert snapshots,'actual retail MMIO read not reached'
        state,ram,n=snapshots[0]
        assert rc==-11 and bytes(actual)==state and native==ram and nhardware==hardware,(driver,fault_read,'fault state')
        assert native_events==expected_events[:n]
    else:
        completed=expected.r[15]==STOP
        assert rc==(0 if completed else -3) and not errors,(driver,mode,active,budget,rc,errors)
        assert bytes(actual)==bytes(expected),(driver,mode,active,budget,'CPU')
        assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,mode,active,'memory')
        assert expected_events==native_events
        if flag_pair is not None:
            value=flag_pair[0]|flag_pair[1]
            independent=(flags&0x30000000)|(value&0x80000000)|(0x40000000 if not value else 0)|0x13
            assert actual.cpsr==independent and actual.r[5]==value,'independent ORRS flags'
        elif completed:
            oracle=bytearray(original)
            def word(a):return struct.unpack_from('<I',original,a)[0]
            def put(a,v,n=4):oracle[a:a+n]=(v&((1<<(n*8))-1)).to_bytes(n,'little')
            for i in range(8):
                s=0xc000+i*0x80;t=target1+i*32;v=0x10000+i*0x40
                for so,to in ((1,1),(7,2),(0x24,0x11),(0x26,0x10)):put(t+to,original[s+so]-128,1)
                for so,to in ((0x10,8),(0x1c,4)):put(t+to,word(s+so)-32768)
                put(t+0x18,word(v+0x10)-word(v+0x14));put(t+0x14,word(v+0x30));put(t+0xc,word(v+0x18))
                put(t+0x1c,original[s+0x29],1);put(t+0x1f,original[s+0x2a],1)
                put(t,original[s+0x28]|(128 if original[s+0x29] or original[s+0x2a] else 0),1)
            for base,target,count,third in ((0x11000,0x13800,8,False),(group3,target3,16,True)):
                for i in range(count):
                    s=base+i*0x58;t=target+i*32
                    for so,to in ((0x1e,1),(0x1f,2),(0x1b,0x11),(0x1a,0x10)):put(t+to,original[s+so]-128,1)
                    for so,to in ((0x44,4),(0x50,8)):put(t+to,word(s+so)-32768)
                    put(t,original[s],1)
                    if original[s]&1:
                        put(t+0x18,word(s+4));put(t+0xc,word(s+(0x2c if third else 0x38)))
                        if third:
                            factor=word(s+0x34);selector=original[s+0x27]&3
                            if not selector:factor>>=1
                            elif selector&2:factor=(factor*2)&0xffffffff
                            value=word(s+0x2c)*factor+word(s+4)
                        else:
                            h=(original[s+3]+0x30)*128
                            high=int.from_bytes(hardware[h+12:h+16],'little');low=int.from_bytes(hardware[h+8:h+12],'little')
                            value=((high-low)&0xffffffff)*original[s+0x2c]+word(s+4)
                        put(t+0x14,value)
            assert native==oracle,(driver,mode,active,'independent snapshot formulas')
    return {'driver':driver,'fill':fill,'flags':hex(flags),'mode':mode,'active':active,'budget':budget,'fault_read':fault_read,'flag_pair':flag_pair,'mmio_events':len(native_events)}
for params in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000),range(8),range(3)):
    cases.append(run(*params))
for driver,budget in itertools.product(range(2),(1,4,16,64,256,512,1024,2048)):
    budgets.append(run(driver,0x55,0xf0000000,7,1,budget))
for driver,ordinal in itertools.product(range(2),(1,2,16)):
    faults.append(run(driver,0x55,0xf0000000,7,1,fault_read=ordinal))
for driver,flags,pair in itertools.product(range(2),(0,0x20000000,0x90000000,0xf0000000),((0,0),(0x80000000,0),(0xffffffff,1),(0,1))):
    flag_cases.append(run(driver,0,flags,0,0,budget=1,flag_pair=pair))
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_snapshot.c',P/'source/sa_arm_sound_snapshot_fixture.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_snapshot.py',P/'create_arm_sound_controls.py',P/'build_arm_sound_snapshot.ps1',P/'analysis/arm_sound_snapshot_translation.json']
report={'cases_passed':len(cases),'budget_cases_passed':len(budgets),'mmio_failure_cases_passed':len(faults),'orrs_flag_cases_passed':len(flag_cases),'cases':cases,'budgets':budgets,'faults':faults,'flag_cases':flag_cases,
 'connected':connected,'native_service_callback':False,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':('Connected driver' if connected else 'Standalone fixture')+': static native snapshot routine in both drivers, full active CPU/CPSR/two MiB RAM/modeled MMIO/ordered bus effects compared to original ARM bytes. Independent formulas for all three output groups, ORRS flag vectors, bounded execution and failures on actual retail MMIO reads. No physical timing/synthesis/playback proof.'}
(P/('analysis/arm_sound_snapshot_connected_verification.json' if connected else 'analysis/arm_sound_snapshot_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'snapshot comparisons,',len(budgets),'budgets,',len(faults),'retail MMIO faults and',len(flag_cases),'ORRS flag vectors passed')
