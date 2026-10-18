import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_subordinate_pitch_get';start=0x8c65ded4;end=0x8c65df12
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_subordinate_tail_65ded4.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail pitch cache getter65ded4..65df12. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
s=(p/'verify_sound_subordinate_queries.py').read_text();a=s.index('specs=[');z=s.index('\nreporter=',a);s=s[:a]+f'specs=[({stem!r},{start},None)]'+s[z:]
s=s.replace('range(32)','range(36)').replace("'<32I'","'<36I'").replace('0xa08080','0xa08090').replace("pair=stem.endswith('effect_get')","pair=True").replace('words[30]','words[32]').replace('words[31]','words[33]')
a=s.index("  scope='");z=s.index('\n  (p/',a);s=s[:a]+"  scope='280 complete retail RAM/CPU/FPU/SR/PR comparisons of pitch cache getter65ded4..65df12. Null/present objects,seven word fixtures,both banks,distinct/aliased outputs and five initial register fixtures. Independent144-byte object preserved; getters write+80 then+84, aliased outputs retain+84. Null reports67217c through native callback-unset reporter then writesffffffff to both outputs. Full16-byte output/guards and stack checked. No modeled calls; invalid output pointers and game integration unverified.'"+s[z:]
(p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==280')
(p/f'register_{stem}.py').write_text(reg)
