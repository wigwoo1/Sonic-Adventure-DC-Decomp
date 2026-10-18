import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_error_callback_set',0x8c667b70,0x8c667b7a),('sound_error_report',0x8c667b7a,0x8c667b9a)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_error_callbacks_667b70.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+f'/* Retail sound error callback routine {start:x}..{end:x}. */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
 (p/f'verify_{stem}.py').write_text("from verify_sound_error_callbacks_connected import *\n")
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:];reg=reg.replace('==800','==80')
(p/'register_sound_error_callbacks.py').write_text(reg)
