"""Complete original sound math instructions and cross-DLL genuine data imports."""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys
from pathlib import Path
from model_reference import make_reference
from camera_reference_numeric import service_for,sqrt_word
from verify_native_audio import bind
from verify_native_input_backend import Peripheral
from native_task_test_support_v2 import Work
U=C.c_uint32;I=C.c_int32;Q=C.c_uint64;P=C.c_void_p
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def words(v):return struct.pack('<'+'I'*len(v),*v)

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['sound_math_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    for n,h in b['sound_state_import_header_sha256'].items():assert sha(a.build/'sound_imports'/n)==h,n
    assert sha(a.build/'external_game_sound.c')==b['sound_math_generated_source_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    for key in ('frames_DLL','game_sound_DLL'):assert sha(b[key])==b[key+'_sha256']
    probe_source=p/'jp_port_tools/sound_math_value_probe.c';probe_dll=d/'sound_math_value_probe.dll';compiler=b['commands'][0]['command'][0]
    cmd=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-shared','-fuse-ld=lld',str(probe_source),str(Path(b['game_sound_DLL']).with_suffix('.lib')),'-o',str(probe_dll)]
    run=subprocess.run(cmd,capture_output=True);(d/'probe_build.log').write_bytes(run.stdout+run.stderr);assert not run.returncode and not(run.stdout+run.stderr)
    frame=C.CDLL(b['frames_DLL']);lib=C.CDLL(b['game_sound_DLL']);probe=C.CDLL(str(probe_dll))
    sin=bind(probe,'sound_probe_sin',U,I);cos=bind(probe,'sound_probe_cos',U,I);sqrt=bind(probe,'sound_probe_sqrt',U,U);length=bind(probe,'sound_probe_length',U,P)
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    ref,refhash=make_reference(executor);numeric_path=r/'work/sonic-adventure/build/sh4_vector_numeric.dll'
    assert sha(numeric_path)=='2e56428fadb1903c319da319cf6fa3731839eab1b16163e4e820821d6e4c2db2'
    numeric=bind(C.CDLL(str(numeric_path)),'sa_sh4_vector_numeric',I,U,C.POINTER(Fpu),U);service=service_for(numeric)
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw
    rng=random.Random(0x534d4154);checks=0;sections={};calls={}
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def original(name,bank,arg=0,inputs=()):
        cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.sr=0x70000000;cpu.r[15]=0x8cf00000;cpu.r[4]=arg&0xffffffff
        for i in range(8,15):cpu.r[i]=0x76540000+i
        saved=list(cpu.r[8:]);f=Fpu();f.fpscr=0x40001|(bank<<21)
        f.banks[bank][:]=[0x12340000+i for i in range(16)];f.banks[bank^1][:]=[0x56780000+i for i in range(16)]
        for i,value in enumerate(inputs):f.banks[bank][4+i]=value
        ref(ram,cpu,fpu=f,vector_instruction=service,instruction_budget=100)
        check(cpu.pc==0x8cfed000 and list(cpu.r[8:])==saved and f.fpscr&0x3c0003==0x40001|(bank<<21),'original complete call preserves stack/callee registers and numerical bank policy')
        calls[name]=calls.get(name,0)+1;return f.banks[bank][0]
    start=checks
    for angle in range(65536):
        full=angle|((angle*31337&0xffff)<<16);bank=angle&1
        check(sin(I(full).value)==original('njSin',bank,full),('original sine all16bit phases and high-word removal',angle))
        check(cos(I(full).value)==original('njCos',bank,full),('original cosine all16bit phases and high-word removal',angle))
    sections['all_65536_phases_both_functions_high_words_alternating_banks']=checks-start;print('all_angles',checks-start,'checks',flush=True)
    edge=[0,0x80000000,1,0x80000001,0x007fffff,0x807fffff,0x00800000,0x80800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fbfffff,0xffbfffff,0x7fc12345,0xffc12345]
    values=edge+[rng.getrandbits(32) for _ in range(4096)]
    start=checks
    for word in values:
        for bank in range(2):check(sqrt(word)==original('njSqrt',bank,inputs=(word,)),('original signed square root, exceptional and DN values',hex(word),bank))
        absolute=word&0x7fffffff
        expected=sqrt_word(absolute)^0x80000000 if word&0x80000000 and 0x800000<=absolute<=0x7f800000 else sqrt_word(word)
        check(sqrt(word)==expected,'independent integer-isqrt/class square-root oracle')
    sections['sqrt_signed_classes_and_integer_isqrt']=checks-start;print('square_root',checks-start,'checks',flush=True)
    start=checks
    vectors=[words([v,0,0]) for v in edge]+[words([0,v,0]) for v in edge]+[words([0,0,v]) for v in edge]
    vectors+=[rng.randbytes(12) for _ in range(2048)]
    for raw in vectors:
        buf=C.create_string_buffer(raw);inputs=struct.unpack('<3I',raw)
        for bank in range(2):check(length(buf)==original('vector_length_squared',bank,inputs=inputs),('original scalar-argument vector length',raw.hex(),bank))
        check(buf.raw[:12]==raw,'native scalar argument bridge leaves input bytes intact')
    for _ in range(256):
        v=[rng.randrange(-128,129) for _ in range(3)];raw=struct.pack('<3f',*v);buf=C.create_string_buffer(raw)
        check(words([length(buf)])==struct.pack('<f',sum(x*x for x in v)),'independent exact integer vector geometry')
    sections['vector_length_classes_scalar_ABI_and_independent_geometry']=checks-start
    start=checks
    addresses=(Q*4)();check(bind(lib,'peer_sound_state_addresses',I,P)(addresses)==0,'read actual imported data addresses')
    camera=P.in_dll(frame,'camera_work');event=I.in_dll(frame,'event_skipping');redraw=I.in_dll(frame,'frame_redrawing');pads=(Peripheral*8).in_dll(frame,'pad_records')
    check(list(addresses)==[C.addressof(camera),C.addressof(event),C.addressof(redraw),C.addressof(pads)] and all(x>0xffffffff for x in addresses),'four actual frame-owned data imports; no shadow global')
    check(bind(lib,'peer_sound_sqrt_import',Q)()==C.cast(frame.jp_rz_camera_sqrt,P).value,'actual frame-owned square-root provider')
    layout=(U*5)();check(bind(lib,'peer_sound_state_layout',I,P)(layout)==0 and list(layout)==[C.sizeof(Work),Work.pos.offset,C.sizeof(Peripheral),Peripheral.press.offset,Peripheral.release.offset],'independent qualified task/peripheral ABI matches sound consumer headers')
    saved=(camera.value,event.value,redraw.value,bytes(pads));fixture=Work();out=(Q*19)();position=(U*3)()
    snapshot=bind(lib,'peer_sound_state_values',I,P);readposition=bind(lib,'peer_sound_camera_position',I,P)
    try:
        for case in range(256):
            camera.value=C.addressof(fixture) if case%3 else None;event.value=I(rng.getrandbits(32)).value;redraw.value=I(rng.getrandbits(32)).value
            for i in range(8):pads[i].press=rng.getrandbits(32);pads[i].release=rng.getrandbits(32)
            raw=rng.randbytes(12);C.memmove(C.addressof(fixture)+Work.pos.offset,raw,12)
            expected=[camera.value or 0,event.value&0xffffffff,redraw.value&0xffffffff]+[v for pad in pads for v in (pad.press,pad.release)]
            check(snapshot(out)==0 and list(out)==expected,'sound reads exact real shared flags and all8 controller records')
            C.memset(position,0xa5,12);code=readposition(position)
            check(code==(0 if camera.value else 2) and bytes(position)==(raw if camera.value else b'\xa5'*12),'high native camera pointer and position, with explicit null-camera result')
        check(snapshot(None)==readposition(None)==1,'invalid readback arguments rejected')
    finally:
        camera.value,event.value,redraw.value=saved[:3];C.memmove(pads,saved[3],C.sizeof(pads))
    check((camera.value,event.value,redraw.value,bytes(pads))==saved,'all explicit shared-state test inputs restored')
    sections['actual_shared_state_identity_native_layout_and_cross_module_reads']=checks-start
    result=dict(status='original_sound_math_all_phase_signed_sqrt_vector_length_and_actual_shared_state_imports_verified',runtime_checks=checks,sections=sections,
        complete_original_entry_calls=sum(calls.values()),original_entry_calls=calls,original_function_hooks=False,
        all_65536_angle_phases_checked=True,native_pointer_bits=64,shared_globals_imported_without_duplicates=True,
        controlled_state_fixtures_not_physical_input_or_camera_gameplay=True,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),reference_hashes=refhash,
        probe_source_sha256=sha(probe_source),probe_DLL_sha256=sha(probe_dll),probe_build_command=cmd,
        pinned_vector_instruction_model_DLL_sha256=sha(numeric_path),vector_model_is_shared_with_native=True,
        original_hardware_FPSCR_side_effects_or_full_game_audio_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks;',sum(calls.values()),'complete original calls',flush=True)
if __name__=='__main__':main()
