from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'register_sound_work_reset.py').read_text().replace('sound_work_reset','sound_work_start').replace('0x8c65b606','0x8c65b778').replace('0x8c65b624','0x8c65b78e').replace('0x64b606:0x64b624','0x64b778:0x64b78e').replace('Sound work reset','Sound work start')
(p/'register_sound_work_start.py').write_text(s)
