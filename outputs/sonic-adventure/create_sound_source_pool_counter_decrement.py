import subprocess,struct,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_source_pool_counter_decrement'
b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
ops=[(0xd439,'mov.l 0x8c65e4a0,r4'),(0x6342,'mov.l @r4,r3'),(0x73ff,'add #-0x1,r3'),(0x6233,'mov r3,r2'),(0x2432,'mov.l r3,@r4'),(0xb,'rts'),(0x4215,'_cmp/pl r2')]
lines=['address\topcode\tinstruction\treferences']
for i,(word,assembly) in enumerate(ops):
 address=0x8c65e3b8+2*i;assert struct.unpack_from('<H',b,address-0x8c010000)[0]==word
 lines.append(f'{address:x}\t{word:x}\t{assembly}\t')
tsv=stem+'_65e3b8.tsv';(p/'analysis/instructions'/tsv).write_text('\n'.join(lines)+'\n')
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,'8c65e3b8','8c65e3c6',tsv],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
source.write_text(s[:a]+'/* Retail source-pool counter decrement 8c65e3b8..8c65e3c6.\n * Decrement modulo32bits; T reports signed-positive result. No pool clearing. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
reg=(p/'register_sound_work_accessors.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+"for stem,start,end in [('sound_source_pool_counter_decrement',0x8c65e3b8,0x8c65e3c6)]:"+reg[z:]
reg=reg.replace("evidence['cases_passed']==140","evidence['cases_passed']==84")
(p/f'register_{stem}.py').write_text(reg)
