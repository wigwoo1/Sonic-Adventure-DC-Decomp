"""Original-opcode toward-zero inverse/determinant and independent scalar checks."""
import argparse,ctypes as C,hashlib,json,random,struct,sys
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
def matrix(v):return struct.pack('<16f',*v)

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    build=json.loads((d/'build_verification.json').read_text())
    assert build['exit_code']==build['diagnostic_bytes']==0 and build['inverse_scalar_mode']=='toward_zero_DN1_exceptions_disabled'
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    pin=json.loads((p/'jp_port_tools/jp_inverse_matrix_external_inputs.json').read_text())
    assert sha(a.binary)==pin['original_binary_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    dll=Path(build['DLL']);assert sha(dll)==build['DLL_sha256']
    sys.path.insert(0,str(p))
    from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import execute_add_sub,execute_multiply,execute_divide
    from verify_native_inverse_matrix import rational_inverse
    lib=C.CDLL(str(dll));P=C.c_void_p;I=C.c_int32;U=C.c_uint32
    for n,ret,args in [('njInitMatrix',None,[P,I,I]),('njPushMatrix',I,[P]),('njInvertMatrix',I,[P]),
                       ('jp_inverse_probe_det_bits',U,[P]),('njGetMatrix',None,[P]),('jp_shz_store',None,[P]),
                       ('jp_shz_store_from_other_unit',None,[P]),('jp_ninja_current_matrix_storage',P,[]),('shz_xmtrx_state_',P,[])]:
        fn=getattr(lib,n);fn.restype=ret;fn.argtypes=args
    for n in ['add','subtract','multiply','divide']:
        fn=getattr(lib,'jp_rz_scalar32_'+n);fn.restype=U;fn.argtypes=[U,U]
    rng=random.Random(0x525a3137);checks=0;sections={};calls={'njDetMatrix':0,'njInvertMatrix':0};singular=0
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    for case in range(1024):
        values=[(rng.getrandbits(1)<<31)|(rng.randrange(255)<<23)|rng.getrandbits(23) for i in range(2)]
        edge=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff]
        if case<64:values=[edge[case%8],edge[case//8]]
        for n in ['add','subtract','multiply','divide']:
            f=Fpu();f.fpscr=0x40001;f.banks[0][0],f.banks[0][1]=values
            if n in ['add','subtract']:execute_add_sub(f,0,1,n=='subtract')
            elif n=='multiply':execute_multiply(f,0,1)
            else:execute_divide(f,0,1)
            check(getattr(lib,'jp_rz_scalar32_'+n)(*values)==f.banks[0][0],('independent RZ scalar',n,values))
    exceptional=[('add',0x7f800000,0xff800000,0x7fbfffff),('add',0xff800000,0x3f800000,0xff800000),
        ('subtract',0x7f800000,0x7f800000,0x7fbfffff),('subtract',0x3f800000,0xff800000,0x7f800000),
        ('multiply',0x7f800000,0,0x7fbfffff),('multiply',0x7f800000,1,0x7fbfffff),
        ('multiply',0xff800000,0xbf800000,0x7f800000),('divide',0x7f800000,0xff800000,0x7fbfffff),
        ('divide',0x3f800000,0xff800000,0x80000000),('divide',0xff800000,0xbf800000,0x7f800000),
        ('divide',0,0,0x7fbfffff),('divide',0x3f800000,0,0x7f800000)]
    for n,x,y,expected in exceptional:check(getattr(lib,'jp_rz_scalar32_'+n)(x,y)==expected,('scalar class',n,x,y))
    for n in ['add','subtract','multiply','divide']:
        for nan in [0x7f800001,0x7fbfffff,0x7fc00000,0xffc12345]:
            check(getattr(lib,'jp_rz_scalar32_'+n)(nan,0x3f800000)==0x7fbfffff,('scalar NaN canonical',n,nan))
    sections['scalar_values_against_integer_dyadics_and_explicit_class_results']=checks
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    arena=(C.c_ubyte*512)();stack=(C.c_ubyte*512)();out=(C.c_ubyte*64)()
    def scenario(name,raw,bank,where):
        nonlocal singular
        count=sum(calls.values());lib.njInitMatrix(C.addressof(stack)+64,6,count%2)
        for i in range(count%6):check(lib.njPushMatrix(None)==1,'fill legal deferred slots')
        ar,st=bytearray(rng.randbytes(512)),bytearray(rng.randbytes(512));offset=128 if where=='saved' else 132
        (st if where=='saved' else ar)[offset:offset+64]=raw
        C.memmove(arena,bytes(ar),512);C.memmove(stack,bytes(st),512)
        ram[0xd10000:0xd10200]=ar;ram[0xd20000:0xd20200]=st
        active=raw if where=='current' else matrix([rng.randrange(-3,4) for i in range(16)])
        pointer=lib.jp_ninja_current_matrix_storage();C.memmove(pointer,active,64)
        metadata={n:C.c_int32.in_dll(lib,n).value for n in ['_stack_size_','_max_stack_size_','_nj_matrix_flag_']}
        pointers={n:C.c_void_p.in_dll(lib,n).value for n in ['_nj_current_matrix_ptr_','_nj_base_matrix_ptr_']}
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)]
        f.banks[bank^1][:]=struct.unpack('<16I',active)
        s=State();s.r[15]=0x8cf00000;s.pc=int(pin['entry_points'][name],16);s.pr=0x8cfed000;s.sr=0x70000000
        guest=0 if where=='current' else (0x8cd20000 if where=='saved' else 0x8cd10000)+offset
        host=None if where=='current' else C.addressof(stack if where=='saved' else arena)+offset
        s.r[4]=guest;reference(ram,s,fpu=f,instruction_budget=4000)
        check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),'original preserves numerical mode and stack')
        if name=='njDetMatrix':
            code=lib.jp_inverse_probe_det_bits(host)
            check(code==f.banks[bank][0],('RZ determinant word',raw.hex(),bank,where,hex(code),hex(f.banks[bank][0])))
        else:
            code=lib.njInvertMatrix(host);check(code==s.r[0],('RZ inverse return',raw.hex(),bank,where))
            singular+=code==0
        check(bytes(arena)==bytes(ram[0xd10000:0xd10200]),'all parameter/guard memory equals original')
        check(bytes(stack)==bytes(ram[0xd20000:0xd20200]),'all deferred-slot/guard memory equals original')
        expected=words(list(f.banks[bank^1]))
        for n in ['njGetMatrix','jp_shz_store','jp_shz_store_from_other_unit']:
            getattr(lib,n)(C.byref(out));check(bytes(out)==expected,('shared current state',n,where))
        check(lib.shz_xmtrx_state_()==lib.jp_ninja_current_matrix_storage()==pointer,'one actual active owner')
        for n,v in metadata.items():check(C.c_int32.in_dll(lib,n).value==v,'stable metadata '+n)
        for n,v in pointers.items():check(C.c_void_p.in_dll(lib,n).value==v,'stable pointer '+n)
        calls[name]+=1
        result=expected if where=='current' else bytes(ram[guest-0x8c000000:guest-0x8c000000+64])
        return code,result
    identity=[int(i%5==0) for i in range(16)];exact=[]
    for case in range(24):
        rows=[identity[4*i:4*i+4] for i in range(4)]
        for step in range(6):
            x,y=rng.sample(range(4),2);factor=rng.choice([-1,1]);rows[x]=[a+factor*b for a,b in zip(rows[x],rows[y])]
        exact.append([v for row in rows for v in row])
    families={'exact_and_independent_rational_inverse':[(matrix(v),v) for v in exact],
              'finite_random':[(matrix([rng.uniform(-3,3) for i in range(16)]),None) for case in range(96)]}
    singulars=[]
    for case in range(24):
        values=[rng.randrange(-4,5) for i in range(16)];values[12:16]=values[4:8] if case%2 else [0]*4
        singulars.append((matrix(values),None))
    families['singular_rows']=singulars
    families['signed_zero']=[(words([0x3f800000 if i%5==0 else rng.choice([0,0x80000000]) for i in range(16)]),None) for case in range(8)]
    small=[]
    for word in [0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x01000000,0x03800000,0x33800000]:
        v=[0x3f800000 if i%5==0 else 0 for i in range(16)];v[15]=word;small.append((words(v),None))
    families['tiny_DN_and_exact_zero_fallback']=small
    # Re-run the precise rounding mismatch discovered by the context audit.
    context=json.loads((r/'work/sonic-adventure/jp_fpscr_context_verification_v1.json').read_text())
    witness=context['rounding_mode_mismatch_witness'];families['reproduced_context_rounding_witness']=[(words(witness['input_words']),None)]
    for family,cases in families.items():
        before=checks;before_calls=sum(calls.values())
        for raw,exact_values in cases:
            for bank in [0,1]:
                for where in ['stored','current','saved']:
                    det,_=scenario('njDetMatrix',raw,bank,where);code,result=scenario('njInvertMatrix',raw,bank,where)
                    if not code:check(result==words([0x7f7fc99e]*16),'exact singular sentinel')
                    if exact_values is not None:
                        true_det,true_inverse=rational_inverse(exact_values)
                        check(struct.unpack('<f',words([det]))[0]==float(true_det),'independent exact determinant')
                        check(code==1 and struct.unpack('<16f',result)==tuple(map(float,true_inverse)),'independent rational inverse')
                    if family=='reproduced_context_rounding_witness':
                        check(result==words(witness['toward_zero_words']) and result!=words(witness['nearest_words']),'actual diagnosed mismatch corrected')
        sections[family]=dict(checks=checks-before,original_calls=sum(calls.values())-before_calls)
        print(family,sections[family],flush=True)
    check(singular>0,'singular branch tested')
    report=dict(status='toward_zero_inverse_determinant_and_scalar_values_verified',checks=checks,sections=sections,
        original_instruction_contract_comparisons=sum(calls.values()),per_function=calls,singular_inverse_calls=singular,
        original_binary_sha256=sha(a.binary),DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),
        verifier_sha256=sha(Path(__file__)),exact_scalar_oracle_sha256=sha(p/'sh4_binary32.py'),
        original_executor_sha256=sha(p/'verify_startup.py'),rational_oracle_source_sha256=sha(Path(__file__).with_name('verify_native_inverse_matrix.py')),
        context_report_sha256=sha(r/'work/sonic-adventure/jp_fpscr_context_verification_v1.json'),numerical_mode='toward_zero_DN1_exceptions_disabled',
        context_mismatch_corrected=True,single_existing_active_state=True,old_nearest_proofs_preserved=True,
        limitations=['Original matrix comparisons cover finite inputs and finite intermediate values, both banks and stored/current/saved-slot cases.',
                     'Scalar class cases do not establish complete original matrix nonfinite propagation.',
                     'Other existing Ninja/renderer arithmetic remains nearest-mode and requires a deliberate migration; this DLL is not whole-game RZ integration.',
                     'FPSCR flags, traps, BIOS handoff, full frame execution, native performance and gameplay are unqualified.'],
        game_source_linked=False,retail_assets_published=False,overall_completion_percent=10,goal_paused=False,playable=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(sum(calls.values()),'original calls;',checks,'checks passed')

if __name__=='__main__':main()
