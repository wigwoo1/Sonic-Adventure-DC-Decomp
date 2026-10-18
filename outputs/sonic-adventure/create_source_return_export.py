from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_source_query_65e4b8.ps1').read_text().replace('sound_source_query_65e4b8.tsv','sound_source_return_65e644.tsv').replace('8c65e4b8 8c65e644','8c65e644 8c65e712')
(p/'ghidra/export_sound_source_return_65e644.ps1').write_text(s)
