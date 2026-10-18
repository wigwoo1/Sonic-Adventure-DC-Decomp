from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_work_reset.ps1').read_text().replace('sound_work_reset_65b606.tsv','sound_source_setup_65e712.tsv').replace('8c65b606 8c65b624','8c65e712 8c65e79c')
(p/'ghidra/export_sound_source_setup.ps1').write_text(s)
