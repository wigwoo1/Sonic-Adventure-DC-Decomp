from pathlib import Path
p=Path(__file__).resolve().parent
(p/'build_sound_backend_volume_get.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan','sound_backend_volume_get'))
s=(p/'verify_sound_descriptor_byte_query.py').read_text().replace('sound_descriptor_byte_query','sound_backend_volume_get').replace('0x8c6691d0','0x8c66bdd4').replace('0x8c6671c4','0x8c667188').replace('itertools.product(range(4),','itertools.product([0,1,3],').replace('selector).value*32)&','selector).value*32+1)&').replace('(value,0x12345678)','((0xdeadbeef&0xffffff00)|value,0x12345678)')
a=s.index("scope='");z=s.index("'),indent=2",a);s=s[:a]+"scope='384 full retail RAM/CPU/FPU/SR/PR comparisons of50-instruction volume-field getter66bdd4. Disabled context,null handle,success;four signed selector/descriptor fixtures,two byte payloads,both banks,two SR fixtures. Native error recorder;lower667188 modeled. Independent signed address plus1,one-byte output with remaining bytes preserved,ignored lower return and exact errors. Null output is not checked and is excluded;native transfer/hardware/game integration unverified."+s[z:]
(p/'verify_sound_backend_volume_get.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([('sound_backend_volume_get',0x8c66bdd4,0x8c66be38)])+':'+reg[z:]
(p/'register_sound_backend_volume_get.py').write_text(reg.replace('==800','==384'))
