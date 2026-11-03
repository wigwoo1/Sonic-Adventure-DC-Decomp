"""Exercise actual upstream nested timing exclusion and task-mark identity."""
import argparse,ctypes as C,hashlib,json,random,time
from pathlib import Path
from native_clock_test_support import Clock,Counter,U,P
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();b=a.build;build=json.loads((b/'build_verification.json').read_text())
    dll=b/'presentation_clock_probe.dll';assert sha(dll)==build['DLL_sha256'];clock=Clock(dll);lib=clock.lib
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    reset=bind('jp_probe_reset',None,C.c_int,C.c_int)
    untimed=bind('present_untimed',None,C.c_int)
    depth=bind('jp_probe_untimed_depth',C.c_int)
    used=bind('jp_probe_marks_used',C.c_int);lost=bind('jp_probe_lost',C.c_int)
    add=bind('jp_probe_add',C.c_int,P,U);find=bind('jp_probe_find',C.c_int,P,C.POINTER(U))
    checks=0;groups={};rng=random.Random(0x50524553)
    def check(value):
        nonlocal checks
        checks+=1;assert value,f'presentation probe check {checks}'
    for split in [0,1]:
        for step in [0,1,2]:
            for initial in [0,0xa0,0xa2,0xff]:
                counter=Counter();check(counter.start(clock,1000000)==0)
                reset(split,step);clock.write(initial);active=0;run=initial;level=0
                for action in [1,1,1,0,0,0,1,0]:
                    delta=rng.randrange(1,1000);counter.tick+=delta
                    if run&2:active+=delta
                    untimed(action)
                    if split and step==1:
                        if action:
                            if level==0:run=initial&~2
                            level+=1
                        else:
                            level-=1
                            if not level:run=initial
                    check(depth()==level);check(clock.tstr()==run)
                    check(clock.timer()==active*12500000//1000000)
                    check(clock.wall()==counter.tick*781250//1000000)
                check(depth()==0 and clock.tstr()==initial and clock.status()==0)
    groups['actual_nested_untimed_step_and_split_contract']=checks
    begin=checks;reset(1,1);expected={};keys=[]
    # Opaque identities only: these addresses are never dereferenced by marks.
    # Many have identical low 32 bits to exercise full native identity.
    for i in range(900):
        key=((i+1)<<32)|0xabcdef00;flags=rng.getrandbits(32);keys.append(key)
        result=add(key,flags)
        if len(expected)<768:expected[key]=flags;check(result==0)
        else:check(result==1)
        check(used()==len(expected))
    check(lost()==1)
    for key in keys:
        flags=U();found=find(key,C.byref(flags));check(bool(found)==(key in expected))
        if key in expected:check(flags.value==expected[key])
    for key in keys[:100]:
        flags=rng.getrandbits(32);add(key,flags);expected[key]|=flags
        result=U();check(find(key,C.byref(result))==1);check(result.value==expected[key])
    check(used()==768);check(add(None,1)==-1)
    reset(0,0);check(used()==0 and lost()==0)
    out=U();check(find(keys[0],C.byref(out))==0)
    groups['actual_mark_table_64bit_identity_capacity_and_updates']=checks-begin
    begin=checks;check(clock.init(None,None,0)==0);reset(1,1);clock.write(2)
    untimed(1);paused=clock.timer();wall=clock.wall()
    deadline=time.perf_counter()+0.003
    while time.perf_counter()<deadline:pass
    check(clock.timer()==paused and clock.wall()>wall)
    untimed(1);untimed(0);check(depth()==1 and clock.tstr()==0)
    untimed(0);check(depth()==0 and clock.tstr()==2)
    deadline=time.perf_counter()+0.003
    while time.perf_counter()<deadline:pass
    check(clock.timer()>paused and clock.status()==0);clock.shutdown()
    groups['actual_untimed_with_real_QPC']=checks-begin
    report=dict(status='actual_presentation_timer_exclusion_and_marks_verified_in_isolation',checks=checks,groups=groups,
        actual_source_functions=build['actual_source_functions'],pointer_identity_cases=900,table_capacity=768,
        full_presentation_linked=False,fixture_frame_step_not_production_scheduler=True,
        task_state_replay_or_gameplay_qualified=False,real_QPC_checked=True,
        DLL_sha256=sha(dll),build_report_sha256=sha(b/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        test_support_sha256=sha(Path(__file__).with_name('native_clock_test_support.py')),
        game_containing_artifact_publishable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks')
if __name__=='__main__':main()
