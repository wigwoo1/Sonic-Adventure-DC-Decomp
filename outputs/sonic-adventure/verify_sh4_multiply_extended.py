"""Exceptional class-table, alias/trap and finite-regression tests."""
import ctypes,hashlib,itertools,json,random
from pathlib import Path
from verify_startup import ROOT
from verify_fpu_cleanup_transfer import Fpu
from sh4_binary32 import execute_multiply
from sh4_multiply_extended_reference import exceptional

PROJECT=Path(__file__).resolve().parent
DLL=ROOT/'work/sonic-adventure/build/sh4_multiply_extended.dll'
LIB=ctypes.CDLL(str(DLL))
extended=LIB.sa_sh4_multiply_extended
legacy=LIB.sa_sh4_multiply
for fn in (extended,legacy):fn.argtypes=[ctypes.POINTER(ctypes.c_uint32)]*4;fn.restype=ctypes.c_int
VALUES=(0,0x80000000,1,0x80000001,0x7FFFFF,0x807FFFFF,0x800000,0x80800000,
        0x3F800000,0xBF800000,0x7F7FFFFF,0xFF7FFFFF,0x7F800000,0xFF800000,
        0x7F800001,0xFF800001,0x7FBFFFFF,0xFFBFFFFF,0x7FC00000,0xFFFFFFFF)


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def call(fn,a,b,status,alias,same=False):
    data=(ctypes.c_uint32*6)(0x11223344,a,b,status,0xDEADBEEF,0x55667788)
    pointer=lambda i:ctypes.cast(ctypes.byref(data,4*i),ctypes.POINTER(ctypes.c_uint32))
    result=fn(pointer(1),pointer(1 if same else 2),pointer(3),pointer((4,1,2)[alias]))
    assert data[0]==0x11223344 and data[5]==0x55667788
    return result,list(data)


def main():
    cases=0;infinity_only=0;nan_cases=0;denormal_errors=0;traps=0
    for a,b,en,dn,rm,flags,alias in itertools.product(VALUES,VALUES,range(32),range(2),range(2),(0,0x3F07C),range(3)):
        # Focus manual class coverage on new infinities and existing NaN priority.
        if (a&0x7FFFFFFF)<0x7F800000 and (b&0x7FFFFFFF)<0x7F800000:continue
        status=(en<<7)|(dn<<18)|rm|flags
        destination=(0xDEADBEEF,a,b)[alias]
        wanted,after,out=exceptional(a,b,status,destination)
        result,data=call(extended,a,b,status,alias)
        expected=[0x11223344,a,b,after,0xDEADBEEF,0x55667788]
        expected[(4,1,2)[alias]]=out
        assert result==wanted and data==expected,(hex(a),hex(b),hex(status),alias,result,wanted,data,expected)
        cases+=1;traps+=wanted==1
        nan_cases+=(a&0x7FFFFFFF)>0x7F800000 or (b&0x7FFFFFFF)>0x7F800000
        infinity_only+=(a&0x7FFFFFFF)<=0x7F800000 and (b&0x7FFFFFFF)<=0x7F800000
        denormal_errors+=bool(after&0x20000)
    same_cases=0
    for a,en,dn,rm,alias in itertools.product(VALUES,range(32),range(2),range(2),(0,1)):
        if (a&0x7FFFFFFF)<0x7F800000:continue
        status=(en<<7)|(dn<<18)|rm|0x3F07C
        wanted,after,out=exceptional(a,a,status,0xDEADBEEF if alias==0 else a)
        result,data=call(extended,a,0xA5A5A5A5,status,alias,same=True)
        expected=[0x11223344,a,0xA5A5A5A5,after,0xDEADBEEF,0x55667788];expected[4 if alias==0 else 1]=out
        assert result==wanted and data==expected
        same_cases+=1
    guards=0
    for a,b,mode,alias in itertools.product(VALUES,VALUES,(0x80000,2,3,0x80002,0x80003),range(3)):
        result,data=call(extended,a,b,mode|0x40000|0x3F07C,alias)
        assert result==-1 and data==[0x11223344,a,b,mode|0x40000|0x3F07C,0xDEADBEEF,0x55667788]
        guards+=1
    rng=random.Random(0xF002E)
    pairs=[(a,b) for a,b in itertools.product(VALUES[:12],repeat=2)]
    pairs.extend((rng.randrange(0x800000,0x7F800000)|(rng.getrandbits(1)<<31),
                  rng.randrange(0x800000,0x7F800000)|(rng.getrandbits(1)<<31)) for _ in range(2000))
    finite_checks=0;delegation=0
    for (a,b),dn,rm,alias in itertools.product(pairs,range(2),range(2),range(3)):
        status=(dn<<18)|rm|0x3F07C
        got,data=call(extended,a,b,status,alias)
        old,old_data=call(legacy,a,b,status,alias)
        assert got==old and data==old_data
        delegation+=1
        # Independent existing Python finite arithmetic, including DN effects.
        if not dn and any(0<(v&0x7FFFFFFF)<0x800000 for v in (a,b)):
            assert got==1;continue
        f=Fpu();f.fpscr=status;f.banks[0][0]=a;f.banks[0][1]=b
        execute_multiply(f,0,1)
        assert got==0 and data[3]==f.fpscr
        assert data[(4,1,2)[alias]]==f.banks[0][0]
        if alias!=1:assert data[1]==(a if a&0x7FFFFFFF>=0x800000 or not dn else a&0x80000000)
        if alias!=2:assert data[2]==f.banks[0][1]
        finite_checks+=1
    files=[Path(__file__),PROJECT/'sh4_multiply_extended_reference.py',PROJECT/'sh4_binary32.py',
           PROJECT/'build_sh4_multiply_extended.ps1',PROJECT/'source/sh4_multiply_extended.c',PROJECT/'source/sh4_multiply_extended.h',
           PROJECT/'source/sh4_multiply.c',PROJECT/'source/sh4_fmac.c',PROJECT/'source/sh4_fmac.h']
    report={'manual_class_cases_passed':cases,'infinity_only_cases':infinity_only,'nan_priority_cases':nan_cases,
            'denormal_error_cases':denormal_errors,'trapping_class_cases':traps,'same_operand_alias_cases_passed':same_cases,
            'unsupported_mode_guards_passed':guards,'finite_delegation_cases_passed':delegation,'independent_finite_checks_passed':finite_checks,
            'source':'source/sh4_multiply_extended.c','source_sha256':digest(PROJECT/'source/sh4_multiply_extended.c'),
            'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'manual':'https://www.renesas.com/en/document/mas/sh-4-software-manual',
            'scope':'New infinity/zero/signed/denormal class handling, NaN priority, all 32 exception enables, DN/RM, sticky/cause bits, destination and same-operand aliases; finite behavior delegated unchanged and compared to independent Python arithmetic.',
            'limitations':['Manual-derived software model, not new physical-hardware measurements. CPU exception delivery is not emulated.',
                           'PR=1 and reserved RM2/3 return -1 atomically; existing finite delegate support/operand-flush semantics remain unchanged.']}
    (PROJECT/'analysis/sh4_multiply_extended_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k.endswith('_passed') or k.endswith('_cases')},indent=2))


if __name__=='__main__':main()
