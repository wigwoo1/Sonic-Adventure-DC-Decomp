from pathlib import Path
p=Path(__file__).resolve().parent
stem='sound_backend_field4_get'
s=(p/'verify_sound_backend_field_get.py').read_text().replace('sound_backend_field_get',stem).replace('0x8c66bc24','0x8c66babc').replace('66bc24','66babc').replace('0x8c667150','0x8c667188').replace('lower667150','lower667188').replace('32+8','32+16').replace('address plus8','address plus16').replace("struct.pack_into('<I',native,r[5]&0xffffff,value)",'native[r[5]&0xffffff]=value&255').replace("struct.pack_into('<I',mem,r[5]&0xffffff,value)",'mem[r[5]&0xffffff]=value&255').replace('(0xdeadbeef&0xffff0000)|(value&65535)','(0xdeadbeef&0xffffff00)|(value&255)').replace('low16-bit output with upper/neighbor words preserved','single-byte output with upper/neighbor bytes preserved')
s=s.replace('six full-word lower payloads', 'six input fixtures truncated to lower byte payloads')
(p/f'verify_{stem}.py').write_text(s)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_backend_field_get.ps1').read_text().replace('sound_backend_field_get',stem))
s=(p/'register_sound_backend_field_get.py').read_text().replace('sound_backend_field_get',stem).replace(str(0x8c66bc24),str(0x8c66babc)).replace(str(0x8c66bc94),str(0x8c66bb2c))
(p/f'register_{stem}.py').write_text(s)
report=p/f'analysis/{stem}_verification.json'
if report.exists():
    report.write_text(report.read_text().replace('six full-word lower payloads','six input fixtures truncated to lower byte payloads'))
