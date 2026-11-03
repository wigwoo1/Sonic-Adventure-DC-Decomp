"""Original-opcode determinant/inverse comparisons plus independent algebra.
Scalar instructions use the existing exact integer-dyadic Python executor.
No native scalar helper is used as the original instruction service.
"""
import argparse,ctypes as C,hashlib,json,random,struct,sys
from fractions import Fraction
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(values):return struct.pack('<'+'I'*len(values),*values)
def matrix(values):return struct.pack('<16f',*values)

def rational_inverse(values):
    rows=[[Fraction(values[4*i+j]) for j in range(4)]+[Fraction(i==j) for j in range(4)] for i in range(4)]
    determinant=Fraction(1)
    for col in range(4):
        pivot=next((j for j in range(col,4) if rows[j][col]),None)
        if pivot is None:return Fraction(0),None
        if pivot!=col:rows[col],rows[pivot]=rows[pivot],rows[col];determinant=-determinant
        scale=rows[col][col];determinant*=scale;rows[col]=[v/scale for v in rows[col]]
        for row in range(4):
            if row!=col:
                factor=rows[row][col];rows[row]=[a-factor*b for a,b in zip(rows[row],rows[col])]
    return determinant,[v for row in rows for v in row[4:]]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,d=[getattr(a,n).resolve() for n in ['workspace','upstream','build']];p=r/'outputs/sonic-adventure'
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    build=json.loads((d/'build_verification.json').read_text());pin_path=p/'jp_port_tools/jp_inverse_matrix_external_inputs.json';pin=json.loads(pin_path.read_text())
    assert build['exit_code']==build['diagnostic_bytes']==0
    dll=Path(build['DLL']);assert sha(dll)==build['DLL_sha256'] and sha(a.binary)==pin['original_binary_sha256']
    for n,h in dict(build['authored_source_sha256'],**pin['reused_authored_scalar_cores_sha256']).items():assert sha(r/n)==h,n
    for n,h in pin['files_sha256'].items():assert sha(u/n)==h,n
    sys.path.insert(0,str(p))
    from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import execute_add_sub,execute_multiply,execute_divide
    lib=C.CDLL(str(dll));P=C.c_void_p;U=C.c_uint32;I=C.c_int32
    for n,ret,args in [('njInitMatrix',None,[P,I,I]),('njPushMatrix',I,[P]),('njInvertMatrix',I,[P]),
        ('jp_inverse_probe_det_bits',U,[P]),('njGetMatrix',None,[P]),('jp_shz_store',None,[P]),
        ('jp_shz_store_from_other_unit',None,[P]),('jp_ninja_current_matrix_storage',P,[]),('shz_xmtrx_state_',P,[])]:
        fn=getattr(lib,n);fn.restype=ret;fn.argtypes=args
    for n in ['add','subtract','multiply','divide']:
        fn=getattr(lib,'jp_scalar32_'+n);fn.restype=U;fn.argtypes=[U,U]
    rng=random.Random(0x494e5652);checks=0;calls=dict.fromkeys(pin['entry_points'],0);sections={};singular=0
    def check(value,label):
        nonlocal checks
        assert value,label
        checks+=1
    start=checks
    for i in range(512):
        operands=[(rng.getrandbits(1)<<31)|(rng.randrange(255)<<23)|rng.getrandbits(23) for j in range(2)]
        if i<16:
            edge=[0,0x80000000,1,0x80000001,0x00800000,0x80800000,0x3f800000,0xbf800000]
            operands=[edge[i%8],edge[(i//2)%8]]
        for name in ['add','subtract','multiply','divide']:
            f=Fpu();f.fpscr=0x40000;f.banks[0][0],f.banks[0][1]=operands
            if name in ['add','subtract']:execute_add_sub(f,0,1,name=='subtract')
            elif name=='multiply':execute_multiply(f,0,1)
            else:execute_divide(f,0,1)
            check(getattr(lib,'jp_scalar32_'+name)(*operands)==f.banks[0][0],('independent scalar value',name,operands))
    exceptional=[('add',0x7f800000,0xff800000,0x7fbfffff),('add',0xff800000,0x3f800000,0xff800000),
        ('subtract',0x7f800000,0x7f800000,0x7fbfffff),('subtract',0x3f800000,0xff800000,0x7f800000),
        ('multiply',0x7f800000,0,0x7fbfffff),('multiply',0x7f800000,1,0x7fbfffff),
        ('multiply',0xff800000,0xbf800000,0x7f800000),('divide',0x7f800000,0xff800000,0x7fbfffff),
        ('divide',0x3f800000,0xff800000,0x80000000),('divide',0xff800000,0xbf800000,0x7f800000),
        ('divide',0,0,0x7fbfffff),('divide',0x3f800000,0,0x7f800000)]
    for name,x,y,expected in exceptional:check(getattr(lib,'jp_scalar32_'+name)(x,y)==expected,('exceptional scalar value',name,x,y))
    sections['scalar_exact_integer_dyadic_oracle_and_exceptional_values']=checks-start
    BASE,ARENA,STACK=0x8c000000,0x8cd10000,0x8cd20000
    arena=(C.c_ubyte*512)();stack=(C.c_ubyte*512)();out=(C.c_ubyte*64)()
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    def store(address,raw):ram[address-BASE:address-BASE+len(raw)]=raw
    def read(address,count):return bytes(ram[address-BASE:address-BASE+count])
    def scenario(name,raw,bank,implicit,saved=False):
        nonlocal singular
        count=sum(calls.values());lib.njInitMatrix(C.addressof(stack)+64,6,count%2)
        for i in range(count%6):check(lib.njPushMatrix(None)==1,'setup deferred stack')
        ar,st=bytearray(rng.randbytes(512)),bytearray(rng.randbytes(512));offset=128 if saved else 132
        (st if saved else ar)[offset:offset+64]=raw
        C.memmove(arena,bytes(ar),512);C.memmove(stack,bytes(st),512);store(ARENA,ar);store(STACK,st)
        active=raw if implicit else matrix([rng.randrange(-3,4) for j in range(16)])
        pointer=lib.jp_ninja_current_matrix_storage();C.memmove(pointer,active,64)
        scalars={n:C.c_int32.in_dll(lib,n).value for n in ['_stack_size_','_max_stack_size_','_nj_matrix_flag_']}
        pointers={n:C.c_void_p.in_dll(lib,n).value for n in ['_nj_current_matrix_ptr_','_nj_base_matrix_ptr_']}
        f=Fpu();f.fpscr=0x40000|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for j in range(16)];f.banks[bank^1][:]=struct.unpack('<16I',active)
        s=State();s.r[:]=[0x45670000+i for i in range(16)];s.r[15]=0x8cf00000
        guest=0 if implicit else (STACK if saved else ARENA)+offset
        host=None if implicit else C.addressof(stack if saved else arena)+offset
        s.r[4]=guest;s.pc=int(pin['entry_points'][name],16);s.pr=0x8cfed000;s.sr=0x70000000
        reference(ram,s,fpu=f,instruction_budget=4000)
        check(s.r[15]==0x8cf00000 and (f.fpscr&0x380000)==bank<<21,'original returns with stack and FPU bank restored')
        if name=='njDetMatrix':
            result=lib.jp_inverse_probe_det_bits(host);check(result==f.banks[bank][0],('determinant result words',raw.hex(),implicit,hex(result),hex(f.banks[bank][0])))
        else:
            result=lib.njInvertMatrix(host);check(result==s.r[0],('inverse return',raw.hex(),implicit,result,s.r[0]))
            if result==0:singular+=1
        check(bytes(arena)==read(ARENA,512),('parameter memory and guards',name,raw.hex(),implicit))
        check(bytes(stack)==read(STACK,512),('deferred records and guards',name,raw.hex(),implicit))
        expected=words(list(f.banks[bank^1]))
        for n in ['njGetMatrix','jp_shz_store','jp_shz_store_from_other_unit']:
            getattr(lib,n)(C.byref(out));check(bytes(out)==expected,('shared active words',name,raw.hex(),implicit,n,bytes(out).hex(),expected.hex()))
        check(lib.jp_ninja_current_matrix_storage()==lib.shz_xmtrx_state_()==pointer,'same active state owner')
        for n,v in scalars.items():check(C.c_int32.in_dll(lib,n).value==v,'unchanged scalar '+n)
        for n,v in pointers.items():check(C.c_void_p.in_dll(lib,n).value==v,'unchanged pointer '+n)
        calls[name]+=1
        return result,expected if implicit else read(guest,64)
    families={}
    identity=[int(i%5==0) for i in range(16)]
    exact=[]
    for case in range(48):
        rows=[identity[i*4:i*4+4] for i in range(4)]
        for i in range(6):
            x,y=rng.sample(range(4),2);factor=rng.choice([-1,1]);rows[x]=[a+factor*b for a,b in zip(rows[x],rows[y])]
        exact.append([v for row in rows for v in row])
    families['exact_unimodular_and_independent_rational_inverse']=[(matrix(v),v) for v in exact]
    families['finite_random']=[(matrix([rng.uniform(-3,3) for i in range(16)]),None) for j in range(96)]
    singulars=[]
    for case in range(32):
        values=[rng.randrange(-4,5) for i in range(16)]
        if case%2:values[12:16]=values[4:8]
        else:values[8:12]=[0]*4
        singulars.append((matrix(values),None))
    families['singular_zero_rows_and_repeated_rows']=singulars
    zeros=[]
    for case in range(16):
        raw=words([0x3f800000 if i%5==0 else rng.choice([0,0x80000000]) for i in range(16)])
        zeros.append((raw,None))
    families['signed_zero_cofactors']=zeros
    small=[]
    for word in [0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x01000000,0x03800000,0x33800000]:
        values=[0x3f800000 if i%5==0 else 0 for i in range(16)];values[15]=word;small.append((words(values),None))
    families['tiny_determinants_DN_and_exact_zero_fallback']=small
    for family,cases in families.items():
        before=checks;before_calls=sum(calls.values())
        for raw,exact_values in cases:
            for bank in [0,1]:
                for implicit in [False,True]:
                    det,unused=scenario('njDetMatrix',raw,bank,implicit)
                    code,inverse=scenario('njInvertMatrix',raw,bank,implicit)
                    if code==0:check(inverse==words([0x7f7fc99e]*16),'exact singular fill word')
                    if exact_values is not None:
                        true_det,true_inverse=rational_inverse(exact_values)
                        check(struct.unpack('<f',words([det]))[0]==float(true_det),'independent rational determinant')
                        check(code==1 and list(struct.unpack('<16f',inverse))==[float(v) for v in true_inverse],'independent pivoted rational inverse')
        sections[family]=dict(checks=checks-before,original_calls=sum(calls.values())-before_calls)
        print(family,sections[family],flush=True)
    before=checks
    for bank in [0,1]:
        for values in exact[:8]:
            scenario('njDetMatrix',matrix(values),bank,False,True)
            scenario('njInvertMatrix',matrix(values),bank,False,True)
    sections['stored_destination_is_a_saved_matrix_slot']=checks-before
    check(singular>0,'singular branch exercised')
    report=dict(status='native_determinant_inverse_and_scalar_contracts_verified',checks=checks,sections=sections,
        original_instruction_contract_comparisons=sum(calls.values()),per_function=calls,singular_inverse_calls=singular,
        original_binary_sha256=sha(a.binary),original_entry_points=pin['entry_points'],DLL_sha256=sha(dll),
        build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        exact_scalar_reference_sha256=sha(p/'sh4_binary32.py'),executor_sha256=sha(p/'verify_startup.py'),
        limitations=['Original matrix comparisons cover finite inputs with finite intermediate scalar results, both active banks, nearest rounding, DN=1 and SZ=PR=0.',
        'Scalar services include independent finite/overflow/underflow and explicit nonfinite class tests; complete matrix nonfinite propagation is not qualified against original execution.',
        'Native value operations use integer scalar cores; FPSCR flags, enabled traps and scratch-register effects are outside the native ABI.',
        'The whole-game FPSCR policy and native performance remain unqualified. Full frames/gameplay/localization and later ports are unfinished.'],
        game_source_linked=False,retail_assets_published=False,overall_completion_percent=10,goal_paused=False,playable=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(sum(calls.values()),'original calls;',checks,'checks passed')
if __name__=='__main__':main()
