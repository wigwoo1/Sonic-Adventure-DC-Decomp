from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'build_sound_work_reset.ps1').read_text().replace('sound_work_reset','sound_work_pool_initialize');(p/'build_sound_work_pool_initialize.ps1').write_text(s)
s=(p/'register_sound_work_reset.py').read_text().replace('sound_work_reset','sound_work_pool_initialize').replace('0x8c65b606','0x8c65b5e8').replace('0x8c65b624','0x8c65b5fc').replace('0x64b606:0x64b624','0x64b5e8:0x64b5fc').replace('==140','==24');(p/'register_sound_work_pool_initialize.py').write_text(s)
