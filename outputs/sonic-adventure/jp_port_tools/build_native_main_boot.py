"""Link the complete original higher bootstrap to real native sound ownership.

Original game/SDK source and combined diagnostic binaries stay private.
"""
import argparse,ctypes as C,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=json.loads((base/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir()
    for source in base.iterdir():
        if source.is_file() and source.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(source,d/source.name)
        elif source.is_dir() and source.name in ('include','inputs','sound_imports','rna_inputs','adxt_inputs','placement_inputs','matching_marker'):shutil.copytree(source,d/source.name)
    main_source=u/'src/main.c';raw=main_source.read_bytes();body=function(raw,'system_boot_init')
    pin=subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:src/main.c'])
    assert raw==pin and hashlib.sha256(body).hexdigest()==b['original_system_function_sha256']['system_boot_init']
    inc=d/'main_boot_imports';(inc/'system').mkdir(parents=True);headers={}
    for relative,names in [('main.h',('cable_mode','dat_8c78b4d8','dat_8c78b4dc','shape_buffer')),('system/pad.h',('per_info',))]:
        original=(u/'src'/relative).read_bytes();changed=original
        for name in names:
            changed,count=re.subn(rb'^extern ([^;\n]*\b'+name.encode()+rb'\b[^;\n]*);',rb'extern __declspec(dllimport) \1;',changed,flags=re.M);assert count==1
        (inc/relative).write_bytes(changed);headers[relative]=dict(original_sha256=hashlib.sha256(original).hexdigest(),native_import_header_sha256=sha(inc/relative))
    card=p/'native/jp_native_card_state.h';(inc/'jp_main_boot_card_imports.h').write_bytes(card.read_bytes().replace(b'__declspec(dllexport)',b'__declspec(dllimport)'))
    prefix=raw[:raw.index(b'enum menu_entrance menu_entrance;')]
    assert prefix.count(b'extern Sint8 texture_buffer[];')==1
    prefix=prefix.replace(b'extern Sint8 texture_buffer[];',b'extern __declspec(dllimport) Sint8 texture_buffer[];')
    assert body.count(b'    sound_system_init();')==1
    adapted=body.replace(b'    sound_system_init();',b'    sound_system_init();\n    if(jp_main_boot_sound_failed())return;')
    generated=d/'external_main_boot.c';generated.write_bytes(prefix+b'\nextern int jp_main_boot_sound_failed(void);\n#define system_boot_init jp_original_system_boot_init\n'+adapted)
    common=list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and any(n.startswith('-I') and Path(n[2:]).resolve()==u/'sdk/include' for n in row['command'])));common=common[:common.index('-c')];common.insert(1,'-I'+str(inc))
    if '-ffreestanding' not in common:common.append('-ffreestanding')
    for folder in ('src','modules'):
        if '-I'+str(u/folder) not in common:common.append('-I'+str(u/folder))
    sources=[generated,p/'native/jp_native_main_boot.c'];objects=[d/'main_boot_original.obj',d/'main_boot_owner.obj']
    # Authored Windows ownership code must use the host CRT headers. The SDK
    # includes SH-4 CRT declarations incompatible with the native Windows ABI.
    host=[common[0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-I'+str(inc)]
    commands=[common+['-c',str(sources[0]),'-o',str(objects[0])],host+['-c',str(sources[1]),'-o',str(objects[1])]];results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);(d/f'main_boot_build_{i}.txt').write_bytes(run.stdout+run.stderr);assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode();results.append(dict(command=cmd,exit_code=0,diagnostic_bytes=0))
    labels=('frames_DLL','GPU_DLL','present_DLL','vertex_lifecycle_DLL','device_DLL','SDK_heap_DLL','RTC_DLL','GDFS_DLL','System_DLL','authored_exit_DLL','DLL','game_sound_DLL')
    modules={n:C.CDLL(b[n]) for n in labels};providers={}
    for symbol in coff_symbols(objects[0]):
        if symbol['section']!=0 or not symbol['name']:continue
        name=symbol['name'];base_name=name.removeprefix('__imp_');providers[name]=[label for label,lib in modules.items() if hasattr(lib,base_name)]
    local={'sound_system_init','jp_main_boot_sound_failed'}
    assert {n for n,v in providers.items() if not v}==local,providers
    definitions={s['name'] for s in coff_symbols(objects[1]) if s['section']>0}
    assert local<=definitions
    for name in local:providers[name]=['main_boot_owner.obj']
    libs=list(dict.fromkeys(str(d/Path(b[n]).with_suffix('.lib').name) for n in labels))
    dll=d/'jp_native_main_boot.dll';cmd=[common[0],'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*map(str,objects),*libs,'-o',str(dll)]
    run=subprocess.run(cmd,capture_output=True);(d/'main_boot_link.txt').write_bytes(run.stdout+run.stderr);assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode();results.append(dict(command=cmd,exit_code=0,diagnostic_bytes=0))
    updates={}
    for n,v in b.items():
        if (n=='DLL' or n.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            assert sha(v)==b[n+'_sha256']==sha(d/Path(v).name);updates[n]=str(d/Path(v).name)
    authored=[p/'native/jp_native_main_boot.c',p/'native/jp_native_main_boot.h',Path(__file__).resolve()]
    updates.update(status='complete_original_higher_bootstrap_linked_runtime_unverified',main_boot_DLL=str(dll),main_boot_DLL_sha256=sha(dll),main_boot_baseline_sha256=sha(a.baseline/'build_verification.json'),main_boot_original_body_sha256=hashlib.sha256(body).hexdigest(),main_boot_original_source_sha256=sha(main_source),main_boot_generated_source_sha256=sha(generated),main_boot_native_adapted_body_sha256=hashlib.sha256(adapted).hexdigest(),main_boot_original_body_unchanged=False,main_boot_native_adaptation='Checked return immediately after genuine System-owned sound initialization; complete original successful call order retained.',main_boot_native_import_headers=headers,main_boot_original_import_providers=providers,native_main_system_boot_init_linked=True,native_main_system_boot_init_runtime_verified=False,authored_source_sha256=dict(b['authored_source_sha256'],**{path.relative_to(r).as_posix():sha(path) for path in authored}),commands=results+b['commands'],playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
