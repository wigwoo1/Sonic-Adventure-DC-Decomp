from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_complete_native_commits.py').read_text()
needle='services=[0x8c6676f6,0x8c6677b6,0x8c6677d8,0x8c010300]'
assert needle in s
s=s.replace(needle,"for address,stem in [(0x8c6676f6,'sound_stream_total_samples_get'),(0x8c6677b6,'sound_stream_decoded_bytes_get'),(0x8c6677d8,'sound_stream_decoded_samples_get')]:\n    helpers[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),'sa_'+stem);helpers[address].argtypes=fn.argtypes\nservices=[0x8c010300]")
s=s.replace('    stream_before=bytes',"    for offset,value in [(0x18,total),(0xa4,36),(0xa0,32)]:struct.pack_into('<I',ram,0xa70000+offset,value)\n    stream_before=bytes")
s=s.replace('[0x8c6676f6,0x8c6677b6,0x8c6677d8]+([0x8c010300] if notify else [])','([0x8c010300] if notify else [])')
s=s.replace('Total/byte/sample getters and notification callback modeled.','Total/byte/sample getters execute natively; only notification callback modeled.')
s=s.replace('work_complete_native_commits_verification','work_complete_native_getters_verification').replace('native commits work completion comparisons','native getters work completion comparisons')
(p/'verify_work_complete_native_getters.py').write_text(s)
r=(p/'register_work_complete_native_commits.py').read_text().replace('work_complete_native_commits','work_complete_native_getters').replace("'sa_sound_source_return'}","'sa_sound_source_return','sa_sound_stream_total_samples_get','sa_sound_stream_decoded_bytes_get','sa_sound_stream_decoded_samples_get'}").replace('Native source commits in stream work completion:','Native completion getters in stream work completion:')
(p/'register_work_complete_native_getters.py').write_text(r)
