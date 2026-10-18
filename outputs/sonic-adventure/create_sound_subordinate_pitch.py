import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_subordinate_pitch';start=0x8c65ddb2;end=0x8c65de00
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_subordinate_effect_65dd26.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail subordinate pitch setter 65ddb2..65de00. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==392').replace('Source size getter verification failed','Pitch setter verification failed')
(p/f'register_{stem}.py').write_text(reg)
test=(p/'verify_sound_subordinate_effect.py').read_text().replace('sound_subordinate_effect','sound_subordinate_pitch').replace('0x8c65dd26','0x8c65ddb2').replace('0x8c6682be','0x8c6683be')
a=test.index('for present,effect,level,bank,answer');z=test.index('\n ram=',a)
test=test[:a]+"for present,effect,level,bank,answer in itertools.product([0,1],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,123]):"+test[z:]
test=test.replace('0xa08080','0xa08090').replace('bytes([0x55])*128','bytes([0x55])*144')
a=test.index(' if present and');z=test.index(' cases+=1',a)
test=test[:a]+" if present:\n  struct.pack_into('<2I',wanted,0x80,effect,level)\n  assert actual==[(0x8ca09000,effect,level)] and errors==([0x8c67239c] if answer else [])\n else:assert not actual and errors==[0x8c67217c]\n assert native[0xa08000:0xa08090]==wanted and r[15]==0x8cf00000\n"+test[z:]
a=test.index("scope='");z=test.index('\n(p/',a)
test=test[:a]+"scope='392 complete retail RAM/CPU/FPU/SR/PR comparisons of pitch setter65ddb2..65de00. Null/present objects, seven full-word patterns for each pitch argument, both FPU banks, modeled driver success/error. Independent complete144-byte object window verifies cached arguments at+80/+84 persist on driver failure, exact forwarded handle/arguments and error strings. Native error reporter with callback unset. Driver6683be modeled; connected native pitch queue, hardware and game callers remain unverified.'"+test[z:]
(p/f'verify_{stem}.py').write_text(test.replace('subordinate effect comparisons','subordinate pitch comparisons'))
