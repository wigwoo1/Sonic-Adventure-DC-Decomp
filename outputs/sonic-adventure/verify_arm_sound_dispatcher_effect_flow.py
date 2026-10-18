"""Persistent dispatcher plus stream/sequencer/effects overlay integration."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,Service,U,U];fn.restype=ctypes.c_int
dll=ROOT/('work/sonic-adventure/build/arm_sound_boot_native.dll' if '--connected' in sys.argv else 'work/sonic-adventure/build/arm_sound_dispatcher_native.dll')
fn=ctypes.CDLL(str(dll)).sa_arm_sound_boot_native
fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,Service,U,U];fn.restype=ctypes.c_int
flows=[];checkpoints=0
for driver,fill,descending in itertools.product(range(2),(0,0x55),(False,True)):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    reference=bytearray([fill])*SIZE;reference[:len(image)]=image
    def literal(a):return struct.unpack_from('<I',reference,a+driver*8)[0]
    def put(a,v,n=4):reference[a:a+n]=(v&((1<<(n*8))-1)).to_bytes(n,'little')
    counter=literal(0x663c);put(counter,0xfffffff8);targets=[]
    for i in range(8):
        s=0xc000+i*128;target=96+i;current=target+(32 if descending else -32)
        for offset in (0,6):reference[s+offset:s+offset+5]=bytes((target,current,1,i%2,8))
        word_target=0x8000+i*32;word_current=word_target+(32 if descending else -32)
        put(s+0x18,word_target);put(s+0x1c,word_current);reference[s+0x20:s+0x23]=bytes((1,i%2,1))
        put(s+0xc,word_target);put(s+0x10,word_current);reference[s+0x14:s+0x17]=bytes((1,i%2,8))
        targets.append((target,word_target))
    for i in range(24):
        s=0x11000+i*0x58;put(s,1,1);put(s+3,i%16,1);put(s+4,0x20000+i*256)
        put(s+0x2c,1);put(s+0x34,1);put(s+0x38,0x1000)
        for offset in (0x40,0x4c):
            target=0x8000+i*8;put(s+offset,target);put(s+offset+4,target+(32 if descending else -32))
            reference[s+offset+8:s+offset+11]=bytes((1,i%2,8))
        put(s+0x4b,0,1)
    table=literal(0x70f8);stream_base=literal(0x70fc)
    for i in range(16):
        put(table+i*12,1);put(table+i*12+4,stream_base+(i%24)*0x58);put(table+i*12+8,MMIO+(48+i)*128)
    for i in range(24):
        record=stream_base+i*0x58;put(record,0x15,1);put(record+3,i%16,1);put(record+0xc,0x2000)
        for offset,value in ((0x14,1),(0x15,8),(0x16,1),(0x17,8),(0x18,96),(0x19,128),(0x1c,1),(0x1d,1),(0x1e,64),(0x1f,64),(0x24,128),(0x28,16),(0x27,i%3)):
            put(record+offset,value,1)
        put(record+0x38,2)
    for i in range(48):
        s=0xe800+i*64;put(s,1 if i%2 else 0xc0);put(s+4,i%8,1);put(s+6,60,1)
        put(s+8,(i*11)&255,1);put(s+9,(i*7)&255,1)
        put(s+0xc,0);put(s+0x10,0x1a000);put(s+0x14,0);put(s+0x38,2)
    put(0x1a026,60,1);put(0x1a027,0,1)
    control=literal(0x48c4);messages=literal(0x48c8);pending=literal(0x48cc)
    reference[0x10000:pending+0x240]=bytes(pending+0x240-0x10000)
    put(control+4,0x10000);put(control+8,messages);put(control+12,messages)
    reference[0xb000:0xc000]=bytes(0x1000);put(0x13000,0);put(literal(0x48d4),0)
    put(0x10000,0x100);put(0x10004,0x1a020);put(0x10008,16);put(0x1000c,1)
    put(0x10010,0x1a000);put(0x10014,0x1a000);put(0x10034,16);put(0x10038,2);put(0x1003c,1)
    reference[0x1a000:0x1a020]=bytes([0,60,64,1,4]+[0x83]*27)
    put(literal(0x4b30),0);put(literal(0x4b34),0) # Initialize command-history ring controls.
    put(0x14200,0) # Deliberately absent DSP bank for opcode 0x84.
    native=bytearray(reference);hardware=bytearray([fill])*MMIO_SIZE
    struct.pack_into('<II',hardware,literal(0x73a8)-MMIO+0x10,0,1)
    nhardware=bytearray(hardware)
    expected=State();expected.r[:]=[0x12340000+i for i in range(16)]
    expected.r[13]=0x1fe000;expected.r[15]=0x1d0;expected.cpsr=0xf0000013
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
        if tick:expected.r[15]=actual.r[15]=0x1d0
        source=struct.unpack_from('<I',reference,0x4b2c+driver*8)[0]
        reference[0x13400]=native[0x13400]=1
        packet=bytes((0x7f if tick%2 else 0x84,))+bytes(511)
        reference[source:source+512]=packet;native[source:source+512]=packet
        for site in range(0x1d0,0x214,4):
            assert expected.r[15]==actual.r[15]==site
            stop=site+4
            machine=cpu(reference,hardware,expected,expected_events)
            try:machine.emu_start(site,stop,count=20000)
            except Exception as exc:raise RuntimeError((driver,fill,descending,tick,hex(site),hex(machine.reg_read(arm.UC_ARM_REG_PC)),[hex(machine.reg_read(arm.UC_ARM_REG_R0+i)) for i in range(13)])) from exc
            expected=state_of(machine);assert expected.r[15]==stop
            reference[:]=machine.mem_read(0,SIZE);hardware[:]=machine.mem_read(MMIO,MMIO_SIZE)
            rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),stop,20000)
            assert rc==0 and not errors,(driver,fill,descending,tick,hex(site),rc,errors)
            assert bytes(actual)==bytes(expected) and native==reference and nhardware==hardware,(driver,fill,descending,tick,hex(site),'state')
            assert native_events==expected_events,'ordered persistent bus effects';checkpoints+=1
    assert struct.unpack_from('<I',native,counter)[0]==1,'dispatcher consumes shared counter before one tick increment'
    for i,(target,word_target) in enumerate(targets):
        s=0xc000+i*128
        assert native[s+1]==native[s+7]==target,'byte ramp convergence'
        assert struct.unpack_from('<I',native,s+0x1c)[0]==word_target,'word ramp convergence'
        for offset in (5,0xb,0x17):assert native[s+offset]==0,'applied markers cleared'
    assert any(e[0]=='write' for e in native_events),'hardware updates occurred'
    assert struct.unpack_from('<I',native,0x13000)[0]!=0 and any(native[0xb000:0xc000]),'sequencer produced native ring commands'
    flows.append({'driver':driver,'fill':fill,'descending':descending,'ticks':16,'checkpoints':272,'mmio_events':len(native_events)})
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_dispatcher_fixture.c',P/'source/sa_arm_sound_sequencer.c',P/'source/sa_arm_sound_stream_update.c',P/'source/sa_arm_sound_effects.c',P/'source/sa_arm_sound_commands.c',P/'source/sa_arm_sound_ramps.c',P/'source/sa_arm_sound_snapshot.c',P/'source/arm_sound_memory.h',P/'build_arm_sound_boot_native.ps1']
import re
paths += [P/'build_arm_sound_dispatcher.ps1']+[P/'source'/name for name in re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_dispatcher.ps1').read_text())]
paths += [P/'source/sa_arm_sound_boot_native.c',P/'build_arm_sound_boot_native.ps1'] if '--connected' in sys.argv else []
report={'flow_cases_passed':len(flows),'tick_cycles_passed':128,'batch_checkpoints_passed':checkpoints,'flows':flows,
 'sha256':{str(path):sha(path) for path in paths},
 'connected':'--connected' in sys.argv,
 'scope':'Dispatcher overlay with alternating invalid opcode 0x7f and missing DSP bank opcode 0x84. Constructed mailbox injection per tick. Contiguous original main-loop segment 0x1d0..0x214, seventeen native calls per cycle with stream, sequencer and channel effects. Sixteen cycles per fixture, no sequencer gap and no ARM callback. Exact active CPU/CPSR/full RAM/modeled MMIO/ordered effects at each original return. Independent convergence, markers, dispatcher counter consumption and nonempty sequencer command-ring assertions. Constructed note sequence; outer scheduling/positive command families/queue-consumption/interrupt/timing/synthesis/playback remain outside proof.'}
(P/('analysis/arm_sound_dispatcher_effect_flow_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_dispatcher_effect_flow_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(flows),'contiguous dispatcher/stream/sequencer/effect flows, 128 cycles and',checkpoints,'checkpoints passed')
