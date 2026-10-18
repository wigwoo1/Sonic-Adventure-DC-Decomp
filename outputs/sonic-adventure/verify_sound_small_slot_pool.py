import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_small_slot_pool.dll'))
lower={a:getattr(dll,n) for a,n in [(0x8c66ab9c,'sa_sound_small_pool_initialize'),(0x8c66abc2,'sa_sound_small_slot_allocate'),(0x8c66abea,'sa_sound_small_slot_release'),(0x8c66abf0,'sa_sound_small_slot_identifier'),(0x8c10d31c,'sa_memory_fill')]}
for fn in lower.values():fn.argtypes=abi
pool=struct.unpack_from('<I',binary,0x65abf4)[0];offset=pool&0xffffff;comparisons=0
def execute(ram,native,address,bank,args=()):
 global comparisons
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:4+len(args)]=args;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=address
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10d31c,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert lower[address](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 comparisons+=1;return r[0]
for bank in (0,0x200000):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[offset-8:offset+72]=bytes([0x55])*80;native=bytearray(ram)
 execute(ram,native,0x8c66ab9c,bank)
 assert native[offset:offset+64]==b''.join(struct.pack('<2I',0,15-i) for i in range(8))
 assert native[offset-8:offset]==bytes([0x55])*8 and native[offset+64:offset+72]==bytes([0x55])*8
for mask,bank in itertools.product(range(256),(0,0x200000)):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[offset-8:offset+72]=bytes([0x55])*80
 for i in range(8):struct.pack_into('<2I',ram,offset+i*8,int(bool(mask&(1<<i))),15-i)
 native=bytearray(ram);before=bytes(native[offset:offset+64]);chosen=next((i for i in range(8) if not mask&(1<<i)),None)
 pointer=execute(ram,native,0x8c66abc2,bank);assert pointer==(pool+chosen*8 if chosen is not None else 0)
 target=bytearray(before)
 if chosen is not None:
  struct.pack_into('<I',target,chosen*8,1);assert native[offset:offset+64]==target
  assert execute(ram,native,0x8c66abf0,bank,(pointer,))==15-chosen
  execute(ram,native,0x8c66abea,bank,(pointer,));assert native[offset:offset+64]==before
  assert execute(ram,native,0x8c66abc2,bank)==pointer
  assert native[offset:offset+64]==target
 else:assert native[offset:offset+64]==before
 assert native[offset-8:offset]==bytes([0x55])*8 and native[offset+64:offset+72]==bytes([0x55])*8
(out/'analysis/sound_small_slot_pool_verification.json').write_text(json.dumps(dict(cases_passed=comparisons,pool_address=hex(pool),scope='Native initialization66ab9c/allocation66abc2/release66abea/identifier66abf0 with native memory fill.2044 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. All256 eight-slot occupancy masks,both banks,first free/full rejection,identifier15..8,release and immediate reuse. Independent exact64-byte pool,neighbor guards and unchanged identifiers verified. Pool lifecycle only; audio channel association/real playback/fullgame unverified.'),indent=2)+'\n')
print(comparisons,'sound slot pool comparisons passed')
