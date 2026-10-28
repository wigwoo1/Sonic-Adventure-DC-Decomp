"""Qualify the complete original alternate response with actual produced stack history."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','history_verification','sequence_verification','retained_verification','unbounded'):
        path=P/f'analysis/movement_alternate_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_alternate.dll';sha=digest(dll)
    for name in ('audit','direct_verification','history_verification','sequence_verification'):assert reports[name]['dll_sha256']==sha
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct=reports['direct_verification'];history=reports['history_verification'];t=reports['translation']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(227,1407,38)
    assert direct['status_counts']=={'0':210,'-1':12,'-3':5}
    body=t['routines'][0];assert len(t['routines'])==1 and body['address']=='0x8c075860' and body['instruction_rows']==202
    rows=[r['coverage_by_routine'][0] for r in (direct,history)]
    assert not (set(rows[0]['missing_rows'])&set(rows[1]['missing_rows']))
    assert not (set(map(tuple,rows[0]['missing_outcomes']))&set(map(tuple,rows[1]['missing_outcomes'])))
    coverage=[dict(address=body['address'],static_rows=202,executed_rows=202,static_outcomes=20,executed_outcomes=20,missing_rows=[],missing_outcomes=[])]
    assert history['original_zero_length_branch_executed'] and history['supplied_game_callee_outputs']==history['supplied_execution_frames']==0
    for name,count,checks in (('history_verification',32,144),('sequence_verification',96,480),('retained_verification',243,1683)):
        assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==(count,checks)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==254 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    returns=P/'analysis/movement_alternate_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert ret['prior_return_sites']==2382 and len(ret['new_call_returns'])==10 and len(ret['allowed_call_returns'])==2392
    actual={b['pr'] for r in (direct,history,reports['sequence_verification']) for case in r['cases'] for b in case['boundaries']};assert set(ret['original_call_returns'])<=actual
    previous=P/'analysis/movement_response_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==73717
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=202,coverage=coverage[0])
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1600,1601);baseline=registry[:1600]
    assert not any(r['address']=='0x8c075860' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+[routine]);assert linked['confirmed_linked_unique_routines_by_name']==643;files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_original_alternate_contact_response',compiled_source_count=254,newly_registered_entries=1,new_original_routine_credit=1,
      newly_qualified_original_rows=202,canonical_duplicate_credit=0,executed_original_rows=202,executed_conditional_outcomes=20,
      unaccounted_outcomes=[],unexecuted_rows=[],domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=[routine],
      prior_registry_entries=1600,result_registry_entries=1601,result_unique_module_address_pairs=1598,linked_routine_inventory=linked,
      fresh_original_native_comparisons=598,fresh_independent_checks=3714,direct_status_counts=direct['status_counts'],independent_finite_oracle_cases=38,
      actual_original_call_sites_executed=10,producer_history_comparisons=32,world_collision_response_sequence_comparisons=96,
      original_zero_length_branch_executed=True,supplied_game_callee_outputs=0,supplied_execution_frames=0,
      flat_return_guard_sites=2392,explicit_nonfunction_resume_routes_total=500,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=429,
      new_nonlocal_resume_exports=10,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,
      retained_previous_evidence_original_native_comparisons=73717,total_evidence_original_native_comparisons=74315,
      world_owner_still_unregistered='0x8c028bfe',world_owner_unaccounted_outcomes=[['0x8c028cba','0x8c028cbc'],['0x8c028e0c','0x8c028e0e']],
      parent_movement_caller_still_unregistered='0x8c0730d8',collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=[['0x8c02a48c','0x8c02a4a8']],
      fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Recover next actual0730d8 dependency075214 at originalcall7347e,using original state/query contracts and real callees. Retain older world/collision/precision obligations.',
      limitations=[
        'Original075860:202instructions/20conditionaloutcomes,all execute throughactualdistance/angle/dot/length/normalization callees. No excluded rows/outcomes or unreachable-branch assumptions. '
        'It filters contacts and conditionally clamps querymotion;declared separate velocity/state records stayunchanged. This differs from the075416 owner.',
        '227directcomparisons/1407checks:210complete,12existingunsupportednumericstops,5mappedguards.38included finiteoracles check axisclamp decisions/results andnearest repeated contacts. '
        'Othergeometry/numerics compare original instructions;notcompleteindependentphysics/physicalFPUproof. SuccessfulgeometricprofilesPR0/SZ0;selectedrawmodes/specialvalues/guardschecked.',
        '32comparisons/144checks cover16actual638eac->075860historysequences:oldRAMword-1/0,zero/nonzeromotion,bothFPbanks,RM0/1. Originalvectoradd producesoldstackRAM;response normalentryallocatesitsownframe. '
        'Contactdistance1001 leaves minimum1000/index-1,so the original readsframe+54beforeits distancearray at+58. Zero>negativeword reaches5996taken/5998..59a4;zero controls preserveoriginalresult. '
        'No bufferoverlap orprovidedexecutionframe/calleeresult isneeded. This israwRAMhistorybehavior,notnaturalgame-layout proof. Ghidraelidedzero pathsremaininoriginalsourceandareexecuted.',
        '96comparisons/480checks cover32actual028bfe->029b00->075860pipelines:lower/upperlists,hit/miss,stationary/downwardmotion,up/downquerynormals,bothFPbanks. '
        'Originalcollisionproducesquerycount/88byterecords6bacf0 consumedbynewowner. Initialscene/query/mesh/velocity/state/normal/farsentinel inputs only;no intermediateoutputs/frames. '
        'Selected horizontalplane cases independentlycheckresponsegates/exactmovement andunchangedvelocity. Olderworld/collisionownercoveragegapsremain.',
        '243freshretainedIRQ/context/taskcomparisons/1683checks preserveoldernumericassertionsbyAST.598freshcomparisons/3714checks;74315cumulativeevidence isnotunique/codecoverage. '
        'Prior216warm/211coldstartup and242full-devicewrite remainhistorical,notrerun. SRFDclear/softwaremappedRAMABI;physicalFPU/exception/MMU/bootROM/GPU/gameplay/frame timingunqualified.',
        'The finitehistoricalPythonmodulechain exceededthe default importrecursionlimit before retainedtestsran. Newreference startup raisesonlyhostPythonfloor to4096;oldsource/proofs and guestCPU/interpreter/guards/step limits areunchanged. '
        'Finaldirect/history/retained tests rerun successfully;initialruns are notdoublecounted. NativeC/probe/build bytes didnotneedchangesforthishoststartupfix.',
        '254-sourceDLL/library andstrictunboundedWall/Wextra/Werror checks pass. Exact253bridge plus1entry/10actualreturns/10continuationexports. OldC/guestCPU/referenceinterpreter/guards/observer/comparisonlogic/rawsession/ABI '
        'and2741oldbuilds preserved;newDLL/libadditional.500explicitresumes,71qualifiednonlocal/429remaining;new10exports notindependentlyqualifiednonlocalresumes. No newsourcewaitbounds.',
        '028bfe null-readoutcomes,029b00 A48cskip andthreeoldmathwrapperprecisionpaths remainunqualified;0730d8 staysunregistered. No upstreamimplementationcodecopied. '
        'Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings unfinished. TextureOffdisablessampling/usesvertexcolours.10%weightedmilestones;researchDLLnonplayable;goalactive,keepSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_alternate_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('ALTERNATE QUALIFIED1routine/202rows/20outcomes;598comparisons/3714checks;643linked')

if __name__=='__main__':main()
