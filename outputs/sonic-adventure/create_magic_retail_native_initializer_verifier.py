from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_magic_retail_initializer.py').read_text()
helper=(root/'verify_audio_filter_native.py').read_text()
functions=helper[helper.index('def f32('):helper.index('wrapper=')]
oracle=helper[helper.index('def trig_oracle('):helper.index('cases=0')]
insert='import math\nfrom fractions import Fraction\nfrom sh4_binary32 import multiply,multiply_add,divide,add_sub\n'+functions+oracle
insert+='''for address,stem,name in [(0x8c644be6,'file_audio_configuration_forward','sa_file_audio_configuration_forward'),(0x8c65adc0,'audio_filter_coefficients','sa_audio_filter_coefficients'),(0x8c10f90c,'math_result_error_wrapper','sa_math_result_error_wrapper'),(0x8c10fcbc,'math_trig_octant_dispatch','sa_math_trig_octant_dispatch'),(0x8c10fbb8,'math_range_reduce','sa_math_range_reduce'),(0x8c110218,'math_trig_polynomials','sa_math_cosine_polynomial'),(0x8c110238,'math_trig_polynomials','sa_math_sine_polynomial')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi
'''
text=text.replace('assets=json.loads',insert+'assets=json.loads')
text=text.replace('asset,bank,configured_result','asset,bank').replace('assets,[0,0x200000],[0,0xffffffff]','assets,[0,0x200000]')
text=text.replace('native=bytearray(ram);', "struct.pack_into('<I',ram,state+8,0x8ca30000);ram[0xa30000:0xa30040]=bytes([0x55])*64\n native=bytearray(ram);")
text=text.replace('if a in lower:return lower[a]', 'if a==0x8c644be6:actual.append(tuple(r[i] for i in range(4,7)))\n  assert a in lower,a\n  return lower[a]')
a=text.index('  assert a==0x8c644be6');b=text.index(' s=State()',a);text=text[:a]+text[b:]
text=text.replace("reference(ram,s,{0x8c644be6:hook},fpu=f,special=special)","reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)")
text=text.replace(' assert native==ram', ' nf.fpul=fpul.value\n assert native==ram').replace(' and actual==expected','')
text=text.replace('target=bytearray([0x55])*256;',"target=bytearray([0x55])*256;struct.pack_into('<I',target,8,0x8ca30000);")
text=text.replace('[(0x55555555,rate,', '[(0x8ca30000,rate,')
text=text.replace(' else:assert not actual', '''  signed=ctypes.c_int16(extension).value;pi2=struct.unpack('<f',struct.pack('<I',0x40c90fda))[0]
  angle=raw(f32(f32(f32(float(signed))*pi2)/f32(float(rate))));trig=trig_oracle(angle)
  root2=f32(math.sqrt(2.));center=f32(root2-trig);denominator=f32(root2-1.)
  coefficient=f32(f32(center-f32(math.sqrt(f32(f32(center-denominator)*f32(center+denominator)))))/denominator)
  a=int(f32(coefficient*8192.))&65535;b=int(-f32(f32(coefficient*coefficient)*4096.))&65535
  file_target=bytearray([0x55])*64;struct.pack_into('<2H',file_target,0x30,a,b)
  assert native[0xa30000:0xa30040]==file_target
 else:assert not actual''')
text=text.replace('configured_result=hex(configured_result),','')
text=text.replace('magic_retail_initializer_verification.json','magic_retail_native_initializer_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='218 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons across109 original magic8000 headers and both banks. Native format selector,initializer,metadata,extension,parameters,signed division,fill,file configuration,coefficients,trig wrapper,dispatcher,reducer,polynomials and numerical helpers connected. No modeled calls. Original64-byte prefixes/actual file sizes; independent full256-byte state,64-byte file-handle guards and coefficient fields,returned offset,header preservation and configuration forwarding. PR/SZ/RM0. Full payload/decoder/physical audio/fullgame unverified."+text[b:]
text=text.replace('original magic audio initializer comparisons passed','original magic fully native initializer comparisons passed')
(root/'verify_magic_retail_native_initializer.py').write_text(text)
