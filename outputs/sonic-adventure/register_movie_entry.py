"""Register qualified bodies with explicit raw-module provenance and load base."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qp=P/'analysis/movie_entry_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==3 and q['qualified_rows']==36 and q['boot_prefix_cases']==8
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/movie_entry.dll')==q['dll_sha256']
rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
for routine in q['routines']:
 module=routine['module'];base=0x8ceb0000 if module=='MOVIE.BIN' else 0x8c010000;binary_path=ROOT/(q['retail_binary'] if module=='MOVIE.BIN' else 'work/sonic-adventure/extracted/1ST_READ.BIN');binary=binary_path.read_bytes();start=int(routine['address'],16);end=int(routine['end_exclusive'],16);span=hashlib.sha256(binary[start-base:end-base]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module=module,name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=routine['source'],verification=qp.relative_to(P).as_posix(),status='Qualified own original body in 62 direct complete-state comparisons plus eight continuous boot prefixes. MOVIE registration covers only its two-instruction entry thunk; its startup parent remains partial.',limitations=q['limitations'])
 if module=='MOVIE.BIN':entry.update(retail_binary=q['retail_binary'],retail_binary_sha256=q['retail_binary_sha256'],load_address=q['load_address'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and r['address']==entry['address']),None)
 if old is None:registry['routines'].append(entry)
 else:assert old==entry
integration=dict(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_movie_entry_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_entry_connected.c'),compiled_source_count=149,newly_registered_entries=3,new_raw_module_thunks=1,new_main_helpers=2,direct_comparisons=62,independent_direct_checks=594,boot_prefix_comparisons=8,boot_prefix_independent_checks=72,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_rows_covered=q['movie_startup_rows_covered'],movie_startup_static_rows=364,movie_startup_recovery_credit=0,indexed_movie_candidates=536,raw_retail_binary=q['retail_binary'],raw_retail_binary_sha256=q['retail_binary_sha256'],raw_runtime_base='0x8ceb0000',next_dependency='8c10d914 MMIO status query (raw read at ff800030 via 8c10d926); finite-clock cases also preserve input exhaustion at 8c602612.',guide='MOVIE_ENTRY.txt',scope=q['scope'],limitations=q['limitations'])
thunk=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c09da40');thunk['movie_entry_integration']=integration
rp.write_text(json.dumps(registry,indent=2)+'\n')
sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_entry_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered three qualified bodies with raw MOVIE provenance; total',len(registry['routines']))
