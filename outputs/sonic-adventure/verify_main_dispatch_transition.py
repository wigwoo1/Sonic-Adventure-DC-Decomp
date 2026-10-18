"""Compare native transition control flow with the untouched retail SH4 executor.

External callees are deterministic services in both runs. This checks the
caller, including call arguments and mutations, not the services themselves.
"""
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
ENTRY, END=0x8C0AFF1A,0x8C0B07E4
OBJECT,WORK,AUX,DATA=0x8CA00000,0x8CA10000,0x8CA20000,0x8CA30000
OUTER,INNER,CHAIN,LEAF=0x8CA40000,0x8CA50000,0x8CA60000,0x8CA70000
COMPANION,TASK=0x8CA80000,0x8CA90000
ALLOC,ALLOC_WORK,ALLOC_BYTE=0x8CAB0000,0x8CAC0000,0x8CAD0000
SERVICES={0x8C0AFE72,0x8C07256A,0x8C0940DA,0x8C0A9660,0x8C0B60B8,0x8C07B320,0x8C09852E}
RAM_SIZE=0x1000000
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
binary=BINARY.read_bytes()
baseline=bytearray(RAM_SIZE)
baseline[0x10000:0x10000+len(binary)]=binary
DLL=ROOT/'work/sonic-adventure/build/main_dispatch_transition.dll'
library=ctypes.CDLL(str(DLL))
native=library.sa_main_dispatch_transition_8c0aff1a
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
native.restype=ctypes.c_int


def store(memory,address,value,width=4):
    for i in range(width):
        memory[(address&0xFFFFFF)+i]=(value>>(8*i))&255


def load(memory,address,width=4):
    return sum(int(memory[(address&0xFFFFFF)+i])<<(8*i) for i in range(width))


def fbits(value):
    return struct.unpack('<I',struct.pack('<f',value))[0]


def service(config,target,memory,regs,sr,pr,fpu):
    assert target in SERVICES,hex(target)
    # Every service writes a separate observable location. The allocator can
    # independently succeed or fail for its two calls; it returns valid guest
    # pointers so both pointer-chasing paths can execute.
    store(memory,0x8CAE0000+(target&0xFFF),0xA5000000|(target&0xFFFF))
    if config.get('mutate',False):
        for i in range(8):
            regs[i]=(0x34120000+config['selector']*0x101+i*0x12345)&0xFFFFFFFF
        sr[0]^=1
        for bank in range(2):
            for i in range(8):
                fpu.banks[bank][i]=0x3F000000+bank*0x10000+i*0x1000
        flags=load(memory,OBJECT+4,2)
        store(memory,OBJECT+4,flags^0x0500,2)
    if config.get('flip_bank',False):
        fpu.fpscr^=0x200000
    if target==0x8C09852E:
        # The PR value identifies the first and second allocation call.
        bit=1 if pr[0]==0x8C0B075A else 2
        regs[0]=ALLOC if config.get('allocation_mask',3)&bit else 0
    else:
        regs[0]=0x12345678


def configurations():
    modes=[bank|width for bank in (0,0x200000) for width in (0,0x100000)]
    for selector,mode in itertools.product(range(256),modes):
        yield {'selector':selector,'mode':mode,'flags':0x1100}
    for selector,mode,flags in itertools.product(range(1,51),modes,(0x1000,0x1500,0x3000,0xFFFF,0x1003)):
        yield {'selector':selector,'mode':mode,'flags':flags,'mutate':bool(selector&1)}
    for mode,gate,value in itertools.product(modes,('gate_byte','gate_word'),(1,0x80,0xFF)):
        yield {'selector':13,'mode':mode,'flags':0xFFFF,gate:value,'mutate':True}
    for mode,selector in itertools.product(modes,(1,26,30,32,46,255)):
        yield {'selector':selector,'mode':mode,'flags':0,'current':0x80}
    for mode,selector,current in itertools.product(modes,(26,30,32,38),(1,2,5,43,50,51,54,55,57,60,61,0x80)):
        yield {'selector':selector,'mode':mode,'flags':0x1100,'current':current}
    for mode,selector,values in itertools.product(modes,(26,30,32),
            ((0.0,0.0,0.0,0.0),(-4.0,2.0,1.0,0.0),(8.0,2.0,1.0,3.0),(1.0,8.0,-2.0,2.0))):
        yield {'selector':selector,'mode':mode,'flags':0x1100,'floats':values,'round_mode':1}
    for mode,raw in itertools.product(modes,(0x7F800000,0xFF800000,0x7F800001,0x7FC00001,1,0x80000001)):
        yield {'selector':26,'mode':mode,'flags':0x1100,'compare_raw':raw}
    for mode,mask in itertools.product(modes,range(4)):
        yield {'selector':46,'mode':mode,'flags':0xFFFF,'allocation_mask':mask,'mutate':True}
    for mode,selector,character in itertools.product(modes,(35,38),(0,1,0x80,0xFF)):
        yield {'selector':selector,'mode':mode,'flags':0x1500,'character':character,'mutate':True}
    for mode,selector in itertools.product(modes,(9,13,24,29,31,33,35,38,46,50)):
        yield {'selector':selector,'mode':mode,'flags':0x1500,'mutate':True,'flip_bank':True}


def main():
    manifest_path=PROJECT/'analysis/main_dispatch_transition_translation.json'
    manifest=json.loads(manifest_path.read_text())
    for path,digest in manifest['sha256'].items():
        assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==digest,path
    covered=set();edges=set();call_sites=set();records=[]
    for number,config in enumerate(configurations()):
        memory=bytearray(baseline)
        selector=config['selector'];flags=config['flags']
        store(memory,OBJECT,config.get('current',1),1)
        store(memory,OBJECT+1,selector,1)
        store(memory,OBJECT+4,flags,2)
        store(memory,OBJECT+8,config.get('character',0),1)
        store(memory,OBJECT+0x3C,DATA)
        store(memory,DATA+0x24,config.get('gate_byte',0),1)
        store(memory,DATA+0x4C,config.get('gate_word',0))
        store(memory,OBJECT+0x10,COMPANION)
        store(memory,COMPANION+0x20,TASK)
        store(memory,WORK+0x68,OUTER)
        store(memory,OUTER+0x20,INNER)
        store(memory,INNER+0x38,CHAIN)
        store(memory,CHAIN+0xC,LEAF)
        store(memory,ALLOC+0x20,ALLOC_WORK)
        store(memory,ALLOC+0x2C,ALLOC_BYTE)
        # Every transfer has distinct adjacent words, including SZ=1 odd FR
        # encodings selecting the other physical register bank.
        object_y,inner_y,leaf_a,leaf_b=config.get('floats',(8.0,2.0,1.0,3.0))
        for address,value in ((OBJECT+0x24,object_y),(INNER+0x24,inner_y),
                              (LEAF+0xC,leaf_a),(LEAF+0x18,leaf_b),
                              (WORK+0x38,inner_y),(WORK+0xC4,leaf_a)):
            store(memory,address,fbits(value));store(memory,address+4,0x40400000)
        if 'compare_raw' in config:
            # In SZ=1 FR3 aliases the other bank's FR2/FR3 pair; only its
            # active-bank value participates in FCMP. Both widths are tested.
            store(memory,WORK+0x38,config['compare_raw'])
            store(memory,WORK+0xC4,config['compare_raw'])
        initial=State();initial.r[:]=[(0x12345678+i*0x111111+number)&0xFFFFFFFF for i in range(16)]
        initial.r[4]=OBJECT;initial.r[5]=AUX;initial.r[6]=WORK
        initial.r[15]=0x8CF00000-(number%4)*0x100
        initial.pr=0x8C000500;initial.pc=ENTRY;initial.sr=0x70000060|(number&1)
        initial_fpu=Fpu();initial_fpu.fpscr=config['mode']|0x40000|config.get('round_mode',0)
        for bank in range(2):
            initial_fpu.banks[bank][:]=[0x3F000000+bank*0x10000+i*0x1000 for i in range(16)]
        initial_fpu.fpul=0xABCDEF01
        expected_memory=bytearray(memory);expected=State.from_buffer_copy(initial)
        expected_fpu=Fpu.from_buffer_copy(initial_fpu);expected_calls=[];path=[]

        def event(target,mem,regs,sr,pr,fpu):
            return (target,pr,tuple(regs),sr,bytes(fpu),
                    load(mem,OBJECT+4,2),load(mem,WORK+0x124,2))

        def hook_for(target):
            def hook(mem,regs,sr):
                expected_calls.append(event(target,mem,regs,sr,expected.pr,expected_fpu))
                sr_value=[sr];pr_value=[expected.pr]
                service(config,target,mem,regs,sr_value,pr_value,expected_fpu)
                expected.pr=pr_value[0]
                return sr_value[0]
            return hook

        def trace(pc,word,regs,sr,pr,fpu):
            assert ENTRY<=pc<END,hex(pc)
            covered.add(pc);path.append(pc)
            if word>>12==0xB or word&0xF0FF==0x400B:
                call_sites.add(pc)

        steps=reference(expected_memory,expected,
            hooks={t:hook_for(t) for t in SERVICES},fpu=expected_fpu,
            trace=trace,instruction_budget=2000)
        # Resolve delayed branches at the instruction after their slot, not
        # the slot itself. Conditional non-delayed edges use the next PC.
        for i,pc in enumerate(path):
            word=int.from_bytes(binary[pc-0x8C010000:pc-0x8C010000+2],'little')
            if word>>8 in (0x89,0x8B,0x8D,0x8F) or word==0x0023:
                j=i+(2 if word>>8 in (0x8D,0x8F) or word==0x0023 else 1)
                if j<len(path):edges.add((pc,path[j]))
        actual=State.from_buffer_copy(initial);actual_fpu=Fpu.from_buffer_copy(initial_fpu)
        view=(ctypes.c_uint8*RAM_SIZE).from_buffer(memory)
        sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
        fpul=ctypes.c_uint32(initial_fpu.fpul);macl=ctypes.c_uint32(0xDEADBEEF)
        actual_calls=[];errors=[]

        @Call
        def call(_ctx,target,mem,fpu_ptr,sr_ptr,regs,pr_ptr):
            try:
                fpu=fpu_ptr.contents
                actual_calls.append(event(target,mem,[regs[i] for i in range(16)],sr_ptr[0],pr_ptr[0],fpu))
                service(config,target,mem,regs,sr_ptr,pr_ptr,fpu)
                return 0
            except Exception as exc:
                errors.append(repr(exc));return -99

        result=native(view,ctypes.byref(actual_fpu),ctypes.byref(sr),actual.r,
            ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
        assert result==0 and not errors,(number,config,result,errors)
        assert memory==expected_memory,(number,config,'RAM differs')
        assert list(actual.r)==list(expected.r),(number,config,'registers differ',list(actual.r),list(expected.r))
        assert sr.value==expected.sr and pr.value==expected.pr,(number,config,'SR/PR differs')
        assert bytes(actual_fpu)==bytes(expected_fpu),(number,config,'FPU differs')
        assert fpul.value==initial_fpu.fpul and macl.value==0xDEADBEEF
        assert actual_calls==expected_calls,(number,config,'callback boundary differs')
        assert actual.r[15]==initial.r[15] and pr.value==initial.pr
        assert list(actual.r)[8:15]==list(initial.r)[8:15]
        # Independent behavioral assertions on representative callback-free
        # transitions, supplementing the whole-state differential comparison.
        if not config.get('gate_byte') and not config.get('gate_word'):
            if not flags&0x1000:assert actual.r[0]==0
            elif selector==1:
                assert load(memory,OBJECT,1)==3 and load(memory,WORK+0x124,2)==11 and actual.r[0]==1
            elif selector==2:
                assert load(memory,OBJECT,1)==7 and load(memory,WORK+0x124,2)==15 and actual.r[0]==1
        elif config.get('gate_word') and not config.get('gate_byte'):
            assert load(memory,OBJECT,1)==80 and load(memory,OBJECT+4,2)==flags&0xDAFF and actual.r[0]==1
        records.append({'input':config,'original_steps':steps,'calls':len(actual_calls),
                        'retail_result_r0':actual.r[0],'state_and_call_boundaries_match':True})
        if (number+1)%500==0:print(number+1,'native/retail cases passed',flush=True)

    expected_addresses={int(a,16) for a in manifest['reachable_addresses']}
    missing=expected_addresses-covered
    expected_conditional_edges={(int(a,16),int(b,16)) for a,b in manifest['conditional_and_switch_edges']
                                if int(a,16)==0x8C0AFFB8 or int.from_bytes(binary[int(a,16)-0x8C010000:int(a,16)-0x8C010000+2],'little')>>8 in (0x89,0x8B,0x8D,0x8F)}
    missing_edges=expected_conditional_edges-edges
    assert not missing,('unexecuted original instructions',[hex(a) for a in sorted(missing)])
    assert not missing_edges,('unexecuted conditional/switch edges',[(hex(a),hex(b)) for a,b in sorted(missing_edges)])
    assert call_sites=={int(a,16) for a in manifest['external_call_site_addresses']}
    # Unsupported precision and missing-service checks must leave all guest
    # state untouched, making the boundary explicit for a future integration.
    guards=0
    for fpscr,callback,want in ((0x80000,Call(),-1),(0,Call(),-2)):
        memory=bytearray(baseline);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(memory)
        fpu=Fpu();fpu.fpscr=fpscr;regs=(ctypes.c_uint32*16)(*range(16))
        values=[ctypes.c_uint32(v) for v in (0x60,0x8C000500,1,2)]
        before=(bytes(memory),bytes(fpu),bytes(regs),[v.value for v in values])
        assert native(view,ctypes.byref(fpu),ctypes.byref(values[0]),regs,ctypes.byref(values[1]),
                      ctypes.byref(values[2]),ctypes.byref(values[3]),None,callback)==want
        assert before==(bytes(memory),bytes(fpu),bytes(regs),[v.value for v in values])
        guards+=1
    source=PROJECT/'source/sa_main_dispatch_transition_8c0aff1a.c'
    report={
        'address':hex(ENTRY),'end_exclusive':hex(END),'cases_passed':len(records),
        'entry_guards_passed':guards,'covered_retail_instruction_rows':len(covered),
        'covered_conditional_and_switch_edges':len(expected_conditional_edges),
        'covered_external_call_sites':len(call_sites),'selector_byte_values':256,
        'table_entries':50,'unique_switch_targets':45,
        'indexed_instruction_bytes':len(covered)*2,
        'binary_sha256':hashlib.sha256(binary).hexdigest(),
        'retail_span_sha256':hashlib.sha256(binary[ENTRY-0x8C010000:END-0x8C010000]).hexdigest(),
        'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
        'dll_sha256':hashlib.sha256(DLL.read_bytes()).hexdigest(),
        'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in
                  (manifest_path,Path(__file__),PROJECT/'verify_startup.py',
                   PROJECT/'sh4_binary32.py',PROJECT/'source/sh4_add_sub.c',
                   PROJECT/'source/sh4_compare_bits.h',PROJECT/'build_main_dispatch_transition.ps1')},
        'comparison_level':'Complete 16 MiB RAM, GPR/SR/PR/FPU/FPUL/MACL and external-call boundary arguments/order compared with original SH4 instructions; every 957 exported row and all conditional/switch edges exercised in constructed scenarios.',
        'scope':'Native control flow for the complete discovered helper instruction set, all 256 selector bytes, both FR banks and SZ widths, transition/early-return flags, finite/NaN comparison cases, allocation success/failure, callback mutations and FR changes.',
        'limitations':['External callees are deterministic services, not recovered implementations or live-game state.',
                      'Existing SH4 subset executor and software floating-point model are not physical Dreamcast hardware.',
                      'FPSCR.PR=1 is rejected before guest mutation; enabled floating-point exceptions, infinity arithmetic, unsupported rounding and invalid guest pointers are not covered.',
                      'This helper does not provide a recovered or playable full dispatcher, PC game or target-platform port.'],
        'cases':records,
    }
    (PROJECT/'analysis/main_dispatch_transition_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(records)} cases, {len(covered)} instruction rows, {len(expected_conditional_edges)} conditional/switch edges and {len(call_sites)} call sites passed; {guards} entry guards passed')


if __name__=='__main__':main()
