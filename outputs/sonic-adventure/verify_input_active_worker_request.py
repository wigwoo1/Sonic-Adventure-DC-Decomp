import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_input_device_backend_host import IO,Read,Write
from verify_input_command_transport import Exchange,load_transport

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 args=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_backend_host.dll')).sa_input_device_backend_initialize_host;init.argtypes=args+[ctypes.POINTER(IO)]
 submit=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_submit_host.dll')).sa_input_device_submit_host;submit.argtypes=init.argtypes
 complete=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_responses_process_native.dll')).sa_input_device_responses_process_native;complete.argtypes=args
 publish=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_status_publish.dll')).sa_input_device_status_publish;publish.argtypes=args
 shutdown=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_service_shutdown_host.dll')).sa_input_device_service_shutdown_host;shutdown.argtypes=args+[ctypes.POINTER(IO),ctypes.c_uint32]
 service_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_service_shutdown_host.dll')).sa_input_service_start_explicit_host;service_init.argtypes=shutdown.argtypes
 context_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll')).sa_engine_context_initialize_native;context_init.argtypes=args
 count=0
 for channel,bank,align,revision,mode in itertools.product([0],[0],[0],[0],[0]):
  status=5
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000)
  struct.pack_into('<2I',ram,0xa20000,channel,channel+1);struct.pack_into('<2I',ram,0xa30000,0x8ca50000+align,0x8ca60000+align)
  hardware={0xa05f6920:0xffffffff,0xffd8000c:0xffffffff,0xa05f689c:revision,**{0xa05f6c00+off:0 for off in [0,4,0x10,0x14,0x18,0x80,0x8c]}}
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
  def stage(fn,entry,r4=0,r5=0,r6=0,r7=0,host=False,vbr=None):
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)]
   s.r[4:8]=[r4,r5,r6,r7];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'mach':0,'macl':0,'gbr':0}
   reference(ram,s,fpu=f,special=special,mmio=hardware,vbr=vbr or 0)
   parameters=[view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call]
   if host:parameters.append(ctypes.byref(io))
   if vbr is not None:parameters.append(ctypes.c_uint32(vbr))
   assert fn(*parameters)==0 and not errors,(hex(entry),errors)
   assert native==ram and native_registers==hardware,(hex(entry),'RAM/registers')
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'],(hex(entry),'CPU/FPU/MACL')
  assert struct.unpack_from('<I',native,0x67c004)[0]==0
  stage(context_init,0x8c6510a4)
  assert struct.unpack_from('<I',native,0x67c004)[0]==0x8c8a2d54
  stage(service_init,0x8c646356,0x8ca20000,0x8ca30000,0x8ca40000+align,2,host=True,vbr=0x8ca08000)
  assert struct.unpack_from('<I',native,0x894090)[0]==1
  base=struct.unpack_from('<I',native,0x8a83b4)[0]&0xffffff;rec=base+0x28+channel*44;info=base+0x44c+channel*120
  transport=load_transport();transport_calls=[]
  @Exchange
  def exchange(ctx,request,response,capacity):
   req=request.contents;primary=(req.destination&0x3f)==0x20
   transport_calls.append((req.port,req.command,req.destination,req.response_address))
   if req.port!=channel//6 or req.packet_mode!=0 or capacity!=1024:
    errors.append('transport request');return -5
   if req.command in [1,9]:
    payload=bytearray(116);payload[:4]=bytes([1,0,0,0]) if primary else bytes([0,0,0,2])
    payload[4:8]=bytes([255,0,0x23,128])
    reply=bytes([5 if req.command==1 else 8,0,1,29])+payload
   elif req.command==10 and not primary:
    if req.payload_words!=2 or bytes(req.payload[:8])!=bytes([0,0,0,2,0,0,0,0]):errors.append('VMU request payload');return -5
    reply=bytearray(40);reply[:4]=bytes([8,0,1,9]);reply[8:40]=bytes(range(32))
   else:errors.append(('unsupported transport command',req.command));return -5
   for i,value in enumerate(reply):response[i]=value
   return len(reply)
  def deliver():
   address=native_registers[0xa05f6c04]
   assert hardware[0xa05f6c04]==address and native_registers[0xa05f6c18]==1
   for mem in [ram,native]:
    memory=(ctypes.c_uint8*len(mem)).from_buffer(mem)
    assert transport(memory,len(mem),address,1024,0,None,exchange)>0 and not errors
   assert native==ram
   hardware[0xa05f6c18]=0;native_registers[0xa05f6c18]=0
  token=struct.unpack_from('<I',native,rec+24)[0]
  assert token==1 and native[rec+6]==1 and native[rec]==0,(channel,bank,align,revision,status,token,list(native[rec:rec+8]))
  assert native[info+20:info+26]==b'(none)'
  stage(submit,0x8c65efda,host=True)
  deliver()
  assert struct.unpack_from('<I',native,rec+28)[0]==token and native[rec+7]==1
  packet=struct.unpack_from('<I',native,base+0xfe8+channel*4)[0]&0xffffff
  assert packet==((0xa50000+align+31)&~31)
  payload=bytearray(116);payload[:4]=bytes([1,0,0,0]);payload[4:8]=bytes([255,0,0x23,128])
  # Host response delivery clears the modeled transport busy bit; it does not change game records.
  hardware[0xa05f6c18]=0;native_registers[0xa05f6c18]=0
  stage(complete,0x8c65f202)
  assert struct.unpack_from('<I',native,rec+28)[0]==0 and struct.unpack_from('<I',native,rec+32)[0]==token
  assert native[rec]==int(status==5) and native[rec+4]==(9 if status==5 else 1)
  if status==5:
   assert struct.unpack_from('<I',native,info)[0]==0x01000000
   assert native[info+4:info+8]==bytes([255,0,0x23,128])
  else:assert struct.unpack_from('<I',native,info)[0]==0
  assert native[base+4+(channel//6)*8]==1
  stage(publish,0x8c6461ae)
  published=0x893bd0+channel*24
  assert native[published:published+4]==native[rec:rec+4]
  assert native[published+4:published+8]==native[rec+8:rec+12]
  assert struct.unpack_from('<I',native,published+8)[0]==0xac000000+packet
  assert native[published]==int(status==5) and native[published+4]==status
  vrec=base+0x28+(channel+1)*44;vinfo=base+0x44c+(channel+1)*120
  assert native[vrec]==0 and native[vrec+4]==1
  rearm=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_submit_rearm.dll')).sa_input_device_submit_rearm;rearm.argtypes=args
  stage(rearm,0x8c65f1ec)
  stage(submit,0x8c65efda,host=True)
  deliver()
  vtoken=struct.unpack_from('<I',native,vrec+28)[0]
  assert vtoken>1 and native[vrec+7]==1,(channel,bank,align,revision,vtoken,list(native[vrec:vrec+44]),list(native[base+0xf90:base+0xfa8]))
  active_bank=1-struct.unpack_from('<I',native,base+0x10b4)[0]
  pp=struct.unpack_from('<I',native,base+0xfe8+active_bank*96+channel*4)[0]&0xffffff
  vp=struct.unpack_from('<I',native,base+0xfe8+active_bank*96+(channel+1)*4)[0]&0xffffff
  vpayload=bytearray(116);vpayload[:4]=bytes([0,0,0,2]);vpayload[4:8]=bytes([255,0,0x23,128])
  hardware[0xa05f6c18]=0;native_registers[0xa05f6c18]=0
  stage(complete,0x8c65f202)
  stage(publish,0x8c6461ae)
  assert native[vrec]==1 and struct.unpack_from('<I',native,vrec+32)[0]==vtoken
  assert struct.unpack_from('<I',native,vinfo)[0]==2
  assert native[vinfo+4:vinfo+8]==bytes([255,0,0x23,128])
  published_vmu=0x893bd0+(channel+1)*24
  assert native[published_vmu]==1 and native[published_vmu+4]==5
  # Logical-to-raw mapping comes from recovered backend initialization.
  logical=next(i for i in range(8) if struct.unpack_from('<I',native,0x6724f4+i*4)[0]==channel+1)
  game_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_system_initialize_native.dll')).sa_input_system_initialize_native;game_init.argtypes=args
  set_mode=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_packet_mode_set.dll')).sa_input_packet_mode_set;set_mode.argtypes=args
  stage(game_init,0x8c647e16,15,1<<logical,0x8caa0000)
  stage(set_mode,0x8c647de8,mode)
  device=0x8a0fb0+logical*76
  lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll'))
  f=Fpu();f.fpscr=bank;f.fpul=0xabcdef01;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  def run(name,entry,args,stop=0x8c000100,result=0):
   fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call]
   for i,v in args.items():s.r[i]=v;r[i]=v
   s.pc=entry;s.pr=0x8c000100;pr.value=s.pr
   special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value)
   reference(ram,s,fpu=f,special=special,stop_pc=stop)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==result
   nf.fpul=fpul.value
   assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   assert [mach.value,macl.value,gbr.value]==[special[k] for k in ['mach','macl','gbr']]
  run('sa_input_work_backend_initialize_native',0x8c66082c,{})
  obj=r[0];assert obj==0x8c8d6938 and struct.unpack_from('<I',native,0x8e9de0)[0]==1
  assert struct.unpack_from('<I',native,0x8a83b8)[0]==obj and struct.unpack_from('<I',native,0x8d6938+0x208)[0]==8

  run('sa_input_work_backend_post_native',0x8c660842,{4:0x8c647982,5:0x8cac0000,6:256})
  assert r[0]==0 and struct.unpack_from('<I',native,0x8d6938+0x210+0xc)[0]==1
  run('sa_input_frame_context_native',0x8c668e60,{4:obj},stop=0x8c647982,result=1)
  assert pr.value==0x8c647982 and r[15]==0x8cac00fc and struct.unpack_from('<I',native,0x8e9dd8)[0]==obj
  for mem in [ram,native]:
   struct.pack_into('<I',mem,0x894138+logical*128+8,18)
   struct.pack_into('<I',mem,0x894138+logical*128+0x14,logical)
   mem[0x89c550+logical*0x148+2]=1
  run('sa_input_channel_worker_queued_native',0x8c647982,{4:logical})
  assert struct.unpack_from('<I',native,vrec+24)[0]>vtoken and native[vrec+6]==10
  request_token=struct.unpack_from('<I',native,vrec+24)[0]
  rearm=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_submit_rearm.dll')).sa_input_device_submit_rearm;rearm.argtypes=init.argtypes[:-1]
  stage(rearm,0x8c65f1ec)
  stage(submit,0x8c65efda,host=True)
  deliver()
  stage(complete,0x8c65f202)
  stage(publish,0x8c6461ae)
  assert struct.unpack_from('<I',native,vrec+32)[0]==request_token
  run('sa_input_frame_queued_native',0x8c668e60,{4:obj})
  assert struct.unpack_from('<I',native,device)[0]==0x0100
  assert struct.unpack_from('<I',native,device+8)[0]==0x0504
  assert struct.unpack_from('<I',native,device)[0]!=struct.unpack_from('<I',native,device+8)[0]
  assert struct.unpack_from('<I',native,0x894118+logical*4)[0]==0xffffff83
  assert struct.unpack_from('<I',native,0x894138+logical*128+8)[0]==0
  for dst,src in zip([0,4,8,12,16,20,24,28,32,52],[0,2,4,6,8,10,12,16,18,20]):
   assert struct.unpack_from('<I',native,device+dst)[0]==int.from_bytes(bytes(range(32))[src:src+2],'big' if mode else 'little')
  assert struct.unpack_from('<4I',native,device+0x3c)==(3,2,512//3,512//2)
  count+=1
 (out/'analysis/input_active_worker_request_prototype.json').write_text(json.dumps(dict(cases_passed=count,scope='Recovered engine context and input service startup, primary/VMU discovery and game system setup connect to native backend work pool/object initialization, work posting and context scheduling. Full retail RAM/CPU/FPU/special-register comparisons. Independent manager pointer, work count, posted context and stack/entry checks. Executes backend request from scheduled context and returns to manager after yielding. Request arguments explicitly supplied by test. No constructed manager/context records in this test. Device responses and hardware behavior remain synthetic; After response completion the recovered manager selects and restores due work, completing request decoding. Test calls manager and supplies worker event 18 and readiness fixtures; worker supplies backend request arguments. Worker ends with status -125 despite decoded descriptor; successful worker operation is not verified. host interrupt/event-triggered scheduling remains unimplemented.'),indent=2)+'\n')
 print(count,'discovered native work scheduling comparisons passed')
if __name__=='__main__':main()

