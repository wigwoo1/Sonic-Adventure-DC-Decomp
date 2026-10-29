"""Ordinary contact-array/query inputs;no supplied game outputs or frames."""
import movement_contact_commit_fixtures as previous
import write_watch_movement_contact_candidates as fast
v=fast.old;P=v.P
A,B,C,Q,D,V,W=(getattr(previous,n) for n in ('A','B','C','Q','D','V','W'))
put,get,bits,number,vector=(getattr(previous,n) for n in ('put','get','bits','number','vector'))
CLAMP,RESPONSE=0x8c075686,0x8c075f8c
CONTACTS=0x8c6bacf0;STRIDE=0x58

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(c,entry=previous.OWNER))
    cfg=v.Input(dict(base,entry=c['entry'],probe=True));s.pc=c['entry'];s.pr=0x8cfed000
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(B),ptr(C),ptr(Q)
    count=c.get('count',1);put(m,Q+0x28,count,2)
    vector(m,Q+0x1c,c.get('query_normal',(0,-1,0) if c['entry']==CLAMP else (0,1,0)))
    vector(m,Q+0x10,c.get('motion',(-3,0,0)))
    vector(m,Q+0x78,c.get('add_delta',(0,0,0)))
    vector(m,Q+0x6c,c.get('delta',(0,0,2)))
    m[CONTACTS&0xffffff:(CONTACTS&0xffffff)+32*STRIDE]=bytes(32*STRIDE)
    specs=c.get('contacts',[{} for _ in range(count)])
    assert len(specs)==count and count<=32
    for j,spec in enumerate(specs):
        a=CONTACTS+j*STRIDE
        vector(m,a+0x14,spec.get('normal',c.get('normal',(1,0,0))))
        vector(m,a+0x20,spec.get('plane_point',c.get('plane_point',(0,0,0))))
        vector(m,a+0x2c,spec.get('point',c.get('contact_point',(1,0,0))))
        for field,offset in (('point_words',0x2c),('normal_words',0x14)):
            for k,word in enumerate(spec.get(field,c.get(field,()))):put(m,a+offset+4*k,word)
    for k,word in enumerate(c.get('motion_words',())):put(m,Q+0x10+4*k,word)
    if 'padding_word' in c:
        # Declared pre-entry caller memory;the original may read its uninitialized
        # min-distance[-1] slot when every eligible distance is at least1000/NaN.
        put(m,s.r[15]-(0x6c if c['entry']==CLAMP else 0xd0),c['padding_word'])
    bad=c.get('bad')
    if bad=='stack':s.r[15]=0x8b000000
    elif bad=='query':s.r[7]=0x8b000000
    elif bad=='state':s.r[6]=0x8b000000
    elif bad=='aux':s.r[5]=0x8b000000
    return m,s,f,cfg
