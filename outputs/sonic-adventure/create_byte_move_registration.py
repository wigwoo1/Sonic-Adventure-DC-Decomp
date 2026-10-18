from pathlib import Path
p=Path(__file__).resolve().parent
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get','byte_move').replace('==140','==384').replace('0x8c65e798','0x8c10d25c').replace('0x8c65e79c','0x8c10d31c').replace('0x64e798:0x64e79c','0xfd25c:0xfd31c')
(p/'register_byte_move.py').write_text(r)
