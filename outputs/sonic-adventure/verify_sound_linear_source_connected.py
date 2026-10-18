import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
specs=[(0x8c65e08c,'sound_linear_source_lock_enter'),(0x8c65e0a4,'sound_linear_source_lock_exit'),(0x8c65e178,'sound_linear_source_reset'),(0x8c65e182,'sound_linear_source_capacity'),(0x8c65e1d0,'sound_linear_source_query'),(0x8c65e242,'sound_linear_source_commit'),(0x8c65e282,'sound_linear_source_return'),(0x8c65e302,'sound_linear_source_contiguous_check')]
helpers={}
for a,stem in specs:
 helpers[a]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helpers[a].argtypes=abi
cases=operations=0;obj=0x8ca40000;dest=0x8ca20004;base=0x8ca10000
for length,request,bank in itertools.product([0,1,32,80,1024],[0,1,7,100,1024],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa40000:0xa40040]=bytes([0x55])*64;ram[0xa20000:0xa20010]=bytes([0x66])*16
 struct.pack_into('<6I',ram,0xa4000c,0x11223344,0x55667788,base,length,0,obj)
 wanted=bytearray(ram[0xa40000:0xa40040]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  assert a in helpers,hex(a)
  return helpers[a](m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 def execute(entry,args):
  global operations
  for i,v in enumerate(args,4):s.r[i]=v;r[i]=v
  s.pc=entry;reference(ram,s,{},fpu=f,special={'macl':0})
  assert helpers[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert native[0xa40000:0xa40040]==wanted and r[15]==0x8cf00000 and (sr.value&0xf0)==0x60
  assert native[0xa20000:0xa20004]==bytes([0x66])*4 and native[0xa2000c:0xa20010]==bytes([0x66])*4
  operations+=1
 struct.pack_into('<2I',wanted,0xc,length,0);execute(0x8c65e178,[obj])
 execute(0x8c65e182,[obj,1]);assert r[0]==length
 count=min(length,request)
 execute(0x8c65e302,[obj,1,request,dest]);assert r[0]==int(count==request) and struct.unpack_from('<I',native,0xa20004)[0]==count
 struct.pack_into('<2I',wanted,0xc,length-count,count);execute(0x8c65e1d0,[obj,1,request,dest]);assert struct.unpack_from('<2I',native,0xa20004)==(base,count)
 execute(0x8c65e242,[obj,1,dest]);assert struct.unpack_from('<2I',native,0xa20004)==(base,count)
 consumed=count//2;unused=count-consumed
 for mem in [ram,native]:struct.pack_into('<2I',mem,0xa20004,base+consumed,unused)
 struct.pack_into('<2I',wanted,0xc,length-consumed,consumed);execute(0x8c65e282,[obj,1,dest])
 execute(0x8c65e182,[obj,1]);assert r[0]==length-consumed
 struct.pack_into('<2I',wanted,0xc,0,length);execute(0x8c65e1d0,[obj,1,length,dest]);assert struct.unpack_from('<2I',native,0xa20004)==(base+consumed,length-consumed)
 execute(0x8c65e242,[obj,1,dest]);execute(0x8c65e182,[obj,1]);assert r[0]==0
 struct.pack_into('<2I',wanted,0xc,length,0);execute(0x8c65e178,[obj]);execute(0x8c65e182,[obj,1]);assert r[0]==length
 cases+=1
scope='50 persistent linear-source reservation sequences;five nonnegative lengths and requests,both FPU banks. Prepared36-byte source fixtures,not native constructor. Native reset/capacity/contiguous check/query/commit/return/locks: reserve,commit,return unused tail,check remaining,reserve all remaining,commit,verify exhausted,reset and verify capacity restored. Caller adjusts unused-tail descriptor in both memories. No RAM/CPU/FPU resets between calls. Full retail RAM/CPU/FPU/SR/PR comparison after every public call and independent64-byte object window,descriptor fields/guards,stack and interrupt mask. No modeled calls;native allocation,game caller integration and playback remain unverified.'
(p/'analysis/sound_linear_source_connected_verification.json').write_text(json.dumps(dict(cases_passed=cases,native_operations_compared=operations,scope=scope),indent=2)+'\n')
print(cases,'linear-source sequences;',operations,'native operations passed')
