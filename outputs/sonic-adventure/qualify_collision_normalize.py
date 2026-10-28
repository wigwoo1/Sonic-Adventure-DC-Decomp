"""Qualify one direct route to a previously recovered canonical routine."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('connection','audit','direct_verification','retained_verification','unbounded'):
  path=P/f'analysis/collision_normalize_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 dll=ROOT/'work/sonic-adventure/build/collision_normalize.dll';h=digest(dll)
 for name in ('audit','direct_verification'):assert reports[name]['dll_sha256']==h
 for suite in reports['retained_verification']['suites']:
  path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==h
  for pin,sha in r['sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 direct=reports['direct_verification'];retained=reports['retained_verification']
 assert (direct['original_native_comparisons'],direct['independent_checks'],direct['finite_unit_vector_oracle_cases'])==(514,4638,52)
 assert direct['status_counts']=={'0':266,'-1':224,'1':24}
 assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
 assert reports['unbounded']['source_count']==245 and reports['unbounded']['exit_code']==0
 for pin,sha in reports['unbounded']['source_sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1586
 old=next(r for r in registry if r['address']=='0x8c63a94c' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN')
 assert old['name']=='sa_face_vector_normalize_8c63a94c' and old['retail_span_sha256']=='91bc28ff6fe7b04c8d5178dd3b7e8ee411224556da83d770225895326e61376b'
 assert not any(r['address'] in ('0x8c10d058','0x8c10d0f8') and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry)
 previous=P/'analysis/collision_wrapper_return_research.json';previous_report=json.loads(previous.read_text());files.append(previous);assert previous_report['total_evidence_original_native_comparisons']==68275
 fresh=514+243;checks=4638+1683
 q=dict(status='partially_verified_one_canonical_normalizer_direct_route',compiled_source_count=245,newly_registered_entries=0,new_original_routine_credit=0,canonical_registered_entry='0x8c63a94c',new_direct_entry_routes_qualified=1,original_canonical_rows_executed=16,new_original_rows_recovered=0,canonical_duplicate_credit=0,new_nonlocal_resume_routes=0,flat_return_guard_sites=2271,explicit_nonfunction_resume_routes_total=379,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=308,registry_entries=1586,registry_unique_module_address_pairs=1583,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,finite_unit_vector_oracle_cases=52,direct_status_counts=direct['status_counts'],retained_previous_evidence_original_native_comparisons=68275,total_evidence_original_native_comparisons=68275+fresh,physical_fpu_all_inputs_qualified=False,physical_exception_delivery_qualified=False,invalid_pointer_behavior_qualified=False,full_gameplay_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,wrappers_still_unregistered=['0x8c10d058','0x8c10d0f8'],optional_660bae_still_unqualified=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Continue029b00 dependency closure:connect/reuse the registered inverse6390b0 implementation with the correct ABI,then recover remaining rendering dependencies. Keep arbitrary paired-FMOV wrapper paths open;no blind parameter sweeps or supplied callee results/frames.',dll_sha256=h,limitations=[
 'Onlyone directrootentry is added:63a94c routes through existing sa_motion_mesh_lower_entry and its own lowercontext/observer/stop/bus. Canonicalbody/source and numericalbackends unchanged. Previouslycompiled/exported normalizer was not directly routed by root;no duplicate routine recovery/registration credit.',
 '514fresh fullCPU/FPU/RAM/device/ordered-boundary comparisons:266completed/224existingunsupportednumeric stops/24numerictrap stops;nonzero statuses not fabricated completions. All16originalcanonical rows execute. 52independent finite-vector component/unit-length/returned-length oracles use2e-5 tolerance;FIPR/FSRRA stillshare pinned numericalbackend,not arbitraryhardwarebitproof.',
 'BothphysicalFPbanks/RM0/1/PR/SZ/DN,selectedenables,finite/zero/NaN/infinity/subnormal/overflowinputs andmappedMMUoffP0/P1/P2views tested. Validboundedmappedvectorbuffers only;no new invalid-pointer/MMU/address-error/physicalexceptiondelivery claim. Original pointer advance/restoration,FPscratch/FPSCR/RTSdelayeffects retained.',
 '245-sourceDLL/lib andstrictunboundedWallWextraWerrorpass. Exactprevious245bridge plusone lowerwhitelistaddress/exportprefix;CPU/flatmodel/rows/guards/productionobserver/ABI unchanged. No new sourcewaitbound/JSRreturn/nonlocalresumeexport.2271returnmembers/379exports/71qualifiednonlocal/308remaining unchanged;2723oldbuildfiles preserved.',
 '243freshretainedIRQ/context/taskcomparisons/1683checks keep oldnumericassertions. Combined69032is historical-plus-fresh,notunique/codecoverage;216warm/211coldstartup/prior242full512write remain historical. No freshcomplete245startup/gameplay/full-devicewrite claim.',
 'Both29rowpublicanglewrappers remainunregistered with6unexecutedrows/2unqualifiedoutcomes. Scalarreturn derivation doesnotclosearbitrarypairedFMOV/dynamic-targeteffects. Full029b00collisioncaller andinverse/renderingdependencies remainunfinished. PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOffdisables sampling/usevertexcolors.10%weightedmilestones,notcoverage;researchbuildnotplayable.'
 ],sha256={str(p):digest(p) for p in files})
 from audit_movie_linked_routines import inventory
 q['linked_routine_inventory']=inventory(dll,registry);assert q['linked_routine_inventory']['confirmed_linked_unique_routines_by_name']==626
 q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
 (P/'analysis/collision_normalize_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('NORMALIZER DIRECT ROUTE QUALIFIED;0newroutines;',fresh,checks,'626confirmedlinked')
if __name__=='__main__':main()
