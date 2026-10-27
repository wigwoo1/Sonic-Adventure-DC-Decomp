"""Original forward-word-copy loop, entire-RAM/CPU and failure-prefix oracles."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_interface_fixtures as fx
v=fx.v;P=v.P;ENTRY=0x8c6607fc;SRC=0x8cd90000;DST=0x8cda0000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
    configs=[]
    for count,alias,(mode,sr) in itertools.product((4,7,8,15,64,256),(0x8c000000,0xac000000),fx.CONFIGS):
        configs.append(dict(kind='normal',count=count,source=alias|(SRC&0xffffff),destination=alias|(DST&0xffffff),mode=mode,sr=sr))
    for delta,(mode,sr) in itertools.product((-4,0,4),fx.CONFIGS):configs.append(dict(kind='overlap',count=16,source=SRC,destination=SRC+delta,mode=mode,sr=sr))
    for kind,(mode,sr) in itertools.product(('bad_source','bad_destination'),fx.CONFIGS):configs.append(dict(kind=kind,count=8,source=0 if kind=='bad_source' else SRC,destination=0 if kind=='bad_destination' else DST,mode=mode,sr=sr))
    for count,(mode,sr) in itertools.product((0,1,3,0xffffffff),fx.CONFIGS):configs.append(dict(kind='boundary_prefix',count=count,source=0x8cfffffc,destination=DST,mode=mode,sr=sr))
    if smoke:configs=[configs[0],next(c for c in configs if c['kind']=='boundary_prefix')]
    for data in configs:
        c,m,s,f=fx.setup(ram,ENTRY,[data['destination'],data['source'],data['count'],0x89abcdef],data['mode'],data['sr'])
        for address in (SRC-8,DST-8):m[address&0xffffff:(address&0xffffff)+288]=bytes((j*37+11)&255 for j in range(288))
        fx.put(m,0x8cfffffc,0x89abcdef)
        before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);regs=list(s.r);regs[6]>>=2;sr=before.sr
        # Specification-level sequential word copy. This also captures the
        # original do-while underflow, and stops only at the declared RAM edge.
        while True:
            def valid(a):return a&0x1f000000==0x0c000000 and (a&0xffffff)<=0xfffffc
            if not valid(regs[5]):status=-3;stop=0x8c6607fe;break
            regs[1]=v.load(wanted,regs[5]);regs[5]=(regs[5]+4)&0xffffffff
            regs[6]=(regs[6]-1)&0xffffffff;sr=(sr&~1)|int(regs[6]==0)
            if not valid(regs[4]):status=-3;stop=0x8c660802;break
            fx.put(wanted,regs[4],regs[1]);regs[4]=(regs[4]+4)&0xffffffff
            if not regs[6]:status=0;stop=before.pr;break
        r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
        assert r['status']==status and r['full_state_match'] and r['ordered_boundaries_match']
        assert a==wanted and list(s.r)==regs and s.sr==sr and s.pr==before.pr and s.pc==stop;n+=5
        assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==c['macl'] and r['final_gbr']==c['gbr'];n+=4
        assert not r['cache_purge_sequence'] and len(r['boundaries'])==1;n+=2
        seen.update(path)
        for j,pc in enumerate(path):
            if pc==0x8c660804 and j+2<len(path):edges.add((hex(pc),hex(path[j+2])))
        checks+=n;records.append(data|dict(status=status,stop_pc=hex(stop),steps=len(path),independent_checks=n))
    if smoke:print('INTERFACE COPY SMOKE PASS',len(records));return
    body=next(r for r in v.manifest['routines'] if r['address']==hex(ENTRY));missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
    assert not missing and not missing_edges
    files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_interface_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=[dict(address=hex(ENTRY),static_rows=8,executed_rows=8,executed_outcomes=2,missing_rows=[],missing_outcomes=[])],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original8row/2outcome forwardwordcopy. Positivefloor(bytecount/4) copies that many words, preserves ignored tail bytes and original forward-overlap order. Aligned coherentRAM/twoaliases/fourCPUFPUprofiles, entireRAM/allGPR/SR/PR/FPU/special oracles. Zero/1/3byte counts retain original do-while underflow:one valid boundary word then reached read fault;hugeunsignedcount same bounded input-edge prefix. No source earlyreturn, waitlimit or bulk-copy shortcut. Fault behavior is existing host mappedRAM policy, not physical exception delivery;zero-count complete2^32iteration execution is not claimed.')
    (P/'analysis/movie_input_interface_copy_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INTERFACE COPY PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
