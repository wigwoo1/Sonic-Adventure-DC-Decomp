import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_linear_source_identity_get',0x8c65e16e,0x8c65e172),('sound_linear_source_error_callback_set',0x8c65e172,0x8c65e178),('sound_linear_source_reset',0x8c65e178,0x8c65e182)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_65e000.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');c=source.read_text();a=c.index('/* Static C recovery');z=c.index(' */',a)+3
 c=c[:a]+f'/* Retail linear-source accessor {start:x}..{end:x}. */'+c[z:];source.write_text(c)
 s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798',hex(start))
 if stem.endswith('get'):
  s=s.replace("struct.pack_into('<I',ram,0xa2001c,count)","struct.pack_into('<I',ram,0xa20008,count)")
  effect='identity word+8 returned in R0,all object bytes preserved.'
 elif stem.endswith('_set'):
  s=s.replace('[0x8ca20000,0x8ca10000,count,','[0x8ca20000,count,phase,')
  s=s.replace('    assert regs[0]==count',"    struct.pack_into('<2I',wanted,0x1c,count,phase)")
  effect='callback+1c=R5,user data+20=R6,all other bytes preserved; second store in RTS delay slot.'
 else:
  s=s.replace("struct.pack_into('<I',ram,0xa2001c,count)","struct.pack_into('<I',ram,0xa20018,count)")
  s=s.replace('    assert regs[0]==count',"    struct.pack_into('<2I',wanted,0xc,count,0)")
  effect='remaining capacity+c receives length+18,reservation offset+10 zero,all other bytes preserved.'
 a=s.index("scope='");z=s.index("'),indent=2)",a)
 scope=f'140 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source routine{start:x}..{end:x}: seven value patterns,five neighboring/user-data fixtures,both banks,two stack fixtures. Independent entire64-byte object-plus-guard window: '+effect+' No modeled calls. Linear-source construction/reservation integration and fullgame remain unverified.'
 s=s[:a]+'scope='+repr(scope)+s[z+1:];(p/f'verify_{stem}.py').write_text(s)
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
reg=(p/'register_sound_work_accessors.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:]
(p/'register_sound_linear_source_accessors.py').write_text(reg)
