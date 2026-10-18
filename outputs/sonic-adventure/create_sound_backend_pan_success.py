from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_backend_volume_get.py').read_text().replace('sound_backend_volume_get','sound_backend_pan_get').replace('0x8c66bdd4','0x8c66bb4c')
a=s.index('for mode,selector,descriptor,value,bank,sr0');z=s.index('\n ram=',a);s=s[:a]+"for mode,selector,descriptor,value,bank,sr0,channels in itertools.product([3],[0,1,128,255],[0,127,128,255],[0,1,63,64,127,128,255],[0,0x200000],[0x70000063],[0,1,2,3]):"+s[z:]
i=s.index(' native=bytearray(ram)');s=s[:i]+" voice=0xa08000+0xa8+ctypes.c_int8(descriptor).value*44;ram[voice+5]=channels\n"+s[i:]
s=s.replace('selector).value*32+1)&','selector).value*32+2)&')
s=s.replace(' reference(ram,s,{0x8c667188:read_hook},fpu=f)'," special={'macl':0};reference(ram,s,{0x8c667188:read_hook},fpu=f,special=special)")
s=s.replace(' assert r[0]==code', " assert macl.value==special['macl']\n assert r[0]==code")
needle="  assert struct.unpack_from('<2I',native,0xa20000)==((0xdeadbeef&0xffffff00)|value,0x12345678)"
s=s.replace(needle,"  result=ctypes.c_int8(value).value\n  if channels==2:\n   if selector==0:result+=64\n   elif selector==1:result=(result if result else -1)-64\n   result*=2\n  assert struct.unpack_from('<2I',native,0xa20000)==((0xdeadbeef&0xffffff00)|(result&255),0x12345678)")
a=s.index("scope='");z=s.index("'),indent=2",a);s=s[:a]+"scope='896 complete retail RAM/CPU/FPU/SR/PR/MACL comparisons of91-instruction backend pan reader66bb4c. Four signed selectors/descriptors,seven byte boundaries,channels0/1/2/3,both FPU banks. Native error recorder;lower667188 modeled. Independent address plus2,mono/nonstereo unchanged byte,stereo selector0 bias+64 and selector1 zero-to-minus1/bias-64 followed by doubling with final byte wrapping. One-byte output/neighbor guards,ignored lower status,restored stack. Native lower transfer/hardware/game integration pending. Separate56-case native diagnostics report covers errors."+s[z:]
(p/'verify_sound_backend_pan_get.py').write_text(s)
reg=(p/'register_sound_backend_volume_get.py').read_text().replace('sound_backend_volume_get','sound_backend_pan_get').replace('==384','==896');a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([('sound_backend_pan_get',0x8c66bb4c,0x8c66bc02)])+':'+reg[z:]
(p/'register_sound_backend_pan_get.py').write_text(reg)
