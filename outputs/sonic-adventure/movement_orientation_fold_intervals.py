"""Exact rational, outward binary32 enclosures for the original scalar math.

These are static enclosures, not replacement game implementations or sampled
callee outputs. Intermediate bounds cover RM0/RM1 and completed scalar paths.
"""
from fractions import Fraction as F
import struct

def value(raw):return F.from_float(struct.unpack('<f',int(raw).to_bytes(4,'little'))[0])
def next_up(raw):return 1 if raw==0x80000000 else raw-1 if raw&0x80000000 else raw+1
def next_down(raw):return 0x80000001 if raw==0 else raw+1 if raw&0x80000000 else raw-1
def directed(x,upper):
    raw=struct.unpack('<I',struct.pack('<f',float(x)))[0]
    while value(raw)<x if upper else value(raw)>x:raw=next_up(raw) if upper else next_down(raw)
    assert value(raw)>=x if upper else value(raw)<=x
    return value(raw)
def interval(lo,hi=None):
    result=(F(lo),F(lo if hi is None else hi));assert result[0]<=result[1];return result
def rounded(lo,hi):return interval(directed(lo,False),directed(hi,True))
def add(a,b):return rounded(a[0]+b[0],a[1]+b[1])
def negative(a):return interval(-a[1],-a[0])
def multiply(a,b):
    products=[x*y for x in a for y in b];return rounded(min(products),max(products))
def divide(a,b):
    assert b[0]>0;quotients=[x/y for x in a for y in b];return rounded(min(quotients),max(quotients))
def fmac(a,b,c):
    values=[x*y+z for x in a for y in b for z in c];return rounded(min(values),max(values))
def union(items):return interval(min(x[0] for x in items),max(x[1] for x in items))
def encode(a):return dict(lower=str(a[0]),upper=str(a[1]),lower_float=float(a[0]),upper_float=float(a[1]))

def certificate(coefficients,pi4,pi2,pi,scale,offsets):
    coeff=list(map(value,coefficients));h=interval(coeff[0]);horner=[]
    for c in coeff[1:]:
        h=fmac(interval(0,1),h,interval(c));horner.append(h)
    assert h[0]>0 and h[1]<=1 and all(max(abs(x) for x in i)<=1 for i in horner)
    # Q'(x)*(1+x^2)=1-x^12 proves monotonicity of the exact degree11
    # alternating lower partial Q on[0,1]. No transcendental approximation
    # is assumed to match the original code.
    q_coeff={2*k+1:F((-1)**k,2*k+1) for k in range(6)}
    derivative={p-1:p*c for p,c in q_coeff.items()};identity={}
    for p,c in derivative.items():
        for shift in (0,2):identity[p+shift]=identity.get(p+shift,F(0))+c
    identity={p:c for p,c in identity.items() if c};assert identity=={0:F(1),12:F(-1)}
    exact=[q_coeff[p] for p in sorted(q_coeff,reverse=True)]
    coefficient_error=sum(abs(a-b) for a,b in zip(coeff,exact))
    h_derivative=sum((5-i)*abs(c) for i,c in enumerate(coeff))
    # All Horner intermediates have magnitude<=1. 2^-23 safely exceeds
    # one directed-rounding error below1 plus any subnormal flush error.
    # Squaring contributes D*u; five FMACs and the final multiply add6*u.
    u=F(1,2**23);error=coefficient_error+(h_derivative+6)*u
    assert error<F(1,2**17)
    def Q(x):return sum(c*x**p for p,c in q_coeff.items())
    def reduced(a):
        assert -1<=a[0]<=a[1]<=1
        # Positive factors make the result sign-preserving. For negative
        # arguments, oddness and Q monotonicity give a stronger lower bound
        # than an unconstrained ideal atan call would provide.
        return interval(-Q(-a[0])-error if a[0]<0 else 0,max(a[1],F(0)))
    v=interval(0,1);base=interval(0,value(pi4));one=interval(1)
    cases={'class0':add(base,reduced(v)),'fallback':interval(F(1,8)),
           'zero':interval(0),'exact_one':interval(value(pi4)),'large':interval(value(pi2))}
    branches={}
    for c in (F(1,4),F(1,2),F(3,4),F(1)):
        numerator=add(v,interval(-c))
        # Classes1..4 call the original bit-based scaling helper with-2/-1.
        # Its source-bound exponent proof gives0<=scale(v,k)<=v<=1. No
        # polynomial result is substituted for this actual callee.
        denominator=add(one,interval(0,1)) if c in (F(1,4),F(1,2)) else fmac(interval(c),v,one)
        argument=divide(numerator,denominator)
        assert argument[0]>=-c and argument[1]<=1
        offset=add(base,interval(value(offsets[str(c)])))
        result=add(offset,reduced(argument));cases[str(c)]=result
        branches[str(c)]=dict(numerator=encode(numerator),denominator=encode(denominator),argument=encode(argument),offset=encode(offset),result=encode(result))
    positive_core=union(cases.values());negative_core=negative(positive_core)
    angle=union((positive_core,add(interval(value(pi)),negative_core)))
    product=multiply(angle,interval(value(scale)))
    assert product[0]>-1 and product[1]<0x8001
    return dict(horner_enclosures=[encode(x) for x in horner],factor=encode(h),
      coefficient_error=str(coefficient_error),h_derivative_bound=str(h_derivative),
      operation_error_allowance=str(u),total_polynomial_error=str(error),
      exact_Q_derivative_identity={str(p):str(c) for p,c in identity.items()},
      branch_enclosures=branches,all_cases={k:encode(x) for k,x in cases.items()},
      positive_core=encode(positive_core),negative_core=encode(negative_core),
      atan2_angle=encode(angle),conversion_product=encode(product),
      truncated_integer_range=[0,0x8000],masked_phase_range=[0,0x8000])
