from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_work_reset.ps1').read_text().replace('sound_work_reset_65b606.tsv','file_decode_pool_initialize_644b64.tsv').replace('8c65b606 8c65b624','8c644b64 8c644b6e')
(p/'ghidra/export_file_decode_pool_initialize.ps1').write_text(s)
