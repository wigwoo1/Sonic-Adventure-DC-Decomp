import ctypes,hashlib,itertools,json,math,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import P
Vector=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32)
class NativeCpu(ctypes.Structure):
 _fields_=[('r',ctypes.c_uint32*16),('sr',ctypes.c_uint32),('pr',ctypes.c_uint32),('pc',ctypes.c_uint32),('fpul',ctypes.c_uint32)]
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 dll=ROOT/'work/sonic-adventure/build/angle_trig_wrappers.dll';lib=ctypes.CDLL(str(dll));sincos_fn=lib.sa_sincos_phase_8c63a9b8;cosine_fn=lib.sa_cosine_phase_8c63a970;injected_fn=lib.sa_angle_trig
 for fn in (sincos_fn,cosine_fn):fn.argtypes=[ctypes.POINTER(Fpu),ctypes.POINTER(NativeCpu)];fn.restype=ctypes.c_int
 injected_fn.argtypes=[ctypes.POINTER(Fpu),P,P,ctypes.c_void_p,Vector,ctypes.c_int]
 count=0
 angles=[0,1,0x3fff,0x4000,0x7fff,0x8000,0xbfff,0xc000,0xffff,0x10000,0xffffffff,0x12345678]
 for angle,bank,mode,cosine,answer in itertools.product(angles,range(2),[0,1,2,3,0x100000],range(2),[(0,0x3f800000),(0xbf800000,0x80000000),(0x7fbfffff,0x7f800000)]):
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|mode;f.fpul=0x12345678
  for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
  nf=Fpu.from_buffer_copy(f);expected=[];actual=[]
  def apply(word,fp,fpul,log):
   assert word==0xf0fd;log.append((word,fpul,bytes(fp)[:132]));fp.banks[(fp.fpscr>>21)&1][0:2]=answer
  @Vector
  def callback(ctx,word,fp,fpul):apply(word,fp.contents,fpul,actual);return 0
  s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=angle;s.pc=0x8c63a970 if cosine else 0x8c63a9b8;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);fpul=ctypes.c_uint32(f.fpul)
  reference(ram,s,fpu=f,vector_instruction=lambda w,fp:apply(w,fp,fp.fpul,expected))
  assert injected_fn(ctypes.byref(nf),r,ctypes.byref(fpul),None,callback,cosine)==0
  nf.fpul=fpul.value
  assert list(r)==list(s.r) and bytes(nf)==bytes(f) and actual==expected,(angle,bank,mode,cosine,list(r),list(s.r),bytes(nf).hex(),bytes(f).hex(),actual,expected)
  assert s.sr==0x700083f3 and s.pr==0x8c000100;count+=1
 numeric_cases=0;native_functions=[(sincos_fn,False),(cosine_fn,True)]
 def float_word(value):return struct.unpack('<I',struct.pack('<f',value))[0]
 def word_float(value):return struct.unpack('<f',struct.pack('<I',value))[0]
 for phase in range(65536):
  angle=(2.0*math.pi*phase)/65536.0;expected_sine=math.sin(angle);expected_cosine=math.cos(angle)
  sine_bits=float_word(expected_sine);cosine_bits=float_word(expected_cosine)
  for native_fn,cosine in native_functions:
   bank=(phase>>13)&1;sz=(phase>>14)&1;rm=(phase>>8)&1
   f=Fpu();f.fpscr=0x40000|(bank<<21)|(sz<<20)|rm|0x30;f.fpul=0xdeadbeef
   for b in range(2):f.banks[b][:]=[((phase*17+b*257+i*13)^0x5a5a0000)&0xffffffff for i in range(16)]
   expected_f=Fpu.from_buffer_copy(f);cpu=NativeCpu();cpu.r[:]=[0x13570000+i for i in range(16)];cpu.r[4]=0xa5c30000|phase;cpu.sr=0x700083f3;cpu.pr=0x8c000100+(phase<<1);cpu.pc=0x8c63a970 if cosine else 0x8c63a9b8;cpu.fpul=0xabcdef01
   expected_cpu=NativeCpu.from_buffer_copy(cpu);expected_cpu.r[4]=phase;expected_cpu.fpul=phase;expected_cpu.pc=cpu.pr
   active=(f.fpscr>>21)&1;other=active^1
   expected_f.fpscr=(f.fpscr&~0x3f000)|0x1000|0x4
   assert native_fn(ctypes.byref(f),ctypes.byref(cpu))==0,(phase,cosine,'status')
   expected_f.banks[active][0]=f.banks[active][0];expected_f.banks[active][1]=f.banks[active][1]
   if cosine:
    if sz:expected_f.banks[active][0:2]=list(Fpu.from_buffer_copy(expected_f).banks[other][0:2])
    else:expected_f.banks[active][0]=f.banks[active][1]
   assert bytes(cpu)==bytes(expected_cpu),(phase,cosine,'cpu',bytes(cpu).hex(),bytes(expected_cpu).hex())
   assert bytes(f)==bytes(expected_f),(phase,cosine,'fpu',bytes(f).hex(),bytes(expected_f).hex())
   if not cosine:
    actual_sine=word_float(f.banks[active][0]);actual_cosine=word_float(f.banks[active][1])
    assert abs(actual_sine-expected_sine)<=2**-21 and abs(actual_cosine-expected_cosine)<=2**-21,(phase,actual_sine,expected_sine,actual_cosine,expected_cosine)
   elif not sz:
    assert abs(word_float(f.banks[active][0])-expected_cosine)<=2**-21,(phase,'cosine')
   numeric_cases+=1
 for native_fn,cosine in native_functions:
  for fpscr,status in ((0x80000,-1),(0x80,-2)):
   f=Fpu();f.fpscr=fpscr;f.banks[0][:]=list(range(16));f.banks[1][:]=list(range(16,32));before_banks=[list(f.banks[0]),list(f.banks[1])]
   cpu=NativeCpu();cpu.r[4]=0x1234ffff;cpu.pc=0x8c000200;cpu.pr=0x8c000300;before_pc=cpu.pc
   assert native_fn(ctypes.byref(f),ctypes.byref(cpu))==status
   assert cpu.r[4]==0xffff and cpu.fpul==0xffff and cpu.pc==before_pc
   assert [list(f.banks[0]),list(f.banks[1])]==before_banks
   if status==-1:assert f.fpscr==fpscr
   else:assert f.fpscr==((fpscr&~0x3f000)|0x1000|0x4)
 routines=[]
 for name,address,length in [('sa_cosine_phase_8c63a970','0x8c63a970',10),('sa_sincos_phase_8c63a9b8','0x8c63a9b8',8)]:
  start=int(address,16);span=binary[start-0x8c010000:start-0x8c010000+length]
  assert len(span)==length
  routines.append(dict(name=name,module='1ST_READ.BIN',load_address='0x8c010000',address=address,end_exclusive=hex(start+length),span_bytes=length,retail_span_sha256=hashlib.sha256(span).hexdigest()))
 report=dict(cases_passed=count+numeric_cases,callback_comparisons_passed=count,native_fsca_cases_passed=numeric_cases,numeric_phase_values_exhaustive=65536,retail_instructions=9,routines=routines,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/angle_trig_wrappers.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),primary_source='https://www.renesas.com/en/document/mas/sh-4',manual_revision='6.00',comparison_level='For named wrappers, all 16 GPRs, SR, PR, PC, FPUL, complete FPU banks and FPSCR are checked. Both named routines are tested for all 65,536 phases; outputs satisfy the SH-4 FSCA absolute-error bound against the mathematical result. Callback integration tests retain injected FSCA values.',scope='Two whole retail entry spans: unsigned16 phase masking, LDS FPUL, FSCA result and inexact status, RTS return PC, and cosine FR1-to-FR0 delay behavior. Full phase-space tests vary FR bank, SZ and RM; PR=1 and enabled-inexact cases exercise explicit unsupported/trap statuses.',limitations=['The implementation uses a software polynomial meeting the documented FSCA error bound; bit-exact SH7091 hardware output has not been measured.','Enabled inexact and PR=1 exception vector dispatch is not implemented; these cases return explicit status while preserving destination registers.'])
 (out/'analysis/angle_trig_wrappers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{count} callback/reference checks and {numeric_cases} native FSCA state checks passed across all {65536} phases')
if __name__=='__main__':main()

