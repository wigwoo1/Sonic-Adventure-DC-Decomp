import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_linear_source_pool_initialize',0x8c65e0c0,0x8c65e0dc),('sound_linear_source_destroy',0x8c65e150,0x8c65e16e)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_65e000.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
 source.write_text(s[:a]+f'/* Retail linear-source lifecycle {start:x}..{end:x}. */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
 s=(p/'verify_sound_source_pool_lifecycle_template.py').read_text().replace('LIFECYCLE_STEM',stem).replace('LIFECYCLE_ADDRESS',hex(start))
 for old,new in [('0x8a65b8','0x8a6138'),('0x8c8a65b8','0x8c8a6138'),('0x67c264','0x67c22c'),('3072','1152'),('3080','1160'),('3088','1168'),('0xa20038','0xa2002c'),('0xa20030','0xa20024'),('bytes(48)','bytes(36)'),('*48','*36'),(')*64',')*44'),('entire48-byte','entire36-byte'),('65e39c..65e3b8','65e0c0..65e0dc'),('65e42c..65e44a','65e150..65e16e')]:
  s=s.replace(old,new)
 s=s.replace(')*44',')*52').replace('(0x8ca20000,0,48)','(0x8ca20000,0,36)')
 (p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_source_pool_lifecycle.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:]
(p/'register_sound_linear_source_lifecycle.py').write_text(reg)
