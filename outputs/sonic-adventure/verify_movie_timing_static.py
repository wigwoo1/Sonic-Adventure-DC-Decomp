"""Conservative original-code constant propagation for timing constructors."""
import collections,csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
ENTRIES=(0x8c657e00,0x8c6585c0,0x8c658804);DIV=0x8c10ca5c
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def signed(x,bits):return x-(1<<bits) if x&(1<<(bits-1)) else x
def read_rows(path,raw):
    rows={int(r['address'],16):r for r in csv.DictReader(path.open(),delimiter='\t')}
    for pc,row in rows.items():assert raw[pc-0x8c010000:pc-0x8c010000+2].hex()==row['bytes']
    return rows
def analyse(entry,rows,raw):
    # Join incoming constants at the same PC/delay/call state. Unknown values
    # explore both branches; discarded edges are never selected from test data.
    states={};todo=collections.deque();edges=set();visits=collections.Counter()
    def enqueue(key,regs,t):
        value=tuple(regs)+(t,)
        if key in states:
            old=states[key];value=tuple(a if a==b else None for a,b in zip(old,value))
            if old==value:return
        states[key]=value;todo.append(key)
    enqueue((entry,None,None),[None]*16,None)
    while todo:
        key=todo.popleft();pc,pending,callee=key;value=states[key];r=list(value[:16]);t=value[16]
        visits[pc]+=1;assert pc in rows,hex(pc)
        row=rows[pc];word=int.from_bytes(bytes.fromhex(row['bytes']),'little')
        text=row['instruction'].lstrip('_');op,_,args=text.partition(' ')
        assert op in ('add','addc','and','bf','bf/s','bra','bsr','bt','bt/s',
                      'cmp/eq','cmp/gt','cmp/hi','cmp/pz','jsr','lds.l','mov',
                      'mov.l','mov.w','mul.l','nop','or','rts','shad','shar',
                      'shll','shll2','sts','sts.l','sub','tst'),(hex(pc),text)
        n=(word>>8)&15;m=(word>>4)&15;target=r[n] if op=='jsr' else None
        dest=re.search(r'(?:^|,)r(\d+)$',args)
        if word>>12==0xe:r[n]=signed(word&255,8)&0xffffffff
        elif word&0xf00f==0x6003:r[n]=r[m]
        elif word>>12 in (9,13):
            a=int(args.split(',')[0],16);width=2 if word>>12==9 else 4
            x=int.from_bytes(raw[a-0x8c010000:a-0x8c010000+width],'little')
            r[n]=(signed(x,16) if width==2 else x)&0xffffffff
        elif op=='tst':
            parts=args.split(',')
            def operand(s):return int(s[1:],0)&0xffffffff if s.startswith('#') else r[int(s[1:])]
            a,b=map(operand,parts);t=True if a==0 or b==0 else ((a&b)==0 if a is not None and b is not None else None)
        elif op.startswith('cmp/') or op in ('div0s','div1','rotcl','rotcr','addc','addv','subc','subv','dt','shll','shlr','shal','shar','rotl','rotr','movt'):
            # These may write T, or depend on it. Forget both T and any result.
            if dest and not op.startswith('cmp/'):r[int(dest[1])]=None
            t=None
        elif dest:r[int(dest[1])]=None
        for index in re.findall(r'@r(\d+)\+|@-r(\d+)',args):
            r[int(index[0] or index[1])]=None
        if pending is not None:
            if pending==-1:continue
            if callee is not None:
                r=[x if callee==DIV and 4<=i<=14 else None for i,x in enumerate(r)];t=None
            enqueue((pending,None,None),r,t);continue
        if op=='rts':enqueue((pc+2,-1,None),r,t)
        elif op in ('jsr','bsr'):
            if op=='bsr':target=pc+4+signed(word&0xfff,12)*2
            enqueue((pc+2,pc+4,target if target is not None else -2),r,t)
        elif op=='bra':enqueue((pc+2,pc+4+signed(word&0xfff,12)*2,None),r,t)
        elif op in ('bt','bf','bt/s','bf/s'):
            taken=pc+4+signed(word&255,8)*2;fall=pc+(4 if '/s' in op else 2)
            choices=(False,True) if t is None else (t if op.startswith('bt') else not t,)
            for take in choices:
                end=taken if take else fall;edges.add((pc,end))
                enqueue((pc+2,end,None) if '/s' in op else (end,None,None),r,t)
        else:
            assert op not in ('jmp','braf','bsrf','rte','trapa','ldc','ldc.l'),(hex(pc),text)
            enqueue((pc+2,None,None),r,t)
    wanted=set()
    for pc,row in rows.items():
        word=int.from_bytes(bytes.fromhex(row['bytes']),'little');op=row['instruction'].lstrip('_').split(' ')[0]
        if op in ('bt','bf','bt/s','bf/s'):wanted.update(((pc,pc+4+signed(word&255,8)*2),(pc,pc+(4 if '/s' in op else 2))))
    reached={key[0] for key in states}
    return dict(address=hex(entry),static_rows=len(rows),abstract_states=len(states),worklist_transfers=sum(visits.values()),
                proven_unreachable_rows=[hex(pc) for pc in sorted(set(rows)-reached)],
                proven_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)])
def main():
    binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
    assert digest(binary)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    division=P/'analysis/boot-file-dependencies/8c10ca5c_instructions.tsv';div=read_rows(division,raw)
    # This byte-checked leaf has no call, bank switch or reference at all to
    # r4..r14. Its stores cannot change CPU registers. No ABI promise is assumed.
    for row in div.values():
        text=row['instruction'].lstrip('_')
        assert not re.search(r'\br(?:[4-9]|1[0-4])\b',text)
        assert text.split(' ')[0] in ('tst','mov.l','bt','mov','div0s','subc','rotcl','div1','addc','rts')
    files=[Path(__file__),binary,division];results=[]
    for entry in ENTRIES:
        listing=P/f'analysis/movie-render-config-dependencies/{entry:08x}_instructions.tsv';files.append(listing)
        results.append(analyse(entry,read_rows(listing,raw),raw))
    report=dict(routines=results,division_rows_byte_checked=len(div),
                preconditions=['Entry at the named original function, immutable code and PC-relative literals, and no asynchronous CPU-register changes or entry into the body interior.',
                               'Direct fixtures use ordinary guest RAM; stores do not rewrite instruction/literal bytes. Memory loads other than PC-relative literal loads remain unknown.',
                               'Every returning call destroys all register/T knowledge, except byte-checked leaf 8c10ca5c cannot change r4..r14. Non-returning calls cannot reach later code.'],
                scope='Conservative joined constant propagation with explicit branch delays. Unknown predicates explore both outcomes. No observed test paths or Ghidra unreachable warnings select the proof result. Unreachable instructions/outcomes are reported separately from executed coverage.',
                sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_timing_static_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    for row in results:print(row)
if __name__=='__main__':main()
