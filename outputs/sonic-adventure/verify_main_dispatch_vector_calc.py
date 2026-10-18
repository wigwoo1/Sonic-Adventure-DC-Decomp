"""Verify existing vector calculation and closed game-call transform chains."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import verify_main_dispatch_axes as axes
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call

PROJECT=Path(__file__).resolve().parent
prior,chain,base,active=axes.prior,axes.chain,axes.base,axes.active
load,store=base.load,base.store
START,END,NORMALIZE,FMUL=0x8C638F28,0x8C638F8C,0x8C890070,0x8C638F7E
SOURCE,DEST,MATRIX=0x8CE80000,0x8CE81000,0x8CE82000
MANIFEST=PROJECT/'analysis/main_dispatch_vector_calc_inventory.json'
inventory=json.loads(MANIFEST.read_text())
ROWS={int(a,16) for a in inventory['reachable_addresses']}
SITES={int(a,16):int(w,16) for a,w in inventory['vector_instruction_sites'].items()}
ALL_SITES={**axes.VECTOR_SITES,**SITES}
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_vector_calc.dll'
LIB=ctypes.CDLL(str(DLL))
functions={'calc':LIB.sa_main_dispatch_vector_calc_connected}
functions.update({k:getattr(LIB,'sa_main_dispatch_calc_'+k+'_connected') for k in ('transform','detach','status','flag')})
for fn in functions.values():fn.argtypes=active.pure_args[:-1]+[Call,axes.Instruction,active.OBSERVE];fn.restype=ctypes.c_int
TRACKED=axes.TRACKED|{START}


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def vector_sites(c):
    result=[0x8C638F44,0x8C638F54,0x8C638F66] if c.get('explicit') else [0x8C638F36]
    return result+([0x8C638F7A,0x8C638F7C] if c.get('normalize',0) else [])


def cases():
    for explicit,normalize,rm,bank,alias,seed in itertools.product((False,True),(0,1,0xFFFFFFFF),range(4),range(2),range(6),range(3)):
        yield {'entry':'calc','explicit':explicit,'normalize':normalize,'rm':rm,'mode':bank<<21,'alias':alias,'seed':seed}
    for explicit,normalize,mode in itertools.product((False,True),(0,1),(0x80000,0x100000,0x180000,0x280000,0x300000,0x380000)):
        yield {'entry':'calc','explicit':explicit,'normalize':normalize,'mode':mode}
    for explicit,bank,rm,factor in itertools.product((False,True),range(2),(0,1),(0,0x80000000,0x3F800000,0x3FA00000,0x3F800001,0xBF800000,1)):
        yield {'entry':'calc','explicit':explicit,'normalize':1,'mode':bank<<21,'rm':rm,'factor':factor}
    for explicit,normalize,bank,kind in itertools.product((False,True),(0,1),range(2),('missing','service_error','null')):
        c={'entry':'calc','explicit':explicit,'normalize':normalize,'mode':bank<<21,'calc_fail':kind}
        for n in range(1,len(vector_sites(c))+1):
            if kind=='null' and n!=1:continue
            yield dict(c,calc_n=n,null_instruction=kind=='null')
    for explicit,normalize,bank in itertools.product((False,True),(0,1),range(2)):
        yield {'entry':'calc','explicit':explicit,'normalize':normalize,'mode':bank<<21,'observe':False}
    for entry,angles,normalize,mode,rm in itertools.product(('transform','detach','status','flag'),chain.ANGLES,(0,1),
            (0,0x100000,0x200000,0x300000),(0,1)):
        yield {'entry':entry,'angles':angles,'normalize':normalize,'mode':mode,'rm':rm}
    for entry,angles,mode,pair in itertools.product(('transform','detach','status','flag'),((0,0,0),(1,2,3)),range(8),((0,8),(8,8))):
        yield {'entry':entry,'angles':angles,'normalize':1,'mode':mode<<19,'depth':pair[0],'limit':pair[1]}
    for entry,normalize,rm,bank in itertools.product(('transform','detach','status','flag'),(0,1),(2,3),range(2)):
        yield {'entry':entry,'angles':(1,2,3),'normalize':normalize,'mode':bank<<21,'rm':rm}
    for entry,n,kind,bank in itertools.product(('transform','detach','status','flag'),(1,2,3),('missing','service_error','null'),range(2)):
        if kind=='null' and n!=1:continue
        yield {'entry':entry,'angles':(0,0,0) if kind=='null' else (1,2,3),'normalize':1,'mode':bank<<21,
               'calc_fail':kind,'calc_n':n,'null_instruction':kind=='null'}
    for entry,angles,normalize,bank in itertools.product(('transform','detach','flag'),((0,0,0),(1,2,3)),(0,1),range(2)):
        yield {'entry':entry,'angles':angles,'normalize':normalize,'mode':bank<<21,'null_external':True}
    for kind,normalize,bank in itertools.product(('missing','service_error'),(0,1),range(2)):
        yield {'entry':'status','angles':(1,2,3),'normalize':normalize,'mode':bank<<21,'sound_fail':kind,'null_external':kind=='missing'}
    for entry,flags,mode in itertools.product(('detach','status','flag'),(0,0x800),range(8)):
        yield {'entry':entry,'flags':flags,'mode':mode<<19,'no_transform':True,'null_instruction':True,'null_external':entry!='status'}
    for entry,normalize,bank in itertools.product(('transform','detach','status','flag'),(0,1),range(2)):
        yield {'entry':entry,'angles':(1,2,3),'normalize':normalize,'mode':bank<<21,'observe':False}


def guard_target(c):
    if c.get('calc_fail') or c.get('sound_fail') or c.get('no_transform'):return None
    if c['entry']=='calc':
        if c['mode']&0x180000:return START
    else:
        if c['entry']!='transform' and c['mode']&0x80000:return 0x8C0B1DA8
        if c['mode']&0x80000 and prior.signed(c.get('depth',2))<prior.signed(c.get('limit',8)):return prior.FLDI
        if c['mode']&0x180000:
            x,y,z=c['angles']
            return axes.ADDRESS['z'] if z else axes.ADDRESS['x'] if x else axes.ADDRESS['y'] if y else START
    return FMUL if c.get('normalize',0) and c.get('rm',0)>1 else None


def prepare(c,number):
    direct=c['entry']=='calc'
    memory,state,fpu=prior.connected_prepare(dict(c,entry='transform' if direct else c['entry']),number)
    store(memory,NORMALIZE,c.get('normalize',0));fpu.fpscr|=c.get('rm',0)
    if direct:
        state.pc=START;state.r[4]=MATRIX if c.get('explicit') else 0;state.r[5]=SOURCE
        state.r[6]=(DEST,SOURCE,SOURCE+4,SOURCE-4,NORMALIZE-4,MATRIX+8)[c.get('alias',0)]
        for i,value in enumerate(prior.PATTERNS):store(memory,MATRIX+4*i,value^(c.get('seed',0)*0x10001))
        for i,value in enumerate((0x3F800001,0x80000000,0x7FC01234)):
            store(memory,SOURCE+4*i,value^(c.get('seed',0)*0x1111))
    return memory,state,fpu


def run_case(c,number,allowed):
    memory,initial,fpu=prepare(c,number)
    em=bytearray(memory);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    path=[];ee=[];ec={};special={'macl':0xFEED1234};latest=None
    def event(kind,target,mem,r,sr,pr,f,fpul):
        return (kind,target,tuple(r),sr,pr,bytes(f)[:132],fpul,load(mem,chain.DEPTH),load(mem,chain.STACK))
    def trace(pc,word,r,sr,pr,_f):
        nonlocal latest
        assert pc in allowed,hex(pc)
        path.append(pc);latest=(pc,r,sr,pr)
        if pc in TRACKED and pc!=initial.pc:ee.append(event('call',pc,em,r,sr,pr,ef,ef.fpul))
    def primitive(word,f,fpul):
        bank=(f.fpscr>>21)&1
        if word==0xF0FD:
            f.banks[bank][0]=0x3F000001^(fpul&0xFFFF);f.banks[bank][1]=0x3F800002^((fpul&0xFFFF)<<3)
        elif word in (0xF1FD,0xF5FD):
            start=(word>>8)&12;v=list(f.banks[bank][start:start+4]);m=list(f.banks[bank^1])
            f.banks[bank][start:start+4]=[(0x3F000000|((x^m[(i*5)%16]^fpul)&0x3FFFFF))|((i&1)<<31) for i,x in enumerate(v)]
        elif word in (0xF4ED,0xF8ED,0xF0ED):
            start=(word>>8)&12
            mix=0
            for x in f.banks[bank][:4]:mix^=x
            for x in f.banks[bank][start:start+4]:mix^=x>>1
            f.banks[bank][start+3]=0x3F800000|(mix&0x3FFFFF)
        elif word==0xF37D:f.banks[bank][3]=c.get('factor',0x3FA00000)
        else:raise AssertionError(hex(word))
        f.fpscr^=0x10040 # Flag effects only; instruction arithmetic remains a fixture.
    def reference_vector(word,f):
        pc,r,sr,pr=latest;ee.append(event('instruction',word,em,r,sr,pr,f,f.fpul));primitive(word,f,f.fpul)
    def sound(mem,r,sr,pr,f,counters):
        counters['sound']=counters.get('sound',0)+1
        base.apply_service(dict(c,character=load(mem,base.OBJECT+8,1)),base.SOUND,mem,r,sr,pr,f,{})
    def sound_hook(mem,r,sr):
        ee.append(event('call',base.SOUND,mem,r,sr,expected.pr,ef,ef.fpul));s=[sr];p=[expected.pr]
        sound(mem,r,s,p,ef,ec);return s[0]
    guard=guard_target(c);stop=guard
    if c.get('calc_fail'):stop=vector_sites(c)[c['calc_n']-1]
    if c.get('sound_fail'):stop=base.SOUND
    steps=reference(em,expected,fpu=ef,special=special,trace=trace,stop_pc=stop,
                    hooks={base.SOUND:sound_hook},vector_instruction=reference_vector,instruction_budget=4000)
    if c.get('calc_fail'):
        if not c.get('null_instruction'):ee.append(event('instruction',SITES[stop],em,list(expected.r),expected.sr,expected.pr,ef,ef.fpul))
        if c['calc_fail']=='service_error':primitive(SITES[stop],ef,ef.fpul)
    elif c.get('sound_fail') or guard in {*axes.ADDRESS.values(),START,chain.POP} and guard!=initial.pc:
        ee.append(event('call',stop,em,list(expected.r),expected.sr,expected.pr,ef,ef.fpul))
    if c.get('sound_fail')=='service_error':
        r=list(expected.r);s=[expected.sr];p=[expected.pr]
        sound(em,r,s,p,ef,ec);expected.r[:]=r;expected.sr=s[0]
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    ae=[];ac={};errors=[];op_count=0
    prefix=0 if c['entry']=='calc' else 3*sum(bool(v) for v in c.get('angles',(0,0,0)))
    @active.OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event('call',t,mem,list(r[:16]),s[0],p[0],f.contents,fpul.value))
        except Exception as exc:errors.append(repr(exc))
    @axes.Instruction
    def instruction(_ctx,word,f,value):
        nonlocal op_count
        try:
            op_count+=1;ae.append(event('instruction',word,memory,list(actual.r),sr.value,pr.value,f.contents,value))
            failing=c.get('calc_fail') and op_count==prefix+c['calc_n']
            if failing and c['calc_fail']=='missing':return -2
            primitive(word,f.contents,value)
            return -17 if failing else 0
        except Exception as exc:errors.append(repr(exc));return -99
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
        Call() if c.get('null_external') or c['entry']=='calc' else call,
        axes.Instruction() if c.get('null_instruction') else instruction,observe if c.get('observe',True) else active.OBSERVE())
    wanted=-1 if c.get('calc_fail') or guard else -2 if c.get('sound_fail')=='missing' else -17 if c.get('sound_fail') else 0
    assert result==wanted and not errors,(number,c,result,errors)
    assert memory==em,(number,c,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,c,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl'],(number,c,'FPU/FPUL/MACL differs')
    assert ac==ec,(number,c,'external calls differ')
    assert ae==(ee if c.get('observe',True) else [e for e in ee if e[0]=='instruction']),(number,c,'boundary differs')
    if not guard and not c.get('calc_fail') and not c.get('sound_fail'):
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
    return path,len(ae),op_count,steps,bool(guard),bool(c.get('calc_fail') or c.get('sound_fail')),ac


def main():
    for p,h in inventory['sha256'].items():assert digest(Path(p))==h,p
    allowed=set(ROWS)|set().union(*axes.ROWSETS.values())|set(range(chain.POP,0x8C639BD4,2))
    dependencies=['main_dispatch_shared_translation.json','main_dispatch_active_translation.json','main_dispatch_detach_translation.json']
    for name in dependencies:
        for r in json.loads((PROJECT/'analysis'/name).read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    for name in ('main_dispatch_transform_translation.json','main_dispatch_matrix_init_translation.json','main_dispatch_shared_check_translation.json'):
        allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis'/name).read_text())['reachable_addresses'])
    allowed.update(range(active.QUERY,0x8C06CEFE,2));allowed.update(range(base.RESET,0x8C05E7D4,2))
    seen=set();edges=set();sites=set();multiply=set();counts={k:0 for k in functions};linked={k:set() for k in functions if k!='calc'}
    records=[];boundaries=0;ops=0;guards=0;failures=0;unobserved=0;closed=0
    wanted_edges={tuple(int(a,16) for a in edge) for edge in inventory['conditional_edges']}
    implicit=set(range(START,0x8C638F38,2))|set(range(0x8C638F6E,END,2))
    for number,c in enumerate(cases()):
        path,b,n,steps,g,f,external=run_case(c,number,allowed)
        counts[c['entry']]+=1;seen.update(set(path)&ROWS);sites.update(set(path)&set(SITES))
        multiply.update(set(path)&{0x8C638F7E,0x8C638F80,0x8C638F82})
        boundaries+=b;ops+=n;guards+=g;failures+=f;unobserved+=not c.get('observe',True)
        closed+=c['entry']!='calc' and c.get('null_external',False) and not g and not f and not external
        for i,pc in enumerate(path):
            if pc in (0x8C638F30,0x8C638F76) and i+2<len(path):edges.add((pc,path[i+2]))
        if c['entry'] in linked:linked[c['entry']].update(set(path)&ROWS)
        records.append({'input':c,'steps':steps,'boundaries':b,'instruction_inputs':n,'partial_guard':g,'partial_failure':f,'full_state_match':True})
        if (number+1)%300==0:print(number+1,'vector-calculation/native-chain comparisons passed',flush=True)
    assert seen==ROWS and edges==wanted_edges and sites==set(SITES) and len(multiply)==3
    assert all(rows==implicit for rows in linked.values())
    files=[Path(__file__),MANIFEST,PROJECT/'build_main_dispatch_vector_calc.ps1',PROJECT/'verify_main_dispatch_axes.py',
           PROJECT/'verify_main_dispatch_matrix_init.py',PROJECT/'verify_main_dispatch_transform.py',PROJECT/'verify_main_dispatch_detach.py',
           PROJECT/'verify_main_dispatch_active_query.py',PROJECT/'verify_main_dispatch_shared_helpers.py',PROJECT/'verify_startup.py',
           PROJECT/'verify_fpu_cleanup_transfer.py',PROJECT/'verify_tikal_controller.py',PROJECT/'sh4_binary32.py',
           PROJECT/'analysis/main-dispatch-vector-calc/diagnostics.tsv',PROJECT/'analysis/main_dispatch_axes_inventory.json']
    for name in ('sa_main_dispatch_vector_calc_connected.c','sa_main_dispatch_matrix_init.c','sa_main_dispatch_transform.c',
                 'sa_main_dispatch_detach.c','sa_main_dispatch_shared_helpers.c','sa_main_dispatch_status_query.c',
                 'sa_main_dispatch_shared_check.c','sa_main_dispatch_active_query.c','sa_main_dispatch_reset.c',
                 'sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','model_matrix_stack_pop.c',
                 'sh4_float_i32.c','sh4_float_i32.h','sh4_compare_bits.h','fpu_cleanup_transfer.h','sh4_multiply.c','sh4_fmac.c','sh4_fmac.h'):
        files.append(PROJECT/'source'/name)
    files.extend(PROJECT/'analysis'/n for n in dependencies+['main_dispatch_transform_translation.json','main_dispatch_matrix_init_translation.json','main_dispatch_shared_check_translation.json'])
    report={k:inventory[k] for k in ('name','address','end_exclusive','instruction_bytes','source','source_sha256','retail_span_sha256')}
    report.update({'binary_sha256':digest(base.BINARY),'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
        'cases_passed':len(records),'counts':counts,'rows_covered':len(seen),'conditional_edges_covered':len(edges),
        'vector_instruction_sites_covered':len(sites),'native_multiply_sites_covered':len(multiply),
        'linked_implicit_rows_by_caller':{k:len(v) for k,v in linked.items()},'boundary_comparisons':boundaries,
        'instruction_inputs_compared':ops,'partial_guards_passed':guards,'partial_failures_passed':failures,
        'unobserved_cases_passed':unobserved,'completed_connected_cases_with_null_external':closed,
        'comparison_level':'Complete 16 MiB RAM, GPR/SR/PR, physical FPU banks/FPSCR, separate FPUL/MACL and ordered native/instruction/sound snapshots against original SH4 instructions; final FMUL uses independent native/Python scalar implementations.',
        'scope':'Existing vector-calculation source reused directly and through native transform/detach/STATUS/FLAG, implicit/explicit matrices, normalization, overlapping buffers, all six vector sites and three native FMUL sites; closed game-call paths with explicit numerical instruction fixtures.',
        'limitations':['FSCA/FTRV/FIPR/FSRRA outputs remain explicit input-sensitive fixtures; this is not proof of their numerical or hardware fidelity.',
                       'Sound is the only remaining external game service in the connected STATUS path. Complete game behavior and playable ports remain unfinished.',
                       'Calculation requires entry PR=0/SZ=0 and the original normalization-pointer literal at 8c638f8c. It collapses instruction/scalar errors to -1 while retaining partial state.',
                       'Normalization FMUL comparisons cover finite fixture inputs, DN=1 and RM0/1; reserved RM2/3 are checked as partial guards. Enabled FPU traps/nonfinite exceptional scalar inputs are outside this integration scope.',
                       'Instruction fixtures preserve FR/SZ/PR/RM mode bits. Mapped guest-pointer preconditions required; arbitrary invalid pointers, MMIO, hardware and all gameplay states remain unverified.'],
        'cases':records})
    (PROJECT/'analysis/main_dispatch_vector_calc_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    keys=('cases_passed','counts','rows_covered','conditional_edges_covered','vector_instruction_sites_covered','native_multiply_sites_covered',
          'linked_implicit_rows_by_caller','boundary_comparisons','instruction_inputs_compared','partial_guards_passed',
          'partial_failures_passed','unobserved_cases_passed','completed_connected_cases_with_null_external')
    print(json.dumps({k:report[k] for k in keys},indent=2))


if __name__=='__main__':main()
