import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram)
 view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll'
 fn=ctypes.CDLL(str(dll)).sa_task_update_list_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 errors=[]
 @Call
 def external(ctx,target,m,f,sr,r,pr):errors.append(hex(target));return -9
 count=0
 for level,length,timer,bank in itertools.product(range(9),range(5),[118,119,120,65535],range(2)):
  tasks=[0x8ca00100+i*128 for i in range(length)];payloads=[0x8ca00800+i*128 for i in range(length)]
  for mem in (ram,native):
   mem[0xa00000:0xa02000]=bytes(8192);mem[0xeffe00:0xf00000]=bytes(512)
   for i in range(8):struct.pack_into('<I',mem,0x79f04c+i*4,tasks[0] if tasks and i==min(level,7) else 0)
   struct.pack_into('<I',mem,0x79f048,0);struct.pack_into('<I',mem,0x79f03c,0);struct.pack_into('<I',mem,0x1c1d40,0xdeadbeef)
   for i,(task,payload) in enumerate(zip(tasks,payloads)):
    at=task&0xffffff;pd=payload&0xffffff
    struct.pack_into('<2I',mem,at,tasks[i+1] if i+1<length else 0,tasks[i-1] if i else 0)
    struct.pack_into('<I',mem,at+16,0x8c07d560);struct.pack_into('<I',mem,at+32,payload)
    struct.pack_into('<4I',mem,pd-16,0,0,0x8ca01800+i*32,0)
    struct.pack_into('<H',mem,pd+6,timer);mem[pd+8]=i
    struct.pack_into('<I',mem,0x78c668+i*4,0x8ca01400+i*32)
    struct.pack_into('<H',mem,0xa01406+i*32,0x1234)
  for tick in range(5):
   f=Fpu();f.fpscr=0x4007c|(bank<<21);f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x3f800000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)]
   s.r[4]=level;s.r[15]=0x8cf00000;s.pc=0x8c09878c;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
   reference(ram,s,{},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,external)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(level,length,timer,bank,tick)
   # A callback exceeding120 schedules deletion; deletion runs on the following update.
   pending_tick=next((k for k in range(5) if ((timer+k+1)&65535)>120),None)
   removed=pending_tick is not None and tick>pending_tick
   head=struct.unpack_from('<I',native,0x79f04c+min(level,7)*4)[0]
   assert head==(0 if removed or not tasks else tasks[0])
   if removed:
    assert struct.unpack_from('<I',native,0x79f048)[0]==(tasks[-1] if tasks else 0)
    assert struct.unpack_from('<I',native,0x79f03c)[0]==(payloads[-1]-16 if payloads else 0)
    for task in tasks:assert struct.unpack_from('<I',native,(task&0xffffff)+32)[0]==0
   for i in range(length):
    flag=struct.unpack_from('<H',native,0xa01406+i*32)[0]
    assert flag==(0x1234 if pending_tick is not None and tick>=pending_tick else 0x9234)
   count+=1
 assert not errors,errors
 sources=json.loads((out/'analysis/task_delete_native_verification.json').read_text())['source_sha256']
 names=[n+'.c' for n in sources]+['sa_task_update_list.c','sa_position_task_callback.c','sa_task_remove_request.c']
 report=dict(cases_passed=count,fixture_count=count//5,updates_per_fixture=5,binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},scope='Native scheduler update list, position callback, deletion request, task deletion and payload heap release across5 consecutive updates. All9 priority inputs,0..4 tasks,timer118/119/120/65535,both banks. No service hooks. Full-state retail comparisons and independent threshold/deferred deletion/list heads/payload clearing/flag assertions. Creation allocator and other game task callbacks remain incomplete.')
 (out/'analysis/task_position_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native position task lifecycle comparisons passed')
if __name__=='__main__':main()
