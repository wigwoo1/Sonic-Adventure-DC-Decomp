from pathlib import Path
p=Path(__file__).resolve().parent
stem='engine_transfer_wait'
(p/f'build_{stem}.ps1').write_text((p/'build_sound_backend_fieldc_get.ps1').read_text().replace('sound_backend_fieldc_get',stem))
s=(p/'register_sound_backend_fieldc_get.py').read_text().replace('sound_backend_fieldc_get',stem).replace(str(0x8c66bd44),str(0x8c10eee0)).replace(str(0x8c66bdb4),str(0x8c10ef12)).replace('==768','==32')
(p/f'register_{stem}.py').write_text(s)
