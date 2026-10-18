from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_public_initialize_headers.py').read_text()
entries=[(0x8c65bc3c,'sound_work_format_get'),(0x8c65bc48,'sound_work_sample_rate_get'),(0x8c65bc54,'sound_work_channel_count_get'),(0x8c65bc60,'sound_work_sample_bits_get'),(0x8c6676a4,'sound_stream_format_get'),(0x8c6676aa,'sound_stream_sample_rate_get'),(0x8c6676ae,'sound_stream_channel_count_get'),(0x8c6676b2,'sound_stream_sample_bits_get')]
at=s.index('assets=json.loads(')
load='for address,stem in '+repr(entries)+":\n lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi\n"
s=s[:at]+load+s[at:]
extra=''' if success:
  untouched=[(state,256),(0xa60000,256),(0xa30000,64),(0xa80000,48)]
  before_queries=[bytes(native[address:address+length]) for address,length in untouched]
  expected_metadata=[(0x8c65bc3c,0),(0x8c65bc48,struct.unpack_from('>I',data,8)[0]),(0x8c65bc54,data[7]),(0x8c65bc60,16)]
  for address,value in expected_metadata:
   s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=address
   reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
   assert lower[address](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
   assert r[0]==value and r[15]==0x8cf00000
   for (region,length),before in zip(untouched,before_queries):assert native[region:region+length]==before
'''
s=s.replace(' records.append(',extra+' records.append(')
s=s.replace('public_initialize_headers_verification','initialized_metadata_verification')
s=s.replace('Full payload/decoder/physical audio/fullgame unverified.', 'Four work metadata wrappers and native stream getters query initialized format0,sample rate from BE header8,channels header7,and sample bits header6. Independent header-derived R0 values,whole work/stream/handle/source objects unchanged,persistent CPU/FPU and restored stack on all876 query calls. Full payload/decoder/physical audio/fullgame unverified.')
s=s.replace('sample bits header6','decoded PCM sample bits16').replace('all876 query calls','all872 query calls')
(p/'verify_initialized_metadata.py').write_text(s)
s=(p/'register_work_native_configuration.py').read_text().replace('work_native_configuration','initialized_metadata').replace('==54','==218')
start=s.index('names={');end=s.index('\n',start)
s=s[:start]+'names='+repr({'sa_'+stem for address,stem in entries}|{'sa_sound_stream_work_initialize','sa_sound_stream_work_update'})+s[end:]
s=s.replace('Native work constructor buffer/callback configuration:', 'Original-header initialization and native metadata queries:')
(p/'register_initialized_metadata.py').write_text(s)
