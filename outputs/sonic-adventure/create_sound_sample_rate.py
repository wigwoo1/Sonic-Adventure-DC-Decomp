import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_stream_sample_rate_get'
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,'8c6676aa','8c6676ae','sound_stream_start_66768c.tsv'],check=True)
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798','0x8c6676aa').replace('0xa2001c','0xa20014')
s=s.replace('two-instruction source buffer getter8c65e798','two-instruction stream sample rate getter6676aa..6676ae,field14')
(p/f'verify_{stem}.py').write_text(s)
build=(p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem);(p/f'build_{stem}.ps1').write_text(build)
subprocess.run(['powershell','-NoProfile','-File',str(p/f'build_{stem}.ps1')],check=True)
subprocess.run([sys.executable,str(p/f'verify_{stem}.py')],check=True)
s=(p/'create_sound_work_metadata.py').read_text();start=s.index('specs=[');end=s.index('\n',start)
s=s[:start]+"specs=[('sound_work_sample_rate_get',0x8c65bc48,0x8c65bc54,0x8c6676aa,'sound_stream_sample_rate_get')]"+s[end:]
start=s.index(' expected=');end=s.index('\n',start);s=s[:start]+" expected='count'"+s[end:]
s=s.replace("s=s.replace('    native=bytearray(ram);'", "s=s.replace('    native=bytearray(ram);'")
s=s.replace("stream_before=bytes(ram[0xa70000:0xa70100])", "struct.pack_into('<I',ram,0xa70014,count);stream_before=bytes(ram[0xa70000:0xa70100])")
exec(compile(s,str(__file__),'exec'))
s=(p/'register_sound_source_sizes.py').read_text();start=s.index('for stem,start,end in [');end=s.index('\n',start)
s=s[:start]+"for stem,start,end in [('sound_stream_sample_rate_get',0x8c6676aa,0x8c6676ae),('sound_work_sample_rate_get',0x8c65bc48,0x8c65bc54)]:"+s[end:]
(p/'register_sound_sample_rate.py').write_text(s)
