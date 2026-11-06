"""Actual combined rotations vs JP machine code and raw-word numeric ABI."""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','numeric-dll','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes());pin=json.loads((p/'jp_port_tools/jp_view_stack_external_inputs.json').read_bytes());assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    sys.path.insert(0,str(p));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)};raw=a.binary.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
    U,I,P=C.c_uint32,C.c_int32,C.c_void_p;lib=C.CDLL(b['frames_DLL']);init=bind(lib,'njInitMatrix',None,P,I,I);setmatrix=bind(lib,'njSetMatrix',None,P,P);current=bind(lib,'jp_ninja_current_matrix_storage',P);numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',I,U,P,U)
    def service(op,f):assert numeric(op,C.byref(f),f.fpul)==0
    checks=calls=0;rng=random.Random(0x41435452);arena=(C.c_uint8*512)();stack=(C.c_uint8*512)();special=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff]
    def finite():return (rng.getrandbits(1)<<31)|(rng.randrange(255)<<23)|rng.getrandbits(23)
    def words(v):return struct.pack('<'+'I'*len(v),*v)
    def check(ok,label):
        nonlocal checks
        assert ok,(calls,label);checks+=1
    for name in ('njRotateXYZ','njRotateZXY'):
        function=bind(lib,name,None,P,I,I,I)
        for bank in range(2):
            for location in ('current','external','saved'):
                for case in range(80):
                    init(C.addressof(stack)+64,6,case%2);initial=rng.randbytes(512);saved=rng.randbytes(512);C.memmove(arena,initial,512);C.memmove(stack,saved,512);ram[0xd00000:0xd00200]=initial;ram[0xd10000:0xd10200]=saved
                    matrix=words([special[(case+i)%len(special)] if case<40 else finite() for i in range(16)]);active=words([finite() for i in range(16)]);setmatrix(None,C.c_char_p(active));target=0 if location=='current' else 0x8cd00080 if location=='external' else 0x8cd10080
                    if not target:setmatrix(None,C.c_char_p(matrix));active=matrix;host=None
                    else:
                        host=C.addressof(arena)+128 if location=='external' else C.addressof(stack)+128;C.memmove(host,matrix,64);at=target&0xffffff;ram[at:at+64]=matrix
                    angles=[0 if not(case&(1<<i)) else [1,-1,0x4000,0x8000,0x10000,0x12345678,-0x80000000][(case+i)%7] for i in range(3)]
                    if case>=56:angles=[C.c_int32(rng.getrandbits(32)).value for _ in range(3)]
                    f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for _ in range(16)];f.banks[bank^1][:]=struct.unpack('<16I',active)
                    s=State();s.pc=table[name];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.r[4]=target;s.r[5:8]=[n&0xffffffff for n in angles];s.sr=0x70000000
                    pointers=[P.in_dll(lib,n).value for n in ('_nj_current_matrix_ptr_','_nj_base_matrix_ptr_')];state=[I.in_dll(lib,n).value for n in ('_stack_size_','_max_stack_size_','_nj_matrix_flag_')]
                    reference(ram,s,fpu=f,vector_instruction=service,instruction_budget=12000);function(host,*angles);calls+=1
                    check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),'original combined rotation restores machine stack/bank/precision mode')
                    check(bytes(arena)==bytes(ram[0xd00000:0xd00200]) and bytes(stack)==bytes(ram[0xd10000:0xd10200]),('all saved/external matrix arenas and guards match original',name,location,angles))
                    check(C.string_at(current(),64)==words(list(f.banks[bank^1])),('complete actual active matrix matches original',name,location,angles))
                    check(pointers==[P.in_dll(lib,n).value for n in ('_nj_current_matrix_ptr_','_nj_base_matrix_ptr_')] and state==[I.in_dll(lib,n).value for n in ('_stack_size_','_max_stack_size_','_nj_matrix_flag_')],'one unchanged actual matrix and deferred-stack owner')
    rotation_checks=checks;d.mkdir();source=p/'jp_port_tools/action_numeric_probe.c';probe=d/'action_numeric_probe.dll';compiler=b['commands'][0]['command'][0]
    cmd=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-shared','-fuse-ld=lld',str(source),str(Path(b['frames_DLL']).with_suffix('.lib')),'-o',str(probe)]
    run=subprocess.run(cmd,capture_output=True);(d/'probe_compile.txt').write_bytes(run.stdout+run.stderr);assert not run.returncode and not run.stdout and not run.stderr,run.stderr.decode(errors='replace')[:5000]
    observe=bind(C.CDLL(str(probe)),'jp_action_numeric_probe',I,U,P,P);boundary=special+[0x7f800000,0xff800000,0x7fbfffff,0x7fc12345,0x7f812345,0xffffffff];numerical_cases=0
    for bank in range(2):
        for operation in range(2):
            for case in range(384):
                values=[boundary[(case//len(boundary)+i+case)%len(boundary)] if case<128 else rng.getrandbits(32) for i in range(4)];f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for _ in range(16)]
                if operation:f.banks[bank][0]=values[0];opcode=0xf07d;slot=0
                else:f.banks[bank][4:8]=values;opcode=0xf5ed;slot=7
                assert numeric(opcode,C.byref(f),0)==0;expected=f.banks[bank][slot];out=U(0xabcdef);check(observe(operation,(U*4)(*values),C.byref(out))==0 and out.value==expected,('real Float ABI matches existing qualified numerical instruction service',operation,values,hex(out.value),hex(expected)));numerical_cases+=1
    out=U(0xabcdef);check(observe(2,(U*4)(),C.byref(out))==1 and out.value==0xabcdef and observe(0,None,C.byref(out))==1 and observe(0,(U*4)(),None)==1,'raw-word ABI observer rejects invalid diagnostics without fabricated math')
    result=dict(status='actual_original_combined_XYZ_ZXY_rotations_and_RZ_FIPR_FSRRA_Float_ABI_verified',checks=checks,rotation_checks=rotation_checks,original_rotation_entry_calls=calls,numerical_ABI_cases=numerical_cases,bank_cases=2,original_finite_rotation_boundary_scope=True,numeric_ABI_includes_NaN_infinity_and_denormals=True,Float_probe_source_sha256=sha(source),Float_probe_DLL_sha256=sha(probe),Float_probe_compile_command=cmd,original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),console_hardware_numeric_equivalence=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks;',calls,'original calls;',numerical_cases,'ABI cases',flush=True)
if __name__=='__main__':main()
