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
from sh4_binary32 import multiply,multiply_add,divide,add_sub
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
for address,stem,name in [(0x8c65b7a8,'sound_stream_work_initialize','sa_sound_stream_work_initialize'),(0x8c65e4b8,'sound_source_query','sa_sound_source_query'),(0x8c65e58a,'sound_source_commit','sa_sound_source_commit'),(0x8c65e644,'sound_source_return','sa_sound_source_return'),(0x8c65e368,'sound_source_lock_enter','sa_sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit','sa_sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder','sa_integer_remainder'),(0x8c6676a4,'sound_stream_format_get','sa_sound_stream_format_get'),(0x8c668af4,'sound_driver_split_range','sa_sound_driver_split_range'),(0x8c10cd38,'engine_frame_pool_queue_initialize','sa_engine_record_copy_dispatch'),(0x8c10cd8e,'engine_frame_pool_queue_initialize','sa_engine_record_copy8_tail')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi
assets=json.loads((out/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[];cases=0
for asset,bank in itertools.product(assets,[0,0x200000]):
 path=ROOT/asset['path'];size=path.stat().st_size;assert size==asset['bytes']
 with path.open('rb') as file:data=file.read(64)
 assert data.hex()==asset['header_hex'] and data[:2]==b'\x80\x00'
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(data)]=data;state=0xa20000;ram[state:state+256]=bytes([0x55])*256
 struct.pack_into('<I',ram,state+8,0x8ca30000);ram[0xa30000:0xa30040]=bytes([0x55])*64
 ram[0xa60000:0xa60100]=bytes([0x77])*256;ram[0xa60001]=1
 struct.pack_into('<2I',ram,0xa60004,0x8ca20000,0x8ca80000)
 struct.pack_into('<12I',ram,0xa80000,0x8ca90000,1,0,4096,0,0,0,0x8ca10000,8192,0,0,0)
 for off,address in [(0x18,0x8c65e4b8),(0x1c,0x8c65e644),(0x20,0x8c65e58a)]:struct.pack_into('<I',ram,0xa90000+off,address)
 work_before=bytes(ram[0xa60000:0xa60100]);object_before=bytes(ram[0xa80000:0xa80030])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c644be6:actual.append(tuple(r[i] for i in range(4,7)))
  assert a in lower,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca60000,0x8ca10000,size];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65b7a8
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
 assert lower[0x8c65b7a8](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 nf.fpul=fpul.value
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 target=bytearray([0x55])*256;struct.pack_into('<I',target,8,0x8ca30000);struct.pack_into('<H',target,2,1);target[12:16]=bytes([data[4],data[6],data[7],data[5]])
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
  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)
  struct.pack_into('<I',target,0x80,0x8ca2004c);struct.pack_into('<I',target,0x9c,0);struct.pack_into('<H',target,0xa8,0);struct.pack_into('<I',target,0x4c,0)
  assert actual==[(0x8ca30000,rate,ctypes.c_int16(extension).value&0xffffffff)]
  signed=ctypes.c_int16(extension).value;pi2=struct.unpack('<f',struct.pack('<I',0x40c90fda))[0]
  angle=raw(f32(f32(f32(float(signed))*pi2)/f32(float(rate))));trig=trig_oracle(angle)
  root2=f32(math.sqrt(2.));center=f32(root2-trig);denominator=f32(root2-1.)
  coefficient=f32(f32(center-f32(math.sqrt(f32(f32(center-denominator)*f32(center+denominator)))))/denominator)
  a=int(f32(coefficient*8192.))&65535;b=int(-f32(f32(coefficient*coefficient)*4096.))&65535
  file_target=bytearray([0x55])*64;struct.pack_into('<2H',file_target,0x30,a,b)
  assert native[0xa30000:0xa30040]==file_target
 else:assert not actual
 assert native[state:state+256]==target,(asset['path'],[(hex(i),native[state+i],target[i]) for i in range(256) if native[state+i]!=target[i]])
 header_length=ctypes.c_int16((offset+4)&65535).value
 success=accepted and 0<header_length<=4096
 wanted_work=bytearray(work_before)
 if success:wanted_work[1]=2
 assert native[0xa60000:0xa60100]==wanted_work
 wanted_object=bytearray(object_before)
 if success:
  for off,value in [(0xc,4096-header_length),(0x10,header_length),(0x18,header_length)]:struct.pack_into('<I',wanted_object,off,value)
 assert native[0xa80000:0xa80030]==wanted_object
 assert native[0xa10000:0xa10000+len(data)]==data
 records.append(dict(path=asset['path'],bytes=size,header_sha256=hashlib.sha256(data).hexdigest(),bank=bank,header_length=header_length,work_initialized=success))
 cases+=1
(out/'analysis/initialize_original_headers_verification.json').write_text(json.dumps(dict(cases_passed=cases,original_files=len(assets),assets=records,scope='218 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons across109 original magic8000 headers and both banks. Native format selector,initializer,metadata,extension,parameters,signed division,fill,file configuration,coefficients,trig wrapper,dispatcher,reducer,polynomials and numerical helpers connected. Native work initializer,source query/commit/return/locks/remainder,format getter,range splitter and copy helpers also connected. No modeled calls. Independent whole256 work state and48-byte source reservation: commits returned header length and returns remainder if length<=4096, otherwise work preserved/reservation restored. Original64-byte prefixes with controlled4096-byte source reservations; independent full256-byte state,64-byte file-handle guards and coefficient fields,returned offset,header preservation and configuration forwarding. PR/SZ/RM0. Full payload/decoder/physical audio/fullgame unverified.'),indent=2)+'\n')
print(cases,'original header work initialization comparisons passed')
