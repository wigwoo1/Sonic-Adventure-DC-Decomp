"""Exact finite binary32 arithmetic core; FPSCR/traps are handled separately.
Uses integer dyadics, avoiding host intermediate floating-point rounding.
"""
def decode(bits):
 exponent=(bits>>23)&255;fraction=bits&0x7fffff
 if exponent==255:raise ValueError('Nonfinite operand requires SH-4 classification')
 coefficient=fraction if exponent==0 else fraction|0x800000
 return (-coefficient if bits>>31 else coefficient),(-149 if exponent==0 else exponent-150)

def round_dyadic(coefficient,power,mode=0,negative_zero=False):
 if mode not in (0,1):raise ValueError('Reserved SH-4 rounding mode')
 sign=0x80000000 if coefficient<0 or (coefficient==0 and negative_zero) else 0
 magnitude=abs(coefficient)
 if not magnitude:return sign,False
 exponent=magnitude.bit_length()-1+power
 quantum=max(exponent-23,-149)
 shift=quantum-power
 if shift>0:significand,remainder=divmod(magnitude,1<<shift)
 else:significand,remainder=magnitude<<(-shift),0
 inexact=bool(remainder)
 if mode==0 and shift>0:
  halfway=1<<(shift-1)
  if remainder>halfway or (remainder==halfway and significand&1):significand+=1
 if significand>=0x1000000:significand>>=1;quantum+=1
 encoded_exponent=quantum+150
 if encoded_exponent>=255:return sign|(0x7f800000 if mode==0 else 0x7f7fffff),True
 if significand<0x800000:return sign|significand,inexact
 return sign|(encoded_exponent<<23)|(significand&0x7fffff),inexact

def add_sub(a,b,subtract=False,mode=0):
 x,px=decode(a);y,py=decode(b)
 if subtract:y=-y;b^=0x80000000
 common=min(px,py);total=(x<<(px-common))+(y<<(py-common))
 negative_zero=not total and not x and not y and bool(a&b&0x80000000)
 return round_dyadic(total,common,mode,negative_zero)

def execute_add_sub(fpu,n,m,subtract=False):
 """Single-precision finite FADD/FSUB; enabled traps/nonfinite input rejected."""
 fpscr=fpu.fpscr
 if fpscr&0x80000:
  from sh4_binary64 import execute_add_sub as execute_double_add_sub
  return execute_double_add_sub(fpu,n,m,subtract)
 assert not fpscr&0x80000 and not fpscr&0xf80,'Unsupported precision or enabled FPU traps'
 assert fpscr&3 in (0,1),'Reserved rounding mode'
 bank=(fpscr>>21)&1
 if any((fpu.banks[bank][i]&0x7fffffff)>0x7f800000 for i in (n,m)):
  invalid=any((fpu.banks[bank][i]&0x7fffffff)>0x7f800000 and fpu.banks[bank][i]&0x400000 for i in (n,m))
  fpu.fpscr=(fpscr&~0x3f000)|(0x10040 if invalid else 0)
  fpu.banks[bank][n]=0x7fbfffff
  return
 for index in (n,m):
  value=fpu.banks[bank][index];absolute=value&0x7fffffff
  assert absolute<0x7f800000,'Nonfinite FADD/FSUB input is not modeled'
  assert not (0<absolute<0x800000 and not fpscr&0x40000),'Denormal-input FPU trap is not modeled'
 fpscr&=~0x3f000
 for index in (n,m):
  if (fpu.banks[bank][index]&0x7fffffff)<0x800000 and fpscr&0x40000:
   fpu.banks[bank][index]&=0x80000000
 a=fpu.banks[bank][n];b=fpu.banks[bank][m]
 result,inexact=add_sub(a,b,subtract,fpscr&1)
 nearest,_=add_sub(a,b,subtract,0)
 if nearest&0x7fffffff==0x7f800000:fpscr|=0x4010;inexact=True
 absolute=result&0x7fffffff
 if absolute<0x800000:
  if fpscr&0x40000 and absolute:result&=0x80000000;inexact=True
  if inexact:fpscr|=0x2008
 if inexact:fpscr|=0x1004
 fpu.banks[bank][n]=result;fpu.fpscr=fpscr

def multiply_add(a,b,c,mode=0):
 x,px=decode(a);y,py=decode(b);z,pz=decode(c)
 product=x*y;power=px+py
 if not product:
  if not z:return (a^b^c)&0x80000000,False
  return c,False
 common=min(power,pz);total=(product<<(power-common))+(z<<(pz-common))
 return round_dyadic(total,common,mode)

def execute_fmac(fpu,n,m):
 fpscr=fpu.fpscr
 assert not fpscr&0x80000 and not fpscr&0xf80,'Unsupported precision or enabled FPU traps'
 assert fpscr&3 in (0,1),'Reserved rounding mode'
 bank=(fpscr>>21)&1
 for index in (0,m,n):
  absolute=fpu.banks[bank][index]&0x7fffffff
  assert absolute<0x7f800000,'Nonfinite FMAC input is not modeled'
  assert not (0<absolute<0x800000 and not fpscr&0x40000),'Denormal-input FPU trap is not modeled'
 fpscr&=~0x3f000
 for index in (0,m,n):
  if (fpu.banks[bank][index]&0x7fffffff)<0x800000 and fpscr&0x40000:fpu.banks[bank][index]&=0x80000000
 a,b,c=(fpu.banks[bank][i] for i in (0,m,n))
 result,inexact=multiply_add(a,b,c,fpscr&1);nearest,_=multiply_add(a,b,c,0)
 if nearest&0x7fffffff==0x7f800000:fpscr|=0x4010;inexact=True
 absolute=result&0x7fffffff
 if absolute<0x800000:
  if fpscr&0x40000 and absolute:result&=0x80000000;inexact=True
  if inexact:fpscr|=0x2008
 if inexact:fpscr|=0x1004
 fpu.banks[bank][n]=result;fpu.fpscr=fpscr

def execute_float(fpu,n):
 fpscr=fpu.fpscr
 if fpscr&0x80000:
  from sh4_binary64 import execute_float as execute_double_float
  return execute_double_float(fpu,n)
 assert not fpscr&0x80000 and not fpscr&0x80,'FLOAT precision or inexact trap unsupported'
 assert fpscr&3 in (0,1),'Reserved rounding mode'
 integer=fpu.fpul if fpu.fpul<0x80000000 else fpu.fpul-0x100000000
 result,inexact=round_dyadic(integer,0,fpscr&1)
 fpu.banks[(fpscr>>21)&1][n]=result
 fpu.fpscr=(fpscr&~0x3f000)|(0x1004 if inexact else 0)


def divide(a,b,mode=0):
 """Exact finite binary32 quotient, rounded once; nonzero denominator only."""
 if mode not in (0,1):raise ValueError('Reserved SH-4 rounding mode')
 x,px=decode(a);y,py=decode(b)
 if not y:raise ValueError('Zero divisor requires SH-4 exception classification')
 sign=(a^b)&0x80000000
 if not x:return sign,False
 numerator,denominator=abs(x),abs(y);power=px-py
 exponent=numerator.bit_length()-denominator.bit_length()+power
 shift=exponent-power
 if shift>=0:below=numerator<(denominator<<shift)
 else:below=(numerator<<(-shift))<denominator
 if below:exponent-=1
 quantum=max(exponent-23,-149);scale=power-quantum
 if scale>=0:numerator<<=scale
 else:denominator<<=-scale
 significand,remainder=divmod(numerator,denominator);inexact=bool(remainder)
 if mode==0 and (2*remainder>denominator or (2*remainder==denominator and significand&1)):significand+=1
 if significand>=0x1000000:significand>>=1;quantum+=1
 encoded_exponent=quantum+150
 if encoded_exponent>=255:return sign|(0x7f800000 if mode==0 else 0x7f7fffff),True
 if significand<0x800000:return sign|significand,inexact
 return sign|(encoded_exponent<<23)|(significand&0x7fffff),inexact


def execute_divide(fpu,n,m):
 """Finite single-precision FDIV; exceptional classes and enabled traps guarded."""
 fpscr=fpu.fpscr
 if fpscr&0x80000:
  from sh4_binary64 import execute_divide as execute_double_divide
  return execute_double_divide(fpu,n,m)
 assert not fpscr&0x80000 and not fpscr&0xf80,'Unsupported precision or enabled FPU traps'
 assert fpscr&3 in (0,1),'Reserved rounding mode'
 bank=(fpscr>>21)&1
 for index in (n,m):
  absolute=fpu.banks[bank][index]&0x7fffffff
  assert absolute<0x7f800000,'Nonfinite FDIV input is not modeled'
  assert not (0<absolute<0x800000 and not fpscr&0x40000),'Denormal-input FPU trap is not modeled'
 divisor=fpu.banks[bank][m]&0x7fffffff
 fpscr&=~0x3f000
 for index in (n,m):
  if (fpu.banks[bank][index]&0x7fffffff)<0x800000 and fpscr&0x40000:fpu.banks[bank][index]&=0x80000000
 a=fpu.banks[bank][n];b=fpu.banks[bank][m]
 if not b&0x7fffffff:
  invalid=not a&0x7fffffff
  fpu.banks[bank][n]=0x7fbfffff if invalid else ((a^b)&0x80000000)|0x7f800000
  fpu.fpscr=fpscr|(0x10040 if invalid else 0x8020)
  return
 result,inexact=divide(a,b,fpscr&1);nearest,_=divide(a,b,0)
 if nearest&0x7fffffff==0x7f800000:fpscr|=0x4010;inexact=True
 absolute=result&0x7fffffff
 if absolute<0x800000:
  if fpscr&0x40000 and absolute:result&=0x80000000;inexact=True
  if inexact:fpscr|=0x2008
 if inexact:fpscr|=0x1004
 fpu.banks[bank][n]=result;fpu.fpscr=fpscr


def multiply(a,b,mode=0):
 x,px=decode(a);y,py=decode(b)
 return round_dyadic(x*y,px+py,mode,bool((a^b)&0x80000000))


def execute_multiply(fpu,n,m):
 """Finite single-precision FMUL; exceptional classes and enabled traps guarded."""
 fpscr=fpu.fpscr
 if fpscr&0x80000:
  from sh4_binary64 import execute_multiply as execute_double_multiply
  return execute_double_multiply(fpu,n,m)
 assert not fpscr&0x80000 and not fpscr&0xf80,'Unsupported precision or enabled FPU traps'
 assert fpscr&3 in (0,1),'Reserved rounding mode'
 bank=(fpscr>>21)&1
 operands=[fpu.banks[bank][i] for i in (n,m)]
 if any((v&0x7fffffff)>0x7f800000 for v in operands):
  invalid=any((v&0x7fffffff)>0x7f800000 and v&0x400000 for v in operands)
  fpu.fpscr=(fpscr&~0x3f000)|(0x10040 if invalid else 0)
  fpu.banks[bank][n]=0x7fbfffff
  return
 for index in (n,m):
  absolute=fpu.banks[bank][index]&0x7fffffff
  assert absolute<0x7f800000,'Nonfinite FMUL input is not modeled'
  assert not (0<absolute<0x800000 and not fpscr&0x40000),'Denormal-input FPU trap is not modeled'
 fpscr&=~0x3f000
 for index in (n,m):
  if (fpu.banks[bank][index]&0x7fffffff)<0x800000 and fpscr&0x40000:fpu.banks[bank][index]&=0x80000000
 a=fpu.banks[bank][n];b=fpu.banks[bank][m];result,inexact=multiply(a,b,fpscr&1);nearest,_=multiply(a,b,0)
 if nearest&0x7fffffff==0x7f800000:fpscr|=0x4010;inexact=True
 absolute=result&0x7fffffff
 if absolute<0x800000:
  if fpscr&0x40000 and absolute:result&=0x80000000;inexact=True
  if inexact:fpscr|=0x2008
 if inexact:fpscr|=0x1004
 fpu.banks[bank][n]=result;fpu.fpscr=fpscr


def truncate_i32(raw):
 absolute=raw&0x7fffffff;negative=bool(raw&0x80000000)
 invalid=absolute>(0x4f000000 if negative else 0x4effffff)
 if invalid:return (0x7fffffff if not negative and absolute<=0x7f800000 else 0x80000000),True
 coefficient,power=decode(raw);magnitude=abs(coefficient)
 result=magnitude<<power if power>=0 else magnitude>>(-power)
 return (-result if coefficient<0 else result)&0xffffffff,False

def execute_truncate_i32(fpu,n):
 status=fpu.fpscr;bank=(status>>21)&1
 if status&0x80000:
  high=fpu.banks[bank][n&~1];low=fpu.banks[bank][(n&~1)+1]
  result,invalid=truncate_i32_double(high,low)
 else:result,invalid=truncate_i32(fpu.banks[bank][n])
 status&=~0x3f000
 if invalid:
  status|=0x10040
  assert not status&0x800,'FTRC invalid-operation trap unsupported'
 fpu.fpul=result;fpu.fpscr=status

def truncate_i32_double(high,low):
 raw=((high&0xffffffff)<<32)|(low&0xffffffff);absolute=raw&0x7fffffffffffffff;negative=bool(raw>>63)
 exponent=(absolute>>52)&0x7ff;fraction=absolute&0xfffffffffffff
 if exponent==0x7ff:
  # Positive infinity saturates high; NaNs and negative infinity use the invalid negative result.
  return (0x7fffffff if not negative and fraction==0 else 0x80000000),True
 if negative:
  if absolute>=0x41e0000000200000:return 0x80000000,True
 else:
  if absolute>=0x41e0000000000000:return 0x7fffffff,True
 if exponent==0:coefficient=fraction;power=-1074
 else:coefficient=(1<<52)|fraction;power=exponent-1023-52
 if power>=0:magnitude=coefficient<<power
 elif -power>=64:magnitude=0
 else:magnitude=coefficient>>(-power)
 result=(-magnitude if negative else magnitude)&0xffffffff
 return result,False

