"""Independent link lookup effects with original interface selection/callbacks."""
import hashlib,itertools,json
from pathlib import Path
from movie_filesystem_directory_session import Session,v,tasks
P=v.P;ENTRY=0x8c64b1c0
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,a,value):m[a&0xffffff:(a&0xffffff)+4]=(value&0xffffffff).to_bytes(4,'little')
def main():
    assert v.effects.DLL.name=='movie_filesystem_directory.dll'
    ram=v.seed_ram();records=[];seen=set();checks=0
    profiles=((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))
    for channel,endian in itertools.product((0,7),(0,1)):
        base=Session(ram,channel=channel,endian=endian);saved=bytes(base.m);cpu=bytes(base.s);fp=bytes(base.f)
        for index,pattern,(mode,sr) in itertools.product((0,1,255,256,507,511,0x80000000,0xffffffff),(0,0x8001,0xfffa,0xffff),profiles):
            m=bytearray(saved);s=type(base.s).from_buffer_copy(cpu);f=type(base.f).from_buffer_copy(fp);c=v.Input(dict(base.c));c.update(entry=ENTRY,mode=mode,sr=sr)
            s.pc=ENTRY;s.pr=0x8cfed000;s.sr=sr;s.r[4]=channel;s.r[5]=index;f.fpscr=0x40001|(mode<<19)
            for bank in range(2):
                for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
            descriptor=base.descriptor;table=v.load(m,descriptor+0x24);offset=(index<<1)&0xffffffff;target=(table+offset)&0xffffffff
            m[target&0xffffff:(target&0xffffff)+2]=pattern.to_bytes(2,'big' if endian else 'little')
            before=type(s).from_buffer_copy(s);f_before=bytes(f);wanted=bytearray(m)
            put(wanted,s.r[15]-8,index);put(wanted,s.r[15]-4,descriptor)
            regs=list(s.r);regs[0]=pattern;regs[1]=pattern|0xffff0000 if (pattern&0x80 if endian else pattern&0x8000) else pattern
            regs[2]=v.load(m,0x8c64b1e8);regs[3]=328;regs[4]=target;regs[5]=offset
            r,path,a,s,f=tasks.fast.compare(m,s,f,c);n=0
            assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n+=3
            assert a==wanted and list(s.r)==regs and s.sr==((before.sr&~1)|(index>>31)) and s.pr==s.pc==before.pr;n+=4
            assert bytes(f)==f_before and r['final_mach']==c['mach'] and r['final_macl']==328*channel and r['final_gbr']==c['gbr'];n+=4
            targets=[b['target'] for b in r['boundaries']];assert targets==[hex(ENTRY),'0x8c66080c' if endian else '0x8c660816'] and not r['cache_purge_sequence'];n+=2
            checks+=n;seen.update(path);records.append(dict(channel=channel,endian=endian,index=hex(index),value=hex(pattern),mode=mode,sr=hex(sr),independent_checks=n))
        print('DIRECTORY LOOKUP',channel,endian,len(records),flush=True)
    body=next(r for r in v.manifest['routines'] if r['address']==hex(ENTRY));missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};assert not missing and not body['conditional_edges']
    files=[Path(__file__),P/'movie_filesystem_directory_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_directory_task_fixtures.py',Path(v.__file__),P/'write_watch_filesystem_directory.py',v.MP,P/'analysis/movie_filesystem_directory_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=[dict(address=hex(ENTRY),static_rows=16,executed_rows=16,executed_outcomes=0,missing_rows=[],missing_outcomes=[])],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual initializer/selector own callback pointers.16row linklookup tailcalls realBE/LEreaders. Independent entireRAM/exacttwo stackwrites/allGPR/SR/PR/FPU/special effects;twochannels,fourprofiles,wordpatterns,index-boundaries andunsignedshift/addwrap. Coherent mappedRAM includingdeclared negative-offset input,not card-index/hardware validation. No callbackresult supplied;no newcredit for existing16bit readers.')
    (P/'analysis/movie_filesystem_directory_lookup_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('DIRECTORY LOOKUP PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
