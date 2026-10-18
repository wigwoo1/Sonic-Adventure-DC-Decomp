"""Retail proof for status query, MACL effects and its native parent connection."""
import csv
import ctypes
import hashlib
import itertools
import json
from pathlib import Path

import verify_main_dispatch_shared_helpers as base
from verify_startup import State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
QUERY,ACTIVE,PARENT=0x8C06CEBE,0x8C05E6E8,base.STATUS
ALT_ACTIVE=0x8CF12340
SOURCE='source/sa_main_dispatch_status_query.c'
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_status_query.dll'
LIB=ctypes.CDLL(str(DLL))
direct=LIB.sa_main_dispatch_status_query_8c06cebe
connected=LIB.sa_main_dispatch_status_query_connected
OBSERVE=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,
    ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P)
common=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
direct.argtypes=common;direct.restype=ctypes.c_int
connected.argtypes=common+[OBSERVE];connected.restype=ctypes.c_int
load,store=base.load,base.store


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def configurations():
    for index,kind in itertools.product(range(256),range(3)):
        yield {'entry':'query','index':index,'mode':(index&7)<<19,
               'flags':0x4000 if kind==0 else 0,'mask':1,'table_value':0 if kind==1 else 1,
               'responses':(0x7FFFFFFF if index&1 else 0xFFFFFFFF,0)}
    for mode,flags,pair,value in itertools.product((n<<19 for n in range(8)),(0,0xC000),
            ((0,0),(0,1),(1,2),(0x80000000,0x80000000),(0xFFFFFFFF,0x80000001)),
            (0,1,0x7FFFFFFF,0x80000000,0xFFFFFFFF)):
        yield {'entry':'query','index':0xFF,'mode':mode,'flags':flags,
               'mask':pair[0],'table_value':pair[1],'responses':(value,0)}
    for flags,pair,responses,check,mode in itertools.product((0,2,0x800,0x802,0x4000),
            ((0,1),(1,2),(1,1)),((0,0),(0,1),(1,0),(0xFFFFFFFF,0)),(0,0x200),(n<<19 for n in range(8))):
        yield {'entry':'connected','index':0x80,'mode':mode,'flags':flags,'mask':pair[0],
               'table_value':pair[1],'responses':responses,'check':check,'pointer':(flags+check+(mode>>19))%3}
    for entry,mode,mutation in itertools.product(('query','connected'),(n<<19 for n in range(8)),
            ('mutate','disable_flag','disable_mask','change_index')):
        yield {'entry':entry,'index':7,'mode':mode,'flags':0x800,'mask':1,'table_value':1,
               'responses':(0,1),'check':0,'pointer':2,mutation:True,'flip_modes':True}
    for entry,index in itertools.product(('query','connected'),(0,0x80,0xFF)):
        yield {'entry':entry,'index':index,'mode':0x300000,'flags':0,'mask':1,'table_value':1,
               'responses':(0,1),'relocate':True,'retarget':True,'check':0,'pointer':2}
    for mode in (n<<19 for n in range(8)):
        yield {'entry':'connected','index':1,'mode':mode,'flags':0,'mask':1,'table_value':1,
               'responses':(0,1),'check':0,'pointer':2,'observe':False}
    for entry,fail_after in (('query',1),('connected',1),('connected',2)):
        yield {'entry':entry,'index':0xFF,'mode':0x380000,'flags':0,'mask':1,'table_value':1,
               'responses':(0,0),'check':0,'pointer':2,'mutate':True,'fail_after':fail_after}


def prepare(config,number):
    memory=bytearray(base.baseline)
    store(memory,base.OBJECT,0x7E,1);store(memory,base.OBJECT+4,config['flags'],2)
    store(memory,base.OBJECT+8,config['index'],1)
    store(memory,base.WORK+0x124,0xA5A5,2);store(memory,base.WORK+8,0x1357,2)
    pointer=config.get('pointer',0)
    store(memory,base.WORK+0x6C,base.LINK if pointer else 0)
    store(memory,base.LINK+0x28,base.NODE if pointer==2 else 0)
    for address in (base.WORK+0xB4,base.NODE+0x10):
        for i,value in enumerate((0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00)):store(memory,address+4*i,value)
    mask_address=load(memory,0x8C06CF4C);table=load(memory,0x8C06CF50)
    if config.get('relocate'):
        mask_address,table=0x8CB00000,0x8CB10000
        store(memory,0x8C06CF4C,mask_address);store(memory,0x8C06CF50,table)
    store(memory,mask_address,config['mask'])
    store(memory,table+0x34*config['index'],config['table_value'])
    store(memory,table+0x34*((config['index']+1)&255),1)
    target=ALT_ACTIVE if config.get('retarget') else ACTIVE
    store(memory,0x8C06CF54,target)
    initial=State();initial.r[:]=[(0x12345678+number*0x100+i*0x10101)&0xFFFFFFFF for i in range(16)]
    initial.r[4]=base.OBJECT;initial.r[5]=base.WORK if config['entry']=='query' else base.AUX
    initial.r[6]=base.WORK;initial.r[15]=0x8CF00000-(number%4)*0x100
    initial.pc=QUERY if config['entry']=='query' else PARENT;initial.pr=0x8C000500;initial.sr=0x70000060|(number&1)
    fpu=Fpu();fpu.fpscr=config['mode']|0x40000;fpu.fpul=0xCAFEBABE
    for bank in range(2):fpu.banks[bank][:]=[0xBC000000+bank*0x10000+i*0x100 for i in range(16)]
    return memory,initial,fpu,target


def service(config,target,memory,regs,sr,pr,fpu,counters):
    assert target!=QUERY,'Native query escaped the integration'
    if target in (ACTIVE,ALT_ACTIVE):
        index=counters.get('active',0);counters['active']=index+1
        result=config['responses'][index]
        store(memory,0x8CAE6000+index*4,0xD0000000|index)
        if config.get('mutate'):
            for i in range(8):regs[i]=(0x12340000+i*0x10101)&0xFFFFFFFF
            sr[0]^=1
            for bank in range(2):
                for i in range(8):fpu.banks[bank][i]=0xDE000000+bank*0x10000+i*0x100
        if config.get('disable_flag'):store(memory,base.OBJECT+4,load(memory,base.OBJECT+4,2)|0x4000,2)
        if config.get('disable_mask'):store(memory,load(memory,0x8C06CF4C),0)
        if config.get('change_index'):store(memory,base.OBJECT+8,(config['index']+1)&255,1)
        if config.get('flip_modes'):fpu.fpscr^=0x380000
        regs[0]=result
    else:
        callback_config=dict(config,character=load(memory,base.OBJECT+8,1))
        base.apply_service(callback_config,target,memory,regs,sr,pr,fpu,counters)


def run_case(config,number,addresses):
    memory,initial,fpu,target=prepare(config,number)
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial);expected_fpu=Fpu.from_buffer_copy(fpu)
    special={'macl':0xFEED1234};expected_events=[];path=[];expected_counters={}
    def event(address,mem,regs,sr,pr,f):
        return (address,tuple(regs),sr,pr,bytes(f),load(mem,base.OBJECT+4,2),load(mem,base.OBJECT+8,1))
    def trace(pc,word,regs,sr,pr,_f):
        assert pc in addresses,hex(pc)
        path.append(pc)
        if pc==QUERY and pc!=initial.pc:
            expected_events.append(event(QUERY,expected_memory,regs,sr,pr,expected_fpu))
    def hook_for(address):
        def hook(mem,regs,sr):
            expected_events.append(event(address,mem,regs,sr,expected.pr,expected_fpu))
            status=[sr];pr_value=[expected.pr]
            service(config,address,mem,regs,status,pr_value,expected_fpu,expected_counters)
            return status[0]
        return hook
    externals={target,base.CHECK,base.DETACH,base.RESET,base.VECTOR,base.SOUND}
    if config.get('fail_after'):
        # Stop the retail executor at each active service boundary so MACL
        # flushes to the explicit special-register state, even on host error.
        steps=0
        for ordinal in range(1,config['fail_after']+1):
            steps+=reference(expected_memory,expected,
                hooks={a:hook_for(a) for a in externals if a!=target},
                fpu=expected_fpu,special=special,stop_pc=target,trace=trace,instruction_budget=2000)
            expected_events.append(event(target,expected_memory,list(expected.r),expected.sr,expected.pr,expected_fpu))
            regs=list(expected.r);status=[expected.sr];pr_value=[expected.pr]
            service(config,target,expected_memory,regs,status,pr_value,expected_fpu,expected_counters)
            expected.r[:]=regs;expected.sr=status[0]
            if ordinal!=config['fail_after']:expected.pc=expected.pr
    else:
        steps=reference(expected_memory,expected,hooks={a:hook_for(a) for a in externals},
            fpu=expected_fpu,special=special,trace=trace,instruction_budget=2000)
    actual=State.from_buffer_copy(initial);actual_fpu=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xFEED1234)
    actual_events=[];errors=[];actual_counters={}
    @OBSERVE
    def observe(_ctx,address,mem,f,s,r,p):
        try:actual_events.append(event(address,mem,list(r[:16]),s[0],p[0],f.contents))
        except Exception as exc:errors.append(repr(exc))
    @Call
    def call(_ctx,address,mem,f,s,r,p):
        try:
            if config['entry']=='query':actual_events.append(event(address,mem,list(r[:16]),s[0],p[0],f.contents))
            service(config,address,mem,r,s,p,f.contents,actual_counters)
            return -17 if config.get('fail_after')==actual_counters.get('active',0) and address==target else 0
        except Exception as exc:errors.append(repr(exc));return -99
    view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
    args=(view,ctypes.byref(actual_fpu),ctypes.byref(sr),actual.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    if config['entry']=='query':result=direct(*args)
    else:result=connected(*args,observe if config.get('observe',True) else OBSERVE())
    assert result==(-17 if config.get('fail_after') else 0) and not errors,(number,config,result,errors)
    assert memory==expected_memory,(number,config,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,config,'GPR differs')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
    assert bytes(actual_fpu)==bytes(expected_fpu),(number,config,'FPU differs')
    assert fpul.value==fpu.fpul and macl.value==special['macl'],(number,config,'FPUL/MACL differs')
    assert actual_counters==expected_counters
    if config.get('observe',True):assert actual_events==expected_events,(number,config,'boundary differs')
    else:assert not actual_events
    if not config.get('fail_after'):
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        if config['entry']=='query':
            active=not config['flags']&0x4000 and bool(config['mask']&config['table_value'])
            value=config['responses'][0];positive=0<value<0x80000000
            assert actual.r[0]==(2 if positive else 1) if active else actual.r[0]==0
            assert macl.value==(0xFEED1234 if config['flags']&0x4000 else config['index']*0x34)
    return path,len(actual_events),steps,actual_counters


def main():
    listing=PROJECT/'analysis/main-dispatch-status-query/8c06cebe_instructions.tsv'
    words={}
    for row in csv.DictReader(listing.open(),delimiter='\t'):
        a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
        assert len(raw)==int(row['length'])==2
        assert base.binary[a-0x8C010000:a-0x8C010000+2]==raw
        words[a]=int.from_bytes(raw,'little')
    assert len(words)==32 and set(words)==set(range(QUERY,0x8C06CEFE,2))
    wanted_edges={(a,b) for a,w in words.items() if w>>8 in (0x89,0x8B)
                  for b in (a+2,a+4+2*((w&255)-256 if w&128 else w&255))}
    assert len(wanted_edges)==6 and words[0x8C06CED2]==0x203E
    manifest_path=PROJECT/'analysis/main_dispatch_shared_translation.json'
    manifest=json.loads(manifest_path.read_text());parent=next(r for r in manifest['routines'] if int(r['address'],16)==PARENT)
    parent_addresses={int(a,16) for a in parent['reachable_addresses']}
    allowed=set(words)|parent_addresses
    coverage={'query':set(),'connected':set()};edges={'query':set(),'connected':set()}
    counts={'query':0,'connected':0};records=[];boundaries=0;errors=0;unobserved=0;parent_seen=set()
    for number,config in enumerate(configurations()):
        path,b,steps,counters=run_case(config,number,allowed)
        kind=config['entry'];counts[kind]+=1;boundaries+=b
        coverage[kind].update(set(path)&set(words))
        if kind=='connected':parent_seen.update(set(path)&parent_addresses)
        for a,c in zip(path,path[1:]):
            if a in words and words[a]>>8 in (0x89,0x8B):edges[kind].add((a,c))
        errors+=bool(config.get('fail_after'));unobserved+=not config.get('observe',True)
        records.append({'input':config,'steps':steps,'boundaries':b,'active_service_calls':counters.get('active',0),'full_state_and_macl_match':True})
        if (number+1)%500==0:print(number+1,'status-query/native caller comparisons passed',flush=True)
    for kind in coverage:
        assert coverage[kind]==set(words),(kind,'unvisited query rows',set(words)-coverage[kind])
        assert edges[kind]==wanted_edges,(kind,'unvisited query edges',wanted_edges-edges[kind])
    assert parent_seen==parent_addresses,('unvisited parent rows',parent_addresses-parent_seen)
    guards=0
    for kind in ('query','connected'):
        config={'entry':kind,'index':1,'mode':0x380000,'flags':0,'mask':1,'table_value':1,'responses':(0,0)}
        memory,state,fpu,_=prepare(config,0);view=(ctypes.c_uint8*base.RAM_SIZE).from_buffer(memory)
        values=[ctypes.c_uint32(v) for v in (state.sr,state.pr,fpu.fpul,0xFEED1234)]
        before=(bytes(memory),bytes(state.r),bytes(fpu),[v.value for v in values])
        args=(view,ctypes.byref(fpu),ctypes.byref(values[0]),state.r,ctypes.byref(values[1]),ctypes.byref(values[2]),ctypes.byref(values[3]),None,Call())
        assert (direct(*args) if kind=='query' else connected(*args,OBSERVE()))==-2
        assert before==(bytes(memory),bytes(state.r),bytes(fpu),[v.value for v in values]);guards+=1
    files=[Path(__file__),PROJECT/'verify_main_dispatch_shared_helpers.py',PROJECT/'verify_startup.py',
           PROJECT/'source/sa_main_dispatch_shared_helpers.c',PROJECT/'build_main_dispatch_status_query.ps1',
           manifest_path,listing,PROJECT/'analysis/main-dispatch-status-query/diagnostics.tsv']
    report={'address':hex(QUERY),'end_exclusive':'0x8c06cefe','indexed_instruction_bytes':64,
            'source':SOURCE,'source_sha256':digest(PROJECT/SOURCE),'binary_sha256':digest(base.BINARY),
            'retail_span_sha256':hashlib.sha256(base.binary[QUERY-0x8C010000:0x8C06CEFE-0x8C010000]).hexdigest(),
            'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'cases_passed':len(records),'counts':counts,'boundary_comparisons':boundaries,
            'service_error_cases_passed':errors,'atomic_guards_passed':guards,'unobserved_cases_passed':unobserved,
            'covered_query_rows_by_entry':{k:len(v) for k,v in coverage.items()},
            'covered_query_conditional_edges_by_entry':{k:len(v) for k,v in edges.items()},
            'covered_parent_rows':len(parent_seen),
            'comparison_level':'Complete 16 MiB RAM, GPR/SR/PR/FPU and explicit MACL/FPUL compared with original SH4 execution, including ordered native-query/external-service entry states.',
            'scope':'32-instruction query directly and inside the 153-instruction native parent; flag/mask/stride/index handling, signed active-service results, real independent query calls, callback changes between calls, FR/SZ/PR modes, relocated literals, all byte-index encodings in constructed entries, MACL side effects and partial error state.',
            'limitations':['Active service 0x8c05e6e8 and other parent callees remain deterministic test services, not recovered native implementations.',
                           'Constructed entries for every index byte do not prove every index valid in actual gameplay.',
                           'The SH4 subset is not physical hardware; invalid pointers, memory exceptions/MMIO, full dispatcher/game behavior and ports remain unverified.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_status_query_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(records)} cases {counts}; 32 query rows / 6 edges at both entries; 153 parent rows; {boundaries} boundaries; {errors} service errors; {guards} atomic guards; {unobserved} unobserved cases; MACL compared')


if __name__=='__main__':main()
