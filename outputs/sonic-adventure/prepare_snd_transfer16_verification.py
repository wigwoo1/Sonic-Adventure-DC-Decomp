from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_aiff_transfer8_step.ps1').read_text().replace('aiff_transfer8_step','snd_transfer16_step')
(p/'build_sound_snd_transfer16_step.ps1').write_text(b)
s=(p/'verify_sound_aiff_transfer_step_native_pending.py').read_text().replace('sound_aiff_transfer_step','sound_snd_transfer16_step').replace('s.pc=0x8c6699d4','s.pc=0x8c66a01c')
s=s.replace('Native137-instruction audio transfer step6699d4.96','Native137-instruction SND16-bit transfer step66a01c.96')
(p/'verify_sound_snd_transfer16_step_native_pending.py').write_text(s)
