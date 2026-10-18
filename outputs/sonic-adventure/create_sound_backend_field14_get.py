from pathlib import Path
p=Path(__file__).resolve().parent
stem='sound_backend_field14_get'
s=(p/'verify_sound_backend_fieldc_get.py').read_text().replace('sound_backend_fieldc_get',stem).replace('0x8c66bd44','0x8c66ba2c').replace('66bd44','66ba2c').replace('32+20','32+28').replace('address plus20','address plus28')
(p/f'verify_{stem}.py').write_text(s)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_backend_fieldc_get.ps1').read_text().replace('sound_backend_fieldc_get',stem))
s=(p/'register_sound_backend_fieldc_get.py').read_text().replace('sound_backend_fieldc_get',stem).replace(str(0x8c66bd44),str(0x8c66ba2c)).replace(str(0x8c66bdb4),str(0x8c66ba9c))
(p/f'register_{stem}.py').write_text(s)
