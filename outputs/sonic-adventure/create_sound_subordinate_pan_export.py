from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_driver_controls.ps1').read_text().replace('sound_driver_controls_65dc1c.tsv','sound_subordinate_pan_65dcca.tsv').replace('8c65dc1c 8c65dc64','8c65dcca 8c65dd26')
(p/'ghidra/export_sound_subordinate_pan.ps1').write_text(s)
