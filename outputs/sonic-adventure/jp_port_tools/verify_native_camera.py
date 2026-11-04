"""Execute complete original njSetView (all real dependencies) and translation.
Compare native camera/stack/error state, with independent pose geometry cases.
"""
import argparse,ctypes as C,hashlib,json,math,random,re,struct,sys
from pathlib import Path
from camera_reference_numeric import service_for

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    pin_path=p/'jp_port_tools/jp_camera_external_inputs.json';pin=json.loads(pin_path.read_text())
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    build=json.loads((d/'build_verification.json').read_text());assert build['exit_code']==build['diagnostic_bytes']==0
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(build['DLL']);assert sha(dll)==build['DLL_sha256']
    sys.path.insert(0,str(p))
    from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    lib=C.CDLL(str(dll));P=C.c_void_p;I=C.c_int32;U=C.c_uint32;F=C.c_float
    for n,ret,args in [('njInitMatrix',None,[P,I,I]),('njSetView',None,[P]),('njTranslate',None,[P,F,F,F]),
                       ('njGetMatrix',None,[P]),('jp_shz_store',None,[P]),('jp_shz_store_from_other_unit',None,[P]),
                       ('jp_ninja_current_matrix_storage',P,[]),('shz_xmtrx_state_',P,[])]:
        fn=getattr(lib,n);fn.restype=ret;fn.argtypes=args
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=C.c_int;numeric.argtypes=[U,C.POINTER(Fpu),U]
    service=service_for(numeric);rng=random.Random(0x53455456);checks=0;sections={};calls={'njSetView':0,'njTranslate':0};geometry=0
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;BASE=0x8c000000
    stack=(C.c_ubyte*512)();arena=(C.c_ubyte*512)();other=(C.c_ubyte*512)();out=(C.c_ubyte*64)()
    ranges=[(0x8cd00000,stack),(0x8cd10000,arena),(0x8cd20000,other)]
    table={n:int(v,16) for n,v in pin['original_globals'].items()}
    vp=P.in_dll(lib,'_nj_view_');vm=(U*16).in_dll(lib,'_nj_view_matrix_');unit=(U*16).in_dll(lib,'_unit_matrix_');error=I.in_dll(lib,'jp_sdk_math_errno')
    canonical=words([0x3f800000 if i%5==0 else 0 for i in range(16)])
    symbols={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    expected_dependency_names=['njSetView','njRotateX','njRotateY','njRotateZ','njTranslate','njClearMatrix','njScale','njMultiMatrix','acosf',
                               '_in_acoss','_in_asins','_in_atans','_app_atans','_in_fmods','_in_ldexps']
    addresses={symbols[n]:n for n in expected_dependency_names};seen=set()
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    def store(address,raw):ram[address-BASE:address-BASE+len(raw)]=raw
    def read(address,count):return bytes(ram[address-BASE:address-BASE+count])
    def trace(pc,op,registers,sr,pr,fp):
        if pc in addresses:seen.add(addresses[pc])
    def scenario(name,bank,where,case):
        nonlocal geometry
        lib.njInitMatrix(C.addressof(stack)+64,6,case%2)
        for guest,native in ranges:
            raw=rng.randbytes(512);C.memmove(native,raw,512);store(guest,raw)
        depth=case%6+1;metadata={'_stack_size_':depth,'_max_stack_size_':6,'_nj_matrix_flag_':case%2}
        pointers={'_nj_base_matrix_ptr_':(0x8cd00040,C.addressof(stack)+64),
                  '_nj_current_matrix_ptr_':(0x8cd00040+64*(depth-1),C.addressof(stack)+64+64*(depth-1))}
        for n,value in metadata.items():I.in_dll(lib,n).value=value;store(table[n],words([value]))
        for n,(guest,host) in pointers.items():P.in_dll(lib,n).value=host;store(table[n],words([guest]))
        # Stored matrix rotations' original contracts assume finite matrix state.
        active=struct.pack('<16f',*[rng.uniform(-4,4) for i in range(16)]) if name=='njSetView' else rng.randbytes(64)
        current=lib.jp_ninja_current_matrix_storage();C.memmove(current,active,64)
        unit_raw=canonical if case%4==0 else struct.pack('<16f',*[rng.randrange(-2,3)/2 for i in range(16)])
        C.memmove(unit,unit_raw,64);store(table['_unit_matrix_'],unit_raw)
        old_vm=rng.randbytes(64);C.memmove(vm,old_vm,64);store(table['_nj_view_matrix_'],old_vm)
        previous_guest,previous_host=0x8cd20084,C.addressof(other)+132
        vp.value=previous_host;store(table['_nj_view_'],words([previous_guest]))
        check(previous_host>0xffffffff,'full-width previous view pointer')
        err=rng.randrange(-1000,1000);error.value=err;store(int(pin['original_errno_address'],16),struct.pack('<i',err))
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)];f.banks[bank^1][:]=struct.unpack('<16I',active)
        s=State();s.r[15]=0x8cf00000;s.pc=int(pin['entry_points'][name],16);s.pr=0x8cfed000;s.sr=0x70000000
        if name=='njSetView':
            if where=='base':guest,host=0x8cd00040,C.addressof(stack)+64
            elif where=='saved':guest,host=0x8cd00080,C.addressof(stack)+128
            else:guest,host=0x8cd10084,C.addressof(arena)+132
            pose=bytearray(rng.randbytes(120));position=[rng.uniform(-100,100) for i in range(3)];direction=[rng.uniform(-4,4) for i in range(3)]
            if case<6:direction=[[0,0,-1],[0,0,1],[1,0,0],[-1,0,0],[0,1,0],[0,-1,0]][case]
            if 6<=case<24:
                scale=2.0**rng.randrange(-20,21);direction=[v*scale for v in direction]
            struct.pack_into('<6f',pose,64,*position,*direction)
            roll=[0,1,-1,0x4000,0x7fffffff,-0x80000000][case%6] if case<36 else rng.randrange(-0x80000000,0x80000000)
            struct.pack_into('<i',pose,88,roll)
            C.memmove(host,bytes(pose),120);store(guest,pose);s.r[4]=guest;host_args=[host]
        else:
            if where=='current':guest,host=0,None
            elif where=='saved':guest,host=0x8cd00080,C.addressof(stack)+128
            else:guest,host=0x8cd10084,C.addressof(arena)+132
            raw=rng.randbytes(64);parameters=[rng.getrandbits(32) for i in range(3)]
            if case<64:
                special=[0,0x80000000,1,0x80000001,0x00800000,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f800001]
                raw=words([special[(case+i)%10] for i in range(16)]);parameters=[special[(case//2+i)%10] for i in range(3)]
            if host is None:C.memmove(current,raw,64);f.banks[bank^1][:]=struct.unpack('<16I',raw)
            else:C.memmove(host,raw,64);store(guest,raw)
            f.banks[bank][4:7]=parameters;s.r[4]=guest
            host_args=[host,*[F.from_buffer_copy(words([v])) for v in parameters]]
        reference(ram,s,fpu=f,vector_instruction=service,instruction_budget=10000,trace=trace)
        getattr(lib,name)(*host_args)
        check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),'original parent restores stack/bank/width/mode')
        for address,native in ranges:check(bytes(native)==read(address,512),('complete camera/stack arenas and guards',name,where,bank,case,hex(address)))
        check(bytes(unit)==unit_raw==read(table['_unit_matrix_'],64),'unit matrix unchanged')
        check(bytes(vm)==read(table['_nj_view_matrix_'],64),'saved view matrix matches original')
        expected_view=host if name=='njSetView' else previous_host
        check(vp.value==expected_view and vp.value>0xffffffff,'correct full-width current view binding')
        check(error.value==struct.unpack('<i',read(int(pin['original_errno_address'],16),4))[0],'SDK math error context matches original')
        for n in metadata:check(C.c_uint32.in_dll(lib,n).value==struct.unpack('<I',read(table[n],4))[0],('stack metadata',n))
        for n in pointers:
            expected=struct.unpack('<I',read(table[n],4))[0]-0x8cd00000+C.addressof(stack)
            check(P.in_dll(lib,n).value==expected,('full-width translated stack pointer',n))
        expected=words(list(f.banks[bank^1]))
        for n in ['njGetMatrix','jp_shz_store','jp_shz_store_from_other_unit']:
            getattr(lib,n)(C.byref(out));check(bytes(out)==expected,('single active state through actual API',n))
        check(lib.jp_ninja_current_matrix_storage()==lib.shz_xmtrx_state_()==current,'one stable active matrix owner')
        if name=='njSetView':
            check(C.c_int32.in_dll(lib,'_stack_size_').value==1,'camera clears deferred stack depth')
            check(C.string_at(host+64,56)==bytes(pose)[64:],'position/direction/roll/absolute fields retained')
            if case%4==0 and where=='external' and case>=24:
                values=struct.unpack('<16f',bytes(vm));pos=struct.unpack_from('<3f',pose,64);direction=struct.unpack_from('<3f',pose,76)
                mapped_pos=[sum(pos[i]*values[4*i+j] for i in range(3))+values[12+j] for j in range(3)]
                mapped_dir=[sum(direction[i]*values[4*i+j] for i in range(3)) for j in range(3)]
                check(max(map(abs,mapped_pos))<0.001,'independent camera-position maps to origin')
                check(max(abs(mapped_dir[0]),abs(mapped_dir[1]),abs(mapped_dir[2]-math.sqrt(sum(v*v for v in direction))))<0.003,'independent camera-forward maps to positive depth')
                geometry+=2
        calls[name]+=1
    for name,count,locations in [('njTranslate',192,['external','current','saved']),('njSetView',128,['external','base','saved'])]:
        before=checks
        for bank in [0,1]:
            for case in range(count):
                for where in locations:scenario(name,bank,where,case)
        sections[name]=dict(checks=checks-before,original_calls=calls[name]);print(name,sections[name],flush=True)
    check(set(expected_dependency_names)<=seen,('all actual original parent dependencies executed',seen))
    report=dict(status='complete_original_camera_setup_and_translation_state_contracts_verified',checks=checks,sections=sections,
        original_instruction_contract_comparisons=sum(calls.values()),per_function=calls,independent_camera_pose_checks=geometry,
        original_dependency_entries_executed=sorted(seen),original_dependency_hooks_used=False,original_binary_sha256=sha(a.binary),
        numerical_model_DLL_sha256=sha(a.numeric_dll),DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),
        external_input_manifest_sha256=sha(pin_path),verifier_sha256=sha(Path(__file__)),executor_sha256=sha(p/'verify_startup.py'),
        sqrt_reference_service_sha256=sha(Path(__file__).with_name('camera_reference_numeric.py')),SDK_errno_native_adapter='jp_sdk_math_errno',
        full_native_view_pointer_width_tested=True,view_aliases_base_and_saved_slots_tested=True,single_active_matrix_owner=True,
        numerical_policy='toward_zero_DN1_exceptions_disabled',
        limitations=['Camera parent comparisons use finite incoming matrix/unit/pose state and nonzero finite direction length over the sampled range. Vertical directions exercise original yaw domain errors; general nonfinite/zero-length camera propagation is not qualified.',
                     'Direct translation includes arbitrary operand words through the pinned vector model. No original dependency bodies are substituted; numeric services remain models, not new hardware measurements.',
                     'SDK math error data is adapted to a separate native global and must be bound for future SDK consumers; host CRT errno is not that game field.',
                     'Whole-game FPSCR policy, renderer arithmetic, full-frame/rasterization/resource lifetime, performance, gameplay, English and later ports remain unqualified.'],
        game_source_linked=False,retail_assets_published=False,overall_completion_percent=10,goal_paused=False,playable=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(sum(calls.values()),'original calls;',checks,'checks; complete camera dependency chain executed.')

if __name__=='__main__':main()
