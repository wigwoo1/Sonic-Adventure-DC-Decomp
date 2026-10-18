from pathlib import Path
import ast,textwrap
p=Path(__file__).resolve().parent
s=(p/'verify_initialize_then_feed.py').read_text()
decode=(p/'verify_tick_native_provider.py').read_text()
extras=decode[decode.index('def extra('):decode.index('def f32(')]
at=s.index('assets=json.loads(')
s=s[:at]+"lower[0x8c65bb4e]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_tick.dll')),'sa_sound_stream_work_tick');lower[0x8c65bb4e].argtypes=abi\nfn=lower[0x8c65b7a8]\nkernel=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')),'sa_sound_decode_block_stereo');kernel.argtypes=abi\n"+extras+s[at:]
s=s.replace(" struct.pack_into('<I',ram,state+4,0)"," struct.pack_into('<I',ram,state+4,0)\n struct.pack_into('<3I',ram,state+0x3c,0x8ca40008,256,32768)\n ram[0xa40000:0xa40300]=bytes([0x5a])*768;ram[0xa50000:0xa50300]=bytes([0x5a])*768")
s=s.replace(' work_before=bytes('," struct.pack_into('<I',ram,0xa60010,0x8cac0100)\n for off in [0x2c,0x30,0x40,0x44,0x48]:struct.pack_into('<I',ram,0xa60000+off,0)\n struct.pack_into('<2I',ram,0xa60038,256,256)\n for address,base in [(0xac0000,0x8ca40008),(0xac0100,0x8ca50008)]:struct.pack_into('<12I',ram,address,0x8ca90000,1,0,0,600,0,0,base,1024,0,0,0)\n struct.pack_into('<I',ram,0xa3000c,0);struct.pack_into('<4h',ram,0xa30028,0,0,0,0)\n work_before=bytes(")
tree=ast.parse(decode)
node=next(n for n in ast.walk(tree) if isinstance(n,ast.FunctionDef) and n.name=='call')
block=textwrap.dedent(ast.get_source_segment(decode,node)).replace('def call(','def call_decode(',1)
at=s.index(' @Call\n def call(')
s=s[:at]+textwrap.indent(block,' ')+'\n'+s[at:]
s=s.replace('  assert a in lower,a\n  return lower[a]',"  if a not in lower:return call_decode(ctx,a,m,f,sr,r,pr)\n  return lower[a]")
s=s.replace('target=bytearray([0x55])*256;',"target=bytearray([0x55])*256;struct.pack_into('<3I',target,0x3c,0x8ca40008,256,32768);")
s=s.replace("  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)","  struct.pack_into('<3I',target,0x64,0x8ca40008,256,32768)")
s=s.replace('file_target=bytearray([0x55])*64;',"file_target=bytearray([0x55])*64;struct.pack_into('<I',file_target,0xc,0);struct.pack_into('<4h',file_target,0x28,0,0,0,0);")
at=s.index('  feed_updates+=1')
addition='''  for entry,target_fn,arg1,arg2 in [(0x8c66768c,input_set,0x8c65b88c,0x8ca60000),(0x8c667696,output_set,0x8c6673a2,0x8ca20000)]:
   s.r[4:7]=[0x8ca20000,arg1,arg2]
   for index in range(4,7):r[index]=s.r[index]
   s.pc=entry;reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
   assert target_fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert native==ram and list(r)==list(s.r) and bytes(f)==bytes(nf)
  s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=0x8c65bb88
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65bb88](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
  assert struct.unpack_from('<I',native,0xa60034)[0]==256
'''
s=s[:at]+addition+s[at:]
s=s.replace('initialize_then_feed_verification','initialized_decode_verification').replace('persistent initialization/feed sequences passed','persistent initialized decoding sequences passed')
s=s.replace('Native decoding/physical playback/fullgame unverified.','Same initialized objects continue through native callback registration,public tick,input provider,decoder,work completion and acknowledgment: full RAM/CPU/FPU/FPUL/MACL comparison and independent phase0/consumed256/stack assertions. Sample oracle for this persistent sequence pending; initial histories zero. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
s=s.replace('from sh4_binary32 import multiply,multiply_add,divide,add_sub','from sh4_binary32 import multiply,multiply_add,divide,add_sub,truncate_i32')
oracle='''  payload=original_prefix[header_length:header_length+288]
  coefficients=[raw(ctypes.c_int16(value).value/4096.) for value in (a,b)]
  for channel,destination in [(0,0xa40008),(1,0xa50008)]:
   newest=older=raw(0.);samples=[]
   for pair in range(8):
    block=payload[(pair*2+channel)*18:(pair*2+channel+1)*18]
    assert len(block)==18 and not (block[0]&128)
    scale=(int.from_bytes(block[:2],'big')&8191)+1
    for packed in block[2:]:
     for nibble in (packed>>4,packed&15):
      signed=nibble-16 if nibble&8 else nibble
      prediction=multiply_add(coefficients[0],newest,multiply(coefficients[1],older)[0])[0]
      sample=multiply_add(raw(float(scale)),raw(float(signed*65536)),prediction)[0]
      samples.append((truncate_i32(sample)[0]>>16)&65535)
      older,newest=newest,sample
   assert native[destination:destination+512]==struct.pack('<256H',*samples)
   assert native[destination-8:destination]==bytes([0x5a])*8
   assert native[destination+512:destination+760]==bytes([0x5a])*248
   assert native[0xa30028+channel*4:0xa3002c+channel*4]==struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
'''
s=s.replace('  feed_updates+=1',oracle+'  feed_updates+=1')
s=s.replace('Sample oracle for this persistent sequence pending; initial histories zero.','Independent exact binary32 recurrence verifies all256 samples per channel, final predictor histories and output guards; initial histories zero.')
(p/'verify_initialized_decode.py').write_text(s)
