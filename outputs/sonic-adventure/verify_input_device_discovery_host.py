import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_input_device_backend_host import IO,Read,Write

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 args=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_backend_host.dll')).sa_input_device_backend_initialize_host;init.argtypes=args+[ctypes.POINTER(IO)]
 submit=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_submit_host.dll')).sa_input_device_submit_host;submit.argtypes=init.argtypes
 complete=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_response_native.dll')).sa_input_device_response_complete_native;complete.argtypes=args
 publish=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_status_publish.dll')).sa_input_device_status_publish;publish.argtypes=args
 count=0
 for channel,bank,align,revision,status in itertools.product([0,18],[0,0x200000],[0,1,31],[0,2],[5,255]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000)
  struct.pack_into('<I',ram,0xa20000,channel);struct.pack_into('<I',ram,0xa30000,0x8ca50000+align)
  hardware={0xa05f689c:revision,**{0xa05f6c00+off:0 for off in [0,4,0x10,0x14,0x18,0x80,0x8c]}}
  native_registers=dict(hardware);accesses=[];errors=[]
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  @Read
  def io_read(ctx,addr,width):
   accesses.append(('read',addr,width))
   if width!=4 or addr not in native_registers:errors.append(('read',addr,width));return 0
   return native_registers[addr]
  @Write
  def io_write(ctx,addr,value,width):
   accesses.append(('write',addr,width,value))
   if width!=4 or addr not in native_registers:errors.append(('write',addr,width));return
   native_registers[addr]=value
  io=IO(None,io_read,io_write)
  def stage(fn,entry,r4=0,r5=0,r6=0,r7=0,host=False):
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)]
   s.r[4:8]=[r4,r5,r6,r7];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'mach':0,'macl':0,'gbr':0}
   reference(ram,s,fpu=f,special=special,mmio=hardware)
   parameters=[view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call]
   if host:parameters.append(ctypes.byref(io))
   assert fn(*parameters)==0 and not errors,(hex(entry),errors)
   assert native==ram and native_registers==hardware,(hex(entry),'RAM/registers')
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'],(hex(entry),'CPU/FPU/MACL')
  stage(init,0x8c65eccc,0x8ca20000,0x8ca30000,0x8ca40000+align,1,True)
  base=struct.unpack_from('<I',native,0x8a83b4)[0]&0xffffff;rec=base+0x28+channel*44;info=base+0x44c+channel*120
  token=struct.unpack_from('<I',native,rec+24)[0]
  assert token==1 and native[rec+6]==1 and native[rec]==0,(channel,bank,align,revision,status,token,list(native[rec:rec+8]))
  assert native[info+20:info+26]==b'(none)'
  stage(submit,0x8c65efda,host=True)
  assert struct.unpack_from('<I',native,rec+28)[0]==token and native[rec+7]==1
  packet=struct.unpack_from('<I',native,base+0xfe8+channel*4)[0]&0xffffff
  assert packet==((0xa50000+align+31)&~31)
  payload=bytearray(116);payload[:4]=bytes([0,0,0,2]);payload[4:8]=bytes([255,0,0x23,128])
  for mem in [ram,native]:mem[packet:packet+120]=bytes([status,0,0,29])+payload
  # Host response delivery clears the modeled transport busy bit; it does not change game records.
  hardware[0xa05f6c18]=0;native_registers[0xa05f6c18]=0
  stage(complete,0x8c65fe14,channel,channel//6)
  assert struct.unpack_from('<I',native,rec+28)[0]==0 and struct.unpack_from('<I',native,rec+32)[0]==token
  assert native[rec]==int(status==5) and native[rec+4]==(9 if status==5 else 1)
  if status==5:
   assert struct.unpack_from('<I',native,info)[0]==2
   assert native[info+4:info+8]==bytes([255,0,0x23,128])
  else:assert struct.unpack_from('<I',native,info)[0]==0
  stage(publish,0x8c6461ae)
  published=0x893bd0+channel*24
  assert native[published:published+4]==native[rec:rec+4]
  assert native[published+4:published+8]==native[rec+8:rec+12]
  assert struct.unpack_from('<I',native,published+8)[0]==0xac000000+packet
  assert native[published]==int(status==5) and native[published+4]==status
  count+=1
 (out/'analysis/input_device_discovery_host_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Connected host-callback backend initialization, actual submission native discovery completion and status publication. Complete retail RAM/CPU/FPU/MACL and final register comparisons at all four stages; independent initial command/token, aligned response pointer, presence/status, metadata and completed-token checks. No injected game calls or manual device-record/metadata setup. Response delivery and transport behavior remain synthetic host fixtures.'),indent=2)+'\n')
 print(count,'host device discovery lifecycle comparisons passed')

if __name__=='__main__':main()
