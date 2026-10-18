"""Original-byte whole sequencer, queue-return and multiply-flag comparisons."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
connected='--connected' in sys.argv
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if connected else 'work/sonic-adventure/build/arm_sound_sequencer_native.dll')
library=ctypes.CDLL(str(dll));fn=library.sa_arm_sound_boot_native if connected else library.sa_arm_sound_sequence_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write]+([Service] if connected else [])+[U,U];fn.restype=ctypes.c_int
cases=[];budgets=[];queue_cases=[];flag_cases=[]
def run(driver,fill,flags,opcode=0,budget=10000,paused=False,queue=None,single=None):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    def literal(a):return struct.unpack_from('<I',original,a+8*driver)[0]
    def put(a,v):struct.pack_into('<I',original,a,v&0xffffffff)
    control=literal(0x48c4);messages=literal(0x48c8);pending=literal(0x48cc)
    original[0x10000:0x10200]=bytes(0x200);original[control:pending+0x240]=bytes(pending+0x240-control)
    put(control+4,0x10000);put(control+8,messages);put(control+12,messages)
    original[control+1]=1 if paused else 0
    original[0xb000:0xc000]=bytes(0x1000);put(0x13000,0);put(literal(0x48d4),0)
    put(0x10000,0x100);put(0x10004,0x1a020);put(0x10008,16);put(0x1000c,1)
    put(0x10010,0x1a000);put(0x10014,0x1a000);put(0x10034,16);put(0x10038,2);put(0x1003c,1)
    data=bytes([opcode,1,2,3,4,5,6,7,8]+[0x83]*23);original[0x1a000:0x1a020]=data
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    entry=0x4210+driver*8;stop=STOP
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.cpsr=flags|0x13
    if queue:
        offset,blocked=queue;entry=0x48a0+driver*8
        initial.r[4]=offset if blocked else (offset+4)&0xfff;initial.r[6]=offset
        initial.r[5]=0x12345678;initial.r[7]=0xb000;initial.r[8]=0x13000
        stop=STOP if blocked else STOP+4
    if single:
        address,a,b=single;entry=address+driver*8
        initial.r[0]=a;initial.r[1]=b;initial.r[2]=0x12345678;initial.r[3]=a;initial.r[4]=b
    initial.r[15]=entry
    events=[];machine=cpu(original,hardware,initial,events)
    machine.emu_start(entry,stop,count=budget);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    @Read
    def read(user,address,size,value):errors.append(('unexpected read',hex(address)));return -9
    @Write
    def write(user,address,value,size):errors.append(('unexpected write',hex(address)));return -9
    args=[view,ctypes.byref(actual),driver,None,read,write]+([Service()] if connected else [])+[stop,budget]
    rc=fn(*args)
    completed=expected.r[15]==stop
    assert rc==(0 if completed else -3) and not errors,(driver,opcode,budget,queue,single,rc,errors,hex(actual.r[15]),hex(expected.r[15]))
    assert bytes(actual)==bytes(expected),(driver,opcode,budget,queue,single,'CPU',list(actual.r),list(expected.r),hex(actual.cpsr),hex(expected.cpsr))
    assert native==machine.mem_read(0,SIZE) and hardware==machine.mem_read(MMIO,MMIO_SIZE) and not events,(driver,opcode,'memory/bus')
    if single:
        address,a,b=single
        if address in (0x4438,0x470c,0x4734,0x487c):
            destination,left,right,addend={0x4438:(4,1,4,3),0x470c:(1,0,1,2),0x4734:(1,0,1,2),0x487c:(3,0,3,4)}[address]
            value=(initial.r[left]*initial.r[right]+initial.r[addend])&0xffffffff
            wanted=(flags&0x30000000)|(value&0x80000000)|(0x40000000 if not value else 0)|0x13
            assert actual.r[destination]==value and actual.cpsr==wanted,'independent MLAS result/NZ and TI925T CV convention'
        else:
            destination={0x443c:4,0x4710:1,0x4738:1,0x4880:3}[address]
            assert actual.r[destination]==(0xffffffff if flags&0x10000000 else initial.r[destination]) and actual.cpsr==initial.cpsr,'independent MVNVS'
    elif queue:
        offset,blocked=queue
        if blocked:assert native==original and actual.r[14]==STOP,'queue backpressure preserves memory/return'
        else:
            assert struct.unpack_from('<I',native,0xb000+offset)[0]==0x12345678
            assert struct.unpack_from('<I',native,0x13000)[0]==(offset+4)&0xfff and actual.r[14]==STOP+4,'queue wrap and skipped caller instruction'
    elif completed and paused:assert native==original,'paused sequencer leaves memory unchanged'
    return {'driver':driver,'fill':fill,'flags':hex(flags),'opcode':opcode,'budget':budget,'completed':completed,'paused':paused,'queue':queue,'single':single}
for driver,fill,flags,opcode in itertools.product(range(2),(0,0x55),(0,0xf0000000),range(256)):
    result=run(driver,fill,flags,opcode);assert result['completed'],('whole sequence did not finish',result);cases.append(result)
for driver,fill,flags in itertools.product(range(2),(0,0x55),(0,0xf0000000)):cases.append(run(driver,fill,flags,paused=True))
for driver,opcode,budget in itertools.product(range(2),(0,0x81,0x82,0x88),(1,4,16,64,128,256)):
    budgets.append(run(driver,0x55,0xf0000000,opcode,budget))
for driver,fill,flags,offset,blocked in itertools.product(range(2),(0,0x55),(0,0xf0000000),(0,4,0xffc),(False,True)):
    queue_cases.append(run(driver,fill,flags,queue=(offset,blocked)))
for driver,flag_bits,address,pair in itertools.product(range(2),range(16),(0x4438,0x470c,0x4734,0x487c,0x443c,0x4710,0x4738,0x4880),((0,0),(0xffffffff,1),(0x80000000,2),(0x7fffffff,0xffffffff))):
    flag_cases.append(run(driver,0,flag_bits<<28,budget=1,single=(address,*pair)))
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_sequencer.c',P/'source/sa_arm_sound_sequencer_fixture.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_sequencer.py',P/'create_arm_sound_controls.py',P/'source/sa_arm_sound_snapshot_fixture.c',P/'build_arm_sound_sequencer.ps1',P/'analysis/arm_sound_sequencer_translation.json']
report={'cases_passed':len(cases),'budget_cases_passed':len(budgets),'queue_cases_passed':len(queue_cases),'flag_cases_passed':len(flag_cases),'cases':cases,'budgets':budgets,'queue_cases':queue_cases,'flag_cases':flag_cases,
 'connected':connected,'native_service_callback':False,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':('Connected driver' if connected else 'Standalone fixture')+': sequencer and indirect handlers with all 256 initial opcodes, full active CPU/CPSR/two MiB RAM against original ARM; queue wrap/backpressure/return and MLAS/MVNVS assertions. No ARM callbacks. Finite constructed input sequences, not complete gameplay/music validation. Native MLAS preserves C like TI925T reference; ARMv4 carry is unpredictable and not hardware-validated. No timing/synthesis/playback proof.'}
(P/('analysis/arm_sound_sequencer_connected_verification.json' if connected else 'analysis/arm_sound_sequencer_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'whole-sequencer comparisons,',len(budgets),'budgets,',len(queue_cases),'queue and',len(flag_cases),'multiply/VS vectors passed')
