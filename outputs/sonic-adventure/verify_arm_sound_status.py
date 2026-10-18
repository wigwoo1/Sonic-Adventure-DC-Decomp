"""Original ARM versus independent native status routines; full state and bus checks."""
from pathlib import Path
# Reuse the established CPU/MMIO ABI without executing its reset cases.
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
connected='--connected' in sys.argv
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if connected else 'work/sonic-adventure/build/arm_sound_status_native.dll')
library=ctypes.CDLL(str(dll))
fn=library.sa_arm_sound_boot_native if connected else library.sa_arm_sound_status_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write]+([Service] if connected else [])+[U,U]
fn.restype=ctypes.c_int
cases=[];budgets=[];faults=[]
def run(driver,fill,flags,entry,values,budget=1000,fault=None,blocked=False):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    def put(literal,value,size=4):
        address=struct.unpack_from('<I',original,literal)[0]
        assert address+size<=SIZE,(hex(literal),hex(address))
        original[address:address+size]=value.to_bytes(size,'little')
        return address
    a,b,c,position,packed,output,dirty=values
    put(0x974,a,1);put(0x97c,b,1);put(0x984,c,1)
    put(0x96c,position);put(0x970,packed);put(0x98c,output)
    put(0x978,(255 if blocked else 0) if entry==0x768 else 0x77,1)
    struct.pack_into('<I',original,0x854,dirty)
    # Retail destinations are shared RAM. Redirect only fault fixtures to the
    # modeled external bus to exercise callback failure propagation explicitly.
    if fault:
        struct.pack_into('<I',original,0x990,MMIO+0x100)
        struct.pack_into('<I',original,0x994,MMIO+0x104)
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE;nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    expected_events=[];native_events=[];seen=[];snapshot=[]
    def trace(uc,address,size,user):
        seen.append(address)
    machine=cpu(original,hardware,initial,expected_events,trace)
    # Fault reference stops before the chosen bus instruction (no committed effects).
    if fault:
        direction,ordinal=fault;counter=[0]
        def before_bus(uc,access,address,size,value,user):
            if ('read' if access==unicorn.UC_MEM_READ else 'write')==direction:
                counter[0]+=1
                if counter[0]==ordinal:
                    snapshot.append((bytes(state_of(uc)),bytes(uc.mem_read(0,SIZE)),bytes(uc.mem_read(MMIO,MMIO_SIZE)),len(expected_events)-1))
                    uc.emu_stop()
        machine.hook_add(unicorn.UC_HOOK_MEM_READ|unicorn.UC_HOOK_MEM_WRITE,before_bus,begin=MMIO,end=MMIO+MMIO_SIZE-1)
    machine.emu_start(entry,STOP,count=budget)
    expected=state_of(machine);actual=State.from_buffer_copy(initial)
    view=(ctypes.c_uint8*SIZE).from_buffer(native);counter=[0]
    def fail(direction):
        if fault and fault[0]==direction:
            counter[0]+=1
            return counter[0]==fault[1]
        return False
    @Read
    def read(user,address,size,value):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:return -9
        if fail('read'):return -11
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:return -9
        if fail('write'):return -11
        value&=(1<<(size*8))-1
        nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    args=[view,ctypes.byref(actual),driver,None,read,write]+([Service()] if connected else [])+[STOP,budget]
    rc=fn(*args)
    if fault:
        assert snapshot,'fault was not reached'
        state,ram,hw,n=snapshot[0]
        assert rc==-11 and bytes(actual)==state and native==ram and nhardware==hw,(driver,hex(entry),fault,'fault state',rc)
        assert native_events==expected_events[:n]
    else:
        assert rc==(0 if expected.r[15]==STOP else -3),(driver,hex(entry),rc)
        assert bytes(actual)==bytes(expected),(driver,hex(entry),values,budget,'CPU')
        assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,hex(entry),values,'memory')
        assert native_events==expected_events,(driver,hex(entry),'events')
        if entry==0x6d0 and expected.r[15]==STOP:
            for literal,value in ((0x978,255 if a and position else 0),(0x980,255 if b else 0),(0x988,255 if c else 0)):
                address=struct.unpack_from('<I',original,literal)[0]
                assert native[address]==value,'independent status result'
    return {'driver':driver,'entry':hex(entry),'fill':fill,'flags':hex(flags),'values':values,'budget':budget,'instructions':len(seen),'mmio_events':len(native_events),'fault':fault,'blocked':blocked}
for driver,fill,flags in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000)):
    for a,b,c,position in itertools.product((0,255),repeat=4):
        cases.append(run(driver,fill,flags,0x6d0,(a,b,c,position,0x4321,0x2468,0)))
    for position,packed,output in itertools.product((0,0x17fff,0x18000,0xffffffff),(0,0x6000,0xffff),(0,0x8000,0x10000,0x20000)):
        cases.append(run(driver,fill,flags,0x768,(0,0,0,position,packed,output,255)))
    for dirty in (0,255):
        cases.append(run(driver,fill,flags,0x768,(0,0,0,0x18000,0x30,0,dirty)))
    cases.append(run(driver,fill,flags,0x768,(0,0,0,0x18000,0,0x20000,255),blocked=True))
for driver,entry,budget in itertools.product(range(2),(0x6d0,0x768),(1,2,4,8,16,32)):
    budgets.append(run(driver,0,0xf0000000,entry,(255,255,255,0x18000,0,0x20000,255),budget))
for driver,ordinal in itertools.product(range(2),(1,2)):
    faults.append(run(driver,0,0xf0000000,0x768,(0,0,0,0x18000,0,0x20000,255),fault=('write',ordinal)))
report={'cases_passed':len(cases),'budget_cases_passed':len(budgets),'cases':cases,'budgets':budgets,
 'connected':connected,'native_service_callback':False,
 'fault_cases_passed':len(faults),'faults':faults,
 'scope':('Connected driver' if connected else 'Standalone fixture')+' with two status routines per driver; full CPU/CPSR/2 MiB RAM/modeled MMIO/ordered bus equality and independent status booleans. Fault fixtures redirect shared-RAM output pointers to modeled MMIO; these are constructed bus-failure cases, not retail MMIO paths. No timing or playback claim.',
 'sha256':{str(p.relative_to(PROJECT)) if p.is_relative_to(PROJECT) else str(p):sha(p) for p in (Path(__file__),support,dll,P/'source/sa_arm_sound_status.c',P/'source/sa_arm_sound_status_fixture.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_status.py',P/'create_arm_sound_controls.py',P/'build_arm_sound_status.ps1',P/'analysis/arm_sound_status_translation.json')}}
(P/('analysis/arm_sound_status_connected_verification.json' if connected else 'analysis/arm_sound_status_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'native status comparisons,',len(budgets),'partial-budget comparisons and',len(faults),'MMIO fault checks passed')
