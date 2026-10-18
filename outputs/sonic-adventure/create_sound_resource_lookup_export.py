from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_backend_field10_get.ps1').read_text().replace('sound_backend_field10_get_66accc.tsv','sound_resource_lookup_65a900.tsv').replace('8c66accc 8c66adc4','8c65a900 8c65aa40')
(p/'ghidra/export_sound_resource_lookup.ps1').write_text(s)
