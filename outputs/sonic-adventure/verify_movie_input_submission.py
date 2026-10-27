"""Original command/submission paths, independent packet and register oracles."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_submission_fixtures as fx
import verify_movie_maple as packet
v=fx.v;P=v.P;put=fx.put

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def swap(value):return int.from_bytes(value.to_bytes(4,'little'),'big')

def packet_expected(m,before,unit,command,value=0):
    # Reuse the separately established packet-format oracle. The scratch payload
    # below belongs only to this expectation; native/original inputs are untouched.
    w=bytearray(m);s=type(before).from_buffer_copy(before)
    s.r[4]=unit;s.r[5]=command;s.r[6]=0x8cc7e000 if command==9 else 0
    s.r[7]=int(command==9);s.r[15]=0x8cc7e100
    put(w,s.r[6] if command==9 else 0x8cc7e000,value)
    put(w,s.r[15],0);put(w,s.r[15]+4,0)
    return packet.packet_oracle(w,s,dict(entry=packet.COMMAND,second_words=0))

def prepare_expected(m,before):
    w=bytearray(m);padding=v.load(w,fx.CTX+0x10e4)
    if 0<padding<0x80000000:
        pointer=v.load(w,fx.CTX+0xfcc)+4*v.load(w,fx.CTX+0xfd8)
        for j in range(padding):put(w,pointer+4*j,0x700)
        put(w,fx.CTX+0xfcc,pointer);put(w,fx.CTX+0xfd8,padding)
        put(w,fx.CTX+0xfdc,v.load(w,fx.CTX+0xfdc)+padding*4)
    for unit in range(24):
        rec=fx.CTX+0x28+unit*44
        if not v.sample(w,fx.CTX+0xf90+unit,1)[0] or v.sample(w,rec+6,1)[0]:continue
        state=v.sample(w,rec+4,1)[0]
        if state not in (1,2,3,4,9):continue
        value=v.load(w,fx.CTX+1100+unit*120)
        if value&1:value=1
        w,_=packet_expected(w,before,unit,state,value)
    pointer=v.load(w,fx.CTX+0xfcc);put(w,pointer,v.load(w,pointer)|0x80000000)
    return w

def submit_expected(m,before):
    w=prepare_expected(m,before)
    rate=v.load(m,fx.CTX+0xfac);last=v.load(m,fx.CTX+0xfb0)
    if rate!=last:
        put(w,fx.CTX+0xfb0,rate)
        clock=v.load(m,v.load(m,0x8c65f15c));product=(clock*1000)&0xffffffff
        signed=product if product<0x80000000 else product-0x100000000
        quotient=abs(signed)//20
        if signed<0:quotient=-quotient
        put(w,fx.CTX+0x10c0,(quotient<<16)&0xffffffff if rate else 0xc3500000)
    bank=v.load(m,fx.CTX+0x10b8);buffers=[v.load(m,fx.CTX+0x10a8+j*4) for j in range(2)]
    put(w,fx.CTX+0x10b0,buffers[bank]);put(w,fx.CTX+0x10b8,bank^1)
    put(w,fx.CTX+0xfcc,buffers[bank^1]);put(w,fx.CTX+0x10b4,v.load(m,fx.CTX+0x10b4)^1)
    put(w,fx.CTX+0xfd8,0);put(w,fx.CTX+0xfdc,0);put(w,fx.CTX+0xfc0,0)
    for unit in range(24):
        rec=fx.CTX+0x28+unit*44
        if not v.sample(w,fx.CTX+0xf90+unit,1)[0]:continue
        put(w,rec+7,v.sample(w,rec+6,1)[0],1);put(w,rec+6,0,1)
        put(w,rec+0x1c,v.load(w,rec+0x18));put(w,rec+0x18,0)
    put(w,fx.LOCK,0,1)
    return w

def projection_equal(actual,wanted):
    for address,length in ((fx.CTX,0x2000),(fx.FRAME,0x10000),(fx.SECOND_FRAME,0x10000)):
        assert v.sample(actual,address,length)==v.sample(wanted,address,length),(hex(address),'independent packet/context projection')

def main():
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
    def note(kind,data,r,path,n):
        nonlocal checks
        checks+=n;seen.update(path)
        for j,pc in enumerate(path):
            text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
        records.append(dict(kind=kind,input=data,status=r['status'],steps=len(path),independent_checks=n))
        if len(records)%50==0:print('SUBMISSION',kind,len(records),flush=True)
    def common(m,s,f,c):
        before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c)
        assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
        assert s.r[15]==before.r[15] and s.pc==s.pr==before.pr
        assert list(s.r)[8:15]==list(before.r)[8:15] and bytes(f)==fp
        assert r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and s.sr&~0x301==before.sr&~0x301
        return before,r,path,a,s,f,10
    entries=((fx.FEATURES,9),(fx.FOUR,4),(fx.THREE,3),(fx.TWO,2))
    for (entry,command),unit,busy,order,(mode,sr) in itertools.product(entries,(0,1,20,23),(0,0xff),(0,1),fx.CONFIGS[:2]):
        c,m,s,f=fx.setup(ram,entry,[unit,0x89abcdef,0x12345678,0x55555555],mode,sr);fx.configure(c,m,order=order)
        fx.record(m,unit,command=busy);before=type(s).from_buffer_copy(s)
        expected,result=packet_expected(m,before,unit,command,0x89abcdef)
        before,r,path,a,s,f,n=common(m,s,f,c);projection_equal(a,expected);n+=3
        assert s.r[0]==result and r['cache_purge_sequence']==[];n+=2
        event=next(b for b in r['boundaries'] if b['target']=='0x8c65f810')
        assert event['arguments'][:2]==[unit,command] and event['arguments'][3]==int(command==9);n+=2
        note('wrapper',dict(entry=hex(entry),unit=unit,busy=busy,order=order,mode=mode),r,path,n)
        if smoke:break
    states=(0,1,2,3,4,9,10,11,12,13,255,5)
    for state,unit,feature,mode in itertools.product(states,(0,1,20,23),(0,0xdeadbeef),(0,7)):
        c,m,s,f=fx.setup(ram,fx.PREPARE,mode=mode);fx.configure(c,m,order=mode&1,padding=unit&1)
        fx.record(m,unit,command=0,state=state);put(m,fx.CTX+1100+unit*120,feature)
        before=type(s).from_buffer_copy(s);expected=prepare_expected(m,before)
        before,r,path,a,s,f,n=common(m,s,f,c);projection_equal(a,expected);n+=3
        assert s.r[0]==0 and r['cache_purge_sequence']==[];n+=2
        selected=state in (1,2,3,4,9)
        assert sum(b['target']=='0x8c65f810' for b in r['boundaries'])==int(selected);n+=1
        note('prepare',dict(state=state,unit=unit,feature=feature,mode=mode),r,path,n)
        if smoke:break
    for lock in range(256):
        mode,sr=fx.CONFIGS[lock%len(fx.CONFIGS)];c,m,s,f=fx.setup(ram,mode=mode,sr=sr);fx.configure(c,m,lock=lock);fx.record(m,0)
        before,r,path,a,s,f,n=common(m,s,f,c)
        assert s.r[0]==(0xfffffffe if lock else 0);n+=1
        assert a[fx.LOCK&0xffffff]==(lock|0x80 if lock else 0) and r['cache_purge_sequence']==[fx.LOCK];n+=2
        assert v.sample(a,fx.LOCK-3,3)==v.sample(m,fx.LOCK-3,3) and v.sample(a,fx.LOCK+1,4)==v.sample(m,fx.LOCK+1,4);n+=2
        rec=fx.CTX+0x28
        assert v.load(a,rec+0x18)==(7 if lock else 0) and v.load(a,rec+0x1c)==(0x89abcdef if lock else 7) and v.load(a,rec+0x20)==0x12345678;n+=3
        assert [e for e in r['startup_events'] if e[0]==28]==([] if lock else [[28,fx.MMIO+4,fx.FRAME&0x1fffffff,4],[28,fx.MMIO+0x18,1,4]]);n+=1
        note('mutex',dict(lock=lock,mode=mode,sr=hex(sr)),r,path,n)
        if smoke:break
    for status,busy in itertools.product((0,2,1),(0,1,2,0xffffffff)):
        c,m,s,f=fx.setup(ram);fx.configure(c,m,status=status,busy=busy);fx.record(m,1)
        before,r,path,a,s,f,n=common(m,s,f,c)
        result=0xfffffffd if status!=1 else 0xffffffff if busy&1 else 0
        assert s.r[0]==result and r['cache_purge_sequence']==([fx.LOCK] if not result else []);n+=2
        if result:
            projection_equal(a,m);n+=3
            assert not any(e[0]==28 for e in r['startup_events']);n+=1
        note('readiness',dict(status=status,busy=busy),r,path,n)
        if smoke:break
    for bank,buffer_bank,rate,last,clock,(mode,sr) in itertools.product((0,1),(0,1),(0,60),(0,60),(60,0x80000001),fx.CONFIGS[:2]):
        c,m,s,f=fx.setup(ram,mode=mode,sr=sr);fx.configure(c,m,bank=bank,buffer_bank=buffer_bank,rate=rate,last_rate=last)
        put(m,v.load(m,0x8c65f15c),clock);put(m,fx.CTX+0x10d0,0x1234)
        for unit in range(24):fx.record(m,unit,present=int(unit%3!=2),command=(unit+1)&255,token=0x1000+unit)
        before=type(s).from_buffer_copy(s);expected=submit_expected(m,before)
        before,r,path,a,s,f,n=common(m,s,f,c);projection_equal(a,expected);n+=3
        assert s.r[0]==0 and a[fx.LOCK&0xffffff]==0 and r['cache_purge_sequence']==[fx.LOCK];n+=3
        writes=[]
        if rate!=last:writes.extend([[28,fx.MMIO+0x14,0,4],[28,fx.MMIO+0x80,v.load(expected,fx.CTX+0x10c0)|0x1234,4],[28,fx.MMIO+0x14,1,4]])
        writes.extend([[28,fx.MMIO+4,(fx.FRAME if not bank else fx.SECOND_FRAME)&0x1fffffff,4],[28,fx.MMIO+0x18,1,4]])
        assert [e for e in r['startup_events'] if e[0]==28]==writes;n+=1
        assert v.sample(a,0x8c5f6c00,0x100)==v.sample(m,0x8c5f6c00,0x100);n+=1
        for unit in range(24):assert v.load(a,fx.CTX+0x28+unit*44+0x20)==0x12345678
        n+=24
        note('submit_banks_rate',dict(bank=bank,buffer_bank=buffer_bank,rate=rate,last=last,clock=clock,mode=mode),r,path,n)
        if smoke:break
    for status,busy,lock,(mode,sr) in itertools.product((0,1),(0,1),(0,1),fx.CONFIGS):
        c,m,s,f=fx.setup(ram,0x8c65f1aa,mode=mode,sr=sr);fx.configure(c,m,status=status,busy=busy,lock=lock)
        before,r,path,a,s,f,n=common(m,s,f,c)
        result=0xfffffffd if status else 0xffffffff if busy else 0xfffffffe if lock else 0
        assert s.r[0]==result and v.load(a,fx.CTX+0xfc0)==status;n+=2
        assert r['cache_purge_sequence']==([fx.LOCK] if not status and not busy else []);n+=1
        assert a[fx.LOCK&0xffffff]==(lock|0x80 if not status and not busy else lock);n+=1
        assert not any(e[0]==28 for e in r['startup_events']);n+=1
        note('canonical_preflight',dict(status=status,busy=busy,lock=lock,mode=mode),r,path,n)
        if smoke:break
    if smoke:print('SUBMISSION SMOKE PASS',len(records));return
    coverage=[]
    for body in v.manifest['routines']:
        missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
        assert not missing and not missing_edges,(body['address'],sorted(missing),sorted(missing_edges))
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges']),missing_rows=[],missing_outcomes=[]))
    files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),P/'input_submission_reference_cpu.py',Path(packet.__file__),v.MP,P/'analysis/movie_input_submission_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Six original bodies351rows/46outcomes. Complete modeled CPU/FPU/RAM/device/ordered-boundary comparisons, independent packet/context/buffer projection, all256TASbytes and purge/neighbors, actual canonical callees, readiness/error priority, all24record tokens and command bytes, bank flips, signed clock quotient and observed MMIO write order. Existing startup bus is explicit register storage only; no automatic device DMA, acquired response, task resumption or complete game claim. Old inflight/completed values are fixture history; queued API lifecycle is separate. New MMIO guard covers only four aligned32bit observed addresses and RAM. No source wait bound or fabricated callee results.')
    report.update(executed_adapter_rows=384,executed_adapter_outcomes=52,new_body_rows=351,new_body_outcomes=46,canonical_preflight_rows=33,canonical_preflight_outcomes=6,canonical_duplicate_credit=0)
    report['scope']+=' Canonical33row/6outcome preflight reuses its original instructions with observed-MMIO guard, zero duplicate credit.'
    (P/'analysis/movie_input_submission_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('SUBMISSION PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
