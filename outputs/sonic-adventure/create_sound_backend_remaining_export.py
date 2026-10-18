from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_backend_fieldc_get.ps1').read_text().replace('sound_backend_fieldc_get_66bd44.tsv','sound_backend_remaining_66b9b0.tsv').replace('8c66bd44 8c66bdd4','8c66b9b0 8c66bb4c')
(p/'ghidra/export_sound_backend_remaining.ps1').write_text(s)
