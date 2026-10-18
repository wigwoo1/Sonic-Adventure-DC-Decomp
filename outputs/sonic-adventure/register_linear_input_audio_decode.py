from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'register_native_sources_audio_decode.py').read_text().replace('native_sources_audio_decode','linear_input_audio_decode')
s=s.replace("names.update({'sa_sound_source_pool_initialize'","names.update({'sa_sound_linear_source_pool_initialize','sa_sound_linear_source_create','sa_sound_linear_source_reset','sa_sound_linear_source_capacity','sa_sound_linear_source_query','sa_sound_linear_source_commit','sa_sound_linear_source_return','sa_sound_linear_source_lock_enter','sa_sound_linear_source_lock_exit','sa_sound_source_pool_initialize'")
s=s.replace('Native source creation and publication through audio decoding:','Native linear input through original-prefix audio decoding:')
exec(compile(s,str(__file__),'exec'))
