from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_stream_sample_bits_get.py').read_text().replace('sound_stream_sample_bits_get','sound_work_reset').replace('0x8c6676b2','0x8c65b606')
s=s.replace("    wanted_bits=4 if phase in [1,2] and count==2 else 8 if phase==2 and count==1 else 16\n    assert regs[0]==wanted_bits", "    wanted[3]=0\n    for off,value in [(0x2c,0),(0x30,0),(0x34,0),(0x38,struct.unpack_from('<I',binary,0x65b710-0x10000)[0]),(0x3c,0xffffffff),(0x40,0),(0x44,0)]:struct.pack_into('<I',wanted,off,value)")
s=s.replace("    native=bytearray(ram);", "    for off in [0x2c,0x30,0x34,0x38,0x3c,0x40,0x44]:struct.pack_into('<I',ram,0xa20000+off,(phase*0x1234567+count)&0xffffffff)\n    ram[0xa20003]=count&255;original=bytes(ram[0xa20000:0xa20100])\n    native=bytearray(ram);")
start=s.index("scope='");end=s.index("'),indent=2)",start)
s=s[:start]+"scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of work reset65b606..65b624. Varied prior counters/stop bytes,both banks and stack data. Independent entire256-byte object: counters2c/30/34/40/44 zero,limit38 copied from retail literal65b710,remaining3c=-1,stop byte3 cleared; all other bytes preserved,no services. Full constructor/producer/playback/fullgame unverified."+s[end:]
(p/'verify_sound_work_reset.py').write_text(s)
build=(p/'build_sound_stream_sample_bits_get.ps1').read_text().replace('sound_stream_sample_bits_get','sound_work_reset').replace('Audio format initializer build failed','Sound work reset build failed')
(p/'build_sound_work_reset.ps1').write_text(build)
