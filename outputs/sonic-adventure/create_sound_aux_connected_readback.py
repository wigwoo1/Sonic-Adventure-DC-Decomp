from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_subordinate_aux_set.py').read_text()
i=s.index('reporter=');s=s[:i]+"getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_aux_get.dll')).sa_sound_subordinate_aux_get;getter.argtypes=abi\n"+s[i:]
block=""" for alias in [0,1]:
  native[0xa20000:0xa20010]=bytes([0x66])*16;ram[0xa20000:0xa20010]=bytes([0x66])*16
  first=0x8ca20004;second=first if alias else 0x8ca20008
  s.r[4:7]=[0x8ca08000 if present else 0,first,second];s.pc=0x8c65df5e;s.pr=0x8c000100
  r[4]=s.r[4];r[5]=first;r[6]=second;pr.value=s.pr
  reference(ram,s,{},fpu=f,special={'macl':macl.value})
  assert getter(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  output=bytearray([0x66]*16)
  if present:
   for pointer,value in [(first,effect),(second,level)]:struct.pack_into('<I',output,(pointer&0xffffff)-0xa20000,value)
  assert native[0xa20000:0xa20010]==output and native[0xa08000:0xa08090]==wanted and r[15]==0x8cf00000
"""
i=s.index(' cases+=1');s=s[:i]+block+s[i:]
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='392 persistent setter/query sequences,1176 public operations. Seven full-word patterns for both auxiliary fields,null/present objects,both banks,two modeled downstream returns. After setter65df2a,getter65df5e runs on the same RAM/CPU/FPU state with distinct then aliased outputs. Independent144-byte cache and16-byte guarded outputs verify+88/+8c readback and alias order;null paths preserve outputs. Complete retail comparisons after each operation. Driver668498 modeled;hardware and game integration unverified.'"+s[z:]
s=s.replace('sound_subordinate_aux_set_verification.json','sound_aux_connected_readback_verification.json')
(p/'verify_sound_aux_connected_readback.py').write_text(s)
reg=(p/'register_sound_subordinate_pan_native_diagnostics.py').read_text();a=reg.index('for stem in');z=reg.index('\n names=',a)
reg=reg[:a]+"for stem in ['sound_aux']:\n report='analysis/sound_aux_connected_readback_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==392"+reg[z:]
a=reg.index(' names=');z=reg.index(';found=set()',a);reg=reg[:a]+" names={'sa_sound_subordinate_aux_set','sa_sound_subordinate_aux_get','sa_sound_error_report'}"+reg[z:]
reg=reg.replace('native_diagnostics','connected_readback')
(p/'register_sound_aux_connected_readback.py').write_text(reg)
