"""Retail comparisons for the early gate and its native transition caller.

All 256 gate bytes are tested directly and through the native caller. Original
SH4 execution enters the real dependency; only its remaining callee is hooked.
"""
import csv
import ctypes
import hashlib
import itertools
import json
import struct
from pathlib import Path

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

PROJECT=Path(__file__).resolve().parent
LEAF,END,CALLER,TARGET=0x8C0AFE72,0x8C0AFF1A,0x8C0AFF1A,0x8C07C298
OBJECT,WORK,AUX,DATA=0x8CA00000,0x8CA10000,0x8CA20000,0x8CA30000
OUTER,INNER,CHAIN,DETAIL=0x8CA40000,0x8CA50000,0x8CA60000,0x8CA70000
COMPANION,TASK,ALLOC,ALLOC_WORK,ALLOC_BYTE=0x8CA80000,0x8CA90000,0x8CAB0000,0x8CAC0000,0x8CAD0000
RAM_SIZE=0x1000000
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
DLL=ROOT/'work/sonic-adventure/build/main_dispatch_early_native.dll'
LIB=ctypes.CDLL(str(DLL))
LEAF_FN=LIB.sa_main_dispatch_early_gate_8c0afe72
CONNECTED_FN=LIB.sa_main_dispatch_transition_early_native
OBSERVE=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,
    ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P)
COMMON=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
LEAF_FN.argtypes=COMMON;LEAF_FN.restype=ctypes.c_int
CONNECTED_FN.argtypes=COMMON+[OBSERVE];CONNECTED_FN.restype=ctypes.c_int
binary=BINARY.read_bytes()
baseline=bytearray(RAM_SIZE);baseline[0x10000:0x10000+len(binary)]=binary


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def load(memory,address,width=4):
    return sum(int(memory[(address&0xFFFFFF)+i])<<(8*i) for i in range(width))


def store(memory,address,value,width=4):
    for i in range(width):memory[(address&0xFFFFFF)+i]=(value>>(8*i))&255


def input_cases():
    for gate,mode in itertools.product(range(256),(0,0x80000,0x100000,0x180000,0x200000,0x280000,0x300000,0x380000)):
        yield {'entry':'leaf','gate':gate,'mode':mode,'flags':0xA5A5}
    for gate,mode in itertools.product(range(256),(0,0x100000,0x200000,0x300000)):
        yield {'entry':'connected','gate':gate,'mode':mode,'flags':0xA5A5}
    for entry,gate,character,pair in itertools.product(('leaf','connected'),(5,6),(0,0x7F,0x80,0xFF),
            ((0,0),(0,0xFFFFFFFF),(0xFFFFFFFF,0),(0x80000000,0x7FFFFFFF),(0xFFFFFFFF,0xFFFFFFFF))):
        yield {'entry':entry,'gate':gate,'mode':0x300000,'flags':0xF123,
               'character':character,'raw_value':pair[0],'base_value':pair[1],
               'mutate':True,'flip_bank':True}
    for entry,gate,flags in itertools.product(('leaf','connected'),(0,1,2,3,4,7,255),(0,0x4000,0x8000,0xFFFF)):
        yield {'entry':entry,'gate':gate,'mode':0,'flags':flags}
    # These leave the early gate disabled and exercise forwarding to every
    # previously exposed external service target of the outer transition.
    for selector,mode in itertools.product((1,2,9,13,24,26,29,30,31,32,33,35,38,46,50),(0,0x100000,0x200000,0x300000)):
        yield {'entry':'connected','gate':0,'mode':mode,'flags':0x1500,'selector':selector,'mutate':True}
    for entry,gate in itertools.product(('leaf','connected'),(5,6)):
        yield {'entry':entry,'gate':gate,'mode':0,'flags':0xA5A5,'mutate':True,
               'literal_target':0x8CF12340}
    for entry,gate in itertools.product(('leaf','connected'),(5,6)):
        yield {'entry':entry,'gate':gate,'mode':0x300000,'flags':0xA5A5,
               'mutate':True,'fail_service':True}


def prepare(config,number):
    memory=bytearray(baseline)
    store(memory,OBJECT,1,1);store(memory,OBJECT+1,config.get('selector',13),1)
    store(memory,OBJECT+4,config['flags'],2);store(memory,OBJECT+8,config.get('character',0x80),1)
    store(memory,OBJECT+0x18,config.get('base_value',0x56789012))
    store(memory,OBJECT+0x3C,DATA);store(memory,DATA+0x24,config['gate'],1)
    store(memory,DATA+0x2C,config.get('raw_value',0x12345678))
    store(memory,DATA+0x4C,0)
    store(memory,0x8C0AFEF8,config.get('literal_target',TARGET))
    for a,v in ((OBJECT+0x10,COMPANION),(COMPANION+0x20,TASK),(WORK+0x68,OUTER),
                (OUTER+0x20,INNER),(INNER+0x38,CHAIN),(CHAIN+0xC,DETAIL),
                (ALLOC+0x20,ALLOC_WORK),(ALLOC+0x2C,ALLOC_BYTE)):
        store(memory,a,v)
    for a in (OBJECT+0x24,INNER+0x24,DETAIL+0xC,DETAIL+0x18,WORK+0x38,WORK+0xC4):
        store(memory,a,0x40000000);store(memory,a+4,0x40400000)
    state=State();state.r[:]=[(0x12345678+number*0x100+i*0x111111)&0xFFFFFFFF for i in range(16)]
    state.r[4]=OBJECT;state.r[5]=AUX;state.r[6]=WORK
    state.r[15]=0x8CF00000-(number%4)*0x100
    state.sr=0x70000060|(number&1);state.pr=0x8C000500
    state.pc=LEAF if config['entry']=='leaf' else CALLER
    fpu=Fpu();fpu.fpscr=config['mode']|0x40000
    for bank in range(2):fpu.banks[bank][:]=[0x3F000000+bank*0x10000+i*0x1000 for i in range(16)]
    fpu.fpul=0xCAFEBABE
    return memory,state,fpu


def service(config,target,memory,regs,sr,pr,fpu):
    allowed={TARGET,0x8CF12340,0x8C07256A,0x8C0940DA,0x8C0A9660,0x8C0B60B8,0x8C07B320,0x8C09852E}
    assert target in allowed,hex(target)
    if target in (TARGET,0x8CF12340):
        character=config.get('character',0x80)
        character=character-256 if character&128 else character
        assert regs[4]==character&0xFFFFFFFF
        argument=config.get('raw_value',0x12345678)
        if config['gate']==5:argument=(argument-config.get('base_value',0x56789012)+0x4000)&0xFFFFFFFF
        assert regs[5]==argument,(config,hex(regs[5]),hex(argument))
        assert pr[0]==0x8C0AFEC2
    store(memory,0x8CAE0000+(target&0xFFF),0xA5000000|(target&0xFFFF))
    if config.get('mutate'):
        for i in range(8):regs[i]=(0xABC00000+i*0x10101)&0xFFFFFFFF
        sr[0]^=1
        store(memory,OBJECT+4,load(memory,OBJECT+4,2)^0x0500,2)
        for bank in range(2):
            for i in range(8):fpu.banks[bank][i]=0x3E000000+i*0x10000
    if config.get('flip_bank'):fpu.fpscr^=0x200000
    regs[0]=ALLOC if target==0x8C09852E else 0x13579BDF


def event(target,memory,regs,sr,pr,fpu):
    return (target,tuple(regs),sr,pr,bytes(fpu),load(memory,OBJECT+4,2),load(memory,WORK+0x124,2))


def instruction_inventory():
    listing=PROJECT/'analysis/main-dispatch-early-dependency/8c0afe72_instructions.tsv'
    rows={}
    for row in csv.DictReader(listing.open(),delimiter='\t'):
        address=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
        assert int(row['length'])==len(raw)==2 and LEAF<=address<END
        assert binary[address-0x8C010000:address-0x8C010000+2]==raw
        assert address not in rows;rows[address]=int.from_bytes(raw,'little')
    seen=set();todo=[LEAF];edges=set()
    while todo:
        a=todo.pop()
        if a in seen:continue
        assert a in rows,hex(a)
        seen.add(a);w=rows[a]
        if w==0xB or w>>12==0xA or w&0xF0FF==0x400B:
            assert a+2 in rows;seen.add(a+2)
            if w==0xB:continue
            if w>>12==0xA:
                d=w&4095;d=d-4096 if d&2048 else d;todo.append(a+4+2*d)
            else:todo.append(a+4)
        elif w>>8==0x89:
            d=w&255;d=d-256 if d&128 else d
            for target in (a+2,a+4+2*d):todo.append(target);edges.add((a,target))
        else:todo.append(a+2)
    assert len(rows)==55 and seen==set(rows)
    assert len(edges)==12
    assert rows[0x8C0AFEBA]==0xD30F and rows[0x8C0AFEBE]==0x430B
    assert load(baseline,0x8C0AFEF8)==TARGET
    return rows,edges


def main():
    rows,wanted_edges=instruction_inventory()
    coverage={'leaf':set(),'connected':set()};edges={'leaf':set(),'connected':set()}
    counts={'leaf':0,'connected':0,'callback_failures':0};boundaries=0;records=[]
    for number,config in enumerate(input_cases()):
        memory,initial,initial_fpu=prepare(config,number)
        expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial)
        expected_fpu=Fpu.from_buffer_copy(initial_fpu);expected_events=[];trace_pcs=[]
        target=config.get('literal_target',TARGET)

        def trace(pc,word,regs,sr,pr,_fpu):
            if pc in rows:
                coverage[config['entry']].add(pc);trace_pcs.append(pc)
            if pc==LEAF and config['entry']=='connected':
                expected_events.append(event(LEAF,expected_memory,regs,sr,pr,expected_fpu))

        def hook_for(address):
            def hook(mem,regs,sr):
                expected_events.append(event(address,mem,regs,sr,expected.pr,expected_fpu))
                status=[sr];return_address=[expected.pr]
                service(config,address,mem,regs,status,return_address,expected_fpu)
                expected.pr=return_address[0]
                return status[0]
            return hook

        externals={target,0x8C07256A,0x8C0940DA,0x8C0A9660,0x8C0B60B8,0x8C07B320,0x8C09852E}
        stop=target if config.get('fail_service') else None
        steps=reference(expected_memory,expected,hooks={a:hook_for(a) for a in externals},
                        fpu=expected_fpu,stop_pc=stop,trace=trace,instruction_budget=2000)
        if config.get('fail_service'):
            expected_events.append(event(target,expected_memory,list(expected.r),expected.sr,expected.pr,expected_fpu))
            sr_value=[expected.sr];pr_value=[expected.pr];regs=list(expected.r)
            service(config,target,expected_memory,regs,sr_value,pr_value,expected_fpu)
            expected.r[:]=regs;expected.sr=sr_value[0];expected.pr=pr_value[0]
            counts['callback_failures']+=1
        for a,b in zip(trace_pcs,trace_pcs[1:]):
            if rows[a]>>8==0x89:edges[config['entry']].add((a,b))
        actual=State.from_buffer_copy(initial);fpu=Fpu.from_buffer_copy(initial_fpu)
        sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
        fpul=ctypes.c_uint32(initial_fpu.fpul);macl=ctypes.c_uint32(0x10203040)
        actual_events=[];external_targets=[];errors=[]
        view=(ctypes.c_uint8*RAM_SIZE).from_buffer(memory)

        @OBSERVE
        def observe(_ctx,address,mem,f,s,r,p):
            try:actual_events.append(event(address,mem,list(r[:16]),s[0],p[0],f.contents))
            except Exception as exc:errors.append(repr(exc))

        @Call
        def call(_ctx,address,mem,f,s,r,p):
            try:
                external_targets.append(address)
                assert address!=LEAF,'Early dependency escaped native dispatch'
                if config['entry']=='leaf':actual_events.append(event(address,mem,list(r[:16]),s[0],p[0],f.contents))
                service(config,address,mem,r,s,p,f.contents)
                return -17 if config.get('fail_service') else 0
            except Exception as exc:errors.append(repr(exc));return -99

        args=(view,ctypes.byref(fpu),ctypes.byref(sr),actual.r,ctypes.byref(pr),
              ctypes.byref(fpul),ctypes.byref(macl),None,call)
        result=LEAF_FN(*args) if config['entry']=='leaf' else CONNECTED_FN(*args,observe)
        want=-17 if config.get('fail_service') else 0
        assert result==want and not errors,(number,config,result,errors)
        assert memory==expected_memory,(number,config,'RAM differs')
        assert list(actual.r)==list(expected.r),(number,config,'GPR differs',list(actual.r),list(expected.r))
        assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
        assert bytes(fpu)==bytes(expected_fpu),(number,config,'FPU differs')
        assert fpul.value==initial_fpu.fpul and macl.value==0x10203040
        assert actual_events==expected_events,(number,config,'boundary differs')
        boundaries+=len(actual_events)
        if not config.get('fail_service'):
            assert actual.r[15]==initial.r[15] and pr.value==initial.pr
            assert list(actual.r)[8:15]==list(initial.r)[8:15]
            if config['entry']=='leaf':
                assert actual.r[0]==int(1<=config['gate']<=6)
                if 1<=config['gate']<=4:assert load(memory,OBJECT+4,2)==config['flags']|0x4000
                if config['gate'] not in range(1,7):assert load(memory,OBJECT+4,2)==config['flags']&0xBFFF
            elif config['gate']:
                # The original caller deliberately discards the child's 0/1.
                assert actual.r[0]==0
        counts[config['entry']]+=1
        records.append({'input':config,'steps':steps,'boundaries':len(actual_events),
                        'external_targets':[hex(a) for a in external_targets],
                        'full_state_match':True,'host_status':result})
        if (number+1)%500==0:print(number+1,'early-dependency comparisons passed',flush=True)
    for kind in ('leaf','connected'):
        assert coverage[kind]==set(rows),(kind,'uncovered instructions',set(rows)-coverage[kind])
        assert edges[kind]==wanted_edges,(kind,'uncovered branches',wanted_edges-edges[kind])
    guards=0
    for connected,mode,want in ((False,0,-2),(True,0,-2),(True,0x80000,-1)):
        config={'entry':'connected' if connected else 'leaf','gate':5,'mode':mode,'flags':0xFFFF}
        mem,state,fpu=prepare(config,0);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(mem)
        sr=ctypes.c_uint32(state.sr);pr=ctypes.c_uint32(state.pr);fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0)
        before=(bytes(mem),bytes(state.r),bytes(fpu),sr.value,pr.value,fpul.value,macl.value)
        # PR rejection is tested with a valid callback; null external is a
        # separate atomic -2 integration precondition.
        callback=Call(lambda *_:0) if mode else Call()
        args=(view,ctypes.byref(fpu),ctypes.byref(sr),state.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,callback)
        result=CONNECTED_FN(*args,OBSERVE()) if connected else LEAF_FN(*args)
        assert result==want
        assert before==(bytes(mem),bytes(state.r),bytes(fpu),sr.value,pr.value,fpul.value,macl.value)
        guards+=1
    files=[Path(__file__),PROJECT/'verify_startup.py',PROJECT/'build_main_dispatch_early_native.ps1',
           PROJECT/'source/sa_main_dispatch_transition_8c0aff1a.c',
           PROJECT/'source/sa_main_dispatch_transition_early_native.c',
           PROJECT/'source/sh4_add_sub.c',PROJECT/'source/sh4_compare_bits.h',
           PROJECT/'sh4_binary32.py',PROJECT/'analysis/main_dispatch_transition_translation.json',
           PROJECT/'analysis/main-dispatch-early-dependency/8c0afe72_instructions.tsv',
           PROJECT/'analysis/main-dispatch-early-dependency/diagnostics.tsv']
    source=PROJECT/'source/sa_main_dispatch_early_gate_8c0afe72.c'
    report={'address':hex(LEAF),'end_exclusive':hex(END),'indexed_instruction_bytes':110,
            'source':'source/sa_main_dispatch_early_gate_8c0afe72.c','source_sha256':digest(source),
            'binary_sha256':digest(BINARY),'retail_span_sha256':hashlib.sha256(binary[LEAF-0x8C010000:END-0x8C010000]).hexdigest(),
            'dll_sha256':digest(DLL),'sha256':{str(p):digest(p) for p in files},
            'cases_passed':len(records),'counts':counts,'entry_guards_passed':guards,
            'boundary_comparisons':boundaries,'instruction_rows':55,'conditional_edges':12,
            'instruction_rows_covered_by_entry':{k:len(v) for k,v in coverage.items()},
            'comparison_level':'Complete 16 MiB RAM, all GPR/SR/PR/FPU and preserved FPUL/MACL against original SH4 execution, with ordered native-dependency/external-service boundary states.',
            'scope':'All 256 gate bytes directly and through the native caller, all FR/SZ/PR combinations for the integer child, wrapping differences, signed character arguments, literal retargeting, callback mutations and four partial-state service-error boundaries.',
            'limitations':['The remaining target 0x8c07c298 and other outer-helper services use deterministic test callbacks, not recovered implementations.',
                           'SH4 subset/software FPU comparisons are not physical hardware or live-game validation.',
                           'The connected outer routine retains its PR=1 rejection and original floating-point limitations.',
                           'Invalid guest pointers, exceptions/MMIO and the complete main dispatcher/game/ports are not verified.'],
            'cases':records}
    (PROJECT/'analysis/main_dispatch_early_native_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{counts["leaf"]} leaf and {counts["connected"]} connected comparisons passed; {boundaries} boundaries; 55 rows / 12 edges in each entry; {counts["callback_failures"]} service-error cases; {guards} atomic guards')


if __name__=='__main__':main()
