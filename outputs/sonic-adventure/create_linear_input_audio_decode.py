import re
from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_native_sources_audio_decode.py').read_text()
loads="""for address,stem in [(0x8c65e0c0,'sound_linear_source_pool_initialize'),(0x8c65e0ea,'sound_linear_source_create'),(0x8c65e178,'sound_linear_source_reset'),(0x8c65e182,'sound_linear_source_capacity'),(0x8c65e1d0,'sound_linear_source_query'),(0x8c65e242,'sound_linear_source_commit'),(0x8c65e282,'sound_linear_source_return'),(0x8c65e08c,'sound_linear_source_lock_enter'),(0x8c65e0a4,'sound_linear_source_lock_exit')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi
"""
s=s.replace('assets=json.loads(',loads+'assets=json.loads(',1)
s=s.replace("struct.pack_into('<I',ram,0x67c264,0)","struct.pack_into('<2I',ram,0x67c264,0,0)\n struct.pack_into('<I',ram,0x67c22c,0)")
a=s.index(' for entry,args in [(0x8c65e39c');z=s.index(' for entry,args in [(0x8c65b5e8',a)
s=s[:a]+''' for entry,args in [(0x8c65e39c,[]),(0x8c65e0c0,[]),(0x8c65e0ea,[0x8ca10000,4096]),(0x8c65e3c6,[0x8ca40008,1024,64512]),(0x8c65e3c6,[0x8ca50008,1024,64512])]:
  for index,value in enumerate(args,4):s.r[index]=value;r[index]=value
  s.pc=entry;reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 values=[0x8c67c1fc,1,0x8c672424,4096,0,0x8ca10000,4096,0x8c65e0bc,0x8c8a6138]
 assert native[0x8a6138:0x8a615c]==struct.pack('<9I',*values)
 assert native[0x8a615c:0x8a65b8]==bytes(1152-36)
 for obj,buffer in [(0x8a65b8,0x8ca40008),(0x8a65e8,0x8ca50008)]:
  values=[0x8c67c234,1,0x8c672434,0,1024,0,0,buffer,1024,64512,0x8c65e398,0x8c000000|obj]
  assert native[obj:obj+48]==struct.pack('<12I',*values)
 assert native[0x8a6618:0x8a71b8]==bytes(3072-96)
 object_before=bytes(native[0x8a6138:0x8a615c])
'''+s[z:]
# Relocate the remaining old encoded/source references, keeping independently checked new construction block fixed.
a=s.index(' for entry,args in [(0x8c65b5e8');prefix=s[:a];tail=s[a:]
tail=tail.replace('native[0x8a65b8:0x8a65e8]','native[0x8a6138:0x8a615c]')
mapping={0x8c8a65b8:0x8c8a6138,0x8c8a65e8:0x8c8a65b8,0x8c8a6618:0x8c8a65e8}
tail=re.sub(r'0x[0-9a-f]+',lambda m:hex(mapping.get(int(m.group(),16),int(m.group(),16))),tail)
tail=tail.replace('[(0xc,4096-header_length),(0x10,4096+header_length),(0x18,header_length)]','[(0xc,4096-header_length),(0x10,header_length)]')
tail=tail.replace("struct.unpack_from('<I',native,0x8a65c4)[0]==0 and struct.unpack_from('<I',native,0x8a65c8)[0]==4096+header_length and struct.unpack_from('<I',native,0x8a65d0)[0]==4096","struct.unpack_from('<2I',native,0x8a6144)==(0,4096)")
s=prefix+tail
s=s.replace("struct.pack_into('<2I',ram,0xab0000,0x8c8a65e8,0x8c8a6618)","struct.pack_into('<2I',ram,0xab0000,0x8c8a65b8,0x8c8a65e8)")
a=s.index("(out/'analysis/native_sources_audio_decode_verification.json')")
scope='218 persistent native linear-input audio decoding sequences across109 original magic8000 files and both FPU banks. Native linear-source initializer/constructor/reset create4096-byte original-prefix input using retail method table8c67c1fc;two native ring sources provide sample outputs. Native work/stream/handle construction,start,limits,header processing,feed,callbacks,decode kernel,completion and acknowledgment on one RAM/CPU/FPU state,no modeled calls. Full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparison at every public call. Independent all9 linear-source words,unused linear pool zero,all12 output-source words,unused ring slots zero,stream/work/handle initial bindings,header metadata/coefficients,source header reservation/remainder restoration,feed exhaustion offset4096,exact binary32 oracle for256 samples per channel/predictor histories and sample guards. Original4096-byte input preserved. Input is a controlled prefix,not whole file;refill/full-file/physical audio playback/game caller integration/fullgame remain unverified.'
s=s[:a]+f"(out/'analysis/linear_input_audio_decode_verification.json').write_text(json.dumps(dict(cases_passed=cases,feed_updates_compared=feed_updates,original_files=len(assets),assets=records,scope={scope!r}),indent=2)+'\\n')\nprint(cases,'linear-input audio decoding sequences passed')\n"
(p/'verify_linear_input_audio_decode.py').write_text(s)
