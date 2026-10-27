"""Actual API queue -> submission -> original response completion, no token injection."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_submission_fixtures as fx
import movie_input_submission_task_fixtures as calls
v=fx.v;P=v.P;put=fx.put

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=v.seed_ram();records=[];checks=0;comparisons=0;smoke='--smoke' in sys.argv
    for unit,entry,bank,order,mode in itertools.product((0,6,12,18),(0x8c660494,0x8c6604e6),(0,1),(0,1),(0,7)):
        c,m,s,f=fx.setup(ram,entry,[unit,0x89abcdef,0x12345678,0x55555555],mode)
        fx.configure(c,m,bank=bank,buffer_bank=bank,order=order)
        rec=fx.CTX+0x28+unit*44;put(m,fx.CTX+0xf90+unit,1,1)
        put(m,rec,1,1);put(m,rec+4,9,1);put(m,s.r[15],0x2468ace0)
        assert [v.load(m,rec+off) for off in (0x18,0x1c,0x20)]==[0,0,0]
        original_f=bytes(f);phases=[]
        def run(target,args=None):
            nonlocal m,s,f,comparisons,checks
            r,path,m,s,f=calls.call(m,s,f,c,target,args)
            assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
            assert bytes(f)==original_f
            c['startup_overrides']={0x5f6800+j*4:value for j,value in enumerate(r['startup_registers'])}
            comparisons+=1;checks+=4
            phases.append(dict(entry=hex(target),steps=len(path),tokens=[v.load(m,rec+off) for off in (0x18,0x1c,0x20)],targets=[b['target'] for b in r['boundaries']]))
            return r,path
        r,path=run(entry,[unit,0x89abcdef,0x12345678,0x55555555]);serial=s.r[0]
        assert serial==1 and [v.load(m,rec+off) for off in (0x18,0x1c,0x20)]==[serial,0,0];checks+=2
        command=10 if entry==0x8c660494 else 11
        assert v.sample(m,rec+6,2)==bytes([command,0]);checks+=1
        r,path=run(fx.SUBMIT)
        assert [v.load(m,rec+off) for off in (0x18,0x1c,0x20)]==[0,serial,0];checks+=1
        assert v.sample(m,rec+6,2)==bytes([0,command]) and v.load(m,fx.CTX+0xfc0)==0;checks+=2
        assert r['startup_registers'][(0x5f6c04-0x5f6800)//4]==(fx.FRAME if not bank else fx.SECOND_FRAME)&0x1fffffff;checks+=1
        assert r['startup_registers'][(0x5f6c18-0x5f6800)//4]==1;checks+=1
        # Declared device-side observation: transport busy clears and raw bytes
        # arrive. No queue/inflight/completed token, saved task frame, parser
        # return or callback result is modified here.
        c['startup_overrides'][0x5f6c18]=0
        current=v.load(m,fx.CTX+0x10b4);rx=v.load(m,fx.CTX+0xfe8+(1-current)*96+unit*4)
        header=bytes([5,0x20,0,30]);payload=bytes(range(120))
        m[rx&0xffffff:(rx&0xffffff)+124]=(header[::-1] if order else header)+payload
        before_tokens=[v.load(m,rec+off) for off in (0x18,0x1c,0x20)]
        assert before_tokens==[0,serial,0];checks+=1
        c['scan_samples']=[0x1122]
        r,path=run(0x8c646654,[0,0,0,0])
        assert [v.load(m,rec+off) for off in (0x18,0x1c,0x20)]==[0,0,serial];checks+=1
        assert v.load(m,fx.CTX+0xfc0)==1 and m[fx.LOCK&0xffffff]==0;checks+=2
        targets=phases[-1]['targets']
        assert '0x8c65fe14' in targets and '0x8c6461ae' in targets and '0x8c65f1ec' in targets;checks+=3
        assert v.sample(m,rec+8,4)==header;checks+=1
        records.append(dict(unit=unit,request_entry=hex(entry),command=command,bank=bank,order=order,mode=mode,serial=serial,response_pointer=hex(rx),raw_header=list(header),phases=phases,completed_token_written_only_by_original_response=True))
        if len(records)%8==0:print('SUBMISSION LIFECYCLE',len(records),comparisons,flush=True)
        if smoke:break
    if smoke:print('SUBMISSION LIFECYCLE SMOKE PASS',comparisons,checks);return
    files=[Path(__file__),Path(fx.__file__),Path(calls.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_submission_connection.json',v.effects.DLL]
    report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='64three-phase actual requestAPI->65efda submit->646654 responsepump lifecycles. Original API creates queued ID, original submission moves it to inflight and writes physical buffer/start registers, original response publishes completed ID and rearms. No completed-token or saved-task-frame injection. External busy-clear/raw response bytes are declared device observations; existing startup register bank is storage, not automatic Maple DMA. Does not yet demonstrate original active task parser continuation, physical controller acquisition/timing or full game.')
    (P/'analysis/movie_input_submission_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('SUBMISSION LIFECYCLE PASS',len(records),comparisons,checks,flush=True)
if __name__=='__main__':main()
