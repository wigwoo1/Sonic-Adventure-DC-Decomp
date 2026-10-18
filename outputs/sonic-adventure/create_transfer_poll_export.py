from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_resource_lookup.ps1').read_text().replace('sound_resource_lookup_65a900.tsv','engine_transfer_poll_10ee20.tsv').replace('8c65a900 8c65aa40','8c10ee20 8c10ef20')
(p/'ghidra/export_engine_transfer_poll.ps1').write_text(s)
