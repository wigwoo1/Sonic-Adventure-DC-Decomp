from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_linear_source_constructor.ps1').read_text().replace('-postScript SeedLinearSource.java ','')
s=s.replace('sound_linear_source_constructor_65e0dc.tsv','sound_driver_controls_65dc1c.tsv').replace('8c65e0dc 8c65e150','8c65dc1c 8c65dc64')
(p/'ghidra/export_sound_driver_controls.ps1').write_text(s)
