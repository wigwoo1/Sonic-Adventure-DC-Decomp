from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_pan_get_selectors.py').read_text();i=s.index('cases=0');s=s[:i]+"wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_readback_wrapper.dll')).sa_sound_driver_readback_wrapper;wrapper.argtypes=abi\n"+s[i:]
s=s.replace('  assert a==0x8c669278;', '  if a==0x8c669278:return wrapper(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  assert a==0x8c66adc4;')
s=s.replace('{0x8c669278:backend}','{0x8c66adc4:backend}')
s=s.replace('Backend669278 modeled','Native readback wrapper669278 connected;backend66adc4 modeled').replace('sound_pan_get_selectors_verification.json','sound_pan_get_native_wrapper_verification.json')
(p/'verify_sound_pan_get_native_wrapper.py').write_text(s)
reg=(p/'register_sound_pan_get_selectors.py').read_text().replace('selectors','native_wrapper').replace("'sa_signed_division'}","'sa_signed_division','sa_sound_driver_readback_wrapper'}")
(p/'register_sound_pan_get_native_wrapper.py').write_text(reg)
