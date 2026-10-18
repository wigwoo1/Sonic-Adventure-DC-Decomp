from pathlib import Path
p=Path(__file__).resolve().parent
stem='sound_backend_fieldc_get'
s=(p/'verify_sound_backend_field_get.py').read_text().replace('sound_backend_field_get',stem).replace('0x8c66bc24','0x8c66bd44').replace('66bc24','66bd44').replace('0x8c667150','0x8c66910c').replace('lower667150','lower66910c').replace('32+8','32+20').replace('address plus8','address plus20').replace('((0xdeadbeef&0xffff0000)|(value&65535),0x12345678)','(value,0x12345678)').replace('low16-bit output with upper/neighbor words preserved','full32-bit output with neighbor word preserved')
(p/f'verify_{stem}.py').write_text(s)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_backend_field_get.ps1').read_text().replace('sound_backend_field_get',stem))
s=(p/'register_sound_backend_field_get.py').read_text().replace('sound_backend_field_get',stem).replace(str(0x8c66bc24),str(0x8c66bd44)).replace(str(0x8c66bc94),str(0x8c66bdb4))
(p/f'register_{stem}.py').write_text(s)
