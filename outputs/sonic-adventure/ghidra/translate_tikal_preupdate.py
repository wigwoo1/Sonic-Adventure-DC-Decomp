import re
from pathlib import Path
p=Path('outputs/sonic-adventure');rows={}
for l in (p/'analysis/instructions/tikal_preupdate.tsv').read_text().splitlines()[1:]:
 a,w,i,*_=l.split('\t');
 if int(a,16)<0xcb00780:rows[int(a,16)]=(int(w,16),i.lstrip('_'))
def reg(x):return f'r[{x}]'
def simple(a):
 w,i=rows[a]
 if i=='nop':return ''
 if m:=re.fullmatch(r'lds r(\d+),FPUL',i):return f'*fpul=r[{m[1]}];'
 if m:=re.fullmatch(r'fsts FPUL,fr(\d+)',i):return f'fr[{m[1]}]=*fpul;'
 if m:=re.fullmatch(r'fmac fr0,fr(\d+),fr(\d+)',i):return f'status=sa_sh4_fmac(&fr[0],&fr[{m[1]}],&fr[{m[2]}],&f->fpscr,&fr[{m[2]}]);if(status)return status;'
 if m:=re.fullmatch(r'mov #(-?)0x([0-9a-f]+),r(\d+)',i):return f'r[{m[3]}]={(-int(m[2],16) if m[1] else int(m[2],16))&0xffffffff}u;'
 if m:=re.fullmatch(r'mov r(\d+),r(\d+)',i):return f'r[{m[2]}]=r[{m[1]}];'
 if m:=re.fullmatch(r'(shll2|shll8|shll) r(\d+)',i):
  n=m[2];return (f'*sr=(*sr&~1u)|(r[{n}]>>31);' if m[1]=='shll' else '')+f'r[{n}]<<={dict(shll=1,shll2=2,shll8=8)[m[1]]};'
 if m:=re.fullmatch(r'add #(-?)0x([0-9a-f]+),r(\d+)',i):return f'r[{m[3]}]+={(-int(m[2],16) if m[1] else int(m[2],16))&0xffffffff}u;'
 if m:=re.fullmatch(r'(add|and) r(\d+),r(\d+)',i):return f'r[{m[3]}]{"+=" if m[1]=="add" else "&="}r[{m[2]}];'
 if m:=re.fullmatch(r'extu.(b|w) r(\d+),r(\d+)',i):return f'r[{m[3]}]=r[{m[2]}]&{255 if m[1]=="b" else 65535}u;'
 if m:=re.fullmatch(r'mov.l 0x([0-9a-f]+),r(\d+)',i):return f'r[{m[2]}]=rd(m,0x{m[1]}u,4);'
 if m:=re.fullmatch(r'mova 0x([0-9a-f]+),r0',i):return f'r[0]=0x{m[1]}u;'
 if m:=re.fullmatch(r'mov.(b|w|l) @r(\d+)(\+?),r(\d+)',i):
  z=dict(b=1,w=2,l=4)[m[1]];return f'r[{m[4]}]={"sx" if z<4 else "rd"}(m,r[{m[2]}],{z});'+(f'r[{m[2]}]+={z};' if m[3] else '')
 if m:=re.fullmatch(r'mov.(b|w|l) @\((r0|0x[0-9a-f]+),r(\d+)\),r(\d+)',i):
  z=dict(b=1,w=2,l=4)[m[1]];off='r[0]' if m[2]=='r0' else m[2]+'u';return f'r[{m[4]}]={"sx" if z<4 else "rd"}(m,r[{m[3]}]+{off},{z});'
 if m:=re.fullmatch(r'mov.(b|w|l) r(\d+),@(-?)r(\d+)',i):
  z=dict(b=1,w=2,l=4)[m[1]];return (f'r[{m[4]}]-={z};' if m[3] else '')+f'wr(m,r[{m[4]}],r[{m[2]}],{z});'
 if m:=re.fullmatch(r'mov.(b|w|l) r(\d+),@\((r0|0x[0-9a-f]+),r(\d+)\)',i):
  z=dict(b=1,w=2,l=4)[m[1]];off='r[0]' if m[3]=='r0' else m[3]+'u';return f'wr(m,r[{m[4]}]+{off},r[{m[2]}],{z});'
 if i=='sts.l PR,@-r15':return 'r[15]-=4;wr(m,r[15],*pr,4);'
 if i=='lds.l @r15+,PR':return '*pr=rd(m,r[15],4);r[15]+=4;'
 if m:=re.fullmatch(r'tst #0x([0-9a-f]+),r0',i):return f'*sr=(*sr&~1u)|!(r[0]&0x{m[1]}u);'
 if m:=re.fullmatch(r'tst r(\d+),r(\d+)',i):return f'*sr=(*sr&~1u)|!(r[{m[1]}]&r[{m[2]}]);'
 if m:=re.fullmatch(r'cmp/eq #0x([0-9a-f]+),r0',i):return f'*sr=(*sr&~1u)|(r[0]==0x{m[1]}u);'
 if m:=re.fullmatch(r'cmp/hs r(\d+),r(\d+)',i):return f'*sr=(*sr&~1u)|(r[{m[2]}]>=r[{m[1]}]);'
 if m:=re.fullmatch(r'or #0x([0-9a-f]+),r0',i):return f'r[0]|=0x{m[1]}u;'
 if m:=re.fullmatch(r'mulu.w r(\d+),r(\d+)',i):return f'*macl=(r[{m[1]}]&65535u)*(r[{m[2]}]&65535u);'
 if i=='sts MACL,r0':return 'r[0]=*macl;'
 if m:=re.fullmatch(r'fldi0 fr(\d+)',i):return f'fr[{m[1]}]=0;'
 if m:=re.fullmatch(r'fmov.s @\(r0,r(\d+)\),fr(\d+)',i):return f'fr[{m[2]}]=rd(m,r[0]+r[{m[1]}],4);'
 if m:=re.fullmatch(r'fmov.s @r(\d+),fr(\d+)',i):return f'fr[{m[2]}]=rd(m,r[{m[1]}],4);'
 if m:=re.fullmatch(r'fmov fr(\d+),@\(r0,r(\d+)\)',i):return f'wr(m,r[0]+r[{m[2]}],fr[{m[1]}],4);'
 if m:=re.fullmatch(r'fadd fr(\d+),fr(\d+)',i):return f'status=sa_sh4_add_sub(&fr[{m[2]}],&fr[{m[1]}],&f->fpscr,&fr[{m[2]}],0);if(status)return status;'
 if m:=re.fullmatch(r'fcmp/gt fr(\d+),fr(\d+)',i):return f'status=gt(f,sr,fr[{m[2]}],fr[{m[1]}]);if(status)return status;'
 raise ValueError((hex(a),i))
out=[];delays=set()
for a,(w,i) in rows.items():
 if a in delays:continue
 code=''
 if m:=re.fullmatch(r'(bt|bf)(/s)? 0x([0-9a-f]+)',i):
  target=int(m[3],16);condition='(*sr&1u)' if m[1]=='bt' else '!(*sr&1u)'
  if m[2]:code=f'take={condition};'+simple(a+2)+f'if(take)goto L{target:x};goto L{a+4:x};';delays.add(a+2)
  else:code=f'if({condition})goto L{target:x};'
 elif m:=re.fullmatch(r'bra 0x([0-9a-f]+)',i):
  t=int(m[1],16);code=simple(a+2)+(f'goto L{t:x};' if t in rows else 'return sa_tikal_transition(m,f,sr,r);');delays.add(a+2)
 elif i=='rts':code=simple(a+2)+'return 0;';delays.add(a+2)
 elif i=='braf r0':
  targets=[0xcb003f6,0xcb0043e,0xcb004b2,0xcb005a0,0xcb005cc,0xcb005f8,0xcb00680,0xcb00720,0xcb00730,0xcb0076e,0xcb00750]
  code=simple(a+2)+f'switch(0x{a+4:x}u+r[0]){{'+''.join(f'case 0x{t:x}u:goto L{t:x};' for t in targets)+'default:return -2;}';delays.add(a+2)
 elif m:=re.fullmatch(r'(jsr|jmp) @r(\d+)',i):
  code=f'target=r[{m[2]}];'+(f'*pr=0x{a+4:x}u;' if m[1]=='jsr' else '')+simple(a+2)+'status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u];'+('return 0;' if m[1]=='jmp' else '')
  delays.add(a+2)
 elif m:=re.fullmatch(r'bsr 0x([0-9a-f]+)',i):
  t=int(m[1],16);code=f'*pr=0x{a+4:x}u;'+simple(a+2)+( 'status=sa_tikal_transition(m,f,sr,r);' if t==0xcb00994 else f'status=call(ctx,0x{t:x}u,m,f,sr,r,pr);')+'if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u];';delays.add(a+2)
 else:code=simple(a)
 out.append(f'L{a:x}: {code} /* {i} */')
# Only branch destination labels needed. Keep every label via explicit sequential goto.
for n in range(len(out)-1):
 if not out[n].split('/*')[0].rstrip().endswith(('return 0;','return -2;}')):
  out[n]+='\n goto '+out[n+1].split(':')[0]+';'
header='''#include "fpu_cleanup_transfer.h"
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
/* Static C recovery of retail Tikal pre-update 0cb003a0..0cb00780.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_tikal_preupdate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;
 goto Lcb003a0;
'''
(p/'source/tikal_preupdate.c').write_text(header+'\n'.join(out)+'\n}\n')
print(len(rows),'retail pre-update instructions translated')
