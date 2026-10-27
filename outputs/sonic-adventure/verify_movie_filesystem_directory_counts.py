"""Independent signed nonpositive directory-count effects."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_filesystem_directory as fast
v=fast.old;P=v.P;ENTRY=0x8c64a9a8;META=0x8cd90000
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,a,value):m[a&0xffffff:(a&0xffffff)+4]=(value&0xffffffff).to_bytes(4,'little')
def main():
    assert v.effects.DLL.name=='movie_filesystem_directory.dll'
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
    for channel,count,(mode,sr) in itertools.product((0,7),(0,0xffffffff,0x80000000),((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))):
        c=v.Input(dict(entry=ENTRY,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,filesystem_directory_arguments=[channel,0,0,0]))
        m,s,f=v.prepare(ram,c);s.sr=sr;f.fpscr=0x40001|(mode<<19)
        for bank in range(2):
            for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
        descriptor=v.load(m,0x8c64aa84)+328*channel;put(m,descriptor+0x1c,META);put(m,META+0x18,count);put(m,META+0x14,507);put(m,descriptor+0x28,0xdeadbeef)
        before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m)
        for j,value in enumerate((s.r[14],s.r[13],s.r[12],s.r[11],s.r[10],s.r[9],s.r[8],s.pr),1):put(wanted,s.r[15]-j*4,value)
        regs=list(s.r);regs[0]=0;regs[3]=v.load(m,0x8c64aa84);regs[4]=descriptor;regs[5]=META
        r,path,a,s,f=fast.compare(m,s,f,c);n=0
        assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n+=3
        assert a==wanted and list(s.r)==regs and s.sr==(before.sr&~1) and s.pr==s.pc==before.pr;n+=4
        assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==328*channel and r['final_gbr']==c['gbr'];n+=4
        assert len(r['boundaries'])==1 and not r['cache_purge_sequence'];n+=2
        seen.update(path)
        for j,pc in enumerate(path):
            text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
        checks+=n;records.append(dict(channel=channel,count=hex(count),mode=mode,sr=hex(sr),independent_checks=n))
    body=next(r for r in v.manifest['routines'] if r['address']==hex(ENTRY));missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
    files=[Path(__file__),Path(v.__file__),P/'write_watch_filesystem_directory.py',v.MP,P/'analysis/movie_filesystem_directory_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=[dict(address=hex(ENTRY),static_rows=55,executed_rows=55-len(missing),executed_outcomes=8-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges))],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Signedcounts0,-1,INT_MIN,twochannels/fourCPUFPUprofiles. Independent entireRAM/exacteightstackwrites/allGPR/SR/PR/FPU/special effects. Delayslot loads directory destination evenonzero path;poison destinationvalue neverdereferenced. Positive loop qualification separate;no supplied downstream read/lookup result.')
    (P/'analysis/movie_filesystem_directory_counts_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('DIRECTORY COUNTS PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
