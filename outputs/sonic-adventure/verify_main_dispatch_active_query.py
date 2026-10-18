"""Verify native active-query group, closed QUERY chain and STATUS connection."""
import ctypes,hashlib,itertools,json,struct
from pathlib import Path

import verify_main_dispatch_shared_helpers as base
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
CLEAR,PREDICATE,ACTIVE,QUERY,CHECK,STATUS=0x8C05E69E,0x8C05E6C4,0x8C05E6E8,base.QUERY,base.CHECK,base.STATUS
FLOAT,COMPARE=0x8C05E734,0x8C05E736
CONTACT,CONTEXT,TASK,INFO=0x8CB00000,0x8CB10000,0x8CB20000,0x8CB30000
GLOBAL_LINK,CLASS_INFO=0x8CB40000,0x8CB50000
POOL_NODES,POOL_FLAGS=0x8CC00000,0x8CD00000
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_active_query.dll'
LIB=ctypes.CDLL(str(DLL))
OBSERVE=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P)
pure_args=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,OBSERVE]
functions={'clear':LIB.sa_main_dispatch_clear_table_8c05e69e,
           'predicate':LIB.sa_main_dispatch_active_predicate_8c05e6c4,
           'active':LIB.sa_main_dispatch_active_query_8c05e6e8,
           'query':LIB.sa_main_dispatch_active_query_connected,
           'status':LIB.sa_main_dispatch_status_active_connected}
for name,fn in functions.items():
    fn.argtypes=pure_args if name in ('clear','predicate','active') else pure_args[:-1]+[Call,OBSERVE]
    fn.restype=ctypes.c_int
load,store=base.load,base.store
new_entries={CLEAR,PREDICATE,ACTIVE}
TRACKED=new_entries|{QUERY,CHECK}


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def cases():
    for mask,mode in itertools.product((0,1,0x155,0x2AA,0x3FF),range(8)):
        yield {'entry':'clear','pool_mask':mask,'mode':mode<<19}
    for index,kind,nonnull in itertools.product(range(256),(7,8,9,0xFF),(False,True)):
        yield {'entry':'predicate','index':index,'type':kind,'nonnull':nonnull}
    for entry,variant,mode in itertools.product(('active','query'),
            ('null_zero','null_nonzero','other_type','other_index','zero_count','other_method'),range(8)):
        config={'entry':entry,'mode':mode<<19,'index':0,'type':8,'nonnull':True,'count':1,'method':1,'value':29}
        if variant=='null_zero':config['nonnull']=False
        if variant=='null_nonzero':config.update(nonnull=False,index=0x80)
        if variant=='other_type':config['type']=7
        if variant=='other_index':config['index']=1
        if variant=='zero_count':config['count']=0
        if variant=='other_method':config['method']=0xFFFF
        yield config
    for entry,value,count,mode,rm in itertools.product(('active','query'),
            (0xFFFFFFFF,0,1,29,30,31,0x7FFFFFFF,0x80000000,0x1000001,0xFEFFFFFF),
            (1,0xFFFF),(0,0x100000,0x200000,0x300000),(0,1)):
        yield {'entry':entry,'index':0,'type':8,'nonnull':True,'count':count,
               'method':1,'value':value,'mode':mode,'rm':rm}
    for index in range(256):
        yield {'entry':'query','index':index,'type':8,'value':29,'mode':0 if index==0 else (index%8)<<19}
    for entry,raw in itertools.product(('active','query'),(0,0x80000000,0x7F800000,0xFF800000,0x7F800001,0x7FC00001)):
        yield {'entry':entry,'index':0,'type':8,'value':1,'threshold':raw,'mode':0x300000}
    for value,flags,mode,kind,pointer in itertools.product((29,30,0xFFFFFFFF,0x1000001),
            (0,0x800,0x802),(0,7),(4,5,6,7),range(3)):
        yield {'entry':'status','index':0,'type':8,'value':value,'flags':flags,
               'class_mode':mode,'class_type':kind,'pointer':pointer,'mode':0x200000 if pointer&1 else 0}
    for entry,mode in itertools.product(('active','query'),(0,0x100000,0x200000,0x300000)):
        yield {'entry':entry,'index':0,'type':8,'value':29,'mode':mode,'observe':False}
    for mode in (0,0x100000,0x200000,0x300000):
        yield {'entry':'clear','pool_mask':3,'pool_alias':True,'mode':mode}
    for entry,guard,mode in itertools.product(('active','query'),
            ('precision','reserved_rm','inexact_enable_exact','inexact_enable_inexact','invalid_enable'),
            (0,0x100000,0x200000,0x300000)):
        config={'entry':entry,'index':0,'type':8,'value':1,'mode':mode,'guard':guard}
        if guard=='precision':config['mode']|=0x80000
        if guard=='reserved_rm':config['rm']=2
        if guard.startswith('inexact_enable'):config['enabled']=0x80
        if guard=='inexact_enable_inexact':config['value']=0x1000001
        if guard=='invalid_enable':config.update(enabled=0x800,threshold=0x7FC00001)
        yield config


def prepare(config,number):
    memory=bytearray(base.baseline)
    store(memory,base.OBJECT,0x7E,1);store(memory,base.OBJECT+4,config.get('flags',0),2)
    store(memory,base.OBJECT+8,config.get('index',0),1);store(memory,base.OBJECT+9,config.get('class_mode',0),1)
    store(memory,base.OBJECT+0x38,CONTACT)
    store(memory,CONTACT+0x18,CONTEXT if config.get('nonnull',True) else 0)
    store(memory,CONTACT+0x16,config.get('method',1),2)
    store(memory,CONTEXT+0x10,TASK);store(memory,TASK+0x20,INFO)
    store(memory,INFO+2,config.get('type',8),1);store(memory,INFO+6,config.get('count',1),2)
    store(memory,INFO+0xC,config.get('value',29))
    store(memory,load(memory,0x8C06CF4C),1)
    store(memory,load(memory,0x8C06CF50)+config.get('index',0)*0x34,1)
    store(memory,load(memory,0x8C06C9C0)+config.get('index',0)*4,base.WORK)
    store(memory,base.WORK+0x68,GLOBAL_LINK);store(memory,GLOBAL_LINK+0x20,CLASS_INFO);store(memory,GLOBAL_LINK+0x24,1)
    store(memory,CLASS_INFO+2,config.get('class_type',4),1);store(memory,CLASS_INFO+0x38,1)
    pointer=config.get('pointer',2)
    store(memory,base.WORK+0x6C,base.LINK if pointer else 0);store(memory,base.LINK+0x28,base.NODE if pointer==2 else 0)
    for a in (base.WORK+0xB4,base.NODE+0x10):
        for i,v in enumerate((0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00)):store(memory,a+4*i,v)
    table=load(memory,0x8C05E760)
    for i in range(10):
        node=POOL_NODES+i*0x100;flag=POOL_FLAGS+i*0x100
        store(memory,node+0x20,flag);store(memory,flag,0xA5,1)
        store(memory,table+i*4,node if config.get('pool_mask',0x155)&(1<<i) else 0)
    if config.get('pool_alias'):
        # Clearing the first node changes the low byte of the next table
        # pointer, so iteration two must reload and use the new node.
        store(memory,table,POOL_NODES+0x2000)
        store(memory,POOL_NODES+0x2020,table+4)
        store(memory,table+4,POOL_NODES+0x80)
        store(memory,POOL_NODES+0xA0,POOL_FLAGS+0x1000)
        store(memory,POOL_NODES+0x20,POOL_FLAGS+0x2000)
        store(memory,POOL_FLAGS+0x1000,0xA5,1);store(memory,POOL_FLAGS+0x2000,0xA5,1)
        for i in range(2,10):store(memory,table+i*4,0)
    if 'threshold' in config:store(memory,0x8C05E764,config['threshold'])
    state=State();state.r[:]=[(0x12345678+number*0x100+i*0x10101)&0xFFFFFFFF for i in range(16)]
    state.r[4]=base.OBJECT;state.r[5]=base.WORK if config['entry']=='query' else base.AUX;state.r[6]=base.WORK
    state.r[15]=0x8CF00000-(number%4)*0x100
    memory[(state.r[15]&0xFFFFFF)-0x80:state.r[15]&0xFFFFFF]=bytes([0xA5])*0x80
    state.pc={'clear':CLEAR,'predicate':PREDICATE,'active':ACTIVE,'query':QUERY,'status':STATUS}[config['entry']]
    state.pr=0x8C000500;state.sr=0x70000060|(number&1)
    fpu=Fpu();fpu.fpscr=config.get('mode',(number%8)<<19)|config.get('rm',0)|0x40000|config.get('enabled',0)
    fpu.fpul=0xCAFEBABE
    for bank in range(2):fpu.banks[bank][:]=[0xBC000000+bank*0x10000+i*0x100 for i in range(16)]
    return memory,state,fpu,table


def run_case(config,number,allowed):
    memory,initial,fpu,table=prepare(config,number)
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial);expected_fpu=Fpu.from_buffer_copy(fpu)
    special={'macl':0xFEED1234};path=[];expected_events=[];expected_counters={}
    def event(target,mem,regs,sr,pr,f,fpul_value=None):
        shadow=Fpu.from_buffer_copy(f)
        if fpul_value is not None:shadow.fpul=fpul_value
        return (target,tuple(regs),sr,pr,bytes(shadow),load(mem,base.OBJECT+4,2))
    def trace(pc,word,regs,sr,pr,_f):
        assert pc in allowed,hex(pc)
        path.append(pc)
        if pc in TRACKED and pc!=initial.pc:expected_events.append(event(pc,expected_memory,regs,sr,pr,expected_fpu))
    def hook_for(target):
        def hook(mem,regs,sr):
            expected_events.append(event(target,mem,regs,sr,expected.pr,expected_fpu))
            value=[sr];pr_value=[expected.pr]
            callback_config=dict(config,character=load(mem,base.OBJECT+8,1))
            base.apply_service(callback_config,target,mem,regs,value,pr_value,expected_fpu,expected_counters)
            expected_counters['external']=expected_counters.get('external',0)+1
            return value[0]
        return hook
    externals={base.DETACH,base.RESET,base.VECTOR,base.SOUND}
    if config.get('guard'):
        stop=COMPARE if config['guard']=='invalid_enable' else FLOAT
        steps=reference(expected_memory,expected,fpu=expected_fpu,special=special,trace=trace,stop_pc=stop,instruction_budget=3000)
        if config['guard'].startswith('inexact_enable'):
            expected_fpu.fpscr&=~0x3F000
            if config['guard']=='inexact_enable_inexact':expected_fpu.fpscr|=0x1004
        if config['guard']=='invalid_enable':expected_fpu.fpscr=(expected_fpu.fpscr&~0x3F000)|0x10040
        wanted=-1 if config['guard'] in ('precision','reserved_rm') else 1
    else:
        steps=reference(expected_memory,expected,fpu=expected_fpu,special=special,
            hooks={t:hook_for(t) for t in externals},trace=trace,instruction_budget=3000)
        wanted=0
    actual=State.from_buffer_copy(initial);actual_fpu=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    events=[];errors=[];actual_counters={}
    @OBSERVE
    def observe(_ctx,target,mem,f,s,r,p):
        try:events.append(event(target,mem,list(r[:16]),s[0],p[0],f.contents,fpul.value))
        except Exception as exc:errors.append(repr(exc))
    @Call
    def call(_ctx,target,mem,f,s,r,p):
        try:
            assert target not in TRACKED
            callback_config=dict(config,character=load(mem,base.OBJECT+8,1))
            base.apply_service(callback_config,target,mem,r,s,p,f.contents,actual_counters)
            actual_counters['external']=actual_counters.get('external',0)+1
            return 0
        except Exception as exc:errors.append(repr(exc));return -99
    observer=observe if config.get('observe',True) else OBSERVE()
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    args=(view,ctypes.byref(actual_fpu),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None)
    result=functions[config['entry']](*args,call if config['entry']=='status' else Call(),observer) if config['entry'] in ('query','status') else functions[config['entry']](*args,observer)
    assert result==wanted and not errors,(number,config,result,wanted,errors)
    assert memory==expected_memory,(number,config,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,config,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
    # C ABI stores banks/FPSCR in one object and FPUL in a separate pointer.
    assert bytes(actual_fpu)[:132]==bytes(expected_fpu)[:132],(number,config,'banks/FPSCR differ')
    assert fpul.value==expected_fpu.fpul and macl.value==special['macl'],(number,config,'FPUL/MACL differ')
    assert actual_counters==expected_counters
    if config.get('observe',True):assert events==expected_events,(number,config,'boundary differs')
    else:assert not events
    if not config.get('guard'):
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        if config['entry']=='predicate':
            assert actual.r[0]==int(config.get('nonnull',True) and config.get('index',0)==0 and config.get('type',8)==8)
        if config['entry']=='active' and 'threshold' not in config:
            active=config.get('nonnull',True) and config.get('index',0)==0 and config.get('type',8)==8
            value=config.get('value',29);value=value-0x100000000 if value&0x80000000 else value
            result_value=0xFFFFFFFF if active and config.get('count',1)==0 else int(active and config.get('method',1)==1 and value<30)
            assert actual.r[0]==result_value
        if config['entry']=='clear':
            if config.get('pool_alias'):
                assert load(memory,POOL_FLAGS+0x1000,1)==0xA5 and load(memory,POOL_FLAGS+0x2000,1)==0
            else:
                for i in range(10):assert load(memory,POOL_FLAGS+i*0x100,1)==(0 if config.get('pool_mask',0x155)&(1<<i) else 0xA5)
    return path,len(events),steps,actual_counters.get('external',0)


def main():
    manifest_path=PROJECT/'analysis/main_dispatch_active_translation.json'
    manifest=json.loads(manifest_path.read_text())
    for file,h in manifest['sha256'].items():assert digest(Path(file))==h,file
    inventories={int(r['address'],16):r for r in manifest['routines']}
    sets={a:{int(p,16) for p in r['reachable_addresses']} for a,r in inventories.items()}
    parent_manifest_path=PROJECT/'analysis/main_dispatch_shared_translation.json'
    parent=next(r for r in json.loads(parent_manifest_path.read_text())['routines'] if int(r['address'],16)==STATUS)
    parent_set={int(a,16) for a in parent['reachable_addresses']}
    check_manifest_path=PROJECT/'analysis/main_dispatch_shared_check_translation.json'
    check_set={int(a,16) for a in json.loads(check_manifest_path.read_text())['reachable_addresses']}
    allowed=set.union(*sets.values())|parent_set|check_set|set(range(QUERY,0x8C06CEFE,2))
    coverage={a:set() for a in inventories};edge_coverage={a:set() for a in inventories};call_coverage=set()
    counts={k:0 for k in functions};records=[];boundaries=0;guards=0;closed_queries=0;unobserved=0
    for number,config in enumerate(cases()):
        path,b,steps,external_calls=run_case(config,number,allowed)
        for entry,addresses in sets.items():
            coverage[entry].update(set(path)&addresses)
            for i,a in enumerate(path):
                if a not in addresses:continue
                w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
                if w>>8 in (0x89,0x8B) and i+1<len(path):edge_coverage[entry].add((a,path[i+1]))
                if w>>12==0xB:call_coverage.add(a)
        counts[config['entry']]+=1;boundaries+=b;guards+=bool(config.get('guard'))
        closed_queries+=config['entry']=='query' and not config.get('guard') and external_calls==0
        unobserved+=not config.get('observe',True)
        records.append({'input':config,'steps':steps,'boundaries':b,'external_calls':external_calls,'full_state_and_fpul_macl_match':True})
        if (number+1)%500==0:print(number+1,'native active-query group comparisons passed',flush=True)
    for entry,row in inventories.items():
        wanted={(int(a,16),int(b,16)) for a,b in row['conditional_edges']}
        assert coverage[entry]==sets[entry],(hex(entry),'missing rows',sets[entry]-coverage[entry])
        assert edge_coverage[entry]==wanted,(hex(entry),'missing edges',wanted-edge_coverage[entry])
    assert call_coverage=={int(a,16) for r in inventories.values() for a in r['native_call_sites']}
    routines=[{k:r[k] for k in ('name','address','end_exclusive','instruction_bytes','retail_span_sha256')} for r in manifest['routines']]
    for row in routines:row.update(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']))
    files=[Path(__file__),PROJECT/'verify_main_dispatch_shared_helpers.py',PROJECT/'verify_startup.py',manifest_path,
           parent_manifest_path,check_manifest_path,PROJECT/'build_main_dispatch_active_query.ps1',
           PROJECT/'source/sa_main_dispatch_active_connected.c',PROJECT/'source/sa_main_dispatch_shared_helpers.c',
           PROJECT/'source/sa_main_dispatch_status_query.c',PROJECT/'source/sa_main_dispatch_shared_check.c',
           PROJECT/'source/sh4_float_i32.c',PROJECT/'source/sh4_compare_bits.h',PROJECT/'sh4_binary32.py',
           PROJECT/'analysis/main-dispatch-active-query/diagnostics.tsv']
    report={'routines':routines,'source':manifest['source'],'source_sha256':digest(PROJECT/manifest['source']),
            'binary_sha256':digest(base.BINARY),'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'cases_passed':len(records),'counts':counts,'boundary_comparisons':boundaries,
            'instruction_rows_covered':sum(len(v) for v in coverage.values()),
            'conditional_edges_covered':sum(len(v) for v in edge_coverage.values()),'native_call_sites_covered':len(call_coverage),
            'closed_query_cases_with_null_external':closed_queries,'unobserved_cases_passed':unobserved,
            'floating_guard_and_trap_checks_passed':guards,
            'comparison_level':'Full 16 MiB RAM, GPR/SR/PR, FPU banks/FPSCR, separate FPUL and explicit MACL against original SH4 instructions, with ordered native dependency and remaining-service boundaries.',
            'scope':'Three native routines, closed original QUERY chain without external service callbacks, extended STATUS integration, pointer/type/index/count branches, ten-entry pool clearing including alias reloads, int32 conversion/threshold decisions and exact partial states for unsupported/trapping floating instructions.',
            'limitations':['PR=1 or reserved rounding is rejected if the FLOAT path is reached; integer paths remain supported for those modes.',
                           'Enabled floating exceptions return host status with partial state; CPU exception delivery is not emulated.',
                           'Other STATUS callees remain deterministic fixtures; full dispatcher/game/ports remain unfinished.',
                           'Constructed pools/index entries do not establish all runtime states; invalid pointers/MMIO and physical hardware are unverified.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_active_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(records)} cases {counts}; 88 rows / 20 edges / 5 native calls; {boundaries} boundaries; {closed_queries} closed query cases; {guards} floating guards/traps; {unobserved} unobserved cases')


if __name__=='__main__':main()
