from pathlib import Path
p=Path(__file__).resolve().parent;name='sound_work_pool_update'
(p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c65bbd4').replace('0x8c65e79c','0x8c65bc02').replace('0x64e798','0x64bbd4').replace('0x64e79c','0x64bc02').replace('==140','==512')
(p/f'register_{name}.py').write_text(r)
