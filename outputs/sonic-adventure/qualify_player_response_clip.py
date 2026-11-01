"""Qualify the complete line-clip helper; keep broader response/renderer limits."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_response_clip_qualification.json';assert not target.exists()
    paths=[P/f'analysis/player_response_clip_{n}.json' for n in ('verification','path_verification','compile_checks','translation','connection')]
    direct,path,compiler,t,c=[json.loads(p.read_text()) for p in paths]
    files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
    for report in (direct,path,compiler,t,c):
        for p,value in report.get('sha256',{}).items():assert sha(p)==value,p;files.append(Path(p))
    for p,value in compiler['source_sha256'].items():assert sha(p)==value,p;files.append(Path(p))
    assert (direct['original_native_comparisons'],direct['checks'],direct['independent_exact_oracles'])==(136,1113,544)
    cov=direct['coverage'];assert (cov['executed_rows'],cov['executed_outcomes'])==(296,16) and not cov['missing_rows'] and not cov['missing_outcomes']
    assert (path['original_native_comparisons'],path['checks'],path['status_counts'])==(51,249,{'0':37,'-1':14})
    assert compiler['source_count']==274 and compiler['compiler_exit_code']==0 and compiler['original_instruction_rows_byte_checked']==296
    dll=R/'work/sonic-adventure/build/player_response_clip.dll';digest=sha(dll)
    assert direct['dll_sha256']==path['dll_sha256']==digest
    body=t['routines'][0];assert body['address']=='0x8c64f5e8'
    row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
    row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=296,coverage=cov)
    prior=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1631];assert len(prior)==1631
    assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==body['address'] for x in prior)
    linked=inventory(dll,prior+[row]);assert linked['confirmed_linked_unique_routines_by_name']==676
    limitations=[
        'Complete296-row near-plane line clip helper:all16conditional outcomes execute in136normal original/native comparisons with1113checks and544independent exact geometry/colour/signed-UV/return-value oracles. Declared finite crossing/visible/hidden/equality endpoints,texture flags,optional output UV and four retained mode/rounding combinations. This is software qualification,not exhaustive arbitrary-alias/nonfinite/physical-hardware proof.',
        '51real072946 response-entry comparisons/249checks:37normal returns and14matched639b68 numeric software stops. Eight debug-enabled cases execute the real043a40/63ce24/64f5e8 calls and return normally. No callee result or private frame is supplied. Declared null-info and state/aux input aliases are adversarial software observations,not gameplay-reachability contracts.',
        'The response236rows/18outcomes and043a40wrapper50rows are observed completely across the declared cases,but remain unregistered here. Full row coverage alone does not settle their initialization,alias or numerical domain contracts.0344dc remains149/476rows and11/52outcomes;63ce24renderer330/684rows and11/32outcomes. Both bodies remain unregistered;all missing outcomes are retained in the report.',
        '274-source native build and strict unbounded Wall/Wextra/Werror checks pass. The296new instruction rows byte-match the original RevA input. No new call-return membership,guest wait bounds,CPU/numeric/bus/ABI changes,or independently qualified nonlocal continuation is introduced.',
        'Full game decompilation,normal complete boot,PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR and display/texture settings remain unfinished. TextureOff must disable sampling/use vertex-material colours. Whole-project10%is a weighted milestone estimate,not measured code coverage. Goal active;SolHigh remains appropriate.'
    ]
    report=dict(status='complete_original_line_clip_software_qualified;broader_response_renderer_contracts_remain_open',routines=[row],new_original_routine_credit=1,newly_qualified_original_rows=296,executed_conditional_outcomes=16,
        fresh_original_native_comparisons=187,fresh_checks=1362,independent_exact_oracles=544,compiled_source_count=274,result_registry_entries=1632,result_unique_module_address_pairs=1629,
        linked_routine_inventory=linked,dll_sha256=digest,response_path_coverage=path['coverage_by_routine'],response_helper_registered=False,debug_wrapper_registered=False,renderer_registered=False,
        new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=compiler['flat_return_membership'],guest_execution_limits_unchanged=True,full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
        next_task='Qualify response initialization/alias/numerical contracts and cover the remaining0344dc/63ce24 outcomes with real inputs;keep the larger movement owner/routing and retail stack provenance obligations open.',limitations=limitations,sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED line clip296rows/16outcomes;187comparisons/1362checks;676confirmedlinks;goal active',flush=True)
if __name__=='__main__':main()
