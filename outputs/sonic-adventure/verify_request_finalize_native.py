"""Native finalizer and status helper; only the device pump is a fixture."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs/sonic-adventure'
FINAL,STATUS,CLOCK,SUB=0x8C600CF0,0x8C6013A4,0x8C60260E,0x8C602618
PUMP,MMIO,SIZE=0x8C700000,0xFFD8000C,16*1024*1024
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll'
lib=ctypes.CDLL(str(dll))
P=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32)
Dispatch=ctypes.CFUNCTYPE(ctypes.c_int,P,ctypes.POINTER(State),ctypes.c_uint32,ctypes.c_void_p)
Observe=ctypes.CFUNCTYPE(None,P,ctypes.POINTER(State))
native=lib.sa_request_finalize_native
native.argtypes=[P,ctypes.POINTER(State),ctypes.c_void_p,Read,Dispatch,Observe,ctypes.c_uint32]
native.restype=ctypes.c_int
status_native=lib.sa_request_status_8c6013a4
status_native.argtypes=[P,ctypes.POINTER(State),Dispatch,ctypes.c_void_p,ctypes.c_uint32]
status_native.restype=ctypes.c_int


def put32(ram,address,value):struct.pack_into('<I',ram,address&0xFFFFFF,value&0xFFFFFFFF)
def put16(ram,address,value):struct.pack_into('<H',ram,address&0xFFFFFF,value&0xFFFF)
def snapshot(ram,state):return bytes(state),hashlib.sha256(ram).hexdigest()
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run_case(case,alias,name):
    rng=random.Random(case)
    request,driver,table=alias|0x790000,alias|0x7A0000,alias|0x7C0000
    ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
    ram[0x7FE000:0x7FF000]=rng.randbytes(0x1000)
    put32(ram,request,driver);put32(ram,driver+0x3C,table);put32(ram,table+0x48,PUMP)
    mode={'zero':0,'success':1,'error5':5,'error40':0x40,'error_positive':5,
          'unknown':7,'wait3':3,'wait4':4}.get(name,2)
    put16(ram,request+0x4C,0 if name=='inactive' else 0xFFFF)
    put16(ram,request+0x50,mode)
    put16(ram,request+0x52,0x8000 if name=='error40' else (123 if name=='error_positive' else 0xFFE7))
    put32(ram,request+0x18,7)
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[4],initial.r[15]=request,alias|0x7FF000
    initial.sr,initial.pr,initial.pc=rng.getrandbits(32),0x8C74913C,FINAL
    expected_ram,expected=bytearray(ram),State.from_buffer_copy(initial)
    actual_ram,actual=bytearray(ram),State.from_buffer_copy(initial)
    expected_entries,actual_entries,executed,errors=[],[],set(),[]
    step=20000000 if name=='timeout' else (10000000 if name=='progress_reset' else 1000)
    base=0xFFFFF800 if name=='timer_wrap' else 0x1000

    class Services(dict):
        def __init__(self,memory):super().__init__({MMIO:0});self.memory,self.reads,self.pumps=memory,[],0
        def __getitem__(self,address):
            assert address==MMIO and len(self.reads)<100
            value=(~(base+len(self.reads)*step))&0xFFFFFFFF
            self.reads.append((address,value));return value
        def pump(self,registers,sr):
            self.pumps+=1
            if name in ('wait2','wait3','wait4','progress_reset','timer_wrap'):
                if name=='progress_reset' and self.pumps<=2:
                    put32(self.memory,request+0x18,7+self.pumps)
                if self.pumps>=3:put16(self.memory,request+0x50,1)
            registers[0]=0x12345678
            put32(self.memory,request+0x54,self.pumps)
            return sr
    expected_services,actual_services=Services(expected_ram),Services(actual_ram)
    def pump_hook(memory,registers,sr):
        entry=State();entry.r[:]=registers;entry.sr,entry.pr,entry.pc=sr,expected.pr,PUMP
        expected_entries.append(snapshot(memory,entry))
        return expected_services.pump(registers,sr)
    def trace(pc,word,registers,sr,pr,fpu):
        if FINAL<=pc<0x8C600D82:executed.add(pc)
        if pc in (CLOCK,SUB,STATUS):
            entry=State();entry.r[:]=registers;entry.sr,entry.pr,entry.pc=sr,pr,pc
            expected_entries.append(snapshot(expected_ram,entry))
    reference(expected_ram,expected,hooks={PUMP:pump_hook},mmio=expected_services,trace=trace,instruction_budget=100000)
    @Read
    def read_register(context,address):
        try:return actual_services[address]
        except Exception as error:errors.append(repr(error));return 0
    @Dispatch
    def device(memory,state_pointer,target,context):
        try:
            assert target==PUMP
            state=state_pointer.contents
            state.sr=actual_services.pump(state.r,state.sr);state.pc=state.pr
            return 0
        except Exception as error:errors.append(repr(error));return -9
    @Observe
    def observe(memory,state_pointer):
        view=(ctypes.c_uint8*SIZE).from_address(ctypes.addressof(memory.contents))
        actual_entries.append(snapshot(view,state_pointer.contents))
    view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
    assert native(view,ctypes.byref(actual),None,read_register,device,observe,100000)==0,(name,errors)
    assert not errors,(name,errors)
    assert actual_entries==expected_entries,f'{name}: service entries differ'
    assert actual_services.reads==expected_services.reads,f'{name}: timer reads differ'
    assert actual_ram==expected_ram,f'{name}: RAM differs'
    assert bytes(actual)==bytes(expected),f'{name}: CPU differs'
    return {'case':case,'alias':hex(alias),'scenario':name,'result':hex(actual.r[0]),
            'service_entries':len(actual_entries),'timer_reads':len(actual_services.reads),
            'pump_calls':actual_services.pumps,'progress':struct.unpack_from('<I',actual_ram,0x790018)[0],
            'executed_instructions':[hex(pc) for pc in sorted(executed)],'matched':True}


def status_cases():
    rng=random.Random(STATUS);cases=[];executed=set()
    @Dispatch
    def unexpected(memory,state,target,context):return -9
    for case in range(64):
        ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
        request=0 if case%8==0 else (0x8C790000 if case%2 else 0xAC790000)
        active=(0,1,0xFFFF,0x8000)[(case//2)%4]
        mode=(0,1,2,3,4,5,0x40,0x7FFF,0x8000,0xFFFF)[case%10]
        if request:put16(ram,request+0x4C,active);put16(ram,request+0x50,mode)
        initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
        initial.r[4]=request;initial.sr,initial.pr,initial.pc=rng.getrandbits(32),0x8C74913C,STATUS
        expected_ram,expected=bytearray(ram),State.from_buffer_copy(initial)
        reference(expected_ram,expected,trace=lambda pc,*args:executed.add(pc))
        actual_ram,actual=bytearray(ram),State.from_buffer_copy(initial)
        view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
        assert status_native(view,ctypes.byref(actual),unexpected,None,100)==0
        assert actual_ram==expected_ram and bytes(actual)==bytes(expected),case
        cases.append({'case':case,'mode':mode,'active':active,'request':hex(request),'matched':True})
    return cases,executed


def main():
    names=('zero','success','error5','error40','error_positive','unknown','inactive',
           'wait2','wait3','wait4','timeout','progress_reset','timer_wrap')
    cases=[run_case(i*len(names)+j,alias,name) for i,alias in enumerate((0x8C000000,0xAC000000)) for j,name in enumerate(names)]
    status,executed_status=status_cases()
    exercised={int(pc,16) for case in cases for pc in case['executed_instructions']}
    routines=[]
    for address,end,symbol,seen in [(FINAL,0x8C600D82,'sa_request_finalize_8c600cf0',exercised),
                                    (STATUS,0x8C6013BC,'sa_request_status_8c6013a4',executed_status)]:
        listing=PROJECT/f'analysis/instructions/{address:08x}.tsv'
        indexed={int(line.split('\t')[0],16) for line in listing.read_text().splitlines()[1:]}
        assert indexed==seen,f'{symbol}: untested instructions {[hex(pc) for pc in indexed-seen]}'
        source=f'source/{symbol}.c'
        routines.append({'name':symbol,'address':hex(address),'end_exclusive':hex(end),'instruction_bytes':len(indexed)*2,
                         'instructions_exercised':len(seen),'source':source,'source_sha256':digest(PROJECT/source),
                         'retail_span_sha256':hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest(),
                         'listing_sha256':digest(listing)})
    sources=['sa_stream_cleanup_abort_native.c','sa_sound_stream_pending_cleanup_8c65bf60.c','sa_request_abort_8c6013d2.c',
             'sa_request_finalize_8c600cf0.c','sa_request_status_8c6013a4.c','sa_mmio_complement_sub_8c60260e.c','retail_scalar_helpers.c','sa_device_cancel_path.c','sa_file_backend_poll_state.c',
             'sa_indirect_service_tail_wrappers_8c10d938.c','sa_file_completion_process.c','sa_file_completion_connected.c','sa_file_device_status.c']
    report={'module':'1ST_READ.BIN','binary_sha256':hashlib.sha256(binary).hexdigest(),
            'finalizer_cases_passed':len(cases),'status_cases_passed':len(status),
            'service_entries_compared':sum(c['service_entries'] for c in cases),
            'timer_reads_compared':sum(c['timer_reads'] for c in cases),'routines':routines,
            'sources':{f'source/{s}':digest(PROJECT/'source'/s) for s in sources},
            'dll_sha256':digest(dll),'verifier_sha256':digest(Path(__file__)),
            'generator_sha256':digest(PROJECT/'recover_request_abort_8c6013d2.py'),
            'interpreter_sha256':digest(PROJECT/'verify_startup.py'),
            'comparison_level':'Full 16 MiB RAM and GPR/SR/PR/PC at return and native service entries; exact ordered timer reads.',
            'scope':'Native finalizer, progress-sensitive timeout reset, signed completion/error results and native request-status helper.',
            'limitations':['Device pumping remains an explicit fixture; clock, subtraction and status execute natively.',
                           'Static annotated instruction-level C preserves behavior; original high-level source, hardware/concurrency and whole-game execution remain unverified.'],
            'cases':cases,'status_cases':status}
    (PROJECT/'analysis/request_finalize_native_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(cases)} finalizer cases, {len(status)} status cases, {report["service_entries_compared"]} service entries '
          f'and {report["timer_reads_compared"]} timer reads passed')


if __name__=='__main__':main()
