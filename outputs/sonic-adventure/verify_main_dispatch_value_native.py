"""Verify the two value routines and the closed native early-transition chain."""
import csv
import ctypes
import hashlib
import itertools
import json
from pathlib import Path

import verify_main_dispatch_early_native as gate
from verify_startup import State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

PROJECT=Path(__file__).resolve().parent
WRAPPER,LEAF,CALLER=0x8C07C298,0x8C07C9D2,0x8C0AFF1A
SOURCE='source/sa_main_dispatch_value_native.c'
ROOT=gate.ROOT
DLL=gate.DLL
LIB=gate.LIB
write=LIB.sa_main_dispatch_value_write_8c07c9d2
write.argtypes=[ctypes.POINTER(ctypes.c_uint8),P];write.restype=None
apply=LIB.sa_main_dispatch_value_apply_8c07c298
apply.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_void_p,gate.OBSERVE];apply.restype=None
connected=LIB.sa_main_dispatch_transition_values_native
connected.argtypes=gate.COMMON+[gate.OBSERVE];connected.restype=ctypes.c_int
load,store=gate.load,gate.store
ALT_PRIMARY,ALT_DATA,SECOND,NEXT_PRIMARY,NEXT_DATA=0x8CB90000,0x8CBA0000,0x8CAF0000,0x8CBC0000,0x8CBD0000
TRACKED={gate.LEAF,WRAPPER,LEAF}


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def cases():
    values=(0,0x4000,0x80000000,0xFFFFFFFF)
    for entry,index,value in itertools.product(('leaf','wrapper'),range(256),values):
        yield {'entry':entry,'index':index,'value':value,'mode':((index&7)<<19),
               'upper':0xA5000000 if index&1 else 0,'gate':6}
    for index,mode,kind,pair in itertools.product((0,1,2,7,0x7F,0x80,0xFF),
            (0,0x100000,0x200000,0x300000),(5,6),
            ((0,0),(0xFFFFFFFF,0x80000000),(0x4000,0xFFFFFFFF),(0x80000000,0x7FFFFFFF))):
        yield {'entry':'connected','index':index,'mode':mode,'gate':kind,
               'raw_value':pair[0],'base_value':pair[1]}
    for entry,layout,index in itertools.product(('leaf','wrapper'),
            ('relocated','distinct_primary','secondary_overlap','stack_reload','table_reload'),(0,0xFF)):
        yield {'entry':entry,'index':index,'mode':0x380000,'gate':5,
               'value':0x12345678,'layout':layout}
    for mode,kind,index in itertools.product((0,0x100000,0x200000,0x300000),(5,6),(0,0xFF)):
        yield {'entry':'connected','index':index,'mode':mode,'gate':kind,
               'raw_value':0xDEADBEEF,'base_value':0x12345678,'observe':False}


def prepare(config,number):
    original={'entry':'connected','gate':config['gate'],'mode':config['mode'],
              'flags':0xA5A5,'character':config['index'],
              'raw_value':config.get('raw_value',0x12345678),
              'base_value':config.get('base_value',0x56789012)}
    memory,state,fpu=gate.prepare(original,number)
    state.pc={'leaf':LEAF,'wrapper':WRAPPER,'connected':CALLER}[config['entry']]
    layout=config.get('layout','normal')
    table_a=load(memory,0x8C07C3A0);table_b=load(memory,0x8C07CB00);table_c=load(memory,0x8C07CB04)
    if layout!='normal':
        table_a,table_b,table_c=0x8CB00000,0x8CB10000,0x8CB20000
        for address,value in ((0x8C07C3A0,table_a),(0x8C07CB00,table_b),(0x8C07CB04,table_c)):
            store(memory,address,value)
    slot_a=table_a+config['index']*4
    primary_a=gate.OBJECT
    primary_b=ALT_PRIMARY if layout in ('distinct_primary','stack_reload','table_reload') else primary_a
    secondary=SECOND
    numeric=config.get('value',0x12345678)
    if layout=='secondary_overlap':secondary=primary_b-0xC
    if layout=='stack_reload':
        # The leaf's +18 write replaces its saved secondary pointer. A later
        # stack reload must choose NEXT_PRIMARY, not the old secondary.
        leaf_sp=state.r[15]-(24 if config['entry']=='wrapper' else 4)
        primary_b=leaf_sp-0x18
        numeric=(0x4000-NEXT_PRIMARY)&0xFFFFFFFF
    if layout=='table_reload':
        # Leaf output changes the wrapper's selected table entry. The wrapper
        # must reload that entry before choosing which pending byte to clear.
        primary_b=slot_a-0x18
        numeric=(0x4000-NEXT_PRIMARY)&0xFFFFFFFF
    for primary,data in ((primary_a,gate.DATA),(primary_b,ALT_DATA),(NEXT_PRIMARY,NEXT_DATA)):
        if primary==primary_a:data=gate.DATA
        store(memory,primary+0x14,0x11223344)
        store(memory,primary+0x1C,0x55667788)
        store(memory,primary+0x3C,data)
        store(memory,data+0x24,config['gate'],1)
    store(memory,slot_a,primary_a)
    store(memory,table_b+config['index']*4,primary_b)
    store(memory,table_c+config['index']*4,secondary)
    if config['entry'] in ('leaf','wrapper'):
        state.r[4]=(config['index']|config.get('upper',0))&0xFFFFFFFF
        if config['entry']=='leaf':
            state.r[5]=0x11223344;state.r[6]=numeric;state.r[7]=0x55667788
        else:state.r[5]=numeric
    # Remember addresses for independent final-state assertions. In the
    # connected mode-five path the original object +18 is its input baseline.
    return memory,state,fpu,{'primary':primary_b,'secondary':secondary,
                            'pending':gate.DATA+0x24,'next_pending':NEXT_DATA+0x24,
                            'numeric':numeric,'layout':layout}


def inventory():
    result={}
    for start,end,count in ((WRAPPER,0x8C07C2D0,28),(LEAF,0x8C07C9FA,20)):
        path=PROJECT/f'analysis/main-dispatch-value-writer/{start:08x}_instructions.tsv'
        rows={}
        for row in csv.DictReader(path.open(),delimiter='\t'):
            address=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
            assert int(row['length'])==len(raw)==2
            assert gate.binary[address-0x8C010000:address-0x8C010000+2]==raw
            rows[address]=int.from_bytes(raw,'little')
        assert len(rows)==count and set(rows)==set(range(start,end,2))
        assert rows[end-4]==0xB
        if start==WRAPPER:
            word=rows[0x8C07C2B8];offset=word&4095;offset-=4096 if offset&2048 else 0
            assert word>>12==0xB and 0x8C07C2BC+offset*2==LEAF
        result[start]=rows
    return result


def execute(config,number,memory,initial,initial_fpu,meta):
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial)
    expected_fpu=Fpu.from_buffer_copy(initial_fpu);expected_events=[];visited=set()
    def snapshot(target,mem,regs,sr,pr,fpu):
        return (target,tuple(regs),sr,pr,bytes(fpu),load(mem,gate.DATA+0x24,1))
    def trace(pc,word,regs,sr,pr,_fpu):
        visited.add(pc)
        if pc in TRACKED and pc!=initial.pc:
            expected_events.append(snapshot(pc,expected_memory,regs,sr,pr,expected_fpu))
    steps=reference(expected_memory,expected,fpu=expected_fpu,trace=trace,instruction_budget=2000)
    actual=State.from_buffer_copy(initial);fpu=Fpu.from_buffer_copy(initial_fpu)
    sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
    fpul=ctypes.c_uint32(initial_fpu.fpul);macl=ctypes.c_uint32(0x10203040)
    view=(ctypes.c_uint8*gate.RAM_SIZE).from_buffer(memory)
    events=[];errors=[]
    @gate.OBSERVE
    def observe(_ctx,target,mem,f,s,r,p):
        try:events.append(snapshot(target,mem,list(r[:16]),s[0],p[0],f.contents))
        except Exception as exc:errors.append(repr(exc))
    observer=observe if config.get('observe',True) else gate.OBSERVE()
    if config['entry']=='leaf':write(view,actual.r)
    elif config['entry']=='wrapper':apply(view,ctypes.byref(fpu),ctypes.byref(sr),actual.r,ctypes.byref(pr),None,observer)
    else:
        # No external-service callback is provided. All instructions and
        # native-dependency bodies execute without synthetic callees.
        assert connected(view,ctypes.byref(fpu),ctypes.byref(sr),actual.r,ctypes.byref(pr),
                         ctypes.byref(fpul),ctypes.byref(macl),None,Call(),observer)==0
    assert not errors,(number,config,errors)
    assert memory==expected_memory,(number,config,'RAM differs')
    assert list(actual.r)==list(expected.r),(number,config,'registers differ',list(actual.r),list(expected.r))
    assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
    assert bytes(fpu)==bytes(expected_fpu),(number,config,'FPU differs')
    assert fpul.value==initial_fpu.fpul and macl.value==0x10203040
    assert actual.r[15]==initial.r[15] and pr.value==initial.pr
    assert list(actual.r)[8:15]==list(initial.r)[8:15]
    if config.get('observe',True):assert events==expected_events,(number,config,'boundary differs')
    else:assert not events
    if meta['layout']=='normal':
        if config['entry']=='connected':
            value=config.get('raw_value',0x12345678)
            if config['gate']==5:value=(value-config.get('base_value',0x56789012)+0x4000)&0xFFFFFFFF
        else:value=meta['numeric']
        derived=(0x4000-value)&0xFFFFFFFF
        assert load(memory,meta['primary']+0x18)==derived
        assert load(memory,meta['secondary']+0x20)==derived
        assert load(memory,meta['primary']+0x14)==0x11223344
        assert load(memory,meta['primary']+0x1C)==0x55667788
        if config['entry']!='leaf':assert load(memory,meta['pending'],1)==0
    if meta['layout']=='table_reload' and config['entry']=='wrapper':
        assert load(memory,meta['pending'],1)==config['gate']
        assert load(memory,meta['next_pending'],1)==0
    actual.sr=sr.value;actual.pr=pr.value;actual.pc=initial.pr
    return visited,len(events),steps,actual,fpu


def main():
    listings=inventory();coverage={key:set() for key in ('leaf','wrapper','connected')}
    records=[];counts={'leaf':0,'wrapper':0,'connected':0};boundaries=0
    for number,config in enumerate(cases()):
        memory,state,fpu,meta=prepare(config,number)
        seen,events,steps,_,_=execute(config,number,memory,state,fpu,meta)
        coverage[config['entry']].update(seen);counts[config['entry']]+=1;boundaries+=events
        records.append({'input':config,'steps':steps,'boundary_comparisons':events,'full_state_match':True})
        if (number+1)%500==0:print(number+1,'native value-chain comparisons passed',flush=True)
    for entry,starts in (('leaf',(LEAF,)),('wrapper',(WRAPPER,LEAF)),('connected',(WRAPPER,LEAF))):
        for start in starts:assert set(listings[start])<=coverage[entry],(entry,hex(start),'unvisited rows')
    # Persistent RAM: repeated calls keep the previous output baseline and
    # table/object state, rather than resetting the fixture between calls.
    sequence_config={'entry':'connected','index':1,'mode':0,'gate':5,'raw_value':1,'base_value':0x12345678}
    memory,state,fpu,meta=prepare(sequence_config,0);persistent=[]
    for number,mode in enumerate((5,6,5,6,5,6,5,6)):
        old=load(memory,gate.OBJECT+0x18);raw=(0x10203040+number*0x11111111)&0xFFFFFFFF
        store(memory,gate.DATA+0x24,mode,1);store(memory,gate.DATA+0x2C,raw)
        state.r[4]=gate.OBJECT;state.r[5]=gate.AUX;state.r[6]=gate.WORK;state.pc=CALLER
        config=dict(sequence_config,gate=mode,raw_value=raw,base_value=old)
        _,events,steps,state,fpu=execute(config,number,memory,state,fpu,meta)
        boundaries+=events;persistent.append({'mode':mode,'previous_baseline':old,'raw_value':raw,'steps':steps,'full_state_match':True})
    # A missing external service only fails if the new integration reaches
    # another unresolved target. Compare that exact partially executed state.
    config={'entry':'connected','index':1,'mode':0,'gate':0}
    memory,state,fpu,meta=prepare(config,0)
    store(memory,gate.OBJECT+1,13,1)
    store(memory,gate.OBJECT+4,0x1500,2)
    expected_memory=bytearray(memory);expected=State.from_buffer_copy(state);expected_fpu=Fpu.from_buffer_copy(fpu)
    reference(expected_memory,expected,fpu=expected_fpu,stop_pc=0x8C07256A,instruction_budget=2000)
    view=(ctypes.c_uint8*gate.RAM_SIZE).from_buffer(memory)
    sr=ctypes.c_uint32(state.sr);pr=ctypes.c_uint32(state.pr);fpul=ctypes.c_uint32(fpu.fpul);macl=ctypes.c_uint32(0x10203040)
    assert connected(view,ctypes.byref(fpu),ctypes.byref(sr),state.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,Call(),gate.OBSERVE())==-2
    assert memory==expected_memory and list(state.r)==list(expected.r)
    assert sr.value==expected.sr and pr.value==expected.pr and bytes(fpu)==bytes(expected_fpu)
    assert fpul.value==0xCAFEBABE and macl.value==0x10203040
    # The outer routine's unsupported-precision guard still rejects atomically.
    memory,state,fpu,meta=prepare(dict(config,gate=5,mode=0x80000),0)
    view=(ctypes.c_uint8*gate.RAM_SIZE).from_buffer(memory)
    sr=ctypes.c_uint32(state.sr);pr=ctypes.c_uint32(state.pr)
    before=(bytes(memory),bytes(state.r),bytes(fpu),sr.value,pr.value)
    assert connected(view,ctypes.byref(fpu),ctypes.byref(sr),state.r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,Call(),gate.OBSERVE())==-1
    assert before==(bytes(memory),bytes(state.r),bytes(fpu),sr.value,pr.value)
    routines=[]
    for start,end,name in ((WRAPPER,0x8C07C2D0,'sa_main_dispatch_value_apply_8c07c298'),
                           (LEAF,0x8C07C9FA,'sa_main_dispatch_value_write_8c07c9d2')):
        routines.append({'name':name,'address':hex(start),'end_exclusive':hex(end),
                         'source':SOURCE,'source_sha256':digest(PROJECT/SOURCE),
                         'instruction_bytes':end-start,
                         'retail_span_sha256':hashlib.sha256(gate.binary[start-0x8C010000:end-0x8C010000]).hexdigest()})
    files=[Path(__file__),PROJECT/'verify_main_dispatch_early_native.py',PROJECT/'verify_startup.py',
           PROJECT/'source/sa_main_dispatch_value_native.c',PROJECT/'source/sa_main_dispatch_transition_early_native.c',
           PROJECT/'source/sa_main_dispatch_early_gate_8c0afe72.c',PROJECT/'source/sa_main_dispatch_transition_8c0aff1a.c',
           PROJECT/'source/sh4_add_sub.c',PROJECT/'source/sh4_compare_bits.h',PROJECT/'sh4_binary32.py',
           PROJECT/'build_main_dispatch_early_native.ps1',PROJECT/'ghidra/export_main_dispatch_value_writer.ps1',
           PROJECT/'analysis/main-dispatch-value-writer/8c07c298_instructions.tsv',
           PROJECT/'analysis/main-dispatch-value-writer/8c07c9d2_instructions.tsv',
           PROJECT/'analysis/main-dispatch-value-writer/diagnostics.tsv']
    report={'routines':routines,'source':SOURCE,'source_sha256':digest(PROJECT/SOURCE),
            'binary_sha256':digest(gate.BINARY),'dll_sha256':digest(DLL),
            'sha256':{str(p):digest(p) for p in files},'cases_passed':len(records),'counts':counts,
            'persistent_calls_passed':len(persistent),'boundary_comparisons':boundaries,
            'unobserved_cases_passed':sum(not c['input'].get('observe',True) for c in records),
            'missing_service_partial_state_checks':1,'atomic_precision_guard_checks':1,
            'new_instruction_rows_verified':48,'new_instruction_rows_covered_through_parent':48,
            'closed_chain_external_calls':0,
            'comparison_level':'Full 16 MiB RAM, GPR/SR/PR/FPU and FPUL/MACL preservation against original SH4 execution without callee hooks; ordered native dependency boundary states when observation is enabled.',
            'scope':'Two integer routines directly and native transition -> early gate -> value wrapper -> writer, all 256 table index bytes in constructed entries, wrapping arithmetic, guest aliases/table relocation, zero external service callbacks on closed early paths, persistent state and unobserved execution.',
            'limitations':['Constructed table entries for all byte indexes do not prove all indexes valid in the real game.',
                           'Other transition cases still depend on external routines; this is not full dispatcher/game recovery.',
                           'Invalid guest pointers, physical hardware, memory exceptions/MMIO and full playable ports are not verified.',
                           'The outer transition retains its existing PR=1 and other floating-point limitations.'],
            'cases':records,'persistent_sequence':persistent}
    (PROJECT/'analysis/main_dispatch_value_native_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(records)} cases {counts}; {len(persistent)} persistent calls; {boundaries} native boundaries; both routines/48 rows covered through parent; zero external callbacks on closed early paths; 2 guard/error checks')


if __name__=='__main__':main()
