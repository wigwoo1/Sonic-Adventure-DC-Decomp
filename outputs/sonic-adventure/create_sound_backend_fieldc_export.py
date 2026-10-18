from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_backend_field8_get.ps1').read_text().replace('sound_backend_field8_get_66bcb4.tsv','sound_backend_fieldc_get_66bd44.tsv').replace('8c66bcb4 8c66bd44','8c66bd44 8c66bdd4')
(p/'ghidra/export_sound_backend_fieldc_get.ps1').write_text(s.rstrip()+'\n')
