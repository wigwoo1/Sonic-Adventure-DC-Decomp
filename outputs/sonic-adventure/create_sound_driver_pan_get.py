from pathlib import Path
p=Path(__file__).resolve().parent
(p/'build_sound_driver_pan_get.ps1').write_text((p/'build_sound_driver_volume_get.ps1').read_text().replace('sound_driver_volume_get','sound_driver_pan_get'))
s=(p/'verify_sound_driver_volume_get.py').read_text().replace('sound_driver_volume_get','sound_driver_pan_get').replace('0x8c6680ea','0x8c668112').replace('((value if value<128 else value-256)*2)&0xffffffff','(int((value if value<128 else value-256)*15/127))&0xffffffff')
s=s.replace('s.r[5]=dest;','s.r[5]=0;s.r[6]=dest;').replace('(r[6]&0xffffff)+2','(r[6]&0xffffff)+3').replace('(r[6]&0xffffff)+2','(r[6]&0xffffff)+3')
i=s.index('cases=0');s=s[:i]+"division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/signed_division.dll')).sa_signed_division;division.argtypes=abi\n"+s[i:]
s=s.replace('  assert a==0x8c669278;', '  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  assert a==0x8c669278;')
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='1024 full retail RAM/CPU/FPU/SR/PR comparisons of26-instruction pan getter668112..668146. All256 backend byte values,both banks,success/ffffffff status,selector0. Native signed division10ca5c;independent signed-byte*15/127 truncation,16-byte output guards,144-byte driver preservation,backend handle/selector/stack packet pointer and status propagation. Backend669278 modeled;other selectors and native backend/hardware/game integration unverified.'"+s[z:]
(p/'verify_sound_driver_pan_get.py').write_text(s)
reg=(p/'register_sound_driver_volume_get.py').read_text().replace('sound_driver_volume_get','sound_driver_pan_get').replace(str(0x8c6680ea),str(0x8c668112)).replace(str(0x8c668112),str(0x8c668146))
# Avoid sequential address substitution changing the start twice.
a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([('sound_driver_pan_get',0x8c668112,0x8c668146)])+':'+reg[z:]
(p/'register_sound_driver_pan_get.py').write_text(reg)
