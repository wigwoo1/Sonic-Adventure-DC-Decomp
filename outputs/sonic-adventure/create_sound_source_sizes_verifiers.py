from pathlib import Path
p=Path(__file__).resolve().parent
for stem,address,offset in [('sound_source_ring_size_get','8c65e79c',0x20),('sound_source_mirror_size_get','8c65e7a0',0x24)]:
 s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798','0x'+address).replace('0xa2001c',hex(0xa20000+offset))
 s=s.replace('two-instruction source buffer getter8c65e798',f'two-instruction source size getter{address},field{offset:x}')
 (p/f'verify_{stem}.py').write_text(s)
 build=(p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem)
 (p/f'build_{stem}.ps1').write_text(build)
