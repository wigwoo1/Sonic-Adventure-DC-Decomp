from pathlib import Path
p=Path(__file__).resolve().parent
items=[('sound_stream_format_get','8c6676a4','8c6676aa','a8',True),('sound_stream_unit_count_get','8c6676ee','8c6676f2','10',False)]
for name,start,end,offset,short in items:
    s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x'+start).replace('0xa2001c','0xa200'+offset).replace('0xa20040','0xa20100').replace('*64','*256').replace('all64','all256')
    if short:
        s=s.replace("struct.pack_into('<I',ram,0xa200"+offset+",count)","struct.pack_into('<H',ram,0xa200"+offset+",count&0xffff)")
        s=s.replace('assert regs[0]==count','assert regs[0]==(ctypes.c_int16(count&0xffff).value&0xffffffff)')
    a=s.index("scope='");z=s.index("'),indent=2)",a)
    s=s[:a]+f"scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of metadata getter{start}. Seven bit patterns,five phase fixtures,banks,two stacks. Independent {'signed16' if short else 'full32'} result and entire256 stream bytes unchanged; no services. Native feeder integration/full playback/fullgame unverified."+s[z:]
    (p/f'verify_{name}.py').write_text(s)
    (p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
    r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x'+start).replace('0x8c65e79c','0x'+end).replace('0x64e798','0x'+format(int(start,16)-0x8c010000,'x')).replace('0x64e79c','0x'+format(int(end,16)-0x8c010000,'x'))
    (p/f'register_{name}.py').write_text(r)
for name,start,end,target in [('sound_work_unit_count_get','8c65bc6c','8c65bc78',0x8c6676ee),('sound_work_total_samples_get','8c65bc84','8c65bc90',0x8c6676f6)]:
    s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x'+start)
    s=s.replace("struct.pack_into('<I',ram,0xf00000,right)","struct.pack_into('<I',ram,0xa20004,0x8ca70000)")
    s=s.replace("    def call(*args):raise AssertionError('Unexpected service')",f"    def call(ctx,address,mem,f,sr,r,pr):\n        assert address=={target} and r[4]==0x8ca70000 and r[15]==0x8cf00000\n        r[0]=count;return 0\n    def hook(mem,r,sr):\n        assert r[4]==0x8ca70000 and r[15]==0x8cf00000\n        r[0]=count;return sr")
    s=s.replace("reference(ram,s,fpu=f",f"reference(ram,s,{{{target}:hook}},fpu=f")
    a=s.index("scope='");z=s.index("'),indent=2)",a)
    s=s[:a]+f"scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of metadata wrapper{start}. Tail getter modeled; independent stream argument from work+4, restored stack at tail call and returned value; work bytes unchanged. Native feeder integration/full playback/fullgame unverified."+s[z:]
    (p/f'verify_{name}.py').write_text(s)
    (p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
    r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x'+start).replace('0x8c65e79c','0x'+end).replace('0x64e798','0x'+format(int(start,16)-0x8c010000,'x')).replace('0x64e79c','0x'+format(int(end,16)-0x8c010000,'x'))
    (p/f'register_{name}.py').write_text(r)
