"""Device formatter and real lower tail, with the runtime vector as fixture."""
import ctypes,hashlib,itertools,json,struct
from pathlib import Path
from verify_request_abort_8c6013d2 import ROOT,PROJECT,State,Dispatch,P,SIZE,put32,snapshot,reference,binary
dll=ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll'
lib=ctypes.CDLL(str(dll));fn=lib.sa_file_device_status_connected
fn.argtypes=[P,ctypes.POINTER(State),Dispatch,ctypes.c_void_p];fn.restype=ctypes.c_int
tail=lib.sa_indirect_service_tail_8c10d94c;tail.argtypes=[P,ctypes.POINTER(State)];tail.restype=None
LOWER,VECTOR=0x8c10d94c,0x8cb10000
for address,value in ((0x8c66d250,0x8c6450ea),(0x8c6451f4,LOWER),(0x8c10d958,1),(0x8c10d95c,0x8c0000bc)):
    assert struct.unpack_from('<I',binary,address-0x8c010000)[0]==value
cases=entries=0
for alias,result,error in itertools.product((0x8c000000,0xac000000),(0,1,2,3,4,0xffffffff,7,0xffffffef),(0,1,2,3,4,5,6,7,11,16,8,0xffffffff)):
    ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary;put32(ram,0x8c0000bc,VECTOR)
    dest=alias+0xa04000;ram[0xa04000:0xa0400c]=b'\xaa'*12
    initial=State();initial.r[:]=[0x22340000+i for i in range(16)]
    initial.r[4]=0x1234;initial.r[5]=dest;initial.r[15]=alias+0xf00000
    initial.sr=0x70000063;initial.pr=0x8c000100;initial.pc=0x8c6450ea
    expected_ram,actual_ram=bytearray(ram),bytearray(ram)
    expected,actual=State.from_buffer_copy(initial),State.from_buffer_copy(initial)
    expected_entries=[];actual_entries=[];errors=[]
    def effect(m,r):
        assert r[4]==0x1234 and r[6]==0 and r[7]==1
        for i,v in enumerate((error,0xabcdef,0x1234,0x5678)):put32(m,r[5]+4*i,v)
        r[0]=result
    def trace(pc,word,r,sr,pr,fpu):
        if pc==LOWER:
            entry=State();entry.r[:]=r;entry.sr,entry.pr,entry.pc=sr,pr,pc
            expected_entries.append(snapshot(expected_ram,entry))
    def hook(m,r,sr):
        entry=State();entry.r[:]=r;entry.sr,entry.pr,entry.pc=sr,expected.pr,VECTOR
        expected_entries.append(snapshot(m,entry));effect(m,r);return sr
    reference(expected_ram,expected,hooks={VECTOR:hook},trace=trace)
    @Dispatch
    def service(memory,state,target,context):
        try:
            assert context==123 and target==LOWER
            actual_entries.append(snapshot(actual_ram,state.contents))
            tail(memory,state)
            assert state.contents.pc==VECTOR
            actual_entries.append(snapshot(actual_ram,state.contents))
            effect(actual_ram,state.contents.r);state.contents.pc=state.contents.pr
            return 0
        except BaseException as exc:errors.append(repr(exc));return -9
    view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
    assert fn(view,ctypes.byref(actual),service,ctypes.c_void_p(123))==0 and not errors,errors
    assert actual_entries==expected_entries and actual_ram==expected_ram and bytes(actual)==bytes(expected),cases
    mapped={0:0,1:4,2:1,3:0x41,4:0x43,0xffffffff:5}.get(result,result)
    assert actual.r[0]==mapped
    out=list(struct.unpack_from('<III',actual_ram,0xa04000));sentinel=0xaaaaaaaa
    if result==1:assert out==[0x1234,sentinel,0x5678]
    elif result==2:assert out==[0x1234,sentinel,sentinel]
    elif result==0xffffffff:
        code={0:-27,1:-28,2:-29,3:-30,4:-31,5:-32,6:-33,7:-34,11:-35,16:-36}.get(error,-26)&0xffffffff
        assert out==[code,0xabcdef,sentinel]
    else:assert out==[sentinel]*3
    cases+=1;entries+=len(actual_entries)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
report={'cases_passed':cases,'service_entries_compared':entries,
 'default_status_binding':{'device_slot':'0x8c66d250','formatter':'0x8c6450ea','lower_tail':'0x8c10d94c','operation':1,'runtime_vector_word':'0x8c0000bc'},
 'binary_sha256':hashlib.sha256(binary).hexdigest(),'dll_sha256':sha(dll),
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('sa_file_device_status.c','sa_file_completion_connected.c','sa_indirect_service_tail_wrappers_8c10d938.c')},
 'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'fixture_harness_sha256':sha(PROJECT/'verify_request_abort_8c6013d2.py'),
 'comparison_level':'Full RAM and GPR/SR/PR/PC at lower-tail and runtime-vector entry and return; independent mappings and partial-write assertions.',
 'limitations':'External operation-1 vector target is a fixture; hardware, concurrency and complete driver startup unverified.'}
(PROJECT/'analysis/device_status_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'connected device-status cases and',entries,'service comparisons passed')
