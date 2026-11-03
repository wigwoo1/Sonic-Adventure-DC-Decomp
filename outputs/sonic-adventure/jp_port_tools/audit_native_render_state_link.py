"""Check real composed objects against the complete model renderer."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','preflight','renderer','matrix','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    previous=json.loads((r/'outputs/sonic-adventure/analysis/jp_native_texture_renderer_link_audit.json').read_text())
    model=a.renderer/'model.obj';assert sha(model)==previous['renderer_object_sha256']
    build=json.loads((a.preflight/'build_verification.json').read_text())
    objects=[Path(n) for n in build['linked_objects_sha256']]
    for o in objects:assert sha(o)==build['linked_objects_sha256'][str(o)]
    preflight=a.preflight/'jp_model_preflight.obj';assert sha(preflight)==build['object_sha256']
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h
    matrix=a.matrix/'jp_shared_matrix.lib'
    old=json.loads((r/'outputs/sonic-adventure/analysis/jp_native_queue_renderer_link_audit.json').read_text())
    assert sha(matrix)==old['shared_matrix_import_library_sha256']
    d.mkdir(parents=True)
    cmd=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld','-Wl,/errorlimit:0',
         str(model),str(preflight),*map(str,objects),str(matrix),'-o',str(d/'renderer_unfinished.dll')]
    run=subprocess.run(cmd,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text();remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    expected=['event_skipping','frame_later_draws','frame_step','port_draw_aspect','port_view_aspect','present_record','present_untimed']
    assert remaining==expected and run.returncode!=0 and not (d/'renderer_unfinished.dll').exists()
    resolved=sorted(set(previous['remaining_imports'])-set(remaining));assert len(resolved)==10
    errors=[line for line in text.splitlines() if 'error:' in line]
    assert len(errors)==8 and errors[-1].startswith('clang: error: linker command failed')
    report=dict(status='ten_real_renderer_state_services_resolved_seven_frame_services_remain',command=cmd,
        link_exit_code=run.returncode,log_sha256=sha(log),renderer_object_sha256=sha(model),
        preflight_build_report_sha256=sha(a.preflight/'build_verification.json'),
        newly_resolved_imports=resolved,remaining_imports=remaining,remaining_import_count=7,
        full_renderer_linked_or_executed=False,dummy_frame_or_presentation_functions=False,
        verifier_sha256=sha(Path(__file__)),overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Ten real render-state imports resolved; seven frame/presentation services remain.')
if __name__=='__main__':main()
