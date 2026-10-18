import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_work_sample_count_get'
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,'8c65bc06','8c65bc0a','sound_work_controls_65bc06.tsv'],check=True)
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798','0x8c65bc06').replace('0xa2001c','0xa2002c')
s=s.replace('two-instruction source buffer getter8c65e798','two-instruction work sample count getter65bc06..65bc0a,field2c')
(p/f'verify_{stem}.py').write_text(s)
build=(p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem);(p/f'build_{stem}.ps1').write_text(build)
subprocess.run(['powershell','-NoProfile','-File',str(p/f'build_{stem}.ps1')],check=True)
subprocess.run([sys.executable,str(p/f'verify_{stem}.py')],check=True)
s=(p/'register_sound_source_sizes.py').read_text();start=s.index('for stem,start,end in [');end=s.index('\n',start)
s=s[:start]+"for stem,start,end in [('sound_work_sample_count_get',0x8c65bc06,0x8c65bc0a)]:"+s[end:]
(p/f'register_{stem}.py').write_text(s)
