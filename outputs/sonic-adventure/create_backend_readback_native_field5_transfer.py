from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_field4_transfer.py').read_text()
s=s.replace('cases=0', "field5_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field5_get.dll')).sa_sound_backend_field5_get;field5_get.argtypes=abi\ncases=0")
s=s.replace('  service(a,m,r);return 0', '  if a==0x8c66b9b0:return field5_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0')
s=s.replace('0x8c66ba2c,0x8c66babc]', '0x8c66ba2c,0x8c66babc,0x8c66b9b0]')
s=s.replace('for i in [1,2,16]]', 'for i in [1,2,16,17]]')
s=s.replace('  else:output[4+o]=(value+i)&255', '  elif a==0x8c66b9b0:output[4+o]=(value+21)&255\n  else:output[4+o]=(value+i)&255')
s=s.replace('(device+16)&0xffffffff,(device+20)&0xffffffff]', '(device+16)&0xffffffff,(device+16)&0xffffffff,(device+20)&0xffffffff]')
s=s.replace('Polling/control services and two other field readers modeled', 'Native byte reader66b9b0 connected through667188,66910c and transfer;independent raw address+17 selects byte1 from aligned MMIO word+16,single packet byte+5,ordered nine MMIO reads and neighbor preservation. Polling/control services and one other field reader66accc modeled')
s=s.replace('backend_readback_native_field4_transfer_verification.json', 'backend_readback_native_field5_transfer_verification.json')
(p/'verify_backend_readback_native_field5_transfer.py').write_text(s)
reg=(p/'register_backend_readback_native_field4_transfer.py').read_text().replace('native_field4_transfer','native_field5_transfer').replace("'sa_sound_backend_field4_get'}", "'sa_sound_backend_field4_get','sa_sound_backend_field5_get'}").replace('native field4 transfer:', 'native field5 transfer:')
(p/'register_backend_readback_native_field5_transfer.py').write_text(reg)
