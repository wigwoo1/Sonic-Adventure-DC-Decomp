"""Audit four genuine entry wrappers, native policies and whole-call evidence."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from package_native_sound_placement import prior
from build_native_read_queue import PIN,owner_function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','check','init','frames','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_sdk_motion_package.json';seen=set();prior(r,previous,seen);assert len(seen)==42
    old=read(a.baseline/'build_verification.json');b=read(a.build/'build_verification.json')
    bh=sha(a.build/'build_verification.json');recipe=b['simple_projection_recipe']
    assert b['simple_projection_baseline_sha256']==sha(a.baseline/'build_verification.json')==read(previous)['build_verification_sha256']
    for meta in (old,b):
        for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h
        for k,v in meta.items():
            if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
                assert sha(v)==meta[k+'_sha256']
                if k!='frames_DLL':assert b[k+'_sha256']==old[k+'_sha256']
    raw=(a.upstream/'sdk/ninja/njModelS.c').read_bytes()
    assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(a.upstream),'show',PIN+':sdk/ninja/njModelS.c'])
    assert hashlib.sha256(raw).hexdigest()==recipe['original_source_sha256']
    names=['njSimpleCalcModel_Loop'+str(i) for i in range(1,5)]
    prefix=b'#include "jp_native_simple_projection.h"\n#define calc_work jp_simple_calc_work\n'
    for i in range(1,5):
        prefix+=('static void loop'+str(i)+'_body(struct jp_simple_calc_work *w,NJS_MATRIX *m){(void)m;jp_simple_projection_loop(w,'+str(i)+');}\n').encode()
    generated=a.build/'original_simple_projection_entries.c'
    assert generated.read_bytes()==prefix+b'\n'.join(owner_function(raw,n) for n in names)
    assert sha(generated)==recipe['generated_entry_sha256']
    for n in names:assert hashlib.sha256(owner_function(raw,n)).hexdigest()==recipe['original_entry_sha256'][n]
    for n,h in b['simple_projection_kept_objects_sha256'].items():assert sha(n)==h
    for n,h in recipe['objects_sha256'].items():assert sha(n)==h
    def link(meta):return next(x['command'] for x in meta['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll')
    before={x for x in link(old) if x.endswith('.obj')};after={x for x in link(b) if x.endswith('.obj')}
    assert before<=after and after-before==set(recipe['objects_sha256'])
    for row in b['commands'][:5]:
        assert row['exit_code']==row['diagnostic_bytes']==0
        if 'response_file' in row:
            assert sha(row['response_file'])==row['response_file_sha256']
            assert shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
    assert recipe['layouts']['native']['sizeof']==64 and recipe['layouts']['original_LP32']['sizeof']==48
    reader=Path(link(b)[0]).with_name('llvm-readobj.exe')
    def pe(flag,path):
        x=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not x.returncode and not x.stderr
        return set(re.findall(r'^  Name: (.+)$',x.stdout.decode(),re.M))
    assert pe('--coff-exports',b['frames_DLL'])==pe('--coff-exports',old['frames_DLL'])|set(names)|{'jp_simple_project'}
    assert pe('--coff-imports',b['frames_DLL'])==pe('--coff-imports',old['frames_DLL'])
    reports={n:read(getattr(a,n)/'verification.json') for n in ('check','init','frames')}
    for n,count,tool in (('check',1554,'verify_native_simple_projection.py'),('init',530,'verify_native_game_init.py'),('frames',439,'verify_native_event_queue_frames.py')):
        row=reports[n];assert row['checks']==count and row['build_verification_sha256']==bh and row['verifier_sha256']==sha(p/'jp_port_tools'/tool)
    v=reports['check'];assert v['original_complete_calls']==192 and len(v['records'])==192
    assert {(x['kind'],x['count'],x['queue_parity'],x['bank'],x['matrix']) for x in v['records']}=={
        (k,c,q,b,m) for k in range(1,5) for c in (0,1,2,3,7,16) for q in (0,1) for b in (0,1) for m in (0,1)}
    assert all(x['expected_sha256']==x['actual_sha256'] for x in v['records'])
    assert v['actual_Windows_guard_page_proves_no_next_point_read'] and v['exact_output_and_both_persistent_queue_lines_compared']
    assert not v['original_game_function_hooks_used'] and not v['full_Simple_model_or_aura_or_gameplay_verified']
    f=reports['frames'];assert f['actual_window_presents']==10 and f['texture_Off_backend_bindings_and_uploads_zero']
    for row in f['screenshots']+f['rendered_original_action_frames']+[f['animated_preview']]:assert sha(row['path'])==row['sha256']
    d.mkdir()
    command=list(b['commands'][1]['command']);command[command.index('-c')]='-S'
    command.insert(command.index('-S'),'-emit-llvm');command[-1]=str(d/'native_projection.ll')
    x=subprocess.run(command,capture_output=True);assert not x.returncode and not x.stderr
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(d/'native_projection.ll').read_text())
    audit=dict(status='original_entries_both_ABIs_explicit_native_value_policies_and_whole_loop_evidence_audited',
        auditor_sha256=sha(__file__),build_verification_sha256=bh,previous_packages_verified=42,
        new_checks=1554,selected_regression_checks=969,native_projection_LLVM_sha256=sha(d/'native_projection.ll'),
        native_projection_residual_host_FP_operations=0,all_previous_objects_preserved=True,
        original_game_source_and_proofs_modified=False,full_Simple_model_or_aura_verified=False,playable=False)
    (d/'audit.json').write_text(json.dumps(audit,indent=2)+'\n')
    evidence={'build':a.build/'build_verification.json','check':a.check/'verification.json',
              'init':a.init/'verification.json','frames':a.frames/'verification.json','audit':d/'audit.json'}
    proof={}
    for n,path in evidence.items():
        to=p/'analysis'/('jp_native_simple_projection_'+n+'.json');assert not to.exists()
        shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    files=['native/jp_native_simple_projection.c','native/jp_native_simple_projection.h',
           'jp_port_tools/build_native_simple_projection.py','jp_port_tools/verify_native_simple_projection.py',
           'jp_port_tools/qualify_native_simple_projection.py','JP_NATIVE_SIMPLE_PROJECTION.txt']
    for n in files:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    out=dict(status='four_original_Simple_projection_entries_native_value_paths_scoped_qualification',
        new_checks=1554,selected_regression_checks=969,
        combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+1554,
        whole_original_projection_calls=192,original_entry_functions=4,
        both_SH4_banks_both_queue_parities_and_two_matrices=True,
        all_output_bytes_and_persistent_queue_words_checked=True,
        actual_guard_page_safety_test_passed=True,native_unused_speculative_reads_removed=True,
        native_QACR_cache_or_FPSCR_side_effects_emulated=False,
        full_Simple_model_aura_original_Sonic_callback_or_gameplay_verified=False,
        prior_qualifications_preserved={'sdk_motion':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in files},
        proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],
        public_research_build_payloads_changed=False,
        retail_generated_external_source_captures_or_combined_DLLs_publishable=False,
        original_source_and_proofs_preserved=True,playable=False,overall_completion_percent=10,goal_active=True)
    target=p/'analysis/jp_native_simple_projection_package.json';assert not target.exists()
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target)
    print(out['status'],'1554 new checks;969 regressions',flush=True)
if __name__=='__main__':main()
