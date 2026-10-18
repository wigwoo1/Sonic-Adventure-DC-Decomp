from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_field8_transfer.py').read_text()
s=s.replace('cases=0', "fieldc_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_fieldc_get.dll')).sa_sound_backend_fieldc_get;fieldc_get.argtypes=abi\ncases=0")
s=s.replace('mmio={device:payload,', "fieldc_payload=int.from_bytes(bytes((value+12+i)&255 for i in range(4)),'little');mmio={(device+20)&0xffffffff:fieldc_payload,device:payload,")
s=s.replace('  service(a,m,r);return 0', '  if a==0x8c66bd44:return fieldc_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0')
s=s.replace('0x8c66bc24,0x8c66bcb4]', '0x8c66bc24,0x8c66bcb4,0x8c66bd44]')
s=s.replace('  else:output[4+o]=(value+i)&255', "  elif a==0x8c66bd44:struct.pack_into('<I',output,4+o,fieldc_payload)\n  else:output[4+o]=(value+i)&255")
s=s.replace('assert reads==[device]*3+[(device+8)&0xffffffff,(device+4)&0xffffffff]', 'assert reads==[device]*3+[(device+8)&0xffffffff,(device+4)&0xffffffff,(device+20)&0xffffffff]')
s=s.replace('Polling/control services and five other field readers modeled', 'Native fieldc reader66bd44 connected through66910c and transfer;independent device address+20,full32-bit packet output+12,ordered six MMIO reads and neighbor preservation. Polling/control services and four other field readers modeled')
s=s.replace('backend_readback_native_field8_transfer_verification.json', 'backend_readback_native_fieldc_transfer_verification.json')
(p/'verify_backend_readback_native_fieldc_transfer.py').write_text(s)
reg=(p/'register_backend_readback_native_field8_transfer.py').read_text().replace('native_field8_transfer','native_fieldc_transfer').replace("'sa_sound_backend_field8_get'}", "'sa_sound_backend_field8_get','sa_sound_backend_fieldc_get'}").replace('native field8 transfer:', 'native fieldc transfer:')
(p/'register_backend_readback_native_fieldc_transfer.py').write_text(reg)
