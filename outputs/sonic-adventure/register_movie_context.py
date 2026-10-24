"""Preserve canonical registrations and attach initialization integration."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qp=P/'analysis/movie_context_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==0 and q['integrated_rows']==312
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/movie_context.dll')==q['dll_sha256']
rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());count_before=len(registry['routines'])
integration=dict(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_movie_context_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_context_connected.c'),compiled_source_count=157,newly_registered_entries=0,integrated_unique_bodies=3,integrated_canonical_entries=4,integrated_rows=312,integrated_conditional_outcomes=2,direct_comparisons=70,independent_direct_checks=514,backend_register_cases=144,backend_alias_reads=432,backend_checks=28,failed_initialization_cases=3,failed_initialization_checks=12,startup_profiles=6,startup_independent_checks=120,initialized_startup_profiles=4,cold_controls=2,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,native_initialized_entry=q['native_initialized_entry'],next_dependency=q['next_dependency'],guide='MOVIE_CONTEXT.txt',scope=q['scope'],limitations=q['limitations'])
for canonical in q['canonical_registrations']:
 row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==canonical['address'] and r['name']==canonical['name']);assert all(row[k]==value for k,value in canonical.items());row['movie_context_integration']=integration
assert len(registry['routines'])==count_before;rp.write_text(json.dumps(registry,indent=2)+'\n')
sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_context_integration']=integration;sp.write_text(json.dumps(status,indent=2)+'\n');print('Attached original initialization evidence to four canonical entries; count remains',count_before)
