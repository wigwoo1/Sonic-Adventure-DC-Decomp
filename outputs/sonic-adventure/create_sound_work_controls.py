import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_work_completion_callback_set',0x8c65bc12,0x8c65bc1c,0x48),('sound_work_remaining_limit_set',0x8c65bc1c,0x8c65bc20,0x3c)]
for stem,start,end,offset in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','stream_work_update_65b930.tsv'],check=True)
 s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798',hex(start))
 s=s.replace('0xa20040','0xa20100').replace('bytes([0x55])*64','bytes([0x55])*256')
 s=s.replace('[0x8ca20000,0x8ca10000,count,','[0x8ca20000,count,phase,')
 expected=f"    struct.pack_into('<I',wanted,{offset},count)"
 if offset==0x48:expected+=";struct.pack_into('<I',wanted,0x4c,phase)"
 s=s.replace('    assert regs[0]==count',expected)
 start_scope=s.index("scope='");end_scope=s.index("'),indent=2)",start_scope)
 scope=f'140 full retail RAM/CPU/FPU/SR/PR comparisons of work control{start:x}..{end:x},both banks,seven R5 values0/1/2/127/128/255/ffffffff,five R6 patterns0/1/2/3/ffffffff,two stack values. Independent whole256-byte object: '+('callback48=R5/user4c=R6;' if offset==0x48 else 'remaining limit3c=R5;')+'all other bytes preserved. No modeled calls. Connected output scheduling/notification and fullgame unverified.'
 s=s[:start_scope]+"scope='"+scope+s[end_scope:]
 (p/f'verify_{stem}.py').write_text(s)
 build=(p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem)
 (p/f'build_{stem}.ps1').write_text(build)
 subprocess.run(['powershell','-NoProfile','-File',str(p/f'build_{stem}.ps1')],check=True)
 subprocess.run([sys.executable,str(p/f'verify_{stem}.py')],check=True)
s=(p/'register_sound_source_sizes.py').read_text();start=s.index('for stem,start,end in [');end=s.index('\n',start)
s=s[:start]+'for stem,start,end in '+repr([(name,start,end) for name,start,end,offset in specs])+':'+s[end:]
(p/'register_sound_work_controls.py').write_text(s)
