import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_source_pool_initialize',0x8c65e39c,0x8c65e3b8,168),('sound_source_destroy',0x8c65e42c,0x8c65e44a,48)]
for stem,start,end,cases in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_source_initialize_65e300.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
 description='If signed initialization counter is nonpositive, increment it and clear3072 source-pool bytes.' if stem.endswith('initialize') else 'Null pointer is a no-op; otherwise clear48-byte source object.'
 source.write_text(s[:a]+f'/* Retail source lifecycle {start:x}..{end:x}. {description} */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
 s=(p/'verify_sound_source_pool_lifecycle_template.py').read_text().replace('LIFECYCLE_STEM',stem).replace('LIFECYCLE_ADDRESS',hex(start))
 (p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_work_accessors.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end) for stem,start,end,cases in specs])+':'+reg[z:]
reg=reg.replace("assert evidence['cases_passed']==140", "assert evidence['cases_passed']==(168 if stem.endswith('initialize') else 48)")
(p/'register_sound_source_pool_lifecycle.py').write_text(reg)
