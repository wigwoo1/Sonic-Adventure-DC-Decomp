"""Recover byte-checked shared dispatcher helpers as annotated static C."""
import ast
import csv
import hashlib
import json
import re
from pathlib import Path

PROJECT=Path(__file__).resolve().parent
ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
TRANSLATOR=PROJECT/'ghidra/translate_tikal_helper.py'
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
simple_node=next(n for n in ast.parse(TRANSLATOR.read_text()).body if isinstance(n,ast.FunctionDef) and n.name=='simple')


def signed(value,bits):return value-(1<<bits) if value&(1<<(bits-1)) else value


HEADER='''/* Static C generated from retail instruction exports by
 * recover_main_dispatch_shared_helpers.py. Descriptive symbols are inferred.
 * No runtime opcode decoding. Guest RAM is 16 MiB; invalid pointers, hardware
 * exceptions and MMIO are not modeled. All callees remain explicit services.
 * Host 0 means complete; the retail result is in R0. Missing call service
 * returns -2 atomically. A service error returns its status with partial state.
 * These routines use integer instructions and bank/width-aware FMOV only;
 * no floating-point arithmetic or PR=0 restriction is introduced.
 */
#include "fpu_cleanup_transfer.h"
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);
 }else f->bank[bank][reg]=rd(m,a,4);
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
}
'''


def recover(start,end,name,count):
    listing=PROJECT/f'analysis/main-dispatch-shared-helpers/{start:08x}_instructions.tsv'
    rows={}
    for row in csv.DictReader(listing.open(),delimiter='\t'):
        a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
        assert len(raw)==int(row['length'])==2 and start<=a<end
        assert binary[a-0x8C010000:a-0x8C010000+2]==raw
        assert a not in rows;rows[a]=int.from_bytes(raw,'little'),row['instruction'].lstrip('_')
    assert len(rows)==count
    seen=set();todo=[start];edges=set();calls=set()
    while todo:
        a=todo.pop()
        if a in seen:continue
        assert a in rows,hex(a)
        seen.add(a);w=rows[a][0]
        if w==0xB or w>>12 in (0xA,0xB) or w&0xF0FF==0x400B:
            assert a+2 in rows;seen.add(a+2)
            if w==0xB:continue
            if w>>12==0xA:todo.append(a+4+2*signed(w&4095,12))
            else:calls.add(a);todo.append(a+4)
        elif w>>8 in (0x89,0x8B,0x8D,0x8F):
            fall=a+4 if w>>8 in (0x8D,0x8F) else a+2
            if fall==a+4:assert a+2 in rows;seen.add(a+2)
            for target in (a+4+2*signed(w&255,8),fall):todo.append(target);edges.add((a,target))
        else:todo.append(a+2)
    assert seen==set(rows),(len(seen),len(rows))
    namespace={'re':re,'rows':rows}
    exec(compile(ast.Module(body=[simple_node],type_ignores=[]),str(TRANSLATOR),'exec'),namespace)
    def simple(a):
        w,text=rows[a]
        if text.startswith('fmov'):
            n,m=w>>8&15,w>>4&15
            form=w&15
            if form==6:return f'fload(m,f,{n}u,r[0]+r[{m}]);'
            if form==8:return f'fload(m,f,{n}u,r[{m}]);'
            if form==7:return f'fstore(m,f,{m}u,r[0]+r[{n}]);'
            if form==10:return f'fstore(m,f,{m}u,r[{n}]);'
            raise AssertionError((hex(a),text))
        assert not text.startswith('f'),text
        return namespace['simple'](a)
    out=[];slots=set()
    for a,(w,text) in sorted(rows.items()):
        if a in slots:continue
        terminal=False
        if w>>8 in (0x89,0x8B,0x8D,0x8F):
            target=a+4+2*signed(w&255,8)
            condition='(*sr&1u)' if w>>8 in (0x89,0x8D) else '!(*sr&1u)'
            if w>>8 in (0x8D,0x8F):
                code=f'take={condition};'+simple(a+2)+f'if(take)goto L{target:x};goto L{a+4:x};'
                slots.add(a+2)
            else:code=f'if({condition})goto L{target:x};goto L{a+2:x};'
            terminal=True
        elif w>>12==0xA:
            code=simple(a+2)+f'goto L{a+4+2*signed(w&4095,12):x};'
            slots.add(a+2);terminal=True
        elif w==0xB:
            code=simple(a+2)+'return 0;';slots.add(a+2);terminal=True
        elif w>>12==0xB or w&0xF0FF==0x400B:
            code=f'target=0x{a+4+2*signed(w&4095,12):08x}u;' if w>>12==0xB else f'target=r[{w>>8&15}];'
            code+=f'*pr=0x{a+4:08x}u;'+simple(a+2)
            code+='status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'
            slots.add(a+2)
        else:code=simple(a)
        if not terminal:
            next_address=a+4 if a+2 in slots else a+2
            assert next_address in rows
            code+=f'goto L{next_address:x};'
        out.append(f'L{a:x}: {code} /* {text} */')
    function=f'''SA_STAGE_EXPORT int {name}(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){{
 (void)fpul;(void)macl;
 if(!call)return -2;
 int status;uint32_t target;
'''
    if any(rows[a][0]>>8 in (0x8D,0x8F) for a in rows):function+=' int take;\n'
    function+=f' goto L{start:x};\n'+'\n'.join(out)+'\n}\n'
    record={'name':name,'address':hex(start),'end_exclusive':hex(end),'instruction_rows':count,
            'instruction_bytes':count*2,'reachable_addresses':[hex(a) for a in sorted(seen)],
            'conditional_edges':[[hex(a),hex(b)] for a,b in sorted(edges)],
            'external_call_sites':[hex(a) for a in sorted(calls)],
            'listing':str(listing),'retail_span_sha256':hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()}
    print(name,count,'instructions;',len(edges),'conditional edges;',len(calls),'call sites')
    return function,record


functions=[];routines=[]
for args in ((0x8C0AFC98,0x8C0AFE20,'sa_main_dispatch_status_transition_8c0afc98',153),
             (0x8C0B11C0,0x8C0B122E,'sa_main_dispatch_flag_transition_8c0b11c0',55)):
    code,record=recover(*args);functions.append(code);routines.append(record)
source=PROJECT/'source/sa_main_dispatch_shared_helpers.c'
source.write_text(HEADER+'\n'.join(functions))
files=[BINARY,TRANSLATOR,Path(__file__),source]+[Path(r['listing']) for r in routines]
report={'routines':routines,'source':'source/sa_main_dispatch_shared_helpers.c',
        'sha256':{str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in files},
        'scope':'Byte validation, exact raw-opcode reachability and static native translation of both discovered function bodies. Behavior verification is separate; external routines remain services.'}
(PROJECT/'analysis/main_dispatch_shared_translation.json').write_text(json.dumps(report,indent=2)+'\n')
