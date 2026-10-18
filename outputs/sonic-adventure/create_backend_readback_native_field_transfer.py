from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_field.py').read_text();i=s.index('cases=0');s=s[:i]+"field_word=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_field_word_read.dll')).sa_sound_field_word_read;field_word.argtypes=abi\n"+s[i:]
s=s.replace('mmio={device:payload}',"field_payload=int.from_bytes(bytes((value+4+i)&255 for i in range(4)),'little');mmio={device:payload,(device+8)&0xffffffff:field_payload}")
s=s.replace('if a!=device or n!=4:', 'if a not in mmio or n!=4:').replace('reads.append(a);return payload','reads.append(a);return mmio[a]')
a=s.index('  if a==0x8c667150:');z=s.index('  assert a in dict(specs)',a)
s=s[:a]+"  if a==0x8c667150:\n   field_reads.append(r[4]);return field_word(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n"+s[z:]
s=s.replace(',0x8c667150:field_hook','')
s=s.replace('assert reads==[(0xa0800000+0x8ca30400+0x55*64+signed_selector*32)&0xffffffff]*3','assert reads==[device]*3+[(device+8)&0xffffffff]')
s=s.replace('Lower667150,polling/control services and six other field readers modeled','Native field-word reader667150 and transfer connected;independent second MMIO address+8 and low16-bit packet output. Polling/control services and six other field readers modeled')
s=s.replace('backend_readback_native_field_verification.json','backend_readback_native_field_transfer_verification.json')
(p/'verify_backend_readback_native_field_transfer.py').write_text(s)
reg=(p/'register_backend_readback_native_field.py').read_text().replace('native_field','native_field_transfer').replace("'sa_sound_backend_field_get'}","'sa_sound_backend_field_get','sa_sound_field_word_read'}")
(p/'register_backend_readback_native_field_transfer.py').write_text(reg)
