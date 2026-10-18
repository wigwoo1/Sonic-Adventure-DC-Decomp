from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_work_reset.ps1').read_text().replace('sound_work_reset_65b606.tsv','sound_source_sizes_65e79c.tsv').replace('8c65b606 8c65b624','8c65e79c 8c65e7a4')
(p/'ghidra/export_sound_source_sizes.ps1').write_text(s)
