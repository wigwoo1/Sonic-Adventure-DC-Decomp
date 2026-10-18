from pathlib import Path
p=Path(__file__).resolve().parent
name='sound_stream_work_tick'
(p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
path=p/f'source/sa_{name}.c';s=path.read_text();a=s.index('/* Static C recovery');z=s.index('SA_STAGE_EXPORT',a);s=s[:a]+'/* Retail stream work dispatcher 8c65bb4e..8c65bb88. Subordinate calls use explicit SH4 ABI services. */\n'+s[z:];path.write_text(s)
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c65bb4e').replace('0x8c65e79c','0x8c65bb88').replace('0x64e798','0x64bb4e').replace('0x64e79c','0x64bb88').replace('==140','==100')
(p/f'register_{name}.py').write_text(r)
