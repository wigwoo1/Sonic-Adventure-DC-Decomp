import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_subordinate_field8_set',0x8c65dc1c,0x8c65dc54),('sound_subordinate_fieldc_set',0x8c65dc54,0x8c65dc64)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_driver_controls_65dc1c.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+f'/* Retail sound subordinate field setter {start:x}..{end:x}. Field meanings await caller recovery. */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
 s=(p/'verify_sound_subordinate_field_setter_template.py').read_text().replace('SETTER_STEM',stem).replace('SETTER_ADDRESS',hex(start))
 (p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:];reg=reg.replace('==800','==224')
(p/'register_sound_subordinate_field_setters.py').write_text(reg)
