"""Replace disposal fixtures with native cleanup in notification caller tests."""
import ctypes,gc,hashlib,json
from pathlib import Path
import verify_main_dispatch_notification as previous
import sh4_binary32
PROJECT=Path(__file__).resolve().parent
DLL=previous.ROOT/'work/sonic-adventure/build/main_dispatch_notification_cleanup.dll';LIB=ctypes.CDLL(str(DLL))
FUNCTIONS={k:getattr(LIB,fn.__name__) for k,fn in previous.FUNCTIONS.items()}
for fn in FUNCTIONS.values():fn.argtypes=previous.previous.ARGS+[previous.OBSERVE];fn.restype=ctypes.c_int
original_prepare=previous.prepare
LAST={}
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def prepare(c,n):
    memory,state,fpu=original_prepare(c,n);LAST['memory']=memory
    return memory,state,fpu


def main():
    baseline=PROJECT/'analysis/main_dispatch_notification_verification.json'
    for p,h in json.loads(baseline.read_text())['sha256'].items():assert digest(Path(p))==h,p
    inventory_path=PROJECT/'analysis/main_dispatch_task_disposal_inventory.json'
    inventory=json.loads(inventory_path.read_text())
    for p,h in inventory['sha256'].items():assert digest(Path(p))==h,p
    previous.FUNCTIONS=FUNCTIONS;previous.prepare=prepare;previous.EXTERNAL-= {previous.DISPOSE}
    cleanup_rows=set().union(*({int(a,16) for a in r['reachable_addresses']} for r in inventory['routines']))
    previous.TRACKED|={previous.DISPOSE,0x8C098998,0x8C098E60,0x8C098E98,0x8C098B42}
    allowed=cleanup_rows.copy()
    for r in json.loads((PROJECT/'analysis/main_dispatch_notification_translation.json').read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    for start,end in ((previous.previous.START,previous.previous.END),(0x8C604370,0x8C604376),(0x8C646868,0x8C646876),(0x8C65F3A0,0x8C65F3B2),
        (0x8C10CA5C,0x8C10CB06),(0x8C09852E,0x8C0985EC),(0x8C0985EC,0x8C09865C),(0x8C098A80,0x8C098B0C),(0x8C098B0C,0x8C098B42)):allowed.update(range(start,end,2))
    for r in json.loads((PROJECT/'analysis/main_dispatch_sound_translation.json').read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    sh4_binary32.execute_multiply=previous.previous.multiply_expected
    counts=dict.fromkeys(FUNCTIONS,0);records=[];boundaries=partial=invariants=closed=restored=0;rows=set();failures={}
    def check(c,n):
        nonlocal boundaries,partial,invariants,closed,restored
        path,b,status,inv,external=previous.run(c,n,allowed);rows.update(set(path)&cleanup_rows)
        counts[c['entry']]+=1;boundaries+=b;partial+=status!=0;invariants+=inv;closed+=status==0 and not external
        checked=False
        if previous.DISPOSE in path and status==0:
            m=LAST['memory'];load=previous.load
            assert load(m,previous.FREE_TASK)==previous.TASK and load(m,previous.LEVELS)==0
            assert load(m,previous.TASK)==previous.NEXTTASK and load(m,previous.NEXTTASK+4)==previous.TASK
            assert all(load(m,previous.TASK+a)==0 for a in (4,8,12,16,20,24,32,36,40,44))
            size=c.get('heap',128)
            assert load(m,previous.FREE_HEAP)==(previous.ARENA if size else 0)
            if size:
                assert [load(m,previous.ARENA+i*4) for i in range(4)]==[previous.ARENA+size,0,previous.ARENA+size,0]
                assert load(m,previous.ARENA+size+12)==previous.ARENA
            checked=True;restored+=1
        if status==0:
            for target in external:failures.setdefault(target,c)
        records.append({'input':c,'steps':len(path),'boundary_comparisons':b,'status':status,'independent_invariant':inv,
                        'allocation_failure_lists_heap_restored':checked,'external_calls':[hex(t) for t in external],'full_state_match':True})
    for n,c in enumerate(previous.cases()):
        check(c,n)
        if (n+1)%200==0:print(n+1,'native cleanup caller comparisons passed',flush=True)
        if (n+1)%25==0:gc.collect()
    for target,c in sorted(failures.items()):check(dict(c,fail_target=target),len(records))
    files=[Path(__file__),Path(previous.__file__),baseline,inventory_path,PROJECT/'build_main_dispatch_notification_cleanup.ps1',
           PROJECT/'source/sa_main_dispatch_notification_cleanup_connected.c']
    for r in inventory['routines']:files.append(PROJECT/r['source'])
    report={'cases_passed':len(records),'counts':counts,'boundary_comparisons':boundaries,'partial_guard_error_cases':partial,
        'independent_invariants':invariants,'completed_without_external_calls':closed,'independent_failure_cleanup_restorations':restored,
        'cleanup_rows_reached':len(rows),'service_error_cases':len(failures),'sha256':{str(p):digest(p) for p in files},'dll_sha256':digest(DLL),
        'scope':'Original notification/query/creator/prefix tests with disposal and heap release now native; independent free-task lists, cleared slots and fully coalesced heap restoration on allocation failure.',
        'limitations':['Prefix spawn/cancel/setter remain service fixtures. This caller suite does not exercise arbitrary task destructors or resource payloads; standalone cleanup tests provide additional evidence.',
                      'Valid RAM/metadata and inherited arithmetic guard limits. Hardware, concurrency and full gameplay/ports remain unverified.'],
        'cases':records}
    (PROJECT/'analysis/main_dispatch_notification_cleanup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('cases_passed','counts','boundary_comparisons','partial_guard_error_cases','independent_invariants','completed_without_external_calls','independent_failure_cleanup_restorations','cleanup_rows_reached','service_error_cases')},indent=2))


if __name__=='__main__':
    try:main()
    finally:sh4_binary32.execute_multiply=previous.previous.scalar.finite_execute
