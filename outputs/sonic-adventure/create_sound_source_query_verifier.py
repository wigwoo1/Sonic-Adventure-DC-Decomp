from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_stream_input_provider.ps1').read_text().replace('sound_stream_input_provider','sound_source_query').replace('Audio format initializer','Sound source query')
(p/'build_sound_source_query.ps1').write_text(b)
s=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get','sound_source_query').replace('==140','==288').replace('0x8c65e798','0x8c65e4b8').replace('0x8c65e79c','0x8c65e58a').replace('0x64e798:0x64e79c','0x64e4b8:0x64e58a').replace('Odd decode flag','Sound source query')
(p/'register_sound_source_query.py').write_text(s)
v=p/'verify_sound_source_query.py'
v.write_text(v.read_text().replace('576 retail','288 retail'))
