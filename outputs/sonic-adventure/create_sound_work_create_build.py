from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'build_sound_work_reset.ps1').read_text().replace('sound_work_reset','sound_work_create').replace('Sound work reset','Sound work constructor')
(p/'build_sound_work_create.ps1').write_text(s)
