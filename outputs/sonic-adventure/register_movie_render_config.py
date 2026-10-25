"""Register closed original bodies while preserving historical helper evidence."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qp=P/'analysis/movie_render_config_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==6 and q['qualified_rows']==822
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/movie_render_config.dll')==q['dll_sha256']
rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
for routine in q['routines']:
 start=int(routine['address'],16);end=int(routine['end_exclusive'],16);span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=routine['source'],verification=qp.relative_to(P).as_posix(),status='Qualified own original reachable body in 753 direct complete-state comparisons plus six staged startup profiles. One configuration outcome is independently proven unreachable; three other timing constructors remain partial.',limitations=q['limitations'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==entry['address']),None)
 if old is None:registry['routines'].append(entry)
 else:assert old==entry
integration=dict(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_movie_render_config_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_render_config_connected.c'),compiled_source_count=158,newly_registered_entries=6,reused_qualified_bodies=4,qualified_rows=822,qualified_static_conditional_outcomes=60,qualified_executed_conditional_outcomes=59,qualified_proven_unreachable_outcomes=1,integrated_static_rows=1814,integrated_tested_rows=q['integrated_tested_rows'],partial_constructor_count=3,direct_comparisons=753,independent_direct_checks=2950,startup_profiles=6,startup_independent_checks=120,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependency=q['next_dependency'],guide='MOVIE_RENDER_CONFIG.txt',scope=q['scope'],limitations=q['limitations'])
for address in ('0x8c665a80','0x8c10d3c8','0x8c10db1c','0x8c6513ee'):
 row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);row['movie_render_config_integration']=integration
rp.write_text(json.dumps(registry,indent=2)+'\n')
sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_render_config_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered six closed bodies; total entries',len(registry['routines']))
