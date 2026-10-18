from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_work_pool_initialize.py').read_text()
s=s.replace('cases=0',"helpers={0x8c10d31c:fill}\nfor address,stem in [(0x8c667370,'sound_stream_pool_initialize'),(0x8c644b64,'file_decode_pool_initialize')]:\n helpers[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helpers[address].argtypes=abi\ncases=0")
s=s.replace(' native=bytearray(ram);', " ram[0x8d61a4:0x8d6714]=bytes([pattern^0x55])*1392\n ram[0x890678:0x890828]=bytes([pattern^0xaa])*432\n native=bytearray(ram);")
start=s.index('  if address==0x8c667370:');end=s.index('\n s=State()',start)
s=s[:start]+'''  assert address in helpers,address
  calls.append(address)
  return helpers[address](m,f,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
'''+s[end:]
s=s.replace('reference(ram,s,{0x8c667370:hook}', 'reference(ram,s,{}')
s=s.replace('and calls==expected==[0x8c667370]', 'and calls==[0x8c667370,0x8c644b64,0x8c10d31c,0x8c10d31c,0x8c10d31c]')
s=s.replace(' assert r[15]==0x8cf00000;cases+=1', ''' assert native[0x8d61ac:0x8d670c]==bytes(1376) and native[0x890680:0x890820]==bytes(416)
 for start,end,value in [(0x8d61a4,0x8d61ac,pattern^0x55),(0x8d670c,0x8d6714,pattern^0x55),(0x890678,0x890680,pattern^0xaa),(0x890820,0x890828,pattern^0xaa)]:assert native[start:end]==bytes([value])*8
 assert r[15]==0x8cf00000;cases+=1''')
s=s.replace('sound_work_pool_initialize_verification','native_audio_pools_verification')
s=s.replace('Stream-pool initializer667370 modeled and observed before clearing.', 'Stream initializer667370 and handle initializer644b64 native,no modeled calls. Exact ordered calls:handle pool cleared first,then stream pool,then work pool. Independent work1152/stream1376/handle416 bytes zero with distinct initial patterns and8-byte guards around all pools unchanged.')
(p/'verify_native_audio_pools.py').write_text(s)
s=(p/'register_work_native_configuration.py').read_text().replace('work_native_configuration','native_audio_pools').replace('==54','==24')
start=s.index('names={');end=s.index('\n',start)
s=s[:start]+"names={'sa_sound_work_pool_initialize','sa_sound_stream_pool_initialize','sa_file_decode_pool_initialize'}"+s[end:]
s=s.replace('Native work constructor buffer/callback configuration:', 'Fully native audio work/stream/decode-handle pool initialization:')
(p/'register_native_audio_pools.py').write_text(s)
