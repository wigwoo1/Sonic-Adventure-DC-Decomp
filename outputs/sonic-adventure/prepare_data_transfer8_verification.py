from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_snd_transfer8_step.ps1').read_text().replace('snd_transfer8_step','data_transfer8_step')
(p/'build_sound_data_transfer8_step.ps1').write_text(b)
s=(p/'verify_sound_snd_transfer8_step_native_pending.py').read_text().replace('sound_snd_transfer8_step','sound_data_transfer8_step').replace('s.pc=0x8c66a13c','s.pc=0x8c66a8fa')
s=s.replace('phase,channels,frames,ready,bank','phase,channels,frames,ready,bank,capacity').replace('[0,0x200000]):','[0,0x200000],[0,2,4]):').replace('(0x54,2)','(0x54,capacity)').replace('n=min(frames,2)','n=min(frames,capacity)')
s=s.replace('samples[channels*i]<<8','samples[channels*i]<<(8 if channels==2 else 0)')
s=s.replace('Native125-instruction SND8-bit transfer step66a13c.96','Native122-instruction data-chunk8-bit transfer step66a8fa.288').replace('frames0/1/3,ready0/1,banks','frames0/1/3,ready0/1,banks,capacity0/2/4').replace('raw8-bit samples shifted left8 to16-bit little-endian mono or separated stereo buffers,clamp2','raw8-bit mono samples stored unshifted16-bit,stereo samples shifted left8 into separated buffers,clamp capacity0/2/4').replace('SND8 transfer step comparisons passed','data8 transfer comparisons passed')
(p/'verify_sound_data_transfer8_step_native_pending.py').write_text(s)
