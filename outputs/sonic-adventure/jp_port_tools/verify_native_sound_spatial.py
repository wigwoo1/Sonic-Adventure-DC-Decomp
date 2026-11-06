"""Unhooked original spatial calls, shared-matrix identity and RZ value edges."""
import argparse,ctypes as C,hashlib,json,math,random,re,struct,subprocess,sys
from pathlib import Path
from model_reference import make_reference
from primitive_reference import exceptional_binary
from camera_reference_numeric import service_for
from verify_native_audio import bind
I=C.c_int32;U=C.c_uint32;P=C.c_void_p;Q=C.c_uint64

def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def words(values):return struct.pack('<'+'I'*len(values),*values)
def floats(values):return struct.pack('<'+'f'*len(values),*values)

def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['spatial_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    for key in ('game_sound_DLL','frames_DLL'):assert sha(b[key])==b[key+'_sha256']
    assert sha(a.build/'external_game_sound.c')==b['spatial_generated_source_sha256']
    probe_source=p/'jp_port_tools/sound_spatial_value_probe.c';probe_dll=d/'spatial_value_probe.dll'
    compiler=b['commands'][0]['command'][0]
    cmd=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-shared','-fuse-ld=lld',str(probe_source),str(Path(b['game_sound_DLL']).with_suffix('.lib')),'-o',str(probe_dll)]
    run=subprocess.run(cmd,capture_output=True);(d/'probe_build.log').write_bytes(run.stdout+run.stderr)
    assert not run.returncode and not (run.stdout+run.stderr),(run.returncode,(run.stdout+run.stderr).decode())
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import execute_add_sub,execute_multiply,truncate_i32
    ref,reference_hashes=make_reference(executor)
    numeric_path=r/'work/sonic-adventure/build/sh4_vector_numeric.dll'
    numeric_pin=read(p/'jp_port_tools/jp_multi_matrix_external_inputs.json')
    assert sha(numeric_path)==numeric_pin['numerical_model_DLL_sha256']
    numeric=bind(C.CDLL(str(numeric_path)),'sa_sh4_vector_numeric',I,U,C.POINTER(Fpu),U)
    service=service_for(numeric)
    frame=C.CDLL(b['frames_DLL']);lib=C.CDLL(b['game_sound_DLL']);probe=C.CDLL(str(probe_dll))
    point=bind(lib,'matrix_calc_point',None,P);unit=bind(probe,'spatial_probe_unit_bits',U,P)
    side=bind(probe,'spatial_probe_side_bits',U,U);channel=bind(probe,'spatial_probe_channel_bits',I,U)
    setpos=bind(lib,'peer_qsound_position',I,U,P);readpos=bind(lib,'peer_qsound_position_readback',I,U,P);place=bind(lib,'peer_qsound_place',I,U,P)
    current=bind(frame,'jp_ninja_current_matrix_storage',P)();shared=bind(frame,'shz_xmtrx_state_',P)
    init=bind(frame,'njInitMatrix',None,P,I,I);push=bind(frame,'njPushMatrix',I,P);getmatrix=bind(frame,'njGetMatrix',None,P)
    rng=random.Random(0x51535041);checks=0;sections={};calls={};nested={};labels=[]
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    addresses=(Q*3)();check(bind(lib,'peer_spatial_import_addresses',I,P)(addresses)==0,'real import readback')
    expected=[C.cast(getattr(frame,n),P).value for n in ('njCalcPoint','jp_rz_scalar32_add','jp_rz_scalar32_multiply')]
    check(list(addresses)==expected and current>0xffffffff and shared()==current,'actual single existing frame math owner, high native pointers')
    stack=(C.c_ubyte*1024)();arena=(C.c_ubyte*512)();readback=(C.c_ubyte*64)()
    symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    tracked={symbols[n]:n for n in ('matrix_calc_point','njUnitVector','qsound_place')}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw
    def put(address,data):offset=address&0xffffff;ram[offset:offset+len(data)]=data
    def guestbytes(address,size):offset=address&0xffffff;return bytes(ram[offset:offset+size])
    def trace(pc,*unused):
        if pc in tracked:n=tracked[pc];nested[n]=nested.get(n,0)+1
    def original(name,bank,active,arg):
        cpu=executor.State();cpu.pc=symbols[name];cpu.pr=0x8cfed000;cpu.sr=0x70000000|((sum(calls.values())%16)<<4);cpu.r[15]=0x8cf00000
        for i in range(8,15):cpu.r[i]=0x12340000+i
        saved=list(cpu.r[8:]);mask=cpu.sr&0xf0;cpu.r[4]=arg
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][:]=[rng.getrandbits(32) for _ in range(16)];f.banks[bank^1][:]=struct.unpack('<16I',active)
        ref(ram,cpu,fpu=f,trace=trace,vector_instruction=service,instruction_budget=5000)
        check(cpu.pc==0x8cfed000 and list(cpu.r[8:])==saved and cpu.sr&0xf0==mask,'original complete call preserves stack, saved integer registers and interrupt mask')
        check(f.fpscr&0x3c0003==0x40001|(bank<<21),'original numerical policy and register bank preserved')
        calls[name]=calls.get(name,0)+1
        return cpu,f
    identity=floats([int(i%5==0) for i in range(16)])
    def state(active,case):
        init(C.addressof(stack)+64,12,case%2)
        for _ in range(case%5):check(push(None)==1,'valid real deferred matrix stack')
        saved=rng.randbytes(1024);C.memmove(stack,saved,1024);C.memmove(current,active,64)
        metadata={n:I.in_dll(frame,n).value for n in ('_stack_size_','_max_stack_size_','_nj_matrix_flag_')}
        pointers={n:P.in_dll(frame,n).value for n in ('_nj_current_matrix_ptr_','_nj_base_matrix_ptr_')}
        return saved,metadata,pointers
    def unchanged(active,saved,metadata,pointers,f):
        check(C.string_at(current,64)==words(f.banks[(f.fpscr>>21)^1])==active,'same active matrix remains unchanged')
        getmatrix(readback);check(bytes(readback)==active and current==shared(),'ordinary Ninja API sees same actual matrix')
        check(bytes(stack)==saved,'all deferred matrix and guard bytes preserved')
        check(all(I.in_dll(frame,n).value==v for n,v in metadata.items()) and all(P.in_dll(frame,n).value==v for n,v in pointers.items()),'matrix owner metadata and native pointers preserved')
    edge=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fbfffff,0x7fc12345]
    vectors=[words([v,0,0]) for v in edge]+[words([0,v,0]) for v in edge]+[words([0,0,v]) for v in edge]
    vectors += [floats(v) for v in ([0,0,0],[1,2,3],[-3,4,-5],[1,1,1],[1e-20,1e-20,1e-20],[1e20,-1e20,1e20])]
    vectors += [rng.randbytes(12) for _ in range(384)]+[floats([rng.uniform(-1000,1000) for _ in range(3)]) for _ in range(384)]
    start=checks
    for case,vector in enumerate(vectors):
        for bank in range(2):
            active=rng.randbytes(64);saved,metadata,pointers=state(active,case)
            block=bytearray(rng.randbytes(512));offset=64+4*(case%16);block[offset:offset+12]=vector
            C.memmove(arena,bytes(block),512);put(0x8cd10000,block)
            cpu,f=original('njUnitVector',bank,active,0x8cd10000+offset)
            result=unit(C.addressof(arena)+offset)
            check(result==f.banks[bank][0],('unit length return exact bits',case,bank,hex(result),hex(f.banks[bank][0])))
            check(bytes(arena)==guestbytes(0x8cd10000,512),('unit vector full memory/guards match',case,bank))
            unchanged(active,saved,metadata,pointers,f)
    sections['unit_vector_classes_random_values_and_return_bits']=checks-start;print('unit_vector',checks-start,'checks',flush=True)
    start=checks
    for case in range(512):
        active=rng.randbytes(64) if case%4==0 else floats([rng.uniform(-100,100) for _ in range(16)])
        vector=vectors[case%len(vectors)] if case%3==0 else rng.randbytes(12)
        for bank in range(2):
            saved,metadata,pointers=state(active,case);block=bytearray(rng.randbytes(512));offset=64+4*(case%16);block[offset:offset+12]=vector
            C.memmove(arena,bytes(block),512);put(0x8cd10000,block)
            cpu,f=original('matrix_calc_point',bank,active,0x8cd10000+offset);point(C.addressof(arena)+offset)
            check(bytes(arena)==guestbytes(0x8cd10000,512),('complete point transform matches original and guards',case,bank))
            unchanged(active,saved,metadata,pointers,f)
    sections['matrix_calc_point_live_state_classes_and_translation']=checks-start;print('matrix_calc_point',checks-start,'checks',flush=True)
    start=checks
    positions=[floats(v) for v in ([1,0,0],[-1,0,0],[0,0,1],[0,0,-1],[0,0,0],[0,20,0])]+vectors[:48]
    for boundary in range(1,31):
        x=boundary/15.5-1;z=math.sqrt(max(0,1-x*x));xb=struct.unpack('<I',floats([x]))[0]
        for delta in (-2,-1,0,1,2):positions.append(words([(xb+delta)&0xffffffff,0,struct.unpack('<I',floats([z]))[0]]))
    positions += [floats([rng.uniform(-1000,1000) for _ in range(3)]) for _ in range(512)]+[rng.randbytes(12) for _ in range(256)]
    for case,position in enumerate(positions):
        active=identity if case<204 or case%4==0 else floats([rng.uniform(-4,4) for _ in range(16)])
        for bank in range(2):
            saved,metadata,pointers=state(active,case);slot=case%38
            fixture=C.create_string_buffer(position);check(setpos(slot,fixture)==0,'explicit input into actual shared source slot')
            put(symbols['sound_slots']+slot*52+36,position)
            cpu,f=original('qsound_place',bank,active,slot);out=I(0x12345678)
            check(place(slot,C.byref(out))==0 and out.value==I(cpu.r[0]).value,('complete original placement with real nested point/normalization',case,bank,out.value,I(cpu.r[0]).value))
            check(readpos(slot,readback)==0 and bytes(readback)[:12]==position and guestbytes(symbols['sound_slots']+slot*52+36,12)==position,'both original and native game positions remain unchanged')
            unchanged(active,saved,metadata,pointers,f)
    sections['original_qsound_place_boundaries_slots_and_nested_dependencies']=checks-start;print('qsound_place',checks-start,'checks',flush=True)
    start=checks
    for value in edge+[rng.getrandbits(32) for _ in range(4096)]:
        expected=I((truncate_i32(value)[0]<<2)&0xffffffff).value
        check(channel(value)==expected,'defined original FTRC/SHLL2 values including nonfinite and overflow')
        f=Fpu();f.fpscr=0x40001;f.banks[0][0]=value;f.banks[0][1]=0x3f800000
        if value&0x7fffffff>=0x7f800000:exceptional_binary(f,0,1,'add')
        else:execute_add_sub(f,0,1)
        f.banks[0][1]=0x41780000
        if f.banks[0][0]&0x7fffffff>=0x7f800000:exceptional_binary(f,0,1,'multiply')
        else:execute_multiply(f,0,1)
        check(side(value)==f.banks[0][0],'RZ add/multiply side value independent dyadic/class oracle')
    sections['independent_side_FTRC_and_defined_shift']=checks-start
    start=checks
    for _ in range(64):
        matrix=[rng.randrange(-8,9) for _ in range(16)];vector=[rng.randrange(-8,9) for _ in range(3)]
        C.memmove(current,floats(matrix),64);buf=C.create_string_buffer(floats(vector));point(buf)
        exact=[sum(matrix[4*i+j]*vector[i] for i in range(3))+matrix[12+j] for j in range(3)]
        check(buf.raw[:12]==floats(exact),'independent exact integer geometry includes translation and matrix orientation')
        check(C.string_at(current,64)==floats(matrix),'exact geometry preserves live matrix')
    for axis in range(3):
        for sign in (-1,1):
            for length in (1,2,4,8,16):
                vector=[0,0,0];vector[axis]=sign*length;buf=C.create_string_buffer(floats(vector));got=unit(buf)
                expected=[0,0,0];expected[axis]=sign
                check(buf.raw[:12]==floats(expected) and words([got])==floats([length]),'independent exact axis normalization and former length')
    C.memmove(current,identity,64)
    for position,expected in (([1,0,0],124),([-1,0,0],0),([0,0,1],60),([0,0,-1],60)):
        buf=C.create_string_buffer(floats(position));out=I()
        check(setpos(0,buf)==0 and place(0,C.byref(out))==0 and out.value==expected,'independent cardinal QSound placement')
    sections['independent_integer_geometry_axis_lengths_and_cardinal_placement']=checks-start
    k=C.WinDLL('kernel32',use_last_error=True);alloc=bind(k,'VirtualAlloc',P,P,C.c_size_t,U,U);free=bind(k,'VirtualFree',I,P,C.c_size_t,U)
    region=alloc(None,8192,0x2000,1);check(bool(region),'guarded reservation')
    try:
        check(alloc(region,4096,0x1000,4)==region,'first page committed; next page inaccessible')
        pointer=region+4096-12;C.memmove(current,identity,64)
        C.memmove(pointer,floats([1,2,3]),12);point(pointer);check(C.string_at(pointer,12)==floats([1,2,3]),'point touches exactly three floats at guard boundary')
        C.memmove(pointer,floats([1,0,0]),12);result=unit(pointer);check(result==0x3f800000 and C.string_at(pointer,12)==floats([1,0,0]),'unit vector touches exactly three floats at guard boundary')
    finally:check(free(region,0,0x8000)==1,'guarded pages released')
    sentinel=I(0x12345678);check(place(38,C.byref(sentinel))==1 and sentinel.value==0x12345678 and setpos(38,readback)==1 and readpos(38,readback)==1,'fixture slot boundary rejects without mutation')
    check(nested['qsound_place']==calls['qsound_place'] and nested['matrix_calc_point']==calls['matrix_calc_point']+calls['qsound_place'] and nested['njUnitVector']==calls['njUnitVector']+calls['qsound_place'],'actual original nested routines executed without hooks')
    result=dict(status='original_spatial_point_unit_vector_and_QSound_place_shared_matrix_RZ_values_verified',runtime_checks=checks,sections=sections,
        complete_original_entry_calls=sum(calls.values()),original_entry_calls=calls,original_nested_entries=nested,original_function_hooks=False,
        actual_single_existing_frame_matrix_and_scalar_imports=True,guard_page_exact_vector_bytes=12,numerical_policy=b['spatial_numeric_policy'],
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),reference_hashes=reference_hashes,
        probe_source_sha256=sha(probe_source),probe_DLL_sha256=sha(probe_dll),probe_build_command=cmd,
        pinned_vector_instruction_model_DLL_sha256=sha(numeric_path),
        vector_instruction_service_sha256=sha(Path(__file__).parent/'camera_reference_numeric.py'),
        vector_approximation_model_is_shared_with_native=True,
        FPSCR_side_effects_hardware_measurements_or_full_game_spatial_acoustics_verified=False,
        full_game_request_queue_streams_or_main_playable=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks;',sum(calls.values()),'complete original calls',flush=True)

if __name__=='__main__':main()
