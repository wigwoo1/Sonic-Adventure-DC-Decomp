"""Compare authored camera acos to the full actual original runtime call chain."""
import argparse,ctypes as C,hashlib,json,math,random,struct,sys
from pathlib import Path
from camera_reference_numeric import service_for,sqrt_word

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    build=json.loads((d/'build_verification.json').read_text());assert build['exit_code']==build['diagnostic_bytes']==0
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    pin_path=p/'jp_port_tools/jp_camera_external_inputs.json';pin=json.loads(pin_path.read_text())
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    dll=Path(build['DLL']);assert sha(dll)==build['DLL_sha256']
    sys.path.insert(0,str(p))
    from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    lib=C.CDLL(str(dll));U=C.c_uint32
    for n in ['jp_rz_camera_sqrt','jp_rz_camera_acos']:
        fn=getattr(lib,n);fn.restype=U;fn.argtypes=[U]
    error=C.c_int32.in_dll(lib,'jp_sdk_math_errno')
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=C.c_int;numeric.argtypes=[U,C.POINTER(Fpu),U]
    service=service_for(numeric);rng=random.Random(0x43414d41);checks=0;sections={}
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    sqrt_cases=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0x40000000,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345]
    sqrt_cases += [rng.getrandbits(32) for i in range(2048)]
    for raw in sqrt_cases:
        expected=sqrt_word(raw);result=lib.jp_rz_camera_sqrt(raw);check(result==expected,('independent integer square root',hex(raw),hex(result),hex(expected)))
        if 0x800000<=raw<0x7f800000:
            # Exact rational squaring brackets the returned binary32 result.
            from fractions import Fraction
            def value(word):
                sig=(word&0x7fffff)|0x800000;e=((word>>23)&255)-150
                return Fraction(sig*(1<<e)) if e>=0 else Fraction(sig,1<<-e)
            check(value(result)**2<=value(raw)<value(result+1)**2,'independent exact square bracket')
    sections['sqrt_values_and_exact_rational_brackets']=checks
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    samples=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f000000,0xbf000000,0x3f7fffff,0xbf7fffff,0x3f800000,0xbf800000,
             0x3f800001,0xbf800001,0x7f800000,0xff800000,0x7f800001,0x7fc12345,0xff800001,0xffc54321]
    for i in range(1024):samples.append((rng.getrandbits(1)<<31)|(rng.randrange(128)<<23)|rng.getrandbits(23))
    samples += [rng.getrandbits(32) for i in range(256)]
    # Dense endpoints and rational grid exercise every asin/atan reduction arm.
    samples += [struct.unpack('<I',struct.pack('<f',i/256))[0] for i in range(-256,257)]
    before=checks;calls=0;independent=0;entries=set();arithmetic=set()
    entry_addresses={int(v,16):n for n,v in pin['entry_points'].items() if n!='njSetView' and n!='njTranslate'}
    for bank in [0,1]:
        for raw in samples:
            initial_error=rng.randrange(-1000,1000);error.value=initial_error;struct.pack_into('<i',ram,int(pin['original_errno_address'],16)-0x8c000000,initial_error)
            f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][4]=raw
            s=State();s.pc=int(pin['entry_points']['acosf'],16);s.pr=0x8cfed000;s.r[15]=0x8cf00000
            def trace(pc,op,registers,sr,pr,fp):
                if pc in entry_addresses:entries.add(entry_addresses[pc])
                if op&0xf00f in [0xf000,0xf001,0xf002,0xf003,0xf00e]:arithmetic.add(op&0xf00f)
            reference(ram,s,fpu=f,vector_instruction=service,instruction_budget=5000,trace=trace)
            result=lib.jp_rz_camera_acos(raw)
            check(result==f.banks[bank][0],('original full acos chain',hex(raw),bank,hex(result),hex(f.banks[bank][0])))
            expected_error=struct.unpack_from('<i',ram,int(pin['original_errno_address'],16)-0x8c000000)[0]
            check(error.value==expected_error,'SDK error state adapter matches original')
            check(s.r[15]==0x8cf00000 and f.fpscr&0x3c0003==(0x40001|(bank<<21)),'original acos returns with stack/mode restored')
            if raw&0x7fffffff<=0x3f800000:
                x=struct.unpack('<f',struct.pack('<I',raw))[0];y=struct.unpack('<f',struct.pack('<I',result))[0]
                check(abs(y-math.acos(x))<0.00001,'sampled independent real acos approximation')
                independent+=1
            calls+=1
    check(set(entry_addresses.values())<=entries,('all actual runtime dependencies executed',entries))
    check(0xf00e in arithmetic,'actual fused atan polynomial executed')
    sections['full_original_acos_dependency_chain']=checks-before
    report=dict(status='camera_acos_full_original_runtime_chain_and_integer_sqrt_values_verified',checks=checks,sections=sections,
        original_acos_calls=calls,original_runtime_dependency_entries_executed=sorted(entries),original_dependency_hooks_used=False,
        independent_sampled_acos_checks=independent,sqrt_word_cases=len(sqrt_cases),SDK_errno_native_adapter='jp_sdk_math_errno',
        original_binary_sha256=sha(a.binary),numerical_model_DLL_sha256=sha(a.numeric_dll),DLL_sha256=sha(dll),
        build_report_sha256=sha(d/'build_verification.json'),external_input_manifest_sha256=sha(pin_path),verifier_sha256=sha(Path(__file__)),
        sqrt_reference_service_sha256=sha(Path(__file__).with_name('camera_reference_numeric.py')),numerical_policy='toward_zero_DN1_exceptions_disabled',
        limitations=['Original runtime comparisons execute actual acosf, acoss, asins, atans, polynomial, power scaling and remainder bodies; instruction numerical services are model-based, not new hardware measurements.',
                     'Independent sqrt oracle and rational brackets qualify value results. Full FPSCR flags/traps and all floating environments are outside this native boundary.',
                     'Native SDK math error state deliberately uses a distinct global from the host CRT errno function; SDK consumers require that documented binding.',
                     'Camera/frame execution, renderer arithmetic, performance/gameplay/localization and later ports require separate qualification.'],
        game_source_linked=False,retail_assets_published=False,overall_completion_percent=10,goal_paused=False,playable=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(calls,'full original acos calls;',checks,'checks;',sorted(entries))

if __name__=='__main__':main()
