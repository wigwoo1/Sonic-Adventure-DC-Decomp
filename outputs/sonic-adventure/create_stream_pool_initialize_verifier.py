from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_work_pool_initialize.py').read_text().replace('sound_work_pool_initialize','sound_stream_pool_initialize')
for old,new in [('0x8c65b5e8','0x8c667370'),('0x8c667370','0x8c644b64'),('0x8a3908','0x8d61a4'),('0x8a3910','0x8d61ac'),('0x8a3d90','0x8d670c'),('0x8a3d98','0x8d6714'),('0x8c8a3910','0x8c8d61ac'),('1168','1392'),('1152','1376')]:s=s.replace(old,new)
# The entrypoint is the stream initializer; its subordinate is the handle initializer.
s=s.replace('s.pc=0x8c644b64','s.pc=0x8c667370')
s=s.replace('work pool initializer65b5e8..65b5fc','stream pool initializer667370..667384').replace('Stream-pool initializer667370 modeled','Decode-handle pool initializer644b64 modeled').replace('work pool8c8a3910','stream pool8c8d61ac').replace('work bytes','stream bytes')
(p/'verify_sound_stream_pool_initialize.py').write_text(s)
s=(p/'build_sound_work_pool_initialize.ps1').read_text().replace('sound_work_pool_initialize','sound_stream_pool_initialize');(p/'build_sound_stream_pool_initialize.ps1').write_text(s)
s=(p/'register_sound_work_pool_initialize.py').read_text().replace('sound_work_pool_initialize','sound_stream_pool_initialize').replace('0x8c65b5e8','0x8c667370').replace('0x8c65b5fc','0x8c667384').replace('0x64b5e8:0x64b5fc','0x657370:0x657384');(p/'register_sound_stream_pool_initialize.py').write_text(s)
