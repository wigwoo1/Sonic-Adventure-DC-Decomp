from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_stream_range_split_668af4.ps1').read_text().replace('stream_range_split_668af4.tsv','byte_copy_10d25c.tsv').replace('8c668af4 8c668b50','8c10d25c 8c10d31c')
(p/'ghidra/export_byte_copy_10d25c.ps1').write_text(s)
