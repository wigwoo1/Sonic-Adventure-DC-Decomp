from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_input_provider_native_query.py').read_text()
s=s.replace('services=[0x8c65e368,0x8c65e380,0x8c10cbb8];cases=0',"remainder=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/integer_remainder.dll')).sa_integer_remainder;remainder.argtypes=fn.argtypes\nservices=[0x8c65e368,0x8c65e380];cases=0")
s=s.replace('0x8c65e4b8:query}', '0x8c65e4b8:query,0x8c10cbb8:remainder}')
s=s.replace('[0x8c65e368,0x8c10cbb8,0x8c65e380]]','[0x8c65e368,0x8c65e380]]')
s=s.replace('input_provider_native_query_verification','input_provider_native_remainder_verification').replace('only lock enter/exit and signed remainder modeled.','native signed remainder10cbb8; only lock enter/exit modeled.').replace('Native lock/remainder integration,','Native lock integration,').replace('native-query comparisons','native-remainder comparisons')
(p/'verify_input_provider_native_remainder.py').write_text(s)
r=(p/'register_input_provider_native_query.py').read_text().replace('input_provider_native_query','input_provider_native_remainder').replace('native source query integration:','native remainder integration:')
(p/'register_input_provider_native_remainder.py').write_text(r)
e=(p/'ghidra/export_sound_source_query_65e4b8.ps1').read_text().replace('sound_source_query_65e4b8.tsv','sound_source_locks_65e368.tsv').replace('8c65e4b8 8c65e644','8c65e368 8c65e3c6')
(p/'ghidra/export_sound_source_locks_65e368.ps1').write_text(e)
