"""Opcode wiring, independent mathematical bounds and state-policy checks.

Primitive comparisons use the same pinned upstream implementation and test
adapter plumbing only. Fraction/math bounds and matrix invariants are separate.
No local physical Dreamcast measurements or hardware dumps are claimed.
"""
import ctypes,hashlib,itertools,json,math,random,struct
from fractions import Fraction
from pathlib import Path
from verify_startup import ROOT
from verify_fpu_cleanup_transfer import Fpu

PROJECT=Path(__file__).resolve().parent
VENDOR=PROJECT/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h'
VENDOR_SHA='cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
BUILD=ROOT/'work/sonic-adventure/build'
LIB=ctypes.CDLL(str(BUILD/'sh4_vector_numeric.dll'))
REF=ctypes.CDLL(str(BUILD/'sh4_vector_numeric_reference.dll'))
execute=LIB.sa_sh4_vector_numeric
execute.argtypes=[ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32];execute.restype=ctypes.c_int
primitive=REF.sa_vector_primitive_reference
primitive.argtypes=[ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32];primitive.restype=ctypes.c_int
EDGES=(0,0x80000000,0x3F800000,0xBF800000,0x7F800000,0xFF800000,0x7FBFFFFF,0x7FC00001,
       1,0x80000001,0x7FFFFF,0x807FFFFF,0x800000,0x7F7FFFFF,0x40000000,0x12345678)
SPECS=[(0,n,0,0xF0FD|(n<<8)) for n in range(0,16,2)]
SPECS+=[(1,n,0,0xF1FD|(n<<8)) for n in range(0,16,4)]
SPECS+=[(2,n,m,0xF0ED|(n<<8)|((m//4)<<8)) for n,m in itertools.product(range(0,16,4),repeat=2)]
SPECS+=[(3,n,0,0xF07D|(n<<8)) for n in range(16)]


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def value(bits):return struct.unpack('<f',struct.pack('<I',bits))[0]


def fresh(seed=0,fpscr=0x40000):
    f=Fpu();f.fpscr=fpscr;f.fpul=0xC0FFEE42
    for b in range(2):f.banks[b][:]=[EDGES[(i+b*3+seed)%len(EDGES)] for i in range(16)]
    return f


def numerical_dot_error(a,b,result):
    products=[Fraction.from_float(value(x))*Fraction.from_float(value(y)) for x,y in zip(a,b)]
    exact=sum(products,Fraction())
    # Renesas geometric-operation error bound for normal 24-significand inputs.
    bound=max(map(abs,products))*Fraction(1,1<<23)+max(abs(exact)*Fraction(1,1<<23),Fraction(1,1<<149))
    error=abs(Fraction.from_float(value(result))-exact)
    assert error<=bound,(list(map(hex,a)),list(map(hex,b)),hex(result),float(error),float(bound))
    return float(error/bound) if bound else 0.0


def main():
    assert digest(VENDOR)==VENDOR_SHA
    assert 'The MIT License (MIT)' in VENDOR.read_text(encoding='utf-8')
    assert len(SPECS)==len({s[3] for s in SPECS})==44
    counts={'adapter_primitive_comparisons':0,'invalid_opcode_guards':0,'reserved_rm_guards':0,
            'pr_noop_and_trap_policy_checks':0,'fsca_status_policy_checks':0,'fsca_angle_mode_checks':0,
            'fsrra_positive_normal_checks':0,'dot_error_bound_checks':0,'matrix_invariant_checks':0}
    rng=random.Random(0x534834)
    for case,(spec,bank,sz,pr,dn,rm,en,flags) in enumerate(itertools.product(SPECS,range(2),range(2),range(2),range(2),range(2),range(32),(0,0x3F07C))):
        kind,n,m,word=spec
        status=(bank<<21)|(sz<<20)|(pr<<19)|(dn<<18)|rm|(en<<7)|flags
        f=fresh(case,status);expected=Fpu.from_buffer_copy(f)
        fpul=(case*0x1234567+0x80000000)&0xFFFFFFFF
        wanted=primitive(kind,n,m,ctypes.byref(expected),fpul)
        got=execute(word,ctypes.byref(f),fpul)
        assert got==wanted and bytes(f)==bytes(expected),(case,spec,hex(status))
        counts['adapter_primitive_comparisons']+=1
        if pr:
            before=fresh(case,status)
            trap=int(bool(en&0x17)) if kind==1 else int(bool(en&1))
            assert got==trap and bytes(f)==bytes(before)
            counts['pr_noop_and_trap_policy_checks']+=1
        if kind==0:
            before=fresh(case,status);b=(status>>21)&1
            expected_status=status if pr else (status&~0x3F000)|0x1004
            assert f.fpscr==expected_status
            if pr or en&1:assert list(f.banks[b])==list(before.banks[b])
            counts['fsca_status_policy_checks']+=1
    print(counts['adapter_primitive_comparisons'],'primitive adapter/state comparisons passed',flush=True)
    supported={s[3] for s in SPECS}
    original=fresh(9,0x40000);raw=bytes(original)
    for word in list(range(65536))+[0x10000,0x1F0FD,0xFFFFFFFF]:
        if word in supported:continue
        f=Fpu.from_buffer_copy(original)
        assert execute(word,ctypes.byref(f),0xFFFFFFFF)==-1 and bytes(f)==raw
        counts['invalid_opcode_guards']+=1
    for spec,mode,rm in itertools.product(SPECS,range(16),(2,3)):
        f=fresh(mode,((mode&1)<<21)|((mode&2)<<19)|((mode&4)<<17)|((mode&8)<<15)|rm)
        before=bytes(f)
        assert execute(spec[3],ctypes.byref(f),0xFFFFFFFF)==-1 and bytes(f)==before
        counts['reserved_rm_guards']+=1
    max_sine_error=0.0
    for rm in range(2):
        f=fresh(0,0x40000|rm)
        for angle in range(65536):
            assert execute(0xF0FD,ctypes.byref(f),angle|(0xA5A5<<16))==0
            theta=angle*(2*math.pi/65536)
            error=max(abs(value(f.banks[0][0])-math.sin(theta)),abs(value(f.banks[0][1])-math.cos(theta)))
            assert error<2**-21,(angle,rm,error)
            max_sine_error=max(max_sine_error,error);counts['fsca_angle_mode_checks']+=1
    print(counts['fsca_angle_mode_checks'],'FSCA angle/mode bounds passed',flush=True)
    mantissas={0,0x7FFFFF}
    for node in range(17):
        for delta in (-262145,-262144,-262143,-1,0,1,262143,262144,262145):
            m=node*524288+delta
            if 0<=m<=0x7FFFFF:mantissas.add(m)
    inputs={e<<23|m for e in range(1,255) for m in mantissas}
    inputs.update(rng.randrange(1,255)<<23|rng.getrandbits(23) for _ in range(5000))
    max_rsqrt_ulp=0.0
    for rm in range(2):
        f=fresh(0,0x40000|rm)
        for bits in sorted(inputs):
            f.banks[0][3]=bits
            assert execute(0xF37D,ctypes.byref(f),0)==0
            out=f.banks[0][3];result=value(out);exact=1/math.sqrt(value(bits))
            ulp=math.ldexp(1.0,((out>>23)&255)-127-23)
            error=abs(result-exact)/ulp
            assert error<2.00001,(hex(bits),rm,hex(out),error)
            max_rsqrt_ulp=max(max_rsqrt_ulp,error);counts['fsrra_positive_normal_checks']+=1
    print(counts['fsrra_positive_normal_checks'],'FSRRA positive-normal bounds passed',flush=True)
    def finite():return (rng.getrandbits(1)<<31)|(rng.randrange(111,144)<<23)|rng.getrandbits(23)
    worst_dot_ratio=0.0
    for rm in range(2):
        f=fresh(0,0x40000|rm)
        for _ in range(5000):
            a=[finite() for _ in range(4)];b=[finite() for _ in range(4)]
            f.banks[0][:4]=a;f.banks[0][4:8]=b
            assert execute(0xF4ED,ctypes.byref(f),0)==0
            worst_dot_ratio=max(worst_dot_ratio,numerical_dot_error(a,b,f.banks[0][7]))
            counts['dot_error_bound_checks']+=1
        for _ in range(2000):
            v=[finite() for _ in range(4)];m=[finite() for _ in range(16)]
            f.banks[0][:4]=v;f.banks[1][:]=m
            assert execute(0xF1FD,ctypes.byref(f),0)==0
            for row in range(4):
                worst_dot_ratio=max(worst_dot_ratio,numerical_dot_error(v,m[row::4],f.banks[0][row]))
                counts['dot_error_bound_checks']+=1
    for bank,rm,n in itertools.product(range(2),range(2),range(0,16,4)):
        for _ in range(256):
            f=fresh(0,0x40000|(bank<<21)|rm);v=[finite() for _ in range(4)]
            f.banks[bank][n:n+4]=v
            f.banks[bank^1][:]=[0x3F800000 if i%5==0 else 0 for i in range(16)]
            assert execute(0xF1FD|(n<<8),ctypes.byref(f),0)==0
            assert list(f.banks[bank][n:n+4])==v
            counts['matrix_invariant_checks']+=1
    # Hand-specified class/status cases independent of adapter primitive calls.
    policies=0
    for en in range(32):
        for word in (0xF0ED,0xF1FD):
            f=fresh(0,0x40000|(en<<7));f.banks[0][:]=[0]*16;f.banks[1][:]=[0]*16
            before=bytes(f);result=execute(word,ctypes.byref(f),0)
            expected_trap=bool(en&(0x17 if word==0xF1FD else 7))
            assert result==expected_trap and f.fpscr==(0x40000|(en<<7)|0x300C)
            if result:assert bytes(f)[:128]==before[:128]
            else:assert all(x==0 for row in f.banks for x in row)
            policies+=1
    files=[Path(__file__),PROJECT/'build_sh4_vector_numeric.ps1',VENDOR,PROJECT/'third_party/skmp-sh4-fpu/README.txt',
           PROJECT/'source/sh4_vector_numeric.c',PROJECT/'source/sh4_vector_numeric.h',PROJECT/'source/sh4_vector_numeric_reference.c',
           PROJECT/'source/fpu_cleanup_transfer.h',PROJECT/'source/stage00_task.h',PROJECT/'source/level_variant.h',
           PROJECT/'verify_fpu_cleanup_transfer.py']
    report={'counts':counts,'total_checks_passed':sum(counts.values())+policies,'zero_status_policy_checks':policies,
        'source':'source/sh4_vector_numeric.c','source_sha256':digest(PROJECT/'source/sh4_vector_numeric.c'),
        'vendor_revision':'29f17b480213f063c6db998ecba5745d3f4ad4b3','vendor_sha256':VENDOR_SHA,
        'vendor_url':'https://gist.github.com/skmp/4bd7b38149fc455978b5104201573236',
        'dll_sha256':digest(BUILD/'sh4_vector_numeric.dll'),'primitive_reference_dll_sha256':digest(BUILD/'sh4_vector_numeric_reference.dll'),
        'sha256':{str(p):digest(p) for p in files},'max_fsca_absolute_error':max_sine_error,
        'max_fsrra_ulp_error':max_rsqrt_ulp,'max_dot_error_bound_fraction':worst_dot_ratio,
        'scope':'All 44 opcode/register variants, both banks/SZ/PR/DN/RM0/1, all 32 enables, preset flags; full 16-bit opcode rejection; all FSCA angles, sampled FSRRA normal domain, Fraction-based dot bounds and identity matrix invariants.',
        'limitations':['Primitive API comparisons share the vendored core and establish adapter wiring, not independent arithmetic or hardware equivalence.',
                       'Mathematical bounds/invariants are independent local checks, not bit-exact reproduction of physical captures.',
                       'Upstream hardware-validation claims belong to the author; original captures were not available in the inspected public repository and were not replayed here.',
                       'Reserved RM2/3 and unknown opcodes return -1 atomically; actual CPU exception dispatch, MMIO, timing and gameplay remain unverified.']}
    (PROJECT/'analysis/sh4_vector_numeric_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('counts','total_checks_passed','zero_status_policy_checks','max_fsca_absolute_error','max_fsrra_ulp_error','max_dot_error_bound_fraction')},indent=2))


if __name__=='__main__':main()
