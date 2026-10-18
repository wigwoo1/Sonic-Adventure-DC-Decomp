from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_stream_work_update_65b930.ps1').read_text().replace('stream_work_update_65b930.tsv','stream_range_split_668af4.tsv').replace('8c65b930 8c65bc9c','8c668af4 8c668b50')
(p/'ghidra/export_stream_range_split_668af4.ps1').write_text(s)
