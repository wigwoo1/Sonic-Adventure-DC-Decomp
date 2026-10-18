"""Numerical game-call chain with native sound and no external-service API."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import verify_main_dispatch_scalar_complete as previous
import verify_main_dispatch_sound as sound
import sh4_binary32
import sh4_multiply_extended_reference as scalar

PROJECT=Path(__file__).resolve().parent
DLL=previous.base.ROOT/'work/sonic-adventure/build/main_dispatch_sound_complete.dll'
LIB=ctypes.CDLL(str(DLL))
NATIVE={k:getattr(LIB,'sa_main_dispatch_sound_axis_'+k+'_connected') for k in previous.axes.AXES}
NATIVE['calc']=LIB.sa_main_dispatch_sound_vector_calc_connected
NATIVE.update({k:getattr(LIB,'sa_main_dispatch_sound_'+k+'_connected') for k in ('transform','detach','status','flag')})
for fn in NATIVE.values():fn.argtypes=previous.active.pure_args[:-1]+[previous.active.OBSERVE];fn.restype=ctypes.c_int
original_prepare=previous.prepare
original_reference=previous.reference


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(c,number):
    memory,state,fpu,vector=original_prepare(c,number)
    previous.store(memory,sound.LOW,c.get('sound_cursor',0));previous.store(memory,sound.HIGH,32)
    for i in range(37):
        for j in range(13):previous.store(memory,sound.TABLE+i*52+j*4,0xCA000000|(i<<8)|j)
        previous.store(memory,sound.TABLE+i*52,0x7FFFFFFF)
        previous.store(memory,sound.TABLE+i*52+4,1 if c.get('sound_full') else 0)
    return memory,state,fpu,vector


def reference(*args,**kwargs):
    # Execute the retail sound/selector opcodes instead of the earlier service.
    kwargs['hooks']={t:fn for t,fn in kwargs.get('hooks',{}).items() if t!=sound.REQUEST}
    return original_reference(*args,**kwargs)


def adapt(fn):
    # Reuse the earlier test's state/analytic assertions. New C API has no Call.
    def call(ram,fpu,sr,r,pr,fpul,macl,user,_old_external,observe):
        return fn(ram,fpu,sr,r,pr,fpul,macl,user,observe)
    return call


def cases():
    for c in previous.cases():
        if not c.get('sound_fail'):yield c
    for full,cursor,rm,bank,normalize in itertools.product((False,True),(0,31),range(2),range(2),(0,1)):
        yield {'entry':'status','angles':(0x1234,0x5678,0x9ABC),'normalize':normalize,'mode':bank<<21,'rm':rm,
               'sound_full':full,'sound_cursor':cursor,'null_external':True}
    for full,bank in itertools.product((False,True),range(2)):
        yield {'entry':'status','angles':(1,2,3),'normalize':1,'mode':bank<<21,'sound_full':full,'observe':False}


def main():
    for name in ('main_dispatch_scalar_complete_verification.json','main_dispatch_sound_verification.json'):
        report=json.loads((PROJECT/'analysis'/name).read_text())
        for p,h in report['sha256'].items():assert digest(Path(p))==h,p
    allowed=set(previous.previous.ROWS)|set().union(*previous.axes.ROWSETS.values())|set(range(previous.chain.POP,0x8C639BD4,2))
    for name in ('main_dispatch_shared_translation.json','main_dispatch_active_translation.json','main_dispatch_detach_translation.json','main_dispatch_sound_translation.json'):
        for r in json.loads((PROJECT/'analysis'/name).read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    for name in ('main_dispatch_transform_translation.json','main_dispatch_matrix_init_translation.json','main_dispatch_shared_check_translation.json'):
        allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis'/name).read_text())['reachable_addresses'])
    allowed.update(range(previous.active.QUERY,0x8C06CEFE,2));allowed.update(range(previous.base.RESET,0x8C05E7D4,2))
    previous.functions={k:adapt(fn) for k,fn in NATIVE.items()}
    previous.prepare=prepare;previous.reference=reference
    previous.TRACKED=set(previous.TRACKED)|{sound.REQUEST,sound.SELECT}
    sh4_binary32.execute_multiply=scalar.execute
    counts=dict.fromkeys(NATIVE,0);records=[];boundaries=ops=analytic=partial=0;max_error=0.0
    for number,c in enumerate(cases()):
        path,steps,b,n,status,check,error=previous.run_case(c,number,allowed)
        counts[c['entry']]+=1;boundaries+=b;ops+=n;analytic+=check;partial+=status!=0;max_error=max(max_error,error)
        records.append({'input':c,'steps':steps,'native_boundaries':b,'numerical_operations':n,'status':status,
                        'analytic_check':check,'max_absolute_error':error,'sound_request_reached':sound.REQUEST in path,
                        'slot_selector_reached':sound.SELECT in path,'full_state_match':True})
        if (number+1)%150==0:print(number+1,'closed native-chain comparisons passed',flush=True)
    files=[Path(__file__),Path(previous.__file__),Path(sound.__file__),PROJECT/'build_main_dispatch_sound_complete.ps1',
           PROJECT/'source/sa_main_dispatch_sound_complete_connected.c',PROJECT/'source/sa_main_dispatch_sound.c',
           PROJECT/'analysis/main_dispatch_scalar_complete_verification.json',PROJECT/'analysis/main_dispatch_sound_verification.json']
    report={'cases_passed':len(records),'counts':counts,'native_boundary_comparisons':boundaries,'numerical_operations_compared':ops,
            'analytic_rotation_vector_checks':analytic,'partial_guard_trap_cases':partial,'completed_without_external_service_cases':len(records)-partial,
            'sound_request_reached_cases':sum(r['sound_request_reached'] for r in records),'slot_selector_reached_cases':sum(r['slot_selector_reached'] for r in records),
            'max_analytic_absolute_error':max_error,'sha256':{str(p):digest(p) for p in files},'dll_sha256':digest(DLL),
            'scope':'Native sound request/selector through recovered STATUS and existing numerical parents. New C exports accept no external game service or numerical fixtures. Full 16 MiB RAM, CPU/FPU and ordered boundaries compared to original instructions.',
            'limitations':['No playable game/audio playback is claimed. This closes only the selected recovered call paths.',
                          'Vector arithmetic shares pinned upstream math through separate primitive/adapter binaries; independent analytic and scalar manual checks are additional evidence, not physical-hardware validation.',
                          'Legacy numerical PR/SZ and reserved RM limits remain; actual exception delivery, MMIO, invalid pointers, concurrency and all gameplay states are unverified.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_sound_complete_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('sha256','dll_sha256','scope','limitations','cases')},indent=2))


if __name__=='__main__':
    try:main()
    finally:sh4_binary32.execute_multiply=scalar.finite_execute
