"""Reject an already claimed History handoff before acquiring System resources."""
import argparse,ast,hashlib,json,re,shutil,subprocess,sys
from pathlib import Path
from build_native_texture_runtime import function
from package_native_sound_placement import prior

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def verifier(r,d):
    original=r/'outputs/sonic-adventure/jp_port_tools/verify_native_history_exit.py'
    raw=original.read_bytes();before=b"            other=Q(0xabcdef);"
    assert raw.count(before)==1
    extra=b'''            before_system=bytes(si());before_claim=bytes(claim())
            check(bind(system,'jp_system_main_initialization_guard',I,U)(1)==0,'explicit guard for foreign-claim handoff rejection')
            rejected=bind(system,'jp_system_adopt_history',I)()
            check(rejected==2 and bytes(si())==before_system and bytes(claim())==before_claim,'already claimed history rejects adoption without changing System resources or existing authority')
            check(bind(system,'jp_system_main_initialization_guard',I,U)(0)==0,'rejected foreign-claim handoff leaves constructor guard releasable')
'''
    # Snapshot helpers add four real checks; explicit assertions add three.
    raw=raw.replace(before,extra+before)
    prefix=('import sys\nsys.path.insert(0,'+repr(str(original.parent))+')\n').encode()
    path=d/'verify_history_claimed_handoff.py';assert not path.exists();path.write_bytes(prefix+raw)
    return path,sha(original)

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--stage',choices=('diagnose','build','check','package'),required=True)
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('build','track','check'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();base=a.baseline.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    old=read(base/'build_verification.json')
    for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    if a.stage in ('diagnose','check'):
        script,vhash=verifier(r,d);build=a.build.resolve() if a.build else base
        cmd=[sys.executable,'-B',str(script),'--workspace',str(r),'--build',str(build),'--track',str(a.track.resolve()),'--output',str(d/'runtime')]
        if a.stage=='diagnose':cmd+=['--case','claim_cancel']
        run=subprocess.run(cmd,capture_output=True,timeout=240,creationflags=subprocess.CREATE_NO_WINDOW);(d/'runtime.log').write_bytes(run.stdout+run.stderr)
        print((run.stdout+run.stderr).decode(errors='replace')[-5000:],flush=True)
        if a.stage=='diagnose':
            assert run.returncode!=0 and b'already claimed history rejects adoption' in run.stderr
            (d/'diagnosis.json').write_text(json.dumps(dict(status='claimed_handoff_rejection_mutates_System_state_reproduced',exit_code=run.returncode,
                baseline_sha256=sha(base/'build_verification.json'),test_sha256=sha(script),original_verifier_sha256=vhash),indent=2)+'\n');return
        assert not run.returncode
        out=read(d/'runtime/verification.json');out['derived_verifier_sha256']=out.pop('verifier_sha256')
        out.update(verifier_sha256=sha(__file__),original_verifier_sha256=vhash,baseline_sha256=sha(base/'build_verification.json'),
            private_derived_verifier_sha256=sha(script),status='claimed_handoff_guard_and_complete_History_System_Exit_suite_verified')
        assert out['checks']==753 and len(out['cases'])==20
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');return
    if a.stage=='build':
        for path in base.iterdir():
            if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
            elif path.is_dir() and path.name=='history_include':shutil.copytree(path,d/path.name)
        source=base/'system_with_history.c';raw=source.read_bytes();body=function(raw,'system_history_adopt')
        before=b'    /* Retain cleanup responsibility'
        guard=b'    struct jp_history_exit_info claim;\n    rc=jp_history_exit_snapshot(&claim);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_HISTORY,rc);\n    if(claim.token||claim.terminal)return JP_SYSTEM_STATE;\n'
        assert body.count(before)==1;raw=raw.replace(body,body.replace(before,guard+before))
        target=d/'system_with_history_adoption_guard.c';target.write_bytes(raw);obj=d/'system_history_adoption_owner.obj'
        compile_cmd=list(old['commands'][4]['command']);assert '-c' in compile_cmd
        compile_cmd[compile_cmd.index('-c')+1]=str(target);compile_cmd[-1]=str(obj)
        link=[]
        for v in old['commands'][5]['command']:
            if v.endswith('.obj') and Path(v).name=='system_history_owner.obj':link.append(str(obj))
            elif v.endswith(('.obj','.lib')) and (d/Path(v).name).exists():link.append(str(d/Path(v).name))
            else:link.append(v)
        link[-1]=str(d/Path(old['System_DLL']).name);commands=[]
        for i,cmd in enumerate((compile_cmd,link)):
            run=subprocess.run(cmd,capture_output=True);log=d/f'adoption_guard_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
            commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
            (d/'compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
        updates={}
        for k,v in old.items():
            if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
        updates.update(status='preexisting_History_claim_guard_compiled_unverified',history_adoption_guard_baseline_sha256=sha(base/'build_verification.json'),
            history_adoption_guard_generated_source_sha256=sha(target),history_adoption_guard_prior_System_source_sha256=sha(source),
            commands=commands+old['commands'],authored_source_sha256=dict(old['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}))
        (d/'build_verification.json').write_text(json.dumps(dict(old,**updates),indent=2)+'\n');print(updates['status'],flush=True);return
    assert a.stage=='package';build=a.build.resolve();b=read(build/'build_verification.json');check=read(a.check/'verification.json')
    assert check['build_verification_sha256']==sha(build/'build_verification.json') and check['verifier_sha256']==sha(__file__)
    assert b['history_adoption_guard_baseline_sha256']==sha(base/'build_verification.json')
    assert check['checks']==753 and check['original_verifier_sha256']==sha(p/'jp_port_tools/verify_native_history_exit.py')
    raw=(build/'system_with_history_adoption_guard.c').read_bytes();body=function(raw,'system_history_adopt')
    assert sha(build/'system_with_history_adoption_guard.c')==b['history_adoption_guard_generated_source_sha256']
    assert body.index(b'if(claim.token||claim.terminal)')<body.index(b'system_owner.info.resources|=')<body.index(b'jp_history_exit_claim(')
    preserved={}
    for k,v in old.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            assert sha(b[k])==b[k+'_sha256']
            if k!='System_DLL':assert sha(v)==sha(b[k]);preserved[Path(v).name]=sha(v)
    assert len(preserved)==17
    reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe');imports=[]
    for path in (old['System_DLL'],b['System_DLL']):
        run=subprocess.run([str(reader),'--coff-imports',path],capture_output=True)
        assert not run.returncode and not run.stderr
        imports.append(re.findall(rb'^\s+(?:Name|Symbol): (.+)$',run.stdout,re.M))
    assert imports[0]==imports[1],'System imports changed unexpectedly'
    assert all(sha(path)==sha(build/path.name) for path in base.glob('*.obj'))
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:2])
    previous=p/'analysis/jp_native_history_exit_package.json';seen=set();prior(r,previous,seen);assert len(seen)==21
    tool=Path(__file__).resolve();guide=p/'JP_NATIVE_HISTORY_ADOPTION_GUARD.txt';ast.parse(tool.read_text())
    evidence={'build':build/'build_verification.json','check':a.check/'verification.json'};proof={}
    targets={n:p/'analysis'/('jp_native_history_adoption_guard_'+n+'.json') for n in evidence}
    package=p/'analysis/jp_native_history_adoption_guard_package.json';assert not package.exists() and all(not v.exists() for v in targets.values())
    for n,path in targets.items():shutil.copyfile(evidence[n],path);proof[path.relative_to(r).as_posix()]=sha(path)
    out=dict(status='already_claimed_History_handoff_rejected_before_System_mutation_scoped_qualification',
        new_checks=7,selected_regression_checks=746,combined_qualified_scoped_checks=943486,
        preexisting_claim_rejection_and_releasable_outer_guard_verified=True,unchanged_prior_DLLs=preserved,
        System_PE_imports_unchanged=True,
        original_source_and_previous_proofs_preserved=True,build_verification_sha256=sha(build/'build_verification.json'),
        private_System_DLL_sha256=b['System_DLL_sha256'],
        prior_qualifications_preserved={'history_exit':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in (tool,guide)},proof_files_sha256=proof,
        public_research_build_payloads_changed=False,combined_game_SDK_binaries_publishable=False,
        full_game_init_executed=False,playable=False,overall_completion_percent=10,goal_active=True)
    target=d/'qualification.json';target.write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(target,package)
    print(out['status'],'7 new checks; 746 regressions; 943486 combined scopes',flush=True)
if __name__=='__main__':main()
