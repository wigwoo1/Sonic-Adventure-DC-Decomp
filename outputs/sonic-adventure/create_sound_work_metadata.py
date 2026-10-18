import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_work_format_get',0x8c65bc3c,0x8c65bc48,0x8c6676a4,'sound_stream_format_get'),('sound_work_channel_count_get',0x8c65bc54,0x8c65bc60,0x8c6676ae,'sound_stream_channel_count_get'),('sound_work_sample_bits_get',0x8c65bc60,0x8c65bc6c,0x8c6676b2,'sound_stream_sample_bits_get')]
for stem,start,end,target,lower in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_work_controls_65bc06.tsv'],check=True)
 s=(p/'verify_sound_work_unit_count_get.py').read_text().replace('sound_work_unit_count_get',stem).replace('0x8c65bc6c',hex(start)).replace('2355525358',str(target))
 s=s.replace('cases=0',f"getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/{lower}.dll')).sa_{lower};getter.argtypes=fn.argtypes\ncases=0")
 s=s.replace('    native=bytearray(ram);',"    ram[0xa70000:0xa70100]=bytes([0x77])*256\n    struct.pack_into('<2H',ram,0xa700a8,count&65535,phase&65535);ram[0xa7000e]=count&255\n    stream_before=bytes(ram[0xa70000:0xa70100])\n    native=bytearray(ram);")
 s=s.replace('        r[0]=count;return 0', '        return getter(mem,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)')
 s=s.replace(f'reference(ram,s,{{{target}:hook}},', 'reference(ram,s,{},')
 expected='ctypes.c_int16(count&65535).value&0xffffffff' if target==0x8c6676a4 else 'ctypes.c_int8(count&255).value&0xffffffff' if target==0x8c6676ae else '4 if count in [1,2] and phase==2 else 8 if count==2 and phase==1 else 16'
 s=s.replace('    assert regs[0]==count',f'    wanted_value={expected}\n    assert regs[0]==wanted_value and native[0xa70000:0xa70100]==stream_before')
 s=s.replace('Tail getter modeled;', 'Tail getter native;independent returned format signed16/channel signed8/sample width4/8/16 as applicable,entire256-byte stream unchanged;')
 s=s.replace('metadata wrapper8c65bc6c',f'metadata wrapper{start:x}')
 (p/f'verify_{stem}.py').write_text(s)
 build=(p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem);(p/f'build_{stem}.ps1').write_text(build)
 subprocess.run(['powershell','-NoProfile','-File',str(p/f'build_{stem}.ps1')],check=True)
 subprocess.run([sys.executable,str(p/f'verify_{stem}.py')],check=True)
s=(p/'register_sound_source_sizes.py').read_text();start=s.index('for stem,start,end in [');end=s.index('\n',start)
s=s[:start]+'for stem,start,end in '+repr([(name,start,end) for name,start,end,target,lower in specs])+':'+s[end:]
(p/'register_sound_work_metadata.py').write_text(s)
