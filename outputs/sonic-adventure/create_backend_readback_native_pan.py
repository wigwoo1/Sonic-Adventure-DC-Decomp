from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_volume_transfer.py').read_text();i=s.index('cases=0');s=s[:i]+"pan_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_pan_get.dll')).sa_sound_backend_pan_get;pan_get.argtypes=abi\n"+s[i:]
s=s.replace('value,bank,answer,mode in itertools.product','value,bank,answer,mode,channels in itertools.product').replace('[0,0xffffffff],[0,1]):','[0,0xffffffff],[0,1],[1,2]):',1)
s=s.replace(" struct.pack_into('<I',ram,0xa091b8,0x8ca30000)"," struct.pack_into('<I',ram,0xa091b8,0x8ca30000);ram[0xa09000+0xa8+0x55*44+5]=channels")
s=s.replace('  service(a,m,r);return 0','  if a==0x8c66bb4c:return pan_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0')
s=s.replace('if a not in [0x8c6691d0,0x8c66bdd4]', 'if a not in [0x8c6691d0,0x8c66bdd4,0x8c66bb4c]')
s=s.replace('  elif a==0x8c66bdd4:output[4+o]=(value+1)&255',"  elif a==0x8c66bdd4:output[4+o]=(value+1)&255\n  elif a==0x8c66bb4c:\n   result=ctypes.c_int8((value+2)&255).value\n   if channels==2:\n    if (value&255)==0:result+=64\n    elif (value&255)==1:result=(result if result else -1)-64\n    result*=2\n   output[4+o]=result&255")
s=s.replace('&0xffffffff]*2','&0xffffffff]*3')
s=s.replace('assert volume_reads==[(0x8ca30400+0x55*64+signed_selector*32+1)&0xffffffff]','assert volume_reads==[(0x8ca30400+0x55*64+signed_selector*32+i)&0xffffffff for i in [1,2]]\n assert macl.value==0x55*44')
s=s.replace('56 complete','112 complete').replace('Polling/control services and eight other field readers modeled','Native pan reader66bb4c connected;independent mono/stereo output byte+3 using hardware byte2. Three aligned MMIO reads;channels1/2. Polling/control services and seven other field readers modeled')
s=s.replace('backend_readback_native_volume_transfer_verification.json','backend_readback_native_pan_verification.json')
(p/'verify_backend_readback_native_pan.py').write_text(s)
reg=(p/'register_backend_readback_native_volume_transfer.py').read_text().replace('native_volume_transfer','native_pan').replace('==56','==112').replace("'sa_sound_volume_byte_read'}","'sa_sound_volume_byte_read','sa_sound_backend_pan_get'}")
(p/'register_backend_readback_native_pan.py').write_text(reg)
