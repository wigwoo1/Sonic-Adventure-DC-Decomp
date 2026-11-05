"""Compile original higher bootstrap with real shared imports; never execute it."""
import argparse,ctypes as C,hashlib,json,random,re,subprocess,sys
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from model_reference import make_reference
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    raw=(u/'src/main.c').read_bytes();body=function(raw,'system_boot_init');assert hashlib.sha256(body).hexdigest()==b['original_system_function_sha256']['system_boot_init']
    prefix=raw[:raw.index(b'enum menu_entrance menu_entrance;')]
    inc=d/'include';(inc/'system').mkdir(parents=True);headers={}
    for relative,names in [('main.h',('cable_mode','dat_8c78b4d8','dat_8c78b4dc','shape_buffer')),('system/pad.h',('per_info',))]:
        raw_header=(u/'src'/relative).read_bytes();changed=raw_header
        for name in names:
            pattern=rb'^extern ([^;\n]*\b'+name.encode()+rb'\b[^;\n]*);'
            changed,count=re.subn(pattern,rb'extern __declspec(dllimport) \1;',changed,flags=re.M);assert count==1,(relative,name)
        target=inc/relative;target.write_bytes(changed);headers[relative]=dict(original_sha256=hashlib.sha256(raw_header).hexdigest(),private_native_overlay_sha256=sha(target))
    old=b'extern Sint8 texture_buffer[];';assert prefix.count(old)==1;prefix=prefix.replace(old,b'extern __declspec(dllimport) Sint8 texture_buffer[];')
    src=d/'external_system_boot_init.c';src.write_bytes(prefix+body);obj=d/'system_boot_init.obj'
    common=list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and any(n.startswith('-I') and Path(n[2:]).resolve()==u/'sdk/include' for n in row['command'])))
    common=common[:common.index('-c')]
    common.insert(1,'-I'+str(inc))
    for root in ('src','modules'):
        if '-I'+str(u/root) not in common:common.append('-I'+str(u/root))
    command=common+['-c',str(src),'-o',str(obj)];run=subprocess.run(command,capture_output=True);(d/'compile.txt').write_bytes(run.stdout+run.stderr);assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()
    labels=('frames_DLL','GPU_DLL','present_DLL','vertex_lifecycle_DLL','device_DLL','SDK_heap_DLL','RTC_DLL','GDFS_DLL','System_DLL','authored_exit_DLL','DLL')
    modules={n:C.CDLL(b[n]) for n in labels};providers={}
    for symbol in coff_symbols(obj):
        if symbol['section']!=0 or not symbol['name']:continue
        n=symbol['name'];base=n.removeprefix('__imp_');providers[n]=dict(export=base,data_import=n.startswith('__imp_'),owners=[label for label,lib in modules.items() if hasattr(lib,base)])
    missing=[n for n,row in providers.items() if not row['owners']]
    compiler=common[0];libs=[]
    for label in labels:
        path=Path(b[label]).with_suffix('.lib')
        if str(path) not in libs:libs.append(str(path))
    link=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(obj),*libs,'-o',str(d/'unexecuted_system_boot_probe.dll')]
    linked=subprocess.run(link,capture_output=True);(d/'link.txt').write_bytes(linked.stdout+linked.stderr)
    assert missing and linked.returncode and not (d/'unexecuted_system_boot_probe.dll').exists(),'do not execute an unqualified whole parent'
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    ref,refhash=make_reference(executor);symbols={name:int(at,16) for at,name in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
    image=a.binary.read_bytes();assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf';ram=bytearray(16<<20);ram[0x10000:0x10000+len(image)]=image
    rng=random.Random(0x43414348);cache={};calls=0
    for name in ('cache_ram_init','cache_ram_restore','cache_ram_end'):
        at=symbols[name];instructions=bytes(ram[at&0xffffff:(at&0xffffff)+4]);assert instructions==bytes.fromhex('0b000900')
        for _ in range(8):
            cpu=executor.State();cpu.pc=at;cpu.pr=0x8cfed000;cpu.r[:]=[rng.getrandbits(32) for j in range(16)];cpu.sr=0x70000000|rng.randrange(16)*16;before=(list(cpu.r),cpu.sr,cpu.pr)
            fpu=Fpu();fpu.fpscr=0x40001;ref(ram,cpu,fpu=fpu,instruction_budget=20)
            assert (list(cpu.r),cpu.sr,cpu.pr)==before and cpu.pc==0x8cfed000;calls+=1
        cache[name]=dict(address=hex(at),first_instructions=instructions.hex(),complete_original_calls=8,registers_and_SR_preserved=True)
    result=dict(status='original_higher_system_boot_compiles_with_real_shared_imports_and_remaining_link_dependency_identified',original_main_sha256=sha(u/'src/main.c'),original_function_sha256=hashlib.sha256(body).hexdigest(),
        generated_private_source_sha256=sha(src),original_function_body_unchanged=function(src.read_bytes(),'system_boot_init')==body,object_sha256=sha(obj),compile_command=command,compile_exit_code=0,compile_diagnostic_bytes=0,
        shared_data_explicitly_imported=['cable_mode','dat_8c78b4d8','dat_8c78b4dc','shape_buffer','texture_buffer','per_info'],private_native_header_overlays=headers,imports=providers,missing_exports=missing,
        expected_unresolved_link_command=link,expected_unresolved_link_exit_code=linked.returncode,link_diagnostic_sha256=sha(d/'link.txt'),linked_or_executed=False,
        actual_native_core_export_provider='System_DLL',no_successful_sound_or_hardware_stubs_added=True,
        original_cache_helpers=cache,original_complete_cache_machine_calls=calls,original_machine_hooks_used=False,reference_hashes=refhash,
        original_binary_sha256=sha(a.binary),build_verification_sha256=sha(a.build/'build_verification.json'),auditor_sha256=sha(__file__),overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],'missing',missing,'cache calls',calls,flush=True)
if __name__=='__main__':main()
