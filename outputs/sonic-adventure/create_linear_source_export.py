from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_work_reset.ps1').read_text().replace('sound_work_reset_65b606.tsv','sound_linear_source_65e000.tsv').replace('8c65b606 8c65b624','8c65e000 8c65e368')
(p/'ghidra/export_sound_linear_source.ps1').write_text(s)
