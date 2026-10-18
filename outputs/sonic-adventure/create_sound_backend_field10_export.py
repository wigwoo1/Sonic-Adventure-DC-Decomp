from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_backend_fieldc_get.ps1').read_text().replace('sound_backend_fieldc_get_66bd44.tsv','sound_backend_field10_get_66accc.tsv').replace('8c66bd44 8c66bdd4','8c66accc 8c66adc4')
(p/'ghidra/export_sound_backend_field10_get.ps1').write_text(s)
