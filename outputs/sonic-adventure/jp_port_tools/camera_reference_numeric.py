"""RZ/DN1 camera instruction service: pinned vectors plus Python integer FSQRT.
The square-root value oracle uses math.isqrt, independently of native C's
iterative bit-trial implementation. This is not new hardware validation.
"""
import ctypes as C
import math

def sqrt_word(raw):
    absolute=raw&0x7fffffff
    if absolute<0x800000:return raw&0x80000000
    if raw&0x80000000 or absolute>0x7f800000:return 0x7fbfffff
    if absolute==0x7f800000:return raw
    exponent=(raw>>23)-127;significand=(raw&0x7fffff)|0x800000
    if exponent%2:significand*=2;exponent-=1
    root=math.isqrt(significand<<23)
    return ((exponent//2+127)<<23)|(root&0x7fffff)

def service_for(numeric):
    def service(op,f):
        if op&0xf0ff!=0xf06d:
            assert numeric(op,C.byref(f),f.fpul)==0
            return
        assert not f.fpscr&0x80f80 and f.fpscr&0x40003==0x40001,'camera FSQRT service requires fixed single RZ/DN1 with traps disabled'
        bank=(f.fpscr>>21)&1;n=(op>>8)&15;raw=f.banks[bank][n];absolute=raw&0x7fffffff
        f.fpscr&=~0x3f000;f.banks[bank][n]=sqrt_word(raw)
        if absolute>=0x800000 and (raw&0x80000000 or absolute>0x7f800000):
            if absolute<=0x7f800000 or raw&0x400000:f.fpscr|=0x10040
        elif 0x800000<=absolute<0x7f800000:
            exponent=(raw>>23)-127;significand=(raw&0x7fffff)|0x800000
            if exponent%2:significand*=2
            radicand=significand<<23
            if math.isqrt(radicand)**2!=radicand:f.fpscr|=0x1004
    return service
