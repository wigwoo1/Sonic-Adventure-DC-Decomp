"""Retail cleanup chain vs native game code, sharing explicit host disc policies."""
import ctypes,hashlib,itertools,json,shlex,struct
from pathlib import Path
from verify_request_abort_8c6013d2 import ROOT,PROJECT,State,P,Read,Observe,SIZE,MMIO,binary,reference,put32,put16,snapshot
dll=ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll';lib=ctypes.CDLL(str(dll))
IO=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,P,ctypes.c_uint32)
U=ctypes.c_uint32
lib.sa_disc_queue_size.restype=U
init=lib.sa_disc_queue_init;init.argtypes=[ctypes.c_void_p,IO,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,U,U]
service=lib.sa_disc_service;service.argtypes=[ctypes.c_void_p,P,U,U,U,ctypes.POINTER(U)]
entry=lib.sa_stream_cleanup_disc_native
entry.argtypes=[P,ctypes.POINTER(State),ctypes.c_void_p,U,ctypes.c_void_p,Read,Observe,U]
manifest=json.loads((PROJECT/'disc_inventory.json').read_text())
track=next(t for t in reversed([shlex.split(s) for s in manifest['gdi'].splitlines()[1:] if s.strip()]) if t[2]=='4')
path=Path(manifest['source'])/track[4];base,stride,offset=int(track[1]),int(track[3]),int(track[5])
VECTOR=0x8c740000
observed={0x8c6013d2,0x8c60260e,0x8c602618,0x8c600cf0,0x8c6013a4,0x8c5ffa12,
          0x8c6005c2,0x8c6451a4,0x8c10d9d8,0x8c6452d6,0x8c10d960,0x8c6450ea,0x8c10d94c}
results=[]
with path.open('rb') as stream:
    for alias,scenario in itertools.product((0x8c000000,0xac000000),('complete','cancel','partial_cancel','io_error','precomplete','missing')):
        initial_ram=bytearray(SIZE);initial_ram[0x10000:0x10000+len(binary)]=binary
        record,request,driver=alias+0x780000,alias+0x790000,alias+0x7a0000
        for address,value in [(0x8c0000bc,VECTOR),(0x8c673414,driver),(record+8,request),
            (request,driver),(request+0x3c,99 if scenario=='missing' else 1),
            (request+0x40,1 if 'cancel' in scenario else 0),(request+0x20,6144),
            (driver+0x1c,request),(driver+12,0x8c66d240),(driver+0x3c,0x8c66c4b4)]:put32(initial_ram,address,value)
        initial_ram[0x780002]=1
        for address,value in [(request+0x48,1),(request+0x4c,1),(request+0x50,2)]:put16(initial_ram,address,value)
        put32(initial_ram,0x8c67c1f0,0xabcdef01)
        struct.pack_into('<4I',initial_ram,0x770000,base+16+150,3,alias+0x800000,0)
        initial_ram[0x800000:0x801800]=b'\xaa'*6144
        expected_ram,actual_ram=bytearray(initial_ram),bytearray(initial_ram)
        io_calls=[[],[]];io_errors=[]
        def io(index):
            @IO
            def read(user,pos,out,size):
                try:
                    io_calls[index].append((pos,size))
                    if scenario=='io_error':return -17
                    stream.seek(pos);data=stream.read(size)
                    if len(data)!=size:return -8
                    ctypes.memmove(out,data,size);return 0
                except BaseException as exc:io_errors.append(repr(exc));return -8
            return read
        readers=[io(0),io(1)];views=[(ctypes.c_uint8*SIZE).from_buffer(m) for m in (expected_ram,actual_ram)]
        queues=[ctypes.create_string_buffer(lib.sa_disc_queue_size()) for _ in range(2)]
        for index in range(2):
            assert init(queues[index],readers[index],None,offset,(path.stat().st_size-offset)//stride,base,stride)==0
            value=U();assert service(queues[index],views[index],0,16,alias+0x770000,ctypes.byref(value))==0 and value.value==1
            for _ in range(3 if scenario=='precomplete' else 1 if scenario=='partial_cancel' else 0):
                assert service(queues[index],views[index],2,0,0,ctypes.byref(value))==0
        initial=State();initial.r[:]=[0x22340000+i for i in range(16)]
        initial.r[4]=record;initial.r[15]=alias+0x7ff000
        initial.sr=0x12345678;initial.pr=0x8c74913c;initial.pc=0x8c65bf60
        expected,actual=State.from_buffer_copy(initial),State.from_buffer_copy(initial)
        logs=[[],[]];operations=[[],[]]
        class Timer(dict):
            def __init__(self):super().__init__({MMIO:0});self.reads=[]
            def __getitem__(self,address):
                assert address==MMIO and len(self.reads)<200
                value=(~(0x1000+1000*len(self.reads)))&0xffffffff
                self.reads.append(value);return value
        timers=[Timer(),Timer()]
        def trace(pc,word,r,sr,pr,fpu):
            if pc in observed:
                s=State();s.r[:]=r;s.sr,s.pr,s.pc=sr,pr,pc;logs[0].append(snapshot(expected_ram,s))
        def hook(memory,r,sr):
            s=State();s.r[:]=r;s.sr,s.pr,s.pc=sr,expected.pr,VECTOR
            logs[0].append(snapshot(memory,s));operations[0].append(r[7])
            assert r[6]==0
            value=U();assert service(queues[0],views[0],r[7],r[4],r[5],ctypes.byref(value))==0
            r[0]=value.value;return sr
        reference(expected_ram,expected,hooks={VECTOR:hook},mmio=timers[0],trace=trace,instruction_budget=500000)
        @Read
        def timer(user,address):
            assert user==123
            return timers[1][address]
        @Observe
        def observe(memory,state):
            logs[1].append(snapshot(actual_ram,state.contents))
            if state.contents.pc==VECTOR:operations[1].append(state.contents.r[7])
        assert entry(views[1],ctypes.byref(actual),queues[1],VECTOR,123,timer,observe,500000)==0
        assert actual_ram==expected_ram and bytes(actual)==bytes(expected),scenario
        if logs[0]!=logs[1]:
            mismatch=next((i for i,(left,right) in enumerate(zip(*logs)) if left!=right),min(map(len,logs)))
            def describe(log):
                return [(hex(State.from_buffer_copy(s).pc),digest) for s,digest in log[max(0,mismatch-1):mismatch+2]]
            raise AssertionError((scenario,'call boundary',list(map(len,logs)),mismatch,describe(logs[0]),describe(logs[1])))
        assert operations[0]==operations[1]
        assert timers[0].reads==timers[1].reads and io_calls[0]==io_calls[1] and not io_errors
        delivered=bytearray()
        if scenario!='io_error':
            for position,size in io_calls[1]:stream.seek(position);delivered.extend(stream.read(size))
        assert actual_ram[0x800000:0x800000+len(delivered)]==delivered
        assert actual_ram[0x800000+len(delivered):0x801800]==b'\xaa'*(6144-len(delivered))
        assert actual_ram[0x780002]==0 and actual_ram[0x780001]==1
        mode=struct.unpack_from('<H',actual_ram,0x790050)[0]
        if scenario in ('cancel','partial_cancel'):assert mode==5 and struct.unpack_from('<h',actual_ram,0x790052)[0]==-27
        elif scenario=='io_error':
            # A terminal state reached during readiness waiting is cleared by
            # retail abort at 8c60142e; its error detail survives.
            assert mode==0 and struct.unpack_from('<h',actual_ram,0x790052)[0]==-26
        elif scenario in ('complete','precomplete'):
            # Retail abort clears the terminal request mode at 8c60142e.
            assert mode==0 and struct.unpack_from('<I',actual_ram,0x790018)[0]==6144
        results.append({'alias':hex(alias),'scenario':scenario,'service_entries':len(logs[1]),'timer_reads':len(timers[1].reads),'operations':operations[1],'io_calls':len(io_calls[1]),'final_mode':mode})
    # Reject missing platform services atomically, and foreign runtime vectors
    # at the exact retail boundary without clearing pending cleanup work.
    guard_ram=bytearray(initial_ram);guard=State.from_buffer_copy(initial)
    view=(ctypes.c_uint8*SIZE).from_buffer(guard_ram);before=bytes(guard)
    assert entry(view,ctypes.byref(guard),None,VECTOR,123,timer,Observe(),100000)==-2
    assert bytes(guard)==before and guard_ram==initial_ram
    expected_ram=bytearray(initial_ram);expected=State.from_buffer_copy(initial)
    timers=[Timer(),Timer()]
    reference(expected_ram,expected,mmio=timers[0],stop_pc=VECTOR)
    guard_queue=ctypes.create_string_buffer(lib.sa_disc_queue_size())
    assert init(guard_queue,readers[1],None,offset,32,base,stride)==0
    assert entry(view,ctypes.byref(guard),guard_queue,VECTOR+4,123,timer,Observe(),100000)==-8
    assert guard_ram==expected_ram and bytes(guard)==bytes(expected)
    assert timers[0].reads==timers[1].reads and guard_ram[0x780002]==1
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
sources=('sa_file_close.c','native_disc_queue.c','native_disc_queue.h','native_disc_sector_reader.c','native_disc_read_command.c','native_disc_service_bridge.c',
 'sa_stream_cleanup_abort_native.c','sa_sound_stream_pending_cleanup_8c65bf60.c','sa_request_abort_8c6013d2.c','sa_mmio_complement_sub_8c60260e.c','retail_scalar_helpers.c',
 'sa_request_finalize_8c600cf0.c','sa_request_status_8c6013a4.c','sa_device_cancel_path.c','sa_file_backend_poll_state.c',
 'sa_indirect_service_tail_wrappers_8c10d938.c','sa_file_completion_process.c','sa_file_completion_connected.c','sa_file_device_status.c')
report={'cases_passed':len(results),'service_entries_compared':sum(c['service_entries'] for c in results),'host_guard_cases_passed':2,'cases':results,
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in sources},'dll_sha256':sha(dll),'binary_sha256':hashlib.sha256(binary).hexdigest(),
 'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'scope':'Retail SH4 game chain vs native game chain, both connected to independent native host queues and actual disc bytes. Full RAM/GPR/SR/PR/PC at every service entry/return, matching I/O, timers and operation order.',
 'limitations':'Both paths share the declared native platform service policy, not original firmware. Only prepared request contexts, six scenarios, two RAM aliases and read commands; no full startup/hardware/whole-game proof.'}
(PROJECT/'analysis/stream_cleanup_disc_native_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(results),'native-disc cleanup scenarios and',report['service_entries_compared'],'service boundaries passed')
