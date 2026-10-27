"""Original sixteen callback instructions with entire-state/RAM byte oracles."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_interface_fixtures as fx
v=fx.v;P=v.P

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def swap_low(value):return (value&0xffff0000)|((value&255)<<8)|((value>>8)&255)

def main():
    ram=v.seed_ram();records=[];seen=set();checks=0;smoke='--smoke' in sys.argv
    patterns=(0,1,0x7fff,0x8000,0xffff,0x12345678,0x80000000,0x7fa12345)
    for entry,pattern,offset,alias,(mode,sr) in itertools.product((fx.READ_BE,fx.READ_LE,fx.WRITE_BE,fx.WRITE_LE),patterns,(0,0xfffffffe),(0x8c000000,0xac000000),fx.CONFIGS):
        address=alias|(fx.DATA&0xffffff)|2;target=(address+offset)&0xffffffff
        c,m,s,f=fx.setup(ram,entry,[address,offset,pattern,0x89abcdef],mode,sr)
        fx.put(m,fx.DATA-8,0x89abcdef);fx.put(m,fx.DATA+8,0x76543210)
        fx.put(m,target,pattern,2);before=type(s).from_buffer_copy(s);fp=bytes(f)
        wanted=bytearray(m);regs=list(before.r);regs[4]=target
        if entry in (fx.READ_BE,fx.READ_LE):
            raw=pattern&65535;regs[1]=(raw if raw<0x8000 else raw|0xffff0000)
            if entry==fx.READ_BE:regs[1]=swap_low(regs[1])
            regs[0]=regs[1]&65535
        else:
            if entry==fx.WRITE_BE:regs[6]=swap_low(pattern)
            fx.put(wanted,target,regs[6],2)
        r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
        assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
        assert a==wanted and list(s.r)==regs and s.sr==before.sr and s.pc==s.pr==before.pr;n+=4
        assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==c['macl'] and r['final_gbr']==c['gbr'];n+=4
        assert not r['cache_purge_sequence'] and len(r['boundaries'])==1;n+=2
        seen.update(path);checks+=n;records.append(dict(entry=hex(entry),pattern=hex(pattern),offset=hex(offset),alias=hex(alias),mode=mode,sr=hex(sr),status=0,independent_checks=n))
        if len(records)%100==0:print('INTERFACE LEAF',len(records),flush=True)
        if smoke:break
    for entry,address,(mode,sr) in itertools.product((fx.READ_BE,fx.READ_LE,fx.WRITE_BE,fx.WRITE_LE),(0,0x8d000000),fx.CONFIGS[:2]):
        c,m,s,f=fx.setup(ram,entry,[address,0,0x89abcdef,0],mode,sr);before=type(s).from_buffer_copy(s);fp=bytes(f)
        r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
        assert r['status']==-3 and r['full_state_match'] and r['ordered_boundaries_match']
        assert a==m and s.r[4]==address and s.r[6]==(swap_low(before.r[6]) if entry==fx.WRITE_BE else before.r[6]);n+=3
        assert s.r[0]==before.r[0] and s.pr==before.pr and s.sr==before.sr and bytes(f)==fp;n+=4
        seen.update(path);checks+=n;records.append(dict(entry=hex(entry),invalid_address=hex(address),mode=mode,status=-3,stop_pc=r['stop_pc'],independent_checks=n))
        if smoke:break
    if smoke:print('INTERFACE LEAF SMOKE PASS',len(records));return
    coverage=[]
    for body in v.manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen}
        if body['address']=='0x8c6607fc':continue
        assert not missing,(body['address'],missing)
        assert not body['conditional_edges']
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=0,missing_rows=[],missing_outcomes=[]))
    files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_interface_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Four original16bit callbacks16rows/no conditional edges. Aligned coherent mapped-RAM operands, two aliases, offsetaddition including negativewrap, signed-load/raw32bit/swap-lowword patterns and fourCPU/FPUprofiles. Independent entireRAM/allGPR/SR/PR/FPU/special oracles:readzeroextends result;writepreservesR0,BEwriter swapslowbytes whilekeepingupper16. ReachedinvalidRAMprefixes preservepriorregistereffects;hosterrorpolicy is not physical exception delivery. No MMIO/unaligned physicalhardware or arbitrary callback contract claim.')
    (P/'analysis/movie_input_interface_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INTERFACE LEAF PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
