from pathlib import Path
p = Path(__file__).resolve().parent
stem = 'sound_backend_field8_get'
s = (p/'verify_sound_backend_field_get.py').read_text().replace('sound_backend_field_get', stem).replace('0x8c66bc24', '0x8c66bcb4').replace('66bc24', '66bcb4').replace('32+8', '32+4').replace('address plus8', 'address plus4')
(p/f'verify_{stem}.py').write_text(s)
s = (p/'build_sound_backend_field_get.ps1').read_text().replace('sound_backend_field_get', stem)
(p/f'build_{stem}.ps1').write_text(s)
s = (p/'register_sound_backend_field_get.py').read_text().replace('sound_backend_field_get', stem).replace(str(0x8c66bc24), str(0x8c66bcb4)).replace(str(0x8c66bc94), str(0x8c66bd24))
(p/f'register_{stem}.py').write_text(s)
