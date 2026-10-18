from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_source_locks_65e368.ps1').read_text().replace('sound_source_locks_65e368.tsv','file_handle_allocate_644b6e.tsv').replace('8c65e368 8c65e3c6','8c644b6e 8c644be6')
(p/'ghidra/export_file_handle_allocate_644b6e.ps1').write_text(s)
