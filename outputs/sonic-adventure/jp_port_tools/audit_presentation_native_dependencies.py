"""Link complete production presentation/model objects to real native providers.
Never link the isolated test fixture or fill missing services with stubs.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','presentation','clock','preflight','renderer','matrix','compiler','output']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    sources=[];input_reports={}
    pb=json.loads((a.presentation/'build_verification.json').read_text())
    present=a.presentation/'present.obj';assert sha(present)==pb['object_sha256'];sources.append(present)
    for folder,fields in [(a.clock,'object_sha256'),(a.preflight,'linked_objects_sha256')]:
        path=folder/'build_verification.json';input_reports[str(path)]=sha(path);b=json.loads(path.read_text())
        for name,h in b[fields].items():
            obj=(folder/name) if fields=='object_sha256' else Path(name)
            assert sha(obj)==h;sources.append(obj)
    preflight=a.preflight/'jp_model_preflight.obj'
    b=json.loads((a.preflight/'build_verification.json').read_text());assert sha(preflight)==b['object_sha256'];sources.append(preflight)
    renderer=a.renderer/'model.obj'
    old=json.loads((r/'outputs/sonic-adventure/analysis/jp_render_state_link_audit.json').read_text())
    assert sha(renderer)==old['renderer_object_sha256'];sources.append(renderer)
    matrix=a.matrix/'jp_shared_matrix.lib'
    old=json.loads((r/'outputs/sonic-adventure/analysis/jp_native_queue_renderer_link_audit.json').read_text())
    assert sha(matrix)==old['shared_matrix_import_library_sha256']
    assert all('probe' not in str(s) for s in sources)
    d.mkdir(parents=True)
    cmd=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld','-Wl,/errorlimit:0',
         *map(str,sources),str(matrix),'-Wl,kernel32.lib','-o',str(d/'presentation_unfinished.dll')]
    run=subprocess.run(cmd,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    text=log.read_text();remaining=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
    assert run.returncode!=0 and remaining and not (d/'presentation_unfinished.dll').exists()
    assert all(n not in remaining for n in ['jp_clock_read_tstr','jp_clock_write_tstr','syTmrGetCount','present_record','present_untimed'])
    errors=[line for line in text.splitlines() if 'error:' in line]
    assert len(errors)==len(remaining)+1 and errors[-1].startswith('clang: error: linker command failed')
    satisfied=sorted(set(pb['undefined_symbols'])-set(remaining))
    report=dict(status='complete_presentation_real_native_dependency_frontier_verified',command=cmd,
        link_exit_code=run.returncode,log_sha256=sha(log),production_presentation_object_sha256=sha(present),
        production_presentation_original_imports=len(pb['undefined_symbols']),
        satisfied_presentation_imports=satisfied,satisfied_presentation_import_count=len(satisfied),
        combined_remaining_imports=remaining,combined_remaining_import_count=len(remaining),
        linked_objects_sha256={str(s):sha(s) for s in sources},input_build_reports_sha256=input_reports,
        full_presentation_linked_or_executed=False,test_fixture_or_dummy_services_linked=False,
        renderer_frame_services_implemented_by_real_source=['present_record','present_untimed','frame_later_draws'],
        verifier_sha256=sha(Path(__file__)),overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Production presentation imports:',len(pb['undefined_symbols']),'; satisfied:',len(satisfied),'; combined remaining:',len(remaining))
if __name__=='__main__':main()
