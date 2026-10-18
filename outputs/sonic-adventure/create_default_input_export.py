from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_stream_default_output.ps1').read_text().replace('stream_default_output.tsv','stream_default_input.tsv').replace('8c6673a2 8c6674a2','8c667384 8c6673a2')
(p/'ghidra/export_stream_default_input.ps1').write_text(s)
