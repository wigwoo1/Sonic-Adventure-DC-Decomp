"""Static recovery of retail axis rotation bodies; vector instructions explicit."""
from pathlib import Path
import re
p=Path('outputs/sonic-adventure')
allrows={}
for line in (p/'analysis/instructions/axis_rotation_helpers.tsv').read_text().splitlines()[1:]:
 a,w,i,*_=line.split('\t');allrows[int(a,16)]=(int(w,16),i.lstrip('_'))
header='''#include "fpu_cleanup_transfer.h"
typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
static uint32_t rd(uint8_t *m,uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
static void move(struct sa_fpu_banks *f,unsigned n,unsigned s){unsigned b=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[b][n]=f->bank[b][s];return;}
 unsigned sb=b^(s&1u),nb=b^(n&1u);s&=~1u;n&=~1u;uint32_t a=f->bank[sb][s],v=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=v;
}
/* Complete retail body. PR=SZ=0 at entry. FSCA/FTRV numerical service explicit. */
SA_STAGE_EXPORT int NAME(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,instruction vector){
 if(f->fpscr&0x180000u||!vector)return -1;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;(void)take;
 goto ENTRY;
'''
for name,start,end in [('sa_matrix_rotate_x',0x8c639ec8,0x8c639f58),('sa_matrix_rotate_y',0x8c639f5c,0x8c639ff2),('sa_matrix_rotate_yxz',0x8c639ff8,0x8c63a1c8),('sa_matrix_rotate_z',0x8c63a1cc,0x8c63a262)]:
 rows={a:v for a,v in allrows.items() if start<=a<end};delays=set();out=[]
 def simple(a):
  w,i=rows[a]
  if i=='nop':return ''
  if i=='fschg':return 'f->fpscr^=0x100000u;'
  if i=='frchg':return 'f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u];'
  if i.startswith(('fsca ','ftrv ')):return f'status=vector(ctx,0x{w:04x}u,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];'
  if m:=re.fullmatch(r'fldi0 fr(\d+)',i):return f'fr[{m[1]}]=0;'
  if m:=re.fullmatch(r'fneg fr(\d+)',i):return f'fr[{m[1]}]^=0x80000000u;'
  if m:=re.fullmatch(r'fmov fr(\d+),fr(\d+)',i):return f'move(f,{m[2]},{m[1]});'
  if m:=re.fullmatch(r'fmov.s fr(\d+),@-r(\d+)',i):return f'r[{m[2]}]-=4;wr(m,r[{m[2]}],fr[{m[1]}]);'
  if m:=re.fullmatch(r'fmov.s @r(\d+)\+,fr(\d+)',i):return f'fr[{m[2]}]=rd(m,r[{m[1]}]);r[{m[1]}]+=4;'
  if m:=re.fullmatch(r'lds r(\d+),FPUL',i):return f'*fpul=r[{m[1]}];'
  if m:=re.fullmatch(r'tst r(\d+),r(\d+)',i):return f'*sr=(*sr&~1u)|!(r[{m[1]}]&r[{m[2]}]);'
  if m:=re.fullmatch(r'add #(-?)0x([0-9a-f]+),r(\d+)',i):return f'r[{m[3]}]+={(-int(m[2],16) if m[1] else int(m[2],16))&0xffffffff}u;'
  raise ValueError((hex(a),i))
 for a,(w,i) in rows.items():
  if a in delays:continue
  if m:=re.fullmatch(r'(bt|bf)(/s)? 0x([0-9a-f]+)',i):
   cond='(*sr&1u)' if m[1]=='bt' else '!(*sr&1u)';t=int(m[3],16)
   if m[2]:code=f'take={cond};'+simple(a+2)+f'if(take)goto L{t:x};';delays.add(a+2)
   else:code=f'if({cond})goto L{t:x};'
  elif i=='rts':code=simple(a+2)+'return 0;';delays.add(a+2)
  else:code=simple(a)
  out.append(f'L{a:x}: {code} /* {i} */')
 for n in range(len(out)-1):
  if 'return 0;' not in out[n]:out[n]+='\n goto '+out[n+1].split(':')[0]+';'
 (p/f'source/{name}.c').write_text(header.replace('NAME',name).replace('ENTRY',f'L{start:x}')+'\n'.join(out)+'\n}\n')
 print(name,len(rows))
