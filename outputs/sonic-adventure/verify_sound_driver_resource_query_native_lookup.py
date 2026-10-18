import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_resource_query.dll')).sa_sound_driver_resource_query;fn.argtypes=abi
lookup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_lookup.dll')).sa_sound_bank_section_lookup;lookup.argtypes=abi
def word(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]
getter=word(0x8c6686f8);query=word(0x8c668700);error=word(0x8c6686c0);cases=0
for selector,resource,missing,bank,sr0 in itertools.product([-1,0,1,2,255,256],[0,15,16,255,256],[False,True],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 handle=0x8ca08000;output1=0x8ca20000;output2=0x8ca20004;values=(0x12345678,0xfedcba98)
 for channel in [-1,0,1,2]:struct.pack_into('<I',ram,0xa08010+4*channel,0x8ca10000+64*(channel+1))
 target=0x8ca10000+64*(ctypes.c_int8(selector).value+1)
 ram[0xa0802c:0xa0803c]=bytes([0xa5])*16
 struct.pack_into('<I',ram,0x67c1d0,0 if missing else 0x8ca09000)
 table_offset=struct.unpack_from('<h',binary,0x65a9aa-0x10000)[0]
 for index in range(16):struct.pack_into('<2I',ram,0xa09000+table_offset+8*index,*values)
 struct.pack_into('<2I',ram,output1&0xffffff,0x55555555,0xaaaaaaaa)
 valid=not missing and 0<=ctypes.c_int8(resource).value<16
 answer=word(0x8c65a9b4) if missing else 0 if valid else word(0x8c65aa28)
 expected_values=values if valid else (0x55555555,0xaaaaaaaa)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];queryargs=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==query:
   queryargs.append((a,r[4],r[5],r[6],r[7]));return lookup(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5],r[6],r[7]))
  assert a in (getter,query,error)
  if a==query:
   for dest,value in zip([r[6],r[7]],values):
    for j in range(4):m[(dest&0xffffff)+j]=(value>>(j*8))&255
  r[0]=resource if a==getter else answer if a==query else 777;return 0
 def hook(a):
  def run(m,r,sr):
   events.append((a,r[4],r[5],r[6],r[7]))
   if a==query:struct.pack_into('<I',m,r[6]&0xffffff,values[0]);struct.pack_into('<I',m,r[7]&0xffffff,values[1])
   r[0]=resource if a==getter else answer if a==query else 777;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=selector&0xffffffff;s.r[6]=output1;s.r[7]=output2;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66863c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (getter,error)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert calls==events and calls[0][0:2]==(getter,target)
 assert queryargs==[(query,0x52535053,resource,output1,output2)]
 assert [x[0] for x in calls]==[getter]+([error] if answer else [])
 if answer:assert calls[-1][1]==word(0x8c668704)
 assert struct.unpack_from('<2I',native,output1&0xffffff)==expected_values
 expected=bytearray(bytes([0xa5])*16);struct.pack_into('<I',expected,4*(ctypes.c_int8(selector).value+1),expected_values[0]);struct.pack_into('<I',expected,12,expected_values[1])
 assert native[0xa0802c:0xa0803c]==expected and r[0]==answer
 cases+=1
(out/'analysis/sound_driver_resource_query_native_lookup_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native47-instruction resource query66863c with native bank section lookup65a900. Selectors signed byte/truncation,resource indexes0/15/16/255/256,missing context,banks/SR. Full retail state and diagnostic traces match. Independent SPRS table output/cache values,range/context error preserving output seeds,selector2 overlapping cache,arguments/message/result checked. Getter and diagnostics modeled; actual bank loading/full game unverified.'),indent=2)+'\n')
print(cases,'driver resource query native lookup comparisons passed')
