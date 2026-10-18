from pathlib import Path
p=Path(__file__).resolve().parent
b=(p/'build_sound_snd_transfer_dispatch.ps1').read_text().replace('snd_transfer8_step','data_transfer8_step').replace('snd_transfer16_step','data_transfer16_step').replace('snd_transfer_dispatch','data_transfer_dispatch').replace('snd_table_transfer_step','data_bytepair_transfer_step')
(p/'build_sound_data_transfer_dispatch.ps1').write_text(b)
s=(p/'verify_sound_aiff_transfer_dispatch.py').read_text().replace('sound_aiff_transfer_dispatch','sound_data_transfer_dispatch')
s=s.replace("(0x8c669af2,'sa_sound_aiff_transfer8_step'),(0x8c6699d4,'sa_sound_aiff_transfer_step')", "(0x8c66a8fa,'sa_sound_data_transfer8_step'),(0x8c66a804,'sa_sound_data_transfer16_step'),(0x8c66a9fa,'sa_sound_data_bytepair_transfer_step')")
s=s.replace("struct.pack('>6H'", "struct.pack('<6H'").replace('s.pc=0x8c669bfc','s.pc=0x8c66ab5e')
s=s.replace('converted=phase==1 and ready==0','valid=format_flag in (0,1,2);converted=valid and phase==1 and ready==0')
s=s.replace('(samples[channels*i]<<8) if format_flag==1 else samples[channels*i]', 'convert(i,0)').replace('(samples[2*i+1]<<8) if format_flag==1 else samples[2*i+1]', 'convert(i,1)')
s=s.replace('target=bytearray([0x55])*64', "target=bytearray([0x55])*64\n def convert(i,ch):\n  if format_flag==1:return samples[channels*i+ch]<<(8 if channels==2 else 0)\n  if format_flag==2 and channels==2:return data[4*i+ch]|data[4*i+ch+2]<<8\n  return samples[channels*i+ch]")
s=s.replace('converted or phase==2','converted or (valid and phase==2)').replace('n*channels*2,n','n*channels*(1 if format_flag==1 else 2),n').replace('if phase==2:assert','if valid and phase==2:assert')
s=s[:s.index("(out/'analysis/")]+"(out/'analysis/sound_data_transfer_dispatch_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native20-instruction data-chunk dispatcher66ab5e with three native transfer paths and pending getter.480 full retail RAM/CPU/FPU/SR/PR/MACL/callback comparisons: selector0/1/2/ffff/8001,phases0..3,mono/stereo,frames0/1/3,pending0/1,banks. Independent LE16 copy,8-bit mono raw/stereo shifted,byte-pair stereo reconstruction,frame clamp/cursor/guards,source preservation,phase/submission arguments. Unknown selectors skip transfer even in phase2. Query/submission callbacks modeled; loader/physical playback/fullgame unverified.'),indent=2)+'\\n')\nprint(cases,'data dispatch comparisons passed')\n"
(p/'verify_sound_data_transfer_dispatch.py').write_text(s)
