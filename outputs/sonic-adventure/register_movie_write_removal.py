"""Record additional evidence and the next frontier, giving no routine credit."""
import ast,csv,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    files=[Path(__file__)];reports={}
    for key,name in (('removal','movie_write_removal_evidence_verification'),('frontier','movie_filesystem_command7_frontier_verification')):
        p=P/'analysis'/(name+'.json');r=json.loads(p.read_text());reports[key]=r;files.append(p)
        for pin,h in r['sha256'].items():assert digest(pin)==h,(key,pin);files.append(Path(pin))
    removal=reports['removal'];frontier=reports['frontier']
    assert (removal['original_native_comparisons'],removal['independent_checks'],frontier['original_native_comparisons'],frontier['independent_checks'])==(728,3590,92,424)
    assert frontier['original_task_sessions']==2 and frontier['next_original_dependency']=='0x8c649d20' and frontier['actual_parent_return']=='0x8c647a70'
    assert removal['dll_sha256']==frontier['dll_sha256']
    folder=P/'analysis/movie-filesystem-command7';rows=list(csv.DictReader((folder/'8c649d20_instructions.tsv').open(),delimiter='\t'));assert len(rows)==92
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    for row in rows:
        address=int(row['address'],16)-0x8c010000;assert binary[address:address+2]==bytes.fromhex(row['bytes'])
    diagnostics=list(csv.DictReader((folder/'diagnostics.tsv').open(),delimiter='\t'));assert len(diagnostics)==1 and diagnostics[0]['completed']=='true' and not diagnostics[0]['error']
    files+=[folder/n for n in ('8c649d20_instructions.tsv','8c649d20_draft.c','diagnostics.tsv')]+[P/'ghidra/export_movie_filesystem_command7.ps1',P/'verify_movie_filesystem_command7_frontier.py']
    scripts=[P/'verify_movie_filesystem_write_removal.py',P/'verify_movie_filesystem_command7_frontier.py',P/'audit_movie_write_hook_literals.py',P/'verify_movie_write_removal_evidence.py',Path(__file__)]
    for script in scripts:ast.parse(script.read_text())
    for path in (P/'ghidra/export_movie_write_hook_references.ps1',P/'ghidra/export_movie_filesystem_command7.ps1'):
        cmd="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',cmd],check=True)
    rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())['routines'];assert len(registry)==1535
    assert not any(r['address'] in ('0x8c660bae','0x8c649d20') for r in registry)
    q=dict(status='partially_verified_remaining_write_removal_and_next_command_frontier',compiled_source_count=231,newly_registered_entries=0,newly_qualified_rows=0,newly_qualified_outcomes=0,newly_verified_partial_backend_outcomes=3,backend_executed_rows=196,backend_static_rows=206,backend_executed_outcomes=31,backend_static_outcomes=34,backend_missing_rows=removal['backend_missing_rows'],backend_missing_outcomes=removal['backend_missing_outcomes'],partial_unregistered_proposals=['0x8c660bae','0x8c649d20'],registry_entries=1535,unique_module_address_pairs=1532,original_task_sessions=14,completed_removal_lifecycles=12,pending_command7_frontier_sessions=2,fresh_original_native_comparisons=820,fresh_independent_checks=4014,retained_previous_evidence_original_native_comparisons=29951,total_evidence_original_native_comparisons=30771,next_original_dependency='0x8c649d20',next_actual_parent_return='0x8c647a70',next_dependency_byte_checked_rows=92,next_dependency_qualification_credit=0,extracted_files_literal_scanned=2832,decoded_resources_literal_scanned=849,bytes_literal_scanned=1296466401,analyzed_nearby_literal_loads_byte_checked=197,hook_nonnull_domain_qualified=False,hook_globally_unreachable_proven=False,physical_maple_dma_complete=False,full_filesystem_qualified=False,native_sources_changed=False,fresh_native_rebuild=False,previous_build_strict_source_pins_current=True,qualified_nonlocal_routes_total=25,unqualified_nonlocal_routes=119,new_nonlocal_route_credit=0,preserved_prior_selected_paths=14673,preserved_prior_selected_files_unchanged=14670,preserved_build_files=2695,baseline_commit=removal['baseline_commit'],overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',dll_sha256=removal['dll_sha256'],next_task='Recover actual command7 handler649d20,92freshbytecheckedrows,reachedafteroriginalrawfilesysteminitialization. PreserveactualPR647a70/filenameABI andall21workerslots;traceitsrealcallees beforeintegration. Backend660bae optionalhookstill10rows/3outcomespartial;donotfakehookoutputs/claimgloballyunreachable basedonliteralscan.',limitations=[removal['scope'],frontier['scope'],'No new routine/row/conditional-outcome qualification credit. Three additional observed outcomes belong to stillpartial660bae. Exports/drafts/reference hits arediscovery evidence only. Earlier29951comparisons historical,notallrerun;combined30771notuniquecases/coverage. Native231DLL andstrictsourcepins reused unchanged;newJava exporteractuallycompiled/executed read-only;Python/workflow inputs parsedandchecked. Goalactive10%;researchlibrariesnotplayablegame. Originalrequirements includingtextureOffvertexcolors preserved.'],sha256={str(p):digest(p) for p in files})
    qp=P/'analysis/movie_write_removal_integration_verification.json';qp.write_text(json.dumps(q,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());assert status['recovered_buildable_routines']==status['verified_progress']['recovered_buildable_routines']==1535
    value={k:v for k,v in q.items() if k!='sha256'};value.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),workflow_sha256=digest(Path(__file__)),guide='MOVIE_WRITE_REMOVAL.txt')
    status['verified_progress']['movie_write_removal_and_command7_frontier']=value;sp.write_text(json.dumps(status,indent=2)+'\n')
    print('WRITE REMOVAL/FRONTIER REGISTERED EVIDENCE820/4014;0routinecredit;goalactive10%',flush=True)
if __name__=='__main__':main()
