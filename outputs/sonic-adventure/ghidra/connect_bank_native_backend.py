from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_sound_bank_load_native_open_dispatch.py').read_text()
anchor=' fn=processor.sa_sound_request_process;fn.argtypes=abi;cases=0'
helpers=[(0x8c6004d4,'sa_file_backend_open','file_backend_open'),(0x8c600464,'sa_file_handle_open_initialize','file_handle_open_initialize'),(0x8c5ffa08,'sa_file_error_set','file_error_set'),(0x8c645348,'sa_file_name_upper','file_filename_lookup'),(0x8c6453c0,'sa_file_name_equal','file_filename_lookup'),(0x8c6453e2,'sa_file_name_equal_folded','file_filename_lookup'),(0x8c645442,'sa_file_name_checksum','file_filename_lookup'),(0x8c6455ce,'sa_file_filename_lookup','file_filename_lookup')]
setup=' for address,name,dll in '+repr(helpers)+':\n  helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=(helper,False)\n'
t=t.replace(anchor,setup+anchor)
t=t.replace('assert regs[4]==0 and regs[5]', 'assert regs[4]==250 and regs[5]')
a=t.index('  def file_service(');b=t.index('  for target in [0x8cb10000',a)
t=t[:a]+'  hooks={}\n'+t[b:]
t=t.replace('   if t==0x8cb40000:file_service(0x8c6016d6,m,r,native_files);return 0\n','')
t=t.replace("struct.pack_into('<I',memory,0xb0403c,0x8cb40000)","struct.pack_into('<I',memory,0xb0403c,0x8c6004d4)")
anchor="   struct.pack_into('<I',memory,0xf00000,0)"
fixture="""   struct.pack_into('<I',memory,0x673414,0x8cb02000)
   for offset,value in [(0xb02000,1),(0xb02010,0x8cb05000),(0xb02018,0x8cb01000),(0xb0200c,0x8cb03000),(0xb0203c,0x8cb03000),(0xb01000,0x8cb02000),(0xb03004,0x8cb20000),(0xb03040,0x8cb30000),(0xb03048,0x8cb10000),(0xb05000,1)]:struct.pack_into('<I',memory,offset,value)
   struct.pack_into('<II',memory,0xb05010,100,len(bank_data))
   name=b'E_00B7.MLT'
   seed=struct.unpack_from('<h',binary,0x64554e-0x10000)[0]
   memory[0xb05019]=((seed+sum(name))&0xffffffff)>>2&255
   memory[0xb0501a:0xb0501a+len(name)+1]=name+b'\\0'
   struct.pack_into('<H',memory,0x66c534,3)
"""
t=t.replace(anchor,fixture+anchor)
t=t.replace("  assert reference_files==native_files and [event[0] for event in native_files]==[0x8c6016d6]", "  assert struct.unpack_from('<I',native,0xb02030)[0]==0\n  assert struct.unpack_from('<II',native,0xb01004)==(1,250)")
t=t.replace('sound_bank_load_native_open_dispatch_verification.json','sound_bank_load_native_backend_verification.json')
t=t.replace('native open dispatcher with explicit backend-open and device read/start/poll hooks; native disc/filesystem internals not executed; hardware readiness modeled, no audio or filesystem service execution.', 'native open dispatcher, backend open, filename checksum/comparison/lookup, handle initializer and error setter. Only device read/start/poll callbacks are hooked; hardware readiness modeled. Disc initialization and device internals remain unrecovered; no audio execution.')
t=t.replace('real bank load with native open dispatch comparisons passed','real bank load with native backend comparisons passed')
(p/'verify_sound_bank_load_native_backend.py').write_text(t)
