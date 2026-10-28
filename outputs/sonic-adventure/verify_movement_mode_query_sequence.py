"""Original parameter stores produce the global consumed by the canonical query."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_movement_mode_query as fast
import movement_parameters_fixtures as producer
v=fast.old;P=v.P;ENTRY,GLOBAL=0x8c049a4e,0x8c7492ac
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(word,mode,view,label);checks+=1
    for word,mode,view in itertools.product((0,1,0x80000000,0xffffffff,producer.bits(4)),(0,4),(0x0c000000,0x8c000000,0xac000000)):
        c=dict(state_address=GLOBAL-0x114,table_words=[dict(index=0,offset=0x78,word=word)],mode=mode,view=view)
        m,s,f,cfg=producer.prepare(ram,c);cfg=v.Input(cfg)
        for entry in (producer.ENTRY,ENTRY):
            cfg['entry']=entry;s.pc=entry;s.pr=0x8cfed000
            r,path,m,s,f=fast.compare(m,s,f,cfg)
            require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'actual producer/query full-state execution matches original')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==producer.STACK,'normal entry/return;no provided execution frame')
            require(producer.get(m,GLOBAL)==word,'actual original parameter store produces and query preserves raw global word')
            if entry==ENTRY:require(s.r[0]==int(word!=0) and (s.sr&1)==int(word==0),'independent bitwise nonzero query including negative zero and NaN words')
            records.append(dict(word=hex(word),mode=mode,view=hex(view),entry=hex(entry),status=0,original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    files=[Path(__file__),P/'movement_parameters_fixtures.py',P/'movement_mode_query_reference.py',P/'write_watch_movement_mode_query.py',v.MP,P/'analysis/movement_mode_query_connection.json',v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,actual_producer_consumer_sequences=30,cases=records,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,canonical_duplicate_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},
      scope='30actual075214->049a4esequences with5rawwords,FPbanks0/1,P0/P1/P2pointerviews. Authoredstate outputinitiallayout overlapsglobal;originalparameterstore producesword consumedbyactualquery. No hostinjected intermediate result/frame. Independent rawwordpredicate distinguishes floatingnegativezero bits fromintegerzero. Not evidence ofnaturalgameplaylayout/full0730d8movement.')
    (P/'analysis/movement_mode_query_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MODE QUERY SEQUENCE PASS',len(records),checks,flush=True)

if __name__=='__main__':main()
