import ctypes,itertools,json,hashlib,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

Instruction=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32)
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native_ram=bytearray(ram)
 view=(ctypes.c_uint8*len(native_ram)).from_buffer(native_ram);dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/axis_rotation_native.dll'));P=ctypes.POINTER(ctypes.c_uint32)
 counts={};edge=[0,1,0x7fff,0x8000,0xffff,0x80000000,0xffffffff]
 for name,address in [('x',0x8c639ec8),('y',0x8c639f5c),('z',0x8c63a1cc),('yxz',0x8c639ff8)]:
  fn=getattr(dll,'sa_matrix_rotate_'+name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_void_p,Instruction];cases=0
  angles=list(itertools.product(edge,repeat=3)) if name=='yxz' else [(v,0x12345678,0x87654321) for v in edge]
  for source,bank,rm,sr0,angle in itertools.product([0,0x8ca02000],range(2),range(2),[0,0x700083f3],angles):
   rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
   for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
   nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=source;state.r[15]=0x8cf00000;state.r[5:8]=angle;state.pc=address;state.pr=0x8c000100;state.sr=sr0
   r=(ctypes.c_uint32*16)(*state.r);ev=[];av=[]
   def apply(word,fp,fpul,log):
    log.append((word,fpul,bytes(fp)[:132]));active=(fp.fpscr>>21)&1
    if word==0xf0fd:fp.banks[active][0:2]=[0x3f000001,0x3f800002]
    elif word in [0xf1fd,0xf5fd]:
     start=(word>>8)&12;fp.banks[active][start:start+4]=[0x3f800000+len(log)*4+i for i in range(4)]
    else:raise AssertionError(hex(word))
   @Instruction
   def callback(ctx,word,fp,fpul):apply(word,fp.contents,fpul,av);return 0
   for mem in (ram,native_ram):mem[0xa02000:0xa02040]=bytes(range(64));mem[0xefffc0:0xf00000]=bytes([0x55])*64
   reference(ram,state,fpu=f,vector_instruction=lambda w,fp:apply(w,fp,fp.fpul,ev))
   sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),None,callback)==0;nf.fpul=fpul.value
   assert ram==native_ram and ev==av and bytes(nf)==bytes(f) and list(r)==list(state.r) and sr.value==state.sr,(name,source,bank,rm,sr0,angle)
   cases+=1
  counts[name]=cases
 names=['sa_matrix_rotate_'+n+'.c' for n in counts]
 report=dict(cases_passed=counts,instruction_counts={'x':72,'y':75,'z':75,'yxz':232},binary_sha256=hashlib.sha256(binary).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/axis_rotation_native.dll').read_bytes()).hexdigest(),scope='Four complete retail bodies unhooked, implicit/explicit matrices, zero/wrapped/high angles, both banks/RM0/1. Full RAM including matrix and stack, all integer registers/SR/FPU/FPUL and every vector input snapshot compared. FSCA/FTRV outputs injected; numerical fidelity remains incomplete. PR=SZ=0 at entry, internal paired transfers and bank switches native.')
 (out/'analysis/axis_rotation_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'axis rotation comparisons passed')
if __name__=='__main__':main()
