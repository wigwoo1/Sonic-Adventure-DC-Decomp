from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_work_pool_initialize.py').read_text().replace('sound_work_pool_initialize','file_decode_pool_initialize')
for old,new in [('0x8c65b5e8','0x8c644b64'),('0x8a3908','0x890678'),('0x8a3910','0x890680'),('0x8a3d90','0x890820'),('0x8a3d98','0x890828'),('0x8c8a3910','0x8c890680'),('1168','432'),('1152','416')]:s=s.replace(old,new)
s=s.replace('reference(ram,s,{0x8c667370:hook}', 'reference(ram,s,{}')
s=s.replace('calls==expected==[0x8c667370]','calls==expected==[]')
s=s.replace('work pool initializer65b5e8..65b5fc','decode-handle pool initializer644b64..644b6e').replace('Stream-pool initializer667370 modeled and observed before clearing.', 'No modeled calls.').replace('work pool8c8a3910','decode-handle pool8c890680').replace('work bytes','handle bytes')
(p/'verify_file_decode_pool_initialize.py').write_text(s)
s=(p/'build_sound_work_pool_initialize.ps1').read_text().replace('sound_work_pool_initialize','file_decode_pool_initialize');(p/'build_file_decode_pool_initialize.ps1').write_text(s)
s=(p/'register_sound_work_pool_initialize.py').read_text().replace('sound_work_pool_initialize','file_decode_pool_initialize').replace('0x8c65b5e8','0x8c644b64').replace('0x8c65b5fc','0x8c644b6e').replace('0x64b5e8:0x64b5fc','0x634b64:0x634b6e');(p/'register_file_decode_pool_initialize.py').write_text(s)
