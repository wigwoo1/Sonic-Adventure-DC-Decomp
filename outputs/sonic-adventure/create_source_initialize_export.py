from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_work_reset.ps1').read_text().replace('sound_work_reset_65b606.tsv','sound_source_initialize_65e300.tsv').replace('8c65b606 8c65b624','8c65e300 8c65e462')
(p/'ghidra/export_sound_source_initialize.ps1').write_text(s)
