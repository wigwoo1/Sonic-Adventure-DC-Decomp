from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_work_reset.ps1').read_text().replace('sound_work_reset_65b606.tsv','sound_work_pool_initialize_65b5e8.tsv').replace('8c65b606 8c65b624','8c65b5e8 8c65b624')
(p/'ghidra/export_sound_work_pool_initialize.ps1').write_text(s)
