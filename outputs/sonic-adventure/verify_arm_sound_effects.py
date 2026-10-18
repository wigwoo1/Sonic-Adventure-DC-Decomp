"""Original-byte comparisons for five effects, with independent RAM/MMIO checks."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
connected='--connected' in sys.argv
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if connected else 'work/sonic-adventure/build/arm_sound_effects_native.dll')
library=ctypes.CDLL(str(dll));fn=library.sa_arm_sound_boot_native if connected else library.sa_arm_sound_effect_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write]+([Service] if connected else [])+[U,U];fn.restype=ctypes.c_int
entries=(0x684c,0x68f8,0x69a8,0x6a5c,0x6bd4)
values=(0,1,0x3fff,0x4000,0x7fff,0x8000,0x8001,0xbfff,0xc000,0xffff,0x7fffffff,0x80000000,0xffffffff)
cases=[];budgets=[];faults=[]
def run(driver,fill,flags,kind,pattern,budget=20000,fault=None):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    original=bytearray([fill])*SIZE;original[:len(image)]=image
    def literal(a):return struct.unpack_from('<I',original,a+driver*8)[0]
    def get(a,n=4):return int.from_bytes(original[a:a+n],'little')
    def set_initial(a,v,n=4):original[a:a+n]=(v&((1<<(n*8))-1)).to_bytes(n,'little')
    for i in range(8):
        s=0xc000+i*0x80
        for off in (1,7):set_initial(s+off,(i*41+pattern*29)&255,1)
        for off in (5,0xb,0x17):set_initial(s+off,255 if (pattern+i)%3 else 0,1)
        set_initial(s+0x10,0x8000+(i-4)*16)
        if kind==0:
            set_initial(s+0xc,values[(pattern+i)%len(values)]);set_initial(s+0x10,values[(pattern+i+3)%len(values)])
            set_initial(s+0x14,0 if pattern%3==0 else 1,1);set_initial(s+0x15,i%2,1);set_initial(s+0x16,(pattern*31+i)&255,1)
    for i in range(24):
        s=0x11000+i*0x58
        set_initial(s,(i+fill)&255,1)
        for offset in (0x40,0x4c):
            set_initial(s+offset,values[(pattern+i)%len(values)]);set_initial(s+offset+4,values[(pattern+i+3)%len(values)])
            set_initial(s+offset+8,0 if pattern%3==0 else 1,1);set_initial(s+offset+9,i%2,1);set_initial(s+offset+10,(pattern*31+i)&255,1)
        set_initial(s+0x4b,0 if (pattern+i)%3==0 else 255,1)
    # Valid tone pointer and bounded pitch inputs, exercising the recovered
    # original pitch helper rather than delegating it to an ARM callback.
    for i in range(48):
        s=0xe800+i*0x40
        set_initial(s,0 if pattern%3==0 else (1 if i%2 else 0xc0));set_initial(s+4,i%8,1)
        set_initial(s+6,60,1);set_initial(s+8,(pattern*37+i*11)&255,1);set_initial(s+9,(pattern*53+i*7)&255,1)
        set_initial(s+0xc,0);set_initial(s+0x10,0x1a000);set_initial(s+0x14,0);set_initial(s+0x38,i%3)
    set_initial(0x1a026,60,1);set_initial(0x1a027,0,1)
    native=bytearray(original);hardware=bytearray((i*17+fill)&255 for i in range(MMIO_SIZE));nhardware=bytearray(hardware)
    initial=State();initial.r[:]=[0x12340000+i for i in range(16)]
    entry=entries[kind]+driver*8;initial.r[13]=0x1fe000;initial.r[14]=STOP;initial.r[15]=entry;initial.cpsr=flags|0x13
    expected_events=[];native_events=[];snapshots=[];reference_count=[0];native_count=[0]
    machine=cpu(original,hardware,initial,expected_events)
    if fault:
        direction,ordinal=fault
        def before_bus(uc,access,address,size,value,user):
            if ('read' if access==unicorn.UC_MEM_READ else 'write')!=direction:return
            reference_count[0]+=1
            if reference_count[0]==ordinal:
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
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('read',hex(address)));return -9
        if fail('read'):return -11
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('write',hex(address)));return -9
        if fail('write'):return -11
        value&=(1<<(size*8))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    args=[view,ctypes.byref(actual),driver,None,read,write]+([Service()] if connected else [])+[STOP,budget]
    rc=fn(*args)
    if fault:
        assert snapshots,'actual retail bus fault not reached'
        state,ram,hw,n=snapshots[0]
        assert rc==-11 and bytes(actual)==state and native==ram and nhardware==hw,(driver,fault,'fault state')
        assert native_events==expected_events[:n]
    else:
        completed=expected.r[15]==STOP
        assert rc==(0 if completed else -3) and not errors,(driver,kind,pattern,budget,rc,errors)
        assert bytes(actual)==bytes(expected),(driver,kind,pattern,budget,'CPU')
        assert native==machine.mem_read(0,SIZE) and nhardware==machine.mem_read(MMIO,MMIO_SIZE),(driver,kind,pattern,'memory')
        assert native_events==expected_events
        if completed and kind<3:
            oracle=bytearray(original)
            def put(a,v,n=4):oracle[a:a+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
            count=8 if kind==0 else 24;base=0xc000 if kind==0 else 0x11000;stride=0x80 if kind==0 else 0x58
            offset=(0xc,0x40,0x4c)[kind];flag_offset=0x28 if kind==0 else 0;mask=(0x20,8,0x20)[kind]
            for i in range(count):
                s=base+i*stride;target=get(s+offset);current=get(s+offset+4);state=get(s+flag_offset,1)
                if target==current:put(s+flag_offset,state&~mask,1);continue
                delay=get(s+offset+9,1)
                if delay:put(s+offset+9,delay-1,1);continue
                reload=get(s+offset+8,1);put(s+offset+9,reload,1)
                if not reload:current=target
                else:
                    step=get(s+offset+10,1)*2
                    if current<target:
                        result=(current+step)&0xffffffff;current=min(target,result)
                    else:
                        result=(current-step)&0xffffffff
                        # Two original routines use MOVCS after SUBS, unlike
                        # the third's MOVCC. Preserve their unusual behavior.
                        if (current>=step if kind<2 else current<step):result=0
                        current=target if target>=result else result
                put(s+offset+4,current);put(s+(0x17 if kind==0 else 0x4b),1 if kind==0 else 255,1);put(s+flag_offset,state|mask,1)
            assert native==oracle,(driver,kind,pattern,'independent ramp fields')
        if completed and kind==3:
            def converted(value):
                delta=(value-0x8000)&0xffffffff
                if delta&0x80000000:delta=((~delta)&0xffffffff)|0x8000
                octave=delta>>8;table=original[literal(0x6d7c)+((octave&127)+0x60)]
                if table==127:result=literal(0x6bd0)
                else:
                    index=((table&15)*128)|((delta>>1)&127);fraction=original[literal(0x6d80)+index]
                    fraction+=256*sum(index>=literal(a) for a in (0x6bbc,0x6bc0,0x6bc4))
                    result=((table&240)<<6)|fraction
                return (-result if octave&128 else result)&0xffffffff
            for i in range(24):
                s=0x11000+i*0x58
                if get(s+0x4b,1):
                    value=(converted(get(s+0x44))+converted(get(s+0x50)))&0xffffffff
                    assert int.from_bytes(native[s+0x10:s+0x14],'little')==value and native[s+0x4b]==0,'independent table conversion'
                else:assert native[s:s+0x58]==original[s:s+0x58],'inactive conversion preserved'
        if completed and kind==4:
            for i in range(48):
                s=0xe800+i*64;h=i*128;channel=0xc000+(i%8)*128
                if not get(s):continue
                assert native[s+0x3f]==(get(s+0x3f,1)+(1 if get(s,1)&0xc0 else 0))&255
                assert int.from_bytes(native[s+0x38:s+0x3c],'little')==max(0,get(s+0x38)-1)
                if get(channel+5,1):
                    volume=max(0,min(255,get(s+8,1)+get(channel+1,1)-128))
                    expected_volume=(int.from_bytes(hardware[h+0x28:h+0x2c],'little')&31)|((255-volume)<<8)
                    assert int.from_bytes(nhardware[h+0x28:h+0x2c],'little')==expected_volume,'independent volume saturation'
                if get(channel+0xb,1):
                    pan=max(0,get(s+9,1)+get(channel+7,1)-128)
                    # Original tests bit 0x80, not an unsigned >127 bound;
                    # constructed sums above 255 therefore take another path.
                    if pan&128:pan=127
                    encoded=(pan>>2)-16
                    if encoded<0:encoded=(encoded^255)+16
                    expected_pan=(int.from_bytes(hardware[h+0x24:h+0x28],'little')&0xff00)|(encoded&31)
                    assert int.from_bytes(nhardware[h+0x24:h+0x28],'little')==expected_pan,'independent pan encoding'
                if get(s+0x38)==1:
                    expected_key=int.from_bytes(hardware[h:h+4],'little')|0xc000
                    assert int.from_bytes(nhardware[h:h+4],'little')==expected_key,'independent delayed key-off'
            for i in range(8):
                for offset in (5,0xb,0x17):assert native[0xc000+i*128+offset]==0,'channel markers cleared'
    return {'driver':driver,'fill':fill,'flags':hex(flags),'kind':kind,'pattern':pattern,'budget':budget,'fault':fault,'mmio_events':len(native_events)}
for params in itertools.product(range(2),(0,0x55),(0,0x20000000,0x90000000,0xf0000000),range(5),range(len(values))):cases.append(run(*params))
for driver,kind,budget in itertools.product(range(2),range(5),(1,4,16,64,256,512)):
    budgets.append(run(driver,0x55,0xf0000000,kind,7,budget))
for driver,direction,ordinal in itertools.product(range(2),('read','write'),(1,2,8)):
    faults.append(run(driver,0x55,0xf0000000,4,7,fault=(direction,ordinal)))
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_effects.c',P/'source/sa_arm_sound_effects_fixture.c',P/'source/sa_arm_sound_commands.c',P/'source/arm_sound_memory.h',P/'create_arm_sound_effects.py',P/'create_arm_sound_controls.py',P/'source/sa_arm_sound_snapshot_fixture.c',P/'build_arm_sound_effects.ps1',P/'analysis/arm_sound_effects_translation.json']
report={'cases_passed':len(cases),'budget_cases_passed':len(budgets),'mmio_failure_cases_passed':len(faults),'cases':cases,'budgets':budgets,'faults':faults,
 'connected':connected,'native_service_callback':False,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':('Connected driver' if connected else 'Standalone fixture')+': five native effects per driver plus recovered pitch helper. Complete active CPU/CPSR/two MiB RAM/modeled MMIO/ordered bus comparisons; independent ramp, conversion, volume, pan, delayed key-off and marker assertions. No external ARM service callbacks; no physical timing/synthesis/playback proof.'}
(P/('analysis/arm_sound_effects_connected_verification.json' if connected else 'analysis/arm_sound_effects_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'effect comparisons,',len(budgets),'budgets and',len(faults),'retail MMIO failures passed')
