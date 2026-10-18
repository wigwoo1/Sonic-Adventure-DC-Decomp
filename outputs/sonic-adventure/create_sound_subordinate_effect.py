import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_subordinate_effect';start=0x8c65dd26;end=0x8c65ddb2
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_subordinate_effect_65dd26.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail subordinate effect setter65dd26..65ddb2;span includes skipped literals. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==432')
(p/f'register_{stem}.py').write_text(reg)
