import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_records_update.dll')).sa_sound_voice_records_update
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
table=lit(0x8c094a18);gate=lit(0x8c094a00);mode=lit(0x8c094a04)
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
cases=0
for disabled,lifetime,flags,bank in itertools.product([0,1],[0,1,5],[0,16],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,gate,disabled);struct.pack_into('<I',ram,mode,0)
 for slot in range(38):struct.pack_into('<13I',ram,table+slot*52,5,lifetime,0x12345678,flags,0x42,0,0,0,0,0,0,0,0xffffffff)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);events=[];actual=[]
 def service(address,regs,log):log.append((address,tuple(regs[i] for i in range(4,8))));regs[0]=0
 hooks={}
 for address in addresses:
  if address==0x8c0948a0:continue
  def hook(m,regs,status,address=address):service(address,regs,events);return status
  hooks[address]=hook
 @Call
 def call(ctx,a,m,f,sr,r,pr):service(a,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c0948a0
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events
 for slot in range(37):
  assert struct.unpack_from('<I',native,table+slot*52+4)[0]==(lifetime if disabled else max(0,lifetime-1))
 cases+=1
(out/'analysis/sound_voice_records_update_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native/reference full RAM/CPU/FPU/SR/PR/MACL/service trace equality; disabled and enabled update, lifetimes0/1/5, flags0/16, both FPU banks. Independent37-slot lifetime checks. Lower calls modeled return0; no playback commands or hardware audio proven.'},indent=2)+'\n')
print(cases,'sound record update comparisons passed')
