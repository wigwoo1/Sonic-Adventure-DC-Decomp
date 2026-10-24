"""Register one cache leaf; never count its RAM address alias twice."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qp=P/'analysis/boot_cache_qualification.json';q=json.loads(qp.read_text());assert q['cases_passed']==88 and q['unique_physical_rows']==6 and q['boot_rows_covered']==33
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/boot_cache.dll')==q['dll_sha256']
routine=next(r for r in q['routines'] if r['address']=='0x8c5fc0c2');start=int(routine['address'],16);end=int(routine['end_exclusive'],16);binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=q['source'],verification=qp.relative_to(P).as_posix(),status='Qualified six original instructions in 88 complete-state comparisons under privileged MMU-off software CCR policy. P1 and P2 execution matched independently; one physical body is registered.',address_aliases=['0xac5fc0c2'],alias_duplicate_credit=0,limitations=q['limitations'])
rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==routine['address']),None)
if existing is None:registry['routines'].append(entry)
else:assert existing==entry
integration=dict(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_boot_cache_connected.c',adapter_source_sha256=digest(P/'source/sa_boot_cache_connected.c'),compiled_source_count=147,newly_registered_entries=1,alias_duplicate_credit=0,direct_comparisons=88,independent_direct_checks=984,ccr_write_read_cases=130,backend_checks=20,boot_prefix_comparisons=4,boot_prefix_independent_checks=36,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,natural_boot_verified=False,next_dependency='0x8ceb0000 entry in the just-loaded MOVIE.BIN image',guide='BOOT_CACHE.txt',scope=q['scope'],limitations=q['limitations'])
thunk=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c09da40');thunk['boot_cache_integration']=integration
rp.write_text(json.dumps(registry,indent=2)+'\n')
sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['boot_cache_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered one cache leaf with no alias duplication; total',len(registry['routines']))
