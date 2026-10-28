"""Strict compiler and preservation audit for the additive alias evidence."""
import ast
import hashlib
import json
import re
import subprocess
from pathlib import Path

P=Path(__file__).resolve().parent
ROOT=P.parents[1]
BASELINE='25a755292e66668e71a4a2bcac4612eb6800d554'
GIT=r'C:\Program Files\Git\cmd\git.exe'
MUTABLE={'outputs/sonic-adventure/'+name for name in ('README.md','SOURCE_HANDOFF.txt','project_status.json')}


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    checkout=ROOT/'publication/sonic-adventure'
    def historical(path):
        return json.loads(subprocess.check_output([GIT,'-C',str(checkout),'show',BASELINE+':'+path]))
    baseline=historical('PUBLICATION_MANIFEST.json')
    for row in baseline['files']:
        if row['path'] not in MUTABLE:
            assert digest(ROOT/row['path'])==row['sha256'],row['path']
    old=historical('outputs/sonic-adventure/project_status.json')
    current=json.loads((P/'project_status.json').read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():
                assert current[key][field]==data,field
        else:
            assert current[key]==value,key
    assert set(current['verified_progress'])-set(old['verified_progress'])=={'collision_contact_counter_alias'}
    prior_build=P/'analysis/collision_contact_counter_alias_prior_build_manifest.json'
    builds=json.loads(prior_build.read_text())
    assert len(builds['files'])==2733 and builds['playable_game'] is False
    for row in builds['files']:
        path=ROOT/row['path']
        assert path.stat().st_size==row['bytes'] and digest(path)==row['sha256'],str(path)
    paths=[P/name for name in ('verify_collision_contact_counter_alias.py',
      'qualify_collision_contact_counter_alias.py','record_collision_contact_counter_alias.py',
      'verify_collision_contact_counter_alias_workflow.py')]
    for path in paths:
        ast.parse(path.read_text(),filename=str(path))
    qualification_path=P/'analysis/collision_contact_counter_alias_qualification.json'
    q=json.loads(qualification_path.read_text())
    for name,sha in q['sha256'].items():
        assert digest(name)==sha,name
    assert q['executed_conditional_outcomes']==73 and q['fresh_original_native_comparisons']==12
    assert q['fresh_independent_checks']==186 and q['new_original_routine_credit']==0
    sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",(P/'build_collision_contact.ps1').read_text())]
    assert len(sources)==len(set(sources))==249
    flags=['-fsyntax-only','-Wall','-Wextra','-Werror']
    compiler=r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
    result=subprocess.run([compiler,*flags,*map(str,sources)],check=True)
    report=dict(status='verified_additive_workflow_with_partial_caller',compiler_exit_code=result.returncode,
      compiler_flags=flags,source_count=249,parsed_new_python_files=len(paths),
      baseline_commit=BASELINE,preserved_prior_selected_files=len(baseline['files'])-len(MUTABLE),
      preserved_prior_build_files=len(builds['files']),reviewed_mutable_paths=sorted(MUTABLE),
      original_registry_preserved=True,historical_progress_preserved=True,
      old_proofs_rewritten=False,native_source_or_build_changed=False,
      overall_completion_percent=10,
      source_sha256={str(path):digest(path) for path in sources},
      sha256={str(path):digest(path) for path in paths+[qualification_path,prior_build,P/'COLLISION_CONTACT_COUNTER_ALIAS.txt']})
    (P/'analysis/collision_contact_counter_alias_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('ALIAS WORKFLOW PASS:strict249sources;4Python;15696priorfiles/2733buildspreserved;oneoutcomeOPEN')


if __name__=='__main__':
    main()
