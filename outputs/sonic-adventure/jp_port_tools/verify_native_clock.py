"""Compare clock-domain accumulation to arbitrary-precision rational oracles."""
import argparse,hashlib,json,random,time
from pathlib import Path
from native_clock_test_support import Clock,Counter
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();b=a.build
    build=json.loads((b/'build_verification.json').read_text())
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h
    dll=b/'jp_native_clock.dll';assert sha(dll)==build['dll_sha256'];c=Clock(dll)
    checks=0;rng=random.Random(0x434c4f43);groups={}
    def check(value):
        nonlocal checks
        checks+=1;assert value,f'clock check {checks}'
    check(c.status()==4);check(c.init(None,None,1)==1)
    maximum=(2**64-1)//12500001
    frequencies=[1,3,7,1000,1000000,10000000,3000000000,maximum]
    for hz in frequencies:
        counter=Counter();counter.tick=123456789
        check(counter.start(c,hz)==0);check(c.frequency()==hz)
        wall_ticks=active_ticks=0;run=0;reload=12500000
        for i in range(256):
            delta=rng.randrange(hz*5+1);counter.tick+=delta
            wall_ticks+=delta
            if run&2:active_ticks+=delta
            action=rng.randrange(4)
            if action==0:
                run=rng.randrange(256);c.write(run)
            elif action==1:
                reload=rng.choice([0,1,3,12500000,0xffffffff])
                check(c.reset(reload)==0);active_ticks=0
            expected_wall=wall_ticks*781250//hz
            expected_timer=active_ticks*12500000//hz
            check(c.wall()==expected_wall)
            check(c.sdk_wall()==expected_wall%(2**32))
            check(c.timer()==expected_timer)
            check(c.tcnt()==reload-expected_timer%(reload+1))
            check(c.tstr()==run);check(c.status()==0)
    groups['rational_domains_fractional_accumulation_pause_and_reload']=checks
    begin=checks;counter=Counter();check(counter.start(c,781250)==0)
    counter.tick=2**32+123;check(c.wall()==counter.tick);check(c.sdk_wall()==123)
    check(c.timer()==0);c.write(2);counter.tick+=781250
    check(c.timer()==12500000);check(c.tcnt()==0)
    counter.tick+=1;check(c.timer()>12500000);check(c.tcnt()<12500000)
    check(counter.start(c,0)==1);check(counter.start(c,maximum+1)==1)
    counter.tick=1000;check(counter.start(c,1000000)==0)
    counter.tick=999;check(c.wall()==0 and c.status()==2)
    counter.tick=2000;check(c.wall()==0 and c.status()==2)
    check(counter.start(c,1000000)==0);counter.ok=False
    check(c.wall()==0 and c.status()==5);counter.ok=True
    counter.tick=0;check(counter.start(c,1)==0);c.write(2)
    counter.tick=2**64-1;check(c.wall()==0 and c.status()==3)
    check(c.timer()==0);check(c.tstr()==2)
    groups['wrap_faults_invalid_providers_and_atomic_overflow']=checks-begin
    begin=checks;check(c.init(None,None,0)==0);check(c.frequency()>0)
    c.write(0xa2);start_w,start_t=c.wall(),c.timer()
    deadline=time.perf_counter()+0.003
    while time.perf_counter()<deadline:pass
    check(c.wall()>start_w and c.timer()>start_t)
    c.write(0xa0);paused=c.timer();wall=c.wall()
    deadline=time.perf_counter()+0.003
    while time.perf_counter()<deadline:pass
    check(c.timer()==paused);check(c.wall()>wall);check(c.tstr()==0xa0)
    c.write(0xa2);deadline=time.perf_counter()+0.003
    while time.perf_counter()<deadline:pass
    check(c.timer()>paused);check(c.status()==0)
    c.shutdown();check(c.status()==4 and c.frequency()==0)
    groups['real_Windows_QPC_progress_pause_and_resume']=checks-begin
    report=dict(status='native_clock_independent_domains_and_real_QPC_verified',checks=checks,groups=groups,
        provider_frequencies=frequencies,randomized_transitions=2048,
        real_QPC_checked=True,SDK_wall_counter_wrap_checked=True,
        physical_SH4_prescaler_phase_or_timing_validated=False,wall_counts_per_second=781250,
        timer1_active_ticks_per_second=12500000,DLL_sha256=sha(dll),
        build_report_sha256=sha(b/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        test_support_sha256=sha(Path(__file__).with_name('native_clock_test_support.py')),
        complete_presentation_or_gameplay_qualified=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks')
if __name__=='__main__':main()
