"""Generate native active-query, predicate and table-clear routines from retail."""
import ast,csv,hashlib,json,re
from pathlib import Path

PROJECT=Path(__file__).resolve().parent
ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
TRANSLATOR=PROJECT/'ghidra/translate_tikal_helper.py'
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
SPECS=((0x8C05E69E,0x8C05E6C4,19,'sa_main_dispatch_clear_table_8c05e69e'),
       (0x8C05E6C4,0x8C05E6E8,18,'sa_main_dispatch_active_predicate_8c05e6c4'),
       (0x8C05E6E8,0x8C05E788,51,'sa_main_dispatch_active_query_8c05e6e8'))
names={s[0]:s[3] for s in SPECS}
node=next(n for n in ast.parse(TRANSLATOR.read_text()).body if isinstance(n,ast.FunctionDef) and n.name=='simple')


def signed(value,bits):return value-(1<<bits) if value&(1<<(bits-1)) else value


header='''/* Generated from retail instruction exports by recover_main_dispatch_active_query.py.
 * Inferred descriptive names; static C, not an opcode interpreter. All game
 * callees in this three-routine group are native; observer is optional host
 * instrumentation. RAM is a valid 16 MiB guest image. No MMIO/memory exceptions.
 * Host 0 = complete, retail return in R0. Floating support can return -1 for
 * unsupported PR/RM or 1 for an enabled exception, preserving partial state.
 * Integer-only paths retain their original behavior for all FPU modes.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
typedef void (*sa_active_observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);}
 else f->bank[bank][reg]=rd(m,a,4);
}
'''
for name in names.values():header+=f'SA_STAGE_EXPORT int {name}(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_active_observer);\n'
functions=[];routines=[];listings=[]
for start,end,count,name in SPECS:
    listing=PROJECT/f'analysis/main-dispatch-active-query/{start:08x}_instructions.tsv';listings.append(listing)
    rows={}
    for row in csv.DictReader(listing.open(),delimiter='\t'):
        a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
        assert len(raw)==int(row['length'])==2 and start<=a<end and a not in rows
        assert binary[a-0x8C010000:a-0x8C010000+2]==raw
        rows[a]=int.from_bytes(raw,'little'),row['instruction'].lstrip('_')
    assert len(rows)==count
    seen=set();todo=[start];edges=set();calls={}
    while todo:
        a=todo.pop()
        if a in seen:continue
        assert a in rows,hex(a)
        seen.add(a);w=rows[a][0]
        if w==0xB or w>>12 in (0xA,0xB):
            assert a+2 in rows;seen.add(a+2)
            if w==0xB:continue
            dest=a+4+2*signed(w&4095,12)
            if w>>12==0xB:assert dest in names;calls[a]=dest;todo.append(a+4)
            else:todo.append(dest)
        elif w>>8 in (0x89,0x8B):
            for dest in (a+2,a+4+2*signed(w&255,8)):todo.append(dest);edges.add((a,dest))
        else:
            assert w&0xF0FF not in (0x400B,0x402B)
            todo.append(a+2)
    assert seen==set(rows)
    namespace={'re':re,'rows':rows}
    exec(compile(ast.Module(body=[node],type_ignores=[]),str(TRANSLATOR),'exec'),namespace)
    def simple(a):
        w,text=rows[a];n,m=w>>8&15,w>>4&15
        if w&0xF00F==0xF008:return f'fload(m,f,{n}u,r[{m}]);'
        if w&0xF0FF==0xF02D:return f'status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][{n}]);if(status)return status;'
        if w&0xF00F==0xF005:return f'status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][{n}],f->bank[(f->fpscr>>21)&1u][{m}],&f->fpscr,sr,1);if(status)return status;'
        assert not text.startswith('f'),text
        return namespace['simple'](a)
    out=[];slots=set()
    for a,(w,text) in sorted(rows.items()):
        if a in slots:continue
        terminal=False
        if w>>8 in (0x89,0x8B):
            dest=a+4+2*signed(w&255,8);condition='(*sr&1u)' if w>>8==0x89 else '!(*sr&1u)'
            code=f'if({condition})goto L{dest:x};goto L{a+2:x};';terminal=True
        elif w>>12==0xA:
            code=simple(a+2)+f'goto L{a+4+2*signed(w&4095,12):x};';slots.add(a+2);terminal=True
        elif w>>12==0xB:
            dest=a+4+2*signed(w&4095,12)
            code=f'*pr=0x{a+4:x}u;'+simple(a+2)
            code+=f'if(observe)observe(ctx,0x{dest:x}u,m,f,sr,r,pr);status={names[dest]}(m,f,sr,r,pr,fpul,macl,ctx,observe);if(status)return status;'
            slots.add(a+2)
        elif w==0xB:code=simple(a+2)+'return 0;';slots.add(a+2);terminal=True
        else:code=simple(a)
        if not terminal:
            next_address=a+4 if a+2 in slots else a+2;assert next_address in rows
            code+=f'goto L{next_address:x};'
        out.append(f'L{a:x}: {code} /* {text} */')
    function=f'''SA_STAGE_EXPORT int {name}(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_active_observer observe){{
 (void)f;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)observe;
'''
    if calls:function+=' int status;\n'
    function+=f' goto L{start:x};\n'+'\n'.join(out)+'\n}\n'
    functions.append(function)
    routines.append({'name':name,'address':hex(start),'end_exclusive':hex(end),'instruction_rows':count,
                     'instruction_bytes':count*2,'reachable_addresses':[hex(a) for a in sorted(seen)],
                     'conditional_edges':[[hex(a),hex(b)] for a,b in sorted(edges)],
                     'native_call_sites':{hex(a):hex(b) for a,b in sorted(calls.items())},
                     'retail_span_sha256':hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()})
    print(name,count,'rows;',len(edges),'conditional edges;',len(calls),'native call sites')
source=PROJECT/'source/sa_main_dispatch_active_query.c'
source.write_text(header+'\n'.join(functions))
files=[BINARY,TRANSLATOR,Path(__file__),source]+listings
report={'routines':routines,'source':'source/sa_main_dispatch_active_query.c',
        'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
        'scope':'Byte/branch proof and static C recovery for three routines; all five game call sites in the active routine resolve to native predicate/clear helpers. Behavioral and FPU boundary proof is separate.'}
(PROJECT/'analysis/main_dispatch_active_translation.json').write_text(json.dumps(report,indent=2)+'\n')
