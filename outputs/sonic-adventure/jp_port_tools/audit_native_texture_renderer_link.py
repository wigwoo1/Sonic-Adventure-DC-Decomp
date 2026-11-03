"""Audit real complete-renderer link with texture ownership objects; no stubs."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','textures','renderer','matrix','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    previous=json.loads((r/'outputs/sonic-adventure/analysis/jp_native_queue_renderer_link_audit.json').read_text())
    obj=a.renderer/'model.obj';matrix=a.matrix/'jp_shared_matrix.lib'
    assert sha(obj)==previous['renderer_object_sha256']
    assert sha(matrix)==previous['shared_matrix_import_library_sha256']
    build=json.loads((a.textures/'build_verification.json').read_text())
    objects=[a.textures/n for n in build['object_sha256']]
    for path in objects:assert sha(path)==build['object_sha256'][path.name]
    queue=Path(build['linked_queue_object']);assert sha(queue)==build['linked_queue_object_sha256']
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h
    d.mkdir(parents=True)
    command=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
        '-Wl,/errorlimit:0',str(obj),*map(str,objects),str(queue),str(matrix),'-o',str(d/'renderer_unfinished.dll')]
    run=subprocess.run(command,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text();remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    resolved=sorted(set(previous['remaining_imports'])-set(remaining))
    assert resolved==['_nj_current_texlist','_nj_current_texture','jp_renderer_texture_lookup']
    assert len(remaining)==17 and set(remaining)<=set(previous['remaining_imports'])
    assert run.returncode!=0 and not (d/'renderer_unfinished.dll').exists()
    errors=[line for line in text.splitlines() if 'error:' in line]
    assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    report=dict(status='native_texture_ownership_imports_resolved_remaining_services_inventoried',
        command=command,link_exit_code=run.returncode,log_sha256=sha(log),
        renderer_object_sha256=sha(obj),texture_build_report_sha256=sha(a.textures/'build_verification.json'),
        newly_resolved_imports=resolved,remaining_imports=remaining,remaining_import_count=17,
        full_renderer_linked=False,dummy_graphics_texture_or_presentation_functions=False,
        verifier_sha256=sha(Path(__file__)),overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Three real texture imports resolved; 17 services remain. Complete link still fails as expected.')
if __name__=='__main__':main()
