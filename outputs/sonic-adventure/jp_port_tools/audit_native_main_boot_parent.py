"""Audit exact higher-bootstrap adaptation and unchanged qualified providers."""
import argparse,ctypes as C,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(a.baseline/'build_verification.json')==b['main_boot_baseline_sha256']
    original=(a.upstream/'src/main.c').read_bytes();pinned=subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(a.upstream),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:src/main.c']);assert original==pinned
    body=function(original,'system_boot_init');adapted=body.replace(b'    sound_system_init();',b'    sound_system_init();\n    if(jp_main_boot_sound_failed())return;')
    source=a.build/'external_main_boot.c';assert function(source.read_bytes(),'system_boot_init')==adapted
    assert hashlib.sha256(body).hexdigest()==b['main_boot_original_body_sha256'] and hashlib.sha256(adapted).hexdigest()==b['main_boot_native_adapted_body_sha256']
    assert sha(source)==b['main_boot_generated_source_sha256'] and b['main_boot_original_body_unchanged'] is False
    owner=r/'outputs/sonic-adventure/native/jp_native_main_boot.c';owner_raw=owner.read_bytes()
    assert b'setjmp' not in owner_raw and b'longjmp' not in owner_raw
    sound=function(owner_raw,'sound_system_init');assert b'jp_system_start_sound_clock(&boot.sound,&boot.clock)' in sound and b'boot.system_owned=1;' in sound
    shutdown=function(owner_raw,'jp_main_boot_shutdown');assert b'if(boot.system_owned)' in shutdown and b'if(jp_textures_live_surfaces()>=before)' in shutdown
    assert shutdown.index(b'buExit()')<shutdown.index(b'jp_system_shutdown()')<shutdown.index(b'njExitPrint()')
    init=function(owner_raw,'jp_main_boot_initialize');assert b'prior_card.backend_initialized' in init and b'if(jp_textures_live_surfaces())' in init
    host=b['commands'][1]['command'];assert host[host.index('-c')+1]==str(owner.resolve())
    assert not any(str(a.upstream.resolve()).lower() in n.lower() for n in host),'host owner must not use SH-4 CRT headers'
    assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:3])
    shared=('cable_mode','dat_8c78b4d8','dat_8c78b4dc','texture_buffer','shape_buffer','per_info')
    original_symbols=coff_symbols(a.build/'main_boot_original.obj')
    for name in shared:
        assert any(s['name']=='__imp_'+name and s['section']==0 for s in original_symbols),name
        assert not any(s['name']==name and s['section']>0 for s in original_symbols),name
    kept={}
    for path in a.baseline.iterdir():
        if path.suffix in ('.dll','.lib','.obj'):
            assert sha(path)==sha(a.build/path.name),path.name;kept[path.name]=sha(path)
    assert (a.build/'external_game_sound.c').read_bytes()==(a.baseline/'external_game_sound.c').read_bytes()
    lib=C.CDLL(b['main_boot_DLL']);assert sha(b['main_boot_DLL'])==b['main_boot_DLL_sha256']
    for name in ('jp_main_boot_configure','jp_main_boot_initialize','jp_main_boot_shutdown','jp_main_boot_snapshot','system_boot_init'):assert hasattr(lib,name)
    old=r/'outputs/sonic-adventure/analysis/jp_native_sound_placement_package.json';seen=set();prior(r,old,seen)
    result=dict(status='exact_native_higher_bootstrap_checked_return_shared_imports_and_preserved_prior_graph_audited',original_body_sha256=hashlib.sha256(body).hexdigest(),adapted_body_sha256=hashlib.sha256(adapted).hexdigest(),native_change='One immediate checked return after the genuine sound initialization call.',original_successful_call_order_retained=True,original_source_unchanged=True,host_CRT_header_path_isolated=True,nonlocal_stack_unwinding_used=False,shared_data_imported_without_duplicates=list(shared),unchanged_prior_binaries_and_objects=kept,qualified_prior_package_sha256=sha(old),prior_qualification_packages_checked=len(seen),terminal_global_card_exit_or_full_game_init_verified=False,build_verification_sha256=sha(a.build/'build_verification.json'),auditor_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    d.mkdir();(d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(kept),'unchanged prior artifacts;',len(seen),'preserved qualification packages',flush=True)
if __name__=='__main__':main()
