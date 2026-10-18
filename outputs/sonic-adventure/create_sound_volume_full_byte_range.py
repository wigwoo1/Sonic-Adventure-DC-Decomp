from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_volume_full_readback.py').read_text().replace("itertools.product([(1,1)],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,0xffffffff],[0,1],[1,2])","itertools.product([(1,1)],range(256),[0,1],[0],[0,1],[1])")
s=s.replace('112 full','1024 full').replace('Seven payload fixtures,backend mono/stereo,both banks,transfer context modes,two modeled service returns','All256 hardware word byte fixtures,backend mono,both banks,both transfer context modes;modeled auxiliary services return zero')
s=s.replace('sound_volume_full_readback_verification.json','sound_volume_full_byte_range_verification.json')
(p/'verify_sound_volume_full_byte_range.py').write_text(s)
reg=(p/'register_sound_volume_full_readback.py').read_text().replace('full_readback','full_byte_range').replace('==112','==1024')
(p/'register_sound_volume_full_byte_range.py').write_text(reg)
