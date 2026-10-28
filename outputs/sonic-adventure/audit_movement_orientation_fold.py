"""Bind the new arithmetic analysis to the unchanged compiled source closure."""
import ast,hashlib,json,re,subprocess,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
BASE='06161710734a2a2603b9acc523e1dc8c4c6179bb'
G=r'C:\Program Files\Git\cmd\git.exe'

def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])

def main():
    certificate=P/'analysis/movement_orientation_fold_certificate.json'
    proof=json.loads(certificate.read_text())
    for path,expected in proof['sha256'].items():assert digest(path)==expected,path
    bridge=P/'source/sa_movement_orientation_response_connected.c';code=bridge.read_text()
    routes={0x8c10d0f8:'sa_collision_angle_8c10d0f8',0x8c10e6a0:'sa_collision_angle_8c10e6a0',
      0x8c10fb94:'sa_collision_angle_8c10fb94',0x8c10fbb8:'sa_collision_angle_8c10fbb8',
      0x8c10ef84:'sa_frame_setup_8c10ef84',0x8c10e7b8:'sa_frame_setup_8c10e7b8'}
    for address,name in routes.items():assert code.count(f'if(target==0x{address:08x}u)return {name}(')==1,name
    # Pin the code that was compiled into the tested DLL, rather than just
    # identifying a similarly named standalone math implementation.
    prior=P/'analysis/movement_orientation_response_unbounded.json';old=json.loads(prior.read_text())
    sources=[P/'source'/n for n in re.findall(r"'([^']+\.c)'",(P/'build_movement_orientation_response.ps1').read_text())]
    assert len(sources)==len(set(sources))==259
    assert {str(p):digest(p) for p in sources}==old['source_sha256']
    dll=ROOT/'work/sonic-adventure/build/movement_orientation_response.dll'
    assert digest(dll)=='0ef9b1bc4950c1f12697df0d60e908657dd2007c797fb325c6adcaaf6a95414f'
    flags=['-fsyntax-only','-Wall','-Wextra','-Werror']
    subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe',*flags,*map(str,sources)],check=True)
    # Never change historical proofs to make the new qualification fit.
    manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(manifest['files'])==16221
    mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in manifest['files']:
        if row['path'] not in mutable:
            path=ROOT/row['path'];assert path.stat().st_size==row['bytes'] and digest(path)==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(historical('outputs/sonic-adventure/'+name))
    old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))
    now=json.loads((P/'analysis/recovered_routines.json').read_text())
    assert len(old_registry['routines'])==1612 and now['routines'][:1612]==old_registry['routines']
    for key,value in old_registry.items():
        if key!='routines':assert now[key]==value
    old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now_status=json.loads((P/'project_status.json').read_text())
    for key,value in old_status.items():
        if key=='verified_progress':
            for field,data in value.items():
                if field!='recovered_buildable_routines':assert now_status[key][field]==data,field
        elif key!='recovered_buildable_routines':assert now_status[key]==value,key
    bp=P/'analysis/movement_orientation_fold_previous_build_manifest.json';builds=json.loads(bp.read_text())
    assert len(builds['files'])==2753
    for row in builds['files']:
        path=ROOT/row['path'];assert path.stat().st_size==row['bytes'] and digest(path)==row['sha256'],row['path']
    scripts=[P/n for n in ('movement_orientation_fold_intervals.py','prove_movement_orientation_fold.py','verify_movement_orientation_fold.py','audit_movement_orientation_fold.py','qualify_movement_orientation_fold.py','register_movement_orientation_fold.py')]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    report=dict(status='source_binding_strict_compile_and_preservation_passed',baseline_commit=BASE,
      compiled_source_count=259,native_sources_changed=0,dll_rebuilt=False,dll_sha256=digest(dll),
      compiler_flags=flags,compiler_exit_code=0,actual_native_routes={hex(a):s for a,s in routes.items()},
      preserved_previous_selected_files_outside_four_mutables=16217,preserved_prior_registry_rows=1612,
      preserved_archived_builds=2753,all_original_fold_code_retained=True,
      old_goal_requirements_and_progress_fields_preserved=True,python_scripts_parsed=len(scripts),
      sha256={str(p):digest(p) for p in [*scripts,certificate,prior,bridge,bp,dll]},
      unchanged_compiled_source_sha256=old['source_sha256'])
    (P/'analysis/movement_orientation_fold_audit.json').write_text(json.dumps(report,indent=2)+'\n')
    print('FOLD AUDIT PASS259strictcompiler/16217oldfiles/1612oldregistryrows/2753oldbuilds',flush=True)
if __name__=='__main__':main()
