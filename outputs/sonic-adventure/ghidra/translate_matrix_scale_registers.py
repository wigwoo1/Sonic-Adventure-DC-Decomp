import re
from pathlib import Path
p=Path('outputs/sonic-adventure');lines=[]
for line in (p/'analysis/instructions/model_base_transform_dependencies.tsv').read_text().splitlines()[1:]:
 fields=line.split('\t');address=int(fields[0],16)
 if address>=0x8c63a742:break
 lines.append((address,fields[2].lstrip('_')))
def translate(start,end):
 out=[]
 for address,ins in lines:
  if not start<=address<end:continue
  if ins=='rts':continue
  if ins=='fschg':code='f->fpscr^=0x100000u;'
  elif (m:=re.fullmatch(r'fmov fr(\d+),fr(\d+)',ins)):code=f'move_pair(f,{m[2]}u,{m[1]}u);'
  elif (m:=re.fullmatch(r'fmov.s @r(\d+)(\+?),fr(\d+)',ins)):code=f'f->bank[bank][{m[3]}]=rd(ram,r[{m[1]}]);'+(f'r[{m[1]}]+=4u;' if m[2] else '')
  elif (m:=re.fullmatch(r'(fmul|fadd|fsub) fr(\d+),fr(\d+)',ins)):
   n,mreg=int(m[3]),int(m[2]);args=f'&f->bank[bank][{n}],&f->bank[bank][{mreg}],&f->fpscr,&f->bank[bank][{n}]'
   call=f'sa_sh4_multiply({args})' if m[1]=='fmul' else f'sa_sh4_add_sub({args},{int(m[1]=="fsub")})'
   code=f'status={call};if(status)return status;'
  elif (m:=re.fullmatch(r'mov #0x([0-9a-f]+),r(\d+)',ins)):code=f'r[{m[2]}]={int(m[1],16)}u;'
  elif (m:=re.fullmatch(r'fmov.s fr(\d+),@r(\d+)',ins)):code=f'wr(ram,r[{m[2]}],f->bank[bank][{m[1]}]);'
  elif (m:=re.fullmatch(r'fmov fr(\d+),@\(r0,r(\d+)\)',ins)):code=f'wr(ram,r[0]+r[{m[2]}],f->bank[bank][{m[1]}]);'
  elif (m:=re.fullmatch(r'mov r(\d+),r(\d+)',ins)):code=f'r[{m[2]}]=r[{m[1]}];'
  elif (m:=re.fullmatch(r'add #(-?)0x([0-9a-f]+),r(\d+)',ins)):code=f'r[{m[3]}]{"-=" if m[1] else "+="}{int(m[2],16)}u;'
  else:raise ValueError((hex(address),ins))
  out.append(f' {code} /* {address:08x} */')
 return '\n'.join(out)
s='''#include "fpu_cleanup_transfer.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
static void move_pair(struct sa_fpu_banks *f,unsigned n,unsigned m){
 unsigned bank=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[bank][n]=f->bank[bank][m];return;}
 unsigned source=bank^(m&1u),dest=bank^(n&1u);m&=~1u;n&=~1u;uint32_t a=f->bank[source][m],b=f->bank[source][m+1u];f->bank[dest][n]=a;f->bank[dest][n+1u]=b;
}
/* Original8c63a69c..8c63a698 matrix scale, PR0/SZ0 entry. */
SA_STAGE_EXPORT int sa_model_matrix_scale_registers(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;
 *sr=(*sr&~1u)|(r[4]==0);
'''+''+'''\n if(!r[4]){
'''+translate(0x8c63a6a0,0x8c63a6e2)+'''\n return 0;
 }
'''+translate(0x8c63a6e2,0x8c63a742)+'''\n return 0;
}
'''
(p/'source/model_matrix_scale_registers.c').write_text(s)
print('Native matrix scale translation written')
