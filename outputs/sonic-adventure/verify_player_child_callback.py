"""Whole initializer/query comparisons, exact table/reset oracles and constant-return gate proof."""
import csv,hashlib,itertools,json,struct
from pathlib import Path
import player_child_callback_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
    for n,(kind,present,mode,rm) in enumerate(itertools.product(range(8),(False,True),(0,4),(0,1))):
        yield dict(kind=kind,present=present,mode=mode,rm=rm,child_index=n%2)
    for mode,rm in itertools.product((0,4),(0,1)):
        yield dict(kind=7,present=False,fallback_kind=0x107,mode=mode,rm=rm,child_index=1)
    for n,(index,present,kind) in enumerate(itertools.product((0,1,255,256,0xffffffff),(False,True),(0x80,0xff))):
        yield dict(entry=q.GETTER,query_index=index,present=present,kind=kind,fallback_kind=0x1234,mode=4*(n%2),rm=(n//2)%2)
def main():
    target=P/'analysis/player_child_callback_verification.json';assert not target.exists()
    ram=q.v.seed_ram();records=[];seen=set();edges=set();checks=oracles=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    for n,c in enumerate(configurations()):
        m,s,f,cfg=q.prepare(ram,c);before=type(s).from_buffer_copy(s)
        before_actor=bytes(m[q.CHILD_ACTOR&0xffffff:(q.CHILD_ACTOR&0xffffff)+0x40])
        table=[q.get(m,q.CHARACTER_TABLE+0xd8*j) for j in range(5)]
        require(table==[2,7,0,3,5],'independent original character table IDs')
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status']==0,(c,result['status'],result['stop_pc']))
        require(result['full_state_match'] and result['ordered_boundaries_match'],'whole state and ordered boundaries')
        require(s.pc==s.pr==before.pr and s.r[15]==before.r[15],'normal return and restored stack')
        require(list(s.r[8:15])==list(before.r[8:15]),'callee saved integer registers')
        expected=c.get('kind',0)&255 if c.get('present',True) else c.get('fallback_kind',c.get('kind',0))&65535
        if cfg['entry']==q.GETTER:
            require(s.r[0]==expected,'independent byte query or unsigned16 fallback');oracles+=1
            fallback=[b for b in result['boundaries'] if b['target']=='0x8c04f698']
            require(bool(fallback)==(not c.get('present',True)),'actual fallback tail call membership')
            require(all(b['pr']==hex(before.pr) for b in fallback),'tail call retains original PR')
        else:
            selected=q.CHARACTER_TABLE+0xd8*table.index(expected) if expected in table else 0
            require([q.get(m,q.TASK+o) for o in (0x10,0x14,0x18)]==[0x8c0b3b1a,0x8c0b3f2c,0x8c0b3afc],'independent three installed callback pointers');oracles+=1
            require(q.get(m,q.CHILD_ACTOR+1,1)==q.get(m,q.CHILD_ACTOR+3,1)==q.get(m,q.CHILD_ACTOR+6,2)==q.get(m,q.CHILD_ACTOR+0x10)==0,'independent original reset fields');oracles+=1
            require(q.get(m,q.CHILD_ACTOR+8)==selected,'independent selected character table pointer');oracles+=1
            after_actor=bytes(m[q.CHILD_ACTOR&0xffffff:(q.CHILD_ACTOR&0xffffff)+0x40])
            written={1,3,6,7,*range(8,12),*range(0x10,0x14)}
            require(all(after_actor[i]==before_actor[i] for i in range(0x40) if i not in written),'other child fields including floating player index stay unchanged');oracles+=1
            calls=[b for b in result['boundaries'] if b['target']=='0x8c07ce40']
            require(len(calls)==(table.index(expected)+1 if expected in table else 5),'independent search loop count')
            require(all(b['arguments'][0]==0 for b in calls),'original initializer queries player0')
            require(0x8c0b3aea in path,'actual ordinary initializer completion')
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        records.append(dict(input=c,status=0,steps=len(path),return_value=s.r[0],selected_character_table=q.get(m,q.CHILD_ACTOR+8) if cfg['entry']!=q.GETTER else None,full_state_match=True,ordered_boundaries_match=True))
        if (n+1)%16==0:print('CHILD CALLBACK',n+1,'comparisons',flush=True)
    t=json.loads((P/'analysis/player_child_callback_translation.json').read_text());coverage=[]
    impossible=('0x8c0b3a5c','0x8c0b3aea')
    for b in t['routines']:
        wanted=set(b['reachable_addresses']);outcomes={tuple(x) for x in b['conditional_edges']}
        missing=wanted-{hex(a) for a in seen};missed=outcomes-edges
        require(not missing,('whole original row coverage',b['address'],sorted(missing)))
        require(missed==({impossible} if b['address']==hex(q.ENTRY) else set()),('feasible outcome coverage',b['address'],sorted(missed)))
        coverage.append(dict(address=b['address'],static_rows=len(wanted),executed_rows=len(wanted),static_outcomes=len(outcomes),executed_outcomes=len(outcomes)-len(missed),proven_infeasible_outcomes=[list(x) for x in sorted(missed)],missing_rows=[],unresolved_outcomes=[]))
    retail=P.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes()
    require(raw[0x8c042dc4-0x8c010000:0x8c042dc4-0x8c010000+4]==bytes.fromhex('0b0000e0'),'original rts/mov-zero constant-return callee')
    require(struct.unpack_from('<I',raw,0x8c0b3acc-0x8c010000)[0]==0x8c042dc4,'original initializer call literal selects constant-zero callee')
    rows={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(t['routines'][0]['listing']).open(),delimiter='\t')}
    require(rows[0x8c0b3a56]=='jsr @r3' and rows[0x8c0b3a5a]=='tst r0,r0' and rows[0x8c0b3a5c]=='bf 0x8c0b3aea','immediate original zero-result gate;no intervening R0 writer')
    files=[Path(__file__),P/'player_child_callback_fixtures.py',P/'player_child_callback_reference.py',P/'write_watch_player_child_callback.py',P/'analysis/player_child_callback_translation.json',P/'analysis/player_child_callback_connection.json',P/'source/retail_return_delay_stubs.c',P/'source/sa_global_u16_get_8c04f698.c',retail,q.v.effects.DLL]
    report=dict(status='complete_initializer_and_query_match;one_original_constant_return_outcome_proven_infeasible',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,cases=records,coverage_by_routine=coverage,
        original_constant_return_gate_proof=dict(callee='0x8c042dc4',original_bytes='0b0000e0',return_value=0,unmodified_caller_test='tst r0,r0',infeasible_edge=list(impossible),scope='Ordinary original call/return only;no callee output is supplied or changed.'),
        supplied_game_callee_outputs=0,supplied_execution_frames=0,native_instructions_removed=0,new_original_routine_credit=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Whole original56-row initializer and12-row player character query. Exact original table IDs,first through fifth match and no match,reset fields,three callback pointers,preserved actor bytes,query loop count,player0 argument,byte-index wrapping,byte IDs,unsigned16fallback and tail PR are checked. Four retained bank/rounding combinations execute. All68rows and7feasible outcomes execute;the remaining branch is proven infeasible from the original constant-zero callee,not forced with a fake result. Stored subsequent callbacks are not executed and retail startup/physics invariants remain open.',sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('CHILD CALLBACK COMPLETE',len(records),checks,oracles,'all68rows/7feasible outcomes;one original constant-zero outcome infeasible',flush=True)
if __name__=='__main__':main()
