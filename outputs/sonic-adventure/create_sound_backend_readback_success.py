from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_backend_readback_errors.py').read_text()
s=s.replace("for (enabled,present),value,bank in itertools.product([(0,0),(0,1),(1,0)]", "for (enabled,present),value,bank,answer in itertools.product([(1,1)]").replace(',[0,1]):',',[0,1],[0,0xffffffff]):',1)
i=s.index('cases=0');s=s[:i]+"specs=[(0x8c66ba2c,0x14),(0x8c6691d0,0x18),(0x8c66bdd4,2),(0x8c66bb4c,3),(0x8c66bc24,6),(0x8c66bcb4,8),(0x8c66babc,4),(0x8c66b9b0,5),(0x8c66accc,0x10),(0x8c66bd44,0xc)]\n"+s[i:]
a=s.index(' @Call');z=s.index(' s=State()',a)
s=s[:a]+" def service(a,m,r):\n  i=next(i for i,x in enumerate(specs) if x[0]==a);m[r[6]&0xffffff]=(value+i)&255;r[0]=answer\n @Call\n def call(ctx,a,m,f,sr,r,pr):\n  assert a in dict(specs);calls.append((a,r[4],r[5],r[6]));service(a,m,r);return 0\n def hook(a):\n  def run(m,r,sr):service(a,m,r);return sr\n  return run\n"+s[z:]
s=s.replace('reference(ram,s,{},','reference(ram,s,{a:hook(a) for a,o in specs},')
a=s.index(' error=');z=s.index(' cases+=1',a)
s=s[:a]+" assert calls==[(a,0x8ca08000,value,0x8ca20004+o) for a,o in specs]\n output=bytearray(before[0xa20000:0xa20028]);output[4:6]=bytes(2)\n for i,(a,o) in enumerate(specs):output[4+o]=(value+i)&255\n assert native[0xa20000:0xa20028]==output and native[0xa08000:0xa08090]==before[0xa08000:0xa08090]\n assert struct.unpack_from('<2I',native,0x67c1c8)==(0x12345678,0x9abcdef0) and r[0]==0 and r[15]==0x8cf00000\n"+s[z:]
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='28 complete retail RAM/CPU/FPU/SR/PR comparisons of backend readback66adc4 successful dispatch. Seven selector/payload fixtures,both banks,zero/ffffffff service returns. Ten field readers modeled with distinct controlled single-byte writes;independent exact call order,handle/selector/output offsets,first two packet bytes zeroed,40-byte guarded packet and144-byte handle preservation. Final status remains zero despite service errors;diagnostic globals preserved. Actual field widths/values and native field readers/hardware/game integration unverified. Together with42 native error-path comparisons covers routine control paths within these scopes.'"+s[z:]
s=s.replace('sound_backend_readback_errors_verification.json','sound_backend_readback_verification.json')
(p/'verify_sound_backend_readback.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([('sound_backend_readback',0x8c66adc4,0x8c66ae8c)])+':'+reg[z:];reg=reg.replace('==800','==28')
(p/'register_sound_backend_readback.py').write_text(reg)
