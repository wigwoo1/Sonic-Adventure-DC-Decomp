"""Compare complete original njScaleV calls, aliasing and active matrix banks."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','numeric-dll','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes());pin=json.loads((p/'jp_port_tools/jp_view_stack_external_inputs.json').read_bytes());assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256'];assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    sys.path.insert(0,str(p));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)};raw=a.binary.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
    U,I,P=C.c_uint32,C.c_int32,C.c_void_p;lib=C.CDLL(b['frames_DLL']);scale=bind(lib,'njScaleV',None,P,P);init=bind(lib,'njInitMatrix',None,P,I,I);setmatrix=bind(lib,'njSetMatrix',None,P,P);current=bind(lib,'jp_ninja_current_matrix_storage',P)
    numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',I,U,P,U)
    def service(op,f):assert numeric(op,C.byref(f),f.fpul)==0
    arena=(C.c_uint8*512)();stack=(C.c_uint8*512)();rng=random.Random(0x53434156);checks=calls=0;special=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff]
    def finite():return (rng.getrandbits(1)<<31)|(rng.randrange(255)<<23)|rng.getrandbits(23)
    def words(v):return struct.pack('<'+'I'*len(v),*v)
    def check(ok,label):
        nonlocal checks
        assert ok,(calls,label);checks+=1
    for bank in range(2):
        for location in ('current','external','alias0','alias4','alias48'):
            for case in range(128):
                init(C.addressof(stack)+64,6,case%2);initial=rng.randbytes(512);C.memmove(arena,initial,512);ram[0xd00000:0xd00200]=initial
                matrix=words([special[(case+i)%len(special)] if case<56 else finite() for i in range(16)])
                source=words([special[(case//4+i)%len(special)] if case<56 else finite() for i in range(3)])
                active=words([rng.getrandbits(32) for i in range(16)]);setmatrix(None,C.c_char_p(active));target=0 if location=='current' else 0x8cd00080
                if target:C.memmove(C.addressof(arena)+128,matrix,64);ram[0xd00080:0xd000c0]=matrix
                else:setmatrix(None,C.c_char_p(matrix));active=matrix
                offset=128+int(location[5:]) if location.startswith('alias') else 292
                if not location.startswith('alias'):C.memmove(C.addressof(arena)+offset,source,12);ram[0xd00000+offset:0xd00000+offset+12]=source
                f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for i in range(16)];f.banks[bank^1][:]=struct.unpack('<16I',active)
                s=State();s.pc=table['njScaleV'];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.r[4]=target;s.r[5]=0x8cd00000+offset;s.sr=0x70000000
                metadata={n:bind(lib,'jp_ninja_current_matrix_storage',P)() if n=='active' else P.in_dll(lib,n).value for n in ('active','_nj_current_matrix_ptr_','_nj_base_matrix_ptr_')}
                stack_before=bytes(stack);reference(ram,s,fpu=f,vector_instruction=service,instruction_budget=4000);scale(None if not target else C.addressof(arena)+128,C.addressof(arena)+offset);calls+=1
                check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),'original call restores stack,bank,width and arithmetic mode')
                check(bytes(arena)==bytes(ram[0xd00000:0xd00200]),'all512 arena bytes including source alias and guards match original machine call')
                check(C.string_at(current(),64)==words(list(f.banks[bank^1])),'same complete active matrix matches actual original bank contents')
                check(bytes(stack)==stack_before,'deferred saved stack bytes remain intact')
                check(P.in_dll(lib,'_nj_current_matrix_ptr_').value==metadata['_nj_current_matrix_ptr_'] and P.in_dll(lib,'_nj_base_matrix_ptr_').value==metadata['_nj_base_matrix_ptr_'] and current()==metadata['active'],'one unchanged matrix owner and full-width stack pointers')
    d.mkdir();out=dict(status='complete_original_SH4_njScaleV_finite_active_external_aliasing_denormal_boundaries_and_both_banks_verified',checks=checks,original_entry_calls=calls,bank_cases=2,locations=['current','external','alias0','alias4','alias48'],whole_arena_bytes_compared_per_call=512,original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),nonfinite_inputs_not_supported_by_frozen_scalar_instruction_executor=True,invalid_null_vector_outside_contract=True,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks',calls,'original calls',flush=True)
if __name__=='__main__':main()
