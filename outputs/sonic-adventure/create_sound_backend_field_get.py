from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_descriptor_byte_query.py').read_text().replace('sound_descriptor_byte_query','sound_backend_field_get').replace('0x8c6691d0','0x8c66bc24').replace('0x8c6671c4','0x8c667150')
s=s.replace('[0,255],[0,0x200000],[0,0x70000063]', '[0,127,128,65535,0x80000000,0xffffffff],[0,0x200000],[0x70000063]')
s=s.replace('native[r[5]&0xffffff]=value',"struct.pack_into('<I',native,r[5]&0xffffff,value)").replace('mem[r[5]&0xffffff]=value',"struct.pack_into('<I',mem,r[5]&0xffffff,value)")
s=s.replace('selector).value*32)&','selector).value*32+8)&').replace('(value,0x12345678)','((0xdeadbeef&0xffff0000)|(value&65535),0x12345678)')
a=s.index("scope='");z=s.index("'),indent=2",a);s=s[:a]+"scope='768 full retail RAM/CPU/FPU/SR/PR comparisons of56-instruction field reader66bc24. Disabled context,null handle,null output,success;four signed selectors/descriptors,six full-word lower payloads,both banks. Native error recorder;lower667150 modeled. Independent address plus8,low16-bit output with upper/neighbor words preserved,ignored lower return,exact error globals. Native lower transfer/hardware/game integration unverified;semantic field name remains provisional."+s[z:]
(p/'verify_sound_backend_field_get.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([('sound_backend_field_get',0x8c66bc24,0x8c66bc94)])+':'+reg[z:]
(p/'register_sound_backend_field_get.py').write_text(reg.replace('==800','==768'))
