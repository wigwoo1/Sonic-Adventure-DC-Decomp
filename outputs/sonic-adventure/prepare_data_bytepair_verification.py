from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_data_transfer16_step.ps1').read_text().replace('data_transfer16_step','data_bytepair_transfer_step')
(p/'build_sound_data_bytepair_transfer_step.ps1').write_text(b)
s=(p/'verify_sound_data_transfer16_step_native_pending.py').read_text().replace('sound_data_transfer16_step','sound_data_bytepair_transfer_step').replace('s.pc=0x8c66a804','s.pc=0x8c66a9fa')
s=s.replace('samples[channels*i])','(data[4*i]|data[4*i+2]<<8) if channels==2 else samples[i])').replace('samples[2*i+1])','data[4*i+1]|data[4*i+3]<<8)')
s=s.replace('Native117-instruction data-chunk16-bit transfer step66a804.288','Native145-instruction data-chunk byte-pair transfer step66a9fa.288').replace('little-endian16 copied into','little-endian16 mono copy or stereo byte pairs(0,2)/(1,3) reconstructed into').replace('data16 transfer comparisons passed','byte-pair transfer comparisons passed')
(p/'verify_sound_data_bytepair_transfer_step_native_pending.py').write_text(s)
