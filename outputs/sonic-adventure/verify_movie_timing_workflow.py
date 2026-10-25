"""Verify timing proof reproducibility, unchanged source/builds and registration."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    generated=[P/f'analysis/{name}.json' for name in ('movie_timing_static_verification','movie_timing_constructors_qualification')]
    before={str(p):digest(p) for p in generated}
    for name in ('verify_movie_timing_static.py','qualify_movie_timing_constructors.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert all(digest(p)==h for p,h in before.items())
    mutable=[P/'analysis/recovered_routines.json',P/'project_status.json'];old=[p.read_bytes() for p in mutable]
    subprocess.run([sys.executable,'-B',str(P/'register_movie_timing_constructors.py')],check=True)
    assert old==[p.read_bytes() for p in mutable]
    names=('verify_movie_timing_constructors.py','verify_movie_timing_static.py','qualify_movie_timing_constructors.py','register_movie_timing_constructors.py','verify_movie_timing_workflow.py')
    scripts=[P/name for name in names]
    for p in scripts:ast.parse(p.read_text())
    pins={}
    for name in ('movie_render_config_qualification','movie_timing_constructors_verification','movie_timing_static_verification','movie_timing_constructors_qualification'):
        for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():
            assert digest(path)==h,path
            if path in pins:assert pins[path]==h,path
            pins[path]=h
    baseline='55222ff04ce0579ecde41a8c0c4eac9dc7acf657';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
    def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
    manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
    for row in manifest['files']:
        path=row['path'];qualifies='/source/' in path or ('/analysis/' in path and any(x in Path(path).name for x in ('verification','qualification','translation','connection')))
        if not qualifies or path.endswith('/analysis/main_dispatch_case_map_verification.json'):continue
        assert digest(ROOT/path)==row['sha256'],path;preserved+=1
    previous=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines']
    current=json.loads(mutable[0].read_text())['routines'];lookup={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
    for row in previous:assert lookup[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]==row,row['address']
    with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
        build=json.loads(z.read('BUILD_MANIFEST.json'))
        for row in build['files']:
            assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path']),row['path']
    sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_render_config.ps1').read_text())
    assert len(sources)==len(set(sources))==158
    subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/name) for name in sources],check=True)
    report=dict(reproduced_generated_artifacts=len(generated),registration_byte_idempotent=True,python_scripts_parsed=len(scripts),
                pinned_inputs_current=len(pins),strict_unbounded_sources_checked=len(sources),preserved_baseline_commit=baseline,
                preserved_original_source_and_proof_files=preserved,preserved_original_registry_entries=len(previous),
                preserved_research_build_files=len(build['files']),newly_registered_entries=3,
                scope='Focused workflow only. All original C and DLL bytes remain unchanged. Strict C compilation checks run; the DLL used by new comparisons is the retained 158-source build. No full project rebuild or accumulated suite rerun is claimed.',
                sha256={str(p):digest(p) for p in scripts+[P/'build_movie_render_config.ps1']})
    (P/'analysis/movie_timing_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
