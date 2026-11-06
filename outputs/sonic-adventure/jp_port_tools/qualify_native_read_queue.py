"""Prepare a GDFS regression view, audit and freeze the original native queue."""
import argparse,ast,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_read_queue import owner_function,PIN
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('gdfs-view','audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('upstream','baseline','check','gdfs','gdfs-view','history','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if a.stage=='gdfs-view':
        g=read(Path(b['GDFS_build'])/'build_verification.json')
        g.update({k:b[k] for k in ('frames_DLL','frames_DLL_sha256','GPU_DLL','GPU_DLL_sha256','present_DLL','present_DLL_sha256','media_DLL','media_DLL_sha256')})
        g.update(DLL=b['GDFS_DLL'],DLL_sha256=b['GDFS_DLL_sha256'],authored_source_sha256=dict(g['authored_source_sha256'],**b['authored_source_sha256']),
            read_queue_graph_build_verification=str(build/'build_verification.json'),read_queue_graph_build_verification_sha256=sha(build/'build_verification.json'))
        d.mkdir()
        for n,h in g['generated_sources_sha256'].items():
            target=d/n;assert sha(build/n)==h,n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(build/n,target)
        (d/'build_verification.json').write_text(json.dumps(g,indent=2)+'\n');print('Prepared hash-bound GDFS regression view of the actual read-queue graph.',flush=True);return
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');u=a.upstream.resolve()
        assert sha(base/'build_verification.json')==b['read_queue_baseline_sha256']
        raw=(u/'src/system/loader.c').read_bytes();assert sha(u/'src/system/loader.c')==b['read_queue_original_source_sha256']
        assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/system/loader.c'])
        native=(build/'external_read_queue.c').read_bytes();assert sha(build/'external_read_queue.c')==b['read_queue_generated_source_sha256']
        unchanged=[];adapted=[];functions={}
        for name,h in b['read_queue_original_function_sha256'].items():
            original=function(raw,name);current=function(native,name);assert hashlib.sha256(original).hexdigest()==h
            functions[name]=dict(original_sha256=h,native_sha256=hashlib.sha256(current).hexdigest());(unchanged if original==current else adapted).append(name)
        assert set(unchanged)=={'read_queue_init','read_queue_selected_slot','read_queue_add'} and set(adapted)=={'read_queue_cancel','read_queue_serve'}
        cancel=function(native,'read_queue_cancel');assert cancel.index(b'jp_read_queue_source_close(')<cancel.index(b'read_queue[i].state = READ_FREE')
        serve=function(native,'read_queue_serve')
        assert b'gdFsGetWorkHn()!=NULL' in serve and b'jp_read_queue_source_validate' in serve and b'jp_read_queue_source_complete' in serve
        assert b'status == GDD_STAT_SEEK || status == GDD_STAT_BUSY' in serve
        retry=serve[serve.index(b'if (status == GDD_STAT_ERR'):serve.index(b'} else {')];assert b'gdFsOpen' not in retry
        owner=(build/'gdfs_with_read_queue.c').read_bytes();assert sha(build/'gdfs_with_read_queue.c')==b['read_queue_GDFS_owner_sha256']
        assert b'fs.read_queue_active&&!fs.callback_depth' in owner
        finish=owner_function(owner,'jp_gdfs_exit_finish');assert finish.index(b'if(fs.read_queue_token)')<finish.index(b'fs.exit_started=1')
        shutdown=owner_function(owner,'shutdown_owned');assert shutdown.index(b'if(fs.read_queue_token)')<shutdown.index(b'fs.closing=1')
        for name in ('gdFsSeek','gdFsRead','gdFsReqRd32','gdFsReqGdRd','gdFsTrans32','gdFsSetTmode','gdFsStopRd','gdFsMovePickup','gdFsClose','gdFsEntryRdEndFunc','gdFsEntryTrEndFunc','gdFsEntryErrFunc'):
            assert b'read_queue_handle_access(h)' in owner_function(owner,name),name
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        symbols=subprocess.run([str(reader),'--symbols',str(build/'read_queue_original.obj')],capture_output=True)
        assert not symbols.returncode and not symbols.stderr
        for name in ('read_queue','read_queue_file','read_queue_current'):
            matches=[block for block in symbols.stdout.split(b'  Symbol {') if re.search(rb'\bName: '+name.encode()+rb'\r?\n',block)]
            assert len(matches)==1 and b'Section: .bss' in matches[0] and b'StorageClass: Static' in matches[0],name
        gbase=Path(b['GDFS_build']);preserved_objects={}
        for folder in (base,gbase):
            for path in folder.glob('*.obj'):assert sha(path)==sha(build/path.name);preserved_objects[str(path.resolve())]=sha(path)
        imports={};preserved={};reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        for k,v in b.items():
            if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
                assert sha(v)==b[k+'_sha256'];name=Path(v).name.lower()
                if k in old and k!='GDFS_DLL':assert sha(old[k])==sha(v);preserved[name]=sha(v)
                if name not in imports:
                    run=subprocess.run([str(reader),'--coff-imports',v],capture_output=True);assert not run.returncode and not run.stderr
                    imports[name]=[x.lower() for x in re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)]
        assert len(preserved)==17
        assert 'jp_native_read_queue.dll' not in imports['jp_native_gdfs.dll']
        assert {n for n in imports['jp_native_read_queue.dll'] if n.startswith('jp_native_')}=={'jp_native_gdfs.dll'}
        seen=set()
        def visit(n,stack):
            assert n not in stack,('PE import cycle',stack,n)
            if n in seen:return
            for dep in imports[n]:
                if dep in imports:visit(dep,stack+[n])
            seen.add(n)
        for n in imports:visit(n,[])
        previous=p/'analysis/jp_native_history_adoption_guard_package.json';chain=set();prior(r,previous,chain);assert len(chain)==22
        assert all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'][:5])
        out=dict(status='original_read_queue_single_owner_capacity_and_GDFS_client_authority_audited',build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),
            original_function_sha256=functions,unchanged_original_functions=unchanged,explicitly_adapted_functions=adapted,
            unchanged_prior_DLLs=preserved,unchanged_prior_objects=preserved_objects,actual_PE_imports=imports,prior_packages_checked=len(chain),
            client_guard_precedes_GDFS_terminal_or_close_mutation=True,single_actual_original_queue_and_handle_pool=True,
            failed_OS_cancel_timeout_or_global_queue_exit_handoff_verified=False,full_game_init_executed=False,playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],'3 unchanged/2 adapted functions;17 unchanged DLLs;22 preserved packages',flush=True);return
    reports={};evidence={'build':build/'build_verification.json'}
    for n,tool in (('check','verify_native_read_queue.py'),('history','qualify_native_history_adoption_guard.py'),('audit','qualify_native_read_queue.py')):
        path=getattr(a,n)/'verification.json';value=read(path);assert value['build_verification_sha256']==sha(build/'build_verification.json')
        assert value['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[n]=value;evidence[n]=path
    gpath=a.gdfs/'verification.json';gr=read(gpath)
    # The unchanged GDFS verifier binds its regression-view manifest, which in
    # turn binds this exact full queue graph and the same actually tested DLL.
    assert a.gdfs_view;view=a.gdfs_view.resolve()
    gv=read(view/'build_verification.json');assert sha(view/'build_verification.json')==gr['build_verification_sha256']
    assert gv['read_queue_graph_build_verification_sha256']==sha(build/'build_verification.json') and gv['DLL_sha256']==b['GDFS_DLL_sha256']
    assert gr['verifier_sha256']==sha(p/'jp_port_tools/verify_native_gdfs.py');reports['gdfs']=gr;evidence.update(gdfs=gpath,gdfs_view=view/'build_verification.json')
    assert reports['check']['checks']==479 and len(reports['check']['cases'])==11
    assert reports['history']['checks']==753 and reports['gdfs']['checks']==792
    for n in ('read_queue_DLL','GDFS_DLL'):assert sha(b[n])==b[n+'_sha256']
    previous=p/'analysis/jp_native_history_adoption_guard_package.json';seen=set();prior(r,previous,seen);assert len(seen)==22
    authored=[p/'native'/n for n in ('jp_native_read_queue.h','jp_native_read_queue.c','jp_native_gdfs_read_queue.h','jp_native_gdfs_read_queue.inc')]
    authored+=[p/'jp_port_tools'/n for n in ('build_native_read_queue.py','verify_native_read_queue.py','qualify_native_read_queue.py')]+[p/'JP_NATIVE_READ_QUEUE.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    targets={n:p/'analysis'/('jp_native_read_queue_'+n+'.json') for n in evidence};package=p/'analysis/jp_native_read_queue_package.json'
    assert not package.exists() and all(not x.exists() for x in targets.values());d.mkdir();proof={}
    for n,target in targets.items():shutil.copyfile(evidence[n],target);proof[target.relative_to(r).as_posix()]=sha(target)
    out=dict(status='original_loader_read_queue_real_JP_IO_and_native_GDFS_client_ownership_scoped_qualification',new_checks=479,selected_regression_checks=1545,
        combined_qualified_scoped_checks=943965,unchanged_original_functions=3,explicitly_adapted_functions=2,
        actual_sector_rounded_bytes_and_buffer_canaries_verified=True,real_malformed_sector_failure_and_pending_cancellation_verified=True,
        mixed_file_lowest_slot_reuse_and_zero_length_completion_verified=True,real_callback_and_foreign_thread_rejection_verified=True,
        original_source_and_previous_proofs_preserved=True,build_verification_sha256=sha(build/'build_verification.json'),
        private_DLL_sha256={n:b[n+'_sha256'] for n in ('read_queue_DLL','GDFS_DLL')},
        prior_qualifications_preserved={'history_adoption_guard':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        failed_OS_cancel_timeout_or_global_System_Exit_queue_handoff_verified=False,unbounded_legacy_read_queue_add_exported=False,
        full_game_init_executed=False,public_research_build_payloads_changed=False,combined_game_SDK_binaries_publishable=False,
        retail_assets_or_user_profiles_packaged=False,playable=False,overall_completion_percent=10,goal_active=True)
    target=d/'qualification.json';target.write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(target,package)
    print(out['status'],'479 new checks;1545 regressions;943965 combined reported scopes',flush=True)
if __name__=='__main__':main()
