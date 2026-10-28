"""Canonical query inputs only: literal pointer, initial RAM word and CPU state."""
import write_watch_movement_mode_query as fast
import collision_contact_fixtures as previous
v=fast.old;P=v.P
ENTRY,SLOT,GLOBAL=0x8c049a4e,0x8c049ae8,0x8c7492ac
put,get=previous.put,previous.get

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(mode=c.get('mode',0),case_id=c.get('case_id',0)))
    cfg=v.Input(dict(base,entry=ENTRY,probe=True));s.pc=ENTRY;s.pr=c.get('pr',0x8cfed000)
    s.sr=(s.sr&~0x8001)|c.get('t',0)|(0x8000 if c.get('fd',False) else 0);cfg['sr']=s.sr
    target=c.get('target',GLOBAL);put(m,SLOT,target)
    if c.get('store_word',True):
        assert target&0x1f000000==0x0c000000 and target&0xffffff<=0xfffffc
        assert not ENTRY<=target<ENTRY+12 and target!=SLOT
        put(m,target,c.get('word',0))
    return m,s,f,cfg
