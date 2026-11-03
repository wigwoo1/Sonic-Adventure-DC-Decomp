"""Link real renderer inputs and report remaining services; never supply stubs."""
import argparse, hashlib, json, re, subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','queue','renderer','matrix','compiler','output']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    previous=json.loads((r/'outputs/sonic-adventure/analysis/jp_renderer_shared_matrix_link_audit.json').read_text())
    obj=a.renderer/'model.obj';matrix=a.matrix/'jp_shared_matrix.lib';queue=a.queue/'queue.obj'
    assert sha(obj)==previous['renderer_object_sha256']
    assert sha(matrix)==previous['shared_matrix_import_library_sha256']
    qb=json.loads((a.queue/'build_verification.json').read_text())
    assert sha(queue)==qb['artifacts_sha256']['queue.obj']
    for name,h in qb['authored_source_sha256'].items():assert sha(r/name)==h
    d.mkdir(parents=True)
    command=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
             '-Wl,/errorlimit:0',str(obj),str(queue),str(matrix),'-o',str(d/'renderer_unfinished.dll')]
    run=subprocess.run(command,capture_output=True)
    log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text()
    remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    resolved=sorted(set(previous['remaining_imports'])-set(remaining))
    expected=['_builtin_prefetch','_nj_parameterkm_','njEndVertex','njGetkmPointer',
              'njSetkmModelAttr','njSetkmPointer','njStartVertexNonTexOpaque',
              'njStartVertexNonTexTrans','njStartVertexTexOpaque','njStartVertexTexTrans']
    assert run.returncode!=0 and not (d/'renderer_unfinished.dll').exists()
    assert resolved==sorted(expected) and len(remaining)==20
    assert set(remaining)<=set(previous['remaining_imports'])
    # Reject crashes or additional linker failures beyond expected unresolved services.
    errors=[line for line in text.splitlines() if 'error:' in line]
    assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    report=dict(status='queue_services_resolved_remaining_renderer_services_inventoried',command=command,
                link_exit_code=run.returncode,log_sha256=sha(log),
                renderer_object_sha256=sha(obj),queue_object_sha256=sha(queue),
                shared_matrix_import_library_sha256=sha(matrix),
                newly_resolved_imports=resolved,remaining_imports=remaining,
                remaining_import_count=len(remaining),full_renderer_linked=False,
                dummy_graphics_texture_or_presentation_functions=False,
                verifier_sha256=sha(Path(__file__)),overall_completion_percent=10,
                goal_paused=False,playable=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Ten real queue imports resolved;',len(remaining),'services remain. Complete link intentionally fails.')

if __name__=='__main__':main()
