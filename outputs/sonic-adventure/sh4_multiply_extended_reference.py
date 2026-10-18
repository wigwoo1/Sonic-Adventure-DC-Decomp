"""Manual-derived exceptional FMUL oracle; finite arithmetic stays independent.

SH-4 manual rev6 sections 9.39/exception helpers. Class-table outcomes include
the SH-4 signaling convention (fraction bit22 set), DN classification, cause
replacement, sticky flags and unconditional O/U/I enables. This is not copied
from the native extension or a hardware-capture oracle.
"""
import sh4_binary32

finite_execute=sh4_binary32.execute_multiply


def classify(raw,dn):
    exponent=(raw>>23)&255;fraction=raw&0x7FFFFF
    if exponent==255:
        if fraction:return 'snan' if fraction&0x400000 else 'qnan'
        return 'inf'
    if exponent==0:return 'denorm' if fraction and not dn else 'zero'
    return 'finite'


def exceptional(a,b,status,old_destination):
    if status&0x80000 or status&3>1:return -1,status,old_destination
    classes={classify(a,bool(status&0x40000)),classify(b,bool(status&0x40000))}
    # Ordered class table: signaling NaN, quiet NaN, unflushed denormal,
    # infinity/zero, infinity/nonzero, then ordinary finite arithmetic.
    actions=(('snan',16,0x7FBFFFFF),('qnan',0,0x7FBFFFFF),('denorm',32,old_destination))
    for kind,cause,result in actions:
        if kind in classes:break
    else:
        if 'inf' not in classes:return None
        cause,result=(16,0x7FBFFFFF) if 'zero' in classes else (0,((a^b)&0x80000000)|0x7F800000)
    after=(status&~0x3F000)|(cause<<12)|((cause&31)<<2)
    trap=bool(cause==32 or status&0x380 or cause==16 and status&0x800)
    return int(trap),after,old_destination if trap else result


def execute(fpu,n,m):
    bank=(fpu.fpscr>>21)&1
    a,b=fpu.banks[bank][n],fpu.banks[bank][m]
    special=exceptional(a,b,fpu.fpscr,a)
    if special is None:
        finite_execute(fpu,n,m)
        return 0
    result,status,destination=special
    if result!=-1:
        fpu.fpscr=status;fpu.banks[bank][n]=destination
    return result
