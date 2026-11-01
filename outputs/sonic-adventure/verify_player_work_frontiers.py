"""Reached numeric/mapping/empty-heap software frontiers; no physical trap claim."""
import collections,hashlib,json
from pathlib import Path
import player_work_lifecycle_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    base=q.fresh(q.v.seed_ram());records=[];checks=0
    def run(base,entry,args,label,expected=None):
        nonlocal checks
        m,s,f,c=base;r,path,m,s,f=q.invoke(m,s,f,c,entry,args)
        assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
        if expected is not None:assert r['status']==expected,(label,r['status'],r['stop_pc']);checks+=1
        else:assert r['status'] in (0,-1,-3),(label,r['status']);checks+=1
        records.append(dict(input=label,entry=hex(entry),status=r['status'],stop_pc=r['stop_pc'],steps=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        return (m,s,f,c)
    base=run(base,q.HEAP_INIT,(),'original_heap_init',0)
    base=run(base,q.POOL_INIT,(),'original_pool_init',0)
    for kind in ('mode1','mode2','mode3','mode5','mode6','mode7','rm2','rm3','bad_actor','bad_aux','bad_task','bad_stack','empty_state_heap','empty_aux_heap','empty_record_heap'):
        case=q.clone(base);case=run(case,q.CREATE,[3,2,0,0],kind+'_original_parent',0)
        m,s,f,c=case;task=s.r[0];actor=q.get(m,task+0x20);aux=q.get(m,task+0x24)
        q.put(m,actor+9,0,1);m[0x799920:0x799b20]=bytes(0x200)
        entry=q.INIT;args=[0,task];expected=None
        if kind.startswith('mode'):f.fpscr=0x40000|(int(kind[-1])<<19)
        elif kind.startswith('rm'):f.fpscr=0x40000|int(kind[-1])
        elif kind=='bad_actor':q.put(m,task+0x20,0x8b000000);expected=-3
        elif kind=='bad_aux':q.put(m,task+0x24,0x8b000000);expected=-3
        elif kind=='bad_task':args=[0,0x8b000000];expected=-3
        elif kind=='bad_stack':s.r[15]=0x8b000000;expected=-3
        elif kind=='empty_state_heap':q.put(m,q.get(m,0x8c098b60),0);expected=-3
        elif kind=='empty_aux_heap':
            q.put(m,q.get(m,0x8c098b60),0);entry=q.AUXILIARY;args=[actor];expected=-3
        elif kind=='empty_record_heap':
            case=run(case,q.INIT,[0,task],'record_failure_original_constructor',0)
            m,s,f,c=case;state=q.get(m,aux);q.put(m,q.get(m,0x8c098b60),0)
            entry=q.RECORDS;args=[state];expected=0
        case=run(case,entry,args,kind,expected)
        if kind=='empty_record_heap':
            assert q.get(case[0],state+0x78)==0 and q.get(case[0],state+0x12,2)==0;checks+=2
        print('PLAYER FRONTIER',kind,records[-1]['status'],records[-1]['stop_pc'],flush=True)
    files=[Path(__file__),P/'player_work_lifecycle_fixtures.py',P/'player_work_allocation_reference.py',P/'write_watch_player_work_allocation.py',q.v.effects.DLL]
    report=dict(status='reached_software_frontiers_match',comparisons=len(records),independent_checks=checks,status_counts=dict(collections.Counter(x['status'] for x in records)),cases=records,
                scope='Original heap/pool/parent creation retained. Six raw PR/SZ/bank modes,two unsupported rounding modes,four invalid mapping inputs,and empty-heap metadata inputs. No returned allocation value or execution frame supplied. Empty free-head is a declared exhaustion-state input,not a natural exhaustion lifecycle. Numeric/mapping stops are existing software-domain behavior,not physical hardware exceptions. No additional routine credit.',
                new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_work_frontier_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
    print('PLAYER FRONTIER PASS',len(records),checks,report['status_counts'],flush=True)
if __name__=='__main__':main()
