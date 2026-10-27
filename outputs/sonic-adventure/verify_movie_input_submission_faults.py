"""Reached bus failures preserve original prefixes; signed timing-product edges."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_input_submission as direct
fx=direct.fx;v=fx.v;P=v.P;put=fx.put

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=v.seed_ram();records=[];checks=0
    for entry,kind,(mode,sr) in itertools.product((fx.SUBMIT,0x8c65f1aa),('null_bus','misaligned'),fx.CONFIGS):
        c,m,s,f=fx.setup(ram,entry,mode=mode,sr=sr);fx.configure(c,m,status=int(entry==fx.SUBMIT));rec=fx.record(m,0)
        if kind=='null_bus':c['null_bus']=True
        else:put(m,fx.CTX+0x10f4,fx.MMIO+2)
        before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
        assert r['status']==(-2 if kind=='null_bus' else -3) and r['full_state_match'] and r['ordered_boundaries_match']
        assert r['stop_pc']==('0x8c65effe' if entry==fx.SUBMIT else '0x8c65f1c2');n+=1
        assert v.sample(a,fx.CTX,0x2000)==v.sample(m,fx.CTX,0x2000) and a[fx.LOCK&0xffffff]==0;n+=2
        assert s.r[15]==before.r[15]-(20 if entry==fx.SUBMIT else 4) and bytes(f)==fp;n+=2
        assert not r['cache_purge_sequence'] and not any(e[0]==28 for e in r['startup_events']);n+=2
        checks+=n;records.append(dict(kind=kind,entry=hex(entry),mode=mode,status=r['status'],stop_pc=r['stop_pc'],independent_checks=n))
    for rejected,(mode,sr) in itertools.product((fx.MMIO+4,fx.MMIO+0x14,fx.MMIO+0x18,fx.MMIO+0x80),fx.CONFIGS):
        c,m,s,f=fx.setup(ram,mode=mode,sr=sr);fx.configure(c,m,rate=1,last_rate=0);rec=fx.record(m,0)
        put(m,v.load(m,0x8c65f15c),60);c.update(reject_bus=rejected,bus_status=-7)
        r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
        assert r['status']==-7 and r['full_state_match'] and r['ordered_boundaries_match']
        assert [v.load(a,rec+off) for off in (0x18,0x1c,0x20)]==[7,0x89abcdef,0x12345678];n+=1
        before_lock=rejected==fx.MMIO+0x18
        assert a[fx.LOCK&0xffffff]==(0 if before_lock else 0x80) and v.load(a,fx.CTX+0xfc0)==1;n+=2
        assert r['cache_purge_sequence']==([] if before_lock else [fx.LOCK]);n+=1
        assert v.load(a,fx.CTX+0x10b8)==0 and v.load(a,fx.CTX+0x10b4)==0;n+=2
        checks+=n;records.append(dict(kind='rejected_bus',address=hex(rejected),mode=mode,status=r['status'],stop_pc=r['stop_pc'],independent_checks=n))
    for clock,(mode,sr) in itertools.product((0x7fffffff,0x200000,0x80000000,0xffffffff),fx.CONFIGS):
        c,m,s,f=fx.setup(ram,mode=mode,sr=sr);fx.configure(c,m,rate=1,last_rate=0);fx.record(m,23)
        put(m,v.load(m,0x8c65f15c),clock);put(m,fx.CTX+0x10d0,0x1234)
        expected=direct.submit_expected(m,s);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
        assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
        direct.projection_equal(a,expected);n+=3
        product=(clock*1000)&0xffffffff;num=product if product<0x80000000 else product-0x100000000
        quotient=abs(num)//20*(-1 if num<0 else 1);timing=(quotient<<16)&0xffffffff
        assert v.load(a,fx.CTX+0x10c0)==timing;n+=1
        assert [e for e in r['startup_events'] if e[0]==28 and e[1]==fx.MMIO+0x80]==[[28,fx.MMIO+0x80,timing|0x1234,4]];n+=1
        checks+=n;records.append(dict(kind='signed_product',clock=hex(clock),product=hex(product),signed_product=num,quotient=quotient,mode=mode,status=r['status'],independent_checks=n))
    files=[Path(__file__),Path(fx.__file__),Path(direct.__file__),Path(v.__file__),v.MP,P/'analysis/movie_input_submission_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Reached null bus/misalignment and rejected MMIO access prefixes retain exact original CPU/FPU/RAM/device boundaries and queued/inflight/completed IDs. No fabricated successful bus result or postfailure token transition. Four raw clock inputs cover negative, wrapped and zero 32bit products; independent truncation-toward-zero quotient/shift and register OR. Existing host error policy and coherent declared inputs, not physical fault vectors or controller DMA.')
    (P/'analysis/movie_input_submission_faults_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('SUBMISSION FAULTS PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
