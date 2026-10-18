import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_subordinate_field8_get',0x8c65de00,0x8c65de1c),('sound_subordinate_fieldc_get',0x8c65de1c,0x8c65de38),('sound_subordinate_volume_get',0x8c65de38,0x8c65de56),('sound_subordinate_pan_get',0x8c65de56,0x8c65de7a),('sound_subordinate_effect_get',0x8c65de7a,0x8c65ded4)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_subordinate_queries_65de00.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+f'/* Retail sound-object query {start:x}..{end:x}. */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
 (p/f'verify_{stem}.py').write_text(f"from verify_sound_subordinate_queries import run\nrun({stem!r})\n")
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:];reg=reg.replace('==800','==280')
(p/'register_sound_subordinate_queries.py').write_text(reg)
