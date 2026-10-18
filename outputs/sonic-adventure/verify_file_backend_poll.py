"""Focused retail differential proof of the backend poll and its call boundaries."""
import ctypes, hashlib, itertools, json, struct
from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

project=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
source=project/'source/sa_file_backend_poll.c'
dll=ROOT/'work/sonic-adventure/build/file_backend_poll.dll'
fn=ctypes.CDLL(str(dll)).sa_file_backend_poll
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
listing=(project/'analysis/instructions/file_backend_poll.tsv').read_text().splitlines()[1:]
for line in listing:
    address,opcode,*_=line.split('\t')
    assert struct.unpack_from('<H',binary,int(address,16)-0x8c010000)[0]==int(opcode,16)
cases=entries=0
for alias,bank,initial,after,retarget,result in itertools.product([0x8c000000,0xac000000],[0,0x200000],[0,1],[0,1],[0,1],[0,0xffffffef]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    def put(m,a,v):struct.pack_into('<I',m,a&0xffffff,v)
    for a,v in [(0x673414,alias+0xa00000),(0xa0000c,alias+0xa02000),(0xa02040,0x8cb10000),(0xa0001c,initial),(0xa0101c,after)]:put(ram,a,v)
    native=bytearray(ram);events=[];actual=[];errors=[]
    def effect(m,regs,sr,pr,f,target,log):
        log.append((target,tuple(regs[i] for i in range(16)),sr,pr,bytes(f),bytes(m)))
        regs[0]=result;regs[2]=0x12345678
        if target==0x8cb10000:
            put(m,0xa0001c,after)
            if retarget:put(m,0x673414,alias+0xa01000)
        else:assert target==0x8c5ffa12
    def hook(target):
        def invoke(m,regs,sr):
            effect(m,regs,sr,state.pr,f,target,events)
            return sr
        return invoke
    @Call
    def call(ctx,target,m,fp,sr,regs,pr):
        try:effect(native,regs,sr[0],pr[0],fp.contents,target,actual)
        except BaseException as exc:errors.append(repr(exc));return -9
        return 0
    state=State();state.r[:]=[0x22340000+i for i in range(16)];state.r[15]=alias+0xf00000
    state.sr=0x70000063;state.pr=0x8c000100;state.pc=0x8c6005c2
    regs=(ctypes.c_uint32*16)(*state.r);sr=ctypes.c_uint32(state.sr);pr=ctypes.c_uint32(state.pr)
    fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
    reference(ram,state,fpu=f,hooks={t:hook(t) for t in [0x8cb10000,0x8c5ffa12]})
    view=(ctypes.c_uint8*len(native)).from_buffer(native)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert not errors,errors
    assert events==actual,(cases,'service boundary mismatch')
    assert ram==native and list(state.r)==list(regs) and state.sr==sr.value and state.pr==pr.value and bytes(f)==bytes(nf),cases
    assert len(events)==1+bool(after)
    entries+=len(events);cases+=1
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
report={'cases_passed':cases,'service_entries_compared':entries,'address':'0x8c6005c2','end_exclusive':'0x8c6005e6',
 'binary_sha256':hashlib.sha256(binary).hexdigest(),'source':'source/sa_file_backend_poll.c','source_sha256':sha(source),'dll_sha256':sha(dll),
 'retail_span_sha256':hashlib.sha256(binary[0x5f05c2:0x5f05e6]).hexdigest(),
 'verifier_sha256':sha(project/'verify_file_backend_poll.py'),'interpreter_sha256':sha(project/'verify_startup.py'),
 'comparison_level':'Full RAM, GPR, SR, PR and FPU at service entries and return; PC is implicit in service targets and not exported by the native ABI.',
 'limitations':'Device pump and completion processor are fixtures; hardware, concurrency and whole-game execution unverified.'}
(project/'analysis/file_backend_poll_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'backend poll cases and',entries,'full service boundaries passed')
