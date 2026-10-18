import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lower={}
for stem,entries in [('sound_stream_format_initialize',[(0x8c667596,'sa_sound_stream_format_initialize')]),('sound_magic_state_initialize',[(0x8c667488,'sa_sound_magic_state_initialize')]),('sound_magic_header_metadata',[(0x8c65ae44,'sa_sound_magic_header_metadata'),(0x8c10ca5c,'sa_signed_division')]),('sound_magic_extension_word',[(0x8c65af64,'sa_sound_magic_extension_word')]),('sound_magic_stream_parameters',[(0x8c65afca,'sa_sound_magic_stream_parameters')]),('engine_frame_initialize_native',[(0x8c10d31c,'sa_memory_fill')])]:
 dll=ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll'))
 for a,n in entries:lower[a]=getattr(dll,n);lower[a].argtypes=abi
import math
from fractions import Fraction
from sh4_binary32 import multiply,multiply_add,divide,add_sub,truncate_i32
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def val(word):
    if word==0:return Fraction(0)
    exponent=(word>>23)-150; significand=(word&0x7fffff)|0x800000
    return Fraction(significand)*Fraction(2)**exponent
def sqrt_service(word,fp):
    assert word&0xf0ff==0xf06d
    bank=(fp.fpscr>>21)&1; n=(word>>8)&15; x=val(fp.banks[bank][n])
    candidate=raw(math.sqrt(float(x)))
    while val(candidate)**2>x:candidate-=1
    while val(candidate+1)**2<=x:candidate+=1
    inexact=val(candidate)**2!=x
    if inexact:
        midpoint=(val(candidate)+val(candidate+1))/2
        if x>midpoint**2 or (x==midpoint**2 and candidate&1):candidate+=1
    fp.banks[bank][n]=candidate
    fp.fpscr=(fp.fpscr&~0x3f000)|(0x1004 if inexact else 0)
def trig_oracle(angle):
    x=angle&0x7fffffff;index=int(struct.unpack('<f',struct.pack('<I',divide(x,0x3f490fdb)[0]))[0])
    remainder=add_sub(x,multiply(raw(float(index)),0x3f490fdb)[0],True)[0];octant=index&7
    argument=add_sub(remainder,0xbf490fdb)[0] if octant&1 else remainder
    cosine=octant in [0,3,4,7];table=0x8c160444 if cosine else 0x8c160460
    coefficients=struct.unpack_from('<7I',binary,table-0x8c010000);square=multiply(argument,argument)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if not cosine:answer=multiply(argument,answer)[0]
    if octant in [1,2,3,4]:answer^=0x80000000
    return struct.unpack('<f',struct.pack('<I',answer))[0]
for address,stem,name in [(0x8c644be6,'file_audio_configuration_forward','sa_file_audio_configuration_forward'),(0x8c65adc0,'audio_filter_coefficients','sa_audio_filter_coefficients'),(0x8c10f90c,'math_result_error_wrapper','sa_math_result_error_wrapper'),(0x8c10fcbc,'math_trig_octant_dispatch','sa_math_trig_octant_dispatch'),(0x8c10fbb8,'math_range_reduce','sa_math_range_reduce'),(0x8c110218,'math_trig_polynomials','sa_math_cosine_polynomial'),(0x8c110238,'math_trig_polynomials','sa_math_sine_polynomial')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi
for address,stem,name in [(0x8c65b930,'sound_stream_work_feed','sa_sound_stream_work_feed'),(0x8c65e462,'sound_source_capacity','sa_sound_source_capacity'),(0x8c65bc6c,'sound_work_unit_count_get','sa_sound_work_unit_count_get'),(0x8c65bc84,'sound_work_total_samples_get','sa_sound_work_total_samples_get'),(0x8c6676ee,'sound_stream_unit_count_get','sa_sound_stream_unit_count_get'),(0x8c6676f6,'sound_stream_total_samples_get','sa_sound_stream_total_samples_get'),(0x8c66771c,'sound_stream_source_set','sa_sound_stream_source_set'),(0x8c66776e,'sound_stream_start','sa_sound_stream_start'),(0x8c65bb88,'sound_stream_work_update','sa_sound_stream_work_update'),(0x8c65b7a8,'sound_stream_work_initialize','sa_sound_stream_work_initialize'),(0x8c65e4b8,'sound_source_query','sa_sound_source_query'),(0x8c65e58a,'sound_source_commit','sa_sound_source_commit'),(0x8c65e644,'sound_source_return','sa_sound_source_return'),(0x8c65e368,'sound_source_lock_enter','sa_sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit','sa_sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder','sa_integer_remainder'),(0x8c6676a4,'sound_stream_format_get','sa_sound_stream_format_get'),(0x8c668af4,'sound_driver_split_range','sa_sound_driver_split_range'),(0x8c10cd38,'engine_frame_pool_queue_initialize','sa_engine_record_copy_dispatch'),(0x8c10cd8e,'engine_frame_pool_queue_initialize','sa_engine_record_copy8_tail')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi
lower[0x8c65bb4e]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_tick.dll')),'sa_sound_stream_work_tick');lower[0x8c65bb4e].argtypes=abi
fn=lower[0x8c65b7a8]
kernel=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')),'sa_sound_decode_block_stereo');kernel.argtypes=abi
def extra(dll,name):
    target=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);target.argtypes=fn.argtypes;return target
dispatch=extra('sound_stream_update_dispatch.dll','sa_sound_stream_update_dispatch')
stream_phase=extra('sound_stream_phase_get.dll','sa_sound_stream_phase_get')
decoder=extra('file_decode_request_process.dll','sa_file_decode_request_process')
magic=extra('sound_stream_magic_update.dll','sa_sound_stream_magic_update')
phase_get=extra('file_decode_phase_get.dll','sa_file_decode_phase_get')
complete=extra('sound_stream_decode_complete.dll','sa_sound_stream_decode_complete')
result_get=extra('file_decode_result_get.dll','sa_file_decode_result_get')
compact=extra('sound_stream_stereo_compact.dll','sa_sound_stream_stereo_compact')
sample_copy=extra('sound_stream_sample_copy.dll','sa_sound_stream_sample_copy')
input_set=extra('sound_stream_input_callback_set.dll','sa_sound_stream_input_callback_set')
output_set=extra('sound_stream_output_callback_set.dll','sa_sound_stream_output_callback_set')
start_stream=extra('sound_stream_start.dll','sa_sound_stream_start')
output_default=extra('sound_stream_default_output.dll','sa_sound_stream_default_output')
input_default=extra('sound_stream_default_input.dll','sa_sound_stream_default_input')
source_set=extra('sound_stream_source_set.dll','sa_sound_stream_source_set')
input_provider=extra('sound_stream_input_provider.dll','sa_sound_stream_input_provider')
channel_get=extra('sound_stream_channel_count_get.dll','sa_sound_stream_channel_count_get')
position_get=extra('sound_stream_position_get.dll','sa_sound_stream_position_get')
buffer_get=extra('sound_source_buffer_get.dll','sa_sound_source_buffer_get')
query=extra('sound_source_query.dll','sa_sound_source_query')
lock_enter=extra('sound_source_lock_enter.dll','sa_sound_source_lock_enter')
lock_exit=extra('sound_source_lock_exit.dll','sa_sound_source_lock_exit')
setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_stereo_request.dll')).sa_file_decode_stereo_request
activate=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_activate.dll')).sa_file_decode_request_activate
setup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes
request=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_decode_schedule.dll')).sa_sound_stream_decode_schedule;request.argtypes=fn.argtypes
wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_stereo_request.dll')).sa_sound_stream_stereo_request;wrapper.argtypes=fn.argtypes
division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/signed_division.dll')).sa_signed_division;division.argtypes=fn.argtypes
remainder=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/integer_remainder.dll')).sa_integer_remainder;remainder.argtypes=fn.argtypes
ack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_completion_acknowledge.dll')).sa_file_decode_completion_acknowledge
ack.argtypes=fn.argtypes
completion_helpers={}
for address,stem,name in [(0x8c65b930,'sound_stream_work_feed','sa_sound_stream_work_feed'),(0x8c65e462,'sound_source_capacity','sa_sound_source_capacity'),(0x8c6676a4,'sound_stream_format_get','sa_sound_stream_format_get'),(0x8c6676ee,'sound_stream_unit_count_get','sa_sound_stream_unit_count_get'),(0x8c65bc6c,'sound_work_unit_count_get','sa_sound_work_unit_count_get'),(0x8c65bc84,'sound_work_total_samples_get','sa_sound_work_total_samples_get'),(0x8c66771c,'sound_stream_source_set','sa_sound_stream_source_set'),(0x8c66776e,'sound_stream_start','sa_sound_stream_start'),(0x8c65ba22,'sound_stream_work_complete','sa_sound_stream_work_complete'),(0x8c668af4,'sound_driver_split_range','sa_sound_driver_split_range'),(0x8c10cd38,'engine_frame_pool_queue_initialize','sa_engine_record_copy_dispatch'),(0x8c10cd8e,'engine_frame_pool_queue_initialize','sa_engine_record_copy8_tail'),(0x8c65e58a,'sound_source_commit','sa_sound_source_commit'),(0x8c65e644,'sound_source_return','sa_sound_source_return'),(0x8c10d25c,'byte_move','sa_byte_move'),(0x8c667796,'sound_stream_decode_acknowledge','sa_sound_stream_decode_acknowledge'),(0x8c6676f6,'sound_stream_total_samples_get','sa_sound_stream_total_samples_get'),(0x8c6677b6,'sound_stream_decoded_bytes_get','sa_sound_stream_decoded_bytes_get'),(0x8c6677d8,'sound_stream_decoded_samples_get','sa_sound_stream_decoded_samples_get')]:completion_helpers[address]=extra(stem+'.dll',name)
for address,stem in [(0x8c65b5e8,'sound_work_pool_initialize'),(0x8c667370,'sound_stream_pool_initialize'),(0x8c644b64,'file_decode_pool_initialize'),(0x8c65b624,'sound_work_create'),(0x8c65b606,'sound_work_reset'),(0x8c65e798,'sound_source_buffer_get'),(0x8c65e79c,'sound_source_ring_size_get'),(0x8c65e7a0,'sound_source_mirror_size_get'),(0x8c6673b4,'sound_stream_create'),(0x8c644b6e,'file_decode_handle_allocate'),(0x8c65b778,'sound_work_start'),(0x8c65b774,'sound_work_sample_limit_set'),(0x8c65bc1c,'sound_work_remaining_limit_set'),(0x8c66768c,'sound_stream_input_callback_set')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi
for address,stem in [(0x8c65b72c,'sound_work_destroy'),(0x8c667440,'sound_stream_destroy'),(0x8c644bfe,'file_decode_handle_release'),(0x8c65ad68,'sound_critical_enter'),(0x8c65ad8e,'sound_critical_exit')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi
assets=json.loads((out/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[];cases=0;feed_updates=0
for asset,bank in itertools.product(assets,[0,0x200000]):
 path=ROOT/asset['path'];size=path.stat().st_size;assert size==asset['bytes']
 with path.open('rb') as file:original_prefix=file.read(4096)
 data=original_prefix[:64];assert len(original_prefix)==4096
 assert data.hex()==asset['header_hex'] and data[:2]==b'\x80\x00'
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa11000]=original_prefix;state=0x8d61ac;ram[state:state+256]=bytes([0x55])*256
 ram[0xa40000:0xa40300]=bytes([0x5a])*768;ram[0xa50000:0xa50300]=bytes([0x5a])*768
 struct.pack_into('<I',ram,0xa90024,0x8c65e462)
 struct.pack_into('<12I',ram,0xa80000,0x8ca90000,1,0,4096,0,0,0,0x8ca10000,8192,0,0,0)
 for off,address in [(0x18,0x8c65e4b8),(0x1c,0x8c65e644),(0x20,0x8c65e58a)]:struct.pack_into('<I',ram,0xa90000+off,address)
 for address,base in [(0xac0000,0x8ca40008),(0xac0100,0x8ca50008)]:struct.pack_into('<12I',ram,address,0x8ca90000,1,0,0,600,0,0,base,1024,64512,0,0)
 struct.pack_into('<2I',ram,0xab0000,0x8cac0000,0x8cac0100)
 object_before=bytes(ram[0xa80000:0xa80030])
 struct.pack_into('<2I',ram,0x67c1e0,0,9)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 def call_decode(ctx,address,mem,fp,srp,r,prp):
         if address in completion_helpers:return completion_helpers[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
         target={0x8c667ad4:dispatch,0x8c667718:stream_phase,0x8c667a5c:magic,0x8c644c16:phase_get,0x8c644cb0:decoder,0x8c667844:request,0x8c667978:complete,0x8c644c8c:result_get,0x8c644c7e:ack,0x8c66792c:compact,0x8c667900:sample_copy,0x8c65b88c:input_provider,0x8c6676ae:channel_get,0x8c6676a0:position_get,0x8c65e798:buffer_get,0x8c65e4b8:query,0x8c65e368:lock_enter,0x8c65e380:lock_exit,0x8c6673a2:output_default,0x8c667384:input_default}.get(address)
         if target:return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
         if address in [0x8c10ca5c,0x8c10cbb8,0x8c66780e]:
             target={0x8c10ca5c:division,0x8c10cbb8:remainder,0x8c66780e:wrapper}[address]
             return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
         if address in [0x8c644c38,0x8c644c56]:
             target={0x8c644c38:setup,0x8c644c56:activate}[address]
             return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
         assert address==0x8c65b238,address
         return kernel(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c644be6:actual.append(tuple(r[i] for i in range(4,7)))
  if a not in lower:return call_decode(ctx,a,m,f,sr,r,pr)
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8c8a3910,0x8ca10000,size];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65bb88
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0}
 for entry,args in [(0x8c65b5e8,[]),(0x8c65b624,[0x8ca80000,2,0x8cab0000]),(0x8c65b778,[0x8c8a3910]),(0x8c65b774,[0x8c8a3910,256]),(0x8c65bc1c,[0x8c8a3910,256])]:
  for index,value in enumerate(args,4):s.r[index]=value;r[index]=value
  s.pc=entry
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  if entry==0x8c65b5e8:
   assert native[0x8a3910:0x8a3d90]==bytes(1152) and native[0x8d61ac:0x8d670c]==bytes(1376) and native[0x890680:0x890820]==bytes(416)
  if entry==0x8c65b624:
   assert r[0]==0x8c8a3910 and struct.unpack_from('<3I',native,0x8a3914)==(0x8c8d61ac,0x8ca80000,0x8cac0000)
   assert struct.unpack_from('<I',native,0x8a3920)[0]==0x8cac0100
   assert struct.unpack_from('<4I',native,state+0x38)==(2,0x8ca40008,512,32768)
 work_before=bytes(native[0x8a3910:0x8a3a10]);stream_before=bytes(native[state:state+256]);file_before=bytes(native[0x890680:0x8906c0])
 s.r[4:7]=[0x8c8a3910,0x8ca10000,size]
 for index in range(4,7):r[index]=s.r[index]
 s.pc=0x8c65bb88
 reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
 assert lower[0x8c65bb88](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 nf.fpul=fpul.value
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 target=bytearray(stream_before);struct.pack_into('<3I',target,0x3c,0x8ca40008,512,32768);struct.pack_into('<I',target,4,0);struct.pack_into('<I',target,8,0x8c890680);struct.pack_into('<H',target,2,1);target[12:16]=bytes([data[4],data[6],data[7],data[5]])
 n=(ctypes.c_int8(data[5]).value-2)*8;d=ctypes.c_int8(data[6]).value;q=abs(n)//abs(d)*(-1 if (n<0)!=(d<0) else 1) if d else 0
 rate,length=struct.unpack_from('>2I',data,8);struct.pack_into('<3I',target,16,q&0xffffffff,rate,length)
 offset=struct.unpack_from('>H',data,2)[0];accepted=size>=18 and offset>13
 if accepted:
  extension=struct.unpack_from('>H',data,16)[0];struct.pack_into('<H',target,0x1c,extension)
  struct.pack_into('<H',target,0x24,0)
  if size>=48 and offset>=44:
   value,flag=struct.unpack_from('>2H',data,20);struct.pack_into('<I',target,0x20,value);struct.pack_into('<H',target,0x24,flag)
   if flag==1:struct.pack_into('<H',target,0x26,struct.unpack_from('>H',data,26)[0]);struct.pack_into('<4I',target,0x28,*struct.unpack_from('>4I',data,28))
  struct.pack_into('<3I',target,0x58,ctypes.c_int8(data[7]).value&0xffffffff,ctypes.c_int8(data[5]).value&0xffffffff,q&0xffffffff)
  struct.pack_into('<3I',target,0x64,0x8ca40008,512,32768)
  struct.pack_into('<I',target,0x80,0x8c8d61f8);struct.pack_into('<I',target,0x9c,0);struct.pack_into('<H',target,0xa8,0);struct.pack_into('<I',target,0x4c,0)
  assert actual==[(0x8c890680,rate,ctypes.c_int16(extension).value&0xffffffff)]
  signed=ctypes.c_int16(extension).value;pi2=struct.unpack('<f',struct.pack('<I',0x40c90fda))[0]
  angle=raw(f32(f32(f32(float(signed))*pi2)/f32(float(rate))));trig=trig_oracle(angle)
  root2=f32(math.sqrt(2.));center=f32(root2-trig);denominator=f32(root2-1.)
  coefficient=f32(f32(center-f32(math.sqrt(f32(f32(center-denominator)*f32(center+denominator)))))/denominator)
  a=int(f32(coefficient*8192.))&65535;b=int(-f32(f32(coefficient*coefficient)*4096.))&65535
  file_target=bytearray(file_before);struct.pack_into('<I',file_target,0xc,0);struct.pack_into('<4h',file_target,0x28,0,0,0,0);struct.pack_into('<2H',file_target,0x30,a,b)
  assert native[0x890680:0x8906c0]==file_target
 else:assert not actual
 assert native[state:state+256]==target,(asset['path'],[(hex(i),native[state+i],target[i]) for i in range(256) if native[state+i]!=target[i]])
 header_length=ctypes.c_int16((offset+4)&65535).value
 success=accepted and 0<header_length<=4096
 wanted_work=bytearray(work_before)
 if success:wanted_work[1]=2
 assert native[0x8a3910:0x8a3a10]==wanted_work
 wanted_object=bytearray(object_before)
 if success:
  for off,value in [(0xc,4096-header_length),(0x10,header_length),(0x18,header_length)]:struct.pack_into('<I',wanted_object,off,value)
 assert native[0xa80000:0xa80030]==wanted_object
 assert native[0xa10000:0xa10000+len(data)]==data
 if success:
  feed_work_before=bytes(native[0x8a3910:0x8a3a10]);feed_stream_before=bytes(native[state:state+256]);feed_handle_before=bytes(native[0x890680:0x8906c0])
  s.r[4]=0x8c8a3910;r[4]=s.r[4];s.pc=0x8c65b930
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65b930](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  reserved=4096-header_length;pointer=0x8ca10000+header_length
  wanted=bytearray(feed_work_before);struct.pack_into('<2I',wanted,0x14,pointer,reserved)
  assert native[0x8a3910:0x8a3a10]==wanted
  wanted=bytearray(feed_stream_before)
  for off,value in [(4,1),(0x50,pointer),(0x54,reserved//18),(0x84,0),(0xa0,0),(0xa4,0)]:struct.pack_into('<I',wanted,off,value)
  assert native[state:state+256]==wanted
  assert struct.unpack_from('<I',native,0xa8000c)[0]==0 and struct.unpack_from('<I',native,0xa80010)[0]==header_length and struct.unpack_from('<I',native,0xa80018)[0]==4096
  assert native[0x890680:0x8906c0]==feed_handle_before and r[15]==0x8cf00000
  for entry,target_fn,arg1,arg2 in [(0x8c66768c,input_set,0x8c65b88c,0x8c8a3910),(0x8c667696,output_set,0x8c6673a2,0x8c8d61ac)]:
   s.r[4:7]=[0x8c8d61ac,arg1,arg2]
   for index in range(4,7):r[index]=s.r[index]
   s.pc=entry;reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
   assert target_fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert native==ram and list(r)==list(s.r) and bytes(f)==bytes(nf)
  s.r[4]=0x8c8a3910;r[4]=s.r[4];s.pc=0x8c65bb88
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65bb88](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
  assert struct.unpack_from('<I',native,0x8a3944)[0]==256
  payload=original_prefix[header_length:header_length+288]
  coefficients=[raw(ctypes.c_int16(value).value/4096.) for value in (a,b)]
  for channel,destination in [(0,0xa40008),(1,0xa50008)]:
   newest=older=raw(0.);samples=[]
   for pair in range(8):
    block=payload[(pair*2+channel)*18:(pair*2+channel+1)*18]
    assert len(block)==18 and not (block[0]&128)
    scale=(int.from_bytes(block[:2],'big')&8191)+1
    for packed in block[2:]:
     for nibble in (packed>>4,packed&15):
      signed=nibble-16 if nibble&8 else nibble
      prediction=multiply_add(coefficients[0],newest,multiply(coefficients[1],older)[0])[0]
      sample=multiply_add(raw(float(scale)),raw(float(signed*65536)),prediction)[0]
      samples.append((truncate_i32(sample)[0]>>16)&65535)
      older,newest=newest,sample
   assert native[destination:destination+512]==struct.pack('<256H',*samples)
   assert native[destination-8:destination]==bytes([0x5a])*8
   assert native[destination+512:destination+760]==bytes([0x5a])*248
   assert native[0x8906a8+channel*4:0x8906ac+channel*4]==struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
  before_work_pool=bytes(native[0x8a3910:0x8a3d90]);before_stream_pool=bytes(native[0x8d61ac:0x8d670c]);before_handle_pool=bytes(native[0x890680:0x890820])
  before_buffers=[bytes(native[a:a+768]) for a in [0xa40000,0xa50000]]
  before_sources=[bytes(native[a:a+48]) for a in [0xa80000,0xac0000,0xac0100]]
  s.r[4]=0x8c8a3910;r[4]=s.r[4];s.pc=0x8c65b72c
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65b72c](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  wanted_pool=bytearray(before_work_pool);wanted_pool[:4]=bytes(4)
  assert native[0x8a3910:0x8a3d90]==wanted_pool
  wanted_pool=bytearray(before_stream_pool);wanted_pool[:172]=bytes(172)
  assert native[0x8d61ac:0x8d670c]==wanted_pool
  wanted_pool=bytearray(before_handle_pool);wanted_pool[:52]=bytes(52)
  assert native[0x890680:0x890820]==wanted_pool
  for a,before in zip([0xa40000,0xa50000],before_buffers):assert native[a:a+768]==before
  for a,before in zip([0xa80000,0xac0000,0xac0100],before_sources):assert native[a:a+48]==before
  assert struct.unpack_from('<2I',native,0x67c1e0)==(0,6) and (sr.value>>4)&15==6 and r[15]==0x8cf00000
  feed_updates+=1
 assert native[0xa10000:0xa11000]==original_prefix
 records.append(dict(path=asset['path'],bytes=size,header_sha256=hashlib.sha256(data).hexdigest(),bank=bank,header_length=header_length,work_initialized=success))
 cases+=1
(out/'analysis/constructed_audio_release_verification.json').write_text(json.dumps(dict(cases_passed=cases,feed_updates_compared=feed_updates,original_files=len(assets),assets=records,scope='218 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons across109 original magic8000 headers and both banks. Native format selector,initializer,metadata,extension,parameters,signed division,fill,file configuration,coefficients,trig wrapper,dispatcher,reducer,polynomials and numerical helpers connected. Public work update65bb88 state1 tail-dispatches native work initializer65b7a8; source query/commit/return/locks/remainder,format getter,range splitter and copy helpers also connected. No modeled calls. Independent whole256 work state and48-byte source reservation: commits returned header length and returns remainder if length<=4096, otherwise work preserved/reservation restored. Original64-byte prefixes with controlled4096-byte source reservations; independent full256-byte state,64-byte file-handle guards and coefficient fields,returned offset,header preservation and configuration forwarding. PR/SZ/RM0. Successful initialization continues native feeding on the same RAM/CPU/FPU/objects without resets. Independent work descriptor starts after header, source remaining reservation exhausted/write position4096, stream source/unit count/reset fields/phase1 and decoder handle unchanged. Actual4096-byte original prefix preserved. Same initialized objects continue through native callback registration,public tick,input provider,decoder,work completion and acknowledgment: full RAM/CPU/FPU/FPUL/MACL comparison and independent phase0/consumed256/stack assertions. Independent exact binary32 recurrence verifies all256 samples per channel, final predictor histories and output guards; initial histories zero. Objects originate from native pool initializer65b5e8 and work constructor65b624 with native stream/handle allocation and source accessors. Native work start and limit setters configure256 samples; constructor derives512-sample output capacity and32768-sample channel stride from1024-byte ring and64512-byte mirror. Initialization,construction,configuration,header processing,feeding and decoding preserve one RAM/CPU/FPU state without replacing constructed objects. Independent initial zero pools,returned work pointer,channel/source/stream bindings and derived capacity/stride. After verified decoding, native work destruction65b72c connects stream destruction667440,handle release644bfe,byte fill and critical enter/exit. Independent entire work1152/stream1376/handle416 pools: work first4 bytes cleared,stream first172 andhandle first52 cleared,other slots/bytes unchanged. Both768-byte sample/guard regions and all48 bytes of encoded and sample source objects preserved. Critical depth0,saved mask6,SR interrupt mask6 and stack restored. Producer/refill/full-file streaming/physical playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'persistent initialized decoding sequences passed')
