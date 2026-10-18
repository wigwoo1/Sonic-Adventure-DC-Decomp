from pathlib import Path
p=Path(__file__).resolve().parent
r=(p/'register_sound_stream_source_set.py').read_text().replace('sound_stream_source_set','sound_stream_work_complete').replace('sound_stream_work_complete_verification','stream_work_complete_verification').replace('==216','==432').replace('0x8c66771c','0x8c65ba22').replace('0x8c66776e','0x8c65bb4e').replace('0x65771c:0x65776e','0x64ba22:0x64bb4e').replace('verify_sound_stream_work_complete','verify_stream_work_complete')
(p/'register_stream_work_complete.py').write_text(r)
path=p/'source/sa_sound_stream_work_complete.c';s=path.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
path.write_text(s[:a]+'/* Retail higher-level stream completion65ba22..65bb4e, explicit SH4 ABI.\n * Caps published samples by signed remaining total; negative values preserved.\n * Getters,range splitting,virtual commits and acknowledgment remain services.\n * Verified scope: stream_work_complete_verification.json. */\n'+s[b:])
