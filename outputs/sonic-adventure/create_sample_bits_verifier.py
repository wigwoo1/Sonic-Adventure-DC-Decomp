from pathlib import Path
p=Path(__file__).resolve().parent;name='sound_stream_sample_bits_get'
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c6676b2').replace('[0,1,2,3,0xffffffff]','[0,1,2,4,0xffffffff]').replace('0xa20040','0xa20100').replace('*64','*256')
s=s.replace("struct.pack_into('<I',ram,0xa20004,phase);struct.pack_into('<I',ram,0xa2001c,count)","struct.pack_into('<H',ram,0xa200a8,phase&0xffff);struct.pack_into('<H',ram,0xa200aa,count&0xffff)")
s=s.replace('assert regs[0]==count',"wanted_bits=4 if phase in [1,2] and count==2 else 8 if phase==2 and count==1 else 16\n    assert regs[0]==wanted_bits")
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of sample width getter6676b2..6676ee. Five format values0/1/2/4/ffff,seven subtype patterns0/1/2/127/128/255/ffff,banks,varied stack. Independent return4 for format1or2/subtype2,8 for format2/subtype1,otherwise16; entire256-byte stream unchanged,no services. Native work wrappers/configuration and fullgame unverified."+s[z:]
(p/f'verify_{name}.py').write_text(s)
(p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c6676b2').replace('0x8c65e79c','0x8c6676ee').replace('0x64e798','0x6576b2').replace('0x64e79c','0x6576ee')
(p/f'register_{name}.py').write_text(r)
