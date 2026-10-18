import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_record_create.dll')).sa_sound_voice_record_create
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
selector=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_record_create.dll')).sa_sound_voice_slot_select
selector.argtypes=fn.argtypes
update=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_records_update.dll')).sa_sound_voice_records_update
update.argtypes=fn.argtypes
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
start=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_start_dispatch.dll')).sa_sound_voice_start_dispatch
start.argtypes=fn.argtypes
setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_program_setup.dll')).sa_sound_program_setup
setup.argtypes=fn.argtypes
def literal(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
table=literal(0x8c094078);reserved_cursor=literal(0x8c09407c);ordinary_cursor=literal(0x8c094220);cases=0
ids=[0,1,0x42,0x7c,0x308,0x309,0x30d,0x30e,0x4be,0x4bf,0x4c6,0x4c7,0x4ef,0x4f0,0x4f7,0x4f8,0x4f9]
for identity,pattern,bank in itertools.product(ids,['free','busy','eligible'],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 reserved=identity in [0,0x42,0x43,0x7c,0xa1,0xa0,0xef,0x229,0x22a,0x423,0x40f,0x438,0x29b,0x403,0x405,0x4f9] or 0x308<identity<0x30e or 0x4be<identity<0x4c7 or 0x4ef<identity<0x4f8
 base=32 if reserved else 0
 struct.pack_into('<I',ram,reserved_cursor,32);struct.pack_into('<I',ram,ordinary_cursor,0)
 for slot in range(38):struct.pack_into('<2I',ram,table+slot*52,10,0 if pattern=='free' else 1)
 if pattern=='eligible':struct.pack_into('<I',ram,table+(base+2)*52,1)
 struct.pack_into('<I',ram,literal(0x8c094a00),0);struct.pack_into('<I',ram,literal(0x8c094a04),0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=identity;s.r[5]=0xabcdef01;s.r[6]=5;s.r[7]=0x12345678;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c0940da
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 errors=[];actual=[];events=[];command_stack=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c093140:return setup(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c093b2a:return start(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c093f02:return selector(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==struct.unpack_from('<I',binary,0x093340-0x10000)[0]:
   command_stack.append(tuple(sum(m[(r[15]&0xffffff)+4*j+i]<<(8*i) for i in range(4)) for j in range(3)))
  actual.append((a,tuple(r[i] for i in range(4,8))));r[0]=0;return 0
 special={'macl':0}
 reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 expected=base if pattern=='free' else base+2 if pattern=='eligible' else 0xffffffff
 assert r[0]==(0xffffffff if expected==0xffffffff else 44),(identity,pattern,r[0],expected)
 assert macl.value==special['macl']
 if expected!=0xffffffff:
  assert struct.unpack_from('<12I',native,table+expected*52)==(0xffffffff,120,0xabcdef01,17,identity,0,0x12345678,0,0,0,0,0)
 hooks={}
 for address in addresses:
  if address in (0x8c0948a0,0x8c093b2a,0x8c093140):continue
  def hook(m,regs,status,address=address):events.append((address,tuple(regs[i] for i in range(4,8))));regs[0]=0;return status
  hooks[address]=hook
 for tick in range(2):
  s.pc=0x8c0948a0;s.pr=0x8c000100;r[:]=s.r;sr.value=s.sr;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks=hooks)
  assert update(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events
  if expected!=0xffffffff:
   assert struct.unpack_from('<I',native,table+expected*52+4)[0]==120-tick
   assert struct.unpack_from('<I',native,table+expected*52+12)[0]==16
 if expected!=0xffffffff:
  if expected<32:
   command=struct.unpack_from('<I',binary,0x093340-0x10000)[0]
   assert command_stack and command_stack[0][0:2]==(((expected&15)|0xb0),0x20)
  else:
   target=struct.unpack_from('<I',binary,0x093bb8-0x10000)[0]
   mapping=literal(0x8c093bb4)
   handle=struct.unpack_from('<I',native,mapping+(expected-30)*4)[0]
   position=literal(0x8c094a14);boundary=0;group=0
   for _ in range(2048):
    value,kind=struct.unpack_from('<ii',native,position)
    if value<0:break
    if value<identity:boundary=value;group=kind;break
    position+=8
   command_identity=identity if boundary==0 else identity-boundary-1
   if boundary==0:group=0
   assert any(a==target and args[:3]==(handle,group,command_identity) for a,args in actual)
 cases+=1
(out/'analysis/sound_voice_create_update_program_setup_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native/reference full RAM/CPU/SR/PR/FPU comparisons. Independent reserved/ordinary selection for boundary identities, free/busy/priority eligible tables and both FPU banks. All 37 table entries initialized; valid cursor starts32/0 only. Native48-instruction caller plus native193-instruction selector, independent created12-word record and MACL checks. Function name provisional; Native530-instruction updater connected for two ticks; independent creation transition17->16 and lifetimes120->119 plus modeled start command slot checked. Native35-instruction start dispatcher connected for ordinary/reserved slots. Existing native128-instruction program setup093140 connected; no added function coverage for alias. Lower command packing/flush calls modeled return0, audio not exercised.'},indent=2)+'\n')
print(cases,'sound record creation/update/program setup comparisons passed')
