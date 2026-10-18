import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll';lib=ctypes.CDLL(str(dll));counts={}
 for address in [0x8c658c20,0x8c658c24]:assert binary[address-0x8c010000:address-0x8c010000+4]==bytes.fromhex('0b0001e0')
 defaults=[0x8c641698,0x8c64169c,0x8c6416a0,0x8c6416a4,0x8c6416e0,0x8c6416e4,0x8c6416e8,0x8c6416ec,0x8c6416f0]
 @Call
 def unexpected(ctx,target,m,f,sr,r,pr):raise AssertionError(hex(target))
 for name,entry in [('primary',0x8c64135a),('secondary',0x8c64137c),('set',0x8c6412c2),('reset',0x8c6411b0)]:
  fn=getattr(lib,'sa_interrupt_handler_'+name+'_native');fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for index,callback,mask,bank in itertools.product(range(11),[0,0x8c010000],[0,15],[0,0x200000]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;base=0x8ca00000
   struct.pack_into('<I',ram,0x67c004,base)
   for i in range(11):struct.pack_into('<2I',ram,0xa00230+i*8,0xdead0000+i,0xbeef0000+i)
   native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[base,index,callback,0xabcdef01]
   if name in ['primary','secondary']:s.r[4:6]=[callback,0xabcdef01]
   s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
   reference(ram,s,{},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,unexpected)==0
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and bytes(nf)==bytes(f),(name,index,callback,mask,bank)
   target_index=1 if name=='primary' else 2 if name=='secondary' else index
   pair=struct.unpack_from('<2I',native,0xa00230+target_index*8)
   if callback and name!='reset':assert pair==(callback,0xabcdef01)
   elif target_index<9:assert pair==(defaults[target_index],0xbeef0000+target_index)
   else:assert pair==(0xdead0000+target_index,0xbeef0000+target_index)
   if name in ['primary','secondary']:assert r[0]==0
   count+=1
  counts[name]=count
 report=dict(cases_passed=counts,instruction_counts={'primary':17,'secondary':21,'set':42,'reset':66},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Full native cluster compared with retail instructions; no injected calls. Independent callback-table and reset-vector assertions across indices 0..10, registration/removal, interrupt masks and both FPU banks. Retail constant-return leaves checked directly against binary. Interrupt execution and portable host scheduling incomplete.')
 (out/'analysis/interrupt_handlers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'interrupt handler comparisons passed')
if __name__=='__main__':main()
