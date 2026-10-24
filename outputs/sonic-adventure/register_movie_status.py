"""Attach new integration evidence while preserving canonical source/proof fields."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qp=P/'analysis/movie_status_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==0 and q['qualified_rows']==15
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/movie_status.dll')==q['dll_sha256']
rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());before=len(registry['routines'])
integration=dict(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_movie_status_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_status_connected.c'),compiled_source_count=151,newly_registered_entries=0,integrated_original_rows=15,direct_comparisons=118,independent_direct_checks=1568,exhaustive_sampled_words=65536,backend_checks=24,boot_prefix_comparisons=18,boot_prefix_independent_checks=216,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependency='8c604594 PVR transition wait; finite clock controls retain input exhaustion at 8c602612.',guide='MOVIE_STATUS.txt',scope=q['scope'],limitations=q['limitations'])
for old in q['canonical_registrations']:
 row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==old['address'])
 assert all(row[k]==value for k,value in old.items());row['movie_status_integration']=integration
assert len(registry['routines'])==before==1298
rp.write_text(json.dumps(registry,indent=2)+'\n')
sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_status_integration']=integration;sp.write_text(json.dumps(status,indent=2)+'\n')
print('Attached MOVIE status integration; preserved all 1298 registry entries')
