"""Compare pending update plus recovered helpers with original ARM execution."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
connected='--connected' in sys.argv
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if connected else 'work/sonic-adventure/build/arm_sound_update_native.dll')
library=ctypes.CDLL(str(dll))
fn=library.sa_arm_sound_boot_native if connected else library.sa_arm_sound_update_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write]+([Service] if connected else [])+[U,U]
fn.restype=ctypes.c_int
cases=[];budgets=[];faults=[]
def run(driver,fill,flags,blocked,requested,rate,budget=200000,fault_write=0):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    delta=driver*8;entry=0x615c+delta
    def literal(a):return struct.unpack_from('<I',original,a+delta)[0]
    def put(a,value,size=4):
        address=literal(a);assert address+size<=SIZE,(hex(a),hex(address))
        original[address:address+size]=value.to_bytes(size,'little');return address
    block_address=put(0x63bc,blocked,1);request_address=put(0x612c,requested,1)
    put(0x6124,rate);put(0x6128,0x1a000)
    # The reset clear-range descriptor must describe a bounded host fixture.
    clear=literal(0x6120);struct.pack_into('<II',original,clear,0,0)
    descriptor=bytes((i*29+fill+driver)&255 for i in range(4096))
    original[0x1a000:0x1b000]=descriptor
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE;nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    expected_events=[];native_events=[]
    machine=cpu(original,hardware,initial,expected_events)
    snapshots=[];reference_writes=[0]
    if fault_write:
        def before_write(uc,access,address,size,value,user):
            reference_writes[0]+=1
            if reference_writes[0]==fault_write:
                snapshots.append((bytes(state_of(uc)),bytes(uc.mem_read(0,SIZE)),bytes(uc.mem_read(MMIO,MMIO_SIZE)),len(expected_events)-1))
                uc.emu_stop()
        machine.hook_add(unicorn.UC_HOOK_MEM_WRITE,before_write,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(entry,STOP,count=budget)
    expected=state_of(machine);actual=State.from_buffer_copy(initial)
    view=(ctypes.c_uint8*SIZE).from_buffer(native)
    native_writes=[0]
    @Read
    def read(user,address,size,value):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('read',hex(address)));return -9
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('write',hex(address)));return -9
        native_writes[0]+=1
        if native_writes[0]==fault_write:return -11
        value&=(1<<(size*8))-1
        nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    args=[view,ctypes.byref(actual),driver,None,read,write]+([Service()] if connected else [])+[STOP,budget]
    rc=fn(*args)
    if fault_write:
        assert snapshots,'retail bus fault not reached'
        state,ram,hw,event_count=snapshots[0]
        assert rc==-11 and bytes(actual)==state and native==ram and nhardware==hw,(driver,fault_write,'fault state')
        assert native_events==expected_events[:event_count]
        return {'driver':driver,'fault_write':fault_write,'mmio_events':len(native_events)}
    completed=expected.r[15]==STOP
    assert rc==(0 if completed else -3) and not errors,(driver,blocked,requested,rate,budget,rc,errors)
    assert bytes(actual)==bytes(expected),(driver,blocked,requested,rate,budget,'CPU',list(actual.r),list(expected.r))
    assert native==machine.mem_read(0,SIZE),(driver,blocked,requested,rate,budget,'RAM')
    assert nhardware==machine.mem_read(MMIO,MMIO_SIZE) and expected_events==native_events,(driver,'bus')
    if completed:
        if blocked or not requested:
            assert native==original and not native_events,'early return should have no bus effects'
        else:
            assert native[request_address:request_address+4]==bytes(4),'request consumed'
            assert native[block_address]==0,'block flag unchanged'
            count=literal(0x6268)*4;destination=literal(0x6134)
            assert 0<count<=len(descriptor)-0x40
            assert MMIO<=destination<=MMIO+MMIO_SIZE-count
            assert nhardware[destination-MMIO:destination-MMIO+count]==descriptor[0x40:0x40+count],'independent MMIO descriptor copy'
            for source,target in ((0x24,0x6154),(0x22,0x63d4),(0x23,0x6150)):
                assert native[literal(target)]==descriptor[source],'independent field copy'
            assert native[literal(0x63c4)]==0,'completion gate cleared'
    return {'driver':driver,'fill':fill,'flags':hex(flags),'blocked':blocked,'requested':requested,'rate':rate,'budget':budget,'completed':completed,'mmio_events':len(native_events)}
for params in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000),(0,255),(0,255),(0,0x18000)):
    cases.append(run(*params))
for driver,budget in itertools.product(range(2),(1,4,8,16,64,512,2000,20000)):
    budgets.append(run(driver,0x55,0xf0000000,0,255,0x18000,budget))
for driver,ordinal in itertools.product(range(2),(1,65,120)):
    faults.append(run(driver,0x55,0xf0000000,0,255,0x18000,fault_write=ordinal))
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_update.c',P/'source/sa_arm_sound_update_fixture.c',P/'source/sa_arm_sound_reset_services.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_update.py',P/'create_arm_sound_controls.py',P/'build_arm_sound_update.ps1',P/'analysis/arm_sound_update_translation.json']
report={'cases_passed':len(cases),'budget_cases_passed':len(budgets),'cases':cases,'budgets':budgets,'connected':connected,'native_service_callback':False,
 'mmio_failure_cases_passed':len(faults),'faults':faults,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':('Connected driver' if connected else 'Standalone fixture')+': original pending update plus four recovered reset helper dependencies, full active CPU/CPSR, two MiB RAM, modeled MMIO and ordered bus effects. Independent MMIO descriptor/field copies, request consumption and early-return effects. No external ARM callbacks. No timing/synthesis/playback claim.'}
(P/('analysis/arm_sound_update_connected_verification.json' if connected else 'analysis/arm_sound_update_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'native pending-update comparisons,',len(budgets),'partial budgets and',len(faults),'retail MMIO failure checks passed')
