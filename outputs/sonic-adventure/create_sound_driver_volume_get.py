from pathlib import Path
p=Path(__file__).resolve().parent
(p/'build_sound_driver_volume_get.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan','sound_driver_volume_get'))
s=(p/'verify_sound_driver_gate_get.py').read_text().replace('sound_driver_gate_get','sound_driver_volume_get').replace('0x8c6680e2','0x8c6680ea')
a=s.index('@Call');z=s.index('cases=0',a);s=s[:a]+s[z:]
a=s.index('for value,bank,dest');z=s.index('\n ram=',a);s=s[:a]+"for value,bank,answer in itertools.product(range(256),[0,1],[0,0xffffffff]):\n dest=0x8ca20004"+s[z:]
s=s.replace("struct.pack_into('<I',wanted,dest&0xffffff,0)","struct.pack_into('<I',wanted,dest&0xffffff,((value if value<128 else value-256)*2)&0xffffffff)")
s=s.replace(" native=bytearray(ram);", " struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',wanted,0xa08028,0x8ca09000)\n native=bytearray(ram);")
i=s.index(' s=State()');s=s[:i]+" actual=[];expected=[]\n @Call\n def call(ctx,a,m,f,sr,r,pr):\n  assert a==0x8c669278;actual.append(tuple(r[i] for i in range(4,7)));m[(r[6]&0xffffff)+2]=value;r[0]=answer;return 0\n def backend(m,r,sr):expected.append(tuple(r[i] for i in range(4,7)));m[(r[6]&0xffffff)+2]=value;r[0]=answer;return sr\n"+s[i:]
s=s.replace('reference(ram,s,{},','reference(ram,s,{0x8c669278:backend},')
s=s.replace('native==ram==wanted','native==ram').replace('r[0]==r[4]==0','r[0]==r[4]==answer')
i=s.index(' assert r[0]');s=s[:i]+" assert actual==expected==[(0x8ca09000,0,0x8cefffe0)]\n assert native[0xa08000:0xa08090]==wanted[0xa08000:0xa08090] and native[0xa20000:0xa20010]==wanted[0xa20000:0xa20010]\n"+s[i:]
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='1024 full retail RAM/CPU/FPU/SR/PR comparisons of20-instruction volume getter6680ea..668112. All256 backend byte values,both banks,success/ffffffff status. Independent signed-byte doubling,16-byte guarded output,144-byte object preservation,backend handle/selector/stack packet pointer and status propagation verified. Backend669278 modeled;native backend/hardware/game integration unverified.'"+s[z:]
(p/'verify_sound_driver_volume_get.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([('sound_driver_volume_get',0x8c6680ea,0x8c668112)])+':'+reg[z:];reg=reg.replace('==800','==1024')
(p/'register_sound_driver_volume_get.py').write_text(reg)
