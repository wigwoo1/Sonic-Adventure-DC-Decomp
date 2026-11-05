"""Real Windows mode query, unchanged SDK decoder and shared original globals."""
import argparse,ctypes as C,hashlib,json,re,struct,subprocess
from pathlib import Path
from native_packet_test_support import bind
from model_reference import make_reference
I=C.c_int;U=C.c_uint32;P=C.c_void_p;S=C.c_size_t
class Info(C.Structure):_fields_=[(n,U) for n in ('bytes','width','height','bpp','hz','hz_valid','legacy_cable','flags')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    out.mkdir();lib=C.CDLL(b['frames_DLL']);query=bind(lib,'jp_display_query',I,P,S);low=bind(lib,'syCblCheckCableType',U);cable=bind(lib,'syCblCheckCable',I)
    checks=[]
    def check(ok,label):assert ok,label;checks.append(label)
    guard=C.create_string_buffer(b'\xa5'*96);before=bytes(guard)
    check(query(None,32)==1 and query(C.byref(guard,32),31)==1 and bytes(guard)==before,'invalid display-query spans preserve all caller bytes')
    check(query(C.byref(guard,32),32)==0 and guard.raw[:32]==guard.raw[64:96]==b'\xa5'*32,'actual bounded Windows display query and both canaries')
    info=Info.from_buffer_copy(guard.raw[32:64]);check(info.bytes==32 and info.width>0 and info.height>0 and info.bpp>0,'actual current display mode returned')
    check(info.hz_valid==(info.hz>1) and info.legacy_cable==0,'driver refresh validity and explicit progressive-PC compatibility profile')
    check(low()==info.legacy_cable<<8 and cable()==info.legacy_cable,'original SDK decoder consumes actual host profile')
    names=['cable_mode','dat_8c78b4d8','dat_8c78b4dc'];values={n:I.in_dll(lib,n) for n in names}
    check(all(v.value==0 for v in values.values()) and len({C.addressof(v) for v in values.values()})==3,'three distinct original startup globals begin as zeroed four-byte data')
    fixture=out/'startup_state_peer.c';fixture.write_text('''
__declspec(dllimport) extern int cable_mode,dat_8c78b4d8,dat_8c78b4dc;
__declspec(dllexport) void peer_read(int *out){out[0]=cable_mode;out[1]=dat_8c78b4d8;out[2]=dat_8c78b4dc;}
__declspec(dllexport) void peer_write(const int *in){cable_mode=in[0];dat_8c78b4d8=in[1];dat_8c78b4dc=in[2];}
''');peer_dll=out/'startup_state_peer.dll';cmd=[b['commands'][0]['command'][0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-shared','-fuse-ld=lld',str(fixture),str(a.build/'jp_native_frames.lib'),'-o',str(peer_dll)]
    run=subprocess.run(cmd,capture_output=True);(out/'peer_build.txt').write_bytes(run.stdout+run.stderr);check(not run.returncode and not run.stdout and not run.stderr,'real peer DLL imports original globals as data')
    peer=C.CDLL(str(peer_dll));get=bind(peer,'peer_read',None,P);put=bind(peer,'peer_write',None,P);observed=(I*3)();pattern=(I*3)(3,0x12345678,-12345)
    put(pattern);get(observed);check(list(observed)==list(pattern)==[values[n].value for n in names],'peer writes reach the same original startup data owners')
    values[names[0]].value=0;values[names[1]].value=-77;values[names[2]].value=1234567;get(observed);check(list(observed)==[0,-77,1234567],'frame-owner writes reach genuine peer data imports')
    check(cable()==0 and [v.value for v in values.values()]==[0,-77,1234567],'display detection does not silently reset unrelated original main state')
    import sys
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,_=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;original_calls=0;entered=set();entries={table[n]:n for n in ('syCblCheckCable','syCblCheckCableType')}
    def trace(pc,*unused):
        if pc in entries:entered.add(entries[pc])
    # The original register-reading instructions use the executor's explicit
    # MMIO word provider; no original function/branch or instruction is replaced.
    for high_bits in (0,0xfcff,0x50a5,0xafff):
        pins=(high_bits&~0x300)|(info.legacy_cable<<8);mmio={0xff800030:pins}
        cpu=executor.State();cpu.pc=table['syCblCheckCable'];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000;fpu=Fpu();fpu.fpscr=0x40001
        reference(ram,cpu,fpu=fpu,mmio=mmio,trace=trace,instruction_budget=10000);original_calls+=1
        check(cpu.r[15]==0x8cf00000 and cpu.r[0]==cable()==info.legacy_cable,'complete original SDK cable reader/decoder matches native profile independently of other register bits')
    check(entered=={'syCblCheckCable','syCblCheckCableType'},'both original hardware-reader and decoder bodies executed with no hooks')
    for v in values.values():v.value=0
    result=dict(status='actual_Windows_display_original_SDK_decoder_and_single_shared_startup_globals_verified',checks=len(checks),check_labels=checks,actual_display=dict(width=info.width,height=info.height,bits_per_pixel=info.bpp,driver_reported_hz=info.hz,refresh_valid=bool(info.hz_valid),legacy_cable=info.legacy_cable,display_flags=info.flags),original_machine_calls=original_calls,original_entries_seen=sorted(entered),original_function_hooks_used=False,original_globals_shared_across_real_DLL_imports=True,OS_display_settings_modified=False,physical_monitor_cable_type_claimed=False,measured_presentation_rate_claimed=False,peer_build_command=cmd,peer_fixture_sha256=sha(fixture),peer_DLL_sha256=sha(peer_dll),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),sbInitSystem_or_sound_implemented=False,whole_gameplay_verified=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',flush=True)
if __name__=='__main__':main()
