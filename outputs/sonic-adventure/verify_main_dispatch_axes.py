"""Verify reused axis bodies and native integration with instruction fixtures."""
import ctypes,hashlib,itertools,json
from pathlib import Path

import verify_main_dispatch_matrix_init as prior
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
chain,base,active=prior.chain,prior.base,prior.active
load,store=base.load,base.store
MANIFEST=PROJECT/'analysis/main_dispatch_axes_inventory.json'
inventory=json.loads(MANIFEST.read_text())
AXES={r['axis']:r for r in inventory['routines']}
ADDRESS={k:int(r['address'],16) for k,r in AXES.items()}
ROWSETS={k:{int(a,16) for a in r['reachable_addresses']} for k,r in AXES.items()}
IMPLICIT={k:set(range(int(r['address'],16),int(r['explicit_vector_sites'][0],16)-4,2)) for k,r in AXES.items()}
# The first explicit vector instruction follows its leading FLDI0. The raw
# branch target, rather than that instruction, is the path boundary.
for k,r in AXES.items():IMPLICIT[k]=set(range(ADDRESS[k],max(int(b,16) for a,b in r['conditional_edges']),2))
VECTOR_SITES={int(a,16):int(w,16) for r in AXES.values() for a,w in r['vector_instruction_sites'].items()}
EXPLICIT_MATRIX=0x8CE70000
Instruction=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32)
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_axes.dll'
LIB=ctypes.CDLL(str(DLL))
functions={k:getattr(LIB,'sa_main_dispatch_axis_'+k+'_connected') for k in AXES}
functions.update({k:getattr(LIB,'sa_main_dispatch_axes_'+k+'_connected') for k in ('transform','detach','status','flag')})
for fn in functions.values():fn.argtypes=active.pure_args[:-1]+[Call,Instruction,active.OBSERVE];fn.restype=ctypes.c_int
TRACKED=chain.TRACKED|{chain.PUSH,*ADDRESS.values()}
EXTERNALS={chain.CALC,base.SOUND}
ANGLES=(0,1,0x7FFF,0x8000,0xFFFF,0x80000000,0xFFFFFFFF)


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def cases():
    for axis,explicit,angle,bank,rm,sr in itertools.product(AXES,(False,True),ANGLES,range(2),range(4),(0,0x700083F3)):
        yield {'entry':axis,'explicit':explicit,'angle':angle,'mode':bank<<21,'rm':rm,'sr':sr}
    for axis,explicit,mode in itertools.product(AXES,(False,True),(0x80000,0x100000,0x180000,0x280000,0x300000,0x380000)):
        yield {'entry':axis,'explicit':explicit,'angle':1,'mode':mode}
    for axis,explicit,n,kind,bank in itertools.product(AXES,(False,True),(1,2,3),('missing','service_error','null'),range(2)):
        if kind=='null' and n!=1:continue
        yield {'entry':axis,'explicit':explicit,'angle':0x80000000,'mode':bank<<21,'vector_fail':kind,'vector_n':n,'null_instruction':kind=='null'}
    for axis,explicit,bank in itertools.product(AXES,(False,True),range(2)):
        yield {'entry':axis,'explicit':explicit,'angle':0xFFFF,'mode':bank<<21,'observe':False}
    for entry,angles,mode,pair in itertools.product(('transform','detach','status','flag'),chain.ANGLES,range(8),((0,8),(8,8))):
        yield {'entry':entry,'angles':angles,'mode':mode<<19,'depth':pair[0],'limit':pair[1]}
    for entry,n,kind,bank in itertools.product(('transform','detach','status','flag'),(1,2,3),('missing','service_error','null'),range(2)):
        if kind=='null' and n!=1:continue
        yield {'entry':entry,'angles':(1,2,3),'mode':bank<<21,'vector_fail':kind,'vector_n':n,'null_instruction':kind=='null'}
    for entry,target,kind,bank in itertools.product(('transform','detach','status','flag'),(chain.CALC,),('missing','service_error'),range(2)):
        yield {'entry':entry,'angles':(1,2,3),'mode':bank<<21,'game_fail':kind,'game_target':target,'mutate_external':True}
    for kind,bank in itertools.product(('missing','service_error'),range(2)):
        yield {'entry':'status','angles':(1,2,3),'mode':bank<<21,'game_fail':kind,'game_target':base.SOUND}
    for entry,bank in itertools.product(('transform','detach','status','flag'),range(2)):
        yield {'entry':entry,'angles':(1,2,3),'mode':bank<<21,'game_fail':'missing','game_target':chain.CALC,'null_external':True}
    for entry,flags,mode in itertools.product(('detach','status','flag'),(0,0x800),range(8)):
        yield {'entry':entry,'flags':flags,'mode':mode<<19,'no_transform':True,'null_instruction':True,'null_external':entry!='status'}
    for entry,variant,bank in itertools.product(('transform','detach','status','flag'),('unobserved','mutate_external','pop_pr','pop_sz','pop_both'),range(2)):
        c={'entry':entry,'angles':(1,2,3),'mode':bank<<21}
        if variant=='unobserved':c['observe']=False
        elif variant=='mutate_external':c['mutate_external']=True
        else:c['pop_mode']={'pop_pr':0x80000,'pop_sz':0x100000,'pop_both':0x180000}[variant]
        yield c


def guard_target(c):
    if c.get('vector_fail') or c.get('game_fail'):return None
    if c['entry'] in AXES:return ADDRESS[c['entry']] if c['mode']&0x180000 else None
    if c.get('no_transform'):return None
    if c['entry']!='transform' and c['mode']&0x80000:return 0x8C0B1DA8
    if c['mode']&0x80000 and prior.signed(c.get('depth',2))<prior.signed(c.get('limit',8)):return prior.FLDI
    if c['mode']&0x180000:
        x,y,z=c['angles']
        return ADDRESS['z'] if z else ADDRESS['x'] if x else ADDRESS['y'] if y else chain.POP
    return chain.POP if c.get('pop_mode') else None


def prepare(c,number):
    direct=c['entry'] in AXES
    fixture=dict(c,entry='transform' if direct else c['entry'])
    memory,state,fpu=prior.connected_prepare(fixture,number)
    if direct:
        state.pc=ADDRESS[c['entry']];state.r[4]=EXPLICIT_MATRIX if c.get('explicit') else 0
        state.r[5]=c.get('angle',1);state.sr=c.get('sr',state.sr)
        for i,value in enumerate(prior.PATTERNS):store(memory,EXPLICIT_MATRIX+4*i,value)
    fpu.fpscr|=c.get('rm',0)
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
    def vector_apply(word,f,fpul):
        assert word in (0xF0FD,0xF1FD,0xF5FD),hex(word)
        bank=(f.fpscr>>21)&1
        if word==0xF0FD:
            # Input-sensitive bit fixtures, not sine/cosine approximations.
            f.banks[bank][0]=0x3F000001^(fpul&0xFFFF)
            f.banks[bank][1]=0x3F800002^((fpul&0xFFFF)<<3)
        else:
            start=(word>>8)&12
            values=list(f.banks[bank][start:start+4]);matrix=list(f.banks[bank^1])
            f.banks[bank][start:start+4]=[(v^matrix[(i*5)%16]^0x10203040^(i*0x10101))&0xFFFFFFFF for i,v in enumerate(values)]
        f.fpscr^=0x10040 # Fixture flag effects preserve FR/SZ/PR/RM.
    def reference_vector(word,f):
        pc,r,sr,pr=latest
        ee.append(event('instruction',word,em,r,sr,pr,f,f.fpul))
        vector_apply(word,f,f.fpul)
    def service(target,mem,r,sr,pr,f,counters):
        assert target in EXTERNALS,hex(target)
        counters[hex(target)]=counters.get(hex(target),0)+1
        if target==chain.CALC:
            assert r[4]==0 and r[5]==r[15]+8
            values=[load(mem,r[5]+4*i)^((0x10203040+i*0x11111111)&0xFFFFFFFF) for i in range(3)]
            for i,v in enumerate(values):store(mem,r[6]+4*i,v)
            if c.get('pop_mode'):f.fpscr|=c['pop_mode']
            if c.get('mutate_external'):
                for i in range(8):r[i]=(0xAB000000+i*0x10101)&0xFFFFFFFF
                sr[0]^=1;f.fpscr^=0x200000
        else:base.apply_service(dict(c,character=load(mem,base.OBJECT+8,1)),target,mem,r,sr,pr,f,{})
        r[0]=0xEA000000|(target&0xFFFF)
    def hook_for(target):
        def hook(mem,r,sr):
            ee.append(event('call',target,mem,r,sr,expected.pr,ef,ef.fpul));s=[sr];p=[expected.pr]
            service(target,mem,r,s,p,ef,ec)
            return s[0]
        return hook
    guard=guard_target(c);stop=guard
    if c.get('vector_fail'):
        axis=c['entry'] if c['entry'] in AXES else 'z'
        key='explicit_vector_sites' if c.get('explicit') else 'implicit_vector_sites'
        stop=int(AXES[axis][key][c['vector_n']-1],16)
    if c.get('game_fail'):stop=c['game_target']
    steps=reference(em,expected,fpu=ef,special=special,trace=trace,stop_pc=stop,
        hooks={t:hook_for(t) for t in EXTERNALS},vector_instruction=reference_vector,instruction_budget=4000)
    if c.get('vector_fail'):
        word=VECTOR_SITES[stop]
        if not c.get('null_instruction'):
            ee.append(event('instruction',word,em,list(expected.r),expected.sr,expected.pr,ef,ef.fpul))
        if c['vector_fail']=='service_error':vector_apply(word,ef,ef.fpul)
    elif c.get('game_fail') or guard in {*ADDRESS.values(),chain.POP} and initial.pc!=guard:
        ee.append(event('call',stop,em,list(expected.r),expected.sr,expected.pr,ef,ef.fpul))
    if c.get('game_fail')=='service_error':
        r=list(expected.r);s=[expected.sr];p=[expected.pr]
        service(stop,em,r,s,p,ef,ec);expected.r[:]=r;expected.sr=s[0]
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    ae=[];ac={};errors=[];op_count=0
    @active.OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event('call',t,mem,list(r[:16]),s[0],p[0],f.contents,fpul.value))
        except Exception as exc:errors.append(repr(exc))
    @Instruction
    def instruction(_ctx,word,f,value):
        nonlocal op_count
        try:
            op_count+=1;ae.append(event('instruction',word,memory,list(actual.r),sr.value,pr.value,f.contents,value))
            failing=c.get('vector_fail') and op_count==c['vector_n']
            if failing and c['vector_fail']=='missing':return -2
            vector_apply(word,f.contents,value)
            return -17 if failing else 0
        except Exception as exc:errors.append(repr(exc));return -99
    @Call
    def call(_ctx,t,mem,f,s,r,p):
        try:
            failing=c.get('game_fail') and t==c['game_target']
            if failing and c['game_fail']=='missing':return -2
            service(t,mem,r,s,p,f.contents,ac)
            return -17 if failing else 0
        except Exception as exc:errors.append(repr(exc));return -99
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    result=functions[c['entry']](view,ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,
        Call() if c.get('null_external') else call,Instruction() if c.get('null_instruction') else instruction,
        observe if c.get('observe',True) else active.OBSERVE())
    failure=c.get('vector_fail',c.get('game_fail'))
    wanted=-2 if failure in ('missing','null') else -17 if failure else -1 if guard else 0
    assert result==wanted and not errors,(number,c,result,errors)
    assert memory==em,(number,c,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,c,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl'],(number,c,'FPU/FPUL/MACL differs')
    assert ac==ec,(number,c,'external calls differ')
    visible=ee if c.get('observe',True) else [e for e in ee if e[0]=='instruction']
    assert ae==visible,(number,c,'ordered input boundary differs')
    if not guard and not failure:assert actual.r[15]==initial.r[15] and pr.value==initial.pr
    return path,len(ae),op_count,steps,bool(guard),bool(failure)


def main():
    for p,h in inventory['sha256'].items():assert digest(Path(p))==h,p
    allowed=set().union(*ROWSETS.values())|set(range(chain.POP,0x8C639BD4,2))
    dependencies=['main_dispatch_shared_translation.json','main_dispatch_active_translation.json','main_dispatch_detach_translation.json']
    for name in dependencies:
        for r in json.loads((PROJECT/'analysis'/name).read_text())['routines']:allowed.update(int(a,16) for a in r['reachable_addresses'])
    for name in ('main_dispatch_transform_translation.json','main_dispatch_matrix_init_translation.json','main_dispatch_shared_check_translation.json'):
        allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis'/name).read_text())['reachable_addresses'])
    allowed.update(range(active.QUERY,0x8C06CEFE,2));allowed.update(range(base.RESET,0x8C05E7D4,2))
    seen={k:set() for k in AXES};edges={k:set() for k in AXES};sites=set();linked={k:set() for k in ('transform','detach','status','flag')}
    counts={k:0 for k in functions};records=[];boundaries=0;ops=0;guards=0;failures=0;unobserved=0;null_ops=0
    for number,c in enumerate(cases()):
        path,b,n,steps,g,f=run_case(c,number,allowed)
        counts[c['entry']]+=1;boundaries+=b;ops+=n;guards+=g;failures+=f
        unobserved+=not c.get('observe',True);null_ops+=bool(c.get('null_instruction'))
        sites.update(set(path)&set(VECTOR_SITES))
        for axis,rows in ROWSETS.items():
            seen[axis].update(set(path)&rows)
            for i,pc in enumerate(path):
                if pc==ADDRESS[axis]+2 and i+2<len(path):edges[axis].add((pc,path[i+2]))
        if c['entry'] in linked:linked[c['entry']].update(set(path)&set().union(*IMPLICIT.values()))
        records.append({'input':c,'steps':steps,'boundaries':b,'instruction_inputs':n,'partial_guard':g,'partial_failure':f,'full_state_match':True})
        if (number+1)%300==0:print(number+1,'axis/native-chain comparisons passed',flush=True)
    for axis,r in AXES.items():
        assert seen[axis]==ROWSETS[axis]
        assert edges[axis]=={tuple(int(a,16) for a in edge) for edge in r['conditional_edges']}
    assert sites==set(VECTOR_SITES) and all(v==set().union(*IMPLICIT.values()) for v in linked.values())
    files=[Path(__file__),MANIFEST,PROJECT/'build_main_dispatch_axes.ps1',PROJECT/'verify_main_dispatch_matrix_init.py',
           PROJECT/'verify_main_dispatch_transform.py',PROJECT/'verify_main_dispatch_detach.py',PROJECT/'verify_main_dispatch_active_query.py',
           PROJECT/'verify_main_dispatch_shared_helpers.py',PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',
           PROJECT/'verify_tikal_controller.py',PROJECT/'analysis/main-dispatch-axes/diagnostics.tsv']
    for name in ('sa_main_dispatch_axes_connected.c','sa_main_dispatch_matrix_init.c','sa_main_dispatch_transform.c',
                 'sa_main_dispatch_detach.c','sa_main_dispatch_shared_helpers.c','sa_main_dispatch_status_query.c',
                 'sa_main_dispatch_shared_check.c','sa_main_dispatch_active_query.c','sa_main_dispatch_reset.c',
                 'model_matrix_stack_pop.c','sh4_float_i32.c','sh4_float_i32.h','sh4_compare_bits.h','fpu_cleanup_transfer.h'):
        files.append(PROJECT/'source'/name)
    files.extend(PROJECT/'analysis'/n for n in dependencies+['main_dispatch_transform_translation.json','main_dispatch_matrix_init_translation.json','main_dispatch_shared_check_translation.json'])
    report={'routines':inventory['routines'],'binary_sha256':digest(base.BINARY),'dll_sha256':digest(DLL),
        'sha256':{str(p):digest(p) for p in files},'cases_passed':len(records),'counts':counts,
        'axis_rows_covered':{k:len(v) for k,v in seen.items()},'conditional_edges_covered':sum(map(len,edges.values())),
        'vector_instruction_sites_covered':len(sites),'linked_implicit_rows_by_caller':{k:len(v) for k,v in linked.items()},
        'boundary_comparisons':boundaries,'vector_instruction_inputs_compared':ops,'partial_precision_guards_passed':guards,
        'partial_failure_cases_passed':failures,'unobserved_cases_passed':unobserved,'null_instruction_cases_passed':null_ops,
        'comparison_level':'Complete 16 MiB RAM, GPR/SR/PR, physical FPU banks/FPSCR, separate FPUL/MACL and ordered game/native/vector input snapshots against original SH4 instructions.',
        'scope':'Existing X/Y/Z C reused and freshly verified across both matrix paths; native rotations connected to transform/detach/STATUS/FLAG; FSCA/FTRV instruction fixtures remain explicit, with partial instruction/game-service failures and mode guards.',
        'limitations':['FSCA/FTRV outputs are input-sensitive bit fixtures, not recovered numerical implementations or proof of hardware fidelity.',
                       'Vector calculation 8c638f28 and sound remain external game services. Complete numerical transforms/full game/ports are unfinished.',
                       'Legacy rotations and pop require entry PR=0/SZ=0 and return -1 before mutating guest state when their mode guard fails. Native init/detach keep their reached-instruction guards.',
                       'Instruction fixtures preserve FR/SZ/PR/RM mode bits; mode-changing instruction callbacks are outside this contract.',
                       'Mapped/nonnull pointer preconditions required; arbitrary invalid pointers, MMIO, hardware and all gameplay states are unverified.'],
        'cases':records}
    (PROJECT/'analysis/main_dispatch_axes_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    keys=('cases_passed','counts','axis_rows_covered','conditional_edges_covered','vector_instruction_sites_covered',
          'linked_implicit_rows_by_caller','boundary_comparisons','vector_instruction_inputs_compared',
          'partial_precision_guards_passed','partial_failure_cases_passed','unobserved_cases_passed','null_instruction_cases_passed')
    print(json.dumps({k:report[k] for k in keys},indent=2))


if __name__=='__main__':main()
