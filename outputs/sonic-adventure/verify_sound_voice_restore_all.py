import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_restore_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for bank,answer,sr0 in itertools.product([0,0x200000],[0,123],[0,0x70000063]):
 entry=0x8c094720;name='sa_sound_voice_restore_all'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 mute_global=lit(0x8c094860);struct.pack_into('<I',ram,mute_global,12345)
 final_target=struct.unpack_from('<I',binary,0x094868-0x10000)[0]
 levels=[(-127,0,127,200,-200,17)[i%6] for i in range(38)]
 table=lit(0x8c094864)
 for i,v in enumerate(levels):struct.pack_into("<i",ram,table+4*i,v)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (0x8c093be0,final_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and len(calls)==39
 assert calls[:38]==[(0x8c093be0,levels[i]&0xffffffff,i) for i in range(38)] and calls[-1][0]==final_target
 assert struct.unpack_from('<I',native,mute_global)[0]==0 and r[0]==0
 cases+=1
(out/'analysis/sound_voice_restore_all_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='33-instruction native restore-all entry; full retail RAM/CPU/FPU/SR/PR across both banks,SR0/high and service returns0/123. Independent38 volume calls in slot order0..37 with varied signed saved levels,final service target,mute global zero and forced zero return. Volume/final services modeled; actual restored audio not proven.'),indent=2)+'\n')
print(cases,'voice restore-all comparisons passed')
