import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_backend_request_queued_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,event,tick,bank,savedbank,mode in itertools.product(range(8),[0],[0,5,0xffffffff],[0,0x200000],[0,0x200000],[0,1]):
  rng=random.Random(channel^tick);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<2I',ram,0x8e9dd8,0x8ca00000,0x8cab0000);struct.pack_into('<2I',ram,0xa00204,tick,1);struct.pack_into('<I',ram,0xa02690,0x8ca03000)
  words=[rng.getrandbits(32) for i in range(38)];words[0]=0x8cefffe0;words[1]=0x8ca03000;words[2]=0x8c668ef2;words[3]=0x8ca00000;words[4]=0;words[5]=3;words[6]=0x8c1108e0;words[7]=0x8c8e9dd8;words[37]=savedbank|0x40001;struct.pack_into('<38I',ram,0xa00004,*words)
  struct.pack_into('<8I',ram,0xefffe0,bank,0x8c000100,*[0x22340000+i for i in range(9,15)])
  offset=0x894138+channel*128;ram[offset:offset+128]=bytes(128);struct.pack_into('<I',ram,offset+8,event)
  struct.pack_into('<I',ram,0x6724f4+channel*4,channel);ram[0xa10000]=0;struct.pack_into('<I',ram,0xa11000,0)
  struct.pack_into('<I',ram,0x8a83b4,0x8ca11000);ram[0x893bd0+channel*24]=1;struct.pack_into('<I',ram,0xa1144c+channel*120,2);devrec=0xa11028+channel*44;ram[devrec+12]=0;struct.pack_into('<I',ram,devrec+32,0);struct.pack_into('<I',ram,0x89c550+channel*0x148+28,0x8ca20000);struct.pack_into('<I',ram,0xa2003c,1);ram[0x893bd0+channel*24+4]=0
  ram[0x6724c4+channel]=0x40;struct.pack_into('<I',ram,0x6607f4,0x8ca40000)
  for off,value in [(0xfcc,0x8ca50000),(0xfd8,0),(0xfdc,0),(0xfe0,7),(0x10c4,1),(0x10e8,0),(0xfe8+channel*4,0x8ca60000)]:struct.pack_into('<I',ram,0xa11000+off,value)
  ram[0x893bd0+channel*24+4]=8;struct.pack_into('<I',ram,0x893bd0+channel*24+8,0x8ca70000)
  struct.pack_into('<I',ram,0x89c54c,0x8ca80000)
  ram[0xa11450+channel*120:0xa11454+channel*120]=bytes([255,0,0x23,128]);ram[0xa70008:0xa70028]=bytes(range(32))
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank|0x40001;f.fpul=0xabcdef01
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  setter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_packet_mode_set.dll')).sa_input_packet_mode_set
  setter.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
  setup=State();setup.r[:]=[0x33440000+i for i in range(16)];setup.r[4]=mode;setup.r[15]=0x8cf00000;setup.sr=0x70000063;setup.pr=0x8c000100;setup.pc=0x8c647de8
  setup_r=(ctypes.c_uint32*16)(*setup.r);setup_sr=ctypes.c_uint32(setup.sr);setup_pr=ctypes.c_uint32(setup.pr);setup_fpul=ctypes.c_uint32(f.fpul);setup_macl=ctypes.c_uint32();setup_f=Fpu.from_buffer_copy(f);retail_f=Fpu.from_buffer_copy(f)
  @Call
  def no_calls(*args):raise AssertionError('Mode setter called external service')
  reference(ram,setup,fpu=retail_f)
  assert setter(view,ctypes.byref(setup_f),ctypes.byref(setup_sr),setup_r,ctypes.byref(setup_pr),ctypes.byref(setup_fpul),ctypes.byref(setup_macl),None,no_calls)==0
  assert native==ram and list(setup_r)==list(setup.r) and setup_sr.value==setup.sr and setup_pr.value==setup.pr and bytes(setup_f)==bytes(retail_f)
  assert struct.unpack_from('<I',native,0xa80000)[0]==mode
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cee0000;s.sr=0x70000063;s.pr=0x8c010600;s.pc=0x8c6608a4;s.r[5]=0x8ca20000;s.r[6]=0x8ca20000
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   errors.append(hex(t));return -9
  def allocate(mem,r,sr):r[0]=7;return sr
  def reader(mem,r,sr):r[0]=struct.unpack_from('<H',mem,(r[4]&0xffffff)+r[5])[0];return sr
  hooks={}
  reference(ram,s,hooks,fpu=f,special=special,stop_pc=0x8c000100)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  nf.fpul=fpul.value
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert [mach.value,macl.value,gbr.value]==words[17:20] and pr.value==0x8c000100 and r[15]==0x8cf00000
  assert struct.unpack_from('<I',native,0xa03014)[0]==((tick+1)&0xffffffff)
  assert struct.unpack_from('<I',native,0xa03020)[0]==0x8c668e48
  assert struct.unpack_from('<I',native,0xa00204)[0]==((tick+1)&0xffffffff) and struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8cab0000
  assert struct.unpack_from('<2I',native,0xa5000c)==(2,0)
  assert native[devrec+6]==10 and native[devrec+13]==1
  assert struct.unpack_from('<3I',native,0xa11fd8)==(5,20,8)
  assert struct.unpack_from('<I',native,devrec+24)[0]==7
  restore=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_context_restore_queued_native;restore.argtypes=fn.argtypes
  struct.pack_into('<I',ram,devrec+32,7);struct.pack_into('<I',native,devrec+32,7)
  s.r[4]=0x8ca03018;s.r[5]=1;s.pr=0x8c010600;s.pc=0x8c1108e0;r[4]=s.r[4];r[5]=1;pr.value=s.pr
  reference(ram,s,hooks,fpu=f,special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value),stop_pc=0x8c010600)
  assert restore(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==0 and r[15]==0x8cee0000 and pr.value==0x8c010600
  assert struct.unpack_from('<4I',native,0xa2003c)==(3,2,512//3,512//2),struct.unpack_from('<4I',native,0xa2003c)
  offsets=[0,4,8,12,16,20,24,28,32,52];indices=[0,2,4,6,8,10,12,16,18,20]
  for dst,src in zip(offsets,indices):assert struct.unpack_from('<I',native,0xa20000+dst)[0]==int.from_bytes(bytes(range(32))[src:src+2],'big' if mode else 'little')
  count+=1
 report=dict(cases_passed=count,scope='Native backend request status/info/mark/simple-allocation/queue/completion chain; actual work context suspension and manager restoration, fixture completion token update then context restore and successful packet decode return. Full retail RAM/CPU/FPU state compared at both stages without injected calls; both retail packet readers execute natively. Native mode setter configures reader/writer table and is independently compared before backend execution. Manager, table storage and device setup remain fixtures; native metadata and signed division covered; hardware completion update remains a fixture.')
 (out/'analysis/input_backend_mode_initialized_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native completion context comparisons passed')
if __name__=='__main__':main()


