import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c665466;LENGTH=18;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'linked_list_push_8c665466.dll'));native=lib.sa_linked_list_push_8c665466;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);cases=[]
def off(a):return (a&0x1fffffff)-0x0c000000
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 head=0x8c740000+case*0x1000;node=head+0x100
 old=[0,1,0x8c740800,0xffffffff,rng.getrandbits(32)][case%5]
 if case%8==0:node=head
 if case%8==1:node=head+4
 struct.pack_into('<I',ram,off(head),old)
 if node!=head:struct.pack_into('<I',ram,off(node+4),rng.getrandbits(32))
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=head;s.r[5]=node;s.r[15]=0x8cf00000;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} old_head={old:#x} head={head:#x} node={node:#x}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'old_head':hex(old),'head_address':hex(head),'node':hex(node),'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_linked_list_push_8c665466.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Exact 18-byte retail linked-list insertion; tests empty/nonempty heads, overlapping head and node pointers, and varied RAM state.','limitations':['List ownership, node type, and caller synchronization remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'linked_list_push_8c665466_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} linked-list push RAM/CPU comparisons passed')
