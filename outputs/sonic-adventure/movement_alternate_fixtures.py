"""Prepare the original query/contact ABI without running or replacing a game callee."""
import movement_response_fixtures as previous
import write_watch_movement_alternate as fast
v=fast.old;P=v.P
A,B,C,QUERY,OUT,CONTACTS,UP,STACK=(getattr(previous,name) for name in ('A','B','C','QUERY','OUT','CONTACTS','UP','STACK'))
bits,number,put,get,vector=(getattr(previous,name) for name in ('bits','number','put','get','vector'))
ENTRY=0x8c075860

def prepare(ram,c):
    assert c['entry']==ENTRY
    # Reuse only the preceding fixture's RAM/register INPUT construction.
    # No 075416 instructions or game results are executed/supplied here.
    m,s,f,cfg=previous.prepare(ram,dict(c,entry=0x8c075416))
    cfg=v.Input(dict(cfg,entry=ENTRY));s.pc=ENTRY
    for j,word in enumerate(c.get('motion_words',())):put(m,QUERY+0x10+j*4,word)
    return m,s,f,cfg
