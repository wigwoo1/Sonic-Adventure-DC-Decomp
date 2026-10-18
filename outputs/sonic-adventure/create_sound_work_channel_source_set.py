import subprocess,sys,struct
from pathlib import Path
p=Path(__file__).resolve().parent
b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
stem='sound_work_channel_source_set';start=0x8c65b768;end=0x8c65b774
instructions=[(0x6343,'mov r4,r3'),(0x730c,'add #0xc,r3'),(0x4508,'shll2 r5'),(0x353c,'add r3,r5'),(0x000b,'rts'),(0x2562,'_mov.l r6,@r5')]
rows=['address\tword\tinstruction\treferences']
for i,(word,assembly) in enumerate(instructions):
 a=start+2*i
 assert struct.unpack_from('<H',b,a-0x8c010000)[0]==word
 rows.append(f'{a:x}\t{word:x}\t{assembly}\t')
tsv=stem+'_65b768.tsv'
(p/'analysis/instructions'/tsv).write_text('\n'.join(rows)+'\n')
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}',tsv],check=True)
source=p/'source'/('sa_'+stem+'.c')
c=source.read_text();comment_start=c.index('/* Static C recovery');comment_end=c.index(' */',comment_start)+3
source.write_text(c[:comment_start]+'/* Retail work channel source setter 8c65b768..8c65b774.\n * R4 is work, R5 is channel index, R6 is source pointer. Address arithmetic\n * wraps at 32 bits and has no index bounds check. Store is in the RTS delay slot. */'+c[comment_end:])
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',stem).replace('0x8c65e798',hex(start))
s=s.replace('for phase,count,bank,right in itertools.product(', 'for channel,phase,count,bank,right in itertools.product([0,1,2,31,0xfffffffd,0xfffffffe,0x40000000],')
s=s.replace('0xa20040','0xa20100').replace('bytes([0x55])*64','bytes([0x55])*256')
s=s.replace('[0x8ca20000,0x8ca10000,count,','[0x8ca20000,channel,count,')
s=s.replace('    assert regs[0]==count',"    offset=(12+4*channel)&0xffffffff\n    struct.pack_into('<I',wanted,offset,count)\n    assert regs[5]==(0x8ca20000+offset)&0xffffffff")
begin=s.index("scope='");finish=s.index("'),indent=2)",begin)
scope='980 full retail RAM/CPU/FPU/SR/PR comparisons of six-instruction channel source setter65b768..65b774. Independently check entire256-byte work object and effective pointer R5=R4+12+4*inputR5 modulo32bits, store R6 in RTS delay slot; no index bounds check. Seven indices including0/1/2/31,negative bit patterns and shift overflow,seven pointer patterns,five neighboring fixtures,both banks,two stack fixtures. Export opcode words checked against retail bytes. No modeled calls. Connected playback/fullgame remain unverified.'
s=s[:begin]+'scope='+repr(scope)+s[finish+1:]
(p/f'verify_{stem}.py').write_text(s)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
reg=(p/'register_sound_work_accessors.py').read_text()
begin=reg.index('for stem,start,end in [');finish=reg.index('\n',begin)
reg=reg[:begin]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[finish:]
reg=reg.replace("evidence['cases_passed']==140","evidence['cases_passed']==980")
(p/f'register_{stem}.py').write_text(reg)
