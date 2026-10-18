from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_aiff_transfer8_step.ps1').read_text().replace('aiff_transfer8_step','snd_transfer8_step')
(p/'build_sound_snd_transfer8_step.ps1').write_text(b)
s=(p/'verify_sound_aiff_transfer8_step_native_pending.py').read_text().replace('sound_aiff_transfer8_step','sound_snd_transfer8_step').replace('s.pc=0x8c669af2','s.pc=0x8c66a13c')
s=s.replace('n*channels*2,n','n*channels,n').replace('Native126-instruction8-bit audio transfer step669af2.96','Native125-instruction SND8-bit transfer step66a13c.96').replace('submission lengths/count and phase transitions','submission length n*channels (retail behavior),count and phase transitions').replace('AIFF transfer step comparisons passed','SND8 transfer step comparisons passed')
(p/'verify_sound_snd_transfer8_step_native_pending.py').write_text(s)
