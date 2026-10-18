from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_stream_default_input.ps1').read_text().replace('stream_default_input.tsv','stream_work_update_65b930.tsv').replace('8c667384 8c6673a2','8c65b930 8c65bc9c')
(p/'ghidra/export_stream_work_update_65b930.ps1').write_text(s)
