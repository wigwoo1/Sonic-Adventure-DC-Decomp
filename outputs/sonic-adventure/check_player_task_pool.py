"""Check unchanged native source hashes and compile all268 sources strictly."""
import ast,hashlib,json,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    prior=P/'analysis/player_work_lifecycle_unbounded.json';old=json.loads(prior.read_text())
    assert old['source_count']==268 and old['exit_code']==0
    for path,expected in old['source_sha256'].items():assert sha(path)==expected,path
    build=P/'build_player_work_allocation.ps1';sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",build.read_text())]
    assert len(sources)==len(set(sources))==268
    flags=['-fsyntax-only','-Wall','-Wextra','-Werror']
    subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
    scripts=sorted(P.glob('*player_task_pool*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    dll=R/'work/sonic-adventure/build/player_work_allocation.dll'
    q=json.loads((P/'analysis/player_work_lifecycle_qualification.json').read_text())
    assert sha(dll)==q['dll_sha256']=='52fbdf679de18c9430390cb42d3cafae8df2157a9f91e58ebd198be244dae4ad'
    result=dict(status='unchanged_268_source_native_code_and_strict_compiler_pass',source_count=268,compiler_exit_code=0,compiler_flags=flags,
                runtime_code_changed=False,binary_rebuilt=False,dll_sha256=sha(dll),python_scripts_parsed=len(scripts),overall_completion_percent=10,
                source_sha256={str(p):sha(p) for p in sources},sha256={str(p):sha(p) for p in [Path(__file__),prior,build,dll,*scripts]})
    target=P/'analysis/player_task_pool_compile_checks.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('PLAYER TASK POOL STRICT268 PASS;native source and DLL unchanged;',len(scripts),'Python scripts parsed',flush=True)
if __name__=='__main__':main()
