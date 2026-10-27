"""Root validation with real synchronous SDK helpers and independent effects."""
import hashlib,itertools,json
from pathlib import Path
from movie_filesystem_root_session import Session,v,tasks
P=v.P;ROOT_CHECK=0x8c64a4b2;STATUS=0x8c660dac
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(n*8))-1)).to_bytes(n,'little')
def scenarios():
    for kind in ('inactive','absent','function_off','busy','disabled','good','latched1','latched_ff'):yield dict(kind=kind)
    for offset,value in itertools.product(range(16),(0,0x54,0x56,0xff)):yield dict(kind='bad_signature',offset=offset,value=value)
def main():
    assert v.effects.DLL.name=='movie_filesystem_root.dll'
    ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
    for channel,(mode,sr) in itertools.product((0,7),((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))):
        base=Session(ram,channel=channel);saved=bytes(base.m);cpu=bytes(base.s);fpu=bytes(base.f)
        for case in scenarios():
            m=bytearray(saved);s=type(base.s).from_buffer_copy(cpu);f=type(base.f).from_buffer_copy(fpu);c=v.Input(dict(base.c))
            c.update(mode=mode,sr=sr);s.sr=sr;f.fpscr=0x40001|(mode<<19)
            for bank in range(2):
                for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
            kind=case['kind'];active=kind!='inactive';present=kind!='absent';feature=kind!='function_off';busy=kind=='busy';enabled=kind!='disabled'
            descriptor=base.descriptor;root=v.load(m,descriptor+0x20)
            put(m,descriptor+2,int(active),1);put(m,descriptor+8,int(enabled),1);put(m,descriptor+0xb,0xa5,1);put(m,descriptor+0xc,0x5a,1)
            # 646856 returns the24byte decoded record,not the separate44byte
            # SDK transfer status. Keep both availability inputs coherent.
            put(m,base.d['header'],int(present),1);put(m,base.d['status'],int(present),1)
            put(m,base.d['status']+0xc,int(busy),1);put(m,base.d['info'],2 if feature else 0)
            latch=1 if kind=='latched1' else 0xffffffff if kind=='latched_ff' else 0
            put(m,0x8c89cf90+channel*4,latch)
            data=bytearray((j*37+channel)&255 for j in range(512));data[:16]=b'U'*16;data[16]=0
            if kind=='bad_signature':data[case['offset']]=case['value']
            m[root&0xffffff:(root&0xffffff)+512]=data
            unavailable=not active or not present or not feature or busy or not enabled
            if unavailable:put(m,descriptor+0x20,0)
            c['entry']=ROOT_CHECK;s.pc=ROOT_CHECK;s.pr=0x8cfed000;s.r[4]=channel
            before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m)
            cleanup=active and (not present or not feature)
            if cleanup:
                for off in (8,9,10,11,12):put(wanted,descriptor+off,0,1)
                put(wanted,descriptor+4,0)
            if kind=='bad_signature':put(wanted,descriptor+0xb,0,1);put(wanted,descriptor+0xc,0,1)
            expected=0xffffff01 if unavailable else 0xffffff03 if kind=='bad_signature' or latch else 0
            result,path,a,s,f=tasks.fast.compare(m,s,f,c);n=0
            assert result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match'],(channel,mode,case,{k:result[k] for k in ('status','stop_pc','full_state_match','ordered_boundaries_match')});n+=3
            # Guest ABI scratch is independently bounded to64bytes; all other
            # RAM changes must match only the documented cache invalidation.
            sp=before.r[15]&0xffffff;wanted[sp-64:sp]=a[sp-64:sp]
            assert a==wanted and s.r[0]==expected and s.pc==s.pr==before.pr;n+=3
            assert list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=2
            assert (s.sr&~1)==(before.sr&~1) and result['final_mach']==c['mach'] and result['final_gbr']==c['gbr'];n+=3
            targets=[b['target'] for b in result['boundaries'] if b['target'] in ('0x8c64a4b2','0x8c660dac','0x8c660dec','0x8c64850a')]
            wanted_targets=['0x8c64a4b2']+(['0x8c660dac'] if active else [])+(['0x8c64850a'] if cleanup else ['0x8c660dec'] if active else [])
            assert targets==wanted_targets and not result['cache_purge_sequence'];n+=2
            if not unavailable:
                assert s.r[5]==root+(case['offset'] if kind=='bad_signature' else 16);n+=1
                assert s.sr&1==int(kind=='good');n+=1
            elif kind in ('inactive','busy','disabled'):assert s.sr&1;n+=1
            else:assert not(s.sr&1);n+=1
            seen.update(path)
            for j,pc in enumerate(path):
                text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
                if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
            checks+=n;records.append(dict(channel=channel,mode=mode,sr=hex(sr),input=case,expected_result=hex(expected),independent_checks=n))
        print('ROOT DIRECT',channel,mode,len(records),flush=True)
    body=v.manifest['routines'][0]
    status=json.loads((P/'analysis/movie_countdown_translation.json').read_text());status=next(r for r in status['routines'] if r['address']==hex(STATUS))
    coverage=[]
    for r in (body,status):
        missing=set(r['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,r['conditional_edges']))-edges
        assert not missing and not missing_edges,(r['address'],missing,missing_edges)
        coverage.append(dict(address=r['address'],static_rows=r['instruction_rows'],executed_rows=r['instruction_rows'],executed_outcomes=len(r['conditional_edges']),missing_rows=[],missing_outcomes=[]))
    files=[Path(__file__),P/'movie_filesystem_root_session.py',P/'movie_input_interface_session.py',P/'movie_filesystem_root_task_fixtures.py',Path(v.__file__),P/'write_watch_filesystem_root.py',v.MP,P/'analysis/movie_filesystem_root_connection.json',P/'analysis/movie_countdown_translation.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Direct root ABI with coherent constructor-derived objects anddeclared SDK presence/function/busy/cache/latch inputs;real status/readiness/cleanup callees,no supplied callee result.16signature byte positions,fourbadvalues,twochannels/fourCPUFPUprofiles,availability exits andlatch state. Independent nonstackRAM/cache effects,64byte ABI scratchbound,return/register/FPU/flags/callselection;strict original/native entirestate includingstack remainsunchanged. Root andexistingunregistered status helper cover allrows/outcomes. Not natural device acquisition orwholegame/hardware qualification.')
    (P/'analysis/movie_filesystem_root_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ROOT DIRECT PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
