from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_fieldc_transfer.py').read_text()
s=s.replace('cases=0', "field14_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field14_get.dll')).sa_sound_backend_field14_get;field14_get.argtypes=abi\ncases=0")
s=s.replace('mmio={(device+20)&0xffffffff:fieldc_payload,', "field14_payload=int.from_bytes(bytes((value+16+i)&255 for i in range(4)),'little');mmio={(device+28)&0xffffffff:field14_payload,(device+20)&0xffffffff:fieldc_payload,")
s=s.replace('  service(a,m,r);return 0', '  if a==0x8c66ba2c:return field14_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0')
s=s.replace('0x8c66bcb4,0x8c66bd44]', '0x8c66bcb4,0x8c66bd44,0x8c66ba2c]')
s=s.replace('  else:output[4+o]=(value+i)&255', "  elif a==0x8c66ba2c:struct.pack_into('<I',output,4+o,field14_payload)\n  else:output[4+o]=(value+i)&255")
s=s.replace('assert reads==[device]*3+', 'assert reads==[(device+28)&0xffffffff]+[device]*3+')
s=s.replace('Polling/control services and four other field readers modeled', 'Native field14 reader66ba2c connected through66910c and transfer;independent first device address+28,full32-bit packet output+20,ordered seven MMIO reads and neighbor preservation. Polling/control services and three other field readers modeled')
s=s.replace('backend_readback_native_fieldc_transfer_verification.json', 'backend_readback_native_field14_transfer_verification.json')
(p/'verify_backend_readback_native_field14_transfer.py').write_text(s)
reg=(p/'register_backend_readback_native_fieldc_transfer.py').read_text().replace('native_fieldc_transfer','native_field14_transfer').replace("'sa_sound_backend_fieldc_get'}", "'sa_sound_backend_fieldc_get','sa_sound_backend_field14_get'}").replace('native fieldc transfer:', 'native field14 transfer:')
(p/'register_backend_readback_native_field14_transfer.py').write_text(reg)
