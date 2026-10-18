import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_linear_source_commit',0x8c65e242,0x8c65e282),('sound_linear_source_return',0x8c65e282,0x8c65e302)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_65e000.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+f'/* Retail linear-source descriptor operation {start:x}..{end:x}. */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_linear_source_capacity.ps1').read_text().replace('sound_linear_source_capacity',stem))
 s=(p/'verify_sound_linear_source_query.py').read_text().replace('sound_linear_source_query',stem).replace('0x8c65e1d0',hex(start))
 s=s.replace('s.r[4:8]=[obj,mode,limit,dest]','s.r[4:8]=[obj,mode,dest,0x11223344]')
 s=s.replace('    before=bytes(',"    signed=lambda v:ctypes.c_int32(v).value\n    offset=(position-limit)&0xffffffff\n    offset=offset if signed(offset)>0 else 0\n    descriptor_pointer=(0x8ca10000+offset)&0xffffffff\n    struct.pack_into('<2I',ram,0xa20004,descriptor_pointer,limit)\n    before=bytes(")
 a=s.index('    if mode==1:');z=s.index('    cases+=1',a)
 if stem.endswith('commit'):
  oracle="""    pointer=descriptor_pointer;count=limit
    invalid=mode not in [0,1]
    if invalid:pointer=count=0
"""
  desc='Commit accepts modes0/1 without object or descriptor changes;invalid modes clear descriptor and optionally call callback(userdata,-3).'
 else:
  oracle="""    pointer=descriptor_pointer;count=limit
    invalid=mode not in [0,1]
    if mode==1:
        capacity=(available+limit)&0xffffffff
        capacity=capacity if signed(capacity)<=80 else 80
        struct.pack_into('<2I',wanted,0xc,capacity,offset)
        invalid=bool(mismatch)
    elif mode!=0:pointer=count=0
"""
  desc='Return mode1 subtracts descriptor count from offset with signed-positive clamp to0;adds count to remaining capacity modulo32 with signed upper clamp to length80. Matching descriptor pointer equals base+new offset;mode0 preserves state but invokes optional error callback;other modes clear descriptor and invoke optional error callback.'
  oracle=oracle.replace('invalid=mode not in [0,1]','invalid=mode!=1')
 s=s[:a]+oracle+"""    assert native[0xa40000:0xa40040]==wanted
    assert native[0xa20000:0xa20010]==bytes([0x66])*4+struct.pack('<2I',pointer,count)+bytes([0x66])*4
    assert regs[15]==0x8cf00000 and (sr.value&0xf0)==0x60
    assert actual==[0x8c65e08c]+([callback] if invalid and callback else [])+[0x8c65e0a4]
"""+s[z:]
 a=s.index("scope='");z=s.index("\n(out/",a)
 scope=f'800 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source operation{start:x}..{end:x}. Modes0/1/2/ffffffff,five full-word capacities/counts including signed extremes,two offsets,optional controlled native callback,both banks. Independent entire64-byte source and descriptor guards. '+desc+' Native interrupt lock enter/exit and identity-getter callback;no modeled calls. Connected reservation lifecycle,retail default handler and game callers unverified.'
 s=s[:a]+'scope='+repr(scope)+s[z:]
 if stem.endswith('return'):
  s=s.replace('callback,bank in itertools.product','callback,bank,mismatch in itertools.product').replace('[0,1]):','[0,1],[0,4]):')
  s=s.replace('descriptor_pointer=(0x8ca10000+offset)&0xffffffff','descriptor_pointer=(0x8ca10000+offset+mismatch)&0xffffffff')
  s=s.replace('800 full retail','1600 full retail').replace('Matching descriptor pointer equals base+new offset;','Matching and mismatching descriptor pointers tested; mismatch calls optional callback after state restoration;')
 s=s.replace("print(cases,'linear source query comparisons passed')",f"print(cases,'{stem} comparisons passed')")
 (p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr(specs)+':'+reg[z:]
reg=reg.replace("==800","==(1600 if stem.endswith('return') else 800)")
(p/'register_sound_linear_source_commit_return.py').write_text(reg)
