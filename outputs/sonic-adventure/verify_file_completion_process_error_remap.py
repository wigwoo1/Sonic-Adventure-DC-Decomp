import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_completion_process.dll')).sa_file_completion_process;fn.argtypes=abi
special_detail=struct.unpack_from('<h',binary,0x5ffd14-0x10000)[0]&0xffffffff
cases=0
for result,notify,bank,detail,disc in itertools.product([5,0x40],[0,1],[0,0x200000],[0x3a,special_detail,0xabc],[0,6,3]):
 mode=1;gate=2;aux=0;err=0xffffffe3
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xa00020,gate)
 events=[];actual=[]
 for o,v in [(0xa0001c,0x8ca03000),(0xa0303c,0x1234),(0xa03014,0xffffffff),(0xa0301c,3),(0xa03018,99),(0xa03020,77),(0xa03024,0x8cb20000 if notify else 0),(0xa03028,0x7654),(0xa02024,0x8cb30000),(0xa00024,0x8cb40000 if notify else 0),(0xa00028,0x6789),(0xa03034,0x8cb50000 if notify else 0),(0xa03038,0x789a)]:struct.pack_into('<I',ram,o,v)
 for o,v in [(0xa03048,mode),(0xa0304a,1),(0xa03050,2)]:struct.pack_into('<H',ram,o,v)
 for o,v in [(0x673414,0x8ca00000),(0xa0000c,0x8ca02000),(0xa02010,0x8cb10000)]:struct.pack_into('<I',ram,o,v)
 def service(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8cb10000:
   dest=regs[5]&0xffffff
   for i,v in enumerate(struct.pack('<III',err,detail,0xdef)):m[dest+i]=v
   regs[0]=result
  elif target==0x8c600688:regs[0]=disc
  elif target==0x8cb30000:
   dest=regs[5]&0xffffff
   for i,v in enumerate(struct.pack('<I',[0,5,0xffffffff][aux])):m[dest+i]=v
   regs[0]=aux
  else:assert target in (0x8cb40000,0x8cb50000);regs[0]=0xabcdef01
 def hook(m,regs,status):service(0x8cb10000,m,regs,events);return status
 def disc_status(m,regs,status):service(0x8c600688,m,regs,events);return status
 def pending(m,regs,status):service(0x8cb30000,m,regs,events);return status
 def global_error(m,regs,status):service(0x8cb40000,m,regs,events);return status
 def local_error(m,regs,status):service(0x8cb50000,m,regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):service(target,m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffa12
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cb10000:hook,0x8c600688:disc_status,0x8cb30000:pending,0x8cb40000:global_error,0x8cb50000:local_error})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert actual[0][0]==0x8cb10000 and actual[0][1][0]==0x1234 and actual[0][1][1]==0x8cefffd8 and r[0]==result
 def word(o):return struct.unpack_from('<I',native,o)[0]
 def half(o):return struct.unpack_from('<H',native,o)[0]
 assert word(0xa00020)==0
 assert word(0xa0001c)==(0 if result in (5,0x40) else 0x8ca03000)
 assert word(0xa0303c)==(0 if result in (5,0x40) else 0x1234)
 assert half(0xa03050)==(result if result in (5,0x40) else 2)
 assert half(0xa03048)==mode and word(0xa03014)==0xffffffff
 if result==4:
  assert word(0xa03040)==0xdef and word(0xa03018)==(err if mode==1 else 99)
  assert half(0xa0304a)==1 and len(actual)==1
 elif result==0x41:
  assert actual[1][0]==0x8cb30000 and actual[1][1][:2]==(0x1234,0x8cefffbc)
  assert word(0xa03018)==(77 if aux==0 else 5 if aux==1 else 99)
  assert half(0xa0304a)==(2 if aux==0 else 1) and len(actual)==2
 else:
  mapped=0xffffffe9 if detail==0x3a and disc in (0,6) else 0xffffffdb if detail==special_detail else err
  assert half(0xa03052)==mapped&0xffff and word(0xa03044)==detail
  assert word(0xa00034)==(1 if mapped==0xffffffe9 else 0)
  offset=1+bool(detail==0x3a)
  assert len(actual)==offset+2*notify
  if detail==0x3a:assert actual[1][0]==0x8c600688
  if notify:
   assert actual[offset][0]==0x8cb40000 and actual[offset][1][:2]==(0x6789,mapped)
   assert actual[offset+1][0]==0x8cb50000 and actual[offset+1][1][:2]==(0x789a,mapped)
 cases+=1
report={'cases_passed':cases,'scope':'Native completion error-29 detail remapping: detail3a with disc states0/6/3, special retail detail and ordinary detail, statuses5/40, optional global/local callbacks and both banks. Full retail RAM/CPU/FPU/SR/PR and callback equality; independent mapped error, media-change marker and notification arguments/order. Disc-state and device callbacks modeled; operation7 pending.'}
(root/'analysis/file_completion_process_error_remap_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'completion error remapping comparisons passed')
