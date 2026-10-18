from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_work_reset.ps1').read_text().replace('sound_work_reset_65b606.tsv','sound_work_controls_65bc06.tsv').replace('8c65b606 8c65b624','8c65bc06 8c65bd24')
(p/'ghidra/export_sound_work_controls.ps1').write_text(s)
