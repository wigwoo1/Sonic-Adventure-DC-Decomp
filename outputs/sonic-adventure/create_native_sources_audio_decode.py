import re
from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_constructed_audio_decode.py').read_text()
a=s.index(" struct.pack_into('<I',ram,0xa90024");z=s.index(' native=bytearray(ram)',a)
s=s[:a]+" struct.pack_into('<I',ram,0x67c264,0)\n struct.pack_into('<2I',ram,0xab0000,0x8cac0000,0x8cac0100)\n"+s[z:]
loads='''for address,stem in [(0x8c65e39c,'sound_source_pool_initialize'),(0x8c65e3c6,'sound_source_create'),(0x8c65e454,'sound_source_reset')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi
'''
s=s.replace('assets=json.loads(',loads+'assets=json.loads(',1)
needle=" for entry,args in [(0x8c65b5e8,[])"
extra=''' for entry,args in [(0x8c65e39c,[]),(0x8c65e3c6,[0x8ca10000,8192,0]),(0x8c65e3c6,[0x8ca40008,1024,64512]),(0x8c65e3c6,[0x8ca50008,1024,64512]),(0x8c65e4b8,[0x8ca80000,0,4096,0x8caa0000]),(0x8c65e58a,[0x8ca80000,1,0x8caa0000])]:
  for index,value in enumerate(args,4):s.r[index]=value;r[index]=value
  s.pc=entry
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  if entry==0x8c65e39c:assert native[0x8a65b8:0x8a71b8]==bytes(3072)
  if entry==0x8c65e4b8:assert struct.unpack_from('<2I',native,0xaa0000)==(0x8ca10000,4096)
 for obj,buffer,ring,mirror in [(0xa80000,0x8ca10000,8192,0),(0xac0000,0x8ca40008,1024,64512),(0xac0100,0x8ca50008,1024,64512)]:
  pointer=0x8c000000|obj
  values=[0x8c67c234,1,0x8c672434,4096 if ring==8192 else 0,4096 if ring==8192 else ring,4096 if ring==8192 else 0,0,buffer,ring,mirror,0x8c65e398,pointer]
  assert native[obj:obj+48]==struct.pack('<12I',*values)
 assert native[0x8a6648:0x8a71b8]==bytes(3072-144)
 object_before=bytes(native[0xa80000:0xa80030])
'''
assert needle in s;s=s.replace(needle,extra+needle)
s=s.replace('(0x10,header_length)','(0x10,4096+header_length)').replace('0xa80010)[0]==header_length','0xa80010)[0]==4096+header_length')
def relocate(m):
 value=int(m.group(),16)
 for source,target in [(0xa80000,0x8a65b8),(0x8ca80000,0x8c8a65b8),(0xac0000,0x8a65e8),(0x8cac0000,0x8c8a65e8),(0xac0100,0x8a6618),(0x8cac0100,0x8c8a6618)]:
  if source<=value<=source+48:return hex(target+value-source)
 return m.group()
s=re.sub(r'0x[0-9a-f]+',relocate,s)
s=s.replace('constructed_audio_decode_verification','native_sources_audio_decode_verification')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'All three encoded/sample source objects originate from guarded native source-pool initialization65e39c and source constructor65e3c6/reset65e454,using retail method table and default handler. Producer reserves4096 bytes with native query mode0 and publishes them with native commit mode1. Independent all12 fields of each source object,unused source slots zero,descriptor pointer/length and encoded ready4096/free4096/write4096/read0 state before native audio construction. Source available sample capacity1024. Full initialization/construction/publication/header/feed/decode path uses persistent RAM/CPU/FPU without fixture source objects or modeled calls. Refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_native_sources_audio_decode.py').write_text(s)
reg=(p/'register_constructed_audio_decode.py').read_text().replace('constructed_audio_decode','native_sources_audio_decode')
reg=reg.replace("names.update({'sa_sound_work_pool_initialize'", "names.update({'sa_sound_source_pool_initialize','sa_sound_source_create','sa_sound_source_reset','sa_sound_work_pool_initialize'")
reg=reg.replace('Native audio construction through original-header decoding:', 'Native source creation and publication through audio decoding:')
(p/'register_native_sources_audio_decode.py').write_text(reg)
