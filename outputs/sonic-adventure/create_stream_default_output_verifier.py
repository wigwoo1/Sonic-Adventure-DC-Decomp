from pathlib import Path
p=Path(__file__).resolve().parent
(p/'build_sound_stream_default_output.ps1').write_text((p/'build_sound_source_query.ps1').read_text().replace('sound_source_query','sound_stream_default_output'))
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get','sound_stream_default_output').replace('0x8c65e798','0x8c6673a2')
s=s.replace('ram[0xa20000:0xa20040]=bytes([0x55])*64','ram[0xa20000:0xa20100]=bytes([0x55])*256').replace('struct.pack_into(\'<I\',ram,0xa2001c,count)',"struct.pack_into('<2I',ram,0xa2006c,phase,phase)")
s=s.replace('0xa20000:0xa20040','0xa20000:0xa20100').replace('0xa2006c','0xa20098')
s=s.replace('    assert regs[0]==count',"    struct.pack_into('<2I',wanted,0x98,(phase+count)&0xffffffff,(phase+count)&0xffffffff)")
start=s.index("scope='");end=s.index("'),indent=2)",start)
s=s[:start]+"scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of default stream output callback6673a2..6673b4. Five initial counter values,seven sample counts including uint32 wraparound,both FPU banks,varied stack. Independent increments at stream+98 and+9c; all256 stream bytes and full RAM checked. No modeled dependencies; host audio output/full playback unverified."+s[end:]
(p/'verify_sound_stream_default_output.py').write_text(s)
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get','sound_stream_default_output').replace('0x8c65e798','0x8c6673a2').replace('0x8c65e79c','0x8c6673b4').replace('0x64e798:0x64e79c','0x6573a2:0x6573b4')
(p/'register_sound_stream_default_output.py').write_text(r)
