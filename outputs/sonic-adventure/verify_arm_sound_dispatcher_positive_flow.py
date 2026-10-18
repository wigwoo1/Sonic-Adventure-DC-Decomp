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
baseline=P/'verify_arm_sound_dispatcher.py'
exec(compile(baseline.read_text().split('special=lambda')[0],str(baseline),'exec'))
flows=[];checkpoints=0
for driver,slot,program in itertools.product(range(2),(0,7),(0,1)):
    prepared,hw=bootstrap(driver);reference=bytearray(prepared)
    def literal(a):return struct.unpack_from('<I',reference,a+driver*8)[0]
    def put(a,v,n=4):reference[a:a+n]=(v&((1<<(n*8))-1)).to_bytes(n,'little')
    bank=0x1c000;reference[bank:bank+0x800]=bytes(0x800)
    reference[bank:bank+4]=b'SOSB';put(bank+4,1);put(bank+8,0x800);put(bank+12,1)
    put(bank+16,0x40);put(bank+20,0x80);reference[bank+0x7fc:bank+0x800]=b'ENDB'
    for index,offset in enumerate((0x40,0x80)):
        record=bank+offset;reference[record:record+4]=b'SOSD';put(record+4,(0x400+index*0x40)<<16)
        for j,value in enumerate((0x00100020,0x00300040,0x00502000,0x00700080,0x009000a0,0x00b000c0,0x00d000e0,0x00f00100)):put(record+8+j*4,value)
        put(record+4+0x10,0xa5,1);put(record+4+0x12,index*31,1);put(record+4+0x15,index*255,1);put(record+4+0x24,9,1)
    put(literal(0x54e4),bank);put(literal(0x54e4)+4,0x800)
    target=0x11000+slot*0x58;put(target+0x18,0x80808080);put(target+0x1e,128,1);put(target+0x1f,128,1)
    native=bytearray(reference);hardware=bytearray(hw)
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
        packet=bytes((0x11,0,slot,0,program,64))+bytes(506)
        reference[source:source+512]=packet;native[source:source+512]=packet
        for site in range(0x1d0,0x214,4):
            assert expected.r[15]==actual.r[15]==site
            stop=site+4
            machine=cpu(reference,hardware,expected,expected_events)
            try:machine.emu_start(site,stop,count=20000)
            except Exception as exc:raise RuntimeError((driver,slot,program,tick,hex(site),hex(machine.reg_read(arm.UC_ARM_REG_PC)),[hex(machine.reg_read(arm.UC_ARM_REG_R0+i)) for i in range(13)])) from exc
            expected=state_of(machine);assert expected.r[15]==stop
            reference[:]=machine.mem_read(0,SIZE);hardware[:]=machine.mem_read(MMIO,MMIO_SIZE)
            rc=fn(view,ctypes.byref(actual),driver,None,read,write,Service(),stop,20000)
            assert rc==0 and not errors,(driver,slot,program,tick,hex(site),rc,errors)
            assert bytes(actual)==bytes(expected) and native==reference and nhardware==hardware,(driver,slot,program,tick,hex(site),'state')
            assert native_events==expected_events,'ordered persistent bus effects';checkpoints+=1
    assert native[target]&1,'successful stream remains active'
    assert struct.unpack_from('<I',native,target+0x30)[0]==bank+0x400+program*0x40,'selected sample retained'
    table=literal(0x77e4)
    assert struct.unpack_from('<I',native,table+4)[0]==target,'ownership retained over repeated starts'
    assert native[source]==0 and native[0x13400]==0,'mailbox consumed'
    flows.append({'driver':driver,'slot':slot,'program':program,'ticks':16,'checkpoints':272,'mmio_events':len(native_events)})
paths=[Path(__file__),support,dll,P/'source/sa_arm_sound_dispatcher_fixture.c',P/'source/sa_arm_sound_sequencer.c',P/'source/sa_arm_sound_stream_update.c',P/'source/sa_arm_sound_effects.c',P/'source/sa_arm_sound_commands.c',P/'source/sa_arm_sound_ramps.c',P/'source/sa_arm_sound_snapshot.c',P/'source/arm_sound_memory.h',P/'build_arm_sound_boot_native.ps1']
import re
paths += [baseline,P/'verify_arm_sound_reset_services.py',P/'build_arm_sound_dispatcher.ps1']+[P/'source'/name for name in re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_dispatcher.ps1').read_text())]
paths += [P/'source/sa_arm_sound_boot_native.c',P/'build_arm_sound_boot_native.ps1'] if '--connected' in sys.argv else []
report={'flow_cases_passed':len(flows),'tick_cycles_passed':128,'batch_checkpoints_passed':checkpoints,'flows':flows,
 'sha256':{str(path):sha(path) for path in paths},
 'connected':'--connected' in sys.argv,
 'scope':'Persistent constructed SOSB successful starts and equal-priority replacements through contiguous original main-loop segment 1d0..214, including stream, sequencer and effects, 16 cycles per fixture. Exact CPU/CPSR/full RAM/modeled MMIO/ordered events; null ARM callback. Independent active/sample/ownership/mailbox assertions. No physical timing, synthesis, audible output, complete outer scheduling or all command families proof.'}
(P/('analysis/arm_sound_dispatcher_positive_flow_connected_verification.json' if '--connected' in sys.argv else 'analysis/arm_sound_dispatcher_positive_flow_verification.json')).write_text(json.dumps(report,indent=2)+'\n')
print(len(flows),'positive SOSB dispatcher/stream/sequencer/effect flows, 128 cycles and',checkpoints,'checkpoints passed')
