"""Whole Japanese PRNG/shuffle execution and independent sequence/permutation.
Fixed RZ/DN1 value contract, not FPSCR emulation or physical-hardware proof.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
U=C.c_uint32;I=C.c_int32;P=C.c_void_p
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists();b=json.loads((d/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['game_random_external_files_sha256'].items():assert sha(a.upstream/n)==h,n
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.upstream/'config/1st_read/symbols.txt')==pin['files_sha256']['config/1st_read/symbols.txt']
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'];lib=C.CDLL(b['frames_DLL']);seed=bind(lib,'srand',None,U);draw=bind(lib,'rand',I);shuffle=bind(lib,'array_shuffle',None,P,I,I);pick=bind(lib,'jp_random_pick',I,U,I);float_word=bind(lib,'jp_random_positive_float_word',U,U);status=bind(lib,'jp_shuffle_status',I);state=U.in_dll(lib,'_rnext')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import round_dyadic,multiply,truncate_i32
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;ARRAY=0x8cd00000;calls=checks=0;seen=set();groups={};rng=random.Random(0x524e4721);traced_calls={n:0 for n in ['rand','srand','array_shuffle','memory_swap']};entries={table[n]:n for n in traced_calls}
    def put(addr,data):at=addr&0xffffff;ram[at:at+len(data)]=data
    def read(addr,n):at=addr&0xffffff;return bytes(ram[at:at+n])
    def guest_state():return struct.unpack('<I',read(table['_rnext'],4))[0]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def trace(pc,*rest):
        if pc in entries:seen.add(entries[pc]);traced_calls[entries[pc]]+=1
    def original(name,*args):
        nonlocal calls
        f=Fpu();f.fpscr=0x40001|((calls%2)<<21);s=State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
        reference(ram,s,fpu=f,trace=trace,instruction_budget=6000000);calls+=1;check(s.r[15]==0x8cf00000,'whole original stack restored');return C.c_int32(s.r[0]).value
    def advance(value):return (value*1103515245+12345)&0xffffffff
    check(state.value==guest_state()==0,'actual original/native initial game PRNG state is zero')
    seeds=[0,1,0xffffffff,0x80000000,0x7fffffff,0xdead0cab,12345]+[rng.getrandbits(32) for _ in range(257)]
    for initial in seeds:
        seed(initial);original('srand',initial);check(state.value==guest_state()==initial,'genuine seed preserves all 32 bits');expected=initial
        for _ in range(64):
            expected=advance(expected);want=(expected>>16)&32767;check(draw()==original('rand')==want and state.value==guest_state()==expected,'whole original/native output and exact state match independent modulo-32-bit LCG')
    groups['whole_original_seed_and_stream_contract']=checks
    # Independent generic integer dyadic rounding, shared with the frozen
    # instruction oracle rather than the new C bit-scan conversion helper.
    integers=list(range(65536))+[0xffffffff,0x80000000,0x7fffffff]+[max(0,min(0xffffffff,(1<<power)+offset)) for power in range(32) for offset in [-2,-1,0,1,2]]+[rng.getrandbits(32) for _ in range(2048)]
    for value in integers:check(float_word(value)==round_dyadic(value,0,1)[0],'independent unsigned integer -> RZ binary32 word')
    bounds=[2,3,7,16,31,257,4096,65535,16777217,2147483647]
    def oracle_pick(value,left):
        product=multiply(round_dyadic(value,0,1)[0],round_dyadic(left,0,1)[0],1)[0]
        return truncate_i32(multiply(product,0x38000000,1)[0])[0]
    for left in bounds:
        for value in range(32768):check(pick(value,left)==oracle_pick(value,left) and 0<=pick(value,left)<left,'entire 15-bit output domain across small/large/inexact-count boundaries')
    for value,left in [(32768,2),(0xffffffff,2),(1,0),(1,-1),(1,-2147483648)]:check(pick(value,left)==-1,'helper rejects out-of-contract arguments')
    groups['all_32768_draw_values_and_independent_RZ_pick_math']=checks-sum(groups.values())
    # Complete original array_shuffle and its real memory_swap/rand callees.
    # Guarded random byte arrays include zero-sized records, which still
    # consume count-1 random draws even though no memory bytes change.
    counts=[0,1,2,3,7,16,31,64,128,257];sizes=[0,1,2,3,4,7,16,32];shuffle_cases=0
    for case in range(160):
        count=counts[case%len(counts)];size=sizes[(case//len(counts))%len(sizes)];initial=seeds[case%len(seeds)];raw=rng.randbytes(64+count*size);arena=C.create_string_buffer(raw);put(ARRAY-32,raw);seed(initial);original('srand',initial)
        original('array_shuffle',ARRAY,count,size);shuffle(C.addressof(arena)+32,count,size);expected=bytearray(raw);value=initial
        for left in range(count,1,-1):
            value=advance(value);chosen=oracle_pick((value>>16)&32767,left);last=left-1
            a0=32+chosen*size;b0=32+last*size;one=expected[a0:a0+size];expected[a0:a0+size]=expected[b0:b0+size];expected[b0:b0+size]=one
        check(arena.raw[:-1]==read(ARRAY-32,len(raw))==expected,'complete shuffled records/guards equal original and independent permutation')
        check(state.value==guest_state()==value and status()==0,'exact RNG draw consumption, including zero record bytes');shuffle_cases+=1
    groups['genuine_whole_game_shuffle_memory_and_draw_consumption']=checks-sum(groups.values())
    for pointer,count,size in [(None,-1,1),(None,2,1),(None,2,-1),(None,2147483647,2),(0xfffffffffffffff0,2,16)]:
        before=state.value;shuffle(pointer,count,size);check(status()==1 and state.value==before,'invalid ranges reject before memory access or RNG consumption')
    for count in [0,1]:before=state.value;shuffle(None,count,0);check(status()==0 and state.value==before,'null empty/single zero-byte input is a no-op')
    for name in ['ucrtbase.dll','msvcrt.dll']:
        crt=C.CDLL(name);cs=bind(crt,'srand',None,U);cr=bind(crt,'rand',I);cs(1234567);expected=[cr() for _ in range(32)];cs(1234567);check([cr() for _ in range(4)]==expected[:4],'host CRT fixture reseed')
        seed(0xdead0cab);value=state.value;cs(7654321);check(state.value==value,'host CRT seed cannot mutate game state');cs(1234567);[cr() for _ in range(4)]
        for _ in range(128):value=advance(value);check(draw()==((value>>16)&32767) and state.value==value,'game stream remains independent of host CRT')
        check([cr() for _ in range(28)]==expected[4:],'game srand/rand cannot mutate host CRT state')
        check(C.cast(draw,P).value!=C.cast(cr,P).value,'distinct game/CRT function entry addresses')
    check(set(traced_calls)<=seen,'all genuine PRNG/shuffle/memory-swap dependencies executed without hooks')
    imports=subprocess.check_output([str(Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')),'--coff-imports',b['frames_DLL']],text=True)
    check(not re.search(r'Symbol:\s+(?:rand|srand)\s*\(',imports),'actual provider DLL imports no host rand/srand')
    groups['native_invalid_input_and_two_CRT_stream_separation']=checks-sum(groups.values())
    report=dict(status='whole_original_game_PRNG_and_complete_shuffle_verified',checks=checks,original_entry_calls=calls,original_dependency_calls=traced_calls,original_function_hooks_used=False,seed_cases=len(seeds),draws_per_seed=64,independent_unsigned_float_word_cases=len(integers),exhaustive_15bit_pick_bounds=bounds,exhaustive_pick_cases=32768*len(bounds),complete_shuffle_cases=shuffle_cases,zero_sized_record_draw_consumption_verified=True,complete_record_guards_and_permutation_verified=True,host_CRT_libraries_kept_independent=['ucrtbase.dll','msvcrt.dll'],host_rand_srand_imports_in_provider=False,original_binary_sha256=sha(a.binary),original_symbols_sha256=sha(a.upstream/'config/1st_read/symbols.txt'),original_reference_sha256=sha(r/'outputs/sonic-adventure/verify_startup.py'),independent_scalar_oracle_sha256=sha(r/'outputs/sonic-adventure/sh4_binary32.py'),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(__file__),groups=groups,fixed_single_RZ_DN1_value_policy=True,FPSCR_flags_traps_or_new_physical_hardware_verified=False,full_display_reset_system_boot_main_loop_gameplay_verified=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
