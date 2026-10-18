from pathlib import Path
p=Path(__file__).resolve().parent
(p/'build_sound_stream_source_set.ps1').write_text((p/'build_sound_stream_default_input.ps1').read_text().replace('sound_stream_default_input','sound_stream_source_set'))
r=(p/'register_sound_stream_default_input.py').read_text().replace('sound_stream_default_input','sound_stream_source_set').replace('==72','==216').replace('0x8c667384','0x8c66771c').replace('0x8c6673a2','0x8c66776e').replace('0x657384:0x6573a2','0x65771c:0x65776e')
(p/'register_sound_stream_source_set.py').write_text(r)
path=p/'source/sa_sound_stream_source_set.c';s=path.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
path.write_text(s[:a]+'/* Retail stream source setter66771c..66776e, explicit SH4 ABI.\n * Verification scope: sound_stream_source_set_verification.json. */\n'+s[b:])
