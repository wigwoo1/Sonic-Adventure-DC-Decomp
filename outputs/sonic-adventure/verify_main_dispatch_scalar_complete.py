"""Extended FMUL integration: real numerical paths and zero normalization.

Retail control flow uses the separately compiled direct upstream primitive API
as its numerical oracle. That is an integration comparison with shared math,
not new hardware validation. Analytic checks and the backend bound suite add
independent numerical evidence. The native entry points accept no math fixture.
"""
import ctypes,hashlib,itertools,json,math,struct
from pathlib import Path
import verify_main_dispatch_vector_calc as previous
import verify_sh4_vector_numeric as numeric
import sh4_binary32
import sh4_multiply_extended_reference as scalar
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call

PROJECT=Path(__file__).resolve().parent
axes,prior,chain,base,active=previous.axes,previous.prior,previous.chain,previous.base,previous.active
load,store=base.load,base.store
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_scalar_complete.dll'
LIB=ctypes.CDLL(str(DLL))
functions={k:getattr(LIB,'sa_main_dispatch_scalar_axis_'+k+'_connected') for k in axes.AXES}
functions['calc']=LIB.sa_main_dispatch_scalar_vector_calc_connected
functions.update({k:getattr(LIB,'sa_main_dispatch_scalar_'+k+'_connected') for k in ('transform','detach','status','flag')})
for fn in functions.values():fn.argtypes=active.pure_args[:-1]+[Call,active.OBSERVE];fn.restype=ctypes.c_int
OPCODES={word:(kind,n,m) for kind,n,m,word in numeric.SPECS}
TRACKED=previous.TRACKED
IDENTITY=[0x3F800000 if i%5==0 else 0 for i in range(16)]
VECTORS=((1.25,-2.0,0.75),(1.0,1.0,0.0),(-0.5,0.25,2.0))


def bits(value):return struct.unpack('<I',struct.pack('<f',value))[0]
def value(raw):return struct.unpack('<f',struct.pack('<I',raw))[0]
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def cases():
    for axis,explicit,angle,bank,rm in itertools.product(axes.AXES,(False,True),axes.ANGLES,range(2),range(2)):
        yield {'entry':axis,'explicit':explicit,'angle':angle,'mode':bank<<21,'rm':rm}
    for explicit,normalize,index,bank,rm,alias in itertools.product((False,True),(0,1),range(3),range(2),range(2),(False,True)):
        yield {'entry':'calc','explicit':explicit,'normalize':normalize,'vector':index,'mode':bank<<21,'rm':rm,'alias':alias}
    for entry,angles,normalize,bank,rm in itertools.product(('transform','detach','status','flag'),chain.ANGLES,(0,1),range(2),range(2)):
        yield {'entry':entry,'angles':angles,'normalize':normalize,'mode':bank<<21,'rm':rm,'null_external':entry!='status'}
    for entry,mode in itertools.product(functions,(0x80000,0x100000,0x180000,0x280000,0x300000,0x380000)):
        yield {'entry':entry,'angles':(1,2,3),'angle':1,'normalize':1,'mode':mode}
    for entry,rm in itertools.product(functions,(2,3)):
        yield {'entry':entry,'angles':(1,2,3),'angle':1,'normalize':1,'mode':0,'rm':rm}
    for axis,enable,bank in itertools.product(axes.AXES,(1,2,4,16),range(2)):
        yield {'entry':axis,'angle':1,'mode':bank<<21,'enable':enable}
    for entry,enable,bank in itertools.product(('calc','transform','detach','status','flag'),(1,2,4,16),range(2)):
        yield {'entry':entry,'angles':(0,0,0),'normalize':0,'mode':bank<<21,'enable':enable}
    for explicit,bank in itertools.product((False,True),range(2)):
        yield {'entry':'calc','explicit':explicit,'zero':True,'normalize':1,'enable':8,'mode':bank<<21}
        yield {'entry':'calc','explicit':explicit,'zero':True,'normalize':1,'mode':bank<<21}
    for entry,bank in itertools.product(('transform','detach','status','flag'),range(2)):
        yield {'entry':entry,'angles':(1,2,3),'normalize':1,'mode':bank<<21,'observe':False,'null_external':entry!='status'}
    for failure,bank in itertools.product(('missing','service_error'),range(2)):
        yield {'entry':'status','angles':(1,2,3),'normalize':1,'mode':bank<<21,'sound_fail':failure,'null_external':failure=='missing'}


    for explicit,normalize,bank,rm,dn,alias,signed in itertools.product((False,True),(0,1),range(2),range(2),range(2),(False,True),(False,True)):
        yield {'entry':'calc','explicit':explicit,'zero':True,'normalize':normalize,'mode':bank<<21,'rm':rm,'dn':dn,'alias':alias,'signed_zero':signed}
    for angles,normalize,bank,rm,dn in itertools.product(((0,0,0),(0x1234,0x5678,0x9ABC)),(0,1),range(2),range(2),range(2)):
        yield {'entry':'transform','angles':angles,'zero':True,'normalize':normalize,'mode':bank<<21,'rm':rm,'dn':dn,'null_external':True}
    for bank,rm,dn in itertools.product(range(2),range(2),range(2)):
        yield {'entry':'calc','explicit':True,'zero':True,'normalize':1,'mode':bank<<21,'rm':rm,'dn':dn,'enable':16}


def prepare(c,number):
    direct=c['entry'] in axes.AXES or c['entry']=='calc'
    memory,state,fpu=prior.connected_prepare(dict(c,entry='transform' if direct else c['entry']),number)
    fpu.fpscr=c['mode']|(0x40000 if c.get('dn',1) else 0)|c.get('rm',0)|(c.get('enable',0)<<7)
    for b in range(2):fpu.banks[b][:]=[bits((i+1)*0.125*(1 if b==0 else -1)) for i in range(16)]
    bank=(fpu.fpscr>>21)&1;fpu.banks[bank^1][:]=IDENTITY
    store(memory,previous.NORMALIZE,c.get('normalize',0))
    vector=(0.0,0.0,0.0) if c.get('zero') else VECTORS[c.get('vector',0)]
    if c['entry'] in axes.AXES:
        state.pc=axes.ADDRESS[c['entry']];state.r[4]=previous.MATRIX if c.get('explicit') else 0
        state.r[5]=c.get('angle',1)
    elif c['entry']=='calc':
        state.pc=previous.START;state.r[4]=previous.MATRIX if c.get('explicit') else 0
        state.r[5]=previous.SOURCE;state.r[6]=previous.SOURCE if c.get('alias') else previous.DEST
    elif c['entry']=='transform':state.r[5]=chain.VECTOR
    for i,v in enumerate(IDENTITY):store(memory,previous.MATRIX+4*i,v)
    for at in (previous.SOURCE,chain.VECTOR):
        for i,v in enumerate(vector):store(memory,at+4*i,bits(v)|(0x80000000 if c.get('signed_zero') and i%2==0 else 0))
    return memory,state,fpu,vector


def stop_information(c,initial):
    entry=c['entry'];rm=c.get('rm',0);enable=c.get('enable',0)
    if c.get('sound_fail'):return base.SOUND,(-2 if c['sound_fail']=='missing' else -17),False
    if entry in axes.AXES:
        if c['mode']&0x180000:return initial.pc,-1,False
        sites=[int(a,16) for a in axes.AXES[entry]['explicit_vector_sites' if c.get('explicit') else 'implicit_vector_sites']]
        if rm>1:return sites[0],-1,False
        if enable:return sites[0 if enable==1 else 1],1,True
        return None,0,False
    if entry!='calc':
        if entry!='transform' and c['mode']&0x80000:return 0x8C0B1DA8,-1,False
        if c['mode']&0x80000:return prior.FLDI,-1,False
        x,y,z=c.get('angles',(0,0,0))
        first='z' if z else 'x' if x else 'y' if y else None
        if c['mode']&0x180000:return axes.ADDRESS[first] if first else previous.START,-1,False
        if first and rm>1:return int(axes.AXES[first]['implicit_vector_sites'][0],16),-1,False
    elif c['mode']&0x180000:return previous.START,-1,False
    first=0x8C638F44 if c.get('explicit') else 0x8C638F36
    if rm>1:return first,-1,False
    if c.get('zero') and c.get('normalize') and enable==16 and c.get('explicit'):return previous.FMUL,-1,True
    if c.get('zero') and enable==8:return 0x8C638F7C,-1,True
    if enable:return first,-1,True
    return None,0,False


def run_case(c,number,allowed):
    memory,initial,fpu,input_vector=prepare(c,number)
    em=bytearray(memory);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    path=[];ee=[];ec={};special={'macl':0xFEED1234};op_count=0
    def event(target,r,sr,pr,f,fpul):return target,tuple(r),sr,pr,bytes(f)[:132],fpul
    def trace(pc,word,r,sr,pr,_f):
        assert pc in allowed,hex(pc)
        path.append(pc)
        if pc in TRACKED and pc!=initial.pc:ee.append(event(pc,r,sr,pr,ef,ef.fpul))
    def primitive(word,f):
        nonlocal op_count
        op_count+=1;kind,n,m=OPCODES[word]
        status=numeric.primitive(kind,n,m,ctypes.byref(f),f.fpul)
        assert status==0,(number,c,hex(word),'unexpected primitive trap')
    def sound(mem,r,sr,pr,f,counters):
        counters['sound']=counters.get('sound',0)+1
        base.apply_service(dict(c,character=load(mem,base.OBJECT+8,1)),base.SOUND,mem,r,sr,pr,f,{})
    def sound_hook(mem,r,sr):
        ee.append(event(base.SOUND,r,sr,expected.pr,ef,ef.fpul));s=[sr];p=[expected.pr]
        sound(mem,r,s,p,ef,ec);return s[0]
    stop,wanted,trap=stop_information(c,initial)
    steps=reference(em,expected,fpu=ef,special=special,trace=trace,stop_pc=stop,
        hooks={base.SOUND:sound_hook},vector_instruction=primitive,instruction_budget=4000)
    if stop in TRACKED and stop!=initial.pc or c.get('sound_fail'):
        ee.append(event(stop,list(expected.r),expected.sr,expected.pr,ef,ef.fpul))
    if trap:
        word=int.from_bytes(base.binary[stop-0x8C010000:stop-0x8C010000+2],'little')
        if stop==previous.FMUL:
            assert scalar.execute(ef,(word>>8)&15,(word>>4)&15)==1
        else:
            kind,n,m=OPCODES[word]
            assert numeric.primitive(kind,n,m,ctypes.byref(ef),ef.fpul)==1
    if c.get('sound_fail')=='service_error':
        r=list(expected.r);s=[expected.sr];p=[expected.pr]
        sound(em,r,s,p,ef,ec);expected.r[:]=r;expected.sr=s[0]
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    ae=[];ac={};errors=[]
    @active.OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event(t,list(r[:16]),s[0],p[0],f.contents,fpul.value))
        except Exception as exc:errors.append(repr(exc))
    @Call
    def call(_ctx,target,mem,f,s,r,p):
        try:
            assert target==base.SOUND,hex(target)
            if c.get('sound_fail')=='missing':return -2
            sound(mem,r,s,p,f.contents,ac)
            return -17 if c.get('sound_fail')=='service_error' else 0
        except Exception as exc:errors.append(repr(exc));return -99
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    result=functions[c['entry']](view,ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,
        Call() if c.get('null_external') or c['entry'] in (*axes.AXES,'calc') else call,
        observe if c.get('observe',True) else active.OBSERVE())
    assert result==wanted and not errors,(number,c,result,wanted,errors)
    assert memory==em and list(actual.r)==list(expected.r),(number,c,'RAM/GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl'],(number,c,'FPU/FPUL/MACL differs')
    assert ac==ec and ae==(ee if c.get('observe',True) else []),(number,c,'native boundary differs')
    checked=False;max_error=0.0
    if wanted==0:
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        if c.get('zero') and c.get('normalize'):
            dest=(previous.SOURCE if c.get('alias') else previous.DEST) if c['entry']=='calc' else chain.VECTOR
            assert [load(memory,dest+4*i) for i in range(3)]==[0x7FBFFFFF]*3,(number,c,'expected SH-4 quiet NaNs')
            assert af.fpscr&0x10060==0x10060,(number,c,'missing invalid/divide-by-zero status')
            return path,steps,len(ae),op_count,wanted,False,0.0
        if c['entry'] in axes.AXES:
            theta=(c.get('angle',1)&0xFFFF)*(2*math.pi/65536);s,co=math.sin(theta),math.cos(theta)
            matrices={'x':[1,0,0,0,0,co,s,0,0,-s,co,0,0,0,0,1],
                      'y':[co,0,-s,0,0,1,0,0,s,0,co,0,0,0,0,1],
                      'z':[co,s,0,0,-s,co,0,0,0,0,1,0,0,0,0,1]}
            b=((fpu.fpscr>>21)&1)^1
            got=[value(load(memory,previous.MATRIX+4*i)) for i in range(16)] if c.get('explicit') else [value(x) for x in af.banks[b]]
            max_error=max(abs(a-b) for a,b in zip(got,matrices[c['entry']]))
            assert max_error<3e-6,(number,c,'analytic axis mismatch',got,matrices[c['entry']])
        else:
            vector=input_vector if c['entry'] in ('calc','transform') else (1.0,1.0,0.0)
            if c['entry']!='calc':
                x,y,z=c['angles'];x,y,z=(a*(2*math.pi/65536) for a in (x&0xFFFF,(-y)&0xFFFF,z&0xFFFF))
                # M = Rz * Rx * Ry(-y): apply rightmost rotation first.
                vx,vy,vz=vector
                vx,vz=math.cos(y)*vx+math.sin(y)*vz,-math.sin(y)*vx+math.cos(y)*vz
                vy,vz=math.cos(x)*vy-math.sin(x)*vz,math.sin(x)*vy+math.cos(x)*vz
                vector=(math.cos(z)*vx-math.sin(z)*vy,math.sin(z)*vx+math.cos(z)*vy,vz)
            if c.get('normalize'):
                length=math.sqrt(sum(v*v for v in vector));vector=tuple(v/length for v in vector)
            dest=(previous.SOURCE if c.get('alias') else previous.DEST) if c['entry']=='calc' else chain.VECTOR if c['entry']=='transform' else chain.previous.ALT_DEST+4
            got=tuple(value(load(memory,dest+4*i)) for i in range(3))
            max_error=max(abs(a-b) for a,b in zip(got,vector))
            assert max_error<8e-6,(number,c,'analytic vector mismatch',got,vector)
        checked=True
    return path,steps,len(ae),op_count,wanted,checked,max_error


def main():
    sh4_binary32.execute_multiply=scalar.execute # process-local exceptional FMUL oracle; original module file untouched
    report_path=PROJECT/'analysis/sh4_vector_numeric_verification.json'
    scalar_path=PROJECT/'analysis/sh4_multiply_extended_verification.json'
    scalar_report=json.loads(scalar_path.read_text())
    for p,h in scalar_report['sha256'].items():assert digest(Path(p))==h,p
    backend=json.loads(report_path.read_text())
    for p,h in backend['sha256'].items():assert digest(Path(p))==h,p
    allowed=set(previous.ROWS)|set().union(*axes.ROWSETS.values())|set(range(chain.POP,0x8C639BD4,2))
    dependencies=['main_dispatch_shared_translation.json','main_dispatch_active_translation.json','main_dispatch_detach_translation.json']
    for name in dependencies:
        for r in json.loads((PROJECT/'analysis'/name).read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    for name in ('main_dispatch_transform_translation.json','main_dispatch_matrix_init_translation.json','main_dispatch_shared_check_translation.json'):
        allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis'/name).read_text())['reachable_addresses'])
    allowed.update(range(active.QUERY,0x8C06CEFE,2));allowed.update(range(base.RESET,0x8C05E7D4,2))
    counts={k:0 for k in functions};records=[];boundaries=0;ops=0;analytic=0;partial=0;closed=0;max_error=0.0
    for number,c in enumerate(cases()):
        path,steps,b,n,status,check,error=run_case(c,number,allowed)
        counts[c['entry']]+=1;boundaries+=b;ops+=n;analytic+=check;partial+=status!=0;max_error=max(max_error,error)
        closed+=bool(c.get('null_external')) and status==0
        records.append({'input':c,'steps':steps,'native_boundaries':b,'numerical_operations':n,'status':status,
                        'analytic_check':check,'max_absolute_error':error,'full_state_match':True})
        if (number+1)%150==0:print(number+1,'real numerical native-chain comparisons passed',flush=True)
    files=[Path(__file__),report_path,scalar_path,PROJECT/'sh4_multiply_extended_reference.py',PROJECT/'build_main_dispatch_scalar_complete.ps1',numeric.VENDOR,
           PROJECT/'verify_main_dispatch_vector_calc.py',PROJECT/'verify_main_dispatch_axes.py',PROJECT/'verify_main_dispatch_matrix_init.py',
           PROJECT/'verify_main_dispatch_transform.py',PROJECT/'verify_main_dispatch_detach.py',PROJECT/'verify_main_dispatch_active_query.py',
           PROJECT/'verify_main_dispatch_shared_helpers.py',PROJECT/'verify_startup.py',PROJECT/'sh4_binary32.py',PROJECT/'verify_sh4_vector_numeric.py']
    for name in ('sa_main_dispatch_scalar_complete_connected.c','model_direction_transform_extended.c','sh4_multiply_extended.c',
                 'sh4_multiply_extended.h','sh4_vector_numeric.c','sh4_vector_numeric.h','model_direction_transform.c',
                 'sa_main_dispatch_matrix_init.c','sa_main_dispatch_transform.c','sa_main_dispatch_detach.c','sa_main_dispatch_shared_helpers.c',
                 'sa_main_dispatch_status_query.c','sa_main_dispatch_shared_check.c','sa_main_dispatch_active_query.c','sa_main_dispatch_reset.c',
                 'sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','model_matrix_stack_pop.c','sh4_float_i32.c',
                 'sh4_multiply.c','sh4_fmac.c','sh4_fmac.h','fpu_cleanup_transfer.h'):
        files.append(PROJECT/'source'/name)
    report={'cases_passed':len(records),'counts':counts,'native_boundary_comparisons':boundaries,'numerical_operations_compared':ops,
            'analytic_rotation_vector_checks':analytic,'partial_guard_trap_error_cases':partial,'closed_null_external_cases':closed,
            'max_analytic_absolute_error':max_error,'backend_verification':'analysis/sh4_vector_numeric_verification.json','scalar_verification':'analysis/sh4_multiply_extended_verification.json',
            'zero_normalization_completed_cases':sum(r['input'].get('zero',False) and r['input'].get('normalize',0) and r['status']==0 for r in records),
            'scalar_invalid_trap_cases':sum(r['input'].get('zero',False) and r['input'].get('enable')==16 and r['status']==-1 for r in records),
            'sha256':{str(p):digest(p) for p in files},'dll_sha256':digest(DLL),
            'scope':'New APIs have no numerical fixture callback. Real FSCA/FTRV/FIPR/FSRRA through axes/calculation and four native parent chains, analytic rotations/vectors, normalization, alias, enabled traps and exact retained partial state.',
            'limitations':['Retail CPU and native code share the pinned upstream arithmetic through separate primitive/adapter binaries; differential integration does not independently prove bit-exact hardware arithmetic.',
                           'Analytic checks and backend error/invariant tests are independent local evidence. Upstream physical-hardware claims were not replayed locally.',
                           'Legacy entry PR/SZ guards remain; numerical adapter rejects reserved RM. Actual CPU trap delivery is not emulated.',
                           'Exceptional FMUL is checked against a manual-derived class oracle. Zero normalization produces canonical SH-4 quiet NaNs, not a fabricated finite direction; invalid-enabled partial state is retained.',
                           'Sound remains an external game service in STATUS. Full game/ports, MMIO, timing, arbitrary invalid pointers and all gameplay states remain unfinished.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_scalar_complete_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('cases_passed','counts','native_boundary_comparisons','numerical_operations_compared',
        'analytic_rotation_vector_checks','partial_guard_trap_error_cases','closed_null_external_cases','max_analytic_absolute_error','zero_normalization_completed_cases','scalar_invalid_trap_cases')},indent=2))
    sh4_binary32.execute_multiply=scalar.finite_execute


if __name__=='__main__':
    try:main()
    finally:sh4_binary32.execute_multiply=scalar.finite_execute
