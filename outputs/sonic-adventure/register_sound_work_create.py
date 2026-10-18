from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'register_sound_work_reset.py').read_text().replace('sound_work_reset','sound_work_create').replace('0x8c65b606','0x8c65b624').replace('0x8c65b624\x27,source','0x8c65b700\x27,source').replace('0x64b606:0x64b624','0x64b624:0x64b700').replace("==140","==108").replace('Sound work reset','Sound work constructor')
exec(compile(s,str(__file__),'exec'))
