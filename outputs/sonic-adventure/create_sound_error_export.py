from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_driver_controls.ps1').read_text().replace('sound_driver_controls_65dc1c.tsv','sound_error_callbacks_667b70.tsv').replace('8c65dc1c 8c65dc64','8c667b70 8c667b9a')
(p/'ghidra/export_sound_error_callbacks.ps1').write_text(s)
