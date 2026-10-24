"""Register qualified original bodies, preserving prior source/proof fields."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qp=P/'analysis/movie_hardware_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==4 and q['qualified_rows']==400
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/movie_hardware.dll')==q['dll_sha256']
rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
for routine in q['routines']:
 start=int(routine['address'],16);end=int(routine['end_exclusive'],16);span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=routine['source'],verification=qp.relative_to(P).as_posix(),status='Qualified own original body with complete rows/outcomes in 400 direct full-state comparisons including explicit alias cases. Startup retains its actual missing-context stop; no parent recovery credit.',limitations=q['limitations'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==entry['address']),None)
 if old is None:registry['routines'].append(entry)
 else:assert old==entry
integration=dict(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_movie_hardware_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_hardware_connected.c'),compiled_source_count=155,newly_registered_entries=4,reused_entries=8,qualified_rows=400,qualified_conditional_outcomes=78,integrated_rows=560,integrated_conditional_outcomes=92,direct_comparisons=400,independent_direct_checks=2450,backend_sampled_words=5000,control_subword_cases=168,backend_checks=40,boot_prefix_comparisons=4,boot_prefix_independent_checks=80,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependency=q['next_dependency'],guide='MOVIE_HARDWARE.txt',scope=q['scope'],limitations=q['limitations'])
for address in ('0x8c10ee9c','0x8c10eee0','0x8c10ee20','0x8c10f9a8','0x8c10f9bc','0x8c10fa60','0x8c10fb48','0x8c1101f0'):
 row=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);row['movie_hardware_integration']=integration
assert len(registry['routines'])==1302;rp.write_text(json.dumps(registry,indent=2)+'\n')
sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_hardware_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered four new bodies; preserved eight reused registrations; total 1302')
