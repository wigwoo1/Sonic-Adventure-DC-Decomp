"""Compare shared helper control flow, FMOV and call boundaries with retail."""
import ctypes
import hashlib
import itertools
import json
import struct
from pathlib import Path

from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent
STATUS,FLAG=0x8C0AFC98,0x8C0B11C0
OBJECT,WORK,AUX,LINK,NODE=0x8CA00000,0x8CA10000,0x8CA20000,0x8CA40000,0x8CA50000
QUERY,CHECK,DETACH,RESET,VECTOR,SOUND=0x8C06CEBE,0x8C06C880,0x8C0B1D60,0x8C05E788,0x8C07CB54,0x8C0940DA
SERVICES={QUERY,CHECK,DETACH,RESET,VECTOR,SOUND}
RAM_SIZE=0x1000000
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
DLL=ROOT/'work/sonic-adventure/build/main_dispatch_shared_helpers.dll'
binary=BINARY.read_bytes();baseline=bytearray(RAM_SIZE);baseline[0x10000:0x10000+len(binary)]=binary
library=ctypes.CDLL(str(DLL))
functions={STATUS:library.sa_main_dispatch_status_transition_8c0afc98,
           FLAG:library.sa_main_dispatch_flag_transition_8c0b11c0}
for fn in functions.values():
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
    fn.restype=ctypes.c_int


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def load(memory,address,width=4):
    return sum(int(memory[(address&0xFFFFFF)+i])<<(8*i) for i in range(width))


def store(memory,address,value,width=4):
    for i in range(width):memory[(address&0xFFFFFF)+i]=(value>>(8*i))&255


def configurations():
    modes=[n<<19 for n in range(8)]
    for responses,flags,check,pointer,mode in itertools.product(
            ((0,0),(1,0),(0,1),(1,2),(2,0),(0xFFFFFFFF,1)),
            (0,2,0x800,0x802,0x900,0xFFFF,0x8000),(0,0x200),range(3),modes):
        yield {'entry':STATUS,'responses':responses,'flags':flags,'check':check,'pointer':pointer,'mode':mode}
    for flags,check,mode in itertools.product((0,1,2,3,0x100,0x101,0x800,0x801,0x900,0xFFFF,0x8000),(0,0x200),modes):
        yield {'entry':FLAG,'flags':flags,'check':check,'mode':mode}
    for entry,mode,flags in itertools.product((STATUS,FLAG),modes,(0x800,0x802,0xFFFF)):
        yield {'entry':entry,'mode':mode,'flags':flags,'check':0x200,
               'responses':(1,1),'pointer':2,'mutate':True,'flip_modes':True}
    for mode,responses,character in itertools.product(modes,((2,0),(1,1)),(0,0x80,0xFF)):
        yield {'entry':STATUS,'mode':mode,'flags':0x800,'check':0,
               'responses':responses,'pointer':2,'mutate':True,'character':character}


def apply_service(config,target,memory,regs,sr,pr,fpu,counters):
    assert target in SERVICES,hex(target)
    if target==QUERY:
        assert regs[4]==OBJECT and regs[5]==WORK
        index=counters.get('queries',0);counters['queries']=index+1
        result=config['responses'][index]
    elif target==CHECK:
        assert regs[4]==OBJECT
        result=config['check']
    else:
        result=0xA1B2C3D4
        if target==DETACH:assert regs[4]==OBJECT and regs[5]==AUX and regs[6]==WORK
        if target==RESET:assert regs[4]==OBJECT
        if target==SOUND:assert regs[4]==0x11 and regs[5]==0 and regs[6]==0
        if target==VECTOR:
            character=config.get('character',0x80)
            assert regs[4]==(character-256 if character&128 else character)&0xFFFFFFFF
    store(memory,0x8CAE0000+(target&0xFFF),0xAB000000|(target&0xFFFF))
    if config.get('mutate'):
        for i in range(8):regs[i]=(0x10203040+i*0x10101+target)&0xFFFFFFFF
        sr[0]^=1
        store(memory,OBJECT+4,load(memory,OBJECT+4,2)^0x800,2)
        for bank in range(2):
            for i in range(8):fpu.banks[bank][i]=0xA0000000+bank*0x10000+i*0x100
    if config.get('flip_modes'):fpu.fpscr^=0x380000
    regs[0]=result


class ServiceStopped(Exception):pass


def run_case(config,number,inventory):
    memory=bytearray(baseline)
    store(memory,OBJECT,0x7E,1);store(memory,OBJECT+4,config['flags'],2)
    store(memory,OBJECT+8,config.get('character',0x80),1)
    store(memory,WORK+0x124,0xA5A5,2);store(memory,WORK+8,0x1357,2)
    pointer=config.get('pointer',0)
    store(memory,WORK+0x6C,LINK if pointer else 0)
    store(memory,LINK+0x28,NODE if pointer==2 else 0)
    for address in (WORK+0xB4,NODE+0x10):
        for i,value in enumerate((0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00)):
            store(memory,address+4*i,value)
    initial=State();initial.r[:]=[(0x12345678+number*0x100+i*0x10101)&0xFFFFFFFF for i in range(16)]
    initial.r[4]=OBJECT;initial.r[5]=AUX;initial.r[6]=WORK
    initial.r[15]=0x8CF00000-(number%4)*0x100;initial.pc=config['entry']
    initial.pr=0x8C000500;initial.sr=0x70000060|(number&1)
    fpu=Fpu();fpu.fpscr=config['mode']|0x40000;fpu.fpul=0xCAFEBABE
    for bank in range(2):fpu.banks[bank][:]=[0xBC000000+bank*0x10000+i*0x100 for i in range(16)]
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial);expected_fpu=Fpu.from_buffer_copy(fpu)
    expected_events=[];path=[];expected_counters={}
    def event(target,mem,regs,sr,pr,f):
        return (target,tuple(regs),sr,pr,bytes(f),load(mem,OBJECT+4,2),load(mem,WORK+0x124,2))
    def hook_for(target):
        def hook(mem,regs,sr):
            expected_events.append(event(target,mem,regs,sr,expected.pr,expected_fpu))
            status=[sr];return_address=[expected.pr]
            apply_service(config,target,mem,regs,status,return_address,expected_fpu,expected_counters)
            if expected.pr==config.get('fail_site',0)+4:
                expected.r[:]=regs;expected.sr=status[0]
                raise ServiceStopped()
            return status[0]
        return hook
    def trace(pc,word,regs,sr,pr,fr):
        assert pc in inventory['addresses'],hex(pc)
        path.append(pc)
    stopped=False
    try:
        steps=reference(expected_memory,expected,hooks={t:hook_for(t) for t in SERVICES},
                        fpu=expected_fpu,trace=trace,instruction_budget=2000)
    except ServiceStopped:
        stopped=True;steps=len(path)
    assert stopped==bool(config.get('fail_site'))
    actual=State.from_buffer_copy(initial);actual_fpu=Fpu.from_buffer_copy(fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0xABCDEF01)
    view=(ctypes.c_uint8*RAM_SIZE).from_buffer(memory)
    actual_events=[];errors=[];actual_counters={}
    @Call
    def call(_ctx,target,mem,f,s,r,p):
        try:
            actual_events.append(event(target,mem,list(r[:16]),s[0],p[0],f.contents))
            apply_service(config,target,mem,r,s,p,f.contents,actual_counters)
            return -17 if p[0]==config.get('fail_site',0)+4 else 0
        except Exception as exc:errors.append(repr(exc));return -99
    result=functions[config['entry']](view,ctypes.byref(actual_fpu),ctypes.byref(sr),actual.r,
        ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert result==(-17 if stopped else 0) and not errors,(number,config,result,errors)
    assert memory==expected_memory,(number,config,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,config,'registers differ')
    assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
    assert bytes(actual_fpu)==bytes(expected_fpu),(number,config,'FPU differs')
    assert fpul.value==fpu.fpul and macl.value==0xABCDEF01
    assert actual_events==expected_events and actual_counters==expected_counters,(number,config,'callback boundary differs')
    if not stopped:
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        assert list(actual.r)[8:15]==list(initial.r)[8:15]
        if config['entry']==STATUS:
            first,second=config['responses']
            assert actual_counters['queries']==(1 if first==2 else 2)
            assert actual.r[0]==int(first==2 or second!=0)
            if first==2:assert load(memory,OBJECT,1)==0x28 and load(memory,WORK+0x124,2)==0x49
        else:
            assert actual.r[0]==int(not config['flags']&3)
            if config['flags']&3:assert not actual_events and load(memory,OBJECT,1)==0x7E
            elif config['flags']&0x100:assert not actual_events and load(memory,OBJECT,1)==8
            elif not config.get('mutate'):
                special=bool(config['flags']&0x800 and config['check']&0x200)
                assert load(memory,OBJECT,1)==(0x1C if special else 0xC)
                assert load(memory,WORK+0x124,2)==(0x39 if special else 0x12)
    edges=set()
    for i,pc in enumerate(path):
        word=inventory['words'][pc]
        if word>>8 in (0x89,0x8B,0x8D,0x8F):
            j=i+(2 if word>>8 in (0x8D,0x8F) else 1)
            if j<len(path):edges.add((pc,path[j]))
    executed_calls={event[3]-4 for event in actual_events}
    return set(path),edges,executed_calls,len(actual_events),steps


def main():
    manifest_path=PROJECT/'analysis/main_dispatch_shared_translation.json'
    manifest=json.loads(manifest_path.read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    inventories={}
    for routine in manifest['routines']:
        addresses={int(a,16) for a in routine['reachable_addresses']}
        words={a:int.from_bytes(binary[a-0x8C010000:a-0x8C010000+2],'little') for a in addresses}
        inventories[int(routine['address'],16)]={'addresses':addresses,'words':words,
            'edges':{(int(a,16),int(b,16)) for a,b in routine['conditional_edges']},
            'calls':{int(a,16) for a in routine['external_call_sites']}}
    coverage={a:set() for a in functions};edge_coverage={a:set() for a in functions};call_coverage={a:set() for a in functions}
    failure_configs={};records=[];counts={a:0 for a in functions};boundaries=0
    for number,config in enumerate(configurations()):
        entry=config['entry'];seen,edges,calls,b,steps=run_case(config,number,inventories[entry])
        coverage[entry].update(seen);edge_coverage[entry].update(edges);call_coverage[entry].update(calls)
        for site in calls:failure_configs.setdefault((entry,site),dict(config,fail_site=site))
        counts[entry]+=1;boundaries+=b
        records.append({'input':config,'steps':steps,'boundary_comparisons':b,'full_state_match':True})
        if (number+1)%500==0:print(number+1,'shared-helper retail comparisons passed',flush=True)
    for entry,inventory in inventories.items():
        assert coverage[entry]==inventory['addresses'],(hex(entry),'missing rows',inventory['addresses']-coverage[entry])
        assert edge_coverage[entry]==inventory['edges'],(hex(entry),'missing edges',inventory['edges']-edge_coverage[entry])
        assert call_coverage[entry]==inventory['calls'],(hex(entry),'missing calls',inventory['calls']-call_coverage[entry])
    for number,((entry,site),config) in enumerate(sorted(failure_configs.items())):
        _,_,_,b,_=run_case(config,number,inventories[entry]);boundaries+=b
    guards=0
    for entry in functions:
        memory=bytearray(baseline);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(memory)
        state=State();state.r[:]=range(16);fpu=Fpu();fpu.fpscr=0x380000
        values=[ctypes.c_uint32(v) for v in (0x60,0x8C000500,0xCAFEBABE,0xABCDEF01)]
        before=(bytes(memory),bytes(state.r),bytes(fpu),[v.value for v in values])
        assert functions[entry](view,ctypes.byref(fpu),ctypes.byref(values[0]),state.r,
            ctypes.byref(values[1]),ctypes.byref(values[2]),ctypes.byref(values[3]),None,Call())==-2
        assert before==(bytes(memory),bytes(state.r),bytes(fpu),[v.value for v in values]);guards+=1
    routines=[]
    for routine in manifest['routines']:
        entry=int(routine['address'],16)
        routines.append({**{k:routine[k] for k in ('name','address','end_exclusive','instruction_bytes','retail_span_sha256')},
                         'source':manifest['source'],'source_sha256':digest(PROJECT/manifest['source']),
                         'cases_passed':counts[entry],'covered_rows':len(coverage[entry]),
                         'covered_conditional_edges':len(edge_coverage[entry]),'covered_external_call_sites':len(call_coverage[entry])})
    files=[Path(__file__),manifest_path,PROJECT/'verify_startup.py',PROJECT/'build_main_dispatch_shared_helpers.ps1',
           PROJECT/'analysis/main-dispatch-shared-helpers/diagnostics.tsv']
    report={'routines':routines,'source':manifest['source'],'source_sha256':digest(PROJECT/manifest['source']),
            'binary_sha256':digest(BINARY),'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'cases_passed':len(records),'boundary_comparisons':boundaries,
            'service_error_cases_passed':len(failure_configs),'atomic_entry_guards_passed':guards,
            'comparison_level':'Full 16 MiB RAM, GPR/SR/PR/FPU, FPUL/MACL preservation and ordered callback-entry states compared against original SH4 execution; all discovered rows, conditional edges and call sites exercised.',
            'scope':'Both shared transitions with independent first/second query returns, object flag branches, nullable pointer chains, signed characters, all FR/SZ/PR combinations, raw FMOV patterns, callback mutations/mode changes and host failure at every external call site.',
            'limitations':['External callees use deterministic services; original implementations remain unrecovered in this checkpoint.',
                           'The retail instruction subset is not physical SH4 hardware, live gameplay or complete ABI/exception proof.',
                           'Invalid pointers, memory exceptions/MMIO, the full dispatcher and playable platform ports remain unverified.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_shared_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(records)} comparisons; 208 rows / 22 conditional edges / 10 call sites covered; {boundaries} boundaries; {len(failure_configs)} service-error cases; {guards} atomic guards')


if __name__=='__main__':main()
