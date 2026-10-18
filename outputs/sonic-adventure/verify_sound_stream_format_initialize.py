import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_format_initialize.dll')).sa_sound_stream_format_initialize;fn.argtypes=abi
lower={}
for stem,entries in [('sound_header_detectors',[(0x8c669e38,'sa_sound_snd_header_detect'),(0x8c66a61a,'sa_sound_spsd_header_detect'),(0x8c66ab2a,'sa_sound_wave_header_detect'),(0x8c10eb1c,'sa_memory_compare_bytes')]),('sound_aiff_header_detect',[(0x8c66980a,'sa_sound_aiff_header_detect')])]:
 dll=ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll'))
 for a,n in entries:lower[a]=getattr(dll,n);lower[a].argtypes=abi
magic=struct.unpack_from('<I',binary,0x657668)[0]&65535
fixtures=[('magic',magic.to_bytes(2,'big')+b'X'*62,0x8c667488),('SPSD',b'SPSD'+b'X'*60,0x8c66a440),('WAVE',b'RIFFxxxxWAVE'+b'X'*52,0x8c66a714),('AIFF',b'FORMxxxxAIFF'+b'X'*52,0x8c6698d6),('SND',b'.snd'+b'X'*60,0x8c669f2e),('SD',b'.sd\0'+b'X'*60,0x8c669f2e),('unknown',b'BAD!'+b'X'*60,None)]
cases=0
for (name,data,chosen),bank,result in itertools.product(fixtures,[0,0x200000],[0,24,0xffffffff]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10040]=data;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in lower:return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==chosen,a;actual.append((a,tuple(r[i] for i in range(4,7))));r[0]=result;return 0
 def hook(mem,r,sr):expected.append((chosen,tuple(r[i] for i in range(4,7))));r[0]=result;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca20000,0x8ca10000,64];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c667596
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,{chosen:hook} if chosen else {},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==expected
 assert actual==([(chosen,(0x8ca20000,0x8ca10000,64))] if chosen else []) and r[0]==(result if chosen else 0)
 cases+=1
(out/'analysis/sound_stream_format_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native93-instruction audio format initializer667596 with native SPSD/WAVE/AIFF/SND detectors and byte comparator.42 full retail RAM/CPU/FPU/SR/PR/MACL/initializer traces. Magic-first,SPSD,WAVE,AIFF,two SND tags,unknown,banks,lower return0/24/ffffffff. Independent selected initializer/forwarded state-data-size/result and unknown no-call checked. Initializers modeled in this selector test; special magic initializer667488 unrecovered. Short-buffer safety/fullgame unverified.'),indent=2)+'\n')
print(cases,'audio format selector comparisons passed')
