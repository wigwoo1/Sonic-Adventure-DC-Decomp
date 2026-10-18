from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_input_provider_native_getters.py').read_text()
text=text.replace('services=[0x8c65e798,0x8c010100];cases=0', "buffer_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_buffer_get.dll')).sa_sound_source_buffer_get;buffer_get.argtypes=fn.argtypes\nservices=[0x8c010100];cases=0")
text=text.replace("struct.pack_into('<I',ram,0xa50018,0x8c010100)", "struct.pack_into('<I',ram,0xa4001c,0x8ca10000);struct.pack_into('<I',ram,0xa50018,0x8c010100)")
text=text.replace('0x8c6676a0:position_get}.get(address)', '0x8c6676a0:position_get,0x8c65e798:buffer_get}.get(address)')
text=text.replace('[0x8c010100]*channels+[0x8c65e798]', '[0x8c010100]*channels')
text=text.replace('input_provider_native_getters_verification.json','input_provider_native_buffer_verification.json').replace('source-buffer getter65e798 and virtual channel queries modeled','native source-buffer getter65e798; only virtual channel queries modeled').replace('Remaining source getter/query integration','Remaining virtual query integration').replace('input provider native-getter comparisons passed','input provider native-buffer comparisons passed')
(root/'verify_input_provider_native_buffer.py').write_text(text)
text=(root/'register_input_provider_native_getters.py').read_text().replace('input_provider_native_getters','input_provider_native_buffer').replace("'sa_sound_stream_position_get'}", "'sa_sound_stream_position_get','sa_sound_source_buffer_get'}").replace('Input provider native getter integration:','Input provider native buffer integration:');(root/'register_input_provider_native_buffer.py').write_text(text)
