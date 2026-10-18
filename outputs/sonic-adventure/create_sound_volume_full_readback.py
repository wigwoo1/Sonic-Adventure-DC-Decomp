from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_pan_full_readback.py').read_text().replace("build/sound_driver_pan_get.dll')).sa_sound_driver_pan_get", "build/sound_driver_volume_get.dll')).sa_sound_driver_volume_get")
s=s.replace('s.r[5]=value;s.r[6]=0x8ca20004;', 's.r[5]=0x8ca20004;').replace('s.pc=0x8c668112','s.pc=0x8c6680ea')
s=s.replace('signed_selector=ctypes.c_int8(value&255).value','signed_selector=0')
s=s.replace('(a,0x8ca08000,value,0x8cefffe0+o)','(a,0x8ca08000,0,0x8cefffe0+o)')
a=s.index(' result=ctypes.c_int8((value+2)');z=s.index(" struct.pack_into('<I',output",a);s=s[:a]+" scaled=ctypes.c_int8((value+1)&255).value*2\n"+s[z:]
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='112 full retail RAM/CPU/FPU/SR/PR comparisons of connected upper volume getter6680ea,wrapper669278,backend66adc4,native descriptor/volume/pan readers,byte/device-word readers and transfer10d460. Seven payload fixtures,backend mono/stereo,both banks,transfer context modes,two modeled service returns. Independent hardware byte1 selection and signed doubling,caller40-byte output guards,upper128-byte/driver144-byte preservation,three aligned MMIO reads,forced selector0 and stack restoration. Seven other field readers/polling modeled;controlled MMIO,real hardware/game integration unverified.'"+s[z:]
s=s.replace('sound_pan_full_readback_verification.json','sound_volume_full_readback_verification.json')
(p/'verify_sound_volume_full_readback.py').write_text(s)
reg=(p/'register_sound_pan_full_readback.py').read_text().replace('sound_pan','sound_volume').replace('sa_sound_driver_pan_get','sa_sound_driver_volume_get')
(p/'register_sound_volume_full_readback.py').write_text(reg)
