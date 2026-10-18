from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_byte.py').read_text();i=s.index('cases=0');s=s[:i]+"word_read=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/device_word_read.dll')).sa_device_word_read;word_read.argtypes=abi\n"+s[i:]
s=s.replace('  if a==0x8c66910c:\n', '  if a==0x8c66910c:return word_read(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  if a==0x8c10d460:\n   assert r[6]==4 and r[7]==1 and all(m[(r[15]&0xffffff)+i]==0 for i in range(8))\n')
s=s.replace('0x8c66910c:read_hook','0x8c10d460:read_hook')
s=s.replace('assert reads==[(0x8ca30400+0x55*64+signed_selector*32)&0xffffffff]', 'assert reads==[(0xa0800000+0x8ca30400+0x55*64+signed_selector*32)&0xffffffff]')
s=s.replace('aligned four-byte lower66910c and nine other field readers modeled','native device-word reader66910c connected;independent device alias,4-byte count,stride1 and zero stack arguments. Transfer10d460 and nine other field readers modeled')
s=s.replace('backend_readback_native_byte_verification.json','backend_readback_native_word_verification.json')
(p/'verify_backend_readback_native_word.py').write_text(s)
reg=(p/'register_backend_readback_native_byte.py').read_text().replace('native_byte','native_word').replace("'sa_sound_byte_read'}","'sa_sound_byte_read','sa_device_word_read'}")
(p/'register_backend_readback_native_word.py').write_text(reg)
