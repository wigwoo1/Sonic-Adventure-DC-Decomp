import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_callback_register.dll')).sa_file_callback_register_io;fn.argtypes=abi[:7]+[ctypes.c_uint32,ctypes.POINTER(IO)]+abi[7:]
cases=0
for event,occupied,bank in itertools.product([0xa40,0xb20],[0,5,255],[0,0x200000]):
 priority=80
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 pool=0x892840;head=0x893840+(event>>4)*4
 struct.pack_into('<I',ram,head,0)
 for i in range(1,occupied+1):struct.pack_into('<4I',ram,pool+i*16,0x8cb10000,0,0x8ca00000,0x80000050)
 events=[];actual=[];mmio={0xa05f6920:0x123,0xa05f6924:0x456};native_mmio=dict(mmio)
 @Read
 def read(ctx,a,n):assert n==4;return native_mmio[a]
 @Write
 def write(ctx,a,v,n):assert n==4 and a in native_mmio;native_mmio[a]=v
 io=IO(None,read,write)
 def hook(m,regs,status):events.append(tuple(regs[i] for i in range(4,8)));regs[0]=0;return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  assert target==0x8c64135a;actual.append(tuple(regs[i] for i in range(4,8)));regs[0]=0;return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=event;s.r[5]=0x8cb20000;s.r[6]=priority;s.r[7]=0x8ca01000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c64594c
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,vbr=0x8c000000,hooks={0x8c64135a:hook},mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),0x8c000000,ctypes.byref(io),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 index=0 if occupied==255 else occupied+1
 assert r[0]==index
 if index:
  node=0x8c892840+index*16
  assert struct.unpack_from('<4I',native,pool+index*16)==(0x8cb20000,0,0x8ca01000,priority|0x80000000)
  assert struct.unpack_from('<I',native,head)[0]==node
  if event==0x100:assert struct.unpack_from('<I',native,0x200+(event-0x200)//8)[0]==0x8c645afa
 assert native_mmio==mmio
 assert native_mmio[0xa05f6920]==(0x123|(1<<14) if index and event==0xa40 else 0x123)
 assert native_mmio[0xa05f6924]==(0x456|2 if index and event==0xb20 else 0x456)
 if index:assert struct.unpack_from('<I',native,0x22c)[0]==0x8c645b8e
 assert actual==events
 assert len(actual)==int(event==0x900 and index!=0)
 if actual:assert actual[0][:2]==(0x8c645b1e,0)
 assert sr.value&0xf0==0x60
 cases+=1
report={'cases_passed':cases,'scope':'Native callback registration into empty event100/900 lists, pool occupancy0/5/255 and both banks. Full retail RAM/CPU/FPU/SR/PR equality; independent allocation index, list insertion links, callback/context/priority fields and restored interrupt mask. Independent empty-list linkage, event100 VBR vector and event900 installer arguments. Interrupt-mask MMIO modeled explicitly; independent bits and preserved registers checked for events a40/b20.'}
(root/'analysis/file_callback_register_mmio_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'callback registration comparisons passed')
