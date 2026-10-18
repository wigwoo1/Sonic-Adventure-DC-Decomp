from pathlib import Path
p=Path(__file__).resolve().parent
stem='sound_backend_field10_get'
s=(p/'verify_sound_backend_fieldc_get.py').read_text().replace('sound_backend_fieldc_get',stem).replace('0x8c66bd44','0x8c66accc').replace('66bd44','66accc').replace('0x8c66910c','0x8c667150').replace('32+20','32+24')
s=s.replace('bank,sr0 in itertools.product', 'bank,sr0,fmt in itertools.product').replace('[0x70000063]):','[0x70000063],range(4)):')
s=s.replace(' native=bytearray(ram)', " voice=0x8ca080a8+ctypes.c_int8(descriptor).value*44\n struct.pack_into('<I',ram,voice&0xffffff,fmt);ram[(voice+7+ctypes.c_int8(selector).value)&0xffffff]=0x81\n native=bytearray(ram)")
s=s.replace(' actual=[];expected=[]', ' actual=[];expected=[];lookups=[]')
needle='  assert a==0x8c667150,a'
s=s.replace(needle,"  if a==0x8c65a900:\n   lookups.append(tuple(r[i] for i in [4,5,6,7]));native[r[6]&0xffffff:(r[6]&0xffffff)+4]=bytes([0x31])*4;native[r[7]&0xffffff:(r[7]&0xffffff)+4]=bytes([0x42])*4;r[0]=123;return 0\n"+needle)
s=s.replace(' s=State();', " def lookup(mem,r,sr):\n  mem[r[6]&0xffffff:(r[6]&0xffffff)+4]=bytes([0x31])*4;mem[r[7]&0xffffff:(r[7]&0xffffff)+4]=bytes([0x42])*4;r[0]=123;return sr\n s=State();")
s=s.replace('reference(ram,s,{0x8c667150:read_hook},fpu=f)', 'reference(ram,s,{0x8c667150:read_hook,0x8c65a900:lookup},fpu=f,special={\'macl\':0})')
s=s.replace("  assert struct.unpack_from('<2I',native,0xa20000)==(value,0x12345678)", "  signed=ctypes.c_int32(value).value\n  result=(value*2)&0xffffffff if fmt==1 else (int(abs(signed)//2)*(-1 if signed<0 else 1))&0xffffffff if fmt==2 else value\n  assert struct.unpack_from('<2I',native,0xa20000)==(result,0x12345678)\n  assert lookups==[(0x52535053,0xffffff81,0x8cefffe4,0x8cefffe8)]\n  assert macl.value==(ctypes.c_int8(descriptor).value*44)&0xffffffff")
s=s.replace('  assert not actual', '  assert not actual and not lookups')
a=s.index("scope='");z=s.index("'),indent=2",a)
s=s[:a]+"scope='3072 full retail RAM/CPU/FPU/SR/PR comparisons of103-instruction field reader66accc. Disabled context,null handle,null output,success;four signed selectors/descriptors,six32-bit payloads,both banks,four voice formats. Native diagnostic;lower667150 and resource lookup65a900 modeled. Independent read address+24,full32-bit output:format1 doubles with wrap,format2 signed truncation toward zero,formats0/3 unchanged. Independent lookup tag52535053,signed voice byte and stack output pointers;lookup outputs ignored. Neighbor words preserved,ignored lower returns,error globals and descriptor MACL checked. Physical hardware/full game integration and semantic field name unverified."+s[z:]
(p/f'verify_{stem}.py').write_text(s)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_backend_fieldc_get.ps1').read_text().replace('sound_backend_fieldc_get',stem))
s=(p/'register_sound_backend_fieldc_get.py').read_text().replace('sound_backend_fieldc_get',stem).replace(str(0x8c66bd44),str(0x8c66accc)).replace(str(0x8c66bdb4),str(0x8c66ad9a)).replace('==768','==3072')
(p/f'register_{stem}.py').write_text(s)
