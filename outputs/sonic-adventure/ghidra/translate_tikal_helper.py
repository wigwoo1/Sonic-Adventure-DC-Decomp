import re,sys
name,start,end=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3],16)
from pathlib import Path
p=Path('outputs/sonic-adventure');rows={};branch_targets={}
for l in (p/'analysis/instructions'/(sys.argv[4] if len(sys.argv)>4 else 'tikal_predicates.tsv')).read_text().splitlines()[1:]:
 a,w,i,*_=l.split('\t');
 if start<=int(a,16)<end:
  rows[int(a,16)]=(int(w,16),i.lstrip('_'))
  branch_targets[int(a,16)]=sorted(set(int(x,16) for x in re.findall(r'COMPUTED_JUMP:([0-9a-f]+)', '\t'.join(_))))
def reg(x):return f'r[{x}]'
def simple(a):
 if m:=re.fullmatch(r'mov.w r(\d+),@\(r0,r(\d+)\)',rows[a][1]):return f'wr(m,r[0]+r[{m[2]}],r[{m[1]}],2);'
 if m:=re.fullmatch(r'mov.w @r(\d+),r(\d+)',rows[a][1]):return f'r[{m[2]}]=sx(m,r[{m[1]}],2);'
 if m:=re.fullmatch(r'mov.w @\(0x([0-9a-f]+),r(\d+)\),r0',rows[a][1]):return f'r[0]=sx(m,r[{m[2]}]+0x{m[1]}u,2);'
 if m:=re.fullmatch(r'mov.w @\(r0,r(\d+)\),r(\d+)',rows[a][1]):return f'r[{m[2]}]=sx(m,r[0]+r[{m[1]}],2);'
 if m:=re.fullmatch(r'sts FPUL,r(\d+)',rows[a][1]):return f'r[{m[1]}]=*fpul;'
 w,i=rows[a]
 if m:=re.fullmatch(r'swap.w r(\d+),r(\d+)',i):
  n,d=m.groups();return f'r[{d}]=(r[{n}]<<16)|(r[{n}]>>16);'
 if m:=re.fullmatch(r'shld r(\d+),r(\d+)',i):
  n,d=m.groups();return f'r[{d}]=(r[{n}]&0x80000000u)?((r[{n}]&31u)?r[{d}]>>((0u-r[{n}])&31u):0u):(r[{d}]<<(r[{n}]&31u));'
 if m:=re.fullmatch(r'swap.b r(\d+),r(\d+)',i):
  n,d=m.groups();return f'r[{d}]=(r[{n}]&0xffff0000u)|((r[{n}]&255u)<<8)|((r[{n}]>>8)&255u);'
 if i=='nop':return ''
 if i=='div0u':return '*sr&=~0x301u;'
 if m:=re.fullmatch(r'(ocbi|ocbp) @r(\d+)',i):return f'status=sa_sh4_cache_{"invalidate" if m[1]=="ocbi" else "purge"}(ctx,r[{m[2]}]);if(status)return status;'
 if m:=re.fullmatch(r'ocbwb @r(\d+)',i):return f'status=sa_sh4_cache_writeback(ctx,r[{m[1]}]);if(status)return status;'
 if m:=re.fullmatch(r'sts FPSCR,r(\d+)',i):return f'r[{m[1]}]=f->fpscr;'
 if m:=re.fullmatch(r'lds r(\d+),FPSCR',i):return f'f->fpscr=r[{m[1]}];fr=f->bank[(f->fpscr>>21)&1u];'
 if m:=re.fullmatch(r'lds r(\d+),fpscr',i):return f'f->fpscr=r[{m[1]}];fr=f->bank[(f->fpscr>>21)&1u];'
 if m:=re.fullmatch(r'xor r(\d+),r(\d+)',i):return f'r[{m[2]}]^=r[{m[1]}];'
 if m:=re.fullmatch(r'movt r(\d+)',i):return f'r[{m[1]}]=*sr&1u;'
 if m:=re.fullmatch(r'and #0x([0-9a-f]+),r0',i):return f'r[0]&=0x{m[1]}u;'
 if m:=re.fullmatch(r'stc SR,r(\d+)',i):return f'r[{m[1]}]=*sr;'
 if m:=re.fullmatch(r'stc VBR,r(\d+)',i):return f'r[{m[1]}]=vbr;'
 if m:=re.fullmatch(r'ldc r(\d+),SR',i):return f'*sr=r[{m[1]}];'
 if m:=re.fullmatch(r'shar r(\d+)',i):return f'*sr=(*sr&~1u)|(r[{m[1]}]&1u);r[{m[1]}]=(r[{m[1]}]>>1)|(r[{m[1]}]&0x80000000u);'
 if m:=re.fullmatch(r'rotr r(\d+)',i):return f'*sr=(*sr&~1u)|(r[{m[1]}]&1u);r[{m[1]}]=(r[{m[1]}]>>1)|(r[{m[1]}]<<31);'
 if m:=re.fullmatch(r'shlr r(\d+)',i):return f'*sr=(*sr&~1u)|(r[{m[1]}]&1u);r[{m[1]}]>>=1;'
 if m:=re.fullmatch(r'cmp/hi r(\d+),r(\d+)',i):return f'*sr=(*sr&~1u)|(r[{m[2]}]>r[{m[1]}]);'
 if m:=re.fullmatch(r'shlr(8|16) r(\d+)',i):return f'r[{m[2]}]>>={m[1]};'
 if m:=re.fullmatch(r'shlr2 r(\d+)',i):return f'r[{m[1]}]>>=2;'
 if m:=re.fullmatch(r'tas.b @r(\d+)',i):return f'{{uint32_t value=rd(m,r[{m[1]}],1);*sr=(*sr&~1u)|(value==0);wr(m,r[{m[1]}],value|0x80u,1);}}'
 if m:=re.fullmatch(r'muls.w r(\d+),r(\d+)',i):return f'*macl=(uint32_t)((int32_t)(int16_t)r[{m[1]}]*(int32_t)(int16_t)r[{m[2]}]);'
 if m:=re.fullmatch(r'mul.l r(\d+),r(\d+)',i):return f'*macl=r[{m[1]}]*r[{m[2]}];'
 if m:=re.fullmatch(r'not r(\d+),r(\d+)',i):return f'r[{m[2]}]=~r[{m[1]}];'
 if m:=re.fullmatch(r'div0s r(\d+),r(\d+)',i):return f'*sr=(*sr&~0x301u)|((r[{m[2]}]>>31)<<8)|((r[{m[1]}]>>31)<<9)|((r[{m[2]}]^r[{m[1]}])>>31);'
 if m:=re.fullmatch(r'(subc|addc) r(\d+),r(\d+)',i):
  op,src,dst=m.groups()
  if op=='subc':return f'{{uint64_t lhs=r[{dst}],rhs=(uint64_t)r[{src}]+(*sr&1u);r[{dst}]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);}}'
  return f'{{uint64_t sum=(uint64_t)r[{dst}]+r[{src}]+(*sr&1u);r[{dst}]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);}}'
 if m:=re.fullmatch(r'rotcr r(\d+)',i):return f'{{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[{m[1]}]&1u);r[{m[1]}]=(r[{m[1]}]>>1)|(incoming<<31);}}'
 if m:=re.fullmatch(r'rotcl r(\d+)',i):return f'{{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[{m[1]}]>>31);r[{m[1]}]=(r[{m[1]}]<<1)|incoming;}}'
 if m:=re.fullmatch(r'div1 r(\d+),r(\d+)',i):return f'div_step(r[{m[1]}],&r[{m[2]}],sr);'

 if m:=re.fullmatch(r'dt r(\d+)',i):return f'r[{m[1]}]--;*sr=(*sr&~1u)|(!r[{m[1]}]);'
 if m:=re.fullmatch(r'fsqrt fr(\d+)',i):return f'status=sa_sh4_sqrt(fr[{m[1]}],&f->fpscr,&fr[{m[1]}]);if(status)return status;'
 if m:=re.fullmatch(r'fneg fr(\d+)',i):return f'fr[{m[1]}]^=0x80000000u;'
 if m:=re.fullmatch(r'fldi1 fr(\d+)',i):return f'fr[{m[1]}]=0x3f800000u;'
 if m:=re.fullmatch(r'neg r(\d+),r(\d+)',i):return f'r[{m[2]}]=0u-r[{m[1]}];'
 if m:=re.fullmatch(r'sts (FPUL|MACL),r(\d+)',i):return f'r[{m[2]}]=*{m[1].lower()};'
 if m:=re.fullmatch(r'fmov.s fr(\d+),@-r(\d+)',i):return f'r[{m[2]}]-=4;wr(m,r[{m[2]}],fr[{m[1]}],4);'
 if m:=re.fullmatch(r'fmov.s @r(\d+)\+,fr(\d+)',i):return f'fr[{m[2]}]=rd(m,r[{m[1]}],4);r[{m[1]}]+=4;'
 if m:=re.fullmatch(r'fmov fr(\d+),fr(\d+)',i):return f'fr[{m[2]}]=fr[{m[1]}];'
 if m:=re.fullmatch(r'float FPUL,fr(\d+)',i):return f'status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[{m[1]}]);if(status)return status;'
 if m:=re.fullmatch(r'ftrc fr(\d+),FPUL',i):return f'status=sa_sh4_truncate_i32(fr[{m[1]}],&f->fpscr,fpul);if(status)return status;'
 if m:=re.fullmatch(r'(fmul|fdiv|fsub) fr(\d+),fr(\d+)',i):
  fn={'fmul':'sa_sh4_multiply','fdiv':'sa_sh4_divide','fsub':'sa_sh4_add_sub'}[m[1]]
  return f'status={fn}(&fr[{m[3]}],&fr[{m[2]}],&f->fpscr,&fr[{m[3]}]'+(',1' if m[1]=='fsub' else '')+');if(status)return status;'
 if m:=re.fullmatch(r'fcmp/eq fr(\d+),fr(\d+)',i):return f'status=eq(f,sr,fr[{m[2]}],fr[{m[1]}]);if(status)return status;'

 if m:=re.fullmatch(r'(or|sub) r(\d+),r(\d+)',i):return f'r[{m[3]}]{"|=" if m[1]=="or" else "-="}r[{m[2]}];'
 if m:=re.fullmatch(r'fmov.s fr(\d+),@r(\d+)',i):return f'wr(m,r[{m[2]}],fr[{m[1]}],4);'
 if m:=re.fullmatch(r'lds r(\d+),FPUL',i):return f'*fpul=r[{m[1]}];'
 if m:=re.fullmatch(r'fsts FPUL,fr(\d+)',i):return f'fr[{m[1]}]=*fpul;'
 if m:=re.fullmatch(r'fmac fr0,fr(\d+),fr(\d+)',i):return f'status=sa_sh4_fmac(&fr[0],&fr[{m[1]}],&fr[{m[2]}],&f->fpscr,&fr[{m[2]}]);if(status)return status;'
 if m:=re.fullmatch(r'mov #(-?)0x([0-9a-f]+),r(\d+)',i):return f'r[{m[3]}]={(-int(m[2],16) if m[1] else int(m[2],16))&0xffffffff}u;'
 if m:=re.fullmatch(r'mov r(\d+),r(\d+)',i):return f'r[{m[2]}]=r[{m[1]}];'
 if m:=re.fullmatch(r'(shll2|shll8|shll16|shll) r(\d+)',i):
  n=m[2];return (f'*sr=(*sr&~1u)|(r[{n}]>>31);' if m[1]=='shll' else '')+f'r[{n}]<<={dict(shll=1,shll2=2,shll8=8,shll16=16)[m[1]]};'
 if m:=re.fullmatch(r'add #(-?)0x([0-9a-f]+),r(\d+)',i):return f'r[{m[3]}]+={(-int(m[2],16) if m[1] else int(m[2],16))&0xffffffff}u;'
 if m:=re.fullmatch(r'(add|and) r(\d+),r(\d+)',i):return f'r[{m[3]}]{"+=" if m[1]=="add" else "&="}r[{m[2]}];'
 if m:=re.fullmatch(r'extu.(b|w) r(\d+),r(\d+)',i):return f'r[{m[3]}]=r[{m[2]}]&{255 if m[1]=="b" else 65535}u;'
 if m:=re.fullmatch(r'exts.(b|w) r(\d+),r(\d+)',i):return f'r[{m[3]}]=(uint32_t)(int32_t)(int{8 if m[1]=="b" else 16}_t)r[{m[2]}];'
 if m:=re.fullmatch(r'mov.l 0x([0-9a-f]+),r(\d+)',i):return f'r[{m[2]}]=rd(m,0x{m[1]}u,4);'
 if m:=re.fullmatch(r'mov.w 0x([0-9a-f]+),r(\d+)',i):return f'r[{m[2]}]=sx(m,0x{m[1]}u,2);'
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
 if m:=re.fullmatch(r'cmp/eq #0x([0-9a-f]+),r0',i):
  immediate=int(m[1],16)&255
  if immediate&128:immediate|=0xffffff00
  return f'*sr=(*sr&~1u)|(r[0]==0x{immediate:08x}u);'
 if m:=re.fullmatch(r'cmp/eq #-0x([0-9a-f]+),r0',i):return f'*sr=(*sr&~1u)|(r[0]==0x{(-int(m[1],16))&0xffffffff:08x}u);'
 if m:=re.fullmatch(r'cmp/str r(\d+),r(\d+)',i):return f'{{uint32_t x=r[{m[1]}]^r[{m[2]}];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));}}'
 if m:=re.fullmatch(r'cmp/eq r(\d+),r(\d+)',i):return f'*sr=(*sr&~1u)|(r[{m[1]}]==r[{m[2]}]);'
 if m:=re.fullmatch(r'cmp/pz r(\d+)',i):return f'*sr=(*sr&~1u)|((int32_t)r[{m[1]}]>=0);'
 if m:=re.fullmatch(r'cmp/pl r(\d+)',i):return f'*sr=(*sr&~1u)|((int32_t)r[{m[1]}]>0);'
 if m:=re.fullmatch(r'shad r(\d+),r(\d+)',i):return f'{{uint32_t shift=r[{m[1]}];if(!(shift&0x80000000u))r[{m[2]}]<<=shift&31u;else{{unsigned n=(0u-shift)&31u;r[{m[2]}]=n?(uint32_t)((int32_t)r[{m[2]}]>>n):((r[{m[2]}]&0x80000000u)?0xffffffffu:0u);}}}}'
 if m:=re.fullmatch(r'cmp/hs r(\d+),r(\d+)',i):return f'*sr=(*sr&~1u)|(r[{m[2]}]>=r[{m[1]}]);'
 if m:=re.fullmatch(r'cmp/(gt|ge) r(\d+),r(\d+)',i):return f'*sr=(*sr&~1u)|((int32_t)r[{m[3]}]{">" if m[1]=="gt" else ">="}(int32_t)r[{m[2]}]);'
 if m:=re.fullmatch(r'or #0x([0-9a-f]+),r0',i):return f'r[0]|=0x{m[1]}u;'
 if m:=re.fullmatch(r'mulu.w r(\d+),r(\d+)',i):return f'*macl=(r[{m[1]}]&65535u)*(r[{m[2]}]&65535u);'
 if i=='sts MACL,r0':return 'r[0]=*macl;'
 if m:=re.fullmatch(r'fldi0 fr(\d+)',i):return f'fr[{m[1]}]=0;'
 if m:=re.fullmatch(r'fmov.s @\(r0,r(\d+)\),fr(\d+)',i):return f'fr[{m[2]}]=rd(m,r[0]+r[{m[1]}],4);'
 if m:=re.fullmatch(r'fmov.s @r(\d+),fr(\d+)',i):return f'fr[{m[2]}]=rd(m,r[{m[1]}],4);'
 if m:=re.fullmatch(r'fmov fr(\d+),@\(r0,r(\d+)\)',i):return f'if(f->fpscr&0x100000u){{unsigned bank=((f->fpscr>>21)&1u)^({m[1]}u&1u);unsigned pair={m[1]}u&~1u;wr(m,r[0]+r[{m[2]}],f->bank[bank][pair],4);wr(m,r[0]+r[{m[2]}]+4,f->bank[bank][pair+1],4);}}else wr(m,r[0]+r[{m[2]}],fr[{m[1]}],4);'
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
  t=int(m[1],16);code=simple(a+2)+(f'goto L{t:x};' if t in rows else 'return sa_tikal_transition(m,f,sr,r);' if t==0xcb00994 else f'return call(ctx,0x{t:x}u,m,f,sr,r,pr);');delays.add(a+2)
 elif i=='rts':code=simple(a+2)+'return 0;';delays.add(a+2)
 elif i=='braf r0':
  targets=branch_targets.get(a) or [t for t in [0xcb003f6,0xcb0043e,0xcb004b2,0xcb005a0,0xcb005cc,0xcb005f8,0xcb00680,0xcb00720,0xcb00730,0xcb0076e,0xcb00750] if t in rows]
  assert targets and all(t in rows for t in targets),'Computed branch needs exported in-range destinations'
  code=simple(a+2)+f'switch(0x{a+4:x}u+r[0]){{'+''.join(f'case 0x{t:x}u:goto L{t:x};' for t in targets)+'default:return -2;}';delays.add(a+2)
 elif m:=re.fullmatch(r'(jsr|jmp) @r(\d+)',i):
  code=f'target=r[{m[2]}];'+(f'*pr=0x{a+4:x}u;' if m[1]=='jsr' else '')+simple(a+2)+'status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u];'+('return 0;' if m[1]=='jmp' else '')
  if m[1]=='jmp' and name in ('sa_engine_cache_writeback','sa_file_cache_invalidate','sa_file_cache_purge'):
   destinations=[t for t in branch_targets.get(a,[]) if (t&0x1fffffff) in {x&0x1fffffff for x in rows}]
   targets={t&0x1fffffff:next(x for x in rows if (x&0x1fffffff)==(t&0x1fffffff)) for t in destinations}
   if targets:code=f'target=r[{m[2]}];'+simple(a+2)+'switch(target&0x1fffffffu){'+''.join(f'case 0x{k:x}u:goto L{v:x};' for k,v in targets.items())+'default:return -2;}'
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
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal pre-update 0cb003a0..0cb00780.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int RECOVERED_NAME(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto ENTRY_LABEL;
'''
header=header.replace('RECOVERED_NAME',name).replace('ENTRY_LABEL',f'L{start:x}').replace('int status,take;','int status,take;' if any('/s ' in i for w,i in rows.values()) else 'int status;').replace('retail Tikal pre-update 0cb003a0..0cb00780',f'retail Tikal helper {start:08x}..{end:08x}')
if not any(i.startswith('f') for w,i in rows.values()):header=header.replace('if(f->fpscr&0x180000u)return -1;','').replace('PR=SZ=0, finite normal/zero FCMP.','Integer routine; FPU mode bits do not affect its instructions.')
if any(i.startswith('div1 ') for w,i in rows.values()):
 header=header.replace('/* Static C recovery','''static void div_step(uint32_t divisor,uint32_t *value,uint32_t *sr){
 unsigned oldq=(*sr>>8)&1u,flagm=(*sr>>9)&1u,q=*value>>31;uint32_t before=(*value<<1)|(*sr&1u),result;unsigned overflow;
 if(oldq==flagm){result=before-divisor;overflow=result>before;}else{result=before+divisor;overflow=result<before;}
 q=flagm?(q?overflow:!overflow):(q?!overflow:overflow);*value=result;*sr=(*sr&~0x101u)|(q<<8)|(q==flagm);
}
/* Static C recovery''')
body=header+'\n'.join(out)+'\n}\n'
if name in ('sa_atan_core','sa_atan_classifier'):
 a=body.index('static int gt(');b=body.index('/* Static C recovery',a)
 body=body[:a]+'''#include "sh4_compare_bits.h"
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){return sa_sh4_compare_bits(a,b,&f->fpscr,sr,1);}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){return sa_sh4_compare_bits(a,b,&f->fpscr,sr,0);}
'''+body[b:]
if not any(i.startswith('f') for w,i in rows.values()):body=body.replace('if(f->fpscr&0x180000u)return -1;','')
(p/f'source/{name}.c').write_text(body)
print(len(rows),'retail pre-update instructions translated')


