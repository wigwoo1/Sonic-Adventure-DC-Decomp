"""Retail comparisons for transform sequencing, native pop and parent paths."""
import ctypes,hashlib,itertools,json
from pathlib import Path

import verify_main_dispatch_detach as previous
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
active,base=previous.active,previous.base
load,store=base.load,base.store
TRANSFORM,DETACH,STATUS,FLAG=previous.TRANSFORM,previous.DETACH,base.STATUS,base.FLAG
PUSH,RZ,RX,RY,CALC,POP=0x8C057B40,0x8C63A1CC,0x8C639EC8,0x8C639F5C,0x8C638F28,0x8C639B98
EXTERNALS={PUSH,RZ,RX,RY,CALC,base.SOUND}
VECTOR,ALT_VECTOR,MATRIX,ALT_MATRIX=0x8CE50000,0x8CE51000,0x8CE60000,0x8CE61000
DEPTH,STACK=0x8C88F69C,0x8C88F5F8
ANGLES=((0,0,0),(1,0,0),(0,1,0),(0,0,1),(1,2,3),
        (0xFFFFFFFF,0x80000000,0x7FFFFFFF),(0x80000000,0xFFFFFFFF,1),(0,0x80000000,0))
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_transform.dll'
LIB=ctypes.CDLL(str(DLL))
functions={'transform':LIB.sa_main_dispatch_transform_connected,
           'detach':LIB.sa_main_dispatch_detach_transform_connected,
           'status':LIB.sa_main_dispatch_status_transform_connected,
           'flag':LIB.sa_main_dispatch_flag_transform_connected}
for fn in functions.values():fn.argtypes=active.pure_args[:-1]+[Call,active.OBSERVE];fn.restype=ctypes.c_int
raw=LIB.sa_main_dispatch_transform_8c078eb0
raw.argtypes=active.pure_args[:-1]+[Call];raw.restype=ctypes.c_int
TRACKED=previous.TRACKED|{TRANSFORM,POP}


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def cases():
    for angles,mode,rm,depth in itertools.product(ANGLES,range(8),range(4),(0,1,2,8,0x80000000,0xFFFFFFFF)):
        yield {'entry':'transform','angles':angles,'mode':mode<<19,'rm':rm,'depth':depth}
    for entry,angles,mode,depth in itertools.product(('detach','status','flag'),ANGLES,
            (0,0x100000,0x200000,0x300000),(0,2)):
        yield {'entry':entry,'angles':angles,'mode':mode,'depth':depth}
    for entry,flags,mode in itertools.product(('detach','status','flag'),(0,0x800),(n<<19 for n in range(8))):
        yield {'entry':entry,'flags':flags,'mode':mode,'no_transform':True,'no_external':entry!='status'}
    for entry,variant,bank in itertools.product(('transform','detach','status','flag'),
            ('mutate','angles','redirect','relocate_matrix','pop_pr','pop_sz','pop_both','unobserved'),range(2)):
        c={'entry':entry,'angles':(1,2,3),'mode':bank<<21}
        if variant=='mutate':c['mutate']=True
        elif variant=='angles':c.update(mutate_angles=True,mutate=True)
        elif variant=='redirect':c['redirect']=True
        elif variant=='relocate_matrix':c['relocate_matrix']=True
        elif variant=='unobserved':c['observe']=False
        else:c['pop_mode']={'pop_pr':0x80000,'pop_sz':0x100000,'pop_both':0x180000}[variant]
        yield c
    for entry,target,failure,mode in itertools.product(('transform','detach','status','flag'),
            (PUSH,RZ,RX,RY,CALC),('missing','service_error'),(0,0x100000,0x200000,0x300000)):
        yield {'entry':entry,'angles':(1,2,3),'mode':mode,'fail_target':target,'failure':failure,'mutate':True}
    for failure,mode in itertools.product(('missing','service_error'),(0,0x200000)):
        yield {'entry':'status','angles':(1,2,3),'mode':mode,'fail_target':base.SOUND,'failure':failure}
    for entry,mode in itertools.product(('transform','detach','status','flag'),(0,0x100000,0x200000,0x300000)):
        yield {'entry':entry,'angles':(1,2,3),'mode':mode,'fail_target':PUSH,'failure':'missing','null_external':True}
    for mode in range(8):
        yield {'entry':'transform','angles':(1,2,3),'mode':mode<<19,'fail_target':PUSH,'failure':'missing','raw':True,'null_external':True}
    for bank in range(2):
        yield {'entry':'transform','mode':bank<<21,'alias_vector':True}


def guard_target(c):
    if c.get('failure'):return None
    if c.get('no_transform'):return None
    if c['entry']!='transform' and c['mode']&0x80000:return 0x8C0B1DA8
    if c['mode']&0x180000 or c.get('pop_mode'):return POP
    return None


def prepare(c,number):
    fixture=dict(c,entry='detach',flags=c.get('flags',0x800),index=1,type=8 if c.get('no_transform') else 4,
                 class_mode=0,class_type=6,pointer=2)
    memory,state,fpu=previous.prepare(fixture,number)
    fpu.fpscr|=c.get('rm',0)
    state.pc={'transform':TRANSFORM,'detach':DETACH,'status':STATUS,'flag':FLAG}[c['entry']]
    vector=base.OBJECT+0x14 if c.get('alias_vector') else VECTOR
    for i,bits in enumerate((0x3F800000,0x80000000,0x7FC01234,0xFF800000,1,0x89ABCDEF)):
        store(memory,vector+4*i,bits);store(memory,ALT_VECTOR+4*i,bits^0x13579BDF)
    if not c.get('alias_vector'):
        for offset,value in zip((0x14,0x18,0x1C),c.get('angles',(1,2,3))):store(memory,base.OBJECT+offset,value)
    if c['entry']=='transform':state.r[5]=vector
    store(memory,DEPTH,c.get('depth',2));store(memory,STACK,MATRIX+0x100)
    for at in (MATRIX,ALT_MATRIX):
        for i in range(128):store(memory,at+4*i,(0x7FC01234+i*0x12345+number*0x10203)&0xFFFFFFFF)
    return memory,state,fpu


def run_case(c,number,allowed):
    memory,initial,fpu=prepare(c,number)
    em=bytearray(memory);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    path=[];ee=[];ec={};special={'macl':0xFEED1234}
    def event(target,mem,r,sr,pr,f,fpul=None):
        shadow=Fpu.from_buffer_copy(f)
        if fpul is not None:shadow.fpul=fpul
        stack_words=tuple(load(mem,(r[15]+i*4)&0xFFFFFFFF) for i in range(7))
        return (target,tuple(r),sr,pr,bytes(shadow),load(mem,DEPTH),load(mem,STACK),stack_words)
    def trace(pc,word,r,sr,pr,_f):
        assert pc in allowed,hex(pc)
        path.append(pc)
        if pc in TRACKED and pc!=initial.pc:ee.append(event(pc,em,r,sr,pr,ef))
    def service(target,mem,r,sr,pr,f,counters):
        assert target in EXTERNALS,hex(target)
        counters[hex(target)]=counters.get(hex(target),0)+1
        store(mem,0x8CEF0000+(target&0xFFF),0xBA000000|(target&0xFFFF))
        if target==PUSH:
            if c.get('redirect'):store(mem,r[15]+4,ALT_VECTOR)
            if c.get('mutate_angles'):
                for offset,value in ((0x14,0),(0x18,0x80000000),(0x1C,0xFFFFFFFF)):store(mem,base.OBJECT+offset,value)
        elif target in (RZ,RX,RY):
            angle=load(mem,base.OBJECT+{RZ:0x1C,RX:0x14,RY:0x18}[target])
            if target==RY:angle=(-angle)&0xFFFFFFFF
            assert r[4]==0 and r[5]==angle,(hex(target),hex(r[5]),hex(angle))
            if target==RZ and c.get('mutate_angles'):
                store(mem,base.OBJECT+0x14,0x7FFFFFFF);store(mem,base.OBJECT+0x18,0)
        elif target==CALC:
            assert r[4]==0 and r[5]==r[15]+8
            # Deterministic fixture output: the original matrix calculation
            # is intentionally not reimplemented or claimed by this test.
            values=[load(mem,r[5]+4*i)^((0x10203040+i*0x11111111)&0xFFFFFFFF) for i in range(3)]
            for i,value in enumerate(values):store(mem,r[6]+4*i,value)
            if c.get('relocate_matrix'):store(mem,STACK,ALT_MATRIX+0x100);store(mem,DEPTH,8)
            if c.get('pop_mode'):f.fpscr|=c['pop_mode']
        else:
            base.apply_service(dict(c,character=load(mem,base.OBJECT+8,1)),target,mem,r,sr,pr,f,{})
        if c.get('mutate'):
            for i in range(8):r[i]=(0xDA000000+i*0x10101+target)&0xFFFFFFFF
            sr[0]^=1;f.fpscr^=0x200000
            for bank in range(2):
                for i in range(8):f.banks[bank][i]=(0xBC000000+bank*0x10000+i+target)&0xFFFFFFFF
        r[0]=(0xEA000000|target&0xFFFF)&0xFFFFFFFF
    def hook_for(target):
        def hook(mem,r,sr):
            ee.append(event(target,mem,r,sr,expected.pr,ef));s=[sr];p=[expected.pr]
            service(target,mem,r,s,p,ef,ec)
            return s[0]
        return hook
    guard=guard_target(c);stop=c.get('fail_target',guard)
    steps=reference(em,expected,fpu=ef,special=special,trace=trace,stop_pc=stop,
        hooks={t:hook_for(t) for t in EXTERNALS},instruction_budget=4000)
    if c.get('failure') or guard==POP:
        ee.append(event(stop,em,list(expected.r),expected.sr,expected.pr,ef))
    if c.get('failure')=='service_error':
        r=list(expected.r);s=[expected.sr];p=[expected.pr]
        service(stop,em,r,s,p,ef,ec);expected.r[:]=r;expected.sr=s[0]
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    ae=[];ac={};errors=[]
    @active.OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event(t,mem,list(r[:16]),s[0],p[0],f.contents,fpul.value))
        except Exception as exc:errors.append(repr(exc))
    @Call
    def call(_ctx,t,mem,f,s,r,p):
        try:
            if c.get('failure')=='missing' and t==c['fail_target']:return -2
            service(t,mem,r,s,p,f.contents,ac)
            return -17 if c.get('failure')=='service_error' and t==c['fail_target'] else 0
        except Exception as exc:errors.append(repr(exc));return -99
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    args=(view,ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,
          Call() if c.get('null_external') or c.get('no_external') else call)
    result=raw(*args) if c.get('raw') else functions[c['entry']](*args,observe if c.get('observe',True) else active.OBSERVE())
    wanted=-2 if c.get('failure')=='missing' else -17 if c.get('failure') else -1 if guard else 0
    assert result==wanted and not errors,(number,c,result,errors)
    assert memory==em,(number,c,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,c,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132],(number,c,'FPU differs')
    assert fpul.value==ef.fpul and macl.value==special['macl']
    assert ac==ec,(number,c,'external calls differ')
    if c.get('observe',True) and not c.get('raw'):assert ae==ee,(number,c,'boundary differs')
    else:assert not ae
    if not guard and not c.get('failure'):
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
    return path,len(ae),steps,guard,ac


def main():
    mp=PROJECT/'analysis/main_dispatch_transform_translation.json'
    manifest=json.loads(mp.read_text())
    for p,h in manifest['sha256'].items():assert digest(Path(p))==h,p
    rows={int(a,16) for a in manifest['reachable_addresses']}
    wanted_edges={tuple(int(a,16) for a in edge) for edge in manifest['conditional_edges']}
    wanted_calls={int(a,16) for a in manifest['external_call_sites']}
    allowed=set(rows)|set(range(POP,0x8C639BD4,2))
    dep_manifests=['main_dispatch_shared_translation.json','main_dispatch_active_translation.json','main_dispatch_detach_translation.json']
    for filename in dep_manifests:
        for item in json.loads((PROJECT/'analysis'/filename).read_text())['routines']:
            allowed.update(int(a,16) for a in item['reachable_addresses'])
    allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis/main_dispatch_shared_check_translation.json').read_text())['reachable_addresses'])
    allowed.update(range(active.QUERY,0x8C06CEFE,2));allowed.update(range(base.RESET,0x8C05E7D4,2))
    seen=set();edges=set();calls=set();pop_rows=set();counts={k:0 for k in functions};records=[]
    linked={k:set() for k in functions};boundaries=0;guards=0;errors=0;unobserved=0;null_cases=0
    for number,c in enumerate(cases()):
        path,b,steps,guard,external=run_case(c,number,allowed)
        counts[c['entry']]+=1;seen.update(set(path)&rows);linked[c['entry']].update(set(path)&rows)
        pop_rows.update(pc for pc in path if POP<=pc<0x8C639BD4)
        boundaries+=b;guards+=bool(guard);errors+=bool(c.get('failure'));unobserved+=not c.get('observe',True)
        null_cases+=bool(c.get('no_external') or c.get('null_external'))
        calls.update(set(path)&wanted_calls)
        for i,pc in enumerate(path):
            if pc not in rows:continue
            w=int.from_bytes(base.binary[pc-0x8C010000:pc-0x8C010000+2],'little')
            if w>>8 in (0x89,0x8B,0x8D,0x8F):
                j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
                if j<len(path):edges.add((pc,path[j]))
        records.append({'input':c,'steps':steps,'boundaries':b,'external_calls':external,
                        'partial_guard_target':hex(guard) if guard else None,'full_state_match':True})
        if (number+1)%400==0:print(number+1,'transform/native-pop/caller comparisons passed',flush=True)
    assert seen==rows and edges==wanted_edges and calls==wanted_calls
    assert all(v==rows for v in linked.values())
    files=[Path(__file__),mp,PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',
           PROJECT/'verify_tikal_controller.py',PROJECT/'verify_main_dispatch_detach.py',
           PROJECT/'verify_main_dispatch_active_query.py',PROJECT/'verify_main_dispatch_shared_helpers.py',
           PROJECT/'build_main_dispatch_transform.ps1',PROJECT/'analysis/main-dispatch-transform/diagnostics.tsv',
           PROJECT/'analysis/model_matrix_stack_pop_verification.json']
    names=('sa_main_dispatch_transform_connected.c','sa_main_dispatch_detach.c','sa_main_dispatch_shared_helpers.c',
           'sa_main_dispatch_status_query.c','sa_main_dispatch_shared_check.c','sa_main_dispatch_active_query.c',
           'sa_main_dispatch_reset.c','model_matrix_stack_pop.c','sh4_float_i32.c','sh4_float_i32.h',
           'sh4_compare_bits.h','fpu_cleanup_transfer.h')
    files.extend(PROJECT/'source'/n for n in names)
    files.extend(PROJECT/'analysis'/n for n in dep_manifests+['main_dispatch_shared_check_translation.json'])
    report={k:manifest[k] for k in ('name','address','end_exclusive','instruction_bytes','retail_span_sha256','source')}
    report.update({'source_sha256':digest(PROJECT/manifest['source']),'binary_sha256':digest(base.BINARY),
        'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},'cases_passed':len(records),'counts':counts,
        'boundary_comparisons':boundaries,'transform_rows_covered':len(seen),'transform_conditional_edges_covered':len(edges),
        'transform_call_sites_covered':len(calls),'transform_rows_by_entry':{k:len(v) for k,v in linked.items()},
        'native_pop_rows_covered':len(pop_rows),'partial_precision_guards_passed':guards,
        'partial_service_error_cases_passed':errors,'unobserved_cases_passed':unobserved,'null_external_cases_passed':null_cases,
        'comparison_level':'Complete 16 MiB RAM, GPR/SR/PR, physical FPU banks/FPSCR, separate FPUL/MACL and ordered native/external boundary snapshots against original SH4 instructions.',
        'scope':'55-row transform sequencing directly and through detach/STATUS/FLAG, native matrix pop, all rotation branch combinations, 32-bit angle negation, callback mutations/angle reloads, input alias/output redirection, matrix-global reloads, raw FR/SZ transfers and partial failures/guards.',
        'limitations':['Matrix push 8c057b40, three rotations 8c63a1cc/8c639ec8/8c639f5c, vector calculation 8c638f28 and sound remain external fixtures; their algorithms are not recovered here.',
                       'Existing native matrix pop accepts PR=0/SZ=0 only. Other modes stop with host -1 at its call boundary; no hardware exception delivery is emulated.',
                       'Raw SZ=1 transform stack stores overlap saved PR; tests compare exact partial state before the unsupported native-pop call rather than claim arbitrary SZ=1 returns.',
                       'Detach still guards reached FLDI0/FLDI1 with PR=1 before execution.',
                       'Mapped/nonnull retail pointer preconditions required; arbitrary invalid pointers, MMIO, hardware and all actual game states are unverified. Full game/ports remain unfinished.'],
        'cases':records})
    (PROJECT/'analysis/main_dispatch_transform_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    keys=('cases_passed','counts','boundary_comparisons','transform_rows_covered','transform_conditional_edges_covered',
          'transform_call_sites_covered','transform_rows_by_entry','native_pop_rows_covered',
          'partial_precision_guards_passed','partial_service_error_cases_passed','unobserved_cases_passed','null_external_cases_passed')
    print(json.dumps({k:report[k] for k in keys},indent=2))


if __name__=='__main__':main()
