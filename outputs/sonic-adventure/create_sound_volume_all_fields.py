from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_all_fields.py').read_text().replace("fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_readback.dll')).sa_sound_backend_readback", "fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_volume_get.dll')).sa_sound_driver_volume_get")
i=s.index('cases=0')
s=s[:i]+"upper_helpers={}\nfor a,dll,name in [(0x8c669278,'sound_driver_readback_wrapper','sa_sound_driver_readback_wrapper'),(0x8c66adc4,'sound_backend_readback','sa_sound_backend_readback')]:\n helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dll}.dll')),name);helper.argtypes=abi;upper_helpers[a]=helper\n"+s[i:]
s=s.replace('signed_selector=ctypes.c_int8(value&255).value', 'signed_selector=0')
s=s.replace(' before=bytes(ram)', " struct.pack_into('<I',ram,0xa06028,0x8ca08000)\n before=bytes(ram)")
s=s.replace('  if a==0x8c6671c4:', '  if a in upper_helpers:return upper_helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  if a==0x8c6671c4:')
s=s.replace('s.r[4]=0x8ca08000 if present else 0;s.r[5]=value;s.r[6]=0x8ca20004;', 's.r[4]=0x8ca06000;s.r[5]=0x8ca20004;').replace('s.pc=0x8c66adc4','s.pc=0x8c6680ea')
s=s.replace('(0x52535053,0,0x8cefffcc,0x8cefffd0)', '(0x52535053,0,0x8cefffa4,0x8cefffa8)')
s=s.replace('(a,0x8ca08000,value,0x8ca20004+o)', '(a,0x8ca08000,0,0x8cefffe0+o)')
s=s.replace('output=bytearray(before[0xa20000:0xa20028]);output[4:6]=bytes(2)', 'output=bytearray(before[0xefffe0:0xeffffc]);output[0:2]=bytes(2)')
s=s.replace('4+o','o').replace('6+o','2+o')
s=s.replace('if (value&255)==0:', 'if True:').replace('elif (value&255)==1:', 'elif False:')
s=s.replace(' assert native[0xa20000:0xa20028]==output and', " caller=bytearray(before[0xa20000:0xa20028]);struct.pack_into('<I',caller,4,(ctypes.c_int8((value+1)&255).value*2)&0xffffffff)\n assert native[0xa20000:0xa20028]==caller and native[0xefffe0:0xeffffc]==output\n assert native[0xa06000:0xa06080]==before[0xa06000:0xa06080]\n assert")
a=s.index("scope='");z=s.index("\n(p/'analysis/",a)
s=s[:a]+"scope='448 full retail RAM/CPU/FPU/SR/PR comparisons of upper volume getter6680ea through wrapper669278,backend66adc4,all ten native field readers,native resource lookup and transfer10d460. Seven controlled payload fixtures,both banks,two transfer modes,mono/stereo,four voice formats. Independent forced selector0,signed hardware-byte1 doubling,complete28-byte stack packet,40-byte caller guards,upper128-byte/handle144-byte preservation,ten ordered MMIO reads,all field widths,format conversion,lookup arguments and stack restoration. Only polling/control services modeled;controlled MMIO device data. Physical hardware and full game integration unverified.'"+s[z:]
s=s.replace('backend_readback_native_all_fields_verification.json','sound_volume_all_fields_verification.json')
(p/'verify_sound_volume_all_fields.py').write_text(s)
reg=(p/'register_backend_readback_native_all_fields.py').read_text().replace("['backend_readback']","['sound_volume']").replace('native_all_fields','all_fields').replace('backend_readback_all_fields_verification.json','sound_volume_all_fields_verification.json').replace("'sa_sound_resource_lookup'}", "'sa_sound_resource_lookup','sa_sound_driver_volume_get','sa_sound_driver_readback_wrapper'}").replace('native all fields:', 'all fields volume:')
(p/'register_sound_volume_all_fields.py').write_text(reg)
