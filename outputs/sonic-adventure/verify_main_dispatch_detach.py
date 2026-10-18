"""Compare native detach/vector and both parent connections to retail SH4."""
import ctypes,hashlib,itertools,json
from pathlib import Path

import verify_main_dispatch_active_query as active
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
base=active.base
load,store=base.load,base.store
DETACH,VECTOR,TRANSFORM,CHECK=base.DETACH,base.VECTOR,0x8C078EB0,base.CHECK
STATUS,FLAG=base.STATUS,base.FLAG
ALT_LINK,ALT_DEST,VECTOR_DEST=0x8CE10000,0x8CE20000,0x8CE30000
ALT_RETURN=0x8C000600
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_detach.dll'
LIB=ctypes.CDLL(str(DLL))
functions={'detach':LIB.sa_main_dispatch_detach_connected,
           'status':LIB.sa_main_dispatch_status_detach_connected,
           'flag':LIB.sa_main_dispatch_flag_detach_connected,
           'vector':LIB.sa_main_dispatch_vector_store_8c07cb54}
for name,fn in functions.items():
    fn.argtypes=active.pure_args[:-1]+[Call]+([] if name=='vector' else [active.OBSERVE])
    fn.restype=ctypes.c_int
TRACKED={DETACH,VECTOR,CHECK,active.QUERY,active.ACTIVE,active.PREDICATE,active.CLEAR,base.RESET}


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def cases():
    for index,mode in itertools.product(range(256),range(8)):
        yield {'entry':'vector','index':index|0xAABBCC00,'mode':mode<<19}
    for flags,child,kind,class_mode,mode in itertools.product(
            (0,1,0x800,0x802,0x900,0xFFFF,0x8800),
            (False,True),(4,5,6,7,8),(0,7,1),range(8)):
        c={'entry':'detach','flags':flags,'child':child,'type':kind,'class_mode':class_mode,'mode':mode<<19}
        if will_transform(c) and c['mode']&0x80000:c['guard']=True
        yield c
    for entry,index,kind,class_type,pointer,mode in itertools.product(
            ('status','flag'),(0,1),(4,8),(4,6),range(3),(0,0x100000,0x200000,0x300000)):
        yield {'entry':entry,'index':index,'flags':0x800,'type':kind,'class_type':class_type,'pointer':pointer,'mode':mode}
    for entry,flags,mode in itertools.product(('status','flag'),(0,1,2,0x100,0x4000),(n<<19 for n in range(8))):
        yield {'entry':entry,'index':1,'flags':flags,'type':8,'class_type':6,'pointer':2,'mode':mode}
    for entry,variant,mode in itertools.product(('detach','status','flag'),
            ('mutate','relocate','alias','unobserved','missing','service_error','precision'),
            (0,0x100000,0x200000,0x300000)):
        c={'entry':entry,'index':1,'flags':0x800,'type':4,'class_type':6,'pointer':2,'mode':mode}
        if variant=='mutate':c['mutate']=True
        elif variant=='relocate':c.update(mutate=True,relocate=True)
        elif variant=='alias':c['alias']=True
        elif variant=='unobserved':c['observe']=False
        elif variant=='precision':c.update(mode=mode|0x80000,guard=True)
        else:c['failure']=variant
        yield c
    for mode,index in itertools.product(range(8),(0,0x7F,0x80,0xFF)):
        yield {'entry':'vector','index':index,'mode':mode<<19,'relocate':True}


def will_transform(c):
    # The native CHECK runs after detach clears object flag 0x800, so it uses
    # the contact chain, rather than the original attached CLASS_INFO node.
    mode,kind=c.get('class_mode',0),c.get('type',8)
    return bool(c.get('flags',0x800)&0x800 and not c.get('flags',0x800)&4 and
                (kind==4 and mode in (0,2,3,5,6,7) or kind in (5,6) and mode==7))


def prepare(c,number):
    fixture=dict(c,entry='status',count=1,value=29,index=c.get('index',1),method=1)
    memory,state,fpu,_=active.prepare(fixture,number)
    state.pc={'detach':DETACH,'vector':VECTOR,'status':STATUS,'flag':FLAG}[c['entry']]
    store(memory,active.CONTACT+2,0xA,2)
    store(memory,active.INFO+4,0x1040 if c.get('alias') else 0x40,2)
    store(memory,active.CLASS_INFO+4,0xFFFF,2)
    store(memory,active.GLOBAL_LINK+0x20,active.INFO if c.get('alias') else active.CLASS_INFO if c.get('child',True) else 0)
    store(memory,active.GLOBAL_LINK+0x24,ALT_DEST)
    store(memory,ALT_LINK+0x20,active.CLASS_INFO);store(memory,ALT_LINK+0x24,ALT_DEST+0x100)
    for at in (ALT_DEST,ALT_DEST+0x100,VECTOR_DEST):
        for i in range(16):store(memory,at+4*i,0x87650000+i)
    # Reset's index-one candidate must be nonnull on the positive QUERY path.
    pool=load(memory,0x8C05E840)
    store(memory,pool,active.POOL_NODES);store(memory,pool+8,active.POOL_NODES+0x200)
    table=load(memory,0x8C07CCA4)
    if c['entry']=='vector':
        state.r[4]=c['index']
        if c.get('relocate'):table=0x8CE40000;store(memory,0x8C07CCA4,table)
    store(memory,table+(c.get('index',1)&255)*4,VECTOR_DEST)
    # SZ=1's FR3 store uses the opposite bank pair and reaches saved PR.
    # Direct tests deliberately return to a different sentinel. Connected
    # tests retain the actual caller return so the parent can execute onward.
    if c['mode']&0x100000:
        caller_pr=ALT_RETURN if c['entry']=='detach' else 0x8C0AFDAA if c['entry']=='status' else 0x8C0B1212
        fpu.banks[((fpu.fpscr>>21)&1)^1][3]=caller_pr
    return memory,state,fpu


def run_case(c,number,allowed):
    memory,initial,fpu=prepare(c,number)
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    path=[];ee=[];ec={};special={'macl':0xFEED1234}
    def event(target,mem,r,sr,pr,f,fpul=None):
        shadow=Fpu.from_buffer_copy(f)
        if fpul is not None:shadow.fpul=fpul
        return (target,tuple(r),sr,pr,bytes(shadow),load(mem,base.OBJECT+4,2),load(mem,base.WORK+0x68))
    def trace(pc,word,r,sr,pr,_f):
        assert pc in allowed,hex(pc)
        path.append(pc)
        if pc in TRACKED and pc!=initial.pc:ee.append(event(pc,expected_memory,r,sr,pr,ef))
    def service(target,mem,r,sr,pr,f,counters):
        assert target in (TRANSFORM,base.SOUND),hex(target)
        counters[hex(target)]=counters.get(hex(target),0)+1
        if target==TRANSFORM:
            assert r[4]==base.OBJECT
            vector=r[5]
            # Fixture output, not a claim to recover the transformation.
            for i,v in enumerate((0xBF800000,0x7FC01234,0x80000000)):store(mem,vector+4*i,v)
            if c.get('relocate'):store(mem,base.WORK+0x68,ALT_LINK)
            if c.get('mutate'):
                for i in range(8):r[i]=(0xDE000000+i*0x10101)&0xFFFFFFFF
                sr[0]^=1;f.fpscr^=0x380000
                for bank in range(2):
                    for i in range(8):f.banks[bank][i]=0xEF000000+bank*0x10000+i
        else:
            callback=dict(c,character=load(mem,base.OBJECT+8,1))
            base.apply_service(callback,target,mem,r,sr,pr,f,{})
    def hook_for(target):
        def hook(mem,r,sr):
            ee.append(event(target,mem,r,sr,expected.pr,ef));s=[sr];pr=[expected.pr]
            service(target,mem,r,s,pr,ef,ec)
            return s[0]
        return hook
    stop=0x8C0B1DA8 if c.get('guard') else TRANSFORM if c.get('failure') else None
    if stop is None and c['entry']=='detach' and will_transform(c) and c['mode']&0x100000:stop=ALT_RETURN
    steps=reference(expected_memory,expected,fpu=ef,special=special,
        trace=trace,stop_pc=stop,hooks={} if c.get('failure') or c.get('guard') else {t:hook_for(t) for t in (TRANSFORM,base.SOUND)},instruction_budget=4000)
    if c.get('failure'):
        ee.append(event(TRANSFORM,expected_memory,list(expected.r),expected.sr,expected.pr,ef))
        if c['failure']=='service_error':
            r=list(expected.r);s=[expected.sr];p=[expected.pr]
            service(TRANSFORM,expected_memory,r,s,p,ef,ec);expected.r[:]=r;expected.sr=s[0]
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
            service(t,mem,r,s,p,f.contents,ac)
            return -17 if c.get('failure')=='service_error' else 0
        except Exception as exc:errors.append(repr(exc));return -99
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    args=(view,ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,
          Call() if c.get('failure')=='missing' or c['entry']=='vector' else call)
    result=functions[c['entry']](*args,*([] if c['entry']=='vector' else [observe if c.get('observe',True) else active.OBSERVE()]))
    wanted=-1 if c.get('guard') else -2 if c.get('failure')=='missing' else -17 if c.get('failure') else 0
    assert result==wanted and not errors,(number,c,result,errors)
    assert memory==expected_memory,(number,c,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,c,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132],(number,c,'FPU differs')
    assert fpul.value==ef.fpul and macl.value==special['macl']
    assert ac==ec,(number,c,'external calls differ')
    if c.get('observe',True) and c['entry']!='vector':assert ae==ee,(number,c,'boundary differs')
    else:assert not ae
    if c['entry']=='detach' and not c.get('failure') and not c.get('guard'):
        assert actual.r[15]==initial.r[15]
        assert pr.value==(ALT_RETURN if will_transform(c) and c['mode']&0x100000 else initial.pr)
        assert load(memory,base.OBJECT+4,2)==c['flags']&~0x800
        if c['flags']&0x800:assert load(memory,base.WORK+0x68)==0
    return path,len(ae),steps,ac


def main():
    manifest_path=PROJECT/'analysis/main_dispatch_detach_translation.json'
    manifest=json.loads(manifest_path.read_text())
    for p,h in manifest['sha256'].items():assert digest(Path(p))==h,p
    inventories={}
    for item in manifest['routines']:
        address=int(item['address'],16)
        rows={int(a,16) for a in item['reachable_addresses']}
        inventories[address]={'rows':rows,'edges':{tuple(int(a,16) for a in edge) for edge in item['conditional_edges']},
                              'calls':{int(a,16) for a in item['external_call_sites']}}
    allowed=set().union(*(v['rows'] for v in inventories.values()))
    for filename in ('main_dispatch_shared_translation.json','main_dispatch_active_translation.json'):
        for item in json.loads((PROJECT/'analysis'/filename).read_text())['routines']:
            allowed.update(int(a,16) for a in item['reachable_addresses'])
    allowed.update(int(a,16) for a in json.loads((PROJECT/'analysis/main_dispatch_shared_check_translation.json').read_text())['reachable_addresses'])
    allowed.update(range(active.QUERY,0x8C06CEFE,2));allowed.update(range(base.RESET,0x8C05E7D4,2))
    coverage={a:set() for a in inventories};edges={a:set() for a in inventories};calls={a:set() for a in inventories}
    counts={k:0 for k in functions};records=[];boundaries=0;guards=0;failures=0;closed_flag=0;closed_detach=0;unobserved=0
    parent_rows={'status':set(),'flag':set()};linked_detach_rows={'status':set(),'flag':set()}
    for number,c in enumerate(cases()):
        path,b,steps,external=run_case(c,number,allowed)
        counts[c['entry']]+=1;boundaries+=b;guards+=bool(c.get('guard'));failures+=bool(c.get('failure'))
        closed_flag+=c['entry']=='flag' and not external and not c.get('guard') and not c.get('failure')
        closed_detach+=c['entry']=='detach' and not external and not c.get('guard') and not c.get('failure')
        unobserved+=not c.get('observe',True)
        for a,inventory in inventories.items():
            coverage[a].update(set(path)&inventory['rows'])
            for i,pc in enumerate(path):
                if pc not in inventory['rows']:continue
                w=int.from_bytes(base.binary[pc-0x8C010000:pc-0x8C010000+2],'little')
                if w>>8 in (0x89,0x8B,0x8D,0x8F):
                    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
                    if j<len(path):edges[a].add((pc,path[j]))
                if pc in inventory['calls']:calls[a].add(pc)
        if c['entry'] in parent_rows:
            parent_rows[c['entry']].update(pc for pc in path if STATUS<=pc<0x8C0AFE20 or FLAG<=pc<0x8C0B122E)
            linked_detach_rows[c['entry']].update(set(path)&inventories[DETACH]['rows'])
        records.append({'input':c,'steps':steps,'boundaries':b,'external_calls':external,'full_state_match':True})
        if (number+1)%500==0:print(number+1,'detach/vector/caller comparisons passed',flush=True)
    for a,inventory in inventories.items():
        assert coverage[a]==inventory['rows'],(hex(a),'uncovered rows',inventory['rows']-coverage[a])
        assert edges[a]==inventory['edges'],(hex(a),'uncovered edges',inventory['edges']-edges[a])
        assert calls[a]==inventory['calls']
    files=[Path(__file__),manifest_path,PROJECT/'verify_startup.py',PROJECT/'verify_main_dispatch_active_query.py',
           PROJECT/'verify_main_dispatch_shared_helpers.py',PROJECT/'build_main_dispatch_detach.ps1',
           PROJECT/'analysis/main-dispatch-detach/diagnostics.tsv']
    for name in ('sa_main_dispatch_detach_connected.c','sa_main_dispatch_shared_helpers.c',
                 'sa_main_dispatch_status_query.c','sa_main_dispatch_shared_check.c','sa_main_dispatch_active_query.c',
                 'sa_main_dispatch_reset.c','sh4_float_i32.c','sh4_float_i32.h','sh4_compare_bits.h','fpu_cleanup_transfer.h'):
        files.append(PROJECT/'source'/name)
    files.extend(PROJECT/'analysis'/s for s in ('main_dispatch_shared_translation.json','main_dispatch_active_translation.json','main_dispatch_shared_check_translation.json'))
    report={'routines':manifest['routines'],'source':manifest['source'],'source_sha256':digest(PROJECT/manifest['source']),
            'binary_sha256':digest(base.BINARY),'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'cases_passed':len(records),'counts':counts,'boundary_comparisons':boundaries,
            'instruction_rows_covered':sum(map(len,coverage.values())),
            'conditional_edges_covered':sum(map(len,edges.values())),'native_or_external_call_sites_covered':sum(map(len,calls.values())),
            'linked_detach_rows_by_parent':{k:len(v) for k,v in linked_detach_rows.items()},
            'parent_rows_covered':{k:len(v) for k,v in parent_rows.items()},
            'closed_flag_cases_without_external_call':closed_flag,'closed_detach_cases_without_external_call':closed_detach,
            'precision_partial_guards_passed':guards,'partial_service_error_cases_passed':failures,'unobserved_cases_passed':unobserved,
            'comparison_level':'Complete 16 MiB RAM, GPR/SR/PR, physical FPU banks/FPSCR, separate FPUL/MACL and ordered native/external entry snapshots against retail SH4 instructions.',
            'scope':'75 native instructions, native CHECK inside detach, both STATUS/FLAG caller connections, native vector writer, full byte-index range, raw SZ/FR transfers, saved-PR overlap, callback mutations/reloaded links and exact partial error/PR guards.',
            'limitations':['Transform 8c078eb0 and sound 8c0940da remain synthetic services; their behavior is not recovered here.',
                           'Reached FLDI0/FLDI1 requires PR=0; PR=1 is a bounded host error, not hardware exception delivery.',
                           'SZ=1 stack writes overlap saved PR; direct cases verify an alternate return, while connected transform cases use the required caller return value in the overlapping pair.',
                           'Retail mapped/nonnull pointer preconditions are required; invalid pointer exceptions, MMIO, hardware and all gameplay states are unverified.',
                           'Full dispatcher/game and playable ports remain unfinished.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_detach_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k in ('cases_passed','counts','boundary_comparisons','instruction_rows_covered','conditional_edges_covered','parent_rows_covered','linked_detach_rows_by_parent','precision_partial_guards_passed','partial_service_error_cases_passed','closed_flag_cases_without_external_call','closed_detach_cases_without_external_call','unobserved_cases_passed')},indent=2))


if __name__=='__main__':main()
