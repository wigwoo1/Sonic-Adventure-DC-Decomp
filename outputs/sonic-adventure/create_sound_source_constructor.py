import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_source_reset',0x8c65e454,0x8c65e462),('sound_source_create',0x8c65e3c6,0x8c65e42c)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_source_initialize_65e300.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
 comment='Resets counters and ring cursors, preserving bindings.' if stem.endswith('reset') else 'Allocates first free of64 source slots, installs bindings and calls reset.'
 source.write_text(s[:a]+f'/* Retail source routine {start:x}..{end:x}. {comment} */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get','sound_source_reset').replace('0x8c65e798','0x8c65e454')
s=s.replace("struct.pack_into('<I',ram,0xa2001c,count)","struct.pack_into('<I',ram,0xa20020,count)")
s=s.replace('    assert regs[0]==count',"    for offset,value in [(0xc,0),(0x10,count),(0x14,0),(0x18,0)]:struct.pack_into('<I',wanted,offset,value)")
a=s.index("scope='");z=s.index("'),indent=2)",a)
scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of native source reset65e454..65e462. Independent entire64-byte object: counter+c zero,capacity+10 receives ring size+20,read+14 and write+18 zero;all other bytes preserved. Seven ring-size patterns,five neighboring fixtures,both banks,two stack fixtures. No modeled calls. Constructor/lifecycle integration separately verified;fullgame unverified.'
s=s[:a]+'scope='+repr(scope)+s[z+1:];(p/'verify_sound_source_reset.py').write_text(s)
reg=(p/'register_sound_work_accessors.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:]
reg=reg.replace("assert evidence['cases_passed']==140", "assert evidence['cases_passed']==(140 if stem=='sound_source_reset' else 910)")
(p/'register_sound_source_constructor.py').write_text(reg)
