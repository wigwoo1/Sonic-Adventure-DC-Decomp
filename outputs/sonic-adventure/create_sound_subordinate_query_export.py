from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_subordinate_effect.ps1').read_text().replace('sound_subordinate_effect_65dd26.tsv','sound_subordinate_queries_65de00.tsv').replace('8c65dd26 8c65de00','8c65de00 8c65ded4')
(p/'ghidra/export_sound_subordinate_queries.ps1').write_text(s)
