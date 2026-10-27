"""Audit new removal evidence without changing old source, proofs or credit."""
import ast,csv,hashlib,json,subprocess,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASE='d7e687d5e2e055c80fc1f150e16b1c1f9bfb205d';G=r'C:\Program Files\Git\cmd\git.exe'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    files=[Path(__file__)];reports={}
    for name,path in (('literals',P/'analysis/movie_write_hook_literal_audit.json'),('tasks',P/'analysis/movie_filesystem_write_removal_tasks_verification.json'),('prior',P/'analysis/movie_filesystem_write_qualification.json')):
        r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    tasks=reports['tasks'];assert tasks['task_sessions_passed']==tasks['completed_initializations']==12
    assert tasks['original_native_comparisons']==728 and tasks['independent_process_shards']==3
    expected={(kind,order,endian) for kind in ('pre_data_removal','pre_commit_removal','post_commit_removal') for order in (0,1) for endian in (0,1)}
    assert {(c['configuration']['kind'],c['configuration']['order'],c['configuration']['endian']) for c in tasks['cases']}==expected
    for case in tasks['cases']:
        kind=case['configuration']['kind'];assert case['result']=='0xffff0000'
        assert len(case['requests'])=={'pre_data_removal':0,'pre_commit_removal':4,'post_commit_removal':5}[kind]
        scans=case['raw_removal_scans'];assert len(scans)==int(kind!='post_commit_removal')
        for scan in scans:
            assert '0x8c65eeda' in scan['actual_targets']
            obs=scan['raw_observation'];assert obs['unplug'] and obs['request_ids_unchanged'] and all(r['header'][2]==0 for r in obs['raw_inputs'])
        assert len(case['progress_calls'])==1 and case['progress_calls'][0][1:]==[20,1,2]
        assert case['phases'][-1]['phase']=='original_owner_teardown'
    assert reports['prior']['total_evidence_original_native_comparisons']==29951
    before=next(r for r in reports['prior']['coverage_by_routine'] if r['address']=='0x8c660bae');after=next(r for r in tasks['coverage_by_routine'] if r['address']=='0x8c660bae')
    missing_rows=set(before['missing_rows'])&set(after['missing_rows']);missing_edges=set(map(tuple,before['missing_outcomes']))&set(map(tuple,after['missing_outcomes']))
    hook_edges={('0x8c660d30','0x8c660d34'),('0x8c660d46','0x8c660d48'),('0x8c660d46','0x8c660d54')}
    assert len(missing_rows)==10 and missing_edges==hook_edges
    assert set(map(tuple,before['missing_outcomes']))-missing_edges=={('0x8c660c3e','0x8c660d0a'),('0x8c660cc8','0x8c660d0a'),('0x8c660d08','0x8c660d0a')}
    binary=ROOT/'work/sonic-adventure/extracted/files/1ST_READ.BIN';data=binary.read_bytes();files.append(binary)
    rows=list(csv.DictReader((P/'analysis/movie-write-hook-references/nearby_literal_loads.tsv').open(),delimiter='\t'));assert len(rows)==197
    for row in rows:
        pc=int(row['address'],16);word=int.from_bytes(data[pc-0x8c010000:pc-0x8c010000+2],'little');assert word&0xf000==0xd000
        literal=((pc+4)&~3)+(word&255)*4;assert literal==int(row['literal'],16)
        value=int.from_bytes(data[literal-0x8c010000:literal-0x8c010000+4],'little');assert value==int(row['value'],16)
        assert 0x0c67c000<=value&0x1fffffff<0x0c67c400 and 0x0c67c27c-(value&0x1fffffff)==int(row['offset_to_cell'])
    refpath=P/'analysis/movie-write-hook-references/cell_references.tsv';refs=list(csv.DictReader(refpath.open(),delimiter='\t'))
    assert {(r['from'],r['type']) for r in refs}=={('8c66089e','WRITE'),('8c660d2a','READ'),('8c660e38','DATA')}
    literals=reports['literals'];assert (literals['extracted_files_checked'],literals['decoded_prs_files_checked'],literals['bytes_scanned'])==(2832,849,1296466401)
    main=next(r for r in literals['matches'] if r['resource']=='1ST_READ.BIN');assert {r['offset'] for r in main['hits']}=={'0x650954','0x650e38'}
    assert len(literals['matches'])==2 and literals['matches'][1]['resource']=='SONICADV/SA1_600.SFD' and len(literals['matches'][1]['hits'])==1 and not literals['matches'][1]['hits'][0]['aligned_word']
    for row in literals['inputs']:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    dll=ROOT/'work/sonic-adventure/build/movie_filesystem_write.dll';assert digest(dll)==tasks['dll_sha256']==reports['prior']['dll_sha256'];files.append(dll)
    strictpath=P/'analysis/movie_filesystem_write_unbounded.json';strict=json.loads(strictpath.read_text());assert strict['exit_code']==0 and strict['source_count']==231
    for pin,h in strict['source_sha256'].items():assert digest(pin)==h
    files.append(strictpath)
    def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
    old=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(old['files'])==14673
    mutable={'outputs/sonic-adventure/README.md','outputs/sonic-adventure/SOURCE_HANDOFF.txt','outputs/sonic-adventure/project_status.json'}
    for row in old['files']:
        if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1535 and not any(r['address']=='0x8c660bae' for r in registry)
    status=json.loads((P/'project_status.json').read_text());prior_status=json.loads(historical('outputs/sonic-adventure/project_status.json'))
    for k,value in prior_status.items():
        if k=='verified_progress':
            for key,v in value.items():assert status[k][key]==v,key
        else:assert status[k]==value,k
    with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
        bm=json.loads(z.read('BUILD_MANIFEST.json'));assert len(bm['files'])==2695
        for row in bm['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    scripts=[P/'verify_movie_filesystem_write_removal.py',P/'audit_movie_write_hook_literals.py',Path(__file__)]
    for script in scripts:ast.parse(script.read_text())
    files+=scripts+[refpath,P/'analysis/movie-write-hook-references/nearby_literal_loads.tsv',P/'ghidra/ExportMovieWriteHookReferences.java',P/'ghidra/export_movie_write_hook_references.ps1']
    report=dict(status='partially_verified_write_removal_and_bounded_hook_investigation',compiled_source_count=231,native_sources_changed=False,fresh_native_rebuild=False,previous_native_build_and_strict_source_pins_current=True,original_native_comparisons=728,independent_checks=tasks['independent_checks'],original_task_sessions=12,newly_verified_backend_outcomes=3,backend_executed_rows=196,backend_static_rows=206,backend_executed_outcomes=31,backend_static_outcomes=34,backend_missing_rows=sorted(missing_rows),backend_missing_outcomes=sorted(missing_edges),newly_registered_entries=0,registry_entries=1535,prior_evidence_comparisons=29951,total_evidence_comparisons=30679,extracted_files_checked=2832,decoded_resources_checked=849,bytes_literal_scanned=1296466401,byte_checked_nearby_literal_loads=197,known_direct_write_clears_hook=True,hook_nonnull_domain_qualified=False,physical_maple_dma_complete=False,full_filesystem_qualified=False,prior_selected_paths_preserved=14673,prior_selected_files_unchanged=14670,reviewed_mutable_paths=sorted(mutable),old_build_files_unchanged=2695,baseline_commit=BASE,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files},scope='12actual raw-response tasks:device removal beforedata queue,beforecommit queue,andaftercommitresponse,alltransport/interface byteorders,bothchannels. Additionalprimaryscan usesoriginalsubmit/pump whileworkerunscheduled;externalmutexrelease neverrepairsframes/IDs/PR/results. Originalwrite-task independentstate/data/counter/callback/idle/teardown assertionsretained. Fullinventory/decoded literal scan and197bytecheckedanalyzedPCrelativeloads giveboundedcallbackleads only,notcomputedalias/unreachability proof. Backendoptionalhook10rows/3outcomes remainsunqualified/unregistered;no inventedhookreturn. NativeC/DLL/proofbytesunchanged;priorbuild reused,notfreshlyrebuilt. Earlier29951comparisons historical,notallrerun;sumisnoteffectivecoverage oruniquecases. Goalactive10%;researchbuildsnotplayableports.')
    out=P/'analysis/movie_write_removal_evidence_verification.json';out.write_text(json.dumps(report,indent=2)+'\n');print('WRITE REMOVAL EVIDENCE PASS',report['original_native_comparisons'],report['independent_checks'],'checks;3additionaloutcomes;0newroutines',flush=True)
if __name__=='__main__':main()
