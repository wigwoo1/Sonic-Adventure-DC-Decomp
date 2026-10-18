from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_snd_transfer16_step.ps1').read_text().replace('snd_transfer16_step','packed_transfer_step')
(p/'build_sound_packed_transfer_step.ps1').write_text(b)
s=(p/'verify_sound_snd_transfer16_step_native_pending.py').read_text().replace('sound_snd_transfer16_step','sound_packed_transfer_step').replace('s.pc=0x8c66a01c','s.pc=0x8c66a524')
s=s.replace("struct.pack('>6H'", "struct.pack('<6H'")
s=s.replace('Native137-instruction SND16-bit transfer step66a01c.96','Native123-instruction packed16-bit transfer step66a524.96').replace('big-endian16 to little-endian','little-endian16 copied into').replace('AIFF transfer step comparisons passed','packed audio transfer comparisons passed')
(p/'verify_sound_packed_transfer_step_native_pending.py').write_text(s)
