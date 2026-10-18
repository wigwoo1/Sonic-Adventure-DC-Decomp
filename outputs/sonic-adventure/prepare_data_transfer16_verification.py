from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_packed_transfer_step.ps1').read_text().replace('packed_transfer_step','data_transfer16_step')
(p/'build_sound_data_transfer16_step.ps1').write_text(b)
s=(p/'verify_sound_packed_transfer_step_native_pending.py').read_text().replace('sound_packed_transfer_step','sound_data_transfer16_step').replace('s.pc=0x8c66a524','s.pc=0x8c66a804')
s=s.replace('phase,channels,frames,ready,bank','phase,channels,frames,ready,bank,capacity').replace('[0,0x200000]):','[0,0x200000],[0,2,4]):').replace('(0x54,2)','(0x54,capacity)').replace('n=min(frames,2)','n=min(frames,capacity)')
s=s.replace('packed16-bit transfer step66a524.96','data-chunk16-bit transfer step66a804.288').replace('frames0/1/3,ready0/1,banks','frames0/1/3,ready0/1,banks,capacity0/2/4').replace('clamp2','clamp capacity0/2/4').replace('packed audio transfer comparisons passed','data16 transfer comparisons passed')
(p/'verify_sound_data_transfer16_step_native_pending.py').write_text(s)
