"""Compare whole native ramp loops with ARM bytes and independent field formulas."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
connected='--connected' in sys.argv
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if connected else 'work/sonic-adventure/build/arm_sound_ramps_native.dll')
library=ctypes.CDLL(str(dll));fn=library.sa_arm_sound_boot_native if connected else library.sa_arm_sound_ramp_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write]+([Service] if connected else [])+[U,U];fn.restype=ctypes.c_int
entries=(0x6620,0x6640,0x66e0,0x6780)
vectors=((0,0,0,0,0),(255,0,0,0,1),(0,255,0,0,255),(100,10,1,0,20),(10,100,1,0,20),
 (100,90,1,0,20),(90,100,1,0,20),(0,1,1,0,255),(1,0,1,1,255),(128,127,255,255,0),
 (0xffffffff,0xfffffff0,1,0,255),(0,0x80000000,1,0,255),(0x80000000,0xffffffff,1,0,255),
 (0xfffffff0,0xfffffff8,1,0,1),(0x4000,0x8000,0,0,0))
cases=[];budgets=[];flag_cases=[]
def run(driver,fill,flags,kind,pattern,budget=10000,single=None):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    def literal(a):return struct.unpack_from('<I',original,a+driver*8)[0]
    counter=literal(0x663c);clock_states=literal(0x6844)
    assert counter+4<=SIZE and clock_states+8*0x40<=SIZE
    struct.pack_into('<I',original,counter,vectors[pattern][0])
    for i in range(8):
        target,current,reload,delay,step=vectors[(pattern+i)%len(vectors)];s=0xc000+i*0x80
        original[s+0x28]=(fill+i*31)&255
        for offset in (0,6):original[s+offset:s+offset+5]=bytes((target&255,current&255,reload,delay,step))
        struct.pack_into('<II',original,s+0x18,target,current)
        original[s+0x20:s+0x23]=bytes((reload,delay,step))
    native=bytearray(original);hardware=bytearray([fill])*MMIO_SIZE
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    entry=entries[kind]+driver*8
    if single is not None:
        instruction,a,b=single;entry=(0x66b0 if instruction=='movcc' else 0x681c)+driver*8
        initial.r[3]=a;initial.r[2]=b
    initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    events=[];machine=cpu(original,hardware,initial,events)
    machine.emu_start(entry,STOP,count=budget);expected=state_of(machine)
    actual=State.from_buffer_copy(initial);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    @Read
    def read(user,address,size,value):errors.append(('unexpected read',address));return -9
    @Write
    def write(user,address,value,size):errors.append(('unexpected write',address));return -9
    args=[view,ctypes.byref(actual),driver,None,read,write]+([Service()] if connected else [])+[STOP,budget]
    rc=fn(*args)
    completed=expected.r[15]==STOP
    assert rc==(0 if completed else -3) and not errors,(driver,kind,pattern,budget,rc,errors)
    assert bytes(actual)==bytes(expected),(driver,kind,pattern,budget,'CPU')
    assert native==machine.mem_read(0,SIZE) and hardware==machine.mem_read(MMIO,MMIO_SIZE) and not events,(driver,kind,pattern,'memory/bus')
    if single is not None:
        instruction,a,b=single
        if instruction=='movcc':
            assert actual.r[3]==(a if flags&0x20000000 else 0) and actual.cpsr==initial.cpsr
        else:
            value=(a+b)&0xffffffff
            independent=(flags&0x0fffffff)|(value&0x80000000)|(0x40000000 if not value else 0)|(0x20000000 if a+b>0xffffffff else 0)
            if ((a>>31)==(b>>31)) and ((value>>31)!=(a>>31)):independent|=0x10000000
            assert actual.r[3]==value and actual.cpsr==independent|0x13,'independent ADDS flags'
    elif completed:
        oracle=bytearray(original)
        if kind==0:struct.pack_into('<I',oracle,counter,(vectors[pattern][0]+1)&0xffffffff)
        else:
            width=4 if kind==3 else 1;offset=(0,0,6,0x18)[kind];mask=(0,4,0x10,8)[kind]
            def get(a,n=1):return int.from_bytes(original[a:a+n],'little')
            def put(a,v,n=1):oracle[a:a+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
            for i in range(8):
                s=0xc000+i*0x80;target=get(s+offset,width);current=get(s+offset+width,width);state=get(s+0x28)
                reload_at=s+(0x20 if kind==3 else offset+2);delay_at=reload_at+1;step_at=reload_at+2
                if target==current:put(s+0x28,state&~mask);continue
                if get(delay_at):put(delay_at,get(delay_at)-1);continue
                reload=get(reload_at);put(delay_at,reload)
                if not reload:current=target
                else:
                    step=get(step_at)*(16 if kind==3 else 1)
                    if current<target:
                        value=(current+step)&0xffffffff;current=target if value>=target else value
                    else:current=max(target,max(0,current-step))
                put(s+offset+width,current,width)
                if kind==3:
                    value=((current&literal(0x6848))-0x8000+0x4000)&0xffffffff
                    put(clock_states+i*0x40+0x3c,0 if value&0x80000000 else value,4)
                else:put(s+offset+5,1)
                # The second byte ramp clears 0x10 at equality but sets 0x04
                # after an update. Preserve that exact original behavior.
                put(s+0x28,state|(8 if kind==3 else 4))
        assert native==oracle,(driver,kind,pattern,'independent complete RAM oracle')
    return {'driver':driver,'fill':fill,'flags':hex(flags),'kind':kind,'pattern':pattern,'budget':budget,'completed':completed,'single':single}
for params in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000),range(4),range(len(vectors))):cases.append(run(*params))
for driver,kind,budget in itertools.product(range(2),range(4),(1,2,4,8,16,64,128,256)):
    budgets.append(run(driver,0x55,0xf0000000,kind,7,budget))
for driver,flags,instruction,pair in itertools.product(range(2),(0,0x20000000,0x90000000,0xf0000000),('movcc','adds'),((0,0),(0xffffffff,1),(0x7fffffff,1),(0x80000000,0x80000000))):
    flag_cases.append(run(driver,0,flags,0,0,budget=1,single=(instruction,*pair)))
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_ramps.c',P/'source/sa_arm_sound_ramps_fixture.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_ramps.py',P/'create_arm_sound_controls.py',P/'source/sa_arm_sound_snapshot_fixture.c',P/'build_arm_sound_ramps.ps1',P/'analysis/arm_sound_ramps_translation.json']
report={'cases_passed':len(cases),'budget_cases_passed':len(budgets),'flag_cases_passed':len(flag_cases),'cases':cases,'budgets':budgets,'flag_cases':flag_cases,
 'connected':connected,'native_service_callback':False,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':('Connected driver' if connected else 'Standalone fixture')+': native counter and three channel ramps for both drivers. Full active CPU/CPSR/two MiB RAM and unchanged modeled MMIO match original ARM; independent complete RAM oracle for every field and explicit carry/ADDS vectors. No external ARM callbacks; no timing/synthesis/playback proof.'}
(P/('analysis/arm_sound_ramps_connected_verification.json' if connected else 'analysis/arm_sound_ramps_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'counter/ramp comparisons,',len(budgets),'budgets and',len(flag_cases),'carry/addition flag vectors passed')
