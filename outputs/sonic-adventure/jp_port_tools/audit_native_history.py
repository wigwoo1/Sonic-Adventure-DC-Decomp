"""Audit genuine UF/game bodies, pointer adaptation and acyclic native ownership."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,build,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','build','output'))
    p=r/'outputs/sonic-adventure';b=read(build/'build_verification.json');old=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    assert sha(base/'build_verification.json')==b['history_baseline_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['history_original_source_sha256'].items():
        assert sha(u/n)==h
        assert hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()==h
    for n,h in b['history_generated_source_sha256'].items():assert sha(build/('history_include' if n=='sg_uf.h' else '')/n)==h,n
    original_objects={}
    for n in ('flash_original.obj','flash_crc.obj'):
        assert sha(base/n)==sha(build/n);original_objects[n]=sha(build/n)
    functions={};unchanged=[];changed=[]
    units={n:'history_'+Path(n).name for n in b['history_original_source_sha256'] if n.endswith('.c')}
    units['src/system/history.c']='history_game.c';units['sdk/shinobi/sg_syb2.c']='history_identity.c'
    pattern=rb'(?m)^(?:static\s+)?(?:[A-Za-z_]\w*[ \t*]+)+([A-Za-z_]\w*)\([^;{}]*\)\r?\n\{'
    for name,target in units.items():
        if name=='sdk/runtime/_ctype.c':assert (build/target).read_bytes()==(u/name).read_bytes();continue
        raw=(u/name).read_bytes();native=(build/target).read_bytes()
        names=[m[1].decode() for m in re.finditer(pattern,raw)]
        if name=='sdk/shinobi/sg_syb2.c':names=['syBtGetBootSystemID','read_boot_identity','read_disc_numbers']
        for n in names:
            original=function(raw,n);adapted=function(native,n);key=name+':'+n
            functions[key]=dict(original_sha256=hashlib.sha256(original).hexdigest(),native_sha256=hashlib.sha256(adapted).hexdigest())
            (unchanged if original==adapted else changed).append(key)
    assert all(n in changed for n in ('sdk/shinobi/sg_userflag.c:ufGetUserState','sdk/shinobi/sg_userhis.c:fhFmReadExec','sdk/shinobi/sg_userhis.c:fhFmWriteExec','src/system/history.c:history_init','src/system/history.c:history_write'))
    assert 'sdk/shinobi/sg_syb2.c:syBtGetBootSystemID' in unchanged
    header=(build/'history_include/sg_uf.h').read_bytes()
    assert b'void ufGetUserState(intptr_t buf);' in header and b'void ufGetUserState(Sint32 buf);' not in header
    assert b'(void *)0x0cd00000' not in (build/'history_game.c').read_bytes()
    host=(p/'native/jp_native_history.c').read_bytes();flash=(build/'flash_with_history.c').read_bytes()
    assert b'rtc_state.initialized' in host and b'jp_history_source_write();' in host
    assert b'jp_flash_history_transaction_abort' in host and b'jp_flash_history_transaction_commit' in host
    assert b'free(history.work3);free(history.work4)' in host
    assert host.index(b'jp_flash_close();')<host.index(b'free(history.work3);free(history.work4)')
    assert b'flash.history_backup=flash.image;flash.image=next' in flash
    commit=function(flash,'jp_flash_history_transaction_commit')
    assert commit.index(b'persist(flash.image,1)')<commit.index(b'free(flash.history_backup)')
    assert b'memcmp(disk,flash.history_backup,IMAGE_BYTES)' in commit
    reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe');imports={};texts={}
    for key in ('history_DLL','flash_DLL'):
        assert sha(b[key])==b[key+'_sha256'];run=subprocess.run([str(reader),'--coff-imports',b[key]],capture_output=True)
        assert not run.returncode and not run.stderr;texts[key]=run.stdout
        imports[key]=[n.strip().lower() for n in re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)]
    assert not any(n.startswith('jp_native_') for n in imports['flash_DLL'])
    assert {'jp_native_flash.dll','jp_native_frames.dll','jp_native_rtc.dll'}<=set(imports['history_DLL'])
    assert not {'jp_native_system.dll','jp_native_exit.dll','jp_native_main_boot.dll'}&set(imports['history_DLL'])
    # One actual original UF record and game counter table, no second producer.
    symbols=[]
    for path in build.glob('history_*.obj'):symbols+=coff_symbols(path)
    for name in ('ufgPlayHistBuffer','history_counts','njMemCopy4','_ctype','ufGetUserState'):
        assert sum(s['name']==name and s['section']>0 for s in symbols)==1,name
    preserved={}
    for k,v in old.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll') and k!='flash_DLL':
            assert sha(v)==sha(b[k])==b[k+'_sha256'];preserved[str(Path(v).resolve())]=sha(v)
    assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:15])
    old_package=p/'analysis/jp_native_flash_manager_package.json';seen=set();prior(r,old_package,seen)
    d.mkdir()
    for k,raw in texts.items():(d/(k+'_imports.txt')).write_bytes(raw)
    report=dict(status='original_UF_game_history_native_width_transaction_and_owned_graph_audited',
        build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),
        function_sha256=functions,unchanged_original_functions=unchanged,explicitly_adapted_original_functions=changed,
        unchanged_original_flash_objects=original_objects,unchanged_prior_DLLs=preserved,actual_PE_imports=imports,
        one_actual_UF_record_and_game_counter_table=True,original_memory_copy_and_character_table_retained=True,
        native_pointer_width_adapted_without_header_warning_suppression=True,
        atomic_profile_commit_after_complete_original_save_or_refresh=True,
        prior_qualification_packages_checked=len(seen),prior_package_sha256=sha(old_package),
        full_game_init_or_global_history_exit_handoff_verified=False,combined_game_SDK_binaries_publishable=False,
        playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],len(unchanged),'unchanged;',len(changed),'adapted original functions;',len(preserved),'unchanged prior DLLs;',len(seen),'prior packages',flush=True)
if __name__=='__main__':main()
