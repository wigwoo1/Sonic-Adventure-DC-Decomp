import re
from pathlib import Path
p=Path('outputs/sonic-adventure');rows=[]
for line in (p/'analysis/instructions/model_matrix_inverse_dependencies.tsv').read_text().splitlines()[1:]:
 f=line.split('\t');rows.append((int(f[0],16),f[2].lstrip('_')))
def translate(start,end):
 out=[]
 for a,s in rows:
  if not start<=a<end:continue
  if s=='rts' or s=='nop' or s.startswith('bra '):continue
  if s=='fschg':c='f->fpscr^=0x100000u;'
  elif (m:=re.fullmatch(r'fmov fr(\d+),fr(\d+)',s)):c=f'move_pair(f,{m[2]}u,{m[1]}u);'
  elif (m:=re.fullmatch(r'fmov.s @r(\d+)(\+?),fr(\d+)',s)):c=f'f->bank[bank][{m[3]}]=rd(ram,r[{m[1]}]);'+(f'r[{m[1]}]+=4u;' if m[2] else '')
  elif (m:=re.fullmatch(r'fmov.s fr(\d+),@(-?)r(\d+)',s)):c=(f'r[{m[3]}]-=4u;' if m[2] else '')+f'wr(ram,r[{m[3]}],f->bank[bank][{m[1]}]);'
  elif (m:=re.fullmatch(r'fmov fr(\d+),@\(r0,r(\d+)\)',s)):c=f'wr(ram,r[0]+r[{m[2]}],f->bank[bank][{m[1]}]);'
  elif (m:=re.fullmatch(r'(fmul|fadd|fsub) fr(\d+),fr(\d+)',s)):
   args=f'&f->bank[bank][{m[3]}],&f->bank[bank][{m[2]}],&f->fpscr,&f->bank[bank][{m[3]}]'
   call=f'sa_sh4_multiply({args})' if m[1]=='fmul' else f'sa_sh4_add_sub({args},{int(m[1]=="fsub")})';c=f'status={call};if(status)return status;'
  elif (m:=re.fullmatch(r'fneg fr(\d+)',s)):c=f'f->bank[bank][{m[1]}]^=0x80000000u;'
  elif (m:=re.fullmatch(r'fsts FPUL,fr(\d+)',s)):c=f'f->bank[bank][{m[1]}]=*fpul;'
  elif (m:=re.fullmatch(r'mov r(\d+),r(\d+)',s)):c=f'r[{m[2]}]=r[{m[1]}];'
  elif (m:=re.fullmatch(r'mov #(-?)0x([0-9a-f]+),r(\d+)',s)):c=f'r[{m[3]}]={"0u-" if m[1] else ""}{int(m[2],16)}u;'
  elif (m:=re.fullmatch(r'add #(-?)0x([0-9a-f]+),r(\d+)',s)):c=f'r[{m[3]}]{"-=" if m[1] else "+="}{int(m[2],16)}u;'
  elif (m:=re.fullmatch(r'mov.l 0x([0-9a-f]+),r(\d+)',s)):c=f'r[{m[2]}]=rd(ram,0x{m[1]}u);'
  else:raise ValueError((hex(a),s))
  out.append(f' {c} /* {a:08x} */')
 return '\n'.join(out)
det=(p/'source/model_matrix_determinant.c').read_text();helper=det[det.index('static void move_pair'):det.index('/* Original')]
s='''#include "fpu_cleanup_transfer.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_model_matrix_determinant(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_inverse_singular(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
'''+helper+'''/* Original8c6390b0..8c6398ae. Native determinant, explicit/implicit inverse,
 * operation order/register aliases/scratch preserved. Finite PR0/SZ0 scope;
 * unsupported arithmetic returns status at first operation, not SH4 trap ABI. */
SA_STAGE_EXPORT int sa_model_matrix_inverse(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,uint32_t incoming_pr){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;uint32_t next;
 r[15]-=4u;wr(ram,r[15],r[14]);r[3]=0x8c64f3ecu;r[15]-=4u;wr(ram,r[15],incoming_pr);r[14]=r[4];
 status=sa_model_matrix_determinant(ram,f,sr,r);if(status)return status;
 status=sa_model_matrix_inverse_singular(ram,f,sr,r,&next);if(status)return status;
 if(next==0x8c639112u){
  f->bank[bank][1]=0x3f800000u;status=sa_sh4_divide(&f->bank[bank][1],&f->bank[bank][0],&f->fpscr,&f->bank[bank][1]);if(status)return status;
  r[15]-=4u;wr(ram,r[15],f->bank[bank][12]);r[15]-=4u;wr(ram,r[15],f->bank[bank][13]);*sr=(*sr&~1u)|(r[14]==0);*fpul=f->bank[bank][1];r[15]-=4u;wr(ram,r[15],f->bank[bank][14]);
  if(!r[14]){
'''+translate(0x8c639126,0x8c63949a)+'''\n  }else{
'''+translate(0x8c63949a,0x8c6398a2)+'''\n  }
  f->bank[bank][14]=rd(ram,r[15]);r[15]+=4u;f->bank[bank][13]=rd(ram,r[15]);r[15]+=4u;f->bank[bank][12]=rd(ram,r[15]);r[15]+=4u;
 }
 r[15]+=4u;r[14]=rd(ram,r[15]);r[15]+=4u;return 0;
}
'''
(p/'source/model_matrix_inverse.c').write_text(s);print('Both inverse paths translated with native scalar arithmetic')
