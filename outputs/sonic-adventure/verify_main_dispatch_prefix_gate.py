"""Byte-executed state gate, native sound integration and independent invariants."""
import ctypes,hashlib,inspect,itertools,json,struct
from pathlib import Path
from verify_startup import ROOT,State,reference,signed
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_main_dispatch_active_query import OBSERVE
import sh4_binary32
import sh4_multiply_extended_reference as scalar
import verify_main_dispatch_sound as sound

PROJECT=Path(__file__).resolve().parent
START,END=0x8C0AFB10,0x8C0AFC98
EVENT,SPAWN,CANCEL,SET=0x8C109FDE,0x8C07B320,0x8C01AE60,0x8C07B2B8
FMUL,FNEG=0x8C0AFC24,0x8C0AFC7C
OBJECT,WORK,SCRATCH=0x8CA00000,0x8CA10000,0x8CEE0000
SERVICES={EVENT,SPAWN,CANCEL,SET,sound.REQUEST}
RAM_SIZE=sound.RAM_SIZE;BASELINE=sound.BASELINE;BINARY=sound.BINARY
DLL=ROOT/'work/sonic-adventure/build/main_dispatch_prefix_gate.dll';LIB=ctypes.CDLL(str(DLL))
direct=LIB.sa_main_dispatch_prefix_gate_8c0afb10;connected=LIB.sa_main_dispatch_prefix_gate_connected
ARGS=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
direct.argtypes=ARGS;direct.restype=ctypes.c_int
connected.argtypes=ARGS+[OBSERVE];connected.restype=ctypes.c_int
# The established executor publishes MACL only on normal return. This test
# needs it at observed boundaries and early stops, so publish the existing
# local value before trace callbacks in a process-local copy. No instruction
# behavior or saved executor file is changed.
_reference_source=inspect.getsource(reference)
assert _reference_source.count('        if trace is not None:')==1
_reference_source=_reference_source.replace('        if trace is not None:',
    "        if special is not None:special['macl']=macl\n        if trace is not None:")
_reference_namespace=dict(reference.__globals__)
exec(compile(_reference_source,'prefix_gate_observed_reference','exec'),_reference_namespace)
reference=_reference_namespace['reference']
STATES=(0,0x10,0x2B,0x2C,0x2D,0x2E,0x30,0x32,0x33,0x3D,0x3E,0x40,0x44,0x45,0x7F,0x80,0xFF)
RAW=(0,0x80000000,1,0x80000001,0x7FFFFF,0x800000,0x800001,0x3F800000,0xC0600000,
     0x7F7FFFFF,0x7F800000,0xFF800000,0x7FBFFFFF,0x7FC00000)


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(m,a,n=4):return sum(m[(a&0xFFFFFF)+i]<<(8*i) for i in range(n))
def store(m,a,v,n=4):
    for i in range(n):m[(a&0xFFFFFF)+i]=(v>>(8*i))&255


def multiply_expected(f,n,m):
    """Manual classes + independent finite integer arithmetic and enable policy."""
    bank=(f.fpscr>>21)&1;status=f.fpscr;a,b=f.banks[bank][n],f.banks[bank][m]
    result=scalar.exceptional(a,b,status,a)
    if result is not None:
        rc,after,out=result
        if rc!=-1:f.fpscr=after;f.banks[bank][n]=out
        return rc
    # Compute actual causes with enables disabled, then apply the documented
    # unconditional O/U/I enable policy without committing the result on trap.
    f.fpscr=status&~0xF80;scalar.finite_execute(f,n,m);f.fpscr|=status&0xF80
    if status&0x380:
        f.banks[bank][n]=(a&0x80000000) if status&0x40000 and (a&0x7FFFFFFF)<0x800000 else a
        return 1
    return 0


def prepare(c,number):
    memory=bytearray(BASELINE);state=State();state.r[:]=[(0x12340000+number+i*0x10201)&0xFFFFFFFF for i in range(16)]
    state.pc=START;state.pr=0x8C000500;state.sr=0x700003F0|(number&1)
    state.r[4]=OBJECT;state.r[5]=0x8CA20000;state.r[6]=0 if c.get('null_work') else WORK
    state.r[15]=0x8CF00000-(number%4)*0x100
    f=Fpu();f.fpscr=c.get('mode',0)|(0x40000 if c.get('dn',1) else 0)|c.get('rm',0)|(c.get('enable',0)<<7)|0x3F07C;f.fpul=0xCAFEBABE
    for b in range(2):f.banks[b][:]=[0xAB000000+b*0x10000+i*0x101 for i in range(16)]
    store(memory,OBJECT,c.get('state',0),1);store(memory,OBJECT+2,c.get('type',0),1)
    store(memory,OBJECT+4,c.get('flags',4),2);store(memory,OBJECT+6,0xAABB,2);store(memory,OBJECT+8,c.get('character',0xFF),1)
    store(memory,WORK+4,c.get('work_flags',0),2);store(memory,WORK+0x5C,c.get('work_trigger',0))
    for a,v in ((0x38,c.get('raw',0xC0600000)),(0x3C,0xA1B2C3D4),(0x40,0x11223344),(0xB4,0x7FC12345),
                (0xB8,0x92345678),(0xC4,c.get('raw',0xC0600000)),(0xC8,0x88776655),(0x124,0xA55A)):
        store(memory,WORK+a,v,2 if a==0x124 else 4)
    store(memory,sound.LOW,c.get('sound_cursor',0));store(memory,sound.HIGH,32)
    for i in range(37):
        for j in range(13):store(memory,sound.TABLE+i*52+j*4,0xDD000000|(i<<8)|j)
        store(memory,sound.TABLE+i*52,0x7FFFFFFF);store(memory,sound.TABLE+i*52+4,1 if c.get('sound_full') else 0)
    return memory,state,f


def cases():
    for typ,flags,null,trigger,mode in itertools.product((0,2),(0,4),(False,True),(0,0x10000),range(8)):
        if typ!=2 and flags&4 and null:continue # invalid original pointer precondition
        yield {'type':typ,'flags':flags,'null_work':null,'work_trigger':trigger,'mode':mode<<19,'state':0x2D}
    for state,wflags,mode,native,flags in itertools.product(STATES,(0,0x8000),range(8),(False,True),(0,4)):
        yield {'state':state,'flags':flags,'work_trigger':0x10000,'work_flags':wflags,'mode':mode<<19,'native_sound':native}
    for raw,dn,rm,bank,sz in itertools.product(RAW,range(2),range(2),range(2),range(2)):
        yield {'state':0x2D,'work_flags':0x8000,'mode':(bank<<21)|(sz<<20),'raw':raw,'dn':dn,'rm':rm}
    for raw,en,rm,bank in itertools.product((1,0x800000,0x3F800000,0x7F800000,0x7FC00000),range(32),range(2),range(2)):
        yield {'state':0x2D,'work_flags':0x8000,'mode':bank<<21,'raw':raw,'dn':0,'rm':rm,'enable':en}
    for state,mode,native in itertools.product((0x2C,0x2D,0x30,0x40),range(8),(False,True)):
        yield {'state':state,'mode':mode<<19,'native_sound':native,'flip':True,'character':0x80}
    for state,after,mode in itertools.product((0x30,0x40),(0x2D,0x2C,0x30,0x40),range(8)):
        yield {'state':state,'mode':mode<<19,'after_event_state':after,'character':0}
    for state,full,mode in itertools.product((0,0x2C,0x30,0x40),(False,True),range(8)):
        yield {'state':state,'mode':mode<<19,'native_sound':True,'sound_full':full,'sound_cursor':31,'observe':False}
    for state,mode in itertools.product((0x30,0x40,0x2D),range(8)):
        yield {'state':state,'mode':mode<<19,'null_service':True}
    for rm,branch in itertools.product((2,3),(0,0x8000)):
        yield {'state':0x2D,'work_flags':branch,'rm':rm}


class Stopped(Exception):pass


def run(c,number,inventory):
    memory,initial,fpu=prepare(c,number);before=bytes(memory)
    em=bytearray(memory);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    special={'macl':0xFEED1234};ee=[];path=[];wanted=0;errors=[];reached_calls=[]
    def event(t,r,sr,pr,f,fpul,macl,mem):
        return (t,tuple(r),sr,pr,bytes(f)[:132],fpul,macl,bytes(mem[(OBJECT&0xFFFFFF):(OBJECT&0xFFFFFF)+12]),
                tuple(load(mem,WORK+a) for a in (0x38,0x3C,0x40,0x124)))
    def stop(regs,sr,pr,pc,status):
        nonlocal wanted
        expected.r[:]=regs;expected.sr=sr;expected.pr=pr;expected.pc=pc;wanted=status;raise Stopped()
    def service(c,t,mem,r,sr,pr,f):
        if t==EVENT:
            assert r[4]==c.get('character',0xFF) and r[5]==7 and r[6]==0x3B and r[7]==6
            if 'after_event_state' in c:store(mem,OBJECT,c['after_event_state'],1)
        elif t==SPAWN:assert r[4]==OBJECT
        elif t==SET:assert r[4]==0 and r[5]==8
        elif t==sound.REQUEST:assert r[4] in (0x458,0x463) and r[5]==r[6]==r[7]==0
        else:assert t==CANCEL
        for i in range(8):r[i]=0xA0000000+i*0x1111+(t&0xFFFF)
        sr[0]^=1;store(mem,SCRATCH+(t&0xFF)*4,0xAA000000|t&0xFFFFFF)
        if c.get('flip'):f.fpscr^=0x380000
        # Non-native services do not claim FPUL/MACL effects through this ABI.
        f.banks[(f.fpscr>>21)&1][7]^=0x12345678
    def trace(pc,w,r,sr,pr,_f):
        assert pc in inventory,hex(pc);path.append(pc)
        if pc in (sound.REQUEST,sound.SELECT) and c.get('native_sound'):
            ee.append(event(pc,r,sr,pr,ef,ef.fpul,special['macl'],em))
        if pc==FNEG and ef.fpscr&0x80000 or pc==sound.FLDI and ef.fpscr&0x80000:stop(r,sr,pr,pc,-1)
        if pc==FMUL:
            probe=Fpu.from_buffer_copy(ef);rc=multiply_expected(probe,2,3)
            if rc:
                ctypes.memmove(ctypes.byref(ef),ctypes.byref(probe),ctypes.sizeof(Fpu));stop(r,sr,pr,pc,rc)
    def hook_for(t):
        def hook(mem,r,sr):
            ee.append(event(t,r,sr,expected.pr,ef,ef.fpul,special['macl'],mem));reached_calls.append(expected.pr-4)
            if c.get('null_service'):stop(r,sr,expected.pr,t,-2)
            s=[sr];p=[expected.pr];service(c,t,mem,r,s,p,ef)
            if expected.pr-4==c.get('fail_site'):stop(r,s[0],p[0],t,c.get('fail_code',-17))
            return s[0]
        return hook
    hooks={t:hook_for(t) for t in SERVICES if not(c.get('native_sound') and t==sound.REQUEST)}
    try:reference(em,expected,fpu=ef,special=special,trace=trace,hooks=hooks,instruction_budget=5000)
    except Stopped:pass
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr);fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    ae=[];ce=[]
    @Call
    def call(_ctx,t,mem,f,s,r,p):
        try:
            ce.append(p[0]-4)
            if not c.get('native_sound'):ae.append(event(t,r[:16],s[0],p[0],f.contents,fpul.value,macl.value,mem))
            service(c,t,mem,r,s,p,f.contents)
            return c.get('fail_code',-17) if p[0]-4==c.get('fail_site') else 0
        except Exception as exc:errors.append(repr(exc));return -99
    @OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event(t,r[:16],s[0],p[0],f.contents,fpul.value,macl.value,mem))
        except Exception as exc:errors.append(repr(exc))
    args=[(ctypes.c_uint8*RAM_SIZE).from_buffer(memory),ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,Call() if c.get('null_service') else call]
    result=connected(*args,observe if c.get('observe',True) else OBSERVE()) if c.get('native_sound') else direct(*args)
    assert result==wanted and not errors,(number,c,result,wanted,errors)
    assert memory==em and list(actual.r)==list(expected.r),(number,c,'RAM/GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl'],(number,c,'FPU/FPUL/MACL differs')
    expected_events=ee if c.get('observe',True) and not(c.get('null_service') and not c.get('native_sound')) else []
    assert ae==expected_events,(number,c,'boundary differs')
    invariant=False
    if wanted==0 and not c.get('flip') and 'after_event_state' not in c:
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        assert list(actual.r)[8:15]==list(initial.r)[8:15]
        active=c.get('type',0)!=2 and (c.get('flags',4)&4 or not c.get('null_work') and c.get('work_trigger',0)&0x10000)
        state=c.get('state',0);flags=c.get('flags',4)
        if not active:
            assert actual.r[0]==0 and load(memory,OBJECT,1)==state and load(memory,OBJECT+6,2)==0xAABB
        else:
            assert load(memory,OBJECT+6,2)==120
            if 46<=signed(state,8)<=50:
                assert actual.r[0]==1 and load(memory,OBJECT,1)==50 and load(memory,WORK+0x124,2)==46
            elif 62<=signed(state,8)<=68:
                assert actual.r[0]==0 and load(memory,OBJECT,1)==68 and load(memory,WORK+0x124,2)==110
                assert load(memory,OBJECT+4,2)==flags&0xFFFB
            elif c.get('work_flags',0)&0x8000 or state==45:
                assert actual.r[0]==0 and load(memory,OBJECT,1)==state
                if c.get('work_flags',0)&0x8000:
                    probe=Fpu.from_buffer_copy(fpu);bank=(probe.fpscr>>21)&1;probe.banks[bank][2]=load(before,WORK+0x38);probe.banks[bank][3]=0x3F000000
                    assert multiply_expected(probe,2,3)==0
                    assert load(memory,WORK+0x38)==probe.banks[bank][2]
                    if fpu.fpscr&0x100000:assert load(memory,WORK+0x3C)==0x3F000000
            else:
                assert actual.r[0]==1 and load(memory,OBJECT,1)==16 and load(memory,WORK+0x124,2)==23
                assert load(memory,OBJECT+4,2)==flags&0xFAFB
                raw=load(before,WORK+0xC4)
                assert load(memory,WORK+0x38)==(raw if fpu.fpscr&0x100000 else raw^0x80000000)
                assert load(memory,WORK+0x3C)==load(before,WORK+(0xC8 if fpu.fpscr&0x100000 else 0xB4))
        invariant=True
    edges=set()
    for i,a in enumerate(path):
        if not START<=a<END:continue
        word=inventory[a]
        if word>>8 in (0x89,0x8B,0x8D,0x8F):
            nxt=i+(2 if word>>8 in (0x8D,0x8F) else 1)
            if nxt<len(path):edges.add((a,path[nxt]))
    return path,edges,reached_calls,len(ae),wanted,invariant


def main():
    sh4_binary32.execute_multiply=multiply_expected
    manifest_path=PROJECT/'analysis/main_dispatch_prefix_gate_translation.json';manifest=json.loads(manifest_path.read_text())
    for p,h in manifest['sha256'].items():assert digest(Path(p))==h,p
    for name in ('main_dispatch_sound_verification.json','sh4_multiply_extended_verification.json'):
        for p,h in json.loads((PROJECT/'analysis'/name).read_text())['sha256'].items():assert digest(Path(p))==h,p
    gate_rows={int(a,16) for a in manifest['reachable_addresses']};allowed=gate_rows.copy()
    for r in json.loads((PROJECT/'analysis/main_dispatch_sound_translation.json').read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    inventory={a:int.from_bytes(sound.binary[a-0x8C010000:a-0x8C010000+2],'little') for a in allowed}
    rows=set();edges=set();calls=set();boundaries=partials=invariants=native=0;records=[];failure_inputs={}
    def check(c,number):
        nonlocal boundaries,partials,invariants,native
        path,e,cs,b,status,inv=run(c,number,inventory);rows.update(a for a in path if a in gate_rows);edges.update(e);calls.update(cs)
        boundaries+=b;partials+=status!=0;invariants+=inv;native+=bool(c.get('native_sound'))
        for site in cs:
            if site in {int(a,16) for a in manifest['external_call_sites']} and not c.get('native_sound') and status==0:failure_inputs.setdefault(site,c)
        records.append({'input':c,'steps':len(path),'boundary_comparisons':b,'status':status,'independent_invariant':inv,'full_state_match':True})
    for number,c in enumerate(cases()):
        check(c,number)
        if (number+1)%300==0:print(number+1,'prefix gate comparisons passed',flush=True)
    assert rows==gate_rows,('missing rows',[hex(a) for a in gate_rows-rows])
    assert edges=={tuple(int(a,16) for a in edge) for edge in manifest['conditional_edges']},('edge coverage',len(edges))
    assert calls=={int(a,16) for a in manifest['external_call_sites']}
    assert len(failure_inputs)==7
    for site,c in sorted(failure_inputs.items()):
        for code in (-17,-2):check(dict(c,fail_site=site,fail_code=code),len(records))
    files=[Path(__file__),manifest_path,PROJECT/'build_main_dispatch_prefix_gate.ps1',PROJECT/'source/sa_main_dispatch_prefix_gate.c',
           PROJECT/'source/sa_main_dispatch_prefix_gate_connected.c',PROJECT/'source/sa_main_dispatch_sound.c',PROJECT/'verify_startup.py',
           PROJECT/'sh4_binary32.py',PROJECT/'sh4_multiply_extended_reference.py',PROJECT/'analysis/main_dispatch_sound_verification.json',
           PROJECT/'analysis/sh4_multiply_extended_verification.json',PROJECT/'source/sh4_multiply_extended.c',PROJECT/'source/sh4_multiply.c',PROJECT/'source/sh4_fmac.c']
    report={**{k:manifest[k] for k in ('name','address','end_exclusive','instruction_bytes','retail_span_sha256','source')},
        'source_sha256':digest(PROJECT/manifest['source']),'cases_passed':len(records),'covered_rows':len(rows),'conditional_edges_covered':len(edges),
        'external_call_sites_covered':len(calls),'boundary_comparisons':boundaries,'partial_guard_trap_error_cases':partials,
        'independent_invariants':invariants,'native_sound_cases':native,'service_error_cases':14,
        'sha256':{str(p):digest(p) for p in files},'dll_sha256':digest(DLL),'binary_sha256':digest(BINARY),
        'scope':'153 original rows, all branches/calls; complete RAM/CPU/FPU/FPUL/MACL and ordered boundaries, independent gate/state/vector invariants; native sound request/selector integration.',
        'limitations':['Event 8c109fde, spawn 8c07b320, cancellation 8c01ae60 and setter 8c07b2b8 remain explicit deterministic services.',
                      'PR=1 stops only at reached unsupported FMUL/FNEG or native sound FLDI0. RM2/3 stops only at reached FMUL. CPU trap delivery/MMIO/invalid pointers/concurrency are not modeled.',
                      'Software/manual arithmetic evidence, not physical hardware or full gameplay/port validation. Names and field meanings are inferred.'],
        'cases':records}
    (PROJECT/'analysis/main_dispatch_prefix_gate_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('cases_passed','covered_rows','conditional_edges_covered','external_call_sites_covered','boundary_comparisons','partial_guard_trap_error_cases','independent_invariants','native_sound_cases','service_error_cases')},indent=2))


if __name__=='__main__':
    try:main()
    finally:sh4_binary32.execute_multiply=scalar.finite_execute
