"""Audit History handoff into the existing System and Exit owners."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from package_native_sound_placement import prior

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,base,build,d=(getattr(a,n).resolve() for n in ('workspace','baseline','build','output'))
    p=r/'outputs/sonic-adventure';b=read(build/'build_verification.json');old=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    assert sha(base/'build_verification.json')==b['history_exit_baseline_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['history_exit_generated_source_sha256'].items():assert sha(build/n)==h,n
    preserved_sources={}
    for n,h in b['history_generated_source_sha256'].items():
        path=build/('history_include' if n=='sg_uf.h' else '')/n
        assert sha(path)==h;preserved_sources[n]=h
    preserved_objects={}
    for path in base.glob('*.obj'):
        assert sha(path)==sha(build/path.name),path.name
        preserved_objects[path.name]=sha(path)
    owner_paths={'history_owner':p/'native/jp_native_history.c'}
    for kind,name in (('System','system_with_boot_lifetimes.c'),('Exit','exit_with_boot_lifetimes.c')):
        cmd=next(row['command'] for row in old['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name==name)
        owner_paths[kind]=Path(cmd[cmd.index('-c')+1])
    assert {k:sha(v) for k,v in owner_paths.items()}==b['history_exit_original_owner_sha256']
    system=(build/'system_with_history.c').read_bytes();exit=(build/'exit_with_history.c').read_bytes();history=(build/'history_with_exit.c').read_bytes()
    assert function(system,'bootstrap_cleanup')==function(owner_paths['System'].read_bytes(),'bootstrap_cleanup')
    assert system.count(b'rc=jp_exit_prepare_bootstrap(')==1
    assert system.count(b'} system_owner=')==1 and history.count(b'} history;')==1
    assert b'jp_exit_prepare_bootstrap' not in (p/'native/jp_native_system_history.inc').read_bytes()
    for raw,name,prefix in ((system,'shutdown_owned',b'CLOSE('),(exit,'jp_exit_cleanup',b'CLOSE_INT(')):
        body=function(raw,name)
        order=[body.index(prefix+bit) for bit in (b'JP_EXIT_CARD,',b'JP_EXIT_HISTORY,',b'JP_EXIT_SOUND,',b'JP_EXIT_GDFS,',b'JP_EXIT_RTC,')]
        assert order==sorted(order)
    cleanup=function(exit,'jp_exit_cleanup')
    assert cleanup.index(b'boot_exit_activity()')<cleanup.index(b'state.busy=1')<cleanup.index(b'bootstrap.started=1')
    finish=function(history,'history_exit_close')
    assert finish.index(b'jp_history_exit_can_close(token)')<finish.index(b'history.exit_terminal=1')
    assert finish.index(b'if(history.exit_finished)return 0;')<finish.index(b'jp_history_shutdown()')
    bind=function(exit,'jp_exit_bind')
    assert bind.index(b'!bootstrap.history_token')<bind.index(b'jp_sdk_heap_exit_claim(')
    assert bind.index(b'jp_device_exit_claim(')<bind.index(b'state.history_token=history_token')<bind.index(b'memset(&bootstrap,0')
    attach=function(exit,'jp_exit_attach_bootstrap_history')
    assert b'bootstrap.cleanup!=cleanup||bootstrap.context!=context' in attach
    assert b'claim.token!=token||claim.terminal||claim.closing' in attach
    start=function(system,'jp_system_start_history')
    assert start.index(b'old.resources')<start.index(b'jp_history_configure(options)')
    assert start.index(b'system_owner.busy=1')<start.index(b'jp_history_initialize()')<start.index(b'system_history_adopt()')
    shutdown=function(history,'jp_history_shutdown')
    assert b'if(history.source_live){history.info.dirty=1;' in shutdown
    assert shutdown.index(b'save_owned()')<shutdown.index(b'jp_flash_close()')<shutdown.index(b'free(history.work3)')
    for name in ('jp_history_save','jp_history_read','count','jp_history_peer_begin','jp_history_refresh'):
        assert b'history.exit_closing||history.exit_terminal' in function(history,name)
    assert b'exit_closing' not in function(history,'jp_history_recover')
    reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe');imports={};texts={};dlls={};preserved={}
    changed={'history_DLL','System_DLL','authored_exit_DLL'}
    for k,v in old.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            assert sha(b[k])==b[k+'_sha256'];dlls[Path(v).name]=b[k+'_sha256']
            if k not in changed:assert sha(v)==sha(b[k]);preserved[Path(v).name]=sha(v)
            else:assert sha(v)!=sha(b[k])
    for name in dlls:
        run=subprocess.run([str(reader),'--coff-imports',str(build/name)],capture_output=True)
        assert not run.returncode and not run.stderr
        imports[name]=[n.strip().lower() for n in re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)]
        if name in ('jp_native_history.dll','jp_native_exit.dll','jp_native_system.dll'):texts[name]=run.stdout
    assert {'jp_native_flash.dll','jp_native_frames.dll','jp_native_rtc.dll'}<=set(imports['jp_native_history.dll'])
    assert not {'jp_native_system.dll','jp_native_exit.dll','jp_native_main_boot.dll'}&set(imports['jp_native_history.dll'])
    assert not {'jp_native_system.dll','jp_native_main_boot.dll'}&set(imports['jp_native_exit.dll'])
    assert 'jp_native_history.dll' in imports['jp_native_exit.dll'] and 'jp_native_history.dll' in imports['jp_native_system.dll']
    # Traverse the actual complete PE graph, including all retained providers.
    complete=set()
    def visit(name,stack):
        assert name not in stack,('native import cycle',stack,name)
        if name in complete:return
        for dep in imports[name]:
            if dep in imports:visit(dep,stack+[name])
        complete.add(name)
    for name in imports:visit(name,[])
    assert len(preserved)==15
    assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:6])
    previous=p/'analysis/jp_native_history_package.json';seen=set();prior(r,previous,seen)
    assert len(seen)==20
    d.mkdir()
    for name,raw in texts.items():(d/(name+'_imports.txt')).write_bytes(raw)
    out=dict(status='same_System_registration_History_authority_order_and_acyclic_PE_graph_audited',
        build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),
        unchanged_prior_DLLs=preserved,unchanged_prior_objects=preserved_objects,unchanged_history_source_units=preserved_sources,
        unchanged_original_owner_sources={k:str(v) for k,v in owner_paths.items()},
        actual_PE_imports=imports,original_bootstrap_callback_body_unchanged=True,single_System_bootstrap_registration=True,
        actual_claim_activity_precedes_terminal_mutation=True,card_history_sound_core_close_order_verified=True,
        same_History_token_transferred_after_all_final_claims_succeed=True,
        prior_qualification_packages_checked=len(seen),prior_package_sha256=sha(previous),
        exhausted_claim_counter_or_failed_rollback_cancellation_verified=False,
        full_game_init_executed=False,combined_game_SDK_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n')
    print(out['status'],len(preserved),'unchanged DLLs;',len(preserved_objects),'unchanged objects;',len(seen),'preserved packages',flush=True)
if __name__=='__main__':main()
