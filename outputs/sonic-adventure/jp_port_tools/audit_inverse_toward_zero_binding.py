"""Replace exactly the two nearest inverse/scalar objects in the real link audit.
No test DLL/probe, second matrix owner or placeholder enters production inputs.
The rest of the renderer is explicitly still awaiting numerical migration.
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
    contracts=json.loads((c/'contract_verification.json').read_text())
    assert build['exit_code']==build['diagnostic_bytes']==0 and contracts['context_mismatch_corrected']
    assert contracts['build_report_sha256']==sha(c/'build_verification.json')
    assert sha(a.compiler)==build['compiler_sha256']
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()}
    for obj,h in objects.items():assert sha(obj)==h,obj
    replacements={}
    for old,new in [('inverse_matrix.obj','inverse_matrix_rz.obj'),('scalar32.obj','scalar32_rz.obj')]:
        matches=[path for path in objects if path.name==old];assert len(matches)==1
        replacement=c/new;assert sha(replacement)==build['production_objects_sha256'][new]
        replacements[matches[0]]=replacement
    for n,h in build['existing_provider_objects_sha256'].items():assert objects[Path(n).resolve()]==h
    current={replacements.get(obj,obj):sha(replacements.get(obj,obj)) for obj in objects}
    assert len(current)==len(objects) and not set(map(Path,build['diagnostic_probes_sha256']))&set(current)
    cmd=list(prior['command']);assert Path(cmd[0]).resolve()==a.compiler.resolve()
    for index,value in enumerate(cmd):
        if value.endswith('.obj') and (r/value).resolve() in replacements:cmd[index]=str(replacements[(r/value).resolve()])
    assert {(r/value).resolve() for value in cmd if value.endswith('.obj')}==set(current)
    symbols={obj:coff_symbols(obj) for obj in current}
    def owners(name):return [obj for obj,rows in symbols.items() if any(row['name']==name and row['section']>0 for row in rows)]
    assert len(owners('jp_ninja_current_matrix_storage'))==len(owners('shz_xmtrx_state_'))==1
    assert owners('njInvertMatrix')==[c/'inverse_matrix_rz.obj']
    assert all(owners('jp_rz_scalar32_'+name)==[c/'scalar32_rz.obj'] for name in ['add','subtract','multiply','divide'])
    assert not any(owners('jp_scalar32_'+name) for name in ['add','subtract','multiply','divide'])
    d.mkdir(parents=True);cmd[-1]=str(d/'production_unfinished.dll')
    run=subprocess.run(cmd,capture_output=True,cwd=r);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text();remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and not (d/'production_unfinished.dll').exists()
    assert remaining==prior['combined_remaining_imports']
    errors=[line for line in text.splitlines() if 'error:' in line]
    assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    report=dict(prior)
    report.update(status='actual_inverse_toward_zero_binding_verified_remaining_renderer_mode_unqualified',command=cmd,
                  linked_objects_sha256={str(o):h for o,h in current.items()},
                  replaced_objects={str(o):dict(old_sha256=objects[o],replacement=str(n),new_sha256=current[n]) for o,n in replacements.items()},
                  baseline_audit_sha256=sha(a.baseline_audit),component_build_report_sha256=sha(c/'build_verification.json'),
                  component_contract_report_sha256=sha(c/'contract_verification.json'),verifier_sha256=sha(Path(__file__)),
                  link_exit_code=run.returncode,log_sha256=sha(log),newly_resolved_services=[],
                  numerical_mode='inverse_and_determinant_toward_zero_DN1_other_components_not_migrated',
                  full_frame_executed=False,goal_paused=False,overall_completion_percent=10,playable=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Real inverse/scalar replacement verified; one matrix owner; remaining',len(remaining),'services unchanged. Other renderer arithmetic is not yet migrated.')

if __name__=='__main__':main()
