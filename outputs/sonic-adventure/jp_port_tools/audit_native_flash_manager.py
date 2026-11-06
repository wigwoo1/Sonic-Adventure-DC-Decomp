"""Prove original FM/CRC inclusion, guarded exports and unchanged prior graph."""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_flash_manager import FM,PIN
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,build,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','build','output'))
    p=r/'outputs/sonic-adventure';b=read(build/'build_verification.json');old=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    assert sha(base/'build_verification.json')==b['flash_baseline_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['flash_original_source_sha256'].items():
        assert sha(u/n)==h
        assert hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()==h
    for n,h in b['flash_generated_source_sha256'].items():assert sha(build/n)==h,n
    prefix=b''.join(('#define '+n+' jp_flash_source_'+n+'\n').encode() for n in FM)+b'#include <shinobi.h>\n#define GCC\n'
    suffix=b'\n'+b''.join(('#undef '+n+'\n').encode() for n in FM)+(p/'native/jp_native_flash_fm.inc').read_bytes()
    raw=(u/'sdk/shinobi/fm.c').read_bytes()
    assert (build/'external_flash_manager.c').read_bytes()==prefix+raw+suffix
    assert (build/'external_flash_crc.c').read_bytes()==(u/'sdk/shinobi/fmcrc.c').read_bytes()
    symbols=coff_symbols(build/'flash_original.obj');names=[s['name'] for s in symbols if s['section']>0]
    reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
    static_run=subprocess.run([str(reader),'--symbols',str(build/'flash_original.obj')],capture_output=True)
    assert not static_run.returncode and not static_run.stderr
    # The existing COFF helper intentionally lists EXTERNAL symbols only.
    # Inspect the actual static state symbols through LLVM as well.
    all_names=re.findall(r'^\s*Name: (\w+)$',static_run.stdout.decode(),re.M)
    assert all_names.count('active')==all_names.count('storage')==1
    for n in FM:assert names.count(n)==names.count('jp_flash_source_'+n)==1,n
    assert not any(s['name'].startswith('__imp_') and 'fmMng' in s['name'] for s in symbols)
    parent=function((u/'src/main.c').read_bytes(),'game_init')
    assert function((build/'external_game_init_probe.c').read_bytes(),'game_init')==parent
    assert {n for n,v in b['game_init_actual_import_providers'].items() if not v}=={
        'mwPlyPreInitSofdec','game_mode','read_queue_init','history_init','loader_error_flag','loader_note_error'}
    preserved={}
    for key,value in old.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):
            assert b[key]==value and sha(value)==old[key+'_sha256']==b[key+'_sha256'],key
            preserved[value]=sha(value)
    for n,h in b['flash_object_sha256'].items():assert sha(build/n)==h,n
    assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:5])
    run=subprocess.run([str(reader),'--coff-imports',b['flash_DLL']],capture_output=True)
    assert not run.returncode and not run.stderr
    imports=[s.strip().lower() for s in re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)]
    assert imports and not any(n.startswith('jp_native_') for n in imports)
    lib=C.CDLL(b['flash_DLL'])
    for n in FM+('fmcalccrc','syBtFlGetInfo','syBtFlRead','syBtFlWrite','syBtFlDeleteSct',
                 'jp_flash_open','jp_flash_close','jp_flash_fm_snapshot'):assert hasattr(lib,n),n
    old_package=p/'analysis/jp_native_boot_lifetimes_package.json';seen=set();prior(r,old_package,seen)
    owner=(p/'native/jp_native_flash.c').read_bytes()
    assert owner.index(b'rc=persist(next,1)')<owner.index(b'free(flash.image);flash.image=next')
    assert b'FlushFileBuffers' in owner and b'MOVEFILE_WRITE_THROUGH' in owner
    assert b'memcmp(next,flash.image,IMAGE_BYTES)' in owner
    d.mkdir();(d/'actual_imports.txt').write_bytes(run.stdout);(d/'actual_symbols.txt').write_bytes(static_run.stdout)
    result=dict(status='original_FM_CRC_full_parent_probe_and_preserved_native_graph_audited',
        build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),
        original_FM_translation_unit_sha256=sha(u/'sdk/shinobi/fm.c'),
        original_CRC_translation_unit_sha256=sha(u/'sdk/shinobi/fmcrc.c'),
        original_FM_exported_algorithms=len(FM),original_FM_state_copies=1,
        original_translation_unit_bytes_preserved=True,SDK_declarations_loaded_before_GCC_body_branch=True,
        unchanged_prior_DLLs=preserved,actual_flash_DLL_imports=imports,
        prior_qualification_packages_checked=len(seen),prior_package_sha256=sha(old_package),
        game_init_probe_unchanged=True,remaining_parent_symbols=[n for n,v in b['game_init_actual_import_providers'].items() if not v],
        original_boot_ROM_SH4_syscall_code_compiled=False,game_init_or_UF_history_executed=False,
        combined_game_SDK_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(preserved),'unchanged prior DLLs;',len(seen),'prior packages',flush=True)
if __name__=='__main__':main()
