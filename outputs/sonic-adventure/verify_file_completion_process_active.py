import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_completion_process.dll')).sa_file_completion_process;fn.argtypes=abi
cases=0
for result,mode,notify,bank in itertools.product([0,1,0x43,2,0x123,0xffffffef],[0,1,3,4],[0,1],[0,0x200000]):
 gate=2
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xa00020,gate)
 events=[];actual=[]
 for o,v in [(0xa0001c,0x8ca03000),(0xa0303c,0x1234),(0xa03014,0xffffffff),(0xa0301c,3),(0xa03018,99),(0xa03020,77),(0xa03024,0x8cb20000 if notify else 0),(0xa03028,0x7654)]:struct.pack_into('<I',ram,o,v)
 for o,v in [(0xa03048,mode),(0xa0304a,1),(0xa03050,2)]:struct.pack_into('<H',ram,o,v)
 for o,v in [(0x673414,0x8ca00000),(0xa0000c,0x8ca02000),(0xa02010,0x8cb10000)]:struct.pack_into('<I',ram,o,v)
 def service(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8cb10000:
   dest=regs[5]&0xffffff
   for i,v in enumerate(struct.pack('<I',12)):m[dest+i]=v
   regs[0]=result
  else:assert target==0x8cb20000;regs[0]=0xabcdef01
 def hook(m,regs,status):service(0x8cb10000,m,regs,events);return status
 def completed(m,regs,status):service(0x8cb20000,m,regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):service(target,m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffa12
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cb10000:hook,0x8cb20000:completed})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert actual[0][0]==0x8cb10000 and actual[0][1][0]==0x1234 and actual[0][1][1]==0x8cefffd8 and r[0]==result
 def word(o):return struct.unpack_from('<I',native,o)[0]
 def half(o):return struct.unpack_from('<H',native,o)[0]
 assert word(0xa00020)==0
 assert word(0xa0001c)==(0x8ca03000 if result==0x43 else 0)
 assert word(0xa0303c)==(0x1234 if result==0x43 else 0)
 assert half(0xa03050)==(2 if result==0x43 else result&255)
 assert half(0xa03048)==(0 if result in (0,1) else mode)
 assert word(0xa03014)==(2 if result==1 and mode in (1,4) else 0xffffffff)
 assert word(0xa03018)==(12 if result==1 and mode==1 else 77 if result==1 and mode==4 else 99)
 assert half(0xa0304a)==(2 if result==1 and mode in (1,3,4) else 1)
 expect_notify=result==1 and mode==1 and notify
 assert len(actual)==1+bool(expect_notify)
 if expect_notify:assert actual[1][0]==0x8cb20000 and actual[1][1][0]==0x7654
 cases+=1
report={'cases_passed':cases,'scope':'Active-handle completion processor: statuses0/1/43 and unrecognized positive/high-bit codes, operation modes0/1/3/4, optional completion notification, position overflow and both FPU banks. Full retail RAM/CPU/FPU/SR/PR and callback equality; independent owner/token/state/progress/reset/notification assertions. Device status and notification callbacks modeled. Status41/4/5/40 and operation7 branches pending.'}
(root/'analysis/file_completion_process_active_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'active completion processor comparisons passed')
