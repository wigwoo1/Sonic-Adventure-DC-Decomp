"""Reproduce initialization artifacts and audit preservation against the published baseline."""
import ast,csv,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='5858bcaaa58905bce54e95860fae6525b91f1133'
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    names=('recover_collision_setup','connect_collision_setup','collision_setup_reference','write_watch_collision_setup',
      'collision_setup_task_fixtures','collision_setup_session','collision_setup_vectors','collision_setup_fixtures',
      'collision_setup_contact_fixtures','verify_collision_setup_direct','verify_collision_setup_sequence',
      'verify_collision_setup_contact_alias','verify_collision_setup_retained','check_collision_setup_unbounded',
      'audit_collision_setup','qualify_collision_setup','register_collision_setup','verify_collision_setup_workflow',
      'audit_collision_contact_register_paths')
    scripts=[P/(name+'.py') for name in names]
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    generated=[P/name for name in ('source/sa_collision_setup.c','analysis/collision_setup_translation.json',
      'analysis/collision_setup_returns.json','source/sa_collision_setup_connected.c','source/collision_setup_probe.c',
      'build_collision_setup.ps1','analysis/collision_setup_connection.json','analysis/collision_setup_audit.json')]
    saved={str(path):digest(path) for path in generated}
    for name in ('recover_collision_setup.py','connect_collision_setup.py','audit_collision_setup.py'):
        subprocess.run([sys.executable,'-B',str(P/name)],check=True)
    assert saved=={str(path):digest(path) for path in generated}
    qpath=P/'analysis/collision_setup_qualification.json';oldq=qpath.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'qualify_collision_setup.py')],check=True)
    assert qpath.read_bytes()==oldq
    rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes()
    subprocess.run([sys.executable,'-B',str(P/'register_collision_setup.py')],check=True)
    assert saved==(rp.read_bytes(),sp.read_bytes())
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15708
    mutable={'outputs/sonic-adventure/'+name for name in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in baseline['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    oldreg=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))
    current=json.loads(rp.read_text());assert current['routines'][:1589]==oldreg['routines'] and len(current['routines'])==1591
    for key,value in oldreg.items():
        if key!='routines':assert current[key]==value,key
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():assert status[key][field]==(1591 if field=='recovered_buildable_routines' else data),field
            assert set(status[key])-set(value)=={'collision_setup_integration'}
        else:assert status[key]==(1591 if key=='recovered_buildable_routines' else value),key
    builds_path=P/'analysis/collision_contact_counter_alias_prior_build_manifest.json';builds=json.loads(builds_path.read_text())
    assert len(builds['files'])==2733
    for row in builds['files']:
        assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    pins={}
    for name in ('translation','connection','audit','direct_verification','sequence_verification','contact_alias_verification','retained_verification','qualification'):
        report=json.loads((P/f'analysis/collision_setup_{name}.json').read_text())
        for path,sha in report['sha256'].items():assert digest(path)==sha,(name,path);pins[path]=sha
    register_audit=P/'analysis/collision_contact_register_paths_audit.json';a=json.loads(register_audit.read_text())
    for path,sha in a['sha256'].items():assert digest(path)==sha,path
    assert a['annotated_body_count']==41 and a['byte_checked_original_instruction_rows']==5981
    assert a['registry_entries']==1591 and a['new_original_routine_credit']==0
    original=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    owners=[];extra_files=[]
    for path in (P/'analysis/collision-contact-input-owners').glob('*_instructions.tsv'):
        rows=list(csv.DictReader(path.open(),delimiter='\t'))
        for row in rows:
            address=int(row['address'],16)
            assert original[address-0x8c010000:address-0x8c010000+2]==bytes.fromhex(row['bytes'])
        owners.append(dict(address=path.name.split('_')[0],original_rows=len(rows),listing=str(path)))
        extra_files.append(path)
    assert next(x for x in owners if x['address']=='8c0730d8')['original_rows']==2041
    reference=P/'analysis/collision-contact-entry-contracts/references.tsv'
    refs=list(csv.DictReader(reference.open(),delimiter='\t'))
    calls=[r for r in refs if r['resource_address']=='8c029b00' and r['reference_type']=='COMPUTED_CALL']
    for row in calls:
        address=int(row['reference_from'],16)
        assert int.from_bytes(original[address-0x8c010000:address-0x8c010000+2],'little')&0xf0ff==0x400b
    ps=[P/'build_collision_setup.ps1']+[P/'ghidra'/name for name in ('export_collision_contact_entry_contracts.ps1','export_collision_contact_input_owners.ps1','export_collision_setup_dependencies.ps1')]
    for path in ps:
        command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
    strict=P/'analysis/collision_setup_unbounded.json';u=json.loads(strict.read_text())
    sources=[P/'source'/name for name in re.findall(r"'([^']+\.c)'",(P/'build_collision_setup.ps1').read_text())]
    assert len(sources)==250 and u['source_sha256']=={str(path):digest(path) for path in sources} and u['exit_code']==0
    q=json.loads(qpath.read_text());assert q['new_original_routine_credit']==2 and q['fresh_original_native_comparisons']==361
    files=scripts+ps+generated+[strict,qpath,builds_path,register_audit,reference,P/'COLLISION_SETUP.txt']+extra_files
    report=dict(status='verified_initialization_workflow_with_collision_branch_still_open',
      reproduced_generated_artifacts=8,qualification_and_registration_byte_idempotent=True,
      python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),
      preserved_baseline_commit=BASE,preserved_prior_selected_files=15704,reviewed_mutable_paths=sorted(mutable),
      prior_registry_rows_preserved=1589,registry_entries=1591,registry_unique_module_address_pairs=1588,
      confirmed_linked_unique_routines_by_name=633,preserved_build_files=2733,
      additional_build_files=['work/sonic-adventure/build/collision_setup.dll','work/sonic-adventure/build/collision_setup.lib'],
      compiled_source_count=250,strict_unbounded_sources_checked=250,new_original_routine_credit=2,
      canonical_duplicate_credit=0,fresh_original_native_comparisons=361,fresh_independent_checks=2439,
      total_evidence_original_native_comparisons=71827,original_input_owner_exports=owners,
      indexed_collision_caller_entries=sorted({r['function'] for r in calls if r['function']}),
      collision_call_reference_sites=len(calls),unindexed_collision_call_reference_sites=sum(not r['function'] for r in calls),
      collision_caller_remaining_outcomes=[['0x8c02a48c','0x8c02a4a8']],original_requirements_preserved=True,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      sha256={str(path):digest(path) for path in files})
    (P/'analysis/collision_setup_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('SETUP WORKFLOW PASS:8artifacts;15704oldfiles/1589registryrows/2733buildspreserved;633linked;10%milestones')


if __name__=='__main__':main()
