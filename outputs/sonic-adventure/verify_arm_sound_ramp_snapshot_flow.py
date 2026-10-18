"""Persistent native ramps -> snapshot -> counter flow, no ARM service callback."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,Service,U,U]
fn.restype=ctypes.c_int
flows=[];checkpoints=0
for driver,fill,descending in itertools.product(range(2),(0,0x55),(False,True)):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    reference=bytearray([fill])*SIZE;reference[:len(image)]=image
    def literal(a):return struct.unpack_from('<I',reference,a+8*driver)[0]
    counter=literal(0x663c);struct.pack_into('<I',reference,counter,0xfffffff8)
    targets=[]
    for i in range(8):
        s=0xc000+i*0x80;target=96+i;current=target+(32 if descending else -32)
        for offset in (0,6):reference[s+offset:s+offset+5]=bytes((target,current,1,i%2,8))
        word_target=0x8000+i*32;word_current=word_target+(32 if descending else -32)
        struct.pack_into('<II',reference,s+0x18,word_target,word_current)
        reference[s+0x20:s+0x23]=bytes((1,i%2,1));targets.append((target,word_target))
    native=bytearray(reference);hardware=bytearray([fill])*MMIO_SIZE;nhardware=bytearray(hardware)
    expected=State();expected.r[:]=[0x12340000+i for i in range(16)];expected.r[13]=0x1fe000;expected.cpsr=0xf0000013
    actual=State.from_buffer_copy(expected);view=(ctypes.c_uint8*SIZE).from_buffer(native)
    expected_events=[];native_events=[]
    @Read
    def read(user,address,size,value):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('read',address));return -9
        value[0]=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        native_events.append(('read',address,size,value[0]));return 0
    @Write
    def write(user,address,value,size):
        if not MMIO<=address<=MMIO+MMIO_SIZE-size:errors.append(('write',address));return -9
        value&=(1<<(size*8))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        native_events.append(('write',address,size,value));return 0
    for tick in range(16):
        for base in (0x6780,0x6640,0x66e0,0x63f8,0x6620):
            entry=base+driver*8
            expected.r[14]=actual.r[14]=STOP;expected.r[15]=actual.r[15]=entry
            machine=cpu(reference,hardware,expected,expected_events);machine.emu_start(entry,STOP,count=10000)
            expected=state_of(machine);assert expected.r[15]==STOP
            reference[:]=machine.mem_read(0,SIZE);hardware[:]=machine.mem_read(MMIO,MMIO_SIZE)
            rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),STOP,10000)
            assert rc==0 and not errors,(driver,fill,descending,tick,hex(base),rc,errors)
            assert bytes(actual)==bytes(expected) and native==reference and nhardware==hardware,(driver,fill,descending,tick,hex(base),'state')
            assert native_events==expected_events,'ordered persistent bus effects'
            checkpoints+=1
    assert struct.unpack_from('<I',native,counter)[0]==8,'independent counter wrap over 16 ticks'
    target1=literal(0x6638)
    for i,(target,word_target) in enumerate(targets):
        s=0xc000+i*0x80
        assert native[s+1]==native[s+7]==target,'independent byte ramp convergence'
        assert struct.unpack_from('<I',native,s+0x1c)[0]==word_target,'independent word ramp convergence'
        assert native[target1+i*32+1]==(target-128)&255,'independent snapshot after convergence'
    flows.append({'driver':driver,'fill':fill,'descending':descending,'ticks':16,'checkpoints':80,'mmio_events':len(native_events)})
paths=[Path(__file__),support,ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll',P/'source/sa_arm_sound_boot_native.c',P/'source/sa_arm_sound_ramps.c',P/'source/sa_arm_sound_snapshot.c',P/'source/arm_sound_memory.h',P/'build_arm_sound_boot_native.ps1']
report={'flow_cases_passed':len(flows),'tick_cycles_passed':128,'batch_checkpoints_passed':checkpoints,'flows':flows,
 'sha256':{str(path):sha(path) for path in paths},
 'scope':'Persistent native counter, three ramps and snapshot, sixteen cycles per fixture with null ARM service callback; exact active CPU/CPSR/full RAM/modeled MMIO/ordered events after every routine. Independent convergence/snapshot/counter-wrap checks. Caller invokes recovered routines in explicit order; this is not a complete main-loop, interrupt, timing, synthesis or playback proof.'}
(P/'analysis/arm_sound_ramp_snapshot_flow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(flows),'persistent ramp/snapshot flows, 128 ticks and',checkpoints,'checkpoints passed')
