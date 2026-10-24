"""Preserve historical fields and attach corrected original-code integration."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qp=P/'analysis/movie_transition_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==0 and q['qualified_rows']==10 and q['qualified_conditional_outcomes']==4
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/movie_transition.dll')==q['dll_sha256']
rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());old=q['canonical_registration'];row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==old['address']);assert all(row[k]==value for k,value in old.items())
integration=dict(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_movie_transition_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_transition_connected.c'),compiled_source_count=153,newly_registered_entries=0,integrated_original_rows=10,integrated_conditional_outcomes=4,direct_comparisons=103,independent_direct_checks=945,backend_sampled_words=5000,backend_checks=23,legacy_mask_counterexamples=12,original_mask='0x000001ff',legacy_mask='0xff0f01ff',legacy_semantics_limited=True,preferred_native_source='source/sa_movie_transition.c',boot_prefix_comparisons=10,boot_prefix_independent_checks=160,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependency='8c6046c0 MMIO control; exhausted scan samples stop at 8c60459e and finite-clock controls retain 8c602612.',guide='MOVIE_TRANSITION.txt',scope=q['scope'],limitations=q['limitations'])
row['movie_transition_integration']=integration;assert len(registry['routines'])==1298;rp.write_text(json.dumps(registry,indent=2)+'\n')
sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_transition_integration']=integration;sp.write_text(json.dumps(status,indent=2)+'\n');print('Attached original-mask integration, zero new routine credit; registry remains 1298')
