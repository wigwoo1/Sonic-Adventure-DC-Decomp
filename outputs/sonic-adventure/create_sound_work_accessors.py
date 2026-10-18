import subprocess
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_work_state_get',0x8c65b760,0x8c65b764,1),('sound_work_source_set',0x8c65b764,0x8c65b768,8),('sound_work_sample_limit_set',0x8c65b774,0x8c65b778,0x38)]
for stem,start,end,offset in specs:
 subprocess.run([__import__('sys').executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_work_constructor_65b624.tsv'],check=True)
 s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798',hex(start))
 if offset==1:
  s=s.replace("struct.pack_into('<I',ram,0xa2001c,count)","ram[0xa20001]=count&255")
  s=s.replace('assert regs[0]==count','assert regs[0]==(ctypes.c_int8(count&255).value&0xffffffff)')
 else:
  s=s.replace('[0x8ca20000,0x8ca10000,count,','[0x8ca20000,count,count,')
  s=s.replace('    assert regs[0]==count',f"    struct.pack_into('<I',wanted,{offset},count)")
 start_scope=s.index("scope='");end_scope=s.index("'),indent=2)",start_scope)
 scope=f'140 full retail RAM/CPU/FPU/SR/PR comparisons of two-instruction work accessor{start:x}..{end:x},both banks,seven values0/1/2/127/128/255/ffffffff,five neighboring-field fixtures,two stack values. Independent entire64-byte object; '+('signed byte1 returned in R0,all bytes unchanged.' if offset==1 else f'field{offset:x} receives R5,all other bytes unchanged.')+' No modeled calls;connected lifecycle/fullgame unverified.'
 s=s[:start_scope]+"scope='"+scope+s[end_scope:]
 (p/f'verify_{stem}.py').write_text(s)
 build=(p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem)
 (p/f'build_{stem}.ps1').write_text(build)
 subprocess.run(['powershell','-NoProfile','-File',str(p/f'build_{stem}.ps1')],check=True)
 subprocess.run([__import__('sys').executable,str(p/f'verify_{stem}.py')],check=True)
s=(p/'register_sound_source_sizes.py').read_text()
start=s.index('for stem,start,end in [');end=s.index('\n',start)
s=s[:start]+'for stem,start,end in '+repr([(name,start,end) for name,start,end,offset in specs])+':'+s[end:]
(p/'register_sound_work_accessors.py').write_text(s)
