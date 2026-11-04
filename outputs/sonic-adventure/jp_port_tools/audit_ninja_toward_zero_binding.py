"""Replace real Ninja state/math and multiplication objects in the frame link.
This is not a migration of the external renderer's inline SH4ZAM arithmetic.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','component','compiler','output']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();c=a.component.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    prior=json.loads(a.baseline_audit.read_text());build=json.loads((c/'build_verification.json').read_text())
    assert build['exit_code']==build['diagnostic_bytes']==0 and sha(a.compiler)==build['compiler_sha256']
    assert build['Ninja_matrix_numerical_mode']=='toward_zero_DN1_exceptions_disabled'
    reports={}
    for name in ['ninja_verification','multiplication_verification','inverse_verification','shared_state_verification']:
        path=c/(name+'.json');report=json.loads(path.read_text());assert report['DLL_sha256']==build['DLL_sha256']
        reports[str(path)]=sha(path)
    assert json.loads((c/'ninja_verification.json').read_text())['original_instruction_contract_comparisons']==6786
    assert json.loads((c/'multiplication_verification.json').read_text())['original_instruction_contract_comparisons']==2268
    assert json.loads((c/'inverse_verification.json').read_text())['original_instruction_contract_comparisons']==1944
    assert json.loads((c/'shared_state_verification.json').read_text())['checks_passed']==961
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for o,h in objects.items():assert sha(o)==h,o
    replacements={}
    for old,new in [('math.obj','math_rz.obj'),('multi_matrix.obj','multi_matrix_rz.obj')]:
        matches=[path for path in objects if path.name==old];assert len(matches)==1
        replacement=c/new;assert sha(replacement)==build['production_objects_sha256'][new]
        replacements[matches[0]]=replacement
    for n,h in build['existing_provider_objects_sha256'].items():assert objects[Path(n).resolve()]==h
    current={replacements.get(o,o):sha(replacements.get(o,o)) for o in objects}
    assert len(current)==len(objects) and not set(map(Path,build['diagnostic_probes_sha256']))&set(current)
    cmd=list(prior['command']);assert Path(cmd[0]).resolve()==a.compiler.resolve()
    for i,value in enumerate(cmd):
        if value.endswith('.obj') and (r/value).resolve() in replacements:cmd[i]=str(replacements[(r/value).resolve()])
    assert {(r/v).resolve() for v in cmd if v.endswith('.obj')}==set(current)
    symbols={o:coff_symbols(o) for o in current}
    def owners(name):return [o for o,rows in symbols.items() if any(v['name']==name and v['section']>0 for v in rows)]
    assert owners('jp_ninja_current_matrix_storage')==[c/'math_rz.obj']
    assert len(owners('shz_xmtrx_state_'))==1
    assert owners('njMultiMatrix')==[c/'multi_matrix_rz.obj']
    assert len(owners('njInvertMatrix'))==1 and len(owners('jp_rz_scalar32_multiply'))==1
    assert not owners('jp_scalar32_multiply')
    d.mkdir(parents=True);cmd[-1]=str(d/'production_unfinished.dll')
    run=subprocess.run(cmd,capture_output=True,cwd=r);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text();remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and not (d/'production_unfinished.dll').exists()
    assert remaining==prior['combined_remaining_imports']
    errors=[line for line in text.splitlines() if 'error:' in line]
    assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    report=dict(prior)
    report.update(status='real_Ninja_RZ_matrix_family_binding_verified_renderer_inline_arithmetic_unmigrated',command=cmd,
        linked_objects_sha256={str(o):h for o,h in current.items()},
        replaced_objects={str(o):dict(old_sha256=objects[o],replacement=str(n),new_sha256=current[n]) for o,n in replacements.items()},
        baseline_audit_sha256=sha(a.baseline_audit),component_build_report_sha256=sha(c/'build_verification.json'),
        component_verification_reports_sha256=reports,verifier_sha256=sha(Path(__file__)),link_exit_code=run.returncode,
        log_sha256=sha(log),newly_resolved_services=[],numerical_mode='Ninja_matrix_family_toward_zero_DN1_renderer_inline_arithmetic_unmigrated',
        full_frame_executed=False,goal_paused=False,overall_completion_percent=10,playable=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Real Ninja state/math and multiplication replacements verified; single owner; remaining',len(remaining),'services unchanged.')

if __name__=='__main__':main()
