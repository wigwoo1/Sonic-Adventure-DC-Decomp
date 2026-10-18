"""Standalone RAM/register cleanup, recursion and pointer-reload comparisons."""
import ctypes,gc,hashlib,itertools,json
from pathlib import Path
import verify_main_dispatch_notification as base
from verify_startup import State
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call
from verify_main_dispatch_active_query import OBSERVE
PROJECT=Path(__file__).resolve().parent
load,store=base.load,base.store
TASK,PARENT,BEFORE,AFTER,CHILD1,CHILD2=0x8CC10000,0x8CC00000,0x8CC20000,0x8CC20100,0x8CC30000,0x8CC30100
ATTACH,ALT_ATTACH=0x8CC40000,0x8CC40100
DESTRUCTOR,UPDATE=0x8C00F100,0x8C00F200
RELEASE1,RELEASE2=base.literal(0x8C098A60),base.literal(0x8C098A64)
EXTERNAL={DESTRUCTOR,UPDATE,RELEASE1,RELEASE2}
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_notification_cleanup.dll';LIB=ctypes.CDLL(str(DLL))
SPECS={'dispose':(0x8C09865C,'sa_task_disposal_connected'),'work':(0x8C098998,'sa_task_work_release_connected'),
       'children':(0x8C098E60,'sa_task_children_delete_connected'),'update':(0x8C098E98,'sa_task_children_update_connected'),
       'heap':(0x8C098B42,'sa_task_heap_release_connected')}
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[OBSERVE];fn.restype=ctypes.c_int
BLOCKS=(0,80,112,144,176,256)
PAYLOAD=[base.ARENA+off+16 for off in BLOCKS]


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def prepare(c,n):
    m,s,f=base.prepare({'entry':'notify','mode':c.get('mode',0)},n)
    s.pc=SPECS[c['entry']][0];s.r[4]=TASK
    for node in (TASK,PARENT,BEFORE,AFTER,CHILD1,CHILD2):
        for i in range(12):store(m,node+4*i,0)
    nxt=AFTER if c.get('next') else 0;prev=BEFORE if c.get('prev') else 0;parent=PARENT if c.get('parent') else 0
    store(m,TASK,nxt);store(m,TASK+4,prev);store(m,TASK+8,parent)
    store(m,BEFORE,TASK);store(m,AFTER+4,TASK);store(m,PARENT+12,BEFORE if prev else TASK)
    for i in range(8):store(m,base.LEVELS+4*i,0)
    if c.get('level',0)<8:store(m,base.LEVELS+4*c.get('level',0),BEFORE if prev else TASK)
    store(m,base.FREE_TASK,base.NEXTTASK if c.get('free_head',True) else 0);store(m,base.NEXTTASK,0);store(m,base.NEXTTASK+4,0)
    for off,nxt_off,prev_off in zip(BLOCKS[:-1],BLOCKS[1:],(None,*BLOCKS[:-2])):
        at=base.ARENA+off
        for i,v in enumerate((0,0,base.ARENA+nxt_off,0 if prev_off is None else base.ARENA+prev_off)):store(m,at+4*i,v)
        for a in range(at+16,base.ARENA+nxt_off,4):store(m,a,0)
    free=base.ARENA+256;end=base.ARENA+512
    for i,v in enumerate((end,0,end,base.ARENA+176)):store(m,free+4*i,v)
    for i,v in enumerate((0,free,0,free)):store(m,end+4*i,v)
    store(m,base.FREE_HEAP,free)
    if 'prefree' in c:
        # Independently construct physical blocks and a valid free-list order,
        # including holes on both sides of the allocation being released.
        physical=[]
        for i,(start,stop) in enumerate(zip(BLOCKS,(*BLOCKS[1:],512))):
            is_free=bool(c['prefree']&(1<<i))
            if is_free and physical and physical[-1][2]:physical[-1]=(physical[-1][0],stop,True)
            else:physical.append((start,stop,is_free))
        previous_block=0
        for start,stop,is_free in physical:
            at=base.ARENA+start
            for i,v in enumerate((0,0,base.ARENA+stop,previous_block)):store(m,at+i*4,v)
            previous_block=at
        free_starts=[base.ARENA+start for start,_,is_free in physical if is_free]
        if c.get('free_order')==1:free_starts.reverse()
        elif c.get('free_order')==2 and free_starts:free_starts=free_starts[1:]+free_starts[:1]
        chain=free_starts+[end]
        for i,at in enumerate(chain):
            store(m,at,chain[i+1] if i+1<len(chain) else 0);store(m,at+4,chain[i-1] if i else 0)
        store(m,end+8,0);store(m,end+12,previous_block);store(m,base.FREE_HEAP,chain[0])
    mask=c.get('mask',0)
    for bit,slot in enumerate((32,40,36,44)):store(m,TASK+slot,PAYLOAD[bit] if mask&(1<<bit) else 0)
    if c['entry']=='work':store(m,TASK+32,PAYLOAD[0])
    store(m,PAYLOAD[0]+56,c.get('resource1',0));store(m,PAYLOAD[0]+60,c.get('resource2',0))
    store(m,PAYLOAD[4]+56,0);store(m,PAYLOAD[4]+60,1)
    if c.get('destructor'):store(m,TASK+24,DESTRUCTOR)
    if c.get('attachment'):store(m,TASK+28,ATTACH)
    store(m,ATTACH,0xA5FF1234);store(m,ALT_ATTACH,0x80FF5678)
    count=c.get('children',0);store(m,TASK+12,CHILD1 if count else 0)
    for i,node in enumerate((CHILD1,CHILD2)):
        store(m,node,CHILD2 if i==0 and count>1 else 0);store(m,node+4,CHILD1 if i else 0);store(m,node+8,TASK)
        store(m,node+16,UPDATE if c.get('child_external') else 0x8C09865C)
    if c.get('special'):store(m,TASK+16,0x8C098E60)
    if c['entry']=='heap':s.r[4]=0 if c.get('heap_null') else PAYLOAD[c.get('heap_block',0)]
    return m,s,f


def cases():
    for nxt,prev,parent,level,mode in itertools.product((False,True),(False,True),(False,True),(0,3,7,8),range(8)):
        yield {'entry':'dispose','next':nxt,'prev':prev,'parent':parent,'level':level,'mode':mode<<19,'attachment':bool(mode&1)}
    for mask,mode,destructor,attachment in itertools.product(range(16),range(8),(False,True),(False,True)):
        yield {'entry':'dispose','mask':mask,'mode':mode<<19,'destructor':destructor,'attachment':attachment,'children':mask%3,
               'resource1':1,'resource2':1,'next':bool(mask&1),'prev':bool(mask&2),'parent':bool(mask&4),'free_head':bool(mask&8)}
    for first,second,swap,mode in itertools.product((0,1),(0,1),(False,True),range(8)):
        if swap and not first:continue
        yield {'entry':'work','resource1':first,'resource2':second,'swap_work':swap,'mode':mode<<19,'flip':True}
    for entry,count,special,remove,mutate,mode in itertools.product(('children','update'),range(3),(False,True),(False,True),(False,True),range(8)):
        if entry=='update' and special:continue
        yield {'entry':entry,'children':count,'special':special,'child_external':True,'remove_child':remove,'mutate_child_next':mutate,'mode':mode<<19}
    for block,mode in itertools.product(range(5),range(8)):yield {'entry':'heap','heap_block':block,'mode':mode<<19}
    for block,prefree,order,bank in itertools.product((2,5),range(64),range(3),range(2)):
        if prefree&(1<<block):continue
        yield {'entry':'heap','heap_block':block,'prefree':prefree,'free_order':order,'mode':bank<<21}
    for mode in range(8):
        yield {'entry':'heap','heap_null':True,'mode':mode<<19}
        yield {'entry':'dispose','destructor':True,'attachment':True,'switch_attach':True,'relink':True,'mode':mode<<19,'flip':True}
        yield {'entry':'dispose','mode':mode<<19,'null_external':True}
        yield {'entry':'dispose','destructor':True,'mode':mode<<19,'null_external':True}


class Stopped(Exception):pass
def run(c,n,allowed,tracked):
    m,initial,fpu=prepare(c,n);em=bytearray(m);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
    special={'macl':0xFEED1234};path=[];ee=[];calls=[];wanted=0;errors=[]
    def event(t,r,sr,pr,f,fpul,macl):return t,tuple(r),sr,pr,bytes(f)[:132],fpul,macl
    def trace(pc,w,r,sr,pr,_f):
        assert pc in allowed,hex(pc);path.append(pc)
        if pc in tracked and len(path)>1:ee.append(event(pc,r,sr,pr,ef,ef.fpul,special['macl']))
    def service(t,mem,r,sr,pr,f):
        if t==DESTRUCTOR:
            assert r[4]==TASK
            if c.get('switch_attach'):store(mem,TASK+28,ALT_ATTACH)
            if c.get('relink'):store(mem,TASK,AFTER)
        elif t==UPDATE:
            node=r[4];assert node in (CHILD1,CHILD2)
            if c.get('remove_child'):
                store(mem,TASK+12,load(mem,node))
                if load(mem,node):store(mem,load(mem,node)+4,0)
                store(mem,node+8,0)
            if c.get('mutate_child_next'):store(mem,node,0)
        elif t==RELEASE1:
            assert r[4]==PAYLOAD[0];store(mem,PAYLOAD[0]+56,0)
            if c.get('swap_work'):store(mem,TASK+32,PAYLOAD[4])
        else:assert t==RELEASE2 and r[4] in (PAYLOAD[0],PAYLOAD[4]);store(mem,r[4]+60,0)
        for i in range(8):r[i]=0xA0000000+i*0x1111+(t&0xFFFF)
        sr[0]^=1
        if c.get('flip'):f.fpscr^=0x380000
    def hook_for(t):
        def hook(mem,r,sr):
            nonlocal wanted
            ee.append(event(t,r,sr,expected.pr,ef,ef.fpul,special['macl']));calls.append(t)
            status=-2 if c.get('null_external') else -17 if c.get('fail_target')==t else 0
            s=[sr];p=[expected.pr]
            if status!=-2:service(t,mem,r,s,p,ef)
            if status:
                expected.r[:]=r;expected.sr=s[0];expected.pr=p[0];expected.pc=t;wanted=status;raise Stopped()
            return s[0]
        return hook
    try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,hooks={t:hook_for(t) for t in EXTERNAL},instruction_budget=5000)
    except Stopped:pass
    actual=State.from_buffer_copy(initial);af=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr);fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234);ae=[];ac=[]
    @Call
    def call(_ctx,t,mem,f,s,r,p):
        try:
            assert t in EXTERNAL;ac.append(t);service(t,mem,r,s,p,f.contents)
            return -17 if c.get('fail_target')==t else 0
        except Exception as exc:errors.append(repr(exc));return -99
    @OBSERVE
    def observe(_ctx,t,mem,f,s,r,p):
        try:ae.append(event(t,r[:16],s[0],p[0],f.contents,fpul.value,macl.value))
        except Exception as exc:errors.append(repr(exc))
    result=FUNCTIONS[c['entry']]((ctypes.c_uint8*base.previous.RAM_SIZE).from_buffer(m),ctypes.byref(af),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,
        Call() if c.get('null_external') else call,observe)
    assert result==wanted and not errors,(n,c,result,wanted,errors)
    assert m==em and list(actual.r)==list(expected.r),(n,c,'RAM/GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(n,c,'SR/PR differs')
    assert bytes(af)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and macl.value==special['macl'],(n,c,'FPU/FPUL/MACL differs')
    assert ae==ee and ac==([] if c.get('null_external') else calls),(n,c,'boundary differs')
    independent=False
    if wanted==0:
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr and list(actual.r)[8:15]==list(initial.r)[8:15]
        if c['entry']=='dispose':
            child_count=c.get('children',0)
            preceding=CHILD2 if child_count>1 else CHILD1 if child_count else base.NEXTTASK if c.get('free_head',True) else 0
            assert load(m,base.FREE_TASK)==TASK and load(m,TASK)==preceding,(n,c,'free task chain')
            assert all(load(m,TASK+a)==0 for a in (4,8,12,16,20,24,28,32,36,40,44))
            if c.get('free_head',True):assert load(m,base.NEXTTASK+4)==(CHILD1 if child_count else TASK)
            if c.get('attachment'):
                at=ALT_ATTACH if c.get('switch_attach') else ATTACH
                assert load(m,at)==(0x80FE5678 if c.get('switch_attach') else 0xA5FE1234)
        elif c['entry']=='work':assert load(m,TASK+32)==0
        elif c['entry']=='children' and (not c.get('special') or c.get('remove_child') or not c.get('children')):
            assert load(m,TASK+12)==0
            if c.get('special'):assert load(m,TASK+16)==0x8C09865C
        # Verify the free chain is acyclic, back-linked and physically bounded.
        at=load(m,base.FREE_HEAP);last=0;free_blocks=[]
        while at:
            assert at not in [a for a,_ in free_blocks] and base.ARENA<=at<=base.ARENA+512
            assert load(m,at+4)==last
            end=load(m,at+8)
            if end:assert at<end<=base.ARENA+512
            free_blocks.append((at,end));last,at=at,load(m,at)
        if c['entry'] in ('dispose','work','heap'):
            released=([PAYLOAD[4] if c.get('swap_work') else PAYLOAD[0]] if c['entry']=='work' else
                      [] if c['entry']=='heap' and c.get('heap_null') else [PAYLOAD[c.get('heap_block',0)]] if c['entry']=='heap' else
                      [PAYLOAD[i] for i in range(4) if c.get('mask',0)&(1<<i)])
            for pointer in released:assert any(a<pointer<end for a,end in free_blocks if end),(n,c,'owned allocation not released')
        independent=True
    return path,len(ae),wanted,independent,calls


def main():
    inventory_path=PROJECT/'analysis/main_dispatch_task_disposal_inventory.json';inventory=json.loads(inventory_path.read_text())
    for p,h in inventory['sha256'].items():assert digest(Path(p))==h,p
    allowed=set().union(*({int(a,16) for a in r['reachable_addresses']} for r in inventory['routines']))
    tracked={int(r['address'],16) for r in inventory['routines']};rows=set();edges=set();counts=dict.fromkeys(FUNCTIONS,0);records=[]
    boundaries=partials=invariants=closed=0;failures={}
    def check(c,n):
        nonlocal boundaries,partials,invariants,closed
        path,b,status,inv,external=run(c,n,allowed,tracked);rows.update(path);counts[c['entry']]+=1
        boundaries+=b;partials+=status!=0;invariants+=inv;closed+=status==0 and not external
        for i,a in enumerate(path):
            w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
            if w>>8 in (0x89,0x8B,0x8D,0x8F):
                nxt=i+(2 if w>>8 in (0x8D,0x8F) else 1)
                if nxt<len(path):edges.add((a,path[nxt]))
        if status==0:
            for t in external:failures.setdefault(t,c)
        records.append({'input':c,'steps':len(path),'boundary_comparisons':b,'status':status,'independent_invariant':inv,'external_calls':[hex(t) for t in external],'full_state_match':True})
    for n,c in enumerate(cases()):
        check(c,n)
        if (n+1)%200==0:print(n+1,'standalone cleanup comparisons passed',flush=True)
        if (n+1)%25==0:gc.collect()
    for t,c in sorted(failures.items()):check(dict(c,fail_target=t),len(records))
    per_routine=[]
    for r in inventory['routines']:
        expected={int(a,16) for a in r['reachable_addresses']};expected_edges={tuple(int(a,16) for a in e) for e in r['conditional_edges']}
        if r['address']!='0x8c098b42':assert expected<=rows,(r['name'],'missing rows',[hex(a) for a in expected-rows])
        per_routine.append({'address':r['address'],'source':r['source'],'rows_covered':len(rows&expected),'indexed_rows':len(expected),
            'conditional_edges_covered':len(edges&expected_edges),'static_conditional_edges':len(expected_edges),
            'uncovered_rows':[hex(a) for a in sorted(expected-rows)],'uncovered_edges':[[hex(a),hex(b)] for a,b in sorted(expected_edges-edges)]})
    files=[Path(__file__),inventory_path,PROJECT/'source/sa_main_dispatch_notification_cleanup_connected.c',PROJECT/'build_main_dispatch_notification_cleanup.ps1',
           Path(base.__file__),PROJECT/'verify_main_dispatch_prefix_gate.py',PROJECT/'verify_startup.py']
    report={'cases_passed':len(records),'counts':counts,'routines':per_routine,'boundary_comparisons':boundaries,'partial_service_error_cases':partials,
        'independent_invariants':invariants,'completed_without_external_calls':closed,'service_error_cases':len(failures),
        'sha256':{str(p):digest(p) for p in files},'dll_sha256':digest(DLL),
        'scope':'Five existing integer cleanup bodies revalidated against original bytes, full state and boundaries; recursive child disposal, resource pointer replacement, attachment word, free-list ownership and callback mutations.',
        'limitations':['Arbitrary destructor/child callbacks and two work-resource releasers are service fixtures. Invalid metadata/double-free corruption fail-stop paths are retained but not invoked by valid-ownership tests.',
                      'Original level-array initial empty-range branch is infeasible for its fixed non-wrapping eight-entry literal. Heap coverage gaps are listed explicitly; no all-code completion claim.',
                      'Valid RAM/heap metadata. Hardware, MMIO, concurrency, live gameplay and playable ports remain unverified.'],
        'cases':records}
    (PROJECT/'analysis/main_dispatch_task_disposal_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('cases_passed','counts','routines','boundary_comparisons','partial_service_error_cases','independent_invariants','completed_without_external_calls','service_error_cases')},indent=2))


if __name__=='__main__':main()
