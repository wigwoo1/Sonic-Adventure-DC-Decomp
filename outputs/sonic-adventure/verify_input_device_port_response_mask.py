import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_port_response_mask.dll')).sa_input_device_port_response_mask
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for port,mode,bank,old,new,kind,retries in itertools.product([0,3],[0,1],[0,0x200000],[0,3,128],[0,1,31,255],['valid','ff','ff00'],[0,3,0xffffffff]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;base=0xa10000;record=base+4+port*8
  struct.pack_into('<I',ram,0x8a83b4,0x8ca10000)
  for off,value in [(0x10b4,0),(0x10c4,mode),(0x10dc,3),(0xfe8+96+port*6*4,0x8ca30000)]:struct.pack_into('<I',ram,base+off,value)
  ram[record:record+4]=bytes([old,0x55,0x66,0x77]);struct.pack_into('<I',ram,record+4,retries)
  packet=bytes([5,new,0,0]) if mode else bytes([5,0,new,0])
  if kind!='valid':packet=struct.pack('<I',0xffffffff if kind=='ff' else 0xffffff00)
  ram[0xa30000:0xa30004]=packet
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)]
  s.r[4]=port;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65fd4e
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  if kind=='valid':
   assert native[record:record+4]==bytes([new,old,new&~old,old&~new])
   assert struct.unpack_from('<I',native,record+4)[0]==0 and r[0]==0
  else:
   retry=(retries+1)&0xffffffff
   assert struct.unpack_from('<I',native,record+4)[0]==retry
   assert native[record:record+4]==(bytes([0,old,0,old]) if retry>3 else bytes([old,0x55,0x66,0x77]))
   assert r[0]==(0xffffffff if retry>3 else 0)
  count+=1
 (out/'analysis/input_device_port_response_mask_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Full retail RAM/CPU/FPU comparison and independent mask insertion/removal, byte-order, retry threshold/overflow and return checks. No injected calls. Covers ports 0/3, both modes and FPU banks, high-bit masks and both error sentinels. Does not verify full response dispatcher or full game.'),indent=2)+'\n')
 print(count,'port response mask comparisons passed')

if __name__=='__main__':main()
