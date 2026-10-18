from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'register_work_native_sizes.py').read_text().replace('work_native_sizes','work_native_allocation')
s=s.replace("'sa_sound_source_mirror_size_get'}", "'sa_sound_source_mirror_size_get','sa_sound_stream_create','sa_sound_stream_destroy','sa_file_decode_handle_allocate','sa_file_decode_handle_release','sa_byte_fill'}")
s=s.replace('Native work constructor size accessors and callback configuration:', 'Fully native work and stream construction:')
s=s.replace(",'sa_byte_fill'",'')
(p/'register_work_native_allocation.py').write_text(s)
