"""Persistent native FIQ input/timer events followed by original main-loop work."""
from pathlib import Path
P=Path(__file__).resolve().parent
support=P/'verify_arm_sound_reset_services.py'
exec(compile(support.read_text().split('for driver,fill,flags,entry_index,pair in ')[0],str(support),'exec'))
from arm_legacy_alignment_reference import install
class Banks(ctypes.Structure):_fields_=[('spsr',U),('svc_r8_r14',U*7),('exception_r8_r14',U*7),('returned',U)]
dll=ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll';library=ctypes.CDLL(str(dll))
fn=library.sa_arm_sound_boot_native;fn.argtypes=[BP,ctypes.POINTER(State),U,ctypes.c_void_p,Read,Write,Service,U,U];fn.restype=ctypes.c_int
exception=library.sa_arm_sound_exception_native;exception.argtypes=[BP,ctypes.POINTER(State),ctypes.POINTER(Banks),U,ctypes.c_void_p,Read,Write,U,U];exception.restype=ctypes.c_int
flows=[];checkpoints=0;corrections=0
for driver,split in itertools.product(range(2),(False,True)):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes();reference=bytearray(SIZE);reference[:len(image)]=image
    hardware=bytearray(MMIO_SIZE);native=bytearray(reference);nhardware=bytearray(hardware)
    expected=State();expected.cpsr=0x13;actual=State.from_buffer_copy(expected)
    expected_events=[];native_events=[];pending=[None];input_index=[0]
    port=struct.unpack_from('<I',reference,0x1814+driver*8)[0]
    @Read
    def read(user,address,size,out):
        assert MMIO<=address<=MMIO+MMIO_SIZE-size
        if address==port and pending[0] is not None:
            value=pending[0][min(input_index[0],len(pending[0])-1)];input_index[0]+=1
            nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little')
        else:value=int.from_bytes(nhardware[address-MMIO:address-MMIO+size],'little')
        out[0]=value;native_events.append(('read',address,size,value));return 0
    @Write
    def write(user,address,value,size):
        assert MMIO<=address<=MMIO+MMIO_SIZE-size
        value&=(1<<(8*size))-1;nhardware[address-MMIO:address-MMIO+size]=value.to_bytes(size,'little');native_events.append(('write',address,size,value));return 0
    view=(ctypes.c_uint8*SIZE).from_buffer(native)
    machine=cpu(reference,hardware,expected,expected_events);machine.emu_start(0,0x194,count=2000000)
    expected=state_of(machine);reference[:]=machine.mem_read(0,SIZE);hardware[:]=machine.mem_read(MMIO,MMIO_SIZE)
    assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),0x194,2000000)==0
    assert bytes(actual)==bytes(expected) and native==reference and nhardware==hardware and native_events==expected_events,'native reset'
    for memory in (reference,native):
        struct.pack_into('<I',memory,0x320,2);struct.pack_into('<I',memory,0x230,0xfffffffe)
    cause_port=struct.unpack_from('<I',reference,0x900)[0];phase_checkpoints=[];fiq_bank=[0x99990000+i for i in range(7)]
    start_ring=struct.unpack_from('<I',native,0x13000)[0];produced=0;consumed=0;full_ticks=0;control=0xc400+7*0x480;wanted_volume=native[control+8];wanted_pan=native[control+0xb]
    for cycle in range(16):
        value=(cycle*13)&127
        data=([0xb0,7] if cycle%2==0 else [value,10,64]) if split else [0xb0,7,value,10,64]
        for cause,payload in ((5,data),(2,[])):
            for hw in (hardware,nhardware):struct.pack_into('<I',hw,cause_port-MMIO,cause)
            svc=State.from_buffer_copy(actual);saved_svc=list(svc.r[8:15]);saved_flags=svc.cpsr
            pending[0]=list(payload)+[0x100];input_index[0]=0;ref_input=[0]
            def supply(uc,address,size,user):
                if address==0x159c+driver*12:
                    word=pending[0][min(ref_input[0],len(pending[0])-1)];ref_input[0]+=1;uc.mem_write(port,word.to_bytes(4,'little'))
            machine=cpu(reference,hardware,expected,expected_events,supply)
            exception_cpsr=(expected.cpsr&~0x3f)|0xd1
            machine.reg_write(arm.UC_ARM_REG_CPSR,exception_cpsr)
            for reg,word in zip(registers[8:15],fiq_bank):machine.reg_write(reg,word)
            machine.reg_write(arm.UC_ARM_REG_LR,expected.r[15]+4);machine.reg_write(arm.UC_ARM_REG_PC,0x1c);machine.reg_write(arm.UC_ARM_REG_SPSR,expected.cpsr)
            machine.emu_start(0x1c,expected.r[15],count=100000);expected=state_of(machine)
            machine.reg_write(arm.UC_ARM_REG_CPSR,(expected.cpsr&~31)|0x11)
            next_bank=[machine.reg_read(reg) for reg in registers[8:15]];machine.reg_write(arm.UC_ARM_REG_CPSR,expected.cpsr)
            context=Banks();context.spsr=saved_flags;context.svc_r8_r14[:]=saved_svc
            actual.r[8:15]=fiq_bank;actual.r[14]=svc.r[15]+4;actual.r[15]=0x1c;actual.cpsr=exception_cpsr
            assert exception(view,ctypes.byref(actual),ctypes.byref(context),driver,None,read,write,svc.r[15],100000)==0
            reference[:]=machine.mem_read(0,SIZE);hardware[:]=machine.mem_read(MMIO,MMIO_SIZE)
            assert bytes(actual)==bytes(expected) and native==reference and nhardware==hardware and native_events==expected_events,(driver,split,cycle,cause,'FIQ state/events')
            assert context.returned==1 and list(context.exception_r8_r14)==next_bank and list(context.svc_r8_r14)==saved_svc,'bank retention'
            assert bytes(actual)==bytes(svc),'FIQ preserves interrupted SVC registers/status'
            fiq_bank=next_bank;pending[0]=None;checkpoints+=1
            if cause==5:
                end=struct.unpack_from('<I',native,0x13000)[0]
                # Split cycles: even has a header/first byte; odd completes it
                # and emits another control, so only odd cycles produce two.
                count=0 if split and cycle%2==0 else 2
                assert ((end-start_ring)&0xfff)==count*4,'input command production'
                produced+=count;start_ring=end
                if count:wanted_volume=value;wanted_pan=64
        if native[0x22c]:full_ticks+=1
        seen=[False]
        def boundary(uc,address,size,user):
            if address==0x194:
                if seen[0]:uc.emu_stop()
                else:seen[0]=True
        machine=cpu(reference,hardware,expected,expected_events,boundary)
        adapted=install(machine,unicorn,arm,expected_events,MMIO,MMIO_SIZE)
        machine.emu_start(0x194,STOP,count=2000000);expected=state_of(machine);assert expected.r[15]==0x194
        assert fn(view,ctypes.byref(actual),driver,None,read,write,Service(),0x194,2000000)==0
        reference[:]=machine.mem_read(0,SIZE);hardware[:]=machine.mem_read(MMIO,MMIO_SIZE)
        assert bytes(actual)==bytes(expected) and native==reference and nhardware==hardware and native_events==expected_events,(driver,split,cycle,'main loop')
        assert struct.unpack_from('<I',native,0x13004)[0]==start_ring,'main loop consumes produced compact commands'
        assert native[control+8]==wanted_volume and native[control+0xb]==wanted_pan,'input controls update volume/pan state'
        consumed=produced;corrections+=len(adapted);checkpoints+=1
        phase_checkpoints.append({'cycle':cycle,'input_commands_total':produced,'full_ticks_so_far':full_ticks,'events':len(native_events)})
    assert produced==(16 if split else 32) and consumed==produced and full_ticks==3
    assert struct.unpack_from('<I',native,0x230)[0]==30,'two interrupts per cycle and counter wrap'
    flows.append({'driver':driver,'split_input':split,'cycles':16,'checkpoints':phase_checkpoints,'commands_produced_and_consumed':produced,'full_tick_iterations':full_ticks})
import re
paths=[Path(__file__),support,dll,P/'arm_legacy_alignment_reference.py',P/'source/arm_sound_memory.h',P/'source/arm_sound_exception.h',P/'build_arm_sound_boot_native.ps1']+[P/'source'/n for n in re.findall(r"'(sa_arm_sound[^']+\.c)'",(P/'build_arm_sound_boot_native.ps1').read_text())]
report={'flow_cases_passed':len(flows),'cycles_passed':64,'phase_checkpoints_passed':checkpoints,'commands_produced_and_consumed':sum(f['commands_produced_and_consumed'] for f in flows),'reference_alignment_corrections':corrections,'flows':flows,'sha256':{str(p):sha(p) for p in paths},'scope':'Main DLL exports share one RAM/MMIO state across original reset, repeated explicitly supplied FIQ input/timer events and complete main-loop iterations. SVC state preserved, FIQ bank retained, partial/running input, produced commands consumed. Exact CPU/RAM/MMIO/ordered effects against original ARM TI925T plus legacy alignment adapter. Event entry and delivery cadence are fixture-supplied; no automatic interrupt scheduler, physical timing, sample synthesis/audio output or full-game proof.'}
(P/'analysis/arm_sound_interrupt_main_flow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(flows),'FIQ/input/timer/main flows, 64 cycles,',checkpoints,'phase checkpoints and',report['commands_produced_and_consumed'],'produced/consumed commands passed')
