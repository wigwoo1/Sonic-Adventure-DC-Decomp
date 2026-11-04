"""Audit real context/title-fade providers without duplicate extracted game state."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-audit','build','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();build=a.build.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    prior=read(a.baseline_audit);b=read(build/'build_verification.json');objects={Path(n).resolve():h for n,h in prior['linked_objects_sha256'].items()};command=list(prior['command'])
    for n,h in objects.items():assert sha(n)==h,n
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for name in ['context_bridge.obj','context_sdk.obj','game_title_fade.obj']:
        path=build/name;assert sha(path)==b['production_objects_sha256'][name];objects[path]=sha(path);command.insert(command.index('-Wl,kernel32.lib'),str(path))
    d.mkdir();original=next(Path(n) for n in prior['graphs']['expanded_boot']['command'] if n.endswith('original_logo_boot.obj'));assert sha(original)==prior['original_boot_object_sha256']
    raw=(original.parent/'external_original_logo_boot.c').read_bytes();removed={}
    for name in ['title_fade_set','title_fade_step','title_fade_draw']:
        body=function(raw,name);removed[name]=hashlib.sha256(body).hexdigest();raw=raw.replace(body,b'')
    assert raw.count(b'static int title_fade_level;')==1;raw=raw.replace(b'static int title_fade_level;',b'')
    source=d/'external_original_boot_shared_title_fade.c';source.write_bytes(raw);obj=d/'original_boot_shared_title_fade.obj';compile_cmd=list(b['commands'][0]['command']);compile_cmd=compile_cmd[:compile_cmd.index('-c')]+['-I'+str(original.parent.parent/'jp-native-logo-asset-probe-v2/inputs'),'-c',str(source),'-o',str(obj)]
    run=subprocess.run(compile_cmd,capture_output=True);log=d/'compile.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    results={}
    for name,extra,expected in [('render',[],prior['combined_remaining_imports']),('expanded_boot',[str(obj)],prior['graphs']['expanded_boot']['remaining_services'])]:
        cmd=list(command);cmd[cmd.index('-Wl,kernel32.lib'):cmd.index('-Wl,kernel32.lib')]=extra;cmd[-1]=str(d/(name+'_unfinished.dll'));run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/(name+'_link.txt');log.write_bytes(run.stdout+run.stderr);text=log.read_text();missing=sorted(set(re.findall(r'undefined symbol: (\S+)',text)))
        assert run.returncode!=0 and not Path(cmd[-1]).exists() and missing==expected,text[:6500]
        assert len([line for line in text.splitlines() if 'error:' in line])==len(missing)+1,text[:6500]
        results[name]=dict(command=cmd,exit_code=run.returncode,log_sha256=sha(log),remaining_services=missing,remaining_count=len(missing))
    report=dict(status='original_title_fade_and_Ninja_context_providers_production_and_shared_boot_state_link_audited',linked_objects_sha256={str(n):h for n,h in objects.items()},command=results['render']['command'],combined_remaining_imports=results['render']['remaining_services'],combined_remaining_import_count=results['render']['remaining_count'],expanded_boot_remaining_import_count=results['expanded_boot']['remaining_count'],graphs=results,original_boot_compile_command=compile_cmd,original_boot_object_sha256=sha(obj),original_boot_generated_source_sha256=sha(source),removed_duplicate_title_fade_bodies=removed,one_actual_game_title_fade_state_owner=True,satisfied_presentation_import_count=prior['satisfied_presentation_import_count'],new_real_providers=['njInitContext','njSetCurrentContext','njColorBlendingMode','kmProcessVertexRenderState','kmSetVertexRenderState','title_fade_set','title_fade_step','title_fade_draw','title_fade_draw_white'],test_fixture_or_dummy_services_linked=False,baseline_audit_sha256=sha(a.baseline_audit),build_report_sha256=sha(build/'build_verification.json'),verifier_sha256=sha(Path(__file__)),full_original_logo_step_or_boot_executed=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Real title-fade/context providers audited:',report['combined_remaining_import_count'],'render services and',report['expanded_boot_remaining_import_count'],'expanded boot services remain; no duplicated fade state.')
if __name__=='__main__':main()
