"""Check the completion adapter against retail at every subordinate call."""
import ctypes,hashlib,itertools,json,random,struct
from pathlib import Path
from verify_request_abort_8c6013d2 import ROOT,PROJECT,State,Dispatch,P,SIZE,put32,put16,snapshot,reference,binary

dll=ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll'
fn=ctypes.CDLL(str(dll)).sa_file_completion_connected
fn.argtypes=[P,ctypes.POINTER(State),Dispatch,ctypes.c_void_p];fn.restype=ctypes.c_int
STATUS,PENDING,NOTIFY,GLOBAL,LOCAL=0x8cb10000,0x8cb30000,0x8cb20000,0x8cb40000,0x8cb50000
targets=(STATUS,PENDING,NOTIFY,GLOBAL,LOCAL)
cases=entries=guards=0
def run(number,alias,result,mode,notify,aux,fail=None):
    rng=random.Random(number)
    ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
    driver,request,table=alias+0xa00000,alias+0xa03000,alias+0xa02000
    for a,v in [(0x8c673414,driver),(driver+12,table),(driver+28,request),(driver+32,2),
                (request+60,0x1234),(request+20,0xffffffff),(request+28,3),(request+24,99),(request+32,77),
                (request+36,NOTIFY if notify else 0),(request+40,0x7654),
                (driver+36,GLOBAL if notify else 0),(driver+40,0x6789),
                (request+52,LOCAL if notify else 0),(request+56,0x789a),
                (table+16,STATUS),(table+36,PENDING)]:put32(ram,a,v)
    for a,v in [(request+72,mode),(request+74,1),(request+80,2)]:put16(ram,a,v)
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[15]=alias+0xf00000;initial.sr=rng.getrandbits(32)
    initial.pr=0x8c000100;initial.pc=0x8c5ffa12
    expected_ram,actual_ram=bytearray(ram),bytearray(ram)
    expected,actual=State.from_buffer_copy(initial),State.from_buffer_copy(initial)
    expected_entries=[];actual_entries=[];errors=[]
    def effect(m,r,target):
        if target==STATUS:
            assert r[4]==0x1234
            for i,v in enumerate((0xffffffef,0xabc,0xdef)):put32(m,r[5]+4*i,v)
            r[0]=result
        elif target==PENDING:put32(m,r[5],5);r[0]=aux
        else:assert target in (NOTIFY,GLOBAL,LOCAL);r[0]=0xabcdef01
    class Boundary(Exception):pass
    def hook(target):
        def invoke(m,r,sr):
            entry=State();entry.r[:]=r;entry.sr,entry.pr,entry.pc=sr,expected.pr,target
            expected_entries.append(snapshot(m,entry))
            if target==fail:
                expected.r[:]=r;expected.sr,expected.pr,expected.pc=sr,entry.pr,target
                raise Boundary()
            effect(m,r,target);return sr
        return invoke
    try:reference(expected_ram,expected,hooks={t:hook(t) for t in targets})
    except Boundary:pass
    @Dispatch
    def service(memory,state,target,context):
        try:
            assert context==123
            actual_entries.append(snapshot(actual_ram,state.contents))
            if target==fail:return -7
            effect(actual_ram,state.contents.r,target)
            state.contents.pc=state.contents.pr
            return 0
        except BaseException as exc:errors.append(repr(exc));return -9
    view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
    host=fn(view,ctypes.byref(actual),service,ctypes.c_void_p(123))
    assert host==(-7 if fail else 0) and not errors,(number,host,errors)
    assert expected_entries==actual_entries,(number,'service state')
    assert expected_ram==actual_ram and bytes(expected)==bytes(actual),(number,'return state')
    if fail:assert expected.pc==fail
    else:
        word=lambda a:struct.unpack_from('<I',actual_ram,a&0xffffff)[0]
        half=lambda a:struct.unpack_from('<H',actual_ram,a&0xffffff)[0]
        assert word(driver+32)==0
        assert word(driver+28)==(request if result in (4,0x41,0x43) else 0)
        assert half(request+80)==(2 if result in (4,0x41,0x43) else result&255)
        if result in (5,0x40):assert half(request+82)==0xffef
    return len(actual_entries)

for values in itertools.product((0x8c000000,0xac000000),(0,1,4,0x41,0x43,5,0x40,0x123,0xffffffef),(0,1,4),(False,True),(0,1,2)):
    entries+=run(cases,*values);cases+=1
for result,fail in ((0x43,STATUS),(0x41,PENDING),(5,GLOBAL),(5,LOCAL),(1,NOTIFY)):
    run(cases+guards,0xac000000,result,1,True,1,fail);guards+=1
ram=bytearray(SIZE);state=State();before=bytes(state)
view=(ctypes.c_uint8*SIZE).from_buffer(ram)
assert fn(view,ctypes.byref(state),Dispatch(),None)==-2
assert bytes(state)==before and not any(ram);guards+=1
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
listing=PROJECT/'analysis/instructions/file_completion_process.tsv'
ranges=[]
for line in listing.read_text().splitlines()[1:]:
    address,opcode,*_=line.split('\t');address=int(address,16)
    assert struct.unpack_from('<H',binary,address-0x8c010000)[0]==int(opcode,16)
    if ranges and int(ranges[-1]['end_exclusive'],16)==address:ranges[-1]['end_exclusive']=hex(address+2)
    else:ranges.append({'start':hex(address),'end_exclusive':hex(address+2)})
report={'module':'1ST_READ.BIN','load_address':'0x8c010000','address':'0x8c5ffa12','end_exclusive':'0x8c5ffc98',
 'source':'source/sa_file_completion_process.c','source_sha256':sha(PROJECT/'source/sa_file_completion_process.c'),
 'retail_span_sha256':hashlib.sha256(binary[0x5efa12:0x5efc98]).hexdigest(),
 'instruction_body_ranges':ranges,'indexed_instruction_bytes':sum(int(r['end_exclusive'],16)-int(r['start'],16) for r in ranges),
 'listing_sha256':sha(listing),'cases_passed':cases,'service_entries_compared':entries,'host_guard_cases_passed':guards,
 'binary_sha256':hashlib.sha256(binary).hexdigest(),'dll_sha256':sha(dll),
 'sources':{f'source/{s}':sha(PROJECT/'source'/s) for s in ('sa_file_completion_process.c','sa_file_completion_connected.c')},
 'verifier_sha256':sha(Path(__file__)),'interpreter_sha256':sha(PROJECT/'verify_startup.py'),
 'fixture_harness_sha256':sha(PROJECT/'verify_request_abort_8c6013d2.py'),
 'comparison_level':'Full 16 MiB RAM and GPR/SR/PR/PC at every service entry, normal return and host-error boundary.',
 'limitations':'Device-status, pending query and notifications are fixtures. Modes 0/1/4 and nine status values covered; operations 7/8, special error remapping, hardware and whole-game behavior are outside this focused suite.'}
(PROJECT/'analysis/file_completion_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'completion adapter cases,',entries,'service comparisons and',guards,'host guards passed')
