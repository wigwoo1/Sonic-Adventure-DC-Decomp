"""Register one original loader, retaining its previously published source bytes."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qpath=P/'analysis/boot_disc_qualification.json';q=json.loads(qpath.read_text());assert q['cases_passed']==82 and q['boot_rows_covered']==30 and q['qualified_rows']==50
for path,h in q['sha256'].items():assert digest(path)==h,path
assert digest(ROOT/'work/sonic-adventure/build/boot_disc.dll')==q['dll_sha256']
routine=q['routines'][0];start=int(routine['address'],16);end=int(routine['end_exclusive'],16);binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=q['source'],verification=qpath.relative_to(P).as_posix(),status='Qualified original 50-row loader / four outcomes within 82 full-state comparisons under explicit native-disc platform policies. Returns zero after close even on a failed read, as in the original code.',limitations=q['limitations'])
registry_path=P/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text());existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==routine['address']),None)
if existing is None:registry['routines'].append(entry)
else:assert existing==entry
integration=dict(verification=qpath.relative_to(P).as_posix(),verification_sha256=digest(qpath),adapter_source='source/sa_boot_disc_connected.c',adapter_source_sha256=digest(P/'source/sa_boot_disc_connected.c'),compiled_source_count=145,newly_registered_entries=1,direct_comparisons=82,independent_direct_checks=406,backend_and_api_checks=34,boot_prefix_comparisons=4,boot_prefix_independent_checks=28,boot_rows_covered=30,boot_static_rows=51,boot_recovery_credit=0,real_movie_bytes=184592,real_movie_sectors=91,real_movie_sha256='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b',firmware_recovered=False,natural_boot_verified=False,next_dependency='0xac5fc0c2 uncached cache routine',guide='BOOT_DISC.txt',scope=q['scope'],limitations=q['limitations'])
thunk=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c09da40');thunk['boot_disc_integration']=integration
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=P/'project_status.json';status=json.loads(status_path.read_text());status.setdefault('verified_progress',{})['boot_disc_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);status_path.write_text(json.dumps(status,indent=2)+'\n');print('Registered original loader; total',len(registry['routines']))
