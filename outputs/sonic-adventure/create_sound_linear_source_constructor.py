import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_linear_source_create';start=0x8c65e0ea;end=0x8c65e150
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_constructor_65e0dc.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail linear-source constructor: first free of32 slots,36-byte objects and native reset. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_linear_source_capacity.ps1').read_text().replace('sound_linear_source_capacity',stem))
s=(p/'verify_sound_source_create.py').read_text()
for old,new in [('sound_source_create',stem),('sound_source_reset','sound_linear_source_reset'),('0x8c65e454','0x8c65e178'),('0x8c65e3c6','0x8c65e0ea'),('0x8a65b8','0x8a6138'),('range(65)','range(33)'),('range(64)','range(32)'),('*48','*36'),('slot<64','slot<32'),('3072','1152'),('3080','1160'),('3088','1168')]:s=s.replace(old,new)
a=s.index('  for off,value in [');z=s.index('\n assert native[pool:',a)
s=s[:a]+"  for off,value in [(0,0x8c67c1fc),(4,1),(8,0x8c672424),(0xc,size),(0x10,0),(0x14,0x8ca40000),(0x18,size),(0x1c,0x8c65e0bc),(0x20,selected)]:struct.pack_into('<I',wanted,offset+off,value)"+s[z:]
a=s.index("(p/'analysis/")
scope='462 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source constructor65e0ea..65e150 with native reset65e178,no modeled calls. All32 candidate slots plus full pool,nonzero occupied flag patterns1/2/128/ffffffff,seven length patterns,both banks. Independent entire1152-byte pool and8-byte guards,first-free selection,returned pointer or null,all9 constructed words: method table8c67c1fc,active1,identity8c672424,remaining=length,offset0,buffer,length,default callback65e0bc,self userdata. Other slots preserved. Retail default callback not executed. Connected pool lifecycle/reservations and game callers remain unverified.'
s=s[:a]+f"(p/'analysis/{stem}_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope={scope!r}),indent=2)+'\\n')\nprint(cases,'native linear source constructor comparisons passed')\n"
(p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==462')
(p/f'register_{stem}.py').write_text(reg)
