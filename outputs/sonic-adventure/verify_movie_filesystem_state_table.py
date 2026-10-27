"""Independent nonpositive-count effects for the still-partial table reader."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_filesystem_state as fast
v=fast.old;P=v.P;ENTRY=0x8c64ae62;METADATA=0x8cd90000
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,a,value):m[a&0xffffff:(a&0xffffff)+4]=(value&0xffffffff).to_bytes(4,'little')
def main():
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
    for channel,count,(mode,sr) in itertools.product((0,7),(0,0xffffffff,0x80000000),((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))):
        c=v.Input(dict(entry=ENTRY,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,filesystem_state_arguments=[channel,0,0,0]))
        m,s,f=v.prepare(ram,c);s.sr=sr;f.fpscr=0x40001|(mode<<19)
        for bank in range(2):
            for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
        descriptor=v.load(m,0x8c64aec0)+channel*328;put(m,descriptor+0x1c,METADATA);put(m,METADATA+0x10,count)
        before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m)
        for j,value in enumerate((s.r[14],s.r[13],s.r[12],s.r[11],s.r[10],s.pr),1):put(wanted,s.r[15]-j*4,value)
        regs=list(s.r);regs[0]=0;regs[3]=v.load(m,0x8c64aec0)
        r,path,a,s,f=fast.compare(m,s,f,c)
        assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n=3
        assert a==wanted and list(s.r)==regs and s.sr==(before.sr&~1) and s.pr==s.pc==before.pr;n+=4
        assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==328*channel and r['final_gbr']==c['gbr'];n+=4
        assert len(r['boundaries'])==1 and not r['cache_purge_sequence'];n+=2
        seen.update(path)
        for j,pc in enumerate(path):
            text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
        checks+=n;records.append(dict(channel=channel,count=hex(count),mode=mode,sr=hex(sr),independent_checks=n))
    body=next(r for r in v.manifest['routines'] if r['address']==hex(ENTRY));missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
    assert missing and missing_edges
    files=[Path(__file__),P/'movie_filesystem_state_reference.py',P/'write_watch_filesystem_state.py',v.MP,P/'analysis/movie_filesystem_state_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=[dict(address=hex(ENTRY),static_rows=44,executed_rows=44-len(missing),executed_outcomes=6-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges))],newly_registered_entries=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Nonpositive signed table counts only:zero,-1,INT_MIN,two channels,four CPU/FPU profiles. Independent entireRAM/GPR/SR/PR/FPU/special effects and original six stack writes. Positive read/loop/error paths remain unqualified;zero new routine credit. Coherent metadata is declared helper input,not a task result or supplied downstream return.')
    (P/'analysis/movie_filesystem_state_table_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('FILESYSTEM TABLE PARTIAL PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
