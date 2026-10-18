import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_linear_source_contiguous_check';start=0x8c65e302;end=0x8c65e35e
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_65e000.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail linear-source contiguous-capacity check 8c65e302..8c65e35e. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_linear_source_capacity.ps1').read_text().replace('sound_linear_source_capacity',stem))
s=(p/'verify_sound_linear_source_query.py').read_text().replace('sound_linear_source_query',stem).replace('0x8c65e1d0',hex(start))
a=s.index('    wanted=');z=s.index('    cases+=1',a)
s=s[:a]+'''    signed=lambda v:ctypes.c_int32(v).value
    count=(available if signed(available)<signed(limit) else limit) if mode==1 else 0
    assert native[0xa40000:0xa40040]==before
    assert native[0xa20000:0xa20010]==bytes([0x66])*4+struct.pack('<I',count)+bytes([0x66])*8
    assert regs[0]==int(count==limit) and regs[15]==0x8cf00000 and (sr.value&0xf0)==0x60
    assert actual==[0x8c65e08c]+([callback] if mode not in [0,1] and callback else [])+[0x8c65e0a4]
'''+s[z:]
a=s.index("scope='");z=s.index('\n(out/',a)
scope='800 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source contiguous check65e302..65e35e. Modes0/1 and invalid2/ffffffff,five full-word capacity/request patterns including signed extremes,two neighboring offset fixtures,optional controlled native callback,both banks. Independent complete64-byte source window preserved and4-byte output with guards: mode1 signed minimum capacity/request,other modes0,R0 indicates count equals request including zero requests. Native interrupt lock enter/exit and identity-getter callback;no modeled calls. Original interrupt mask restored. Connected reservation lifecycle,retail default handler and game callers unverified.'
s=s[:a]+'scope='+repr(scope)+s[z:];s=s.replace("print(cases,'linear source query comparisons passed')","print(cases,'linear source contiguous check comparisons passed')")
(p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:]
(p/f'register_{stem}.py').write_text(reg)
