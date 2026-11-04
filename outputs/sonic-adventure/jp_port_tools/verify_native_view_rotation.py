"""Original-opcode view rotations, independent integer-dyadic fused arithmetic,
guarded shared state and independent geometric/roll-wrap expectations.
"""
import argparse,ctypes as C,hashlib,json,math,random,struct,sys
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    pin_path=p/'jp_port_tools/jp_view_rotation_external_inputs.json';pin=json.loads(pin_path.read_text())
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    build=json.loads((d/'build_verification.json').read_text());assert build['exit_code']==build['diagnostic_bytes']==0
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(build['DLL']);assert sha(dll)==build['DLL_sha256']
    sys.path.insert(0,str(p))
    from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import execute_fmac,execute_multiply,execute_add_sub
    lib=C.CDLL(str(dll));P=C.c_void_p;I=C.c_int32;U=C.c_uint32
    for n,ret,args in [('njInitMatrix',None,[P,I,I]),('njGetMatrix',None,[P]),('jp_shz_store',None,[P]),
                       ('jp_shz_store_from_other_unit',None,[P]),('jp_ninja_current_matrix_storage',P,[]),('shz_xmtrx_state_',P,[]),
                       ('jp_rz_scalar32_fmac',U,[U,U,U])]+[(n,None,[P,I]) for n in pin['entry_points']]:
        fn=getattr(lib,n);fn.restype=ret;fn.argtypes=args
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric
    numeric.restype=C.c_int;numeric.argtypes=[U,C.POINTER(Fpu),U]
    def service(op,f):assert numeric(op,C.byref(f),f.fpul)==0
    rng=random.Random(0x56524f54);checks=0;sections={};calls=dict.fromkeys(pin['entry_points'],0)
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    def finite():return (rng.getrandbits(1)<<31)|(rng.randrange(255)<<23)|rng.getrandbits(23)
    edge=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff,0x3f800000,0xbf800000]
    for case in range(4096):
        triple=[finite() for i in range(3)]
        if case<1000:triple=[edge[case%10],edge[(case//10)%10],edge[case//100]]
        f=Fpu();f.fpscr=0x40001;f.banks[0][0],f.banks[0][1],f.banks[0][2]=triple
        execute_fmac(f,2,1)
        check(lib.jp_rz_scalar32_fmac(*triple)==f.banks[0][2],('independent fused scalar result',triple))
    check(lib.jp_rz_scalar32_fmac(0x3f800001,0x3f7ffffe,0xbf800000)==0xa8800000,'exact fused cancellation counterexample')
    exceptional=[(0x3f800000,0x7f800000,0x3f800000,0x7f800000),
                 (0xbf800000,0x7f800000,0x3f800000,0xff800000),
                 (0x3f800000,0xff800000,0x7f800000,0x7fbfffff),
                 (0x3f800000,0xff800000,0xff800000,0xff800000),
                 (0,0x7f800000,0x3f800000,0x7fbfffff),
                 (1,0x7f800000,0x3f800000,0x7fbfffff),
                 (0x3f800000,0x3f800000,0xff800000,0xff800000),
                 (0x7f800000,0x3f800000,0,0x7f800000),
                 (0xff800000,0x3f800000,0x7f800000,0x7fbfffff)]
    for x,y,z,expected in exceptional:check(lib.jp_rz_scalar32_fmac(x,y,z)==expected,'explicit documented nonfinite scalar value')
    for nan in [0x7f800001,0x7fc12345,0xff800001,0xffc54321]:
        for position in range(3):
            triple=[0x3f800000]*3;triple[position]=nan
            check(lib.jp_rz_scalar32_fmac(*triple)==0x7fbfffff,'NaN scalar value dispatch')
    sections['scalar_fused_dyadic_and_explicit_class_values']=checks
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;BASE=0x8c000000
    stack=(C.c_ubyte*512)();stored=(C.c_ubyte*512)();current_view=(C.c_ubyte*512)();out=(C.c_ubyte*64)()
    arenas=[(0x8cd00000,stack),(0x8cd10000,stored),(0x8cd20000,current_view)]
    table={n:int(v,16) for n,v in pin['original_globals'].items()};vp=P.in_dll(lib,'_nj_view_')
    view_matrix=(U*16).in_dll(lib,'_nj_view_matrix_');unit=(U*16).in_dll(lib,'_unit_matrix_')
    angles=[0,1,-1,0x4000,0x8000,0xc000,0xffff,0x10000,0x7fffffff,-0x80000000,0x12345678,-0x12345678]
    fused_differences=0;fused_witness=None;geometry=0;roll_wrap=0
    def store(address,raw):ram[address-BASE:address-BASE+len(raw)]=raw
    def read(address,count):return bytes(ram[address-BASE:address-BASE+count])
    def trace(pc,op,registers,sr,pr,raw):
        nonlocal fused_differences,fused_witness
        if op&0xf00f!=0xf00e:return
        n,m=(op>>8)&15,(op>>4)&15
        fused=Fpu.from_buffer_copy(raw);separate=Fpu.from_buffer_copy(raw);bank=(fused.fpscr>>21)&1
        triple=[fused.banks[bank][i] for i in [0,m,n]]
        execute_fmac(fused,n,m);execute_multiply(separate,0,m);execute_add_sub(separate,n,0)
        if fused.banks[bank][n]!=separate.banks[bank][n]:
            fused_differences+=1
            if fused_witness is None:fused_witness=dict(operands=triple,fused=fused.banks[bank][n],separately_rounded=separate.banks[bank][n])
    def scenario(name,bank,implicit,case):
        nonlocal geometry,roll_wrap
        lib.njInitMatrix(C.addressof(stack)+64,6,case%2)
        for address,native in arenas:
            raw=rng.randbytes(512);C.memmove(native,raw,512);store(address,raw)
        depth=case%6+1;metadata={'_stack_size_':depth,'_max_stack_size_':6,'_nj_matrix_flag_':case%2}
        pointers={'_nj_base_matrix_ptr_':(0x8cd00040,C.addressof(stack)+64),
                  '_nj_current_matrix_ptr_':(0x8cd00040+64*(depth-1),C.addressof(stack)+64+64*(depth-1))}
        for n,v in metadata.items():C.c_int32.in_dll(lib,n).value=v;store(table[n],words([v]))
        for n,(guest,host) in pointers.items():P.in_dll(lib,n).value=host;store(table[n],words([guest]))
        active=rng.randbytes(64);pointer=lib.jp_ninja_current_matrix_storage();C.memmove(pointer,active,64)
        vm=rng.randbytes(64);C.memmove(view_matrix,vm,64);store(table['_nj_view_matrix_'],vm)
        un=rng.randbytes(64);C.memmove(unit,un,64);store(table['_unit_matrix_'],un)
        view_guest,view_host=0x8cd20084,C.addressof(current_view)+132
        vp.value=view_host;store(table['_nj_view_'],words([view_guest]));check(view_host>0xffffffff,'full-width current-view pointer')
        if implicit or case%3==0:target_guest,target_host=view_guest,view_host
        else:target_guest,target_host=0x8cd10084,C.addressof(stored)+132
        raw=bytearray(rng.randbytes(120))
        xyz=[finite() for i in range(3)]
        if case<12:xyz=list(struct.unpack('<3I',struct.pack('<3f',1.25,-2.5,3.75)))
        if case in range(12,32):xyz=[edge[(case+i)%10] for i in range(3)]
        if name=='njRotateViewXAbsolute':struct.pack_into('<3I',raw,104,*xyz)
        elif name=='njRotateViewYRelative':struct.pack_into('<3I',raw,76,*xyz)
        original=bytes(raw);C.memmove(target_host,original,120);store(target_guest,original)
        unsigned_angle=(angles[case] if case<len(angles) else rng.getrandbits(32))&0xffffffff
        angle=unsigned_angle if unsigned_angle<0x80000000 else unsigned_angle-0x100000000
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)];f.banks[bank^1][:]=struct.unpack('<16I',active)
        s=State();s.r[15]=0x8cf00000;s.r[4]=0 if implicit else target_guest;s.r[5]=unsigned_angle
        s.pc=int(pin['entry_points'][name],16);s.pr=0x8cfed000;s.sr=0x70000000
        reference(ram,s,fpu=f,vector_instruction=service,instruction_budget=256,trace=trace)
        getattr(lib,name)(None if implicit else target_host,angle)
        check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),'original restores SP/bank/width/mode')
        for address,native in arenas:check(bytes(native)==read(address,512),('full view/stack arenas and guards',name,bank,implicit,case))
        check(bytes(view_matrix)==vm==read(table['_nj_view_matrix_'],64),'saved view matrix unchanged')
        check(bytes(unit)==un==read(table['_unit_matrix_'],64),'unit matrix unchanged')
        check(vp.value==view_host,'actual current-view binding unchanged')
        for n,v in metadata.items():check(C.c_int32.in_dll(lib,n).value==v,'stack metadata unchanged '+n)
        for n,(guest,host) in pointers.items():check(P.in_dll(lib,n).value==host,'stack pointer unchanged '+n)
        for n in ['njGetMatrix','jp_shz_store','jp_shz_store_from_other_unit']:
            getattr(lib,n)(C.byref(out));check(bytes(out)==active==words(list(f.banks[bank^1])),('active matrix unchanged',n))
        check(lib.jp_ninja_current_matrix_storage()==lib.shz_xmtrx_state_()==pointer,'one stable active state owner')
        result=C.string_at(target_host,120)
        if name=='njRotateViewZAbsolute':
            base=struct.unpack_from('<I',original,116)[0]
            check(struct.unpack_from('<I',result,88)[0]==(base+unsigned_angle)&0xffffffff,'independent full-width integer roll wrap')
            roll_wrap+=1
        elif case<12:
            x,y,z=struct.unpack('<3f',words(xyz));theta=(unsigned_angle&0xffff)*math.tau/65536
            c,sine=math.cos(theta),math.sin(theta)
            expected=(x,y*c-z*sine,z*c+y*sine) if name=='njRotateViewXAbsolute' else (x*c+z*sine,y,z*c-x*sine)
            actual=struct.unpack_from('<3f',result,76)
            check(max(abs(v-e) for v,e in zip(actual,expected))<0.00001,'independent view geometry')
            geometry+=1
        calls[name]+=1
    for name in calls:
        before=checks
        for bank in [0,1]:
            for implicit in [False,True]:
                for case in range(256):scenario(name,bank,implicit,case)
        sections[name]=dict(checks=checks-before,original_calls=calls[name]);print(name,sections[name],flush=True)
    check(fused_differences>0,'original view operations distinguish fused from separate rounding')
    report=dict(status='three_original_view_rotation_contracts_and_RZ_fused_values_verified',checks=checks,sections=sections,
        original_instruction_contract_comparisons=sum(calls.values()),per_function=calls,independent_geometry_checks=geometry,independent_roll_wrap_checks=roll_wrap,
        original_fused_vs_separate_rounding_difference_count=fused_differences,fused_rounding_witness=fused_witness,
        full_native_current_view_pointer_tested=True,single_active_matrix_owner=True,numerical_policy='toward_zero_DN1_exceptions_disabled',
        original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),DLL_sha256=sha(dll),
        build_report_sha256=sha(d/'build_verification.json'),external_input_manifest_sha256=sha(pin_path),verifier_sha256=sha(Path(__file__)),
        executor_sha256=sha(p/'verify_startup.py'),scalar_reference_sha256=sha(p/'sh4_binary32.py'),
        limitations=['Original X/Y numerical comparisons cover finite operated-on direction values with RZ/DN1 finite intermediates, signed zero and DN inputs; untouched view words can be arbitrary.',
                     'Z integer wrap is checked across full-width angle/base words. Stored/current targets and aliases to the current target retain all surrounding view and matrix state.',
                     'Nonfinite FMAC class cases are explicit value checks, not whole-routine or physical hardware qualification. Existing approximation model supplies FSCA values.',
                     'FPSCR flags/traps, BIOS handoff, njSetView, complete renderer arithmetic, full frames, performance, gameplay, English and later ports remain unqualified.'],
        game_source_linked=False,retail_assets_published=False,overall_completion_percent=10,goal_paused=False,playable=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(sum(calls.values()),'original calls;',checks,'checks;',fused_differences,'fused rounding differences')

if __name__=='__main__':main()
