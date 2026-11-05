"""Compile real main/boot bodies and identify their actual unresolved providers.

Private extracted game bodies are for dependency inspection only. No game
function, SDK service or indirect overlay call is replaced to make a link pass.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;b=read(base/'build_verification.json')
    assert sha(c)==b['compiler_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir()
    src=u/'src/main.c';raw=src.read_bytes();prefix=raw[:raw.index(b'void main(void)')]
    # Inspect PE exports without executing or initializing any native service.
    reader=c.with_name('llvm-readobj.exe');assert reader.exists();providers={}
    definitions={}
    for label in ['frames_DLL','GPU_DLL','present_DLL','authored_exit_DLL','DLL']:
        result=subprocess.run([str(reader),'--coff-exports',b[label]],capture_output=True);assert result.returncode==0 and not result.stderr
        providers[label]=set(re.findall(rb'^  Name: (\S+)$',result.stdout,re.M))
        link=next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b[label]) if label!='GPU_DLL' else []
        for obj in {Path(n) for n in link if n.endswith('.obj')}:
            for symbol in coff_symbols(obj):
                if symbol['section']>0:definitions.setdefault(symbol['name'],[]).append(dict(provider=label,object=str(obj),object_sha256=sha(obj)))
    def available(name):
        result=[]
        for label,names in providers.items():
            if name.encode() in names:result.append(label)
        return result
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    # This audit compiles the original Dreamcast void main signature and
    # extracted bodies with the same original global declarations. Unused
    # game_leaving in individual extracts is an inspection-only exception.
    common+=['-Wno-main-return-type','-Wno-unused-variable']
    results={}
    for name in ['main','game_init','system_boot_init','display_reset_full','game_mode_step']:
        body=function(raw,name);source=d/('external_'+name+'.c')
        # The externally callable setter must be present for main's private
        # game_leaving state to remain mutable across reentrant external calls.
        # Otherwise this isolated extract would falsely optimize away shutdown.
        source.write_bytes(prefix+(function(raw,'game_leave') if name=='main' else b'')+body)
        exceptions=['-Wno-return-mismatch'] if name=='game_mode_step' else []
        obj=d/(name+'.obj');cmd=common+exceptions+['-c',str(source),'-o',str(obj)];run=subprocess.run(cmd,capture_output=True);log=d/(name+'.txt');log.write_bytes(run.stdout+run.stderr)
        assert run.returncode==0 and not log.stat().st_size,(name,log.read_text()[:5000])
        symbols=coff_symbols(obj);undefined=sorted({s['name'] for s in symbols if s['section']==0})
        own={s['name'] for s in symbols if s['section']>0};known={n:available(n) for n in undefined};missing=sorted(n for n in undefined if not known[n])
        results[name]=dict(original_function_sha256=hashlib.sha256(body).hexdigest(),extracted_source_sha256=sha(source),object_sha256=sha(obj),command=cmd,exit_code=0,diagnostic_bytes=0,undefined_symbols=undefined,existing_export_providers={n:v for n,v in known.items() if v},missing_exports=missing,existing_object_definitions={n:definitions[n] for n in undefined if n in definitions},missing_compiled_definitions=sorted(n for n in undefined if not known[n] and n not in definitions),defined_external_symbols=sorted(own),indirect_overlay_calls_present=name=='game_mode_step',inspection_only_warning_exceptions=exceptions)
    diagnostic_fog={} if b.get('diagnostic_only_fog_setters_removed') else {'njSetFogColor':'present_imported_demo_state.c currently records event 5 without setting fog','njSetFogTable':'present_imported_demo_state.c currently records event 6 without setting fog'}
    result=dict(status='real_main_boot_dependency_frontier_compiled_without_stubs',baseline_build_sha256=sha(base/'build_verification.json'),original_source_sha256=sha(src),compiler_sha256=sha(c),audit_tool_sha256=sha(Path(__file__)),export_reader_sha256=sha(reader),upstream_commit='25a3d3154dbe657325e97dbe8c5291086f6af5c0',functions=results,diagnostic_fog_provider_definitions_requiring_replacement=diagnostic_fog,availability_is_not_runtime_qualification=True,inspection_warning_exceptions=['original void main signature','unused game_leaving in individual extracts','original game_mode_step bare return in nonvoid function'],original_bodies_rewritten=False,stubbed_services=False,links_or_runtime_behavior_verified=False,private_game_bodies_publishable=False,playable=False,overall_completion_percent=10)
    (d/'dependency_audit.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({n:v['missing_compiled_definitions'] for n,v in results.items()},indent=2),flush=True)
if __name__=='__main__':main()
