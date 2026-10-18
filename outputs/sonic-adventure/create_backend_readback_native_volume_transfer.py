from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_volume.py').read_text();i=s.index('cases=0');s=s[:i]+"volume_byte=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_byte_read.dll')).sa_sound_volume_byte_read;volume_byte.argtypes=abi\n"+s[i:]
s=s.replace('volume_reads.append(r[4]);m[r[5]&0xffffff]=(value+2)&255;r[0]=answer;return 0','volume_reads.append(r[4]);return volume_byte(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)')
s=s.replace('0x8c667188:volume_hook','').replace(']},},fpu',']}},fpu')
s=s.replace('  else:output[4+o]=(value+i)&255','  elif a==0x8c66bdd4:output[4+o]=(value+1)&255\n  else:output[4+o]=(value+i)&255')
s=s.replace('assert reads==[(0xa0800000+0x8ca30400+0x55*64+signed_selector*32)&0xffffffff]','assert reads==[(0xa0800000+0x8ca30400+0x55*64+signed_selector*32)&0xffffffff]*2')
s=s.replace('Lower667188,polling/control services and eight other field readers modeled','Native volume byte reader667188 connected through device-word/transfer. Two aligned MMIO reads of same controlled word;descriptor selects byte0,volume selects byte1. Polling/control services and eight other field readers modeled')
s=s.replace('backend_readback_native_volume_verification.json','backend_readback_native_volume_transfer_verification.json')
(p/'verify_backend_readback_native_volume_transfer.py').write_text(s)
reg=(p/'register_backend_readback_native_volume.py').read_text().replace('native_volume','native_volume_transfer').replace("'sa_sound_backend_volume_get'}","'sa_sound_backend_volume_get','sa_sound_volume_byte_read'}")
(p/'register_backend_readback_native_volume_transfer.py').write_text(reg)
