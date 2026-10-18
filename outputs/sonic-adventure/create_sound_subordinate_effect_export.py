from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_subordinate_pan.ps1').read_text().replace('sound_subordinate_pan_65dcca.tsv','sound_subordinate_effect_65dd26.tsv').replace('8c65dcca 8c65dd26','8c65dd26 8c65de00')
(p/'ghidra/export_sound_subordinate_effect.ps1').write_text(s)
