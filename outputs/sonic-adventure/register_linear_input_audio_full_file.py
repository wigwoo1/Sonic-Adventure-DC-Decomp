from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'register_native_audio_full_file.py').read_text().replace('native_audio_full_file','linear_input_audio_full_file').replace("r['full_file_published']","r['full_file_available']")
s=s.replace("names={'sa_sound_source_pool_initialize'","names={'sa_sound_linear_source_pool_initialize','sa_sound_linear_source_create','sa_sound_linear_source_reset','sa_sound_linear_source_capacity','sa_sound_linear_source_query','sa_sound_linear_source_commit','sa_sound_linear_source_return','sa_sound_linear_source_lock_enter','sa_sound_linear_source_lock_exit','sa_sound_source_pool_initialize'")
s=s.replace('Complete original FISHMISS audio streaming:','Complete original FISHMISS decoding from native linear input:')
exec(compile(s,str(__file__),'exec'))
