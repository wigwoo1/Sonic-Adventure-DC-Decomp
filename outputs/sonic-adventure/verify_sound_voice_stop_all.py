import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for bank,answer,sr0 in itertools.product([0,0x200000],[0,123],[0,0x70000063]):
 entry=0x8c094762;name='sa_sound_voice_stop_all'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 table=lit(0x8c09486c);expected_records=[]
 for slot in range(38):
  record=[0x10000000+slot*100+i for i in range(13)];record[1]=slot+1;record[3]=0x1001 if slot%2 else 0x1010
  struct.pack_into('<13I',ram,table+52*slot,*record);record[1]=0;record[3]|=0x10;expected_records.append(struct.pack('<13I',*record))
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (0x8c093b70,)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and len(calls)==38
 assert [x[:2] for x in calls]==[(0x8c093b70,i) for i in range(38)]
 assert r[0]==0 and macl.value==special['macl']==37*52
 for slot,record in enumerate(expected_records):assert native[table+52*slot:table+52*(slot+1)]==record
 cases+=1
(out/'analysis/sound_voice_stop_all_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='44-instruction native stop-all entry, both FPU banks/SR variants/service returns. Full retail RAM/CPU/FPU/SR/PR/MACL comparison and independently checked38 record lifetimes cleared, stop flag set, other record bytes preserved, slot-order stop calls and forced zero return. Stop dispatcher modeled; actual stopping audio unverified.'),indent=2)+'\n')
print(cases,'voice stop-all comparisons passed')
