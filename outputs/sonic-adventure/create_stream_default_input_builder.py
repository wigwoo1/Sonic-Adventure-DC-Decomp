from pathlib import Path
p=Path(__file__).resolve().parent
(p/'build_sound_stream_default_input.ps1').write_text((p/'build_sound_stream_default_output.ps1').read_text().replace('sound_stream_default_output','sound_stream_default_input'))
r=(p/'register_sound_stream_default_output.py').read_text().replace('sound_stream_default_output','sound_stream_default_input').replace('==140','==72').replace('0x8c6673a2','0x8c667384').replace('0x8c6673b4','0x8c6673a2').replace('0x6573a2:0x6573b4','0x657384:0x6573a2')
(p/'register_sound_stream_default_input.py').write_text(r)
path=p/'source/sa_sound_stream_default_input.c';s=path.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
path.write_text(s[:a]+'/* Retail default stream input callback667384..6673a2.\n * Explicit SH4 ABI. Position and capacity counters use unsigned subtraction.\n * Verified scope: sound_stream_default_input_verification.json. */\n'+s[b:])
