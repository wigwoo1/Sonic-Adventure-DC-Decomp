import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_source_identity_get',0x8c65e44a,0x8c65e44e),('sound_source_error_callback_set',0x8c65e44e,0x8c65e454)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_source_initialize_65e300.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
 source.write_text(s[:a]+f'/* Retail source accessor {start:x}..{end:x}. '+('Returns identity pointer at +8.' if stem.endswith('get') else 'Stores callback R5 at +28 and user data R6 at +2c.')+' */'+s[z:])
 s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798',hex(start))
 if stem.endswith('get'):
  s=s.replace("struct.pack_into('<I',ram,0xa2001c,count)","struct.pack_into('<I',ram,0xa20008,count)")
 else:
  s=s.replace('[0x8ca20000,0x8ca10000,count,','[0x8ca20000,count,phase,')
  s=s.replace('    assert regs[0]==count',"    struct.pack_into('<2I',wanted,0x28,count,phase)")
 a=s.index("scope='");z=s.index("'),indent=2)",a)
 scope=f'140 full retail RAM/CPU/FPU/SR/PR comparisons of source accessor{start:x}..{end:x}: seven value patterns,five neighboring/user-data fixtures,both FPU banks,two stack fixtures. Independent entire64-byte object; '+('identity word+8 returned in R0,all bytes unchanged.' if stem.endswith('get') else 'callback+28=R5,user data+2c=R6,all other bytes unchanged,second store in RTS delay slot.')+' No modeled calls. Error callback invocation through this setter and fullgame remain unverified.'
 s=s[:a]+'scope='+repr(scope)+s[z+1:]
 (p/f'verify_{stem}.py').write_text(s)
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
reg=(p/'register_sound_work_accessors.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:]
(p/'register_sound_source_accessors.py').write_text(reg)
