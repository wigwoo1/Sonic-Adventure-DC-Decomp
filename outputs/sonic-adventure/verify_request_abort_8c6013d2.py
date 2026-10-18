"""Exercise native abort wait loops/interrupt restoration with explicit device fixtures."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path
from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs/sonic-adventure'
ENTRY, END = 0x8C6013D2, 0x8C601500
CLOCK, SUBTRACT, FINALIZE = 0x8C60260E, 0x8C602618, 0x8C600CF0
PUMP, CANCEL, MMIO = 0x8C700000, 0x8C700020, 0xFFD8000C
SIZE = 16 * 1024 * 1024
SOURCES = ['sa_request_abort_8c6013d2.c','sa_mmio_complement_sub_8c60260e.c','retail_scalar_helpers.c']
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll = ROOT / 'work/sonic-adventure/build/request_abort_8c6013d2.dll'
P = ctypes.POINTER(ctypes.c_uint8)
Dispatch = ctypes.CFUNCTYPE(ctypes.c_int, P, ctypes.POINTER(State), ctypes.c_uint32, ctypes.c_void_p)
Read = ctypes.CFUNCTYPE(ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32)
Observe = ctypes.CFUNCTYPE(None, P, ctypes.POINTER(State))
native = clock = subtract = connected_native = None
SCENARIOS = ('null','inactive','nonwaiting','owner_mismatch2','owner_mismatch4',
             'ready','wait','multi_wait','mode4_wait','timeout','mode_changed_by_pump',
             'owner_changed','mode_changed','finalizer_error','timer_wrap')


def load_native():
    global native, clock, subtract
    library = ctypes.CDLL(str(dll))
    native = library.sa_request_abort_8c6013d2
    native.argtypes = [P,ctypes.POINTER(State),Dispatch,ctypes.c_void_p,ctypes.c_uint32]
    native.restype = ctypes.c_int
    clock = library.sa_mmio_complement_sub_8c60260e
    clock.argtypes = [P,ctypes.POINTER(State),ctypes.c_void_p,Read]
    clock.restype = None
    subtract = library.sa_subtract_arguments_8c602618
    subtract.argtypes = [P,ctypes.POINTER(State)]
    subtract.restype = None


def put32(ram, address, value):
    struct.pack_into('<I',ram,address & 0xFFFFFF,value & 0xFFFFFFFF)


def put16(ram, address, value):
    struct.pack_into('<H',ram,address & 0xFFFFFF,value & 0xFFFF)


def snapshot(ram,state):
    return bytes(state), hashlib.sha256(ram).hexdigest()


def run_case(number, alias, scenario, cleanup=False, native_finalizer=False, native_cancellation=False, native_backend=False, native_lower=False):
    if not cleanup and native is None: load_native()
    rng = random.Random(number)
    request, driver = alias | 0x790000, alias | 0x7A0000
    cancel_table, pump_table = alias | 0x7B0000, alias | 0x7C0000
    if native_cancellation:cancel_table=0x8C66D240
    ram = bytearray(SIZE)
    ram[0x10000:0x10000+len(binary)] = binary
    ram[0x7FE000:0x7FF000] = rng.randbytes(0x1000)
    put32(ram,request,driver)
    put16(ram,request+0x4C,0 if scenario=='inactive' else 0xFFFF)
    mode = 4 if scenario in ('mode4_wait','owner_mismatch4') else 2
    if scenario=='nonwaiting': mode=3
    put16(ram,request+0x50,mode)
    put32(ram,request+0x40,1 if scenario in ('ready','owner_changed','mode_changed','finalizer_error') else 0)
    put32(ram,request+0x3C,0xABCDEF01)
    put32(ram,driver+0x1C,0 if scenario.startswith('owner_mismatch') else request)
    put32(ram,driver+0xC,cancel_table)
    put32(ram,driver+0x3C,pump_table)
    put32(ram,cancel_table+0x14,0x8C6451A4 if native_cancellation else CANCEL)
    if native_cancellation:put32(ram,0x8C0000BC,CANCEL)
    put32(ram,pump_table+0x48,PUMP)
    if native_backend:
        put32(ram,pump_table+0x48,0x8C6005C2)
        put32(ram,0x8C673414,driver)
        put32(ram,cancel_table+0x40,PUMP)
        put32(ram,cancel_table+0x10,0x8C700040)
        if native_lower:
            put32(ram,cancel_table+0x40,0x8C6452D6)
            put32(ram,cancel_table+0x10,0x8C6450EA)
    record = alias | 0x780000
    if cleanup:
        ram[0x780000:0x780064] = rng.randbytes(100)
        ram[0x780002] = 1
        put32(ram,record+8,0 if scenario=='null' else request)
        put32(ram,0x8C67C1F0,rng.getrandbits(32))
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[4] = record if cleanup else (0 if scenario=='null' else request)
    state.r[15] = alias | 0x7FF000
    state.sr,state.pr,state.pc = rng.getrandbits(32),0x8C74913C,(0x8C65BF60 if cleanup else ENTRY)
    expected_ram, expected = bytearray(ram), State.from_buffer_copy(state)
    actual_ram, actual = bytearray(ram), State.from_buffer_copy(state)
    expected_entries, actual_entries, errors = [],[],[]
    executed = set()
    step = 20000000 if scenario=='timeout' else 1000
    base = 0xFFFFF800 if scenario=='timer_wrap' else 0x1000

    class Services(dict):
        def __init__(self,memory):
            super().__init__({MMIO:0})
            self.memory,self.reads,self.pumps,self.calls = memory,[],0,[]
            self.status_calls=0
        def __getitem__(self,address):
            assert address==MMIO and len(self.reads)<100
            tick = (base+len(self.reads)*step)&0xFFFFFFFF
            self.reads.append((address,(~tick)&0xFFFFFFFF))
            if len(self.reads)==3:
                if scenario=='owner_changed':put32(self.memory,driver+0x1C,0)
                if scenario=='mode_changed':put16(self.memory,request+0x50,1)
            return (~tick)&0xFFFFFFFF
        def external(self,target,registers,sr):
            self.calls.append(target)
            if target==PUMP or (native_lower and target==CANCEL and registers[7]==2):
                if native_lower:assert registers[6]==0 and registers[7]==2
                self.pumps+=1
                if scenario=='mode_changed_by_pump':put16(self.memory,request+0x50,1)
                elif self.pumps >= (3 if scenario=='multi_wait' else 1):put32(self.memory,request+0x40,1)
                registers[0]=0x13579BDF
            elif native_lower and target==CANCEL and registers[7]==1:
                self.status_calls+=1
                assert registers[6]==0 and registers[4]==0xABCDEF01
                for i in range(4):put32(self.memory,registers[5]+4*i,0)
                registers[0]=4  # Retail formatter translates this to 0x43.
            elif target==0x8C700040 and native_backend:
                self.status_calls+=1
                assert registers[4]==0xABCDEF01
                put32(self.memory,registers[5],0)
                put32(self.memory,registers[5]+4,0)
                put32(self.memory,registers[5]+8,0)
                registers[0]=0x43
            elif target==CANCEL:
                assert registers[4]==0xABCDEF01
                if native_cancellation:assert registers[6]==0 and registers[7]==8
                assert sr & 0xF0 == 0xF0, 'Cancellation must run with interrupts masked'
                put32(self.memory,request+0x54,registers[4])
                if native_finalizer:
                    put16(self.memory,request+0x50,5 if scenario=='finalizer_error' else 1)
                    put16(self.memory,request+0x52,0xFFE7)
                registers[0]=0x2468ACE0
            elif target==FINALIZE:
                assert registers[4]==request
                put32(self.memory,request+0x58,0xDEADBEEF)
                registers[0]=0xFFFFFFE7 if scenario=='finalizer_error' else 0
            else:raise AssertionError(hex(target))
            return sr

    expected_services,actual_services = Services(expected_ram),Services(actual_ram)
    def make_hook(target):
        def hook(memory,registers,sr):
            entry=State();entry.r[:]=registers;entry.sr,entry.pr,entry.pc=sr,expected.pr,target
            expected_entries.append(snapshot(memory,entry))
            return expected_services.external(target,registers,sr)
        return hook
    def trace(pc,word,registers,sr,pr,fpu):
        if ENTRY <= pc < END:
            executed.add(pc)
        if pc in (CLOCK,SUBTRACT) or (cleanup and pc==ENTRY) or (native_finalizer and pc in (FINALIZE,0x8C6013A4)) or (native_cancellation and pc in (0x8C6451A4,0x8C10D9D8)) or (native_backend and pc in (0x8C6005C2,0x8C5FFA12)) or (native_lower and pc in (0x8C6452D6,0x8C10D960,0x8C6450EA,0x8C10D94C)):
            entry=State();entry.r[:]=registers;entry.sr,entry.pr,entry.pc=sr,pr,pc
            expected_entries.append(snapshot(expected_ram,entry))
    external_targets=(PUMP,CANCEL) if native_finalizer else (PUMP,CANCEL,FINALIZE)
    if native_backend:external_targets+= (0x8C700040,)
    reference(expected_ram,expected,hooks={target:make_hook(target) for target in external_targets},
              mmio=expected_services,trace=trace,instruction_budget=100000)

    @Read
    def read_register(context,address):
        try:return actual_services[address]
        except Exception as error:errors.append(repr(error));return 0
    @Dispatch
    def dispatch(memory_pointer,state_pointer,target,context):
        try:
            entry=state_pointer.contents
            memory=(ctypes.c_uint8*SIZE).from_address(ctypes.addressof(memory_pointer.contents))
            if not cleanup:
                actual_entries.append(snapshot(memory,entry))
            elif target in (CLOCK,SUBTRACT,FINALIZE,0x8C6013A4,0x8C6451A4,0x8C10D9D8):
                raise AssertionError('Connected native dispatch must handle clock/subtraction internally')
            if target==CLOCK:clock(memory_pointer,state_pointer,None,read_register)
            elif target==SUBTRACT:subtract(memory_pointer,state_pointer)
            else:
                entry.sr=actual_services.external(target,entry.r,entry.sr)
                entry.pc=entry.pr
            return 0
        except Exception as error:errors.append(repr(error));return -9
    view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
    @Observe
    def observe(memory_pointer,state_pointer):
        memory=(ctypes.c_uint8*SIZE).from_address(ctypes.addressof(memory_pointer.contents))
        actual_entries.append(snapshot(memory,state_pointer.contents))
    if cleanup:
        assert connected_native is not None
        result=connected_native(view,ctypes.byref(actual),None,read_register,dispatch,observe,100000)
    else:
        result=native(view,ctypes.byref(actual),dispatch,None,100000)
    assert result==0 and not errors,(scenario,result,errors)
    assert actual_entries==expected_entries,f'{scenario}: service entry differs'
    assert actual_services.reads==expected_services.reads,f'{scenario}: timer sequence differs'
    assert actual_services.calls==expected_services.calls
    assert actual_ram==expected_ram,f'{scenario}: full RAM differs'
    assert bytes(actual)==bytes(expected),f'{scenario}: CPU differs'
    if CANCEL in actual_services.calls:
        assert actual.sr & 0xF0 == state.sr & 0xF0, 'Interrupt mask not restored'
    return {'case':number,'alias':hex(alias),'scenario':scenario,'connected_cleanup':cleanup,
            'native_finalizer':native_finalizer,
            'native_cancellation':native_cancellation,
            'native_backend':native_backend,
            'native_lower':native_lower,
            'completion_calls':actual_services.status_calls,
            'result':hex(actual.r[0]),
            'timer_reads':len(actual_services.reads),'pump_calls':actual_services.pumps,
            'service_entries':len(actual_entries),'external_services':[hex(x) for x in actual_services.calls],
            'executed_instruction_addresses':[hex(pc) for pc in sorted(executed)],
            'matched':True}


def main():
    load_native()
    scenarios=SCENARIOS
    cases=[run_case(i*len(scenarios)+j,alias,name) for i,alias in enumerate((0x8C000000,0xAC000000))
           for j,name in enumerate(scenarios)]
    listing=PROJECT/'analysis/instructions/8c6013d2.tsv'
    indexed={int(line.split('\t')[0],16) for line in listing.read_text().splitlines()[1:]}
    executed={int(pc,16) for case in cases for pc in case['executed_instruction_addresses']}
    assert executed==indexed, f'Unexercised body instructions: {[hex(pc) for pc in sorted(indexed-executed)]}'

    # Host guard contracts: missing dispatcher, instruction budget, service failure.
    guard_ram=bytearray(SIZE);guard_ram[0x10000:0x10000+len(binary)]=binary
    request,driver=0x8C790000,0x8C7A0000
    put32(guard_ram,request,driver);put32(guard_ram,driver+0x1C,request)
    put16(guard_ram,request+0x4C,1);put16(guard_ram,request+0x50,2)
    initial=State();initial.r[4]=request;initial.r[15]=0x8C7FF000
    initial.pr,initial.pc=0x8C74913C,ENTRY
    @Dispatch
    def fail_service(memory,state,target,context): return -7
    for budget,handler,result,stop in [(1000,Dispatch(),-2,None),
                                        (1,fail_service,-3,ENTRY+2),
                                        (1000,fail_service,-7,CLOCK)]:
        actual_ram,actual=bytearray(guard_ram),State.from_buffer_copy(initial)
        expected_ram,expected=bytearray(guard_ram),State.from_buffer_copy(initial)
        if stop is not None: reference(expected_ram,expected,stop_pc=stop)
        view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
        assert native(view,ctypes.byref(actual),handler,None,budget)==result
        assert actual_ram==expected_ram and bytes(actual)==bytes(expected)
    report={'module':'1ST_READ.BIN','load_address':'0x8c010000','address':hex(ENTRY),
            'end_exclusive':hex(END),'indexed_instruction_bytes':300,
            'instruction_body_ranges':[{'start':hex(ENTRY),'end_exclusive':'0x8c601410'},
                                       {'start':'0x8c601412','end_exclusive':hex(END)}],
            'retail_span_sha256':hashlib.sha256(binary[ENTRY-0x8C010000:END-0x8C010000]).hexdigest(),
            'binary_sha256':hashlib.sha256(binary).hexdigest(),
            'source_sha256':hashlib.sha256((PROJECT/'source'/SOURCES[0]).read_bytes()).hexdigest(),
            'sources':{f'source/{s}':hashlib.sha256((PROJECT/'source'/s).read_bytes()).hexdigest() for s in SOURCES},
            'dll_sha256':hashlib.sha256(dll.read_bytes()).hexdigest(),
            'verifier_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
            'interpreter_sha256':hashlib.sha256((PROJECT/'verify_startup.py').read_bytes()).hexdigest(),
            'listing_sha256':hashlib.sha256((PROJECT/'analysis/instructions/8c6013d2.tsv').read_bytes()).hexdigest(),
            'generator_sha256':hashlib.sha256((PROJECT/'recover_request_abort_8c6013d2.py').read_bytes()).hexdigest(),
            'cases_passed':len(cases),'service_entries_compared':sum(c['service_entries'] for c in cases),
            'body_instructions_exercised':len(executed),'body_instructions_indexed':len(indexed),
            'host_guard_cases_passed':3,
            'timer_reads_compared':sum(c['timer_reads'] for c in cases),
            'comparison_level':'Full 16 MiB RAM and GPR/SR/PR/PC at return and all service entries, exact timer reads and device-service order.',
            'scope':'Native request-abort control flow, both wait loops, timeout exit, mode/owner changes, cancellation interrupt masking/restoration, and finalizer-result propagation.',
            'limitations':['Timer and subtraction run natively; device pump, device cancellation and 0x8c600cf0 finalizer remain explicit service fixtures, not recovered native implementations.',
                           'The generated source is static annotated instruction-level C with a host execution budget; it is not reconstructed original high-level source.',
                           'Hardware behavior, concurrent interrupt delivery and whole-game execution remain unverified.'],
            'cases':cases}
    (PROJECT/'analysis/request_abort_8c6013d2_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(cases)} abort scenarios, {report["service_entries_compared"]} service entries and '
          f'{report["timer_reads_compared"]} timer reads passed')


if __name__=='__main__':main()
