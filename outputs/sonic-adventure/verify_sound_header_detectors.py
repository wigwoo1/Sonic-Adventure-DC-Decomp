import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_header_detectors.dll'));compare=dll.sa_memory_compare_bytes;compare.argtypes=abi
records=[];cases=0
for address,name,tags,positions in [(0x8c669e38,'sa_sound_snd_header_detect',[b'.snd',b'.sd\0'],list(range(4))),(0x8c66a61a,'sa_sound_spsd_header_detect',[b'SPSD'],list(range(4))),(0x8c66ab2a,'sa_sound_wave_header_detect',[b'RIFF'],[0,1,2,3,8,9,10,11])]:
 fn=getattr(dll,name);fn.argtypes=abi;count=0
 for tag,bank,mutation in itertools.product(tags,[0,0x200000],[None]+[(pos,mask) for pos in positions for mask in (1,128)]):
  data=bytearray([0x55])*64;data[:4]=tag
  if address==0x8c66ab2a:data[8:12]=b'WAVE'
  if mutation:data[mutation[0]]^=mutation[1]
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10040]=data
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=address
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  @Call
  def call(ctx,a,m,f,sr,r,pr):
   assert a==0x8c10eb1c,a
   return compare(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  special={'macl':0};reference(ram,s,{},fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  assert r[0]==int(mutation is None) and native[0xa10000:0xa10040]==data
  count+=1;cases+=1
 records.append(dict(name=name,address=hex(address),cases_passed=count))
(out/'analysis/sound_header_detectors_verification.json').write_text(json.dumps(dict(cases_passed=cases,routines=records,scope='Native SND25-instruction669e38,SPSD10-instruction66a61a,WAVE26-instruction66ab2a with native byte comparator10eb1c; no modeled calls.88 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. SND tags .snd/.sd-NUL,SPSD tag,RIFF/WAVE tags; every compared byte individually XOR1/XOR128,valid cases,banks. Independent acceptance/input preservation. Constructed64-byte fixtures; short-buffer safety and metadata/loader integration/actual assets/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'audio header detector comparisons passed')
