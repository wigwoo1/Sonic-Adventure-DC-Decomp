"""Full original/native state checks plus independent pool/flag initialization oracles."""
import collections,hashlib,itertools,json
from pathlib import Path
import collision_setup_fixtures as fixture

P=fixture.P


def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def configurations():
    for mode in range(8):
        yield dict(entry=fixture.WORLD_INIT,mode=mode,kind='world')
    for mode,a,b in itertools.product((0,4),range(3),range(3)):
        yield dict(entry=fixture.DEBUG_INIT,mode=mode,mode_input=a,variant_input=b,kind='debug')
    for entry,count,bank,view in itertools.product((0x8c02ce80,0x8c02ceca),(2,3,16),(0,4),(0x0c000000,0x8c000000,0xac000000)):
        yield dict(entry=entry,mode=bank,arguments=[view|(fixture.DIRECT_POOL&0xffffff),count],kind='pool',count=count)
    for index,mode in itertools.product((0,1,2,3,255,256),(0,4)):
        yield dict(entry=0x8c0509b8,mode=mode,arguments=[index],kind='input_flag',index=index&255)
    for entry,mode in itertools.product((0x8c0869e0,0x8c08a05c),range(8)):
        yield dict(entry=entry,mode=mode,kind='leaf')
    for entry in (fixture.WORLD_INIT,fixture.DEBUG_INIT):
        yield dict(entry=entry,kind='guard',bad='stack')
    for entry in (0x8c02ce80,0x8c02ceca):
        yield dict(entry=entry,kind='guard',arguments=[0x8b000000,2])


def main():
    ram=fixture.v.seed_ram();seen=set();edges=set();records=[];checks=0;counts=collections.Counter();oracles=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    for case_id,c in enumerate(configurations()):
        c=dict(c,case_id=case_id);m,s,f,cfg=fixture.prepare(ram,c)
        before=bytes(m);initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
        result,path,actual,s,f=fixture.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,-1,-3),'complete or reached existing unsupported PR numeric mode/mapped-memory guard')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/device/queue/boundaries')
        if result['status']==0:
            require(s.pc==s.pr==initial_s.pr and s.r[15]==initial_s.r[15],'return/stack')
            require(list(s.r[8:15])==list(initial_s.r[8:15]),'callee-saved registers')
        if c['kind']=='world':
            expected=bytearray(before)
            for address in (0x8c73e27c,0x8c73e400,0x8c02d548,0x8c73edc4):fixture.put(expected,address,0)
            for j in range(fixture.COUNT_A):fixture.put(expected,fixture.POOL_A+64*j+8,0)
            fixture.put(expected,initial_s.r[15]-4,initial_s.pr)
            if not initial_f.fpscr&0x80000:
                width=8 if initial_f.fpscr&0x100000 else 4
                bank=(initial_f.fpscr>>21)&1
                for j in range(fixture.COUNT_B):
                    fixture.put(expected,fixture.POOL_B+64*j+0x14,0)
                    if width==8:fixture.put(expected,fixture.POOL_B+64*j+0x18,initial_f.banks[bank][5])
                for base,n,head in ((fixture.POOL_B,fixture.COUNT_B,0x8c02d540),(fixture.POOL_A,fixture.COUNT_A,0x8c02d544)):
                    fixture.put(expected,head,base)
                    for j in range(n):fixture.put(expected,base+64*j,base+64*(j+1) if j+1<n else 0)
                require(result['status']==0 and s.r[0]==1,'world initializer completes and returns1')
            else:require(result['status']==-1,'PR1 stops at reached FLDI0 after original integer resets')
            require(actual==expected,'independent whole-RAM pool/reset footprint including saved PR and SZ pair store')
            oracles+=1
        elif c['kind']=='debug':
            require(result['status']==0,'debug initializer uses integer instructions in every selected FP bank')
            require(fixture.get(actual,0x8c752bdc,1)==255 and fixture.get(actual,0x8c19de26,2)==0,'original debug byteFF and short clear')
            require(fixture.get(actual,0x8c19a8d8)==int(c['variant_input']!=0),'variant-present flag')
            require(fixture.get(actual,0x8c19a8dc)==(0x12345678 if c['variant_input']==0 else int(c['variant_input']!=1)),'variant secondary flag or untouched sentinel')
            require(bytes(f)==bytes(initial_f),'integer-only initializer preserves both FP banks and FPSCR')
            oracles+=1
        elif c['kind']=='pool':
            expected=bytearray(before);base=c['arguments'][0];n=c['count']
            fixture.put(expected,0x8c02d540 if c['entry']==0x8c02ce80 else 0x8c02d544,base)
            for j in range(n):fixture.put(expected,base+64*j,base+64*(j+1) if j+1<n else 0)
            if c['entry']==0x8c02ceca:fixture.put(expected,0x8c02d548,0)
            require(actual==expected,'independent pool links, terminating null, heads and untouched whole RAM')
            oracles+=1
        elif c['kind']=='input_flag':
            expected=bytearray(before);base=fixture.get(ram,0x8c050a24);other=fixture.get(ram,0x8c050a28)
            for address in ([base+c['index']] if c['index']<=1 else [base,other]):fixture.put(expected,address,1,1)
            require(actual==expected,'independent low-byte index flag selection and untouched RAM')
            oracles+=1
        elif c['kind']=='guard':require(result['status']==-3,'reached invalid stack/pool mapped-memory guard')
        seen.update(path)
        for i,pc in enumerate(path):
            text=fixture.v.ROWS.get(pc,'')
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')):
                j=i+(2 if '/s ' in text else 1)
                if j<len(path):edges.add((pc,path[j]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],original_rows=len(path),result=s.r[0],boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%6==0:print('SETUP DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in fixture.v.ANALYSIS_ROUTINES:
        wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),
          executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),
          missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    files=[Path(__file__),P/'collision_setup_fixtures.py',P/'collision_setup_reference.py',P/'write_watch_collision_setup.py',
           fixture.v.MP,P/'analysis/collision_setup_connection.json',fixture.v.effects.DLL,P/'source/sa_collision_setup.c']
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,
      status_counts=dict(counts),cases=records,coverage_by_routine=coverage,dll_sha256=digest(fixture.v.effects.DLL),
      new_original_routine_credit=0,sha256={str(p):digest(p) for p in files},
      scope='Two original initialization owners plus five canonical dependencies through full current ABI. '
      'Original flags/pools generated by actual code,not supplied callee results/frames. '
      'Independent whole-RAM world/pool/input flag oracles and debug control fields. '
      'Worldinitializer original counts192and8192,40hex recordstride. SZ1 writes real second FP word. '
      'PR1 stops after initial integer clears,not completion. Direct pool counts>=2 because original DT loop '
      'does not implement a zero/singleton pool. No fresh full game startup/hardware/GPU claim.')
    (P/'analysis/collision_setup_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('SETUP DIRECT PASS',len(records),checks,dict(counts),coverage,flush=True)


if __name__=='__main__':main()
