"""Compare original reset/clear and the extended native STATUS connection."""
import csv,ctypes,hashlib,itertools,json
from pathlib import Path

import verify_main_dispatch_active_query as active
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
base=active.base
RESET,CLEAR,ACTIVE,QUERY,CHECK,STATUS=0x8C05E788,active.CLEAR,active.ACTIVE,active.QUERY,active.CHECK,active.STATUS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_reset.dll'
LIB=ctypes.CDLL(str(DLL))
reset=LIB.sa_main_dispatch_reset_8c05e788
connected=LIB.sa_main_dispatch_status_reset_connected
reset.argtypes=active.pure_args;reset.restype=ctypes.c_int
connected.argtypes=active.pure_args[:-1]+[Call,active.OBSERVE];connected.restype=ctypes.c_int
load,store=base.load,base.store
FINAL_NODE,FINAL_FLAG,CANDIDATE_NODE,CANDIDATE_FLAG=0x8CE00000,0x8CE01000,0x8CE02000,0x8CE03000
TRACKED={RESET,CLEAR,ACTIVE,QUERY,CHECK,active.PREDICATE}


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def signed16(value):
    value&=0xFFFF
    return value-0x10000 if value&0x8000 else value


def cases():
    for current,present,mode,mask in itertools.product(
            (-32768,-32767,-256,-2,-1,0,1,8,9,10,255,256,32766,32767),
            (False,True),range(8),(1,0x155,0x3FF)):
        if current==-1 and not present:continue # Table zero must be a valid object.
        yield {'entry':'reset','current':current,'present':present,'mode':mode<<19,'pool_mask':mask}
    for current,present,flags,mode,rm in itertools.product((1,8,9,32767,-1),
            (False,True),(0,0x800),(0,0x100000,0x200000,0x300000),(0,1)):
        if current==-1 and not present:continue
        yield {'entry':'status','current':current,'present':present,'flags':flags,'mode':mode,'rm':rm}
    for current,present,mode in itertools.product((0,8,9,32767),(False,True),range(8)):
        yield {'entry':'reset','current':current,'present':present,'mode':mode<<19,'relocate_table':True}
    for entry,alias,mode in itertools.product(('reset','status'),('count','chain','selected_flag'),
            (0,0x100000,0x200000,0x300000)):
        yield {'entry':entry,'current':9 if alias!='selected_flag' else 1,
               'present':True,'mode':mode,'alias':alias}
    for entry,current,mode in itertools.product(('reset','status'),(1,9,32767),
            (0,0x100000,0x200000,0x300000)):
        yield {'entry':entry,'current':current,'present':True,'mode':mode,'observe':False}
    for failure,mode in itertools.product(('missing','service_error'),(0,0x100000,0x200000,0x300000)):
        yield {'entry':'status','current':1,'present':True,'mode':mode,'failure':failure}


def prepare(config,number):
    fixture=dict(config,entry='status' if config['entry']=='status' else 'active',index=0,type=8,
                 count=config['current']&0xFFFF,method=1,value=29)
    memory,state,fpu,clear_table=active.prepare(fixture,number)
    state.pc=RESET if config['entry']=='reset' else STATUS
    store(memory,active.INFO,0xA5,1)
    table=load(memory,0x8C05E840)
    if config.get('relocate_table'):
        table=0x8CE80000;store(memory,0x8C05E840,table)
    effective=config['current']
    if config.get('alias')=='count':
        # Clear writes the current count before reset reads it: 9 -> 0.
        store(memory,active.POOL_NODES+0x20,active.INFO+6)
        effective=0
    if config.get('alias')=='chain':
        # Clear zeroes the low byte of TASK+20 (INFO+80 -> INFO), so reset
        # must reload the chain after clearing rather than caching INFO+80.
        store(memory,active.TASK+0x20,active.INFO+0x80)
        for offset,value,width in ((0,0xA5,1),(2,8,1),(6,9,2),(0xC,29,4)):
            store(memory,active.INFO+0x80+offset,value,width)
        store(memory,active.INFO+6,2,2)
        store(memory,active.POOL_NODES+0x20,active.TASK+0x20)
        effective=2
    store(memory,table,active.POOL_NODES) # Retail assumes a nonnull first entry.
    store(memory,FINAL_NODE+0x20,FINAL_FLAG);store(memory,FINAL_FLAG,0xA5,1)
    store(memory,CANDIDATE_NODE+0x20,CANDIDATE_FLAG);store(memory,CANDIDATE_FLAG,0xA5,1)
    candidate=effective+1
    selected=0 if effective==9 or not config['present'] else signed16(candidate)
    if effective!=9:
        store(memory,(table+candidate*4)&0xFFFFFFFF,CANDIDATE_NODE if config['present'] else 0)
    if selected:
        store(memory,(table+selected*4)&0xFFFFFFFF,FINAL_NODE)
        selected_flag=FINAL_FLAG
    else:selected_flag=active.POOL_FLAGS
    # When candidate and selected slot coincide, the final pointer is the
    # same object. At +32768/-32768 the two slots remain deliberately distinct.
    if effective==-1:store(memory,table,active.POOL_NODES)
    if config.get('alias')=='selected_flag':
        node=load(memory,(table+selected*4)&0xFFFFFFFF)
        store(memory,node+0x20,active.INFO);selected_flag=active.INFO
    return memory,state,fpu,{'table':table,'selected':selected,'flag':selected_flag,'effective':effective}


def run_case(config,number,allowed):
    memory,initial,fpu,meta=prepare(config,number)
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial);expected_fpu=Fpu.from_buffer_copy(fpu)
    expected_events=[];expected_counters={};special={'macl':0xFEED1234};path=[]
    def event(target,mem,regs,sr,pr,f,fpul_value=None):
        shadow=Fpu.from_buffer_copy(f)
        if fpul_value is not None:shadow.fpul=fpul_value
        return (target,tuple(regs),sr,pr,bytes(shadow),load(mem,active.INFO+6,2))
    def trace(pc,word,regs,sr,pr,_f):
        assert pc in allowed,hex(pc)
        path.append(pc)
        if pc in TRACKED and pc!=initial.pc:expected_events.append(event(pc,expected_memory,regs,sr,pr,expected_fpu))
    def apply_service(target,mem,regs,sr,pr,fpu,counters):
        assert target not in TRACKED
        callback_config=dict(config,character=load(mem,base.OBJECT+8,1))
        base.apply_service(callback_config,target,mem,regs,sr,pr,fpu,counters)
        counters['external']=counters.get('external',0)+1
    def hook_for(target):
        def hook(mem,regs,sr):
            expected_events.append(event(target,mem,regs,sr,expected.pr,expected_fpu))
            status=[sr];pr_value=[expected.pr]
            apply_service(target,mem,regs,status,pr_value,expected_fpu,expected_counters)
            return status[0]
        return hook
    externals={base.DETACH,base.VECTOR,base.SOUND}
    if config.get('failure'):
        steps=reference(expected_memory,expected,fpu=expected_fpu,special=special,
            trace=trace,stop_pc=base.SOUND,instruction_budget=3000)
        expected_events.append(event(base.SOUND,expected_memory,list(expected.r),expected.sr,expected.pr,expected_fpu))
        if config['failure']=='service_error':
            regs=list(expected.r);status=[expected.sr];pr_value=[expected.pr]
            apply_service(base.SOUND,expected_memory,regs,status,pr_value,expected_fpu,expected_counters)
            expected.r[:]=regs;expected.sr=status[0]
    else:
        steps=reference(expected_memory,expected,fpu=expected_fpu,special=special,
            hooks={t:hook_for(t) for t in externals},trace=trace,instruction_budget=3000)
    actual=State.from_buffer_copy(initial);actual_fpu=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    events=[];errors=[];actual_counters={}
    @active.OBSERVE
    def observe(_ctx,target,mem,f,s,r,p):
        try:events.append(event(target,mem,list(r[:16]),s[0],p[0],f.contents,fpul.value))
        except Exception as exc:errors.append(repr(exc))
    @Call
    def call(_ctx,target,mem,f,s,r,p):
        try:
            apply_service(target,mem,r,s,p,f.contents,actual_counters)
            return -17 if config.get('failure')=='service_error' else 0
        except Exception as exc:errors.append(repr(exc));return -99
    observer=observe if config.get('observe',True) else active.OBSERVE()
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    args=(view,ctypes.byref(actual_fpu),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None)
    result=reset(*args,observer) if config['entry']=='reset' else connected(*args,Call() if config.get('failure')=='missing' else call,observer)
    wanted=-2 if config.get('failure')=='missing' else -17 if config.get('failure') else 0
    assert result==wanted and not errors,(number,config,result,errors)
    assert memory==expected_memory,(number,config,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,config,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
    assert bytes(actual_fpu)[:132]==bytes(expected_fpu)[:132]
    assert fpul.value==expected_fpu.fpul and macl.value==special['macl']
    assert actual_counters==expected_counters
    if config.get('observe',True):assert events==expected_events,(number,config,'boundary differs')
    else:assert not events
    assert load(memory,meta['flag'],1)==2,(number,config,'selected flag not set')
    assert load(memory,active.INFO,1)==(2 if config.get('alias')=='selected_flag' else 1)
    assert load(memory,active.INFO+6,2)==meta['effective']&0xFFFF
    if not config.get('failure'):
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        if config['entry']=='status':
            assert actual.r[0]==1 and load(memory,base.OBJECT,1)==0x28
            assert load(memory,base.WORK+0x124,2)==0x49
    return path,len(events),steps,actual_counters.get('external',0)


def main():
    listing=PROJECT/'analysis/main-dispatch-reset/8c05e788_instructions.tsv'
    words={}
    for row in csv.DictReader(listing.open(),delimiter='\t'):
        a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
        assert len(raw)==int(row['length'])==2 and base.binary[a-0x8C010000:a-0x8C010000+2]==raw
        words[a]=int.from_bytes(raw,'little')
    assert len(words)==38 and set(words)==set(range(RESET,0x8C05E7D4,2))
    seen=set();todo=[RESET];wanted_edges=set();call_sites={}
    while todo:
        a=todo.pop()
        if a in seen:continue
        assert a in words
        seen.add(a);w=words[a]
        if w==0xB or w>>12 in (0xA,0xB):
            assert a+2 in words;seen.add(a+2)
            if w==0xB:continue
            d=w&4095;d-=4096 if d&2048 else 0;target=a+4+2*d
            if w>>12==0xB:assert target==CLEAR;call_sites[a]=target;todo.append(a+4)
            else:todo.append(target)
        elif w>>8 in (0x89,0x8B,0x8D):
            d=w&255;d-=256 if d&128 else 0;fall=a+4 if w>>8==0x8D else a+2
            if fall==a+4:seen.add(a+2)
            for t in (a+4+2*d,fall):todo.append(t);wanted_edges.add((a,t))
        else:todo.append(a+2)
    assert seen==set(words) and len(wanted_edges)==4 and len(call_sites)==1
    active_manifest_path=PROJECT/'analysis/main_dispatch_active_translation.json'
    group=json.loads(active_manifest_path.read_text())
    active_rows={int(a,16) for r in group['routines'] for a in r['reachable_addresses']}
    parent_manifest_path=PROJECT/'analysis/main_dispatch_shared_translation.json'
    parent=next(r for r in json.loads(parent_manifest_path.read_text())['routines'] if int(r['address'],16)==STATUS)
    parent_rows={int(a,16) for a in parent['reachable_addresses']}
    check_manifest_path=PROJECT/'analysis/main_dispatch_shared_check_translation.json'
    check_rows={int(a,16) for a in json.loads(check_manifest_path.read_text())['reachable_addresses']}
    allowed=set(words)|active_rows|parent_rows|check_rows|set(range(QUERY,0x8C06CEFE,2))
    coverage={'reset':set(),'status':set()};edges={'reset':set(),'status':set()}
    counts={'reset':0,'status':0};records=[];boundaries=0;unobserved=0;failures=0;caller_resets=0
    for number,config in enumerate(cases()):
        path,b,steps,external_calls=run_case(config,number,allowed)
        kind=config['entry'];coverage[kind].update(set(path)&set(words));counts[kind]+=1;boundaries+=b
        for i,a in enumerate(path):
            if a in words and words[a]>>8 in (0x89,0x8B,0x8D):
                j=i+(2 if words[a]>>8==0x8D else 1)
                if j<len(path):edges[kind].add((a,path[j]))
        unobserved+=not config.get('observe',True);failures+=bool(config.get('failure'))
        caller_resets+=kind=='status' and RESET in path
        records.append({'input':config,'steps':steps,'boundaries':b,'external_calls':external_calls,'full_state_match':True})
        if (number+1)%250==0:print(number+1,'native reset/caller comparisons passed',flush=True)
    for kind in coverage:
        assert coverage[kind]==set(words),(kind,'uncovered reset rows',set(words)-coverage[kind])
        assert edges[kind]==wanted_edges,(kind,'uncovered reset edges',wanted_edges-edges[kind])
    files=[Path(__file__),PROJECT/'verify_main_dispatch_active_query.py',PROJECT/'verify_main_dispatch_shared_helpers.py',
           PROJECT/'verify_startup.py',listing,PROJECT/'analysis/main-dispatch-reset/diagnostics.tsv',
           active_manifest_path,parent_manifest_path,check_manifest_path,PROJECT/'build_main_dispatch_reset.ps1',
           PROJECT/'source/sa_main_dispatch_reset_connected.c',PROJECT/'source/sa_main_dispatch_active_query.c',
           PROJECT/'source/sa_main_dispatch_status_query.c',PROJECT/'source/sa_main_dispatch_shared_check.c',
           PROJECT/'source/sa_main_dispatch_shared_helpers.c',PROJECT/'source/sh4_float_i32.c',PROJECT/'source/sh4_compare_bits.h',PROJECT/'sh4_binary32.py']
    source='source/sa_main_dispatch_reset.c'
    report={'address':hex(RESET),'end_exclusive':'0x8c05e7d4','indexed_instruction_bytes':76,
            'source':source,'source_sha256':digest(PROJECT/source),'binary_sha256':digest(base.BINARY),
            'retail_span_sha256':hashlib.sha256(base.binary[RESET-0x8C010000:0x8C05E7D4-0x8C010000]).hexdigest(),
            'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'cases_passed':len(records),'counts':counts,'boundary_comparisons':boundaries,
            'reset_rows_covered_by_entry':{k:len(v) for k,v in coverage.items()},
            'reset_conditional_edges_by_entry':{k:len(v) for k,v in edges.items()},
            'native_clear_call_sites_verified':1,'connected_native_reset_calls':caller_resets,
            'unobserved_cases_passed':unobserved,'post_reset_partial_error_cases_passed':failures,
            'comparison_level':'Full 16 MiB RAM, GPR/SR/PR, FPU banks/FPSCR, separate FPUL and MACL against original SH4 instructions; ordered native/reset/clear and remaining-service boundary states.',
            'scope':'38-row reset with native clear directly and through STATUS; index-nine fallback, absent candidates, signed 16-bit rollover, distinct candidate/final slots, relocated tables, clear-induced chain/count changes, alias writes, unobserved execution and partial state after reset.',
            'limitations':['Constructed extra table entries test instruction semantics, not validity of all signed indexes in the game.',
                           'The first selected table entry and original object pointers must satisfy retail nonnull/mapping preconditions.',
                           'Detach/vector/sound remain external fixtures in STATUS; full dispatcher/game/ports remain unfinished.',
                           'Physical hardware, arbitrary invalid pointers and memory exceptions/MMIO are not verified.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_reset_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(records)} cases {counts}; 38 reset rows / 4 edges at both entries; {boundaries} boundaries; {caller_resets} native caller resets; {unobserved} unobserved cases; {failures} post-reset error cases')


if __name__=='__main__':main()
