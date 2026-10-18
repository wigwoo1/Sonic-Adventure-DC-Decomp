import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_source_contiguous_check'
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,'8c65e712','8c65e798','sound_source_setup_65e712.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
source.write_text(s[:a]+'/* Retail contiguous-capacity check 8c65e712..8c65e798. R4 source,\n * R5 mode, R6 requested bytes, R7 output count. Signed minimum of request,\n * capacity and ring remainder plus mirror; invalid mode reports error -3. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
reg=(p/'register_sound_work_accessors.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+"for stem,start,end in [('sound_source_contiguous_check',0x8c65e712,0x8c65e798)]:"+reg[z:]
reg=reg.replace("evidence['cases_passed']==140","evidence['cases_passed']==2100")
(p/f'register_{stem}.py').write_text(reg)
