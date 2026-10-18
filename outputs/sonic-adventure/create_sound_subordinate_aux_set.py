import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_subordinate_aux_set';start=0x8c65df2a;end=0x8c65df5e
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_subordinate_tail_65ded4.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail auxiliary sound setter65df2a..65df5e; field names provisional. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
s=(p/'verify_sound_subordinate_pitch.py').read_text().replace('sound_subordinate_pitch',stem).replace('0x8c65ddb2','0x8c65df2a').replace('0x8c6683be','0x8c668498').replace("wanted,0x80,effect,level","wanted,0x88,effect,level").replace("errors==([0x8c67239c] if answer else [])","errors==[]")
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='392 complete retail RAM/CPU/FPU/SR/PR comparisons of auxiliary sound setter65df2a..65df5e. Null/present objects,seven full-word patterns per argument,both banks,two downstream return values. Independent144-byte object window verifies writes+88/+8c before tail call668498, exact handle/arguments, return propagation and restored stack. Null reports67217c through native callback-unset reporter. Downstream routine modeled; semantic field names, hardware and game integration unverified.'"+s[z:]
(p/f'verify_{stem}.py').write_text(s.replace('subordinate pitch comparisons','auxiliary setter comparisons'))
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==392')
(p/f'register_{stem}.py').write_text(reg)
