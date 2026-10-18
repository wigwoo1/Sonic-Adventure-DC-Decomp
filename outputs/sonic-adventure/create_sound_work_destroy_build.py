from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'build_sound_work_reset.ps1').read_text().replace('sound_work_reset','sound_work_destroy').replace('Sound work reset','Sound work destruction')
(p/'build_sound_work_destroy.ps1').write_text(s)
s=(p/'register_sound_work_reset.py').read_text().replace('sound_work_reset','sound_work_destroy').replace('0x8c65b606','0x8c65b72c').replace('0x8c65b624','0x8c65b760').replace('0x64b606:0x64b624','0x64b72c:0x64b760').replace('==140','==84')
(p/'register_sound_work_destroy.py').write_text(s)
