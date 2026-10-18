from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_data_chunk_metadata.py').read_text().replace('sound_data_chunk_metadata.dll','sound_data_chunk_state_initialize.dll').replace('fn=dll.sa_sound_data_chunk_metadata','fn=dll.sa_sound_data_chunk_state_initialize')
s=s.replace('lower={0x8c10ca5c:dll.sa_signed_division,0x8c10cb10:dll.sa_unsigned_quotient}', 'lower={0x8c66a63c:dll.sa_sound_data_chunk_metadata,0x8c10ca5c:dll.sa_signed_division,0x8c10cb10:dll.sa_unsigned_quotient,0x8c10d31c:dll.sa_memory_fill}')
s=s.replace('ram[0xa20000:0xa20040]=bytes([0x55])*64','ram[0xa20000:0xa20100]=bytes([0x55])*256')
s=s.replace(" struct.pack_into('<7I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24,28,32)])\n",'')
s=s.replace('s.r[4:8]=[0x8ca10000,words*4+3,0x8ca20000,0x8ca20004]','s.r[4:7]=[0x8ca20000,0x8ca10000,words*4+3]').replace('s.pc=0x8c66a63c','s.pc=0x8c66a714')
a=s.index(' found=position');b=s.index(' cases+=1',a)
s=s[:a]+''' found=position is not None and position<words;target=bytearray([0x55])*256;struct.pack_into('<H',target,2,1)
 if found:
  width=bits;step=stride;frames=signed_div(ctypes.c_int32(length).value,ctypes.c_int8(stride).value);kind=1
  if bits==4:
   width=16;step=(channels*2)&255;kind=4
   denom=ctypes.c_int8(channels).value&0xffffffff;frames=(length>>1)//denom if denom else 0
  target[12:16]=bytes([255,width,channels,step]);struct.pack_into('<3I',target,16,kind,22050,frames)
  for x in (0x1c,0x48,0x4a,0x26,0x24,0x7c,0x7e):struct.pack_into('<H',target,x,0)
  for x in (0x34,0x30,0x2c,0x28,0x20,0x4c,0x98,0x9c):struct.pack_into('<I',target,x,0)
  struct.pack_into('<3I',target,0x58,ctypes.c_int8(channels).value&0xffffffff,ctypes.c_int8(step).value&0xffffffff,kind)
  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)
  struct.pack_into('<I',target,0x80,0x8ca2004c);struct.pack_into('<H',target,0xa8,1)
  if bits in (4,8,16):struct.pack_into('<H',target,0xaa,{4:2,8:1,16:0}[bits])
 assert r[0]==((position+2)*4 if found else 0) and native[0xa20000:0xa20100]==target,(words,position,bits,stride,channels,length)
 assert native[0xa10000:0xa10060]==data
'''+s[b:]
s=s[:s.index("(out/'analysis/")]+"(out/'analysis/sound_data_chunk_state_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native113-instruction audio data-chunk state initializer66a714 with native metadata/signed and unsigned division/memory fill; no modeled calls.1728 full retail RAM/CPU/FPU/SR/PR/MACL comparisons: marker missing/index0/index10 with word-limit0/1/12/16,width4/8/16/7,stride0/2/128,channels1/2/128,length19/80000001,banks. Independent entire256-byte state,partial rejection preservation,4-bit normalization, signed extensions,resets,copied defaults,internal pointer,format1,selector/unknown preservation and returned offset. Constructed fixtures only; actual assets/playback/fullgame unverified.'),indent=2)+'\\n')\nprint(cases,'data-chunk state comparisons passed')\n"
(p/'verify_sound_data_chunk_state_initialize.py').write_text(s)
