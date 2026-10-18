"""Generate static C for the byte-checked integer shared-check body."""
import ast,csv,hashlib,json,re
from pathlib import Path

PROJECT=Path(__file__).resolve().parent
ROOT=PROJECT.parents[1]
START,END=0x8C06C880,0x8C06CA30
NAME='sa_main_dispatch_shared_check_8c06c880'
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
LISTING=PROJECT/'analysis/main-dispatch-shared-check/8c06c880_instructions.tsv'
TRANSLATOR=PROJECT/'ghidra/translate_tikal_helper.py'
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
rows={}
for row in csv.DictReader(LISTING.open(),delimiter='\t'):
    a=int(row['address'],16);raw=bytes.fromhex(row['bytes'])
    assert int(row['length'])==len(raw)==2 and START<=a<END and a not in rows
    assert binary[a-0x8C010000:a-0x8C010000+2]==raw
    rows[a]=int.from_bytes(raw,'little'),row['instruction'].lstrip('_')
assert len(rows)==195


def signed(value,bits):return value-(1<<bits) if value&(1<<(bits-1)) else value


seen=set();todo=[START];edges=set()
while todo:
    a=todo.pop()
    if a in seen:continue
    assert a in rows,hex(a)
    seen.add(a);w=rows[a][0]
    assert w>>12!=0xB and w&0xF0FF not in (0x400B,0x402B) and w>>12!=0xF
    if w==0xB or w>>12==0xA:
        assert a+2 in rows;seen.add(a+2)
        if w!=0xB:todo.append(a+4+2*signed(w&4095,12))
    elif w>>8 in (0x89,0x8B,0x8D,0x8F):
        fall=a+4 if w>>8 in (0x8D,0x8F) else a+2
        if fall==a+4:assert a+2 in rows;seen.add(a+2)
        for target in (a+4+2*signed(w&255,8),fall):todo.append(target);edges.add((a,target))
    else:todo.append(a+2)
assert seen==set(rows)
node=next(n for n in ast.parse(TRANSLATOR.read_text()).body if isinstance(n,ast.FunctionDef) and n.name=='simple')
namespace={'re':re,'rows':rows}
exec(compile(ast.Module(body=[node],type_ignores=[]),str(TRANSLATOR),'exec'),namespace)
simple=namespace['simple']
out=[];slots=set()
for a,(w,text) in sorted(rows.items()):
    if a in slots:continue
    terminal=False
    if w>>8 in (0x89,0x8B,0x8D,0x8F):
        target=a+4+2*signed(w&255,8)
        condition='(*sr&1u)' if w>>8 in (0x89,0x8D) else '!(*sr&1u)'
        if w>>8 in (0x8D,0x8F):
            code=f'take={condition};'+simple(a+2)+f'if(take)goto L{target:x};goto L{a+4:x};';slots.add(a+2)
        else:code=f'if({condition})goto L{target:x};goto L{a+2:x};'
        terminal=True
    elif w>>12==0xA:
        code=simple(a+2)+f'goto L{a+4+2*signed(w&4095,12):x};';slots.add(a+2);terminal=True
    elif w==0xB:
        code=simple(a+2)+'return 0;';slots.add(a+2);terminal=True
    else:code=simple(a)
    if not terminal:
        next_address=a+4 if a+2 in slots else a+2
        assert next_address in rows
        code+=f'goto L{next_address:x};'
    out.append(f'L{a:x}: {code} /* {text} */')
header='''/* Generated from retail bytes by recover_main_dispatch_shared_check.py.
 * Inferred symbol, original-PC annotations and static native C control flow;
 * no opcode interpreter. 195 integer instructions, no external calls/FPU.
 * Valid 16 MiB RAM and original pointer preconditions are required. Memory
 * exceptions/MMIO are excluded. SR and stack writes remain explicit.
 * Host 0 means completion, with retail flag result in R0. The callback can
 * be null because this routine never calls it. PR/FPU/FPUL/MACL are untouched.
 */
#include "fpu_cleanup_transfer.h"
typedef int (*sa_check_service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
SA_STAGE_EXPORT int sa_main_dispatch_shared_check_8c06c880(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_check_service call){
 (void)f;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)call;
 int take;
 goto L8c06c880;
'''
source=PROJECT/'source/sa_main_dispatch_shared_check.c'
source.write_text(header+'\n'.join(out)+'\n}\n')
files=[BINARY,LISTING,TRANSLATOR,Path(__file__),source]
report={'address':hex(START),'end_exclusive':hex(END),'name':NAME,'source':'source/sa_main_dispatch_shared_check.c',
        'instruction_rows':195,'instruction_bytes':390,'external_calls':0,
        'reachable_addresses':[hex(a) for a in sorted(seen)],
        'conditional_edges':[[hex(a),hex(b)] for a,b in sorted(edges)],
        'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
        'scope':'Original-byte comparison and raw branch reachability account for all 195 rows; fixed native C labels retain integer/SR/stack effects. Behavioral proof is separate.'}
(PROJECT/'analysis/main_dispatch_shared_check_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'195 shared-check instructions translated; {len(edges)} conditional edges; no external calls')
