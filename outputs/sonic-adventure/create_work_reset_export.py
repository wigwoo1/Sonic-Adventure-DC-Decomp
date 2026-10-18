from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_stream_buffer_configuration.ps1').read_text()
s=s.replace('stream_buffer_configuration_667600.tsv','sound_work_reset_65b606.tsv').replace('8c667600 8c66768c','8c65b606 8c65b624')
(p/'ghidra/export_sound_work_reset.ps1').write_text(s)
