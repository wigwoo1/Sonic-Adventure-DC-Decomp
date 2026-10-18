"""Original cleanup caller and abort routine, both native, with device fixtures only."""
import ctypes
import hashlib
import json
from pathlib import Path
import verify_request_abort_8c6013d2 as abort

PROJECT,ROOT=abort.PROJECT,abort.ROOT
dll=ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll'
lib=ctypes.CDLL(str(dll))
entry=lib.sa_stream_cleanup_abort_native
entry.argtypes=[abort.P,ctypes.POINTER(abort.State),ctypes.c_void_p,
                abort.Read,abort.Dispatch,abort.Observe,ctypes.c_uint32]
entry.restype=ctypes.c_int
abort.connected_native=entry


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    cases=[abort.run_case(i*len(abort.SCENARIOS)+j,alias,name,cleanup=True,native_finalizer=True,native_cancellation=True,native_backend=backend,native_lower=lower)
           for backend,lower in ((False,False),(True,False),(True,True))
           for i,alias in enumerate((0x8C000000,0xAC000000))
           for j,name in enumerate(abort.SCENARIOS)]
    sources=['sa_stream_cleanup_abort_native.c','sa_sound_stream_pending_cleanup_8c65bf60.c',
             'sa_request_abort_8c6013d2.c','sa_request_finalize_8c600cf0.c','sa_request_status_8c6013a4.c',
             'sa_mmio_complement_sub_8c60260e.c','retail_scalar_helpers.c','sa_device_cancel_path.c','sa_file_backend_poll_state.c','sa_file_backend_poll.c',
             'sa_indirect_service_tail_wrappers_8c10d938.c','sa_callback_service_call_return_one_8c6452d6.c',
             'sa_file_completion_process.c','sa_file_completion_connected.c','sa_file_device_status.c']
    exercised={int(pc,16) for case in cases for pc in case['executed_instruction_addresses']}
    listing=PROJECT/'analysis/instructions/8c6013d2.tsv'
    indexed={int(line.split('\t')[0],16) for line in listing.read_text().splitlines()[1:]}
    assert exercised==indexed,'Connected scenarios must exercise every abort body instruction'
    # Bridge rejects unavailable services without any state or memory change.
    ram=bytearray(abort.SIZE)
    state=abort.State();state.pc=0x8C65BF60
    before=bytes(state)
    view=(ctypes.c_uint8*abort.SIZE).from_buffer(ram)
    assert entry(view,ctypes.byref(state),None,abort.Read(),abort.Dispatch(),abort.Observe(),100)==-2
    assert bytes(state)==before and not any(ram)

    # A nested budget/service failure must escape cleanup at the exact retail
    # boundary, preserving its partial frame and pending flag.
    initial_ram=bytearray(abort.SIZE)
    initial_ram[0x10000:0x10000+len(abort.binary)]=abort.binary
    record,request,driver,pump_table=0x8C780000,0x8C790000,0x8C7A0000,0x8C7C0000
    initial_ram[0x780002]=1
    for address,value in [(record+8,request),(request,driver),(driver+0x1C,request),
                          (driver+0x3C,pump_table),(pump_table+0x48,abort.PUMP)]:
        abort.put32(initial_ram,address,value)
    abort.put16(initial_ram,request+0x4C,1)
    abort.put16(initial_ram,request+0x50,2)
    initial=abort.State();initial.r[4]=record;initial.r[15]=0x8C7FF000
    initial.sr,initial.pr,initial.pc=0x12345678,0x8C74913C,0x8C65BF60

    class Timer(dict):
        def __init__(self):super().__init__({abort.MMIO:0});self.reads=[]
        def __getitem__(self,address):
            assert address==abort.MMIO and len(self.reads)<10
            value=(~(0x1000+len(self.reads)*1000))&0xFFFFFFFF
            self.reads.append((address,value))
            return value

    @abort.Dispatch
    def fail_device(memory,state,target,context):return -7
    for budget,result,stop in [(1,-3,abort.ENTRY+2),(100000,-7,abort.PUMP)]:
        actual_ram,actual=bytearray(initial_ram),abort.State.from_buffer_copy(initial)
        expected_ram,expected=bytearray(initial_ram),abort.State.from_buffer_copy(initial)
        expected_timer,actual_timer=Timer(),Timer()
        abort.reference(expected_ram,expected,mmio=expected_timer,stop_pc=stop)
        @abort.Read
        def read_guard(context,address):return actual_timer[address]
        view=(ctypes.c_uint8*abort.SIZE).from_buffer(actual_ram)
        assert entry(view,ctypes.byref(actual),None,read_guard,fail_device,abort.Observe(),budget)==result
        assert actual_ram==expected_ram and bytes(actual)==bytes(expected)
        assert actual_timer.reads==expected_timer.reads
        assert actual_ram[0x780002]==1, 'Cleanup must leave work pending on host failure'

    # A malformed runtime retargeting can create a tail-call cycle. Compare
    # the bounded native stop with the stable state at the second retail entry.
    loop_ram=bytearray(initial_ram)
    abort.put32(loop_ram,request+0x40,1)
    abort.put32(loop_ram,driver+0xC,0x8C66D240)
    abort.put32(loop_ram,0x8C6451F8,0x8C6451A4)
    expected_ram,expected=bytearray(loop_ram),abort.State.from_buffer_copy(initial)
    actual_ram,actual=bytearray(loop_ram),abort.State.from_buffer_copy(initial)
    expected_timer,actual_timer=Timer(),Timer()
    visits=[]
    class AtBoundary(Exception):pass
    def cycle_trace(pc,word,registers,sr,pr,fpu):
        if pc==0x8C6451A4:
            visits.append(pc)
            if len(visits)==2:
                expected.r[:]=registers;expected.sr,expected.pr,expected.pc=sr,pr,pc
                raise AtBoundary()
    try:
        abort.reference(expected_ram,expected,mmio=expected_timer,trace=cycle_trace,instruction_budget=10000)
    except AtBoundary:pass
    assert len(visits)==2
    @abort.Read
    def read_cycle(context,address):return actual_timer[address]
    view=(ctypes.c_uint8*abort.SIZE).from_buffer(actual_ram)
    assert entry(view,ctypes.byref(actual),None,read_cycle,fail_device,abort.Observe(),128)==-3
    assert actual_ram==expected_ram and bytes(actual)==bytes(expected)
    assert actual_timer.reads==expected_timer.reads and actual_ram[0x780002]==1
    # A failing device call inside the native backend retains its saved PR frame.
    guard_ram=bytearray(initial_ram)
    for address,value in [(pump_table+0x48,0x8C6005C2),(0x8C673414,driver),
                          (driver+12,0x8C7B0000),(0x8C7B0040,abort.PUMP)]:
        abort.put32(guard_ram,address,value)
    actual_ram,actual=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    expected_ram,expected=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    actual_timer,expected_timer=Timer(),Timer()
    abort.reference(expected_ram,expected,mmio=expected_timer,stop_pc=abort.PUMP)
    @abort.Read
    def read_backend_guard(context,address):return actual_timer[address]
    view=(ctypes.c_uint8*abort.SIZE).from_buffer(actual_ram)
    assert entry(view,ctypes.byref(actual),None,read_backend_guard,fail_device,abort.Observe(),100000)==-7
    assert actual_ram==expected_ram and bytes(actual)==bytes(expected)
    assert actual_timer.reads==expected_timer.reads and actual_ram[0x780002]==1
    # Failure after both lower wrappers must retain both pending return frames.
    abort.put32(guard_ram,0x8C7B0040,0x8C6452D6)
    abort.put32(guard_ram,0x8C0000BC,abort.PUMP)
    actual_ram,actual=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    expected_ram,expected=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    actual_timer,expected_timer=Timer(),Timer()
    abort.reference(expected_ram,expected,mmio=expected_timer,stop_pc=abort.PUMP)
    view=(ctypes.c_uint8*abort.SIZE).from_buffer(actual_ram)
    assert entry(view,ctypes.byref(actual),None,read_backend_guard,fail_device,abort.Observe(),100000)==-7
    assert actual_ram==expected_ram and bytes(actual)==bytes(expected)
    assert actual.r[6]==0 and actual.r[7]==2
    assert actual_timer.reads==expected_timer.reads and actual_ram[0x780002]==1
    # Failure at the completion status callback retains the completion frame.
    abort.put32(guard_ram,0x8C7B0010,0x8C700040)
    actual_ram,actual=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    expected_ram,expected=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    actual_timer,expected_timer=Timer(),Timer()
    def successful_pump(memory,registers,sr):registers[0]=1;return sr
    abort.reference(expected_ram,expected,mmio=expected_timer,
                    hooks={abort.PUMP:successful_pump},stop_pc=0x8C700040)
    @abort.Dispatch
    def completion_failure(memory,state,target,context):
        if target==abort.PUMP:
            state.contents.r[0]=1;state.contents.pc=state.contents.pr;return 0
        assert target==0x8C700040
        return -7
    view=(ctypes.c_uint8*abort.SIZE).from_buffer(actual_ram)
    assert entry(view,ctypes.byref(actual),None,read_backend_guard,completion_failure,abort.Observe(),100000)==-7
    assert actual_ram==expected_ram and bytes(actual)==bytes(expected)
    assert actual_timer.reads==expected_timer.reads and actual_ram[0x780002]==1
    # Deepest failure: formatter -> real operation-1 tail -> runtime vector.
    abort.put32(guard_ram,0x8C7B0010,0x8C6450EA)
    actual_ram,actual=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    expected_ram,expected=bytearray(guard_ram),abort.State.from_buffer_copy(initial)
    actual_timer,expected_timer=Timer(),Timer()
    class StatusBoundary(Exception):pass
    def status_vector(memory,registers,sr):
        if registers[7]==1:
            expected.r[:]=registers;expected.sr,expected.pc=sr,abort.PUMP
            raise StatusBoundary()
        registers[0]=1
        return sr
    try:
        abort.reference(expected_ram,expected,mmio=expected_timer,
                        hooks={abort.PUMP:status_vector})
    except StatusBoundary:pass
    @abort.Dispatch
    def status_failure(memory,state,target,context):
        if target==abort.PUMP and state.contents.r[7]==2:
            state.contents.r[0]=1;state.contents.pc=state.contents.pr;return 0
        assert target==abort.PUMP and state.contents.r[7]==1
        return -7
    view=(ctypes.c_uint8*abort.SIZE).from_buffer(actual_ram)
    assert entry(view,ctypes.byref(actual),None,read_backend_guard,status_failure,abort.Observe(),100000)==-7
    assert actual_ram==expected_ram and bytes(actual)==bytes(expected)
    assert actual.r[6]==0 and actual.r[7]==1
    assert actual_timer.reads==expected_timer.reads and actual_ram[0x780002]==1
    report={'module':'1ST_READ.BIN','cases_passed':len(cases),
            'service_entries_compared':sum(c['service_entries'] for c in cases),
            'timer_reads_compared':sum(c['timer_reads'] for c in cases),
            'abort_body_instructions_exercised':len(exercised),
            'host_guard_cases_passed':8,
            'native_completion_calls_compared':sum(c['completion_calls'] for c in cases),
            'binary_sha256':hashlib.sha256(abort.binary).hexdigest(),
            'sources':{f'source/{source}':digest(PROJECT/'source'/source) for source in sources},
            'dll_sha256':digest(dll),'verifier_sha256':digest(Path(__file__)),
            'fixture_harness_sha256':digest(PROJECT/'verify_request_abort_8c6013d2.py'),
            'interpreter_sha256':digest(PROJECT/'verify_startup.py'),
            'comparison_level':'Full 16 MiB RAM and GPR/SR/PR/PC at return and every outer/nested service entry; exact shared timer reads and device-call order.',
            'scope':'Native cleanup -> native abort -> native finalizer/status, clock/subtraction, including readiness waits, timeout, owner/mode changes, cancellation interrupt masking/restoration and elapsed-time accounting.',
            'limitations':['Thirty baseline cases fixture the backend pump. Thirty more execute backend polling with pump/status fixtures. The final thirty execute both lower pump wrappers plus the device-status formatter and operation-1 tail. Both backend groups execute native completion with status 0x43 (raw status 4 in the deepest chain). External operation-1/2/8 runtime-vector targets remain fixtures.',
                           'This verifies an original nested caller chain in controlled scenarios, not hardware interrupts, complete device implementations or whole-game execution.'],
            'cases':cases}
    path=PROJECT/'analysis/stream_cleanup_abort_native_verification.json'
    path.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f'{len(cases)} connected cleanup/abort cases, {report["service_entries_compared"]} service entries '
          f'and {report["timer_reads_compared"]} timer reads passed')


if __name__=='__main__':main()
