from pathlib import Path
p=Path(__file__).resolve().parent;name='sound_source_capacity'
(p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c65e462').replace('0x8c65e79c','0x8c65e49a').replace('0x64e798','0x64e462').replace('0x64e79c','0x64e49a').replace('==140','==280')
(p/f'register_{name}.py').write_text(r)
path=p/f'source/sa_{name}.c';s=path.read_text();a=s.index('/* Static C recovery');z=s.index('SA_STAGE_EXPORT',a);path.write_text(s[:a]+'/* Retail source capacity query 8c65e462..8c65e49a. Invalid modes optionally invoke error callback. */\n'+s[z:])
