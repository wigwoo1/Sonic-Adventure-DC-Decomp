"""Verify pure shared classification and both native transition connections."""
import ctypes,hashlib,itertools,json
from pathlib import Path

import verify_main_dispatch_shared_helpers as base
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
CHECK,QUERY,ACTIVE=base.CHECK,base.QUERY,0x8C05E6E8
SOURCE='source/sa_main_dispatch_shared_check.c'
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_shared_check.dll'
LIB=ctypes.CDLL(str(DLL))
OBSERVE=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P)
common=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
functions={'check':LIB.sa_main_dispatch_shared_check_8c06c880,
           'status':LIB.sa_main_dispatch_status_check_connected,'flag':LIB.sa_main_dispatch_flag_check_connected}
for name,fn in functions.items():fn.argtypes=common+([] if name=='check' else [OBSERVE]);fn.restype=ctypes.c_int
CONTACT,CONTACT_NODE,CONTACT_TASK,INNER,GLOBAL_LINK,GATE,SUBOBJECT=0x8CB00000,0x8CB10000,0x8CB20000,0x8CB30000,0x8CB50000,0x8CB60000,0x8CB70000
load,store=base.load,base.store


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def classification(config):
    if config.get('stop'):return 0
    mode,kind=config['class_mode'],config['type']
    if mode in (0,2,3,5,6):return {4:0x200,5:0x400,7:0x800}.get(kind,0)
    if mode==7:return {4:0x200,5:0x600,6:0x600,7:0x800}.get(kind,0)
    return 0


def cases():
    for attached,mode,kind in itertools.product((False,True),range(256),(0,4,5,6,7,0xFF)):
        yield {'entry':'check','flags':0x800 if attached else 0,'class_mode':mode,'type':kind,'index':7}
    for index in range(256):
        yield {'entry':'check','flags':0x800,'class_mode':7,'type':5,'index':index}
    for attached,stop,mode in [(a,s,m) for a,values in (
            (False,('root_zero','root_missing_two','root_missing_eight','input_blocked','contact_null','inner_ineligible','inner_inhibited')),
            (True,('gate_null','inner_null','subobject_null')))
            for s,m in itertools.product(values,range(8))]:
        yield {'entry':'check','flags':0x800 if attached else 0,'class_mode':7,'type':5,'index':1,'stop':stop,'mode':mode<<19}
    for flags,mode,kind in itertools.product((0,1,2,0x100,0x800),(0,2,3,5,6,7,1,0xFF),(4,5,6,7)):
        yield {'entry':'flag','flags':flags,'class_mode':mode,'type':kind,'index':1}
    for flags,mode,kind,value,pointer in itertools.product((0,0x800,0x802),
            (0,2,3,5,6,7,1,0xFF),(4,5,6,7),(0,1),range(3)):
        yield {'entry':'status','flags':flags,'class_mode':mode,'type':kind,'index':0x80,'active_result':value,'pointer':pointer}
    for mode,inactive in itertools.product((n<<19 for n in range(8)),('flag','mask')):
        yield {'entry':'status','flags':0x4000 if inactive=='flag' else 0x800,
               'class_mode':0,'type':4,'index':1,'mode':mode,
               'query_mask':0 if inactive=='mask' else 1}
    for mode,kind,fpscr in itertools.product((0,7),(4,5),(n<<19 for n in range(8))):
        if not classification({'class_mode':mode,'type':kind})&0x200:continue
        yield {'entry':'flag','flags':0x800,'class_mode':mode,'type':kind,'index':1,'mode':fpscr,'observe':False,'no_external':True}
    for entry,failure in itertools.product(('status','flag'),('missing','service_error')):
        yield {'entry':entry,'flags':0x800,'class_mode':0,'type':6,'index':1,'mode':0x380000,
               'pointer':2,'failure':failure,'mutate':failure=='service_error'}


def prepare(config,number):
    memory=bytearray(base.baseline)
    store(memory,base.OBJECT,0x7E,1);store(memory,base.OBJECT+4,config['flags'],2)
    store(memory,base.OBJECT+8,config['index'],1);store(memory,base.OBJECT+9,config['class_mode'],1)
    store(memory,base.OBJECT+0x38,CONTACT)
    store(memory,CONTACT+2,0xA,2);store(memory,CONTACT+0x18,CONTACT_NODE)
    store(memory,CONTACT_NODE+0x10,CONTACT_TASK);store(memory,CONTACT_TASK+0x20,INNER)
    store(memory,INNER+2,config['type'],1);store(memory,INNER+4,0x40,2);store(memory,INNER+0x38,SUBOBJECT)
    table=load(memory,0x8C06C9C0)
    store(memory,table+config['index']*4,base.WORK)
    store(memory,base.WORK+0x68,GLOBAL_LINK);store(memory,GLOBAL_LINK+0x20,INNER);store(memory,GLOBAL_LINK+0x24,GATE)
    stop=config.get('stop')
    if stop=='root_zero':store(memory,CONTACT+2,0,2)
    if stop=='root_missing_two':store(memory,CONTACT+2,8,2)
    if stop=='root_missing_eight':store(memory,CONTACT+2,2,2)
    if stop=='input_blocked':store(memory,base.OBJECT+4,config['flags']|4,2)
    if stop=='contact_null':store(memory,CONTACT+0x18,0)
    if stop=='inner_ineligible':store(memory,INNER+4,0,2)
    if stop=='inner_inhibited':store(memory,INNER+4,0x1040,2)
    if stop=='gate_null':store(memory,GLOBAL_LINK+0x24,0)
    if stop=='inner_null':store(memory,GLOBAL_LINK+0x20,0)
    if stop=='subobject_null':store(memory,INNER+0x38,0)
    store(memory,base.WORK+0x124,0xA5A5,2);store(memory,base.WORK+8,0x1357,2)
    pointer=config.get('pointer',2)
    store(memory,base.WORK+0x6C,base.LINK if pointer else 0)
    store(memory,base.LINK+0x28,base.NODE if pointer==2 else 0)
    for address in (base.WORK+0xB4,base.NODE+0x10):
        for i,value in enumerate((0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00)):store(memory,address+i*4,value)
    store(memory,load(memory,0x8C06CF4C),config.get('query_mask',1))
    store(memory,load(memory,0x8C06CF50)+config['index']*0x34,1)
    state=State();state.r[:]=[(0x12345678+number*0x100+i*0x10101)&0xFFFFFFFF for i in range(16)]
    state.r[4]=base.OBJECT;state.r[5]=base.AUX;state.r[6]=base.WORK
    state.r[15]=0x8CF00000-(number%4)*0x100
    memory[(state.r[15]&0xFFFFFF)-0x80:state.r[15]&0xFFFFFF]=bytes([0xA5])*0x80
    state.pc={'check':CHECK,'status':base.STATUS,'flag':base.FLAG}[config['entry']]
    state.sr=0x70000060|(number&1);state.pr=0x8C000500
    fpu=Fpu();fpu.fpscr=config.get('mode',(number%8)<<19)|0x40000;fpu.fpul=0xCAFEBABE
    for bank in range(2):fpu.banks[bank][:]=[0xBC000000+bank*0x10000+i*0x100 for i in range(16)]
    return memory,state,fpu


def service(config,target,memory,regs,sr,pr,fpu,counters):
    assert target not in (CHECK,QUERY),'A recovered dependency escaped native dispatch'
    if target==ACTIVE:
        counters['active']=counters.get('active',0)+1
        store(memory,0x8CAE6800+counters['active']*4,0xF1000000|counters['active'])
        if config.get('mutate'):
            for i in range(8):regs[i]=(0xDE000000+i*0x10101)&0xFFFFFFFF
            sr[0]^=1;fpu.fpscr^=0x380000
        regs[0]=config.get('active_result',0)
    else:
        callback_config=dict(config,character=load(memory,base.OBJECT+8,1))
        base.apply_service(callback_config,target,memory,regs,sr,pr,fpu,counters)
    counters['external']=counters.get('external',0)+1


def run_case(config,number,allowed):
    memory,initial,fpu=prepare(config,number)
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial);expected_fpu=Fpu.from_buffer_copy(fpu)
    expected_counters={};expected_events=[];special={'macl':0xFEED1234};path=[]
    def event(target,mem,regs,sr,pr,f):return (target,tuple(regs),sr,pr,bytes(f),load(mem,base.OBJECT+4,2),load(mem,base.OBJECT+9,1))
    def trace(pc,word,regs,sr,pr,_f):
        assert pc in allowed,hex(pc)
        path.append(pc)
        if pc in (CHECK,QUERY) and pc!=initial.pc:expected_events.append(event(pc,expected_memory,regs,sr,pr,expected_fpu))
    def hook_for(target):
        def hook(mem,regs,sr):
            expected_events.append(event(target,mem,regs,sr,expected.pr,expected_fpu))
            value=[sr];pr_value=[expected.pr]
            service(config,target,mem,regs,value,pr_value,expected_fpu,expected_counters)
            return value[0]
        return hook
    externals={ACTIVE,base.DETACH,base.RESET,base.VECTOR,base.SOUND}
    failed_target=ACTIVE if config['entry']=='status' else base.DETACH
    if config.get('failure'):
        steps=reference(expected_memory,expected,fpu=expected_fpu,special=special,
            stop_pc=failed_target,trace=trace,instruction_budget=2000)
        expected_events.append(event(failed_target,expected_memory,list(expected.r),expected.sr,expected.pr,expected_fpu))
        if config['failure']=='service_error':
            regs=list(expected.r);value=[expected.sr];pr_value=[expected.pr]
            service(config,failed_target,expected_memory,regs,value,pr_value,expected_fpu,expected_counters)
            expected.r[:]=regs;expected.sr=value[0]
    else:
        steps=reference(expected_memory,expected,hooks={a:hook_for(a) for a in externals},
            fpu=expected_fpu,special=special,trace=trace,instruction_budget=2000)
    actual=State.from_buffer_copy(initial);actual_fpu=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    actual_events=[];actual_counters={};errors=[]
    @OBSERVE
    def observe(_ctx,target,mem,f,s,r,p):
        try:actual_events.append(event(target,mem,list(r[:16]),s[0],p[0],f.contents))
        except Exception as exc:errors.append(repr(exc))
    @Call
    def call(_ctx,target,mem,f,s,r,p):
        try:
            service(config,target,mem,r,s,p,f.contents,actual_counters)
            return -17 if config.get('failure')=='service_error' else 0
        except Exception as exc:errors.append(repr(exc));return -99
    external=call
    if config['entry']=='check' or config.get('no_external') or config.get('failure')=='missing':external=Call()
    if config['entry']=='flag' and not config.get('failure'):
        flags=config['flags']
        needs_service=not flags&3 and not flags&0x100 and flags&0x800 and not classification(config)&0x200
        if not needs_service:external=Call()
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    args=(view,ctypes.byref(actual_fpu),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,external)
    result=functions['check'](*args) if config['entry']=='check' else functions[config['entry']](*args,observe if config.get('observe',True) else OBSERVE())
    want=-2 if config.get('failure')=='missing' else -17 if config.get('failure') else 0
    assert result==want and not errors,(number,config,result,errors)
    assert memory==expected_memory,(number,config,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,config,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
    assert bytes(actual_fpu)==bytes(expected_fpu),(number,config,'FPU differs')
    assert fpul.value==fpu.fpul and macl.value==special['macl'],(number,config,'FPUL/MACL differs')
    assert actual_counters==expected_counters
    if config.get('observe',True):assert actual_events==expected_events,(number,config,'boundary differs')
    else:assert not actual_events
    if not config.get('failure'):
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        assert list(actual.r)[12:15]==list(initial.r)[12:15]
        if config['entry']=='check':assert actual.r[0]==classification(config),(number,config,hex(actual.r[0]))
        if config['entry']=='flag':
            flags=config['flags'];assert actual.r[0]==int(not flags&3)
            if flags&3:assert load(memory,base.OBJECT,1)==0x7E
            elif flags&0x100:assert load(memory,base.OBJECT,1)==8
            else:assert load(memory,base.OBJECT,1)==(0x1C if flags&0x800 and classification(config)&0x200 else 0xC)
    return path,len(actual_events),steps,actual_counters.get('external',0),not bool(external)


def main():
    manifest_path=PROJECT/'analysis/main_dispatch_shared_check_translation.json'
    manifest=json.loads(manifest_path.read_text())
    for file,h in manifest['sha256'].items():assert digest(Path(file))==h,file
    check_addresses={int(a,16) for a in manifest['reachable_addresses']}
    check_words={a:int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little') for a in check_addresses}
    wanted_edges={(int(a,16),int(b,16)) for a,b in manifest['conditional_edges']}
    parent_manifest_path=PROJECT/'analysis/main_dispatch_shared_translation.json'
    parents=json.loads(parent_manifest_path.read_text())['routines']
    parent_sets={int(r['address'],16):{int(a,16) for a in r['reachable_addresses']} for r in parents}
    query_addresses=set(range(QUERY,0x8C06CEFE,2))
    allowed=check_addresses|query_addresses|set.union(*parent_sets.values())
    coverage={k:set() for k in functions};edge_coverage=set();parent_coverage={'status':set(),'flag':set()}
    records=[];counts={k:0 for k in functions};boundaries=0;closed_flags=0;unobserved=0
    for number,config in enumerate(cases()):
        path,b,steps,external_calls,null_external=run_case(config,number,allowed)
        kind=config['entry'];coverage[kind].update(set(path)&check_addresses)
        if kind!='check':parent_coverage[kind].update(set(path)&parent_sets[base.STATUS if kind=='status' else base.FLAG])
        if kind=='check':
            for i,a in enumerate(path):
                w=check_words[a]
                if w>>8 in (0x89,0x8B,0x8D,0x8F):
                    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
                    if j<len(path):edge_coverage.add((a,path[j]))
        counts[kind]+=1;boundaries+=b
        closed_flags+=kind=='flag' and null_external and not config.get('failure')
        unobserved+=not config.get('observe',True)
        records.append({'input':config,'steps':steps,'boundaries':b,'external_calls':external_calls,
                        'null_external_service':null_external,'full_state_match':True})
        if (number+1)%500==0:print(number+1,'shared-check/native parent comparisons passed',flush=True)
    assert coverage['check']==check_addresses,('uncovered checker rows',check_addresses-coverage['check'])
    assert edge_coverage==wanted_edges,('uncovered checker edges',wanted_edges-edge_coverage)
    for name,entry in (('status',base.STATUS),('flag',base.FLAG)):
        assert parent_coverage[name]==parent_sets[entry],(name,'uncovered parent rows',parent_sets[entry]-parent_coverage[name])
    files=[Path(__file__),PROJECT/'verify_main_dispatch_shared_helpers.py',PROJECT/'verify_startup.py',
           manifest_path,parent_manifest_path,PROJECT/'build_main_dispatch_shared_check.ps1',
           PROJECT/'source/sa_main_dispatch_shared_helpers.c',PROJECT/'source/sa_main_dispatch_status_query.c',
           PROJECT/'source/sa_main_dispatch_shared_check_connected.c',
           PROJECT/'analysis/main-dispatch-shared-check/diagnostics.tsv']
    report={'address':hex(CHECK),'end_exclusive':'0x8c06ca30','indexed_instruction_bytes':390,
            'source':SOURCE,'source_sha256':digest(PROJECT/SOURCE),'binary_sha256':digest(base.BINARY),
            'retail_span_sha256':hashlib.sha256(base.binary[CHECK-0x8C010000:0x8C06CA30-0x8C010000]).hexdigest(),
            'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'cases_passed':len(records),'counts':counts,'boundary_comparisons':boundaries,
            'direct_check_rows_covered':len(coverage['check']),'direct_conditional_edges_covered':len(edge_coverage),
            'check_rows_covered_through_parents':{k:len(coverage[k]) for k in parent_coverage},
            'parent_rows_covered':{k:len(v) for k,v in parent_coverage.items()},
            'closed_flag_cases_with_null_external_service':closed_flags,'unobserved_cases_passed':unobserved,
            'missing_service_checks_passed':2,'partial_service_error_checks_passed':2,
            'comparison_level':'Full 16 MiB RAM, GPR/SR/PR/FPU and explicit FPUL/MACL compared with original SH4 execution; ordered native dependency and external service entry states; independent return-mask assertions.',
            'scope':'195-row pure checker with both pointer paths, all mode bytes, classified/default types, constructed index entries and guarded pointer exits; both original native parents connected to native CHECK and QUERY, closed FLAG paths without services and partial errors.',
            'limitations':['Only the attached checker path is reachable from these two callers; all 195 rows and 76 edges are covered in direct tests.',
                           'Remaining parent callees, including active query service 0x8c05e6e8, remain deterministic fixtures.',
                           'Constructed table entries do not prove every index valid in the real game.',
                           'Physical hardware, invalid pointers, memory exceptions/MMIO, live game behavior and full ports remain unverified.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_shared_check_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(records)} cases {counts}; 195 checker rows / {len(edge_coverage)} edges; parent rows {report["parent_rows_covered"]}; {boundaries} boundaries; {closed_flags} closed flag cases with null external service; {unobserved} unobserved cases; 4 partial error checks')


if __name__=='__main__':main()
