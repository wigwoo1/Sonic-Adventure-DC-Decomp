import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lower={}
for stem,entries in [('sound_stream_format_initialize',[(0x8c667596,'sa_sound_stream_format_initialize')]),('sound_header_detectors',[(0x8c66a61a,'sa_sound_spsd_header_detect'),(0x8c66ab2a,'sa_sound_wave_header_detect'),(0x8c10eb1c,'sa_memory_compare_bytes')]),('sound_data_chunk_state_initialize',[(0x8c66a714,'sa_sound_data_chunk_state_initialize'),(0x8c66a63c,'sa_sound_data_chunk_metadata'),(0x8c10ca5c,'sa_signed_division'),(0x8c10cb10,'sa_unsigned_quotient'),(0x8c10d31c,'sa_memory_fill')]),('sound_data_transfer_dispatch',[(0x8c66ab5e,'sa_sound_data_transfer_dispatch'),(0x8c66a804,'sa_sound_data_transfer16_step'),(0x8c66a8fa,'sa_sound_data_transfer8_step'),(0x8c66a9fa,'sa_sound_data_bytepair_transfer_step'),(0x8c644c16,'sa_file_pending_state_get')])]:
 dll=ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll'))
 for addr,name in entries:lower[addr]=getattr(dll,name);lower[addr].argtypes=abi
assets=json.loads((out/'analysis/spsd_wave_asset_inventory.json').read_text())['assets'];records=[];comparisons=0
for asset,bank,capacity in itertools.product(assets,[0,0x200000],[0,2,128]):
 assert asset['format']=='WAVE'
 data=(ROOT/asset['path']).read_bytes();assert hashlib.sha256(data).hexdigest()==asset['sha256']
 chunks={};cursor=12
 while cursor+8<=len(data):
  tag=data[cursor:cursor+4];size=struct.unpack_from('<I',data,cursor+4)[0];assert cursor+8+size<=len(data)
  chunks.setdefault(tag,(cursor+8,data[cursor+8:cursor+8+size]));cursor+=8+size+(size&1)
 _,fmt=chunks[b'fmt '];format_code,channels,rate,byte_rate,stride,bits=struct.unpack_from('<HHIIHH',fmt)
 payload_offset,payload=chunks[b'data'];assert format_code==1 and bits in (8,16) and channels in (1,2) and stride==channels*bits//8
 # Retail reads fixed metadata offsets and searches all aligned words.
 assert struct.unpack_from('<H',data,22)[0]==channels and struct.unpack_from('<I',data,24)[0]==rate and struct.unpack_from('<H',data,32)[0]==stride and struct.unpack_from('<H',data,34)[0]==bits
 assert next(i+8 for i in range(0,len(data)//4*4,4) if data[i:i+4]==b'data')==payload_offset
 frames=len(payload)//stride;assert frames>=4
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10000+len(data)]=data
 state=0xa20000;ram[state:state+256]=bytes([0x55])*256;ram[0xa40000:0xa40400]=bytes([0x55])*1024
 for off,value in [(4,1),(8,0x8ca50000),(0x3c,0x8ca40000),(0x40,frames),(0x44,256),(0x50,0x8ca10000+payload_offset),(0x54,capacity),(0x88,0x8c000200),(0x8c,0x8ca60000),(0x90,0x8c000202),(0x94,0x8ca70000)]:struct.pack_into('<I',ram,state+off,value)
 struct.pack_into('<I',ram,0xa5000c,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 def effect(mem,r,t):
  trace=(t,tuple(r[i] for i in range(4,8)))
  if t==0x8c000200:
   for ptr,value in zip((r[5],r[6],r[7]),(1,3,99)):struct.pack_into('<I',mem,ptr&0xffffff,value)
   r[0]=123
  else:r[0]=456
  return trace
 def hook(t):
  def body(mem,r,sr):expected.append(effect(mem,r,t));return sr
  return body
 for address,args in [(0x8c66ab2a,[0x8ca10000]),(0x8c667596,[0x8ca20000,0x8ca10000,len(data)]),(0x8c66ab5e,[0x8ca20000])]:
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:4+len(args)]=args;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=address
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  @Call
  def call(ctx,a,m,f,sr,r,pr):
   if a in lower:return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   assert a in (0x8c000200,0x8c000202),a
   actual.append(effect(native,r,a));return 0
  special={'macl':0};reference(ram,s,{t:hook(t) for t in (0x8c000200,0x8c000202)},fpu=f,special=special)
  assert lower[address](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  assert actual==expected
  if address==0x8c66ab2a:assert r[0]==1
  if address==0x8c667596:
   assert r[0]==payload_offset and native[state+12:state+16]==bytes([255,bits,channels,stride])
   assert struct.unpack_from('<3I',native,state+16)==(1,rate,frames)
   assert struct.unpack_from('<3I',native,state+0x64)==(0x8ca40000,frames,256)
   assert struct.unpack_from('<2H',native,state+0xa8)==(1,int(bits==8))
  comparisons+=1
 n=min(3,capacity);target=bytearray([0x55])*1024
 for i in range(n):
  for ch in range(channels):
   sample=struct.unpack_from('<H',payload,(channels*i+ch)*2)[0] if bits==16 else payload[channels*i+ch]<<(8 if channels==2 else 0)
   struct.pack_into('<H',target,2*(1+i+ch*256),sample)
 assert native[0xa40000:0xa40400]==target and native[0xa10000:0xa10000+len(data)]==data
 assert struct.unpack_from('<I',native,state+4)[0]==3 and struct.unpack_from('<2I',native,state+0xa0)==(n,n*channels*(1 if bits==8 else 2))
 assert actual[-1][1][:3]==(0x8ca70000,n*channels*(1 if bits==8 else 2),n)
 records.append(dict(path=asset['path'],sha256=asset['sha256'],bank=bank,capacity=capacity,bits=bits,channels=channels,rate=rate,frames=frames,payload_offset=payload_offset))
(out/'analysis/wave_format_pipeline_verification.json').write_text(json.dumps(dict(cases_passed=comparisons,asset_fixtures=len(records),assets=records,scope='54 original WAVE fixtures from9 files,banks,capacities0/2/128.162 full retail RAM/CPU/FPU/SR/PR/MACL comparisons across native detector->format selector->initializer->transfer dispatcher on persistent state. Native SPSD/WAVE detectors,comparator,format selector,metadata/division/fill,pending getter and transfer paths. Independent RIFF chunk/PCM metadata and payload-offset checks,original hashes,actual payload sample output/destination guards,source preservation,phase/count/submission length. Only first up-to3 frames transferred; query/submission callbacks modeled. SPSD original assets absent from standalone inventory; physical audio/fullgame unverified.'),indent=2)+'\n')
print(comparisons,'original WAVE format pipeline comparisons passed')
