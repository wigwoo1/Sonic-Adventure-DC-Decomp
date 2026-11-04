"""Execute the actual original init-view, scale and complete clear-stack paths.
Original clear calls real original scale/multiplication bodies, with no hooks.
"""
import argparse,ctypes as C,hashlib,json,random,struct,sys
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    pin_path=p/'jp_port_tools/jp_view_stack_external_inputs.json';pin=json.loads(pin_path.read_text())
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    build=json.loads((d/'build_verification.json').read_text());assert build['exit_code']==build['diagnostic_bytes']==0
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(build['DLL']);assert sha(dll)==build['DLL_sha256']
    sys.path.insert(0,str(p))
    from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    lib=C.CDLL(str(dll));P=C.c_void_p;I=C.c_int32;U=C.c_uint32;F=C.c_float
    for n,ret,args in [('njInitMatrix',None,[P,I,I]),('njInitView',None,[P]),('njScale',None,[P,F,F,F]),
                       ('njClearMatrix',None,[]),('njGetMatrix',None,[P]),('jp_shz_store',None,[P]),
                       ('jp_shz_store_from_other_unit',None,[P]),('jp_ninja_current_matrix_storage',P,[]),('shz_xmtrx_state_',P,[])]:
        fn=getattr(lib,n);fn.restype=ret;fn.argtypes=args
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric
    numeric.restype=C.c_int;numeric.argtypes=[U,C.POINTER(Fpu),U]
    def service(instruction,fpu):assert numeric(instruction,C.byref(fpu),fpu.fpul)==0
    table={n:int(v,16) for n,v in pin['original_symbols'].items()};BASE=0x8c000000
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    arena=(C.c_ubyte*512)();stack=(C.c_ubyte*512)();view=(C.c_ubyte*512)();out=(C.c_ubyte*64)()
    unit=(U*16).in_dll(lib,'_unit_matrix_');view_matrix=(U*16).in_dll(lib,'_nj_view_matrix_');view_pointer=P.in_dll(lib,'_nj_view_')
    ranges=[(0x8cd00000,stack),(0x8cd10000,view),(0x8cd20000,arena)]
    rng=random.Random(0x56494557);checks=0;calls=dict.fromkeys(['njInitView','njScale','njClearMatrix'],0);sections={}
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    def store(address,raw):ram[address-BASE:address-BASE+len(raw)]=raw
    def read(address,count):return bytes(ram[address-BASE:address-BASE+count])
    def store_word(address,value):store(address,words([value]))
    def finite():return (rng.getrandbits(1)<<31)|(rng.randrange(255)<<23)|rng.getrandbits(23)
    canonical_unit=words([0x3f800000 if i%5==0 else 0 for i in range(16)])
    independent=0
    def scenario(name,bank,where,case):
        nonlocal independent
        lib.njInitMatrix(C.addressof(stack)+64,6,case%2)
        for guest,native in ranges:
            raw=rng.randbytes(512);C.memmove(native,raw,512);store(guest,raw)
        depth=case%6+1
        scalars={'_stack_size_':depth,'_max_stack_size_':6,'_nj_matrix_flag_':case%2}
        pointers={'_nj_base_matrix_ptr_':(0x8cd00040,C.addressof(stack)+64),
                  '_nj_current_matrix_ptr_':(0x8cd00040+64*(depth-1),C.addressof(stack)+64+64*(depth-1))}
        for n,value in scalars.items():C.c_int32.in_dll(lib,n).value=value;store_word(table[n],value)
        for n,(guest,host) in pointers.items():P.in_dll(lib,n).value=host;store_word(table[n],guest)
        if name=='njInitView':unit_raw=rng.randbytes(64)
        else:unit_raw=canonical_unit if case%4==0 else words([finite() for i in range(16)])
        C.memmove(unit,unit_raw,64);store(table['_unit_matrix_'],unit_raw)
        active=rng.randbytes(64);current=lib.jp_ninja_current_matrix_storage();C.memmove(current,active,64)
        old_view_matrix=rng.randbytes(64);C.memmove(view_matrix,old_view_matrix,64);store(table['_nj_view_matrix_'],old_view_matrix)
        if where=='base':view_guest,view_host=0x8cd00040,C.addressof(stack)+64
        elif where=='saved':view_guest,view_host=0x8cd00080,C.addressof(stack)+128
        else:view_guest,view_host=0x8cd10084,C.addressof(view)+132
        view_raw=rng.randbytes(120)
        if name=='njClearMatrix' and case%4==0 and where=='external':
            integer_view=[rng.randrange(-4,5) for i in range(16)]
            view_raw=struct.pack('<16f',*integer_view)+view_raw[64:]
        store(view_guest,view_raw);C.memmove(view_host,view_raw,120)
        view_pointer.value=view_host;store_word(table['_nj_view_'],view_guest)
        check(view_host>0xffffffff,'native view pointer keeps all bits')
        f=Fpu();f.fpscr=0x40001|(bank<<21)
        f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)];f.banks[bank^1][:]=struct.unpack('<16I',active)
        s=State();s.r[15]=0x8cf00000;s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000
        if name=='njInitView':s.r[4]=view_guest;host_args=[view_host]
        elif name=='njScale':
            guest=0 if where=='current' else (0x8cd00080 if where=='saved' else 0x8cd20084)
            host=None if where=='current' else (C.addressof(stack)+128 if where=='saved' else C.addressof(arena)+132)
            matrix_raw=words([finite() for i in range(16)])
            special=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff]
            scale_words=[finite() for i in range(3)]
            if case<100:
                matrix_raw=words([special[(case+i)%len(special)] for i in range(16)])
                scale_words=[special[(case//10+i)%len(special)] for i in range(3)]
            if host is None:
                C.memmove(current,matrix_raw,64);f.banks[bank^1][:]=struct.unpack('<16I',matrix_raw)
            else:C.memmove(host,matrix_raw,64);store(guest,matrix_raw)
            f.banks[bank][4:7]=scale_words;s.r[4]=guest
            host_args=[host,*struct.unpack('<3f',words(scale_words))]
        else:host_args=[];s.r[4]=rng.getrandbits(32)  # ignored original argument
        reference(ram,s,fpu=f,vector_instruction=service,instruction_budget=6000)
        getattr(lib,name)(*host_args)
        check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),'original restores stack/bank/width/mode')
        for guest,native in ranges:check(bytes(native)==read(guest,512),('complete arena and guards',name,where,case,hex(guest)))
        check(bytes(unit)==read(table['_unit_matrix_'],64),'unit matrix retained')
        check(bytes(view_matrix)==read(table['_nj_view_matrix_'],64),('view matrix equals original',name,where,case))
        check(view_pointer.value==view_host,'current view pointer retained, full native width')
        for n in scalars:check(C.c_uint32.in_dll(lib,n).value==struct.unpack('<I',read(table[n],4))[0],('matrix metadata',n))
        for n,(guest,host) in pointers.items():
            expected=struct.unpack('<I',read(table[n],4))[0]-0x8cd00000+C.addressof(stack)
            check(P.in_dll(lib,n).value==expected,('translated native stack pointer',n))
        expected=words(list(f.banks[bank^1]))
        for n in ['njGetMatrix','jp_shz_store','jp_shz_store_from_other_unit']:
            getattr(lib,n)(C.byref(out));check(bytes(out)==expected,('same active state through actual API',name,n,where,case))
        check(lib.jp_ninja_current_matrix_storage()==lib.shz_xmtrx_state_()==current,'one stable actual state provider')
        if name=='njInitView':
            expected_view=unit_raw+struct.pack('<14I',0,0,0,0,0,0xbf800000,0,0,0,0,0,0,0xbf800000,0)
            check(C.string_at(view_host,120)==expected_view,'independent default view fields and unit copy')
            independent+=1
        elif name=='njClearMatrix':
            check(C.c_int32.in_dll(lib,'_stack_size_').value==1,'clear resets depth')
            check(P.in_dll(lib,'_nj_current_matrix_ptr_').value==C.addressof(stack)+64,'clear resets saved-slot pointer')
            check(bytes(view_matrix)==C.string_at(C.addressof(stack)+64,64)==expected,'base, saved view and current agree after clear')
            if case%4==0 and where=='external':
                geometry=[(-value if i%4 in [0,1] else value) for i,value in enumerate(integer_view)]
                check(struct.unpack('<16f',bytes(view_matrix))==tuple(geometry),'independent row-vector reflection/view product')
                independent+=1
        calls[name]+=1
    for name,cases,locations in [('njInitView',128,['external','base','saved']),
                                 ('njScale',256,['external','current','saved']),
                                 ('njClearMatrix',128,['external','base','saved'])]:
        before=checks;old_calls=sum(calls.values())
        for bank in [0,1]:
            for case in range(cases):
                for where in locations:scenario(name,bank,where,case)
        sections[name]=dict(checks=checks-before,original_calls=sum(calls.values())-old_calls)
        print(name,sections[name],flush=True)
    report=dict(status='original_view_initialization_scale_and_complete_clear_stack_contracts_verified',checks=checks,sections=sections,
        original_instruction_contract_comparisons=sum(calls.values()),per_function=calls,independent_layout_geometry_checks=independent,
        original_clear_dependency_bodies_executed=['njScale','njMultiMatrix'],original_dependency_hooks_used=False,
        full_native_view_pointer_width_tested=True,view_aliases_base_and_saved_slots_tested=True,single_active_matrix_owner=True,
        original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),DLL_sha256=sha(dll),
        build_report_sha256=sha(d/'build_verification.json'),external_input_manifest_sha256=sha(pin_path),
        verifier_sha256=sha(Path(__file__)),executor_sha256=sha(p/'verify_startup.py'),numerical_policy='toward_zero_DN1_exceptions_disabled',
        limitations=['Original scalar scaling comparisons cover finite binary32 inputs, signed zero, DN operands and finite RZ saturation; complete nonfinite scalar propagation is not qualified here.',
                     'View initialization copies arbitrary word payloads. Clear tests use arbitrary view matrix words and finite unit matrices; vector numerics use the existing pinned model, not new hardware validation.',
                     'Clear requires a valid current view and initialized nonempty stack, as in the original. Null/current-view fallback and njSetView are not substituted.',
                     'Flags/traps, complete renderer arithmetic, full frames, BIOS handoff, performance, gameplay/localization/later ports remain unqualified.'],
        game_source_linked=False,retail_assets_published=False,overall_completion_percent=10,goal_paused=False,playable=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(sum(calls.values()),'original calls;',checks,'checks passed')

if __name__=='__main__':main()
