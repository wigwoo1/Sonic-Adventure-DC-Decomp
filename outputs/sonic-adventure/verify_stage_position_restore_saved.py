import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/stage_position_restore_saved.dll';fn=ctypes.CDLL(str(dll)).sa_stage_position_restore_saved;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 targets=[0x8c0940da,0x8c09852e,0x8c08a580,0x8c08ab10,0x0c9001a0,0x0c900180,0x0c9031e0,0x0c900160,0x8c07cef2,0x8c08ab02];visited=set();expected=[];actual=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr,bytes(fp)[:132])
  if target==0x8c08ab02:mem[r[4]&0xffffff]=variant;mem[r[5]&0xffffff]=59
  if target==0x8c10ca5c:
   num=r[1] if r[1]<0x80000000 else r[1]-0x100000000;den=r[0]
   q=abs(num)//den;r[0]=(-q if num<0 else q)&0xffffffff
   return trace,sr^0x31
  for i in range(4):r[i]=0x11223300+i
  if target==0x8c09852e:r[0]=0x8ca00380 if variant else 0
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);actual.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);expected.append(trace);return sr
  return body
 tables=[0x19c4f8,0x19ca98,0x19cdcc,0x19d0d8,0x19d394,0x19d6c8];count=0
 for stage,kind,fast,present,match,variant in itertools.product([0,1,14,26,27,28,29,32,33,34,0xffff],[0,2,3,5,6,7],range(2),range(2),range(2),range(2)):
  for mem in [ram,native]:
   mem[0xa00000:0xa00400]=bytes(1024);mem[0xefffc0:0xf00000]=bytes(64);mem[0xa00108]=0 if variant else 7;mem[0xa00109]=kind
   struct.pack_into('<I',mem,0xa003a0,0x8ca00340);mem[0xa00348]=0xa5
   struct.pack_into('<H',mem,0x78c590,((stage<<8)|2)&0xffff if match else (((stage<<8)|2)^1)&0xffff);struct.pack_into('<I',mem,0x78c598,fast)
   struct.pack_into('<6I',mem,0x78c59c,0x3f800000,0x40000000,0xbf800000,0x1234,0x8000,0xffff)
   struct.pack_into('<H',mem,0x7493ba,stage);struct.pack_into('<H',mem,0x7493bc,2);struct.pack_into('<H',mem,0x7493c0,2 if variant else 3);struct.pack_into('<H',mem,0x7493c8,6 if variant else 0)
   struct.pack_into('<I',mem,0x19de28,0x8ca00200 if present else 0);struct.pack_into('<5I',mem,0xa00200,0,0x3f800000,0xbf800000,0x40000000,0x12345678)
   for table in tables:
    struct.pack_into('<2H4I',mem,table,stage if match else 0x2b,2,0x3f000000,0x3f800000,0xbf800000,0xabcdef01)
    struct.pack_into('<2H4I',mem,table+20,0x2b,0,0,0,0,0)
  f=Fpu();f.fpscr=0x4007c|(variant<<21);f.fpul=0x12345678
  for b in range(2):f.banks[b][:]=[0x3f800000+i for i in range(16)]
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[15]=0x8cf00000;s.pc=0x8c04f8fa;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and expected==actual,(stage,kind,fast,present,match,variant)
  assert macl.value==variant*60
  count+=1
 assert not errors,errors
 report=dict(cases_passed=count,instruction_count=209,services_visited=[hex(t) for t in sorted(visited)],source_sha256=hashlib.sha256((out/'source/sa_stage_position_restore.c').read_bytes()).hexdigest(),binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Full209-instruction stage position restoration body including sparse branches, table scan/match/sentinel, pending position consume, stage-specific callbacks, timer and character paths. Full integer regs/SR/PR/FPU/FPUL/stack/RAM/call snapshots compared against original instructions; both banks PR=SZ=0. Saved-position availability/load/store and stage/act key/reset native; timer setter/getter native; position task creator native with allocation null/success and payload initialization; remaining subordinate services injected; MACL timer multiply independently checked; character table IDs0/2/3/5/6/7 covered, invalid unmapped-address character paths excluded; caller integration and full engine recovery incomplete.')
 report['dependency_source_sha256']={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in ['sa_position_task_create.c','sa_signed_division.c','sa_counter_award.c','sa_stage_mode_query.c','sa_counter_add.c','sa_counter_get.c','sa_timer_set.c','sa_timer_get.c','sa_saved_position_reset.c','sa_stage_act_key.c','stage_position_restore_saved.c','sa_stage_position_restore.c','sa_saved_position_available.c','sa_saved_position_load.c','sa_saved_position_store.c']}
 (out/'analysis/stage_position_restore_saved_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'stage position restoration comparisons passed')
if __name__=='__main__':main()

